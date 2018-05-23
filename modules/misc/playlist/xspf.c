/******************************************************************************
 * xspf.c : XSPF playlist export functions
 ******************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 * $Id: xspf.c 15909 2006-06-14 14:48:32Z yoann $
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
 * \file modules/misc/playlist/xspf.c
 * \brief XSPF playlist export functions
 */
#include <stdio.h>
#include <vlc/vlc.h>
#include <vlc/intf.h>
#include "vlc_meta.h"
#include "vlc_strings.h"
#include "xspf.h"

/**
 * \brief Prints the XSPF header to file, writes each item by xspf_export_item()
 * and closes the open xml elements
 * \param p_this the VLC playlist object
 * \return VLC_SUCCESS if some memory is available, otherwise VLC_ENONMEM
 */
int E_(xspf_export_playlist)( vlc_object_t *p_this )
{
    const playlist_t *p_playlist = (playlist_t *)p_this;
    const playlist_export_t *p_export =
        (playlist_export_t *)p_playlist->p_private;
    int               i, i_count;
    char             *psz_temp;
    playlist_item_t  *p_node = p_export->p_root;

    /* write XSPF XML header */
    fprintf( p_export->p_file, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" );
    fprintf( p_export->p_file,
             "<playlist version=\"1\" xmlns=\"http://xspf.org/ns/0/\">\n" );

    if( !p_node ) return VLC_SUCCESS;

    /* save name of the playlist node */
    psz_temp = convert_xml_special_chars( p_node->p_input->psz_name );
    if( *psz_temp )
    {
        fprintf(  p_export->p_file, "\t<title>%s</title>\n", psz_temp );
    }
    free( psz_temp );

    /* save location of the playlist node */
    psz_temp = assertUTF8URI( p_export->psz_filename );
    if( psz_temp && *psz_temp )
    {
        fprintf( p_export->p_file, "\t<location>%s</location>\n", psz_temp );
        free( psz_temp );
    }

    /* export all items in a flat format */
    fprintf( p_export->p_file, "\t<trackList>\n" );
    i_count = 0;
    for( i = 0; i < p_node->i_children; i++ )
    {
        xspf_export_item( p_node->pp_children[i], p_export->p_file,
                          &i_count );
    }
    fprintf( p_export->p_file, "\t</trackList>\n" );

    /* export the tree structure in <extension> */
    fprintf( p_export->p_file, "\t<extension>\n" );
    i_count = 0;
    for( i = 0; i < p_node->i_children; i++ )
    {
        xspf_extension_item( p_node->pp_children[i], p_export->p_file,
                             &i_count );
    }
    fprintf( p_export->p_file, "\t</extension>\n" );

    /* close the header elements */
    fprintf( p_export->p_file, "</playlist>\n" );

    return VLC_SUCCESS;
}

/**
 * \brief exports one item to file or traverse if item is a node
 * \param p_item playlist item to export
 * \param p_file file to write xml-converted item to
 * \param p_i_count counter for track identifiers
 */
static void xspf_export_item( playlist_item_t *p_item, FILE *p_file,
                              int *p_i_count )
{
    char *psz;
    char *psz_temp;

    if( !p_item ) return;

    /* if we get a node here, we must traverse it */
    if( p_item->i_children > 0 )
    {
        int i;
        for( i = 0; i < p_item->i_children; i++ )
        {
            xspf_export_item( p_item->pp_children[i], p_file, p_i_count );
        }
        return;
    }

    /* don't write empty nodes */
    if( p_item->i_children == 0 )
    {
        return;
    }

    /* leaves can be written directly */
    fprintf( p_file, "\t\t<track>\n" );

    /* print identifier and increase the counter */
    fprintf( p_file, "\t\t\t<identifier>%i</identifier>\n", *p_i_count );
    ( *p_i_count )++;

    /* -> the location */
    if( p_item->p_input->psz_uri && *p_item->p_input->psz_uri )
    {
        psz = assertUTF8URI( p_item->p_input->psz_uri );
        fprintf( p_file, "\t\t\t<location>%s</location>\n", psz );
        free( psz );
    }

    /* -> the name/title (only if different from uri)*/
    if( p_item->p_input->psz_name &&
        p_item->p_input->psz_uri &&
        strcmp( p_item->p_input->psz_uri, p_item->p_input->psz_name ) )
    {
        psz_temp = convert_xml_special_chars( p_item->p_input->psz_name );
        if( *psz_temp )
            fprintf( p_file, "\t\t\t<title>%s</title>\n", psz_temp );
        free( psz_temp );
    }

    if( p_item->p_input->p_meta == NULL )
    {
        goto xspfexportitem_end;
    }

    /* -> the artist/creator */
    psz = p_item->p_input->p_meta->psz_artist ?
                        strdup( p_item->p_input->p_meta->psz_artist ):
                        strdup( "" );
    if( psz && !*psz )
    {
        free( psz );
        psz = NULL;
    }
    if( !psz )
    {
        psz = p_item->p_input->p_meta->psz_author ?
                        strdup( p_item->p_input->p_meta->psz_author ):
                        strdup( "" );
    }
    psz_temp = convert_xml_special_chars( psz );
    if( psz ) free( psz );
    if( *psz_temp )
    {
        fprintf( p_file, "\t\t\t<creator>%s</creator>\n", psz_temp );
    }
    free( psz_temp );

    /* -> the album */
    psz = p_item->p_input->p_meta->psz_album ?
                        strdup( p_item->p_input->p_meta->psz_album ):
                        strdup( "" );
    psz_temp = convert_xml_special_chars( psz );
    if( psz ) free( psz );
    if( *psz_temp )
    {
        fprintf( p_file, "\t\t\t<album>%s</album>\n", psz_temp );
    }
    free( psz_temp );

    /* -> the track number */
    psz = p_item->p_input->p_meta->psz_tracknum ?
                        strdup( p_item->p_input->p_meta->psz_tracknum ):
                        strdup( "" );
    if( psz )
    {
        if( *psz )
        {
            fprintf( p_file, "\t\t\t<trackNum>%i</trackNum>\n", atoi( psz ) );
        }
        free( psz );
    }

xspfexportitem_end:
    /* -> the duration */
    if( p_item->p_input->i_duration > 0 )
    {
        fprintf( p_file, "\t\t\t<duration>%ld</duration>\n",
                 (long)(p_item->p_input->i_duration / 1000) );
    }

    fprintf( p_file, "\t\t</track>\n" );

    return;
}

/**
 * \brief exports one item in extension to file and traverse if item is a node
 * \param p_item playlist item to export
 * \param p_file file to write xml-converted item to
 * \param p_i_count counter for track identifiers
 */
static void xspf_extension_item( playlist_item_t *p_item, FILE *p_file,
                                 int *p_i_count )
{
    if( !p_item ) return;

    /* if we get a node here, we must traverse it */
    if( p_item->i_children >= 0 )
    {
        int i;
        char *psz_temp;
        psz_temp = convert_xml_special_chars( p_item->p_input->psz_name );
        fprintf( p_file, "\t\t<node title=\"%s\">\n",
                 *psz_temp ? p_item->p_input->psz_name : "" );
        free( psz_temp );

        for( i = 0; i < p_item->i_children; i++ )
        {
            xspf_extension_item( p_item->pp_children[i], p_file, p_i_count );
        }

        fprintf( p_file, "\t\t</node>\n" );
        return;
    }


    /* print leaf and increase the counter */
    fprintf( p_file, "\t\t\t<item href=\"%i\" />\n", *p_i_count );
    ( *p_i_count )++;

    return;
}

/**
 * \param psz_name the location of the media ressource (e.g. local file,
 *        device, network stream, etc.)
 * \return a new char buffer which asserts that the location is valid UTF-8
 *         and a valid URI
 * \note the returned buffer must be freed, when it isn't used anymore
 */
static char *assertUTF8URI( char *psz_name )
{
    char *psz_ret = NULL;              /**< the new result buffer to return */
    char *psz_s = NULL, *psz_d = NULL; /**< src & dest pointers for URI conversion */
    vlc_bool_t b_name_is_uri = VLC_FALSE;

    if( !psz_name || !*psz_name )
        return NULL;

    /* check that string is valid UTF-8 */
    /* XXX: Why do we even need to do that ? (all strings in core are UTF-8 encoded */
    if( !( psz_s = EnsureUTF8( psz_name ) ) )
        return NULL;

    /* max. 3x for URI conversion (percent escaping) and
       8 bytes for "file://" and NULL-termination */
    psz_ret = (char *)malloc( sizeof(char)*strlen(psz_name)*6*3+8 );
    if( !psz_ret )
        return NULL;

    /** \todo check for a valid scheme part preceding the colon */
    if( strchr( psz_s, ':' ) )
    {
        psz_d = psz_ret;
        b_name_is_uri = VLC_TRUE;
    }
    /* assume "file" scheme if no scheme-part is included */
    else
    {
        strcpy( psz_ret, "file://" );
        psz_d = psz_ret + 7;
    }

    while( *psz_s )
    {
        /* percent-encode all non-ASCII and the XML special characters and the percent sign itself */
        if( *psz_s & B10000000 ||
            *psz_s == '<' ||
            *psz_s == '>' ||
            *psz_s == '&' ||
            *psz_s == ' ' ||
            ( *psz_s == '%' && !b_name_is_uri ) )
        {
            *psz_d++ = '%';
            *psz_d++ = hexchars[(*psz_s >> 4) & B00001111];
            *psz_d++ = hexchars[*psz_s & B00001111];
        }
        else
        {
            *psz_d++ = *psz_s;
        }

        psz_s++;
    }
    *psz_d = '\0';

    return (char *)realloc( psz_ret, sizeof(char)*strlen( psz_ret ) + 1 );
}
