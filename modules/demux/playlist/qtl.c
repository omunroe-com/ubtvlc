/*****************************************************************************
 * qtl.c: QuickTime Media Link Importer
 *****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 * $Id: qtl.c 16203 2006-08-03 15:34:08Z zorglub $
 *
 * Authors: Antoine Cellerier <dionoea -@t- videolan -Dot- org>
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

/*
See
http://developer.apple.com/documentation/QuickTime/QT6WhatsNew/Chap1/chapter_1_section_54.html
and
http://developer.apple.com/documentation/QuickTime/WhatsNewQT5/QT5NewChapt1/chapter_1_section_39.html

autoplay - true/false
controller - true/false
fullscreen - normal/double/half/current/full
href - url
kioskmode - true/false
loop - true/false/palindrome
movieid - integer
moviename - string
playeveryframe - true/false
qtnext - url
quitwhendone - true/false
src - url (required)
type - mime type
volume - 0 (mute) - 100 (max)

*/

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
    playlist_t *p_playlist;
    playlist_item_t *p_current;
    playlist_item_t *p_item_in_category;
    int i_parent_id;

    xml_t *p_xml;
    xml_reader_t *p_xml_reader;
};

typedef enum { FULLSCREEN_NORMAL,
               FULLSCREEN_DOUBLE,
               FULLSCREEN_HALF,
               FULLSCREEN_CURRENT,
               FULLSCREEN_FULL } qtl_fullscreen_t;
char* ppsz_fullscreen[] = { "normal", "double", "half", "current", "full" };
typedef enum { LOOP_TRUE,
               LOOP_FALSE,
               LOOP_PALINDROME } qtl_loop_t;
char* ppsz_loop[] = { "true", "false", "palindrome" };

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int Demux( demux_t *p_demux);
static int Control( demux_t *p_demux, int i_query, va_list args );

/*****************************************************************************
 * Import_QTL: main import function
 *****************************************************************************/
int E_(Import_QTL)( vlc_object_t *p_this )
{
    DEMUX_BY_EXTENSION_MSG( ".qtl", "using QuickTime Media Link reader" );
    p_demux->p_sys->p_playlist = NULL;
    p_demux->p_sys->p_xml = NULL;
    p_demux->p_sys->p_xml_reader = NULL;
    return VLC_SUCCESS;
}

/*****************************************************************************
 * Deactivate: frees unused data
 *****************************************************************************/
void E_(Close_QTL)( vlc_object_t *p_this )
{
    demux_t *p_demux = (demux_t *)p_this;
    demux_sys_t *p_sys = p_demux->p_sys;

    if( p_sys->p_playlist )
        vlc_object_release( p_sys->p_playlist );
    if( p_sys->p_xml_reader )
        xml_ReaderDelete( p_sys->p_xml, p_sys->p_xml_reader );
    if( p_sys->p_xml )
        xml_Delete( p_sys->p_xml );
    free( p_sys );
}

static int Demux( demux_t *p_demux )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    xml_t *p_xml;
    xml_reader_t *p_xml_reader;
    char *psz_eltname = NULL;

    /* List of all possible attributes. The only required one is "src" */
    vlc_bool_t b_autoplay = VLC_FALSE;
    vlc_bool_t b_controler = VLC_TRUE;
    qtl_fullscreen_t fullscreen = VLC_FALSE;
    char *psz_href = NULL;
    vlc_bool_t b_kioskmode = VLC_FALSE;
    qtl_loop_t loop = LOOP_FALSE;
    int i_movieid = -1;
    char *psz_moviename = NULL;
    vlc_bool_t b_playeveryframe = VLC_FALSE;
    char *psz_qtnext = NULL;
    vlc_bool_t b_quitwhendone = VLC_FALSE;
    char *psz_src = NULL;
    char *psz_mimetype = NULL;
    int i_volume = 100;

    INIT_PLAYLIST_STUFF;

    p_sys->p_playlist = p_playlist;
    p_sys->p_current = p_current;
    p_sys->i_parent_id = i_parent_id;
    p_sys->p_item_in_category = p_item_in_category;

    p_xml = p_sys->p_xml = xml_Create( p_demux );
    if( !p_xml ) return -1;

    p_xml_reader = xml_ReaderCreate( p_xml, p_demux->s );
    if( !p_xml_reader ) return -1;
    p_sys->p_xml_reader = p_xml_reader;

    /* check root node */
    if( xml_ReaderRead( p_xml_reader ) != 1 )
    {
        msg_Err( p_demux, "invalid file (no root node)" );
        return -1;
    }

    if( xml_ReaderNodeType( p_xml_reader ) != XML_READER_STARTELEM ||
        ( psz_eltname = xml_ReaderName( p_xml_reader ) ) == NULL ||
        strcmp( psz_eltname, "embed" ) )
    {
        msg_Err( p_demux, "invalid root node %i, %s",
                 xml_ReaderNodeType( p_xml_reader ), psz_eltname );
        FREE( psz_eltname );

        /* second line has <?quicktime tag ... so we try to skip it */
        msg_Dbg( p_demux, "trying to read one more node" );
        xml_ReaderRead( p_xml_reader );
        if( xml_ReaderNodeType( p_xml_reader ) != XML_READER_STARTELEM ||
            ( psz_eltname = xml_ReaderName( p_xml_reader ) ) == NULL ||
            strcmp( psz_eltname, "embed" ) )
        {
            msg_Err( p_demux, "invalid root node %i, %s",
                     xml_ReaderNodeType( p_xml_reader ), psz_eltname );
            FREE( psz_eltname );
            return -1;
        }
    }
    FREE( psz_eltname );

    while( xml_ReaderNextAttr( p_sys->p_xml_reader ) == VLC_SUCCESS )
    {
        char *psz_attrname = xml_ReaderName( p_sys->p_xml_reader );
        char *psz_attrvalue = xml_ReaderValue( p_sys->p_xml_reader );

        if( !psz_attrname || !psz_attrvalue )
        {
            FREE( psz_attrname );
            FREE( psz_attrvalue );
            return -1;
        }

        if( !strcmp( psz_attrname, "autoplay" ) )
        {
            if( !strcmp( psz_attrvalue, "true" ) )
            {
                b_autoplay = VLC_TRUE;
            }
            else
            {
                b_autoplay = VLC_FALSE;
            }
        }
        else if( !strcmp( psz_attrname, "controler" ) )
        {
            if( !strcmp( psz_attrvalue, "false" ) )
            {
                b_controler = VLC_FALSE;
            }
            else
            {
                b_controler = VLC_TRUE;
            }
        }
        else if( !strcmp( psz_attrname, "fullscreen" ) )
        {
            if( !strcmp( psz_attrvalue, "double" ) )
            {
                fullscreen = FULLSCREEN_DOUBLE;
            }
            else if( !strcmp( psz_attrvalue, "half" ) )
            {
                fullscreen = FULLSCREEN_HALF;
            }
            else if( !strcmp( psz_attrvalue, "current" ) )
            {
                fullscreen = FULLSCREEN_CURRENT;
            }
            else if( !strcmp( psz_attrvalue, "full" ) )
            {
                fullscreen = FULLSCREEN_FULL;
            }
            else
            {
                fullscreen = FULLSCREEN_NORMAL;
            }
        }
        else if( !strcmp( psz_attrname, "href" ) )
        {
            psz_href = psz_attrvalue;
            psz_attrvalue = NULL;
        }
        else if( !strcmp( psz_attrname, "kioskmode" ) )
        {
            if( !strcmp( psz_attrvalue, "true" ) )
            {
                b_kioskmode = VLC_TRUE;
            }
            else
            {
                b_kioskmode = VLC_FALSE;
            }
        }
        else if( !strcmp( psz_attrname, "loop" ) )
        {
            if( !strcmp( psz_attrvalue, "true" ) )
            {
                loop = LOOP_TRUE;
            }
            else if( !strcmp( psz_attrvalue, "palindrome" ) )
            {
                loop = LOOP_PALINDROME;
            }
            else
            {
                loop = LOOP_FALSE;
            }
        }
        else if( !strcmp( psz_attrname, "movieid" ) )
        {
            i_movieid = atoi( psz_attrvalue );
        }
        else if( !strcmp( psz_attrname, "moviename" ) )
        {
            psz_moviename = psz_attrvalue;
            psz_attrvalue = NULL;
        }
        else if( !strcmp( psz_attrname, "playeveryframe" ) )
        {
            if( !strcmp( psz_attrvalue, "true" ) )
            {
                b_playeveryframe = VLC_TRUE;
            }
            else
            {
                b_playeveryframe = VLC_FALSE;
            }
        }
        else if( !strcmp( psz_attrname, "qtnext" ) )
        {
            psz_qtnext = psz_attrvalue;
            psz_attrvalue = NULL;
        }
        else if( !strcmp( psz_attrname, "quitwhendone" ) )
        {
            if( !strcmp( psz_attrvalue, "true" ) )
            {
                b_quitwhendone = VLC_TRUE;
            }
            else
            {
                b_quitwhendone = VLC_FALSE;
            }
        }
        else if( !strcmp( psz_attrname, "src" ) )
        {
            psz_src = psz_attrvalue;
            psz_attrvalue = NULL;
        }
        else if( !strcmp( psz_attrname, "mimetype" ) )
        {
            psz_mimetype = psz_attrvalue;
            psz_attrvalue = NULL;
        }
        else if( !strcmp( psz_attrname, "volume" ) )
        {
            i_volume = atoi( psz_attrvalue );
        }
        else
        {
            msg_Dbg( p_demux, "Attribute %s with value %s isn't valid",
                     psz_attrname, psz_attrvalue );
        }
        FREE( psz_attrname );
        FREE( psz_attrvalue );
    }

    msg_Dbg( p_demux, "autoplay: %s (unused by VLC)",
             b_autoplay==VLC_TRUE ? "true": "false" );
    msg_Dbg( p_demux, "controler: %s (unused by VLC)",
             b_controler==VLC_TRUE?"true": "false" );
    msg_Dbg( p_demux, "fullscreen: %s (unused by VLC)",
             ppsz_fullscreen[fullscreen] );
    msg_Dbg( p_demux, "href: %s", psz_href );
    msg_Dbg( p_demux, "kioskmode: %s (unused by VLC)",
             b_kioskmode==VLC_TRUE?"true":"false" );
    msg_Dbg( p_demux, "loop: %s (unused by VLC)", ppsz_loop[loop] );
    msg_Dbg( p_demux, "movieid: %d (unused by VLC)", i_movieid );
    msg_Dbg( p_demux, "moviename: %s", psz_moviename );
    msg_Dbg( p_demux, "playeverframe: %s (unused by VLC)",
             b_playeveryframe==VLC_TRUE?"true":"false" );
    msg_Dbg( p_demux, "qtnext: %s", psz_qtnext );
    msg_Dbg( p_demux, "quitwhendone: %s (unused by VLC)",
             b_quitwhendone==VLC_TRUE?"true":"false" );
    msg_Dbg( p_demux, "src: %s", psz_src );
    msg_Dbg( p_demux, "mimetype: %s", psz_mimetype );
    msg_Dbg( p_demux, "volume: %d (unused by VLC)", i_volume );


    if( !psz_src )
    {
        msg_Err( p_demux, "Mandatory attribute 'src' not found" );
    }
    else
    {
        p_input = input_ItemNewExt( p_sys->p_playlist,
                                psz_src, psz_moviename, 0, NULL, -1 );
#define SADD_INFO( type, field ) if( field ) { vlc_input_item_AddInfo( \
                    p_input, "QuickTime Media Link", _(type), "%s", field ) ; }
        SADD_INFO( "href", psz_href );
        SADD_INFO( "mime type", psz_mimetype );
        playlist_AddWhereverNeeded( p_sys->p_playlist, p_input,
                            p_sys->p_current, p_sys->p_item_in_category,
                            (p_sys->i_parent_id > 0 ) ? VLC_TRUE: VLC_FALSE,
                            PLAYLIST_APPEND );

        if( psz_qtnext )
        {
            p_input = input_ItemNewExt( p_sys->p_playlist,
                                        psz_qtnext, psz_qtnext, 0, NULL, -1 );
            playlist_AddWhereverNeeded( p_sys->p_playlist, p_input,
                            p_sys->p_current, p_sys->p_item_in_category,
                            (p_sys->i_parent_id > 0 ) ? VLC_TRUE: VLC_FALSE,
                            PLAYLIST_APPEND );
        }
    }

    HANDLE_PLAY_AND_RELEASE;

    p_sys->p_playlist = NULL;

    FREE( psz_href );
    FREE( psz_moviename );
    FREE( psz_qtnext );
    FREE( psz_src );
    FREE( psz_mimetype );

    return VLC_SUCCESS;
}

static int Control( demux_t *p_demux, int i_query, va_list args )
{
    return VLC_EGENERIC;
}
