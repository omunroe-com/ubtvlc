/*****************************************************************************
 * http.c
 *****************************************************************************
 * Copyright (C) 2001-2003 VideoLAN
 * $Id: http.c 9280 2004-11-11 12:31:27Z zorglub $
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
#include <vlc/sout.h>

#include "vlc_httpd.h"
#include "vlc_tls.h"

#define FREE( p ) if( p ) { free( p); (p) = NULL; }

#define DEFAULT_PORT        8080
#define DEFAULT_SSL_PORT    8443

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open ( vlc_object_t * );
static void Close( vlc_object_t * );

#define SOUT_CFG_PREFIX "sout-http-"

#define USER_TEXT N_("Username")
#define USER_LONGTEXT N_("Allows you to give a user name that will be " \
                         "requested to access the stream." )
#define PASS_TEXT N_("Password")
#define PASS_LONGTEXT N_("Allows you to give a password that will be " \
                         "requested to access the stream." )
#define MIME_TEXT N_("Mime")
#define MIME_LONGTEXT N_("Allows you to give the mime returned by the server." )

#define CERT_TEXT N_( "Certificate file" )
#define CERT_LONGTEXT N_( "Path to the x509 PEM certificate file that will "\
                          "be used by the HTTP/SSL stream output" )
#define KEY_TEXT N_( "Private key file" )
#define KEY_LONGTEXT N_( "Path to the x509 PEM private key file that will " \
                         " be used by the HTTP/SSL stream output. Leave " \
                         "empty if you don't have one." )
#define CA_TEXT N_( "Root CA file" )
#define CA_LONGTEXT N_( "Path to the x509 PEM trusted root CA certificates " \
                        "(certificate authority) file that will be used by " \
                        "the HTTP/SSL stream output. Leave empty if you " \
                        "don't have one." )
#define CRL_TEXT N_( "CRL file" )
#define CRL_LONGTEXT N_( "Path to the x509 PEM Certificates Revocation List " \
                         "file that will be HTTP/SSL stream output. Leave " \
                         "empty if you don't have one." )

vlc_module_begin();
    set_description( _("HTTP stream output") );
    set_capability( "sout access", 0 );
    add_shortcut( "http" );
    add_shortcut( "https" );
    add_shortcut( "mmsh" );
    add_string( SOUT_CFG_PREFIX "user", "", NULL,
                USER_TEXT, USER_LONGTEXT, VLC_TRUE );
    add_string( SOUT_CFG_PREFIX "pwd", "", NULL,
                PASS_TEXT, PASS_LONGTEXT, VLC_TRUE );
    add_string( SOUT_CFG_PREFIX "mime", "", NULL,
                MIME_TEXT, MIME_LONGTEXT, VLC_TRUE );
    add_string( SOUT_CFG_PREFIX "cert", "vlc.pem", NULL,
                CERT_TEXT, CERT_LONGTEXT, VLC_TRUE );
    add_string( SOUT_CFG_PREFIX "key", NULL, NULL,
                KEY_TEXT, KEY_LONGTEXT, VLC_TRUE );
    add_string( SOUT_CFG_PREFIX "ca", NULL, NULL,
                CA_TEXT, CA_LONGTEXT, VLC_TRUE );
    add_string( SOUT_CFG_PREFIX "crl", NULL, NULL,
                CRL_TEXT, CRL_LONGTEXT, VLC_TRUE );
    set_callbacks( Open, Close );
vlc_module_end();


/*****************************************************************************
 * Exported prototypes
 *****************************************************************************/
static const char *ppsz_sout_options[] = {
    "user", "pwd", "mime", NULL
};

static int Write( sout_access_out_t *, block_t * );
static int Seek ( sout_access_out_t *, off_t  );

struct sout_access_out_sys_t
{
    /* host */
    httpd_host_t        *p_httpd_host;

    /* stream */
    httpd_stream_t      *p_httpd_stream;

    /* gather header from stream */
    int                 i_header_allocated;
    int                 i_header_size;
    uint8_t             *p_header;
    vlc_bool_t          b_header_complete;
};

/*****************************************************************************
 * Open: open the file
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    sout_access_out_t       *p_access = (sout_access_out_t*)p_this;
    sout_access_out_sys_t   *p_sys;
    tls_server_t            *p_tls;

    char                *psz_parser, *psz_name;

    char                *psz_bind_addr;
    int                 i_bind_port;
    char                *psz_file_name;
    char                *psz_user = NULL;
    char                *psz_pwd = NULL;
    char                *psz_mime = NULL;
    vlc_value_t         val;

    if( !( p_sys = p_access->p_sys =
                malloc( sizeof( sout_access_out_sys_t ) ) ) )
    {
        msg_Err( p_access, "Not enough memory" );
        return VLC_ENOMEM ;
    }

    sout_CfgParse( p_access, SOUT_CFG_PREFIX, ppsz_sout_options, p_access->p_cfg );

    /* p_access->psz_name host.name:port/filename */
    psz_name = psz_parser = strdup( p_access->psz_name );

    psz_bind_addr = psz_parser;
    i_bind_port = 0;
    psz_file_name = "";

    while( *psz_parser && *psz_parser != ':' && *psz_parser != '/' )
    {
        psz_parser++;
    }
    if( *psz_parser == ':' )
    {
        *psz_parser = '\0';
        psz_parser++;
        i_bind_port = atoi( psz_parser );

        while( *psz_parser && *psz_parser != '/' )
        {
            psz_parser++;
        }
    }
    if( *psz_parser == '/' )
    {
        *psz_parser = '\0';
        psz_parser++;
        psz_file_name = psz_parser;
    }

    if( !*psz_file_name )
    {
        psz_file_name = strdup( "/" );
    }
    else if( *psz_file_name != '/' )
    {
        char *p = psz_file_name;

        psz_file_name = malloc( strlen( p ) + 2 );
        strcpy( psz_file_name, "/" );
        strcat( psz_file_name, p );
    }
    else
    {
        psz_file_name = strdup( psz_file_name );
    }

    /* SSL support */
    if( p_access->psz_access && !strcmp( p_access->psz_access, "https" ) )
    {
        const char *psz_cert, *psz_key;
        psz_cert = config_GetPsz( p_this, SOUT_CFG_PREFIX"cert" );
        psz_key = config_GetPsz( p_this, SOUT_CFG_PREFIX"key" );

        p_tls = tls_ServerCreate( p_this, psz_cert, psz_key );
        if ( p_tls == NULL )
        {
            msg_Err( p_this, "TLS initialization error" );
            free( psz_file_name );
            free( psz_name );
            free( p_sys );
            return VLC_EGENERIC;
        }

        psz_cert = config_GetPsz( p_this, SOUT_CFG_PREFIX"ca" );
        if ( ( psz_cert != NULL) && tls_ServerAddCA( p_tls, psz_cert ) )
        {
            msg_Err( p_this, "TLS CA error" );
            tls_ServerDelete( p_tls );
            free( psz_file_name );
            free( psz_name );
            free( p_sys );
            return VLC_EGENERIC;
        }

        psz_cert = config_GetPsz( p_this, SOUT_CFG_PREFIX"crl" );
        if ( ( psz_cert != NULL) && tls_ServerAddCRL( p_tls, psz_cert ) )
        {
            msg_Err( p_this, "TLS CRL error" );
            tls_ServerDelete( p_tls );
            free( psz_file_name );
            free( psz_name );
            free( p_sys );
            return VLC_EGENERIC;
        }

        if( i_bind_port <= 0 )
            i_bind_port = DEFAULT_SSL_PORT;
    }
    else
    {
        p_tls = NULL;
        if( i_bind_port <= 0 )
            i_bind_port = DEFAULT_PORT;
    }

    p_sys->p_httpd_host = httpd_TLSHostNew( VLC_OBJECT(p_access),
                                            psz_bind_addr, i_bind_port,
                                            p_tls );
    if( p_sys->p_httpd_host == NULL )
    {
        msg_Err( p_access, "cannot listen on %s:%d",
                 psz_bind_addr, i_bind_port );

        if( p_tls != NULL )
            tls_ServerDelete( p_tls );
        free( psz_name );
        free( psz_file_name );
        free( p_sys );
        return VLC_EGENERIC;
    }

    if( p_access->psz_access && !strcmp( p_access->psz_access, "mmsh" ) )
    {
        psz_mime = strdup( "video/x-ms-asf-stream" );
    }
    else
    {
        var_Get( p_access, SOUT_CFG_PREFIX "mime", &val );
        if( *val.psz_string )
            psz_mime = val.psz_string;
        else
            free( val.psz_string );
    }

    var_Get( p_access, SOUT_CFG_PREFIX "user", &val );
    if( *val.psz_string )
        psz_user = val.psz_string;
    else
        free( val.psz_string );

    var_Get( p_access, SOUT_CFG_PREFIX "pwd", &val );
    if( *val.psz_string )
        psz_pwd = val.psz_string;
    else
        free( val.psz_string );

    p_sys->p_httpd_stream =
        httpd_StreamNew( p_sys->p_httpd_host, psz_file_name, psz_mime,
                         psz_user, psz_pwd );
    if( psz_user ) free( psz_user );
    if( psz_pwd ) free( psz_pwd );
    if( psz_mime ) free( psz_mime );

    if( p_sys->p_httpd_stream == NULL )
    {
        msg_Err( p_access, "cannot add stream %s", psz_file_name );
        httpd_HostDelete( p_sys->p_httpd_host );

        free( psz_name );
        free( psz_file_name );
        free( p_sys );
        return VLC_EGENERIC;
    }

    free( psz_file_name );
    free( psz_name );

    p_sys->i_header_allocated = 1024;
    p_sys->i_header_size      = 0;
    p_sys->p_header           = malloc( p_sys->i_header_allocated );
    p_sys->b_header_complete  = VLC_FALSE;

    p_access->pf_write       = Write;
    p_access->pf_seek        = Seek;


    /* update p_sout->i_out_pace_nocontrol */
    p_access->p_sout->i_out_pace_nocontrol++;

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close: close the target
 *****************************************************************************/
static void Close( vlc_object_t * p_this )
{
    sout_access_out_t       *p_access = (sout_access_out_t*)p_this;
    sout_access_out_sys_t   *p_sys = p_access->p_sys;

    /* update p_sout->i_out_pace_nocontrol */
    p_access->p_sout->i_out_pace_nocontrol--;

    httpd_StreamDelete( p_sys->p_httpd_stream );
    httpd_HostDelete( p_sys->p_httpd_host );

    FREE( p_sys->p_header );

    msg_Dbg( p_access, "Close" );

    free( p_sys );
}

/*****************************************************************************
 * Write:
 *****************************************************************************/
static int Write( sout_access_out_t *p_access, block_t *p_buffer )
{
    sout_access_out_sys_t *p_sys = p_access->p_sys;
    int i_err = 0;

    while( p_buffer )
    {
        block_t *p_next;

        if( p_buffer->i_flags & BLOCK_FLAG_HEADER )
        {
            /* gather header */
            if( p_sys->b_header_complete )
            {
                /* free previously gathered header */
                p_sys->i_header_size = 0;
                p_sys->b_header_complete = VLC_FALSE;
            }
            if( (int)(p_buffer->i_buffer + p_sys->i_header_size) >
                p_sys->i_header_allocated )
            {
                p_sys->i_header_allocated =
                    p_buffer->i_buffer + p_sys->i_header_size + 1024;
                p_sys->p_header =
                    realloc( p_sys->p_header, p_sys->i_header_allocated );
            }
            memcpy( &p_sys->p_header[p_sys->i_header_size],
                    p_buffer->p_buffer,
                    p_buffer->i_buffer );
            p_sys->i_header_size += p_buffer->i_buffer;
        }
        else if( !p_sys->b_header_complete )
        {
            p_sys->b_header_complete = VLC_TRUE;

            httpd_StreamHeader( p_sys->p_httpd_stream, p_sys->p_header,
                                p_sys->i_header_size );
        }

        /* send data */
        i_err = httpd_StreamSend( p_sys->p_httpd_stream, p_buffer->p_buffer,
                                  p_buffer->i_buffer );

        p_next = p_buffer->p_next;
        block_Release( p_buffer );
        p_buffer = p_next;

        if( i_err < 0 )
        {
            break;
        }
    }

    if( i_err < 0 )
    {
        block_ChainRelease( p_buffer );
    }

    return( i_err < 0 ? VLC_EGENERIC : VLC_SUCCESS );
}

/*****************************************************************************
 * Seek: seek to a specific location in a file
 *****************************************************************************/
static int Seek( sout_access_out_t *p_access, off_t i_pos )
{
    msg_Warn( p_access, "HTTP sout access cannot seek" );
    return VLC_EGENERIC;
}
