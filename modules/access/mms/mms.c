/*****************************************************************************
 * mms.c: MMS over tcp, udp and http access plug-in
 *****************************************************************************
 * Copyright (C) 2002-2004 VideoLAN
 * $Id: mms.c 7522 2004-04-27 16:35:15Z sam $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/


/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <stdlib.h>

#include <vlc/vlc.h>
#include <vlc/input.h>

#include "mms.h"

/****************************************************************************
 * NOTES:
 *  MMSProtocole documentation found at http://get.to/sdp
 ****************************************************************************/

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/

struct access_sys_t
{
    int i_proto;

};

static int  Open        ( vlc_object_t * );
static void Close       ( vlc_object_t * );


/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define CACHING_TEXT N_("Caching value in ms")
#define CACHING_LONGTEXT N_( \
    "Allows you to modify the default caching value for MMS streams. This " \
    "value should be set in millisecond units." )

#define ALL_TEXT N_("Force selection of all streams")

#define BITRATE_TEXT N_("Select maximum bitrate stream")
#define BITRATE_LONGTEXT N_( \
    "Always select the stream with the maximum bitrate." )

vlc_module_begin();
    set_description( _("Microsoft Media Server (MMS) input") );
    set_capability( "access", 0 );

    add_integer( "mms-caching", 4 * DEFAULT_PTS_DELAY / 1000, NULL,
                 CACHING_TEXT, CACHING_LONGTEXT, VLC_TRUE );

    add_bool( "mms-all", 0, NULL, ALL_TEXT, "", VLC_TRUE );
    add_integer( "mms-maxbitrate", 0, NULL, BITRATE_TEXT, BITRATE_LONGTEXT ,
                 VLC_FALSE );

    add_shortcut( "mms" );
    add_shortcut( "mmsu" );
    add_shortcut( "mmst" );
    add_shortcut( "mmsh" );
    set_callbacks( Open, Close );
vlc_module_end();

/*****************************************************************************
 * Open:
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    input_thread_t  *p_input = (input_thread_t*)p_this;

    /* First set ipv4/ipv6 */
    var_Create( p_input, "ipv4", VLC_VAR_BOOL | VLC_VAR_DOINHERIT );
    var_Create( p_input, "ipv6", VLC_VAR_BOOL | VLC_VAR_DOINHERIT );

    /* mms-caching */
    var_Create( p_input, "mms-caching", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );

    /* use specified method */
    if( *p_input->psz_access )
    {
        if( !strncmp( p_input->psz_access, "mmsu", 4 ) )
        {
            return E_( MMSTUOpen )( p_input );
        }
        else if( !strncmp( p_input->psz_access, "mmst", 4 ) )
        {
            return E_( MMSTUOpen )( p_input );
        }
        else if( !strncmp( p_input->psz_access, "mmsh", 4 ) )
        {
            return E_( MMSHOpen )( p_input );
        }
    }

    if( E_( MMSTUOpen )( p_input ) )
    {
        /* try mmsh if mmstu failed */
        return E_( MMSHOpen )( p_input );
    }
    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close: free unused data structures
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    input_thread_t *  p_input = (input_thread_t *)p_this;
    access_sys_t   *  p_sys   = p_input->p_access_data;

    if( p_sys->i_proto == MMS_PROTO_TCP || p_sys->i_proto == MMS_PROTO_UDP )
    {
        E_( MMSTUClose )( p_input );
    }
    else if( p_sys->i_proto == MMS_PROTO_HTTP )
    {
        E_( MMSHClose )( p_input );
    }
}

/****************************************************************************
 * parse hostname:port/path@username:password
 * FIXME ipv6 ip will be baddly parsed (contain ':' )
 ****************************************************************************/
url_t *E_( url_new )  ( char * psz_url )
{
    url_t *p_url = malloc( sizeof( url_t ) );

    char  *psz_dup    = strdup( psz_url );
    char  *psz_parser = psz_dup;

    char  *psz_tmp;

    /* 1: get hostname:port */
    while( *psz_parser == '/' )
    {
        psz_parser++;
    }

    psz_tmp = psz_parser;

    while( *psz_parser &&
           *psz_parser != ':' &&  *psz_parser != '/' && *psz_parser != '@' )
    {
        psz_parser++;
    }

    p_url->psz_host     = strndup( psz_tmp, psz_parser - psz_tmp );

    if( *psz_parser == ':' )
    {
        psz_parser++;
        psz_tmp = psz_parser;

        while( *psz_parser && *psz_parser != '/' && *psz_parser != '@' )
        {
            psz_parser++;
        }
        p_url->i_port = atoi( psz_tmp );
    }
    else
    {
        p_url->i_port = 0;
    }

    /* 2: get path */
    if( *psz_parser == '/' )
    {
        //psz_parser++;

        psz_tmp = psz_parser;

        while( *psz_parser && *psz_parser != '@' )
        {
            psz_parser++;
        }

        p_url->psz_path = strndup( psz_tmp, psz_parser - psz_tmp );
    }
    else
    {
        p_url->psz_path = strdup( "" );
    }

    /* 3: usrname and password */
    if( *psz_parser == '@' )
    {
        psz_parser++;

        psz_tmp = psz_parser;

        while( *psz_parser && *psz_parser != ':' )
        {
            psz_parser++;
        }

        p_url->psz_username = strndup( psz_tmp, psz_parser - psz_tmp );

        if( *psz_parser == ':' )
        {
            psz_parser++;

            p_url->psz_password = strdup( psz_parser );
        }
        else
        {
            p_url->psz_password = strdup( "" );
        }
    }
    else
    {
        p_url->psz_username = strdup( "" );
        p_url->psz_password = strdup( "" );
    }
#if 0
    fprintf( stderr,
             "host=`%s' port=%d path=`%s' username=`%s' password=`%s'\n",
             p_url->psz_host,
             p_url->i_port,
             p_url->psz_path,
             p_url->psz_username,
             p_url->psz_password );
#endif
    free( psz_dup );
    return p_url;
}

void   E_( url_free ) ( url_t * p_url )
{
    free( p_url->psz_host );
    free( p_url->psz_path );
    free( p_url->psz_username );
    free( p_url->psz_password );
    free( p_url );
}
