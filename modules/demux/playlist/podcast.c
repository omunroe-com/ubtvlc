/*****************************************************************************
 * podcast.c : podcast playlist imports
 *****************************************************************************
 * Copyright (C) 2005 the VideoLAN team
 * $Id: podcast.c 16086 2006-07-19 11:53:18Z zorglub $
 *
 * Authors: Antoine Cellerier <dionoea -at- videolan -dot- org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <ctype.h>                                              /* isspace() */

#include <vlc/vlc.h>
#include <vlc/input.h>
#include <vlc/intf.h>

#include "playlist.h"
#include "vlc_xml.h"

struct demux_sys_t
{
    char *psz_prefix;
    playlist_t *p_playlist;
    xml_t *p_xml;
    xml_reader_t *p_xml_reader;
};

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int Demux( demux_t *p_demux);
static int Control( demux_t *p_demux, int i_query, va_list args );

/*****************************************************************************
 * Import_podcast: main import function
 *****************************************************************************/
int E_(Import_podcast)( vlc_object_t *p_this )
{
    demux_t *p_demux = (demux_t *)p_this;

    if( !isDemux( p_demux, "podcast" ) )
        return VLC_EGENERIC;
    
    STANDARD_DEMUX_INIT_MSG( "using podcast reader" );
    p_demux->p_sys->psz_prefix = E_(FindPrefix)( p_demux );
    p_demux->p_sys->p_playlist = NULL;
    p_demux->p_sys->p_xml = NULL;
    p_demux->p_sys->p_xml_reader = NULL;

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Deactivate: frees unused data
 *****************************************************************************/
void E_(Close_podcast)( vlc_object_t *p_this )
{
    demux_t *p_demux = (demux_t *)p_this;
    demux_sys_t *p_sys = p_demux->p_sys;

    if( p_sys->psz_prefix ) free( p_sys->psz_prefix );
    if( p_sys->p_playlist ) vlc_object_release( p_sys->p_playlist );
    if( p_sys->p_xml_reader ) xml_ReaderDelete( p_sys->p_xml, p_sys->p_xml_reader );
    if( p_sys->p_xml ) xml_Delete( p_sys->p_xml );
    free( p_sys );
}

/* "specs" : http://phobos.apple.com/static/iTunesRSS.html */
static int Demux( demux_t *p_demux )
{
    demux_sys_t *p_sys = p_demux->p_sys;

    vlc_bool_t b_item = VLC_FALSE;
    vlc_bool_t b_image = VLC_FALSE;
    int i_ret;

    xml_t *p_xml;
    xml_reader_t *p_xml_reader;
    char *psz_elname = NULL;
    char *psz_item_mrl = NULL;
    char *psz_item_size = NULL;
    char *psz_item_type = NULL;
    char *psz_item_name = NULL;
    char *psz_item_date = NULL;
    char *psz_item_author = NULL;
    char *psz_item_category = NULL;
    char *psz_item_duration = NULL;
    char *psz_item_keywords = NULL;
    char *psz_item_subtitle = NULL;
    char *psz_item_summary = NULL;
    int i_type;

    INIT_PLAYLIST_STUFF;

    p_xml = p_sys->p_xml = xml_Create( p_demux );
    if( !p_xml ) return -1;

/*    psz_elname = stream_ReadLine( p_demux->s );
    if( psz_elname ) free( psz_elname );
    psz_elname = 0;*/

    p_xml_reader = xml_ReaderCreate( p_xml, p_demux->s );
    if( !p_xml_reader ) return -1;
    p_sys->p_xml_reader = p_xml_reader;

    /* xml */
    /* check root node */
    if( xml_ReaderRead( p_xml_reader ) != 1 )
    {
        msg_Err( p_demux, "invalid file (no root node)" );
        return -1;
    }
    if( xml_ReaderNodeType( p_xml_reader ) != XML_READER_STARTELEM ||
        ( psz_elname = xml_ReaderName( p_xml_reader ) ) == NULL ||
        strcmp( psz_elname, "rss" ) )
    {
        msg_Err( p_demux, "invalid root node %i, %s",
                 xml_ReaderNodeType( p_xml_reader ), psz_elname );
        if( psz_elname ) free( psz_elname );
        return -1;
    }
    free( psz_elname ); psz_elname = NULL;

    while( (i_ret = xml_ReaderRead( p_xml_reader )) == 1 )
    {
        // Get the node type
        i_type = xml_ReaderNodeType( p_xml_reader );
        switch( i_type )
        {
            // Error
            case -1:
                return -1;
                break;

            case XML_READER_STARTELEM:
            {
                // Read the element name
                if( psz_elname ) free( psz_elname );
                psz_elname = xml_ReaderName( p_xml_reader );
                if( !psz_elname ) return -1;

                if( !strcmp( psz_elname, "item" ) )
                {
                    b_item = VLC_TRUE;
                }
                else if( !strcmp( psz_elname, "image" ) )
                {
                    b_item = VLC_TRUE;
                }

                // Read the attributes
                while( xml_ReaderNextAttr( p_xml_reader ) == VLC_SUCCESS )
                {
                    char *psz_name = xml_ReaderName( p_xml_reader );
                    char *psz_value = xml_ReaderValue( p_xml_reader );
                    if( !psz_name || !psz_value ) return -1;
                    if( !strcmp( psz_elname, "enclosure" ) &&
                        !strcmp( psz_name, "url" ) )
                    {
                        psz_item_mrl = strdup( psz_value );
                    }
                    else if( !strcmp( psz_elname, "enclosure" ) &&
                        !strcmp( psz_name, "length" ) )
                    {
                        psz_item_size = strdup( psz_value );
                    }
                    else if( !strcmp( psz_elname, "enclosure" ) &&
                        !strcmp( psz_name, "type" ) )
                    {
                        psz_item_type = strdup( psz_value );
                    }
                    else
                    {
                        msg_Dbg( p_demux,"unhandled attribure %s in element %s",
                                  psz_name, psz_elname );
                    }
                    free( psz_name );
                    free( psz_value );
                }
                break;
            }
            case XML_READER_TEXT:
            {
#define SET_DATA( field, name ) else if( b_item == VLC_TRUE \
                && !strcmp( psz_elname, name ) ) \
                { \
                    field = strdup( psz_text ); \
                }
                char *psz_text = xml_ReaderValue( p_xml_reader );
                /* item specific meta data */
                if( b_item == VLC_TRUE && !strcmp( psz_elname, "title" ) )
                {
                    psz_item_name = strdup( psz_text );
                }
                else if( b_item == VLC_TRUE
                         && ( !strcmp( psz_elname, "itunes:author" )
                            ||!strcmp( psz_elname, "author" ) ) )
                { /* <author> isn't standard iTunes podcast stuff */
                    psz_item_author = strdup( psz_text );
                }
                else if( b_item == VLC_TRUE
                         && ( !strcmp( psz_elname, "itunes:summary" )
                            ||!strcmp( psz_elname, "description" ) ) )
                { /* <description> isn't standard iTunes podcast stuff */
                    psz_item_summary = strdup( psz_text );
                }
                SET_DATA( psz_item_date, "pubDate" )
                SET_DATA( psz_item_category, "itunes:category" ) 
                SET_DATA( psz_item_duration, "itunes:duration" )
                SET_DATA( psz_item_keywords, "itunes:keywords" )
                SET_DATA( psz_item_subtitle, "itunes:subtitle" )
                /* toplevel meta data */
                else if( b_item == VLC_FALSE && b_image == VLC_FALSE
                         && !strcmp( psz_elname, "title" ) )
                {
                    playlist_ItemSetName( p_current, psz_text );
                }
#define ADD_GINFO( info, name ) \
    else if( !b_item && !b_image && !strcmp( psz_elname, name ) ) \
    { \
        vlc_input_item_AddInfo( p_current->p_input, _("Podcast Info"), \
                                _( info ), "%s", psz_text ); \
    }
                ADD_GINFO( "Podcast Link", "link" )
                ADD_GINFO( "Podcast Copyright", "copyright" )
                ADD_GINFO( "Podcast Category", "itunes:category" )
                ADD_GINFO( "Podcast Keywords", "itunes:keywords" )
                ADD_GINFO( "Podcast Subtitle", "itunes:subtitle" )
#undef ADD_GINFO
                else if( b_item == VLC_FALSE && b_image == VLC_FALSE
                         && ( !strcmp( psz_elname, "itunes:summary" )
                            ||!strcmp( psz_elname, "description" ) ) )
                { /* <description> isn't standard iTunes podcast stuff */
                    vlc_input_item_AddInfo( p_current->p_input,
                             _( "Podcast Info" ), _( "Podcast Summary" ),
                             "%s", psz_text );
                }
                else
                {
                    msg_Dbg( p_demux, "unhandled text in element '%s'",
                              psz_elname );
                }
                free( psz_text );
                break;
            }
            // End element
            case XML_READER_ENDELEM:
            {
                // Read the element name
                free( psz_elname );
                psz_elname = xml_ReaderName( p_xml_reader );
                if( !psz_elname ) return -1;
                if( !strcmp( psz_elname, "item" ) )
                {
                    p_input = input_ItemNewExt( p_playlist, psz_item_mrl,
                                                psz_item_name, 0, NULL, -1 );
                    if( p_input == NULL ) break;
#define ADD_INFO( info, field ) \
    if( field ) { vlc_input_item_AddInfo( p_input, \
                            _( "Podcast Info" ),  _( info ), "%s", field ); }
                    ADD_INFO( "Podcast Publication Date", psz_item_date  );
                    ADD_INFO( "Podcast Author", psz_item_author );
                    ADD_INFO( "Podcast Subcategory", psz_item_category );
                    ADD_INFO( "Podcast Duration", psz_item_duration );
                    ADD_INFO( "Podcast Keywords", psz_item_keywords );
                    ADD_INFO( "Podcast Subtitle", psz_item_subtitle );
                    ADD_INFO( "Podcast Summary", psz_item_summary );
                    ADD_INFO( "Podcast Type", psz_item_type );
                    if( psz_item_size )
                    {
                        vlc_input_item_AddInfo( p_input,
                                                _( "Podcast Info" ),
                                                _( "Podcast Size" ),
                                                "%s bytes",
                                                psz_item_size );
                    }
                    playlist_AddWhereverNeeded( p_playlist, p_input, p_current,
                          p_item_in_category, (i_parent_id > 0 ) ? VLC_TRUE:
                                                VLC_FALSE, PLAYLIST_APPEND );
                    FREENULL( psz_item_name );
                    FREENULL( psz_item_mrl );
                    FREENULL( psz_item_size );
                    FREENULL( psz_item_type );
                    FREENULL( psz_item_date );
                    FREENULL( psz_item_author );
                    FREENULL( psz_item_category );
                    FREENULL( psz_item_duration );
                    FREENULL( psz_item_keywords );
                    FREENULL( psz_item_subtitle );
                    FREENULL( psz_item_summary );
                    b_item = VLC_FALSE;
                }
                else if( !strcmp( psz_elname, "image" ) )
                {
                    b_image = VLC_FALSE;
                }
                free( psz_elname );
                psz_elname = strdup("");

                break;
            }
        }
    }

    if( i_ret != 0 )
    {
        msg_Warn( p_demux, "error while parsing data" );
    }

    HANDLE_PLAY_AND_RELEASE;
    return VLC_SUCCESS;
}

static int Control( demux_t *p_demux, int i_query, va_list args )
{
    return VLC_EGENERIC;
}
