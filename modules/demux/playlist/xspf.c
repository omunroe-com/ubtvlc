/*******************************************************************************
 * xspf.c : XSPF playlist import functions
 *******************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 * $Id: xspf.c 16071 2006-07-18 17:08:18Z zorglub $
 *
 * Authors: Daniel Stränger <vlc at schmaller dot de>
 *          Yoann Peronneau <yoann@videolan.org>
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
 *******************************************************************************/
/**
 * \file modules/demux/playlist/xspf.c
 * \brief XSPF playlist import functions
 */

#include <vlc/vlc.h>
#include <vlc/input.h>
#include <vlc/intf.h>

#include "playlist.h"
#include "vlc_xml.h"
#include "vlc_strings.h"
#include "xspf.h"

struct demux_sys_t
{
    playlist_item_t *p_item_in_category;
    int i_parent_id;
    input_item_t **pp_tracklist;
    int i_tracklist_entries;
    int i_identifier;
};

static int Control( demux_t *, int, va_list );
static int Demux( demux_t * );

/**
 * \brief XSPF submodule initialization function
 */
int E_(xspf_import_Activate)( vlc_object_t *p_this )
{
    DEMUX_BY_EXTENSION_OR_FORCED_MSG( ".xspf", "xspf-open", 
                                      "using XSPF playlist reader" );
    return VLC_SUCCESS;
}

/**
 * \brief demuxer function for XSPF parsing
 */
int Demux( demux_t *p_demux )
{
    int i_ret = VLC_SUCCESS;
    xml_t *p_xml = NULL;
    xml_reader_t *p_xml_reader = NULL;
    char *psz_name = NULL;
    INIT_PLAYLIST_STUFF;
    p_demux->p_sys->p_item_in_category = p_item_in_category;
    p_demux->p_sys->i_parent_id = i_parent_id;
    p_demux->p_sys->pp_tracklist = NULL;
    p_demux->p_sys->i_tracklist_entries = 0;
    p_demux->p_sys->i_identifier = -1;

    /* create new xml parser from stream */
    p_xml = xml_Create( p_demux );
    if( !p_xml )
        i_ret = VLC_ENOMOD;
    else
    {
        p_xml_reader = xml_ReaderCreate( p_xml, p_demux->s );
        if( !p_xml_reader )
            i_ret = VLC_EGENERIC;
    }

    /* locating the root node */
    if( i_ret == VLC_SUCCESS )
    {
        do
        {
            if( xml_ReaderRead( p_xml_reader ) != 1 )
            {
                msg_Err( p_demux, "can't read xml stream" );
                i_ret = VLC_EGENERIC;
            }
        } while( i_ret == VLC_SUCCESS &&
                 xml_ReaderNodeType( p_xml_reader ) != XML_READER_STARTELEM );
    }
    /* checking root node name */
    if( i_ret == VLC_SUCCESS )
    {
        psz_name = xml_ReaderName( p_xml_reader );
        if( !psz_name || strcmp( psz_name, "playlist" ) )
        {
            msg_Err( p_demux, "invalid root node name: %s", psz_name );
            i_ret = VLC_EGENERIC;
        }
        FREE_NAME();
    }

    i_ret = parse_playlist_node( p_demux, p_playlist, p_current, NULL,
                                 p_xml_reader, "playlist" );
    HANDLE_PLAY_AND_RELEASE;
    if( p_xml_reader )
        xml_ReaderDelete( p_xml, p_xml_reader );
    if( p_xml )
        xml_Delete( p_xml );

    return i_ret;
    return 0;
}

/** \brief dummy function for demux callback interface */
static int Control( demux_t *p_demux, int i_query, va_list args )
{
    return VLC_EGENERIC;
}

/**
 * \brief parse the root node of a XSPF playlist
 * \param p_demux demuxer instance
 * \param p_playlist playlist instance
 * \param p_item current playlist item
 * \param p_input current input item
 * \param p_xml_reader xml reader instance
 * \param psz_element name of element to parse
 */
static vlc_bool_t parse_playlist_node COMPLEX_INTERFACE
{
    char *psz_name=NULL;
    char *psz_value=NULL;
    vlc_bool_t b_version_found = VLC_FALSE;
    int i_node;
    xml_elem_hnd_t *p_handler=NULL;

    xml_elem_hnd_t pl_elements[] =
        { {"title",        SIMPLE_CONTENT,  {.smpl = set_item_info} },
          {"creator",      SIMPLE_CONTENT,  {.smpl = set_item_info} },
          {"annotation",   SIMPLE_CONTENT,  {NULL} },
          {"info",         SIMPLE_CONTENT,  {NULL} },
          {"location",     SIMPLE_CONTENT,  {NULL} },
          {"identifier",   SIMPLE_CONTENT,  {NULL} },
          {"image",        SIMPLE_CONTENT,  {NULL} },
          {"date",         SIMPLE_CONTENT,  {NULL} },
          {"license",      SIMPLE_CONTENT,  {NULL} },
          {"attribution",  COMPLEX_CONTENT, {.cmplx = skip_element} },
          {"link",         SIMPLE_CONTENT,  {NULL} },
          {"meta",         SIMPLE_CONTENT,  {NULL} },
          {"extension",    COMPLEX_CONTENT, {.cmplx = parse_extension_node} },
          {"trackList",    COMPLEX_CONTENT, {.cmplx = parse_tracklist_node} },
          {NULL,           UNKNOWN_CONTENT, {NULL} }
        };

    /* read all playlist attributes */
    while( xml_ReaderNextAttr( p_xml_reader ) == VLC_SUCCESS )
    {
        psz_name = xml_ReaderName( p_xml_reader );
        psz_value = xml_ReaderValue( p_xml_reader );
        if( !psz_name || !psz_value )
        {
            msg_Err( p_demux, "invalid xml stream @ <playlist>" );
            FREE_ATT();
            return VLC_FALSE;
        }
        /* attribute: version */
        if( !strcmp( psz_name, "version" ) )
       {
            b_version_found = VLC_TRUE;
            if( strcmp( psz_value, "0" ) && strcmp( psz_value, "1" ) )
                msg_Warn( p_demux, "unsupported XSPF version" );
        }
        /* attribute: xmlns */
        else if( !strcmp( psz_name, "xmlns" ) )
            ;
        /* unknown attribute */
        else
            msg_Warn( p_demux, "invalid <playlist> attribute:\"%s\"", psz_name);

        FREE_ATT();
    }
    /* attribute version is mandatory !!! */
    if( !b_version_found )
        msg_Warn( p_demux, "<playlist> requires \"version\" attribute" );

    /* parse the child elements - we only take care of <trackList> */
    while( xml_ReaderRead( p_xml_reader ) == 1 )
    {
        i_node = xml_ReaderNodeType( p_xml_reader );
        switch( i_node )
        {
            case XML_READER_NONE:
                break;
            case XML_READER_STARTELEM:
                /*  element start tag  */
                psz_name = xml_ReaderName( p_xml_reader );
                if( !psz_name || !*psz_name )
                {
                    msg_Err( p_demux, "invalid xml stream" );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                /* choose handler */
                for( p_handler = pl_elements;
                     p_handler->name && strcmp( psz_name, p_handler->name );
                     p_handler++ );
                if( !p_handler->name )
                {
                    msg_Err( p_demux, "unexpected element <%s>", psz_name );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                FREE_NAME();
                /* complex content is parsed in a separate function */
                if( p_handler->type == COMPLEX_CONTENT )
                {
                    if( p_handler->pf_handler.cmplx( p_demux,
                                                     p_playlist,
                                                     p_item,NULL,
                                                     p_xml_reader,
                                                     p_handler->name ) )
                    {
                        p_handler = NULL;
                        FREE_ATT();
                    }
                    else
                    {
                        FREE_ATT();
                        return VLC_FALSE;
                    }
                }
                break;

            case XML_READER_TEXT:
                /* simple element content */
                FREE_ATT();
                psz_value = xml_ReaderValue( p_xml_reader );
                if( !psz_value )
                {
                    msg_Err( p_demux, "invalid xml stream" );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                break;

            case XML_READER_ENDELEM:
                /* element end tag */
                psz_name = xml_ReaderName( p_xml_reader );
                if( !psz_name )
                {
                    msg_Err( p_demux, "invalid xml stream" );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                /* leave if the current parent node <playlist> is terminated */
                if( !strcmp( psz_name, psz_element ) )
                {
                    FREE_ATT();
                    return VLC_TRUE;
                }
                /* there MUST have been a start tag for that element name */
                if( !p_handler || !p_handler->name
                    || strcmp( p_handler->name, psz_name ))
                {
                    msg_Err( p_demux, "there's no open element left for <%s>",
                             psz_name );
                    FREE_ATT();
                    return VLC_FALSE;
                }

                if( p_handler->pf_handler.smpl )
                {
                    p_handler->pf_handler.smpl( p_item, NULL, p_handler->name,
                                                psz_value );
                }
                FREE_ATT();
                p_handler = NULL;
                break;

            default:
                /* unknown/unexpected xml node */
                msg_Err( p_demux, "unexpected xml node %i", i_node );
                FREE_ATT();
                return VLC_FALSE;
        }
        FREE_NAME();
    }
    return VLC_FALSE;
}

/**
 * \brief parses the tracklist node which only may contain <track>s
 */
static vlc_bool_t parse_tracklist_node COMPLEX_INTERFACE
{
    char *psz_name=NULL;
    int i_node;
    int i_ntracks = 0;

    /* now parse the <track>s */
    while( xml_ReaderRead( p_xml_reader ) == 1 )
    {
        i_node = xml_ReaderNodeType( p_xml_reader );
        if( i_node == XML_READER_STARTELEM )
        {
            psz_name = xml_ReaderName( p_xml_reader );
            if( !psz_name )
            {
                msg_Err( p_demux, "unexpected end of xml data" );
                FREE_NAME();
                return VLC_FALSE;
            }
            if( strcmp( psz_name, "track") )
            {
                msg_Err( p_demux, "unexpected child of <trackList>: <%s>",
                         psz_name );
                FREE_NAME();
                return VLC_FALSE;
            }
            FREE_NAME();

            /* parse the track data in a separate function */
            if( parse_track_node( p_demux, p_playlist, p_item, NULL,
                                   p_xml_reader,"track" ) == VLC_TRUE )
                i_ntracks++;
        }
        else if( i_node == XML_READER_ENDELEM )
            break;
    }

    /* the <trackList> has to be terminated */
    if( xml_ReaderNodeType( p_xml_reader ) != XML_READER_ENDELEM )
    {
        msg_Err( p_demux, "there's a missing </trackList>" );
        FREE_NAME();
        return VLC_FALSE;
    }
    psz_name = xml_ReaderName( p_xml_reader );
    if( !psz_name || strcmp( psz_name, "trackList" ) )
    {
        msg_Err( p_demux, "expected: </trackList>, found: </%s>", psz_name );
        FREE_NAME();
        return VLC_FALSE;
    }
    FREE_NAME();

    msg_Dbg( p_demux, "parsed %i tracks successfully", i_ntracks );

    return VLC_TRUE;
}

/**
 * \brief parse one track element
 * \param COMPLEX_INTERFACE
 */
static vlc_bool_t parse_track_node COMPLEX_INTERFACE
{
    input_item_t *p_new_input = NULL;
    int i_node;
    char *psz_name=NULL;
    char *psz_value=NULL;
    xml_elem_hnd_t *p_handler=NULL;

    xml_elem_hnd_t track_elements[] =
        { {"location",     SIMPLE_CONTENT,  {NULL} },
          {"identifier",   SIMPLE_CONTENT,  {NULL} },
          {"title",        SIMPLE_CONTENT,  {.smpl = set_item_info} },
          {"creator",      SIMPLE_CONTENT,  {.smpl = set_item_info} },
          {"annotation",   SIMPLE_CONTENT,  {NULL} },
          {"info",         SIMPLE_CONTENT,  {NULL} },
          {"image",        SIMPLE_CONTENT,  {NULL} },
          {"album",        SIMPLE_CONTENT,  {.smpl = set_item_info} },
          {"trackNum",     SIMPLE_CONTENT,  {.smpl = set_item_info} },
          {"duration",     SIMPLE_CONTENT,  {.smpl = set_item_info} },
          {"link",         SIMPLE_CONTENT,  {NULL} },
          {"meta",         SIMPLE_CONTENT,  {NULL} },
          {"extension",    COMPLEX_CONTENT, {.cmplx = skip_element} },
          {NULL,           UNKNOWN_CONTENT, {NULL} }
        };

    while( xml_ReaderRead( p_xml_reader ) == 1 )
    {
        i_node = xml_ReaderNodeType( p_xml_reader );
        switch( i_node )
        {
            case XML_READER_NONE:
                break;

            case XML_READER_STARTELEM:
                /*  element start tag  */
                psz_name = xml_ReaderName( p_xml_reader );
                if( !psz_name || !*psz_name )
                {
                    msg_Err( p_demux, "invalid xml stream" );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                /* choose handler */
                for( p_handler = track_elements;
                     p_handler->name && strcmp( psz_name, p_handler->name );
                     p_handler++ );
                if( !p_handler->name )
                {
                    msg_Err( p_demux, "unexpected element <%s>", psz_name );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                FREE_NAME();
                /* complex content is parsed in a separate function */
                if( p_handler->type == COMPLEX_CONTENT )
                {
                    if( !p_new_input )
                    {
                        msg_Err( p_demux,
                                 "at <%s> level no new item has been allocated",
                                 p_handler->name );
                        FREE_ATT();
                        return VLC_FALSE;
                    }
                    if( p_handler->pf_handler.cmplx( p_demux,
                                                     p_playlist,
                                                     NULL, p_new_input,
                                                     p_xml_reader,
                                                     p_handler->name ) )
                    {
                        p_handler = NULL;
                        FREE_ATT();
                    }
                    else
                    {
                        FREE_ATT();
                        return VLC_FALSE;
                    }
                }
                break;

            case XML_READER_TEXT:
                /* simple element content */
                FREE_ATT();
                psz_value = xml_ReaderValue( p_xml_reader );
                if( !psz_value )
                {
                    msg_Err( p_demux, "invalid xml stream" );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                break;

            case XML_READER_ENDELEM:
                /* element end tag */
                psz_name = xml_ReaderName( p_xml_reader );
                if( !psz_name )
                {
                    msg_Err( p_demux, "invalid xml stream" );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                /* leave if the current parent node <track> is terminated */
                if( !strcmp( psz_name, psz_element ) )
                {
                    FREE_ATT();
                    /* Add it */
                    playlist_AddWhereverNeeded( p_playlist, p_new_input,
                              p_item, p_demux->p_sys->p_item_in_category,
                              (p_demux->p_sys->i_parent_id >0 ) ? VLC_TRUE:
                              VLC_FALSE, PLAYLIST_APPEND );
                    if( p_demux->p_sys->i_identifier <
                        p_demux->p_sys->i_tracklist_entries )
                    {
                        p_demux->p_sys->pp_tracklist[
                            p_demux->p_sys->i_identifier ] = p_new_input;
                    }
                    else
                    {
                        if( p_demux->p_sys->i_identifier >
                            p_demux->p_sys->i_tracklist_entries )
                        {
                            p_demux->p_sys->i_tracklist_entries =
                                p_demux->p_sys->i_identifier;
                        }
                        INSERT_ELEM( p_demux->p_sys->pp_tracklist,
                                     p_demux->p_sys->i_tracklist_entries,
                                     p_demux->p_sys->i_tracklist_entries,
                                     p_new_input );
                    }
                    return VLC_TRUE;
                }
                /* there MUST have been a start tag for that element name */
                if( !p_handler || !p_handler->name
                    || strcmp( p_handler->name, psz_name ))
                {
                    msg_Err( p_demux, "there's no open element left for <%s>",
                             psz_name );
                    FREE_ATT();
                    return VLC_FALSE;
                }

                /* special case: location */
                if( !strcmp( p_handler->name, "location" ) )
                {
                    char *psz_uri=NULL;
                    /* there MUST NOT be an item */
                    if( p_new_input )
                    {
                        msg_Err( p_demux, "item <%s> already created",
                                 psz_name );
                        FREE_ATT();
                        return VLC_FALSE;
                    }
                    psz_uri = unescape_URI_duplicate( psz_value );

                    if( psz_uri )
                    {
                        p_new_input = input_ItemNewExt( p_playlist, psz_uri,
                                                        NULL, 0, NULL, -1 );
                        p_new_input->p_meta = vlc_meta_New();
                        free( psz_uri );
                        vlc_input_item_CopyOptions( p_item->p_input, p_new_input );
                        psz_uri = NULL;
                        FREE_ATT();
                        p_handler = NULL;
                    }
                    else
                    {
                        FREE_ATT();
                        return VLC_FALSE;
                    }
                }
                else if( !strcmp( p_handler->name, "identifier" ) )
                {
                    p_demux->p_sys->i_identifier = atoi( psz_value );
                }
                else
                {
                    /* there MUST be an item */
                    if( !p_new_input )
                    {
                        msg_Err( p_demux, "item not yet created at <%s>",
                                 psz_name );
                        FREE_ATT();
                        return VLC_FALSE;
                    }
                    if( p_handler->pf_handler.smpl )
                    {
                        p_handler->pf_handler.smpl( NULL, p_input,
                                                    p_handler->name,
                                                    psz_value );
                        FREE_ATT();
                    }
                }
                FREE_ATT();
                p_handler = NULL;
                break;

            default:
                /* unknown/unexpected xml node */
                msg_Err( p_demux, "unexpected xml node %i", i_node );
                FREE_ATT();
                return VLC_FALSE;
        }
        FREE_NAME();
    }
    msg_Err( p_demux, "unexpected end of xml data" );
    FREE_ATT();
    return VLC_FALSE;
}

/**
 * \brief handles the supported <track> sub-elements
 */
static vlc_bool_t set_item_info SIMPLE_INTERFACE
{
    /* exit if setting is impossible */
    if( !psz_name || !psz_value || !p_input )
        return VLC_FALSE;

    /* re-convert xml special characters inside psz_value */
    resolve_xml_special_chars( psz_value );

    /* handle each info element in a separate "if" clause */
    if( !strcmp( psz_name, "title" ) )
    {
        p_input->psz_name = strdup( (char*)psz_value );
    }
    else if( !strcmp( psz_name, "creator" ) )
    {
        vlc_meta_SetArtist( p_input->p_meta, psz_value );
    }
    else if( !strcmp( psz_name, "album" ) )
    {
        vlc_meta_SetAlbum( p_input->p_meta, psz_value );

    }
    else if( !strcmp( psz_name, "trackNum" ) )
    {
        vlc_meta_SetTracknum( p_input->p_meta, psz_value );
    }
    else if( !strcmp( psz_name, "duration" ) )
    {
        long i_num = atol( psz_value );
        p_input->i_duration = i_num*1000;
    }
    return VLC_TRUE;
}


/**
 * \brief parse the extension node of a XSPF playlist
 */
static vlc_bool_t parse_extension_node COMPLEX_INTERFACE
{
    char *psz_name = NULL;
    char *psz_value = NULL;
    char *psz_title = NULL;
    int i_node;
    xml_elem_hnd_t *p_handler = NULL;

    xml_elem_hnd_t pl_elements[] =
        { {"node",  COMPLEX_CONTENT, {.cmplx = parse_extension_node} },
          {"item",  COMPLEX_CONTENT, {.cmplx = parse_extitem_node} },
          {NULL,    UNKNOWN_CONTENT, {NULL} }
        };

    /* read all extension node attributes */
    while( xml_ReaderNextAttr( p_xml_reader ) == VLC_SUCCESS )
    {
        psz_name = xml_ReaderName( p_xml_reader );
        psz_value = xml_ReaderValue( p_xml_reader );
        if( !psz_name || !psz_value )
        {
            msg_Err( p_demux, "invalid xml stream @ <node>" );
            FREE_ATT();
            return VLC_FALSE;
        }
        /* attribute: title */
        if( !strcmp( psz_name, "title" ) )
        {
            psz_title = unescape_URI_duplicate( psz_value );
        }
        /* unknown attribute */
        else
            msg_Warn( p_demux, "invalid <node> attribute:\"%s\"", psz_name);

        FREE_ATT();
    }

    /* attribute title is mandatory except for <extension> */
    if( !strcmp( psz_element, "node" ) && !psz_title )
    {
        msg_Warn( p_demux, "<node> requires \"title\" attribute" );
        return VLC_FALSE;
    }

    if( !strcmp( psz_element, "node" ) )
    {
        fprintf( stderr, "  node: %s\n", psz_title );
    }
    if( psz_title ) free( psz_title );

    /* parse the child elements */
    while( xml_ReaderRead( p_xml_reader ) == 1 )
    {
        i_node = xml_ReaderNodeType( p_xml_reader );
        switch( i_node )
        {
            case XML_READER_NONE:
                break;
            case XML_READER_STARTELEM:
                /*  element start tag  */
                psz_name = xml_ReaderName( p_xml_reader );
                if( !psz_name || !*psz_name )
                {
                    msg_Err( p_demux, "invalid xml stream" );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                /* choose handler */
                for( p_handler = pl_elements;
                     p_handler->name && strcmp( psz_name, p_handler->name );
                     p_handler++ );
                if( !p_handler->name )
                {
                    msg_Err( p_demux, "unexpected element <%s>", psz_name );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                FREE_NAME();
                /* complex content is parsed in a separate function */
                if( p_handler->type == COMPLEX_CONTENT )
                {
                    if( p_handler->pf_handler.cmplx( p_demux,
                                                     p_playlist,
                                                     p_item, NULL,
                                                     p_xml_reader,
                                                     p_handler->name ) )
                    {
                        p_handler = NULL;
                        FREE_ATT();
                    }
                    else
                    {
                        FREE_ATT();
                        return VLC_FALSE;
                    }
                }
                break;

            case XML_READER_TEXT:
                /* simple element content */
                FREE_ATT();
                psz_value = xml_ReaderValue( p_xml_reader );
                if( !psz_value )
                {
                    msg_Err( p_demux, "invalid xml stream" );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                break;

            case XML_READER_ENDELEM:
                /* element end tag */
                psz_name = xml_ReaderName( p_xml_reader );
                if( !psz_name )
                {
                    msg_Err( p_demux, "invalid xml stream" );
                    FREE_ATT();
                    return VLC_FALSE;
                }
                /* leave if the current parent node is terminated */
                if( !strcmp( psz_name, psz_element ) )
                {
                    fprintf( stderr, "  </node>\n" );
                    FREE_ATT();
                    return VLC_TRUE;
                }
                /* there MUST have been a start tag for that element name */
                if( !p_handler || !p_handler->name
                    || strcmp( p_handler->name, psz_name ))
                {
                    msg_Err( p_demux, "there's no open element left for <%s>",
                             psz_name );
                    FREE_ATT();
                    return VLC_FALSE;
                }

                if( p_handler->pf_handler.smpl )
                {
                    p_handler->pf_handler.smpl( p_item, NULL, p_handler->name,
                                                psz_value );
                }
                FREE_ATT();
                p_handler = NULL;
                break;

            default:
                /* unknown/unexpected xml node */
                msg_Err( p_demux, "unexpected xml node %i", i_node );
                FREE_ATT();
                return VLC_FALSE;
        }
        FREE_NAME();
    }
    return VLC_FALSE;
}

/**
 * \brief parse the extension item node of a XSPF playlist
 */
static vlc_bool_t parse_extitem_node COMPLEX_INTERFACE
{
    char *psz_name = NULL;
    char *psz_value = NULL;
    int i_href = -1;

    /* read all extension item attributes */
    while( xml_ReaderNextAttr( p_xml_reader ) == VLC_SUCCESS )
    {
        psz_name = xml_ReaderName( p_xml_reader );
        psz_value = xml_ReaderValue( p_xml_reader );
        if( !psz_name || !psz_value )
        {
            msg_Err( p_demux, "invalid xml stream @ <item>" );
            FREE_ATT();
            return VLC_FALSE;
        }
        /* attribute: href */
        if( !strcmp( psz_name, "href" ) )
        {
            i_href = atoi( psz_value );
        }
        /* unknown attribute */
        else
            msg_Warn( p_demux, "invalid <item> attribute:\"%s\"", psz_name);

        FREE_ATT();
    }

    /* attribute href is mandatory */
    if( i_href < 0 )
    {
        msg_Warn( p_demux, "<item> requires \"href\" attribute" );
        return VLC_FALSE;
    }

    if( i_href > p_demux->p_sys->i_tracklist_entries )
    {
        msg_Warn( p_demux, "invalid \"href\" attribute" );
        return VLC_FALSE;
    }

    fprintf( stderr, "    %s\n", p_demux->p_sys->pp_tracklist[i_href]->psz_name );

    return VLC_TRUE;
}

/**
 * \brief skips complex element content that we can't manage
 */
static vlc_bool_t skip_element COMPLEX_INTERFACE
{
    char *psz_endname;

    while( xml_ReaderRead( p_xml_reader ) == 1 )
    {
        if( xml_ReaderNodeType( p_xml_reader ) == XML_READER_ENDELEM )
        {
            psz_endname = xml_ReaderName( p_xml_reader );
            if( !psz_endname )
                return VLC_FALSE;
            if( !strcmp( psz_element, psz_endname ) )
            {
                free( psz_endname );
                return VLC_TRUE;
            }
            else
                free( psz_endname );
        }
    }
    return VLC_FALSE;
}
