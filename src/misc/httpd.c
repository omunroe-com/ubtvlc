/*****************************************************************************
 * httpd.c
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 * $Id: httpd.c 9284 2004-11-11 18:50:53Z gbazin $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Remi Denis-Courmont <courmisch@via.ecp.fr>
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

#include <stdlib.h>
#include <vlc/vlc.h>

#ifdef ENABLE_HTTPD

#include "vlc_httpd.h"
#include "network.h"
#include "vlc_tls.h"

#include <string.h>
#include <errno.h>

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif

#ifdef HAVE_FCNTL_H
#   include <fcntl.h>
#endif

#if defined( UNDER_CE )
#   include <winsock.h>
#elif defined( WIN32 )
#   include <winsock2.h>
#   include <ws2tcpip.h>
#else
#   include <netdb.h>                                         /* hostent ... */
#   include <sys/socket.h>
#   include <netinet/in.h>
#   ifdef HAVE_ARPA_INET_H
#       include <arpa/inet.h>                    /* inet_ntoa(), inet_aton() */
#   endif
#endif

#if defined(WIN32)
static const struct in6_addr in6addr_any = {{IN6ADDR_ANY_INIT}};
#endif

#ifndef PF_INET
#    define PF_INET AF_INET                                          /* BeOS */
#endif

#if 0
typedef struct httpd_t          httpd_t;

typedef struct httpd_host_t     httpd_host_t;
typedef struct httpd_url_t      httpd_url_t;
typedef struct httpd_client_t   httpd_client_t;

enum
{
    HTTPD_MSG_NONE,

    /* answer */
    HTTPD_MSG_ANSWER,

    /* channel communication */
    HTTPD_MSG_CHANNEL,

    /* http request */
    HTTPD_MSG_GET,
    HTTPD_MSG_HEAD,
    HTTPD_MSG_POST,

    /* rtsp request */
    HTTPD_MSG_OPTIONS,
    HTTPD_MSG_DESCRIBE,
    HTTPD_MSG_SETUP,
    HTTPD_MSG_PLAY,
    HTTPD_MSG_PAUSE,
    HTTPD_MSG_TEARDOWN,

    /* just to track the count of MSG */
    HTTPD_MSG_MAX
};

enum
{
    HTTPD_PROTO_NONE,
    HTTPD_PROTO_HTTP,
    HTTPD_PROTO_RTSP,
};

typedef struct
{
    httpd_client_t *cl; /* NULL if not throught a connection e vlc internal */

    int     i_type;
    int     i_proto;
    int     i_version;

    /* for an answer */
    int     i_status;
    char    *psz_status;

    /* for a query */
    char    *psz_url;
    char    *psz_args;  /* FIXME find a clean way to handle GET(psz_args) and POST(body) through the same code */

    /* for rtp over rtsp */
    int     i_channel;

    /* options */
    int     i_name;
    char    **name;
    int     i_value;
    char    **value;

    /* body */
    int64_t i_body_offset;
    int     i_body;
    uint8_t *p_body;

} httpd_message_t;

typedef struct httpd_callback_sys_t httpd_callback_sys_t;
/* answer could be null, int this case no answer is requested */
typedef int (*httpd_callback_t)( httpd_callback_sys_t *, httpd_client_t *, httpd_message_t *answer, httpd_message_t *query );


/* create a new host */
httpd_host_t *httpd_HostNew( vlc_object_t *, char *psz_host, int i_port );
/* delete a host */
void          httpd_HostDelete( httpd_host_t * );

/* register a new url */
httpd_url_t *httpd_UrlNew( httpd_host_t *, char *psz_url );
/* register callback on a url */
int          httpd_UrlCatch( httpd_url_t *, int i_msg,
                             httpd_callback_t, httpd_callback_sys_t * );
/* delete an url */
void         httpd_UrlDelete( httpd_url_t * );


void httpd_ClientModeStream( httpd_client_t *cl );
void httpd_ClientModeBidir( httpd_client_t *cl );
static void httpd_ClientClean( httpd_client_t *cl );

/* High level */
typedef struct httpd_file_t     httpd_file_t;
typedef struct httpd_file_sys_t httpd_file_sys_t;
typedef int (*httpd_file_callback_t)( httpd_file_sys_t*, httpd_file_t *, uint8_t *psz_request, uint8_t **pp_data, int *pi_data );
httpd_file_t *httpd_FileNew( httpd_host_t *,
                             char *psz_url, char *psz_mime,
                             char *psz_user, char *psz_password,
                             httpd_file_callback_t pf_fill,
                             httpd_file_sys_t *p_sys );
void         httpd_FileDelete( httpd_file_t * );

typedef struct httpd_redirect_t httpd_redirect_t;
httpd_redirect_t *httpd_RedirectNew( httpd_host_t *, char *psz_url_dst, char *psz_url_src );
void              httpd_RedirectDelete( httpd_redirect_t * );

#if 0
typedef struct httpd_stream_t httpd_stream_t;
httpd_stream_t *httpd_StreamNew( httpd_host_t * );
int             httpd_StreamHeader( httpd_stream_t *, uint8_t *p_data, int i_data );
int             httpd_StreamSend( httpd_stream_t *, uint8_t *p_data, int i_data );
void            httpd_StreamDelete( httpd_stream_t * );
#endif

/* Msg functions facilities */
void         httpd_MsgInit( httpd_message_t * );
void         httpd_MsgAdd( httpd_message_t *, char *psz_name, char *psz_value, ... );
/* return "" if not found. The string is not allocated */
char        *httpd_MsgGet( httpd_message_t *, char *psz_name );
void         httpd_MsgClean( httpd_message_t * );
#endif

#if 0
struct httpd_t
{
    VLC_COMMON_MEMBERS

    /* vlc_mutex_t  lock; */
    int          i_host;
    httpd_host_t **host;
};
#endif

static void httpd_ClientClean( httpd_client_t *cl );

/* each host run in his own thread */
struct httpd_host_t
{
    VLC_COMMON_MEMBERS

    httpd_t     *httpd;

    /* ref count */
    int         i_ref;

    /* address/port and socket for listening at connections */
    struct sockaddr_storage sock;
    int                     i_sock_size;
    int                     fd;

    vlc_mutex_t lock;

    /* all registered url (becarefull that 2 httpd_url_t could point at the same url)
     * This will slow down the url research but make my live easier
     * All url will have their cb trigger, but only the first one can answer
     * */
    int         i_url;
    httpd_url_t **url;

    int            i_client;
    httpd_client_t **client;
    
    /* TLS data */
    tls_server_t *p_tls;
};

struct httpd_url_t
{
    httpd_host_t *host;

    vlc_mutex_t lock;

    char    *psz_url;
    char    *psz_user;
    char    *psz_password;

    struct
    {
        httpd_callback_t     cb;
        httpd_callback_sys_t *p_sys;
    } catch[HTTPD_MSG_MAX];
};

/* status */
enum
{
    HTTPD_CLIENT_RECEIVING,
    HTTPD_CLIENT_RECEIVE_DONE,

    HTTPD_CLIENT_SENDING,
    HTTPD_CLIENT_SEND_DONE,

    HTTPD_CLIENT_WAITING,

    HTTPD_CLIENT_DEAD,
};
/* mode */
enum
{
    HTTPD_CLIENT_FILE,      /* default */
    HTTPD_CLIENT_STREAM,    /* regulary get data from cb */
    HTTPD_CLIENT_BIDIR,     /* check for reading and get data from cb */
};

struct httpd_client_t
{
    httpd_url_t *url;

    int     i_ref;

    struct  sockaddr_storage sock;
    int     i_sock_size;
    int     fd;

    int     i_mode;
    int     i_state;
    int     b_read_waiting; /* stop as soon as possible sending */

    mtime_t i_activity_date;
    mtime_t i_activity_timeout;

    /* buffer for reading header */
    int     i_buffer_size;
    int     i_buffer;
    uint8_t *p_buffer;

    /* */
    httpd_message_t query;  /* client -> httpd */
    httpd_message_t answer; /* httpd -> client */
    
    /* TLS data */
    tls_session_t *p_tls;
};


/*****************************************************************************
 * Various functions
 *****************************************************************************/
/*char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";*/
static void b64_decode( char *dest, char *src )
{
    int  i_level;
    int  last = 0;
    int  b64[256] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
        };

    for( i_level = 0; *src != '\0'; src++ )
    {
        int  c;

        c = b64[(unsigned int)*src];
        if( c == -1 )
        {
            continue;
        }

        switch( i_level )
        {
            case 0:
                i_level++;
                break;
            case 1:
                *dest++ = ( last << 2 ) | ( ( c >> 4)&0x03 );
                i_level++;
                break;
            case 2:
                *dest++ = ( ( last << 4 )&0xf0 ) | ( ( c >> 2 )&0x0f );
                i_level++;
                break;
            case 3:
                *dest++ = ( ( last &0x03 ) << 6 ) | c;
                i_level = 0;
        }
        last = c;
    }

    *dest = '\0';
}

static struct
{
    char *psz_ext;
    char *psz_mime;
} http_mime[] =
{
    { ".htm",   "text/html" },
    { ".html",  "text/html" },
    { ".txt",   "text/plain" },
    { ".xml",   "text/xml" },
    { ".dtd",   "text/dtd" },

    { ".css",   "text/css" },

    /* image mime */
    { ".gif",   "image/gif" },
    { ".jpe",   "image/jpeg" },
    { ".jpg",   "image/jpeg" },
    { ".jpeg",  "image/jpeg" },
    { ".png",   "image/png" },
    { ".mpjpeg","multipart/x-mixed-replace; boundary=This Random String" },

    /* media mime */
    { ".avi",   "video/avi" },
    { ".asf",   "video/x-ms-asf" },
    { ".m1a",   "audio/mpeg" },
    { ".m2a",   "audio/mpeg" },
    { ".m1v",   "video/mpeg" },
    { ".m2v",   "video/mpeg" },
    { ".mp2",   "audio/mpeg" },
    { ".mp3",   "audio/mpeg" },
    { ".mpa",   "audio/mpeg" },
    { ".mpg",   "video/mpeg" },
    { ".mpeg",  "video/mpeg" },
    { ".mpe",   "video/mpeg" },
    { ".mov",   "video/quicktime" },
    { ".moov",  "video/quicktime" },
    { ".ogg",   "application/ogg" },
    { ".ogm",   "application/ogg" },
    { ".wav",   "audio/wav" },
    { ".wma",   "audio/x-ms-wma" },
    { ".wmv",   "video/x-ms-wmv" },


    /* end */
    { NULL,     NULL }
};
static char *httpd_MimeFromUrl( char *psz_url )
{

    char *psz_ext;

    psz_ext = strrchr( psz_url, '.' );
    if( psz_ext )
    {
        int i;

        for( i = 0; http_mime[i].psz_ext != NULL ; i++ )
        {
            if( !strcasecmp( http_mime[i].psz_ext, psz_ext ) )
            {
                return http_mime[i].psz_mime;
            }
        }
    }
    return "application/octet-stream";
}

/*****************************************************************************
 * High Level Funtions: httpd_file_t
 *****************************************************************************/
struct httpd_file_t
{
    httpd_url_t *url;

    char *psz_url;
    char *psz_mime;

    httpd_file_callback_t pf_fill;
    httpd_file_sys_t      *p_sys;

};


static int httpd_FileCallBack( httpd_callback_sys_t *p_sys, httpd_client_t *cl, httpd_message_t *answer, httpd_message_t *query )
{
    httpd_file_t *file = (httpd_file_t*)p_sys;

    if( answer == NULL || query == NULL )
    {
        return VLC_SUCCESS;
    }
    answer->i_proto  = HTTPD_PROTO_HTTP;
    answer->i_version= query->i_version;
    answer->i_type   = HTTPD_MSG_ANSWER;

    answer->i_status = 200;
    answer->psz_status = strdup( "OK" );

    httpd_MsgAdd( answer, "Content-type",  "%s", file->psz_mime );
    httpd_MsgAdd( answer, "Cache-Control", "%s", "no-cache" );

    if( query->i_type != HTTPD_MSG_HEAD )
    {
        char *psz_args = query->psz_args;
        if( query->i_type == HTTPD_MSG_POST )
        {
            /* Check that */
            psz_args = query->p_body;
        }
        file->pf_fill( file->p_sys, file, psz_args, &answer->p_body,
                       &answer->i_body );
    }
    /* We respect client request */
    if( strcmp( httpd_MsgGet( &cl->query, "Connection" ), "" ) )
    {
        httpd_MsgAdd( answer, "Connection",
                      httpd_MsgGet( &cl->query, "Connection" ) );
    }

    httpd_MsgAdd( answer, "Content-Length", "%d", answer->i_body );

    return VLC_SUCCESS;
}


httpd_file_t *httpd_FileNew( httpd_host_t *host,
                             char *psz_url, char *psz_mime,
                             char *psz_user, char *psz_password,
                             httpd_file_callback_t pf_fill,
                             httpd_file_sys_t *p_sys )
{
    httpd_file_t *file = malloc( sizeof( httpd_file_t ) );

    if( ( file->url = httpd_UrlNewUnique( host, psz_url, psz_user,
                                          psz_password ) ) == NULL )
    {
        free( file );
        return NULL;
    }

    file->psz_url  = strdup( psz_url );
    if( psz_mime && *psz_mime )
    {
        file->psz_mime = strdup( psz_mime );
    }
    else
    {
        file->psz_mime = strdup( httpd_MimeFromUrl( psz_url ) );
    }

    file->pf_fill = pf_fill;
    file->p_sys   = p_sys;

    httpd_UrlCatch( file->url, HTTPD_MSG_HEAD, httpd_FileCallBack,
                    (httpd_callback_sys_t*)file );
    httpd_UrlCatch( file->url, HTTPD_MSG_GET,  httpd_FileCallBack,
                    (httpd_callback_sys_t*)file );
    httpd_UrlCatch( file->url, HTTPD_MSG_POST, httpd_FileCallBack,
                    (httpd_callback_sys_t*)file );

    return file;
}

void httpd_FileDelete( httpd_file_t *file )
{
    httpd_UrlDelete( file->url );

    free( file->psz_url );
    free( file->psz_mime );

    free( file );
}

/*****************************************************************************
 * High Level Funtions: httpd_redirect_t
 *****************************************************************************/
struct httpd_redirect_t
{
    httpd_url_t *url;
    char        *psz_dst;
};

static int httpd_RedirectCallBack( httpd_callback_sys_t *p_sys,
                                   httpd_client_t *cl, httpd_message_t *answer,
                                   httpd_message_t *query )
{
    httpd_redirect_t *rdir = (httpd_redirect_t*)p_sys;
    uint8_t *p;

    if( answer == NULL || query == NULL )
    {
        return VLC_SUCCESS;
    }
    answer->i_proto  = query->i_proto;
    answer->i_version= query->i_version;
    answer->i_type   = HTTPD_MSG_ANSWER;
    answer->i_status = 301;
    answer->psz_status = strdup( "Moved Permanently" );

    p = answer->p_body = malloc( 1000 + strlen( rdir->psz_dst ) );
    p += sprintf( p, "<html>\n" );
    p += sprintf( p, "<head>\n" );
    p += sprintf( p, "<title>Redirection</title>\n" );
    p += sprintf( p, "</head>\n" );
    p += sprintf( p, "<body>\n" );
    p += sprintf( p, "<h1><center>You should be <a href=\"%s\">redirected</a></center></h1>\n", rdir->psz_dst );
    p += sprintf( p, "<hr />\n" );
    p += sprintf( p, "<a href=\"http://www.videolan.org\">VideoLAN</a>\n" );
    p += sprintf( p, "</body>\n" );
    p += sprintf( p, "</html>\n" );
    answer->i_body = p - answer->p_body;

    /* XXX check if it's ok or we need to set an absolute url */
    httpd_MsgAdd( answer, "Location",  "%s", rdir->psz_dst );

    httpd_MsgAdd( answer, "Content-Length", "%d", answer->i_body );

    return VLC_SUCCESS;
}

httpd_redirect_t *httpd_RedirectNew( httpd_host_t *host, char *psz_url_dst,
                                     char *psz_url_src )
{
    httpd_redirect_t *rdir = malloc( sizeof( httpd_redirect_t ) );

    if( !( rdir->url = httpd_UrlNewUnique( host, psz_url_src, NULL, NULL ) ) )
    {
        free( rdir );
        return NULL;
    }
    rdir->psz_dst = strdup( psz_url_dst );

    /* Redirect apply for all HTTP request and RTSP DESCRIBE resquest */
    httpd_UrlCatch( rdir->url, HTTPD_MSG_HEAD, httpd_RedirectCallBack,
                    (httpd_callback_sys_t*)rdir );
    httpd_UrlCatch( rdir->url, HTTPD_MSG_GET, httpd_RedirectCallBack,
                    (httpd_callback_sys_t*)rdir );
    httpd_UrlCatch( rdir->url, HTTPD_MSG_POST, httpd_RedirectCallBack,
                    (httpd_callback_sys_t*)rdir );
    httpd_UrlCatch( rdir->url, HTTPD_MSG_DESCRIBE, httpd_RedirectCallBack,
                    (httpd_callback_sys_t*)rdir );

    return rdir;
}
void httpd_RedirectDelete( httpd_redirect_t *rdir )
{
    httpd_UrlDelete( rdir->url );
    free( rdir->psz_dst );
    free( rdir );
}

/*****************************************************************************
 * High Level Funtions: httpd_stream_t
 *****************************************************************************/
struct httpd_stream_t
{
    vlc_mutex_t lock;
    httpd_url_t *url;

    char    *psz_mime;

    /* Header to send as first packet */
    uint8_t *p_header;
    int     i_header;

    /* circular buffer */
    int         i_buffer_size;      /* buffer size, can't be reallocated smaller */
    uint8_t     *p_buffer;          /* buffer */
    int64_t     i_buffer_pos;       /* absolute position from begining */
    int64_t     i_buffer_last_pos;  /* a new connection will start with that */
};

static int httpd_StreamCallBack( httpd_callback_sys_t *p_sys,
                                 httpd_client_t *cl, httpd_message_t *answer,
                                 httpd_message_t *query )
{
    httpd_stream_t *stream = (httpd_stream_t*)p_sys;

    if( answer == NULL || query == NULL || cl == NULL )
    {
        return VLC_SUCCESS;
    }
    if( answer->i_body_offset > 0 )
    {
        int64_t i_write;
        int     i_pos;

#if 0
        fprintf( stderr, "httpd_StreamCallBack i_body_offset=%lld\n",
                 answer->i_body_offset );
#endif

        if( answer->i_body_offset >= stream->i_buffer_pos )
        {
            /* fprintf( stderr, "httpd_StreamCallBack: no data\n" ); */
            return VLC_EGENERIC;    /* wait, no data available */
        }
        if( answer->i_body_offset + stream->i_buffer_size <
            stream->i_buffer_pos )
        {
            /* this client isn't fast enough */
            fprintf( stderr, "fixing i_body_offset (old=%lld new=%lld)\n",
                     answer->i_body_offset, stream->i_buffer_last_pos );
            answer->i_body_offset = stream->i_buffer_last_pos;
        }

        i_pos   = answer->i_body_offset % stream->i_buffer_size;
        i_write = stream->i_buffer_pos - answer->i_body_offset;
        if( i_write > 10000 )
        {
            i_write = 10000;
        }
        else if( i_write <= 0 )
        {
            return VLC_EGENERIC;    /* wait, no data available */
        }

        /* Don't go past the end of the circular buffer */
        i_write = __MIN( i_write, stream->i_buffer_size - i_pos );

        /* using HTTPD_MSG_ANSWER -> data available */
        answer->i_proto  = HTTPD_PROTO_HTTP;
        answer->i_version= 0;
        answer->i_type   = HTTPD_MSG_ANSWER;

        answer->i_body = i_write;
        answer->p_body = malloc( i_write );
        memcpy( answer->p_body, &stream->p_buffer[i_pos], i_write );

        answer->i_body_offset += i_write;

        return VLC_SUCCESS;
    }
    else
    {
        answer->i_proto  = HTTPD_PROTO_HTTP;
        answer->i_version= 0;
        answer->i_type   = HTTPD_MSG_ANSWER;

        answer->i_status = 200;
        answer->psz_status = strdup( "OK" );

        if( query->i_type != HTTPD_MSG_HEAD )
        {
            httpd_ClientModeStream( cl );
            vlc_mutex_lock( &stream->lock );
            /* Send the header */
            if( stream->i_header > 0 )
            {
                answer->i_body = stream->i_header;
                answer->p_body = malloc( stream->i_header );
                memcpy( answer->p_body, stream->p_header, stream->i_header );
            }
            answer->i_body_offset = stream->i_buffer_last_pos;
            vlc_mutex_unlock( &stream->lock );
        }
        else
        {
            httpd_MsgAdd( answer, "Content-Length", "%d", 0 );
            answer->i_body_offset = 0;
        }

        if( !strcmp( stream->psz_mime, "video/x-ms-asf-stream" ) )
        {
            vlc_bool_t b_xplaystream = VLC_FALSE;
            int i;

            httpd_MsgAdd( answer, "Content-type", "%s",
                          "application/octet-stream" );
            httpd_MsgAdd( answer, "Server", "Cougar 4.1.0.3921" );
            httpd_MsgAdd( answer, "Pragma", "no-cache" );
            httpd_MsgAdd( answer, "Pragma", "client-id=%d", rand()&0x7fff );
            httpd_MsgAdd( answer, "Pragma", "features=\"broadcast\"" );

            /* Check if there is a xPlayStrm=1 */
            for( i = 0; i < query->i_name; i++ )
            {
                if( !strcasecmp( query->name[i],  "Pragma" ) &&
                    strstr( query->value[i], "xPlayStrm=1" ) )
                {
                    b_xplaystream = VLC_TRUE;
                }
            }

            if( !b_xplaystream )
            {
                answer->i_body_offset = 0;
            }
        }
        else
        {
            httpd_MsgAdd( answer, "Content-type",  "%s", stream->psz_mime );
        }
        httpd_MsgAdd( answer, "Cache-Control", "%s", "no-cache" );
        return VLC_SUCCESS;
    }
}

httpd_stream_t *httpd_StreamNew( httpd_host_t *host,
                                 char *psz_url, char *psz_mime,
                                 char *psz_user, char *psz_password )
{
    httpd_stream_t *stream = malloc( sizeof( httpd_stream_t ) );

    if( ( stream->url = httpd_UrlNewUnique( host, psz_url, psz_user,
                                            psz_password ) ) == NULL )
    {
        free( stream );
        return NULL;
    }
    vlc_mutex_init( host, &stream->lock );
    if( psz_mime && *psz_mime )
    {
        stream->psz_mime = strdup( psz_mime );
    }
    else
    {
        stream->psz_mime = strdup( httpd_MimeFromUrl( psz_url ) );
    }
    stream->i_header = 0;
    stream->p_header = NULL;
    stream->i_buffer_size = 5000000;    /* 5 Mo per stream */
    stream->p_buffer = malloc( stream->i_buffer_size );
    /* We set to 1 to make life simpler
     * (this way i_body_offset can never be 0) */
    stream->i_buffer_pos = 1;
    stream->i_buffer_last_pos = 1;

    httpd_UrlCatch( stream->url, HTTPD_MSG_HEAD, httpd_StreamCallBack,
                    (httpd_callback_sys_t*)stream );
    httpd_UrlCatch( stream->url, HTTPD_MSG_GET, httpd_StreamCallBack,
                    (httpd_callback_sys_t*)stream );
    httpd_UrlCatch( stream->url, HTTPD_MSG_POST, httpd_StreamCallBack,
                    (httpd_callback_sys_t*)stream );

    return stream;
}

int httpd_StreamHeader( httpd_stream_t *stream, uint8_t *p_data, int i_data )
{
    vlc_mutex_lock( &stream->lock );
    if( stream->p_header )
    {
        free( stream->p_header );
        stream->p_header = NULL;
    }
    stream->i_header = i_data;
    if( i_data > 0 )
    {
        stream->p_header = malloc( i_data );
        memcpy( stream->p_header, p_data, i_data );
    }
    vlc_mutex_unlock( &stream->lock );

    return VLC_SUCCESS;
}

int httpd_StreamSend( httpd_stream_t *stream, uint8_t *p_data, int i_data )
{
    int i_count;
    int i_pos;

    if( i_data < 0 || p_data == NULL )
    {
        return VLC_SUCCESS;
    }
    vlc_mutex_lock( &stream->lock );

    /* save this pointer (to be used by new connection) */
    stream->i_buffer_last_pos = stream->i_buffer_pos;

    i_pos = stream->i_buffer_pos % stream->i_buffer_size;
    i_count = i_data;
    while( i_count > 0)
    {
        int i_copy;

        i_copy = __MIN( i_count, stream->i_buffer_size - i_pos );

        /* Ok, we can't go past the end of our buffer */
        memcpy( &stream->p_buffer[i_pos], p_data, i_copy );

        i_pos = ( i_pos + i_copy ) % stream->i_buffer_size;
        i_count -= i_copy;
        p_data  += i_copy;
    }

    stream->i_buffer_pos += i_data;

    vlc_mutex_unlock( &stream->lock );
    return VLC_SUCCESS;
}

void httpd_StreamDelete( httpd_stream_t *stream )
{
    httpd_UrlDelete( stream->url );
    vlc_mutex_destroy( &stream->lock );
    if( stream->psz_mime ) free( stream->psz_mime );
    if( stream->p_header ) free( stream->p_header );
    if( stream->p_buffer ) free( stream->p_buffer );
    free( stream );
}


/*****************************************************************************
 * Low level
 *****************************************************************************/
#define LISTEN_BACKLOG          100

#if defined(HAVE_GETNAMEINFO) && !defined(HAVE_GETADDRINFO)
/* 
 * For now, VLC's configure script does not check for getaddrinfo(),
 * but it should be present if getnameinfo() is (the opposite is untrue, with
 * Debian potato as an example)
 */
# define HAVE_GETADDRINFO 1
#endif

static void httpd_HostThread( httpd_host_t * );
static int GetAddrPort( const struct sockaddr_storage *p_ss );

#ifndef HAVE_GETADDRINFO
struct httpd_addrinfo
{
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    /*int ai_flags;*/
    struct sockaddr *ai_addr;
    int ai_addrlen;
    struct httpd_addrinfo *ai_next;
};
#   define addrinfo httpd_addrinfo

static int BuildAddr( struct sockaddr_in * p_socket,
                      const char * psz_address, int i_port );
#endif


/* create a new host */
httpd_host_t *httpd_HostNew( vlc_object_t *p_this, char *psz_host,
                             int i_port )
{
    return httpd_TLSHostNew( p_this, psz_host, i_port, NULL );
}

httpd_host_t *httpd_TLSHostNew( vlc_object_t *p_this, char *psz_host,
                                int i_port, tls_server_t *p_tls )
{
    httpd_t      *httpd;
    httpd_host_t *host = NULL;
    vlc_value_t lockval;
    int fd = -1;
    struct addrinfo *res, *ptr;

    /* resolv */
#ifdef HAVE_GETADDRINFO
    {
        vlc_value_t val;
        char psz_port[6];
        struct addrinfo hints;

        memset( &hints, 0, sizeof( hints ) );

#if 0
        /* 
         * For now, keep IPv4 by default. That said, it should be safe to use
         * IPv6 by default *on the server side*, as, apart from NetBSD, most
         * systems accept IPv4 clients on IPv6 listening sockets.
         */
        hints.ai_family = PF_INET;
#else
        hints.ai_family = 0;

        /* Check if ipv4 or ipv6 were forced */
        var_Create( p_this, "ipv4", VLC_VAR_BOOL | VLC_VAR_DOINHERIT );
        var_Get( p_this, "ipv4", &val );
        if( val.b_bool )
            hints.ai_family = PF_INET;
#endif
        var_Create( p_this, "ipv6", VLC_VAR_BOOL | VLC_VAR_DOINHERIT );
        var_Get( p_this, "ipv6", &val );
        if( val.b_bool )
            hints.ai_family = PF_INET6;

        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        if (*psz_host == '\0')
            psz_host = NULL;

        snprintf( psz_port, sizeof( psz_port ), "%d", i_port );
        psz_port[sizeof( psz_port ) - 1] = '\0';
        
        if( getaddrinfo( psz_host, psz_port, &hints, &res ) )
        {
            msg_Err( p_this, "cannot resolve %s:%d", psz_host, i_port );
            return NULL;
        }
    }

#else
    struct sockaddr_in sock;
    struct httpd_addrinfo info;
    
    info.ai_family = PF_INET;
    info.ai_socktype = SOCK_STREAM;
    info.ai_protocol = 0;
    info.ai_addr = (struct sockaddr *)&sock;
    info.ai_addrlen = sizeof( sock );
    info.ai_next = NULL;
    
    res = &info;

    if( BuildAddr( &sock, psz_host, i_port ) )
    {
        msg_Err( p_this, "cannot build address for %s:%d", psz_host, i_port );
        return NULL;
    }

#   define freeaddrinfo( r ) (void)0;
#endif

    /* to be sure to avoid multiple creation */
    var_Create( p_this->p_libvlc, "httpd_mutex", VLC_VAR_MUTEX );
    var_Get( p_this->p_libvlc, "httpd_mutex", &lockval );
    vlc_mutex_lock( lockval.p_address );

    if( !(httpd = vlc_object_find( p_this, VLC_OBJECT_HTTPD, FIND_ANYWHERE )) )
    {
        msg_Info( p_this, "creating httpd" );
        if( ( httpd = vlc_object_create( p_this, VLC_OBJECT_HTTPD ) ) == NULL )
        {
            vlc_mutex_unlock( lockval.p_address );
            freeaddrinfo( res );
            return NULL;
        }

        httpd->i_host = 0;
        httpd->host   = NULL;

        vlc_object_yield( httpd );
        vlc_object_attach( httpd, p_this->p_vlc );
    }

    for( ptr = res; (ptr != NULL) && (fd == -1); ptr = ptr->ai_next )
    {
        int i;

        if( ((unsigned)ptr->ai_addrlen) > sizeof( struct sockaddr_storage ) )
        {
            msg_Dbg( p_this, "socket address too big" );
            continue;
        }

        /* verify if it already exist */
        for( i = 0; i < httpd->i_host; i++ )
        {
            if( GetAddrPort (&httpd->host[i]->sock) != i_port )
                continue;

            /* Cannot re-use host if it uses TLS/SSL */
            if( &httpd->host[i]->p_tls != NULL )
                continue;

#ifdef AF_INET6
            if( httpd->host[i]->sock.ss_family == AF_INET6 )
            {
                const struct sockaddr_in6 *p_hsock, *p_sock;

                p_hsock = (const struct sockaddr_in6 *)&httpd->host[i]->sock;
                p_sock = (const struct sockaddr_in6 *)ptr->ai_addr;

                if( memcmp( &p_hsock->sin6_addr, &in6addr_any,
                            sizeof( struct in6_addr ) ) &&
                            ( p_sock->sin6_family != AF_INET6 ||
                              memcmp( &p_hsock->sin6_addr, &p_sock->sin6_addr,
                                      sizeof( struct in6_addr ) ) ) )
                    continue; /* does not match */
            }
            else if( ptr->ai_family == PF_INET6 )
                continue;
            else
#endif
            if( httpd->host[i]->sock.ss_family == AF_INET )
            {
                const struct sockaddr_in *p_hsock, *p_sock;

                p_hsock = (const struct sockaddr_in *)&httpd->host[i]->sock;
                p_sock = (const struct sockaddr_in *)ptr->ai_addr;

                if( p_hsock->sin_addr.s_addr != INADDR_ANY &&
                    ( p_sock->sin_family != AF_INET ||
                      p_hsock->sin_addr.s_addr != p_sock->sin_addr.s_addr ) )
                    continue; /* does not match */
            }
            else if( ptr->ai_family == PF_INET )
                continue;
            else
            {
                msg_Dbg( p_this, "host with unknown address family" );
                continue;
            }

            freeaddrinfo( res );

            /* yep found */
            host = httpd->host[i];
            host->i_ref++;

            vlc_mutex_unlock( lockval.p_address );

            msg_Dbg( p_this, "host already registered" );
            return host;
        }

        /* create the listening socket */
        fd = socket( ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol );
        if( fd == -1 )
            continue;

        /* reuse socket */
        {
            int dummy = 1;
            if( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
                            (void *)&dummy, sizeof( dummy ) ) < 0 )
            {
                msg_Warn( p_this, "cannot configure socket (SO_REUSEADDR)" );
            }
        }

        /* bind it */
        if( bind( fd, ptr->ai_addr, ptr->ai_addrlen ) )
        {
            msg_Err( p_this, "cannot bind socket" );
            goto socket_error;
        }
        /* set to non-blocking */
#if defined( WIN32 ) || defined( UNDER_CE )
        {
            unsigned long i_dummy = 1;
            if( ioctlsocket( fd, FIONBIO, &i_dummy ) != 0 )
            {
                msg_Err( p_this, "cannot set socket to non-blocking mode" );
                goto socket_error;
            }
        }
#else
        {
            unsigned int i_flags;
            if( ( i_flags = fcntl( fd, F_GETFL, 0 ) ) < 0 )
            {
                msg_Err( p_this, "cannot F_GETFL socket" );
                goto socket_error;
            }
            if( fcntl( fd, F_SETFL, i_flags | O_NONBLOCK ) < 0 )
            {
                msg_Err( p_this, "cannot F_SETFL O_NONBLOCK" );
                goto socket_error;
            }
        }
#endif
        /* listen */
        if( listen( fd, LISTEN_BACKLOG ) < 0 )
        {
            msg_Err( p_this, "cannot listen socket" );
            goto socket_error;
        }

        break; // success

socket_error:
        net_Close( fd );
        fd = -1;
    }


    if( fd == -1 )
    {
        freeaddrinfo( res );
        goto error;
    }

    /* create the new host */
    host = vlc_object_create( p_this, sizeof( httpd_host_t ) );
    host->httpd = httpd;
    vlc_mutex_init( httpd, &host->lock );
    host->i_ref = 1;
    host->fd = fd;

    memcpy( &host->sock, ptr->ai_addr, ptr->ai_addrlen );
    host->i_sock_size = ptr->ai_addrlen;
    host->i_url     = 0;
    host->url       = NULL;
    host->i_client  = 0;
    host->client    = NULL;

    freeaddrinfo( res );
    host->p_tls = p_tls;

    /* create the thread */
    if( vlc_thread_create( host, "httpd host thread", httpd_HostThread,
                           VLC_THREAD_PRIORITY_LOW, VLC_FALSE ) )
    {
        msg_Err( p_this, "cannot spawn http host thread" );
        goto error;
    }

    /* now add it to httpd */
    TAB_APPEND( httpd->i_host, httpd->host, host );
    vlc_mutex_unlock( lockval.p_address );

    return host;

error:
    vlc_mutex_unlock( lockval.p_address );

    if( fd != -1 )
        net_Close( fd );

    if( host != NULL )
    {
        vlc_mutex_destroy( &host->lock );
        vlc_object_destroy( host );
    }

    /* TODO destroy no more used httpd TODO */
    vlc_object_release( httpd );
    return NULL;
}

/* delete a host */
void httpd_HostDelete( httpd_host_t *host )
{
    httpd_t *httpd = host->httpd;
    vlc_value_t lockval;
    int i;

    msg_Dbg( host, "httpd_HostDelete" );

    var_Get( httpd->p_libvlc, "httpd_mutex", &lockval );
    vlc_mutex_lock( lockval.p_address );

    vlc_object_release( httpd );

    host->i_ref--;
    if( host->i_ref > 0 )
    {
        /* still used */
        vlc_mutex_unlock( lockval.p_address );
        msg_Dbg( host, "httpd_HostDelete: host still used" );
        return;
    }
    TAB_REMOVE( httpd->i_host, httpd->host, host );

    msg_Dbg( host, "httpd_HostDelete: host removed from http" );

    host->b_die = 1;
    vlc_thread_join( host );

    msg_Dbg( host, "httpd_HostDelete: host thread joined" );

    for( i = 0; i < host->i_url; i++ )
    {
        msg_Err( host, "url still registered:%s", host->url[i]->psz_url );
    }
    for( i = 0; i < host->i_client; i++ )
    {
        httpd_client_t *cl = host->client[i];
        msg_Warn( host, "client still connected" );
        httpd_ClientClean( cl );
        TAB_REMOVE( host->i_client, host->client, cl );
        free( cl );
        i--;
        /* TODO */
    }

    if( host->p_tls != NULL)
        tls_ServerDelete( host->p_tls );
    net_Close( host->fd );
    vlc_mutex_destroy( &host->lock );
    vlc_object_destroy( host );

    if( httpd->i_host <= 0 )
    {
        msg_Info( httpd, "httpd doesn't reference any host, deleting" );
        vlc_object_detach( httpd );
        vlc_object_destroy( httpd );
    }
    vlc_mutex_unlock( lockval.p_address );
}

/* register a new url */
static httpd_url_t *httpd_UrlNewPrivate( httpd_host_t *host, char *psz_url,
                                         char *psz_user, char *psz_password,
                                         vlc_bool_t b_check )
{
    httpd_url_t *url;
    int         i;

    vlc_mutex_lock( &host->lock );
    if( b_check )
    {
        for( i = 0; i < host->i_url; i++ )
        {
            if( !strcmp( psz_url, host->url[i]->psz_url ) )
            {
                msg_Warn( host->httpd,
                          "cannot add '%s' (url already defined)", psz_url );
                vlc_mutex_unlock( &host->lock );
                return NULL;
            }
        }
    }

    url = malloc( sizeof( httpd_url_t ) );
    url->host = host;

    vlc_mutex_init( host->httpd, &url->lock );
    url->psz_url = strdup( psz_url );
    url->psz_user = strdup( psz_user ? psz_user : "" );
    url->psz_password = strdup( psz_password ? psz_password : "" );
    for( i = 0; i < HTTPD_MSG_MAX; i++ )
    {
        url->catch[i].cb = NULL;
        url->catch[i].p_sys = NULL;
    }

    TAB_APPEND( host->i_url, host->url, url );
    vlc_mutex_unlock( &host->lock );

    return url;
}

httpd_url_t *httpd_UrlNew( httpd_host_t *host, char *psz_url,
                           char *psz_user, char *psz_password )
{
    return httpd_UrlNewPrivate( host, psz_url, psz_user,
                                psz_password, VLC_FALSE );
}

httpd_url_t *httpd_UrlNewUnique( httpd_host_t *host, char *psz_url,
                                 char *psz_user, char *psz_password )
{
    return httpd_UrlNewPrivate( host, psz_url, psz_user,
                                psz_password, VLC_TRUE );
}

/* register callback on a url */
int httpd_UrlCatch( httpd_url_t *url, int i_msg, httpd_callback_t cb,
                    httpd_callback_sys_t *p_sys )
{
    vlc_mutex_lock( &url->lock );
    url->catch[i_msg].cb   = cb;
    url->catch[i_msg].p_sys= p_sys;
    vlc_mutex_unlock( &url->lock );

    return VLC_SUCCESS;
}


/* delete an url */
void httpd_UrlDelete( httpd_url_t *url )
{
    httpd_host_t *host = url->host;
    int          i;

    vlc_mutex_lock( &host->lock );
    TAB_REMOVE( host->i_url, host->url, url );

    vlc_mutex_destroy( &url->lock );
    free( url->psz_url );
    free( url->psz_user );
    free( url->psz_password );

    for( i = 0; i < host->i_client; i++ )
    {
        httpd_client_t *client = host->client[i];

        if( client->url == url )
        {
            /* TODO complete it */
            msg_Warn( host, "force closing connections" );
            httpd_ClientClean( client );
            TAB_REMOVE( host->i_client, host->client, client );
            free( client );
            i--;
        }
    }
    free( url );
    vlc_mutex_unlock( &host->lock );
}

void httpd_MsgInit( httpd_message_t *msg )
{
    msg->cl         = NULL;
    msg->i_type     = HTTPD_MSG_NONE;
    msg->i_proto    = HTTPD_PROTO_NONE;
    msg->i_version  = -1;

    msg->i_status   = 0;
    msg->psz_status = NULL;

    msg->psz_url = NULL;
    msg->psz_args = NULL;

    msg->i_channel = -1;

    msg->i_name = 0;
    msg->name   = NULL;
    msg->i_value= 0;
    msg->value  = NULL;

    msg->i_body_offset = 0;
    msg->i_body        = 0;
    msg->p_body        = 0;
}

void httpd_MsgClean( httpd_message_t *msg )
{
    int i;

    if( msg->psz_status )
    {
        free( msg->psz_status );
    }
    if( msg->psz_url )
    {
        free( msg->psz_url );
    }
    if( msg->psz_args )
    {
        free( msg->psz_args );
    }
    for( i = 0; i < msg->i_name; i++ )
    {
        free( msg->name[i] );
        free( msg->value[i] );
    }
    if( msg->name )
    {
        free( msg->name );
    }
    if( msg->value )
    {
        free( msg->value );
    }
    if( msg->p_body )
    {
        free( msg->p_body );
    }
    httpd_MsgInit( msg );
}

char *httpd_MsgGet( httpd_message_t *msg, char *name )
{
    int i;

    for( i = 0; i < msg->i_name; i++ )
    {
        if( !strcasecmp( msg->name[i], name ))
        {
            return msg->value[i];
        }
    }
    return "";
}
void httpd_MsgAdd( httpd_message_t *msg, char *name, char *psz_value, ... )
{
    va_list args;
    char *value = NULL;

    va_start( args, psz_value );
#if defined(HAVE_VASPRINTF) && !defined(SYS_DARWIN) && !defined(SYS_BEOS)
    vasprintf( &value, psz_value, args );
#else
    {
        int i_size = strlen( psz_value ) + 4096;    /* FIXME stupid system */
        value = calloc( i_size, sizeof( char ) );
        vsnprintf( value, i_size, psz_value, args );
        value[i_size - 1] = 0;
    }
#endif
    va_end( args );

    name = strdup( name );

    TAB_APPEND( msg->i_name,  msg->name,  name );
    TAB_APPEND( msg->i_value, msg->value, value );
}

static void httpd_ClientInit( httpd_client_t *cl )
{
    cl->i_state = HTTPD_CLIENT_RECEIVING;
    cl->i_activity_date = mdate();
    cl->i_activity_timeout = I64C(10000000);
    cl->i_buffer_size = 10000;
    cl->i_buffer = 0;
    cl->p_buffer = malloc( cl->i_buffer_size );
    cl->i_mode   = HTTPD_CLIENT_FILE;
    cl->b_read_waiting = VLC_FALSE;

    httpd_MsgInit( &cl->query );
    httpd_MsgInit( &cl->answer );
}

void httpd_ClientModeStream( httpd_client_t *cl )
{
    cl->i_mode   = HTTPD_CLIENT_STREAM;
}

void httpd_ClientModeBidir( httpd_client_t *cl )
{
    cl->i_mode   = HTTPD_CLIENT_BIDIR;
}

char* httpd_ClientIP( httpd_client_t *cl )
{
#ifdef HAVE_GETNAMEINFO
    char sz_ip[INET6_ADDRSTRLEN + 2];
    int i;

    if( (cl->sock.ss_family == AF_INET6) &&
        IN6_IS_ADDR_V4MAPPED( &((const struct sockaddr_in6 *)
                              &cl->sock)->sin6_addr) )
    {
        /* If client is using IPv4 but server is using IPv6 */
        struct sockaddr_in a;
        
        memset( &a, 0, sizeof( a ) );
        a.sin_family = AF_INET;
        a.sin_port = ((const struct sockaddr_in6 *)&cl->sock)->sin6_port;
        a.sin_addr.s_addr = ((const uint32_t *)&((const struct sockaddr_in6 *)
                            &cl->sock)->sin6_addr)[3];
        i = getnameinfo( (const struct sockaddr *)&a, sizeof( a ),
                         &sz_ip[1], INET6_ADDRSTRLEN, NULL, 0, NI_NUMERICHOST );
    }
    else
        i = getnameinfo( (const struct sockaddr *)&cl->sock, cl->i_sock_size,
                         &sz_ip[1], INET6_ADDRSTRLEN, NULL, 0,
                         NI_NUMERICHOST );

    if( i != 0 )
        /* FIXME: msg_Err */
        return NULL;
        
    if( strchr( &sz_ip[1], ':' ) != NULL )
    {
        *sz_ip = '[';
        i = strlen( sz_ip );
        sz_ip[i++] = ']';
        sz_ip[i] = '\0';
       
        return strdup( sz_ip );
    }
    
    return strdup( &sz_ip[1] );

#else
    /* FIXME not thread safe */
    return strdup( inet_ntoa( ((const struct sockaddr_in *)&cl->sock)->sin_addr ) );
#endif
}

static void httpd_ClientClean( httpd_client_t *cl )
{
    if( cl->fd >= 0 )
    {
        if( cl->p_tls != NULL )
            tls_SessionClose( cl->p_tls );
        net_Close( cl->fd );
        cl->fd = -1;
    }

    httpd_MsgClean( &cl->answer );
    httpd_MsgClean( &cl->query );

    if( cl->p_buffer )
    {
        free( cl->p_buffer );
        cl->p_buffer = NULL;
    }
}

static httpd_client_t *httpd_ClientNew( int fd, struct sockaddr_storage *sock,
                                        int i_sock_size,
                                        tls_session_t *p_tls )
{
    httpd_client_t *cl = malloc( sizeof( httpd_client_t ) );
    /* set this new socket non-block */
#if defined( WIN32 ) || defined( UNDER_CE )
    {
        unsigned long i_dummy = 1;
        ioctlsocket( fd, FIONBIO, &i_dummy );
    }
#else
    fcntl( fd, F_SETFL, O_NONBLOCK );
#endif
    cl->i_ref   = 0;
    cl->fd      = fd;
    memcpy( &cl->sock, sock, sizeof( cl->sock ) );
    cl->i_sock_size = i_sock_size;
    cl->url     = NULL;
    cl->p_tls = p_tls;

    httpd_ClientInit( cl );

    return cl;
}


static int httpd_NetRecv( httpd_client_t *cl, char *p, int i_len )
{
    tls_session_t *p_tls;
    
    p_tls = cl->p_tls;
    if( p_tls != NULL)
        return tls_Recv( p_tls, p, i_len );

    return recv( cl->fd, p, i_len, 0 );
}


static int httpd_NetSend( httpd_client_t *cl, const char *p, int i_len )
{
    tls_session_t *p_tls;

    p_tls = cl->p_tls;
    if( p_tls != NULL)
        return tls_Send( p_tls, p, i_len );

    return send( cl->fd, p, i_len, 0 );
}


static void httpd_ClientRecv( httpd_client_t *cl )
{
    int i_len;

    if( cl->query.i_proto == HTTPD_PROTO_NONE )
    {
        /* enought to see if it's rtp over rtsp or RTSP/HTTP */
        i_len = httpd_NetRecv( cl, &cl->p_buffer[cl->i_buffer],
                               4 - cl->i_buffer );
        if( i_len > 0 )
        {
            cl->i_buffer += i_len;
        }

        if( cl->i_buffer >= 4 )
        {
            fprintf( stderr, "peek=%4.4s\n", cl->p_buffer );
            /* detect type */
            if( cl->p_buffer[0] == '$' )
            {
                /* RTSP (rtp over rtsp) */
                cl->query.i_proto = HTTPD_PROTO_RTSP;
                cl->query.i_type  = HTTPD_MSG_CHANNEL;
                cl->query.i_channel = cl->p_buffer[1];
                cl->query.i_body  = (cl->p_buffer[2] << 8)|cl->p_buffer[3];
                cl->query.p_body  = malloc( cl->query.i_body );

                cl->i_buffer      = 0;
            }
            else if( !strncmp( cl->p_buffer, "HTTP", 4 ) )
            {
                cl->query.i_proto = HTTPD_PROTO_HTTP;
                cl->query.i_type  = HTTPD_MSG_ANSWER;
            }
            else if( !strncmp( cl->p_buffer, "RTSP", 4 ) )
            {
                cl->query.i_proto = HTTPD_PROTO_RTSP;
                cl->query.i_type  = HTTPD_MSG_ANSWER;
            }
            else if( !strncmp( cl->p_buffer, "GET", 3 ) ||
                     !strncmp( cl->p_buffer, "HEAD", 4 ) ||
                     !strncmp( cl->p_buffer, "POST", 4 ) )
            {
                cl->query.i_proto = HTTPD_PROTO_HTTP;
                cl->query.i_type  = HTTPD_MSG_NONE;
            }
            else
            {
                cl->query.i_proto = HTTPD_PROTO_RTSP;
                cl->query.i_type  = HTTPD_MSG_NONE;
            }
        }
    }
    else if( cl->query.i_body > 0 )
    {
        /* we are reading the body of a request or a channel */
        i_len = httpd_NetRecv( cl, &cl->query.p_body[cl->i_buffer],
                               cl->query.i_body - cl->i_buffer );
        if( i_len > 0 )
        {
            cl->i_buffer += i_len;
        }
        if( cl->i_buffer >= cl->query.i_body )
        {
            cl->i_state = HTTPD_CLIENT_RECEIVE_DONE;
        }
    }
    else
    {
        /* we are reading a header -> char by char */
        for( ;; )
        {
            i_len = httpd_NetRecv (cl, &cl->p_buffer[cl->i_buffer], 1 );
            if( i_len <= 0 )
            {
                break;
            }
            cl->i_buffer++;

            if( cl->i_buffer + 1 >= cl->i_buffer_size )
            {
                cl->i_buffer_size += 1024;
                cl->p_buffer = realloc( cl->p_buffer, cl->i_buffer_size );
            }
            if( ( cl->i_buffer >= 2 && !strncmp( &cl->p_buffer[cl->i_buffer-2], "\n\n", 2 ) )||
                ( cl->i_buffer >= 4 && !strncmp( &cl->p_buffer[cl->i_buffer-4], "\r\n\r\n", 4 ) ) )
            {
                char *p;

                /* we have finished the header so parse it and set i_body */
                cl->p_buffer[cl->i_buffer] = '\0';

                if( cl->query.i_type == HTTPD_MSG_ANSWER )
                {
                    cl->query.i_status =
                        strtol( &cl->p_buffer[strlen( "HTTP/1.x" )], &p, 0 );
                    while( *p == ' ' )
                    {
                        p++;
                    }
                    cl->query.psz_status = strdup( p );
                }
                else
                {
                    static const struct
                    {
                        char *name;
                        int  i_type;
                        int  i_proto;
                    }
                    msg_type[] =
                    {
                        { "GET",        HTTPD_MSG_GET,  HTTPD_PROTO_HTTP },
                        { "HEAD",       HTTPD_MSG_HEAD, HTTPD_PROTO_HTTP },
                        { "POST",       HTTPD_MSG_POST, HTTPD_PROTO_HTTP },

                        { "OPTIONS",    HTTPD_MSG_OPTIONS,  HTTPD_PROTO_RTSP },
                        { "DESCRIBE",   HTTPD_MSG_DESCRIBE, HTTPD_PROTO_RTSP },
                        { "SETUP",      HTTPD_MSG_SETUP,    HTTPD_PROTO_RTSP },
                        { "PLAY",       HTTPD_MSG_PLAY,     HTTPD_PROTO_RTSP },
                        { "PAUSE",      HTTPD_MSG_PAUSE,    HTTPD_PROTO_RTSP },
                        { "TEARDOWN",   HTTPD_MSG_TEARDOWN, HTTPD_PROTO_RTSP },

                        { NULL,         HTTPD_MSG_NONE,     HTTPD_PROTO_NONE }
                    };
                    int  i;

                    p = NULL;
                    cl->query.i_type = HTTPD_MSG_NONE;

                    fprintf( stderr, "received new request=%s\n", cl->p_buffer);

                    for( i = 0; msg_type[i].name != NULL; i++ )
                    {
                        if( !strncmp( cl->p_buffer, msg_type[i].name,
                                      strlen( msg_type[i].name ) ) )
                        {
                            p = &cl->p_buffer[strlen(msg_type[i].name) + 1 ];
                            cl->query.i_type = msg_type[i].i_type;
                            if( cl->query.i_proto != msg_type[i].i_proto )
                            {
                                p = NULL;
                                cl->query.i_proto = HTTPD_PROTO_NONE;
                                cl->query.i_type = HTTPD_MSG_NONE;
                            }
                            break;
                        }
                    }
                    if( p == NULL )
                    {
                        if( strstr( cl->p_buffer, "HTTP/1." ) )
                        {
                            cl->query.i_proto = HTTPD_PROTO_HTTP;
                        }
                        else if( strstr( cl->p_buffer, "RTSP/1." ) )
                        {
                            cl->query.i_proto = HTTPD_PROTO_RTSP;
                        }
                    }
                    else
                    {
                        char *p2;
                        char *p3;

                        while( *p == ' ' )
                        {
                            p++;
                        }
                        p2 = strchr( p, ' ' );
                        if( p2 )
                        {
                            *p2++ = '\0';
                        }
                        if( !strncasecmp( p, "rtsp:", 5 ) )
                        {
                            /* for rtsp url, you have rtsp://localhost:port/path */
                            p += 5;
                            while( *p == '/' ) p++;
                            while( *p && *p != '/' ) p++;
                        }
                        cl->query.psz_url = strdup( p );
                        if( ( p3 = strchr( cl->query.psz_url, '?' ) )  )
                        {
                            *p3++ = '\0';
                            cl->query.psz_args = strdup( p3 );
                        }
                        if( p2 )
                        {
                            while( *p2 == ' ' )
                            {
                                p2++;
                            }
                            if( !strncasecmp( p2, "HTTP/1.", 7 ) )
                            {
                                cl->query.i_proto = HTTPD_PROTO_HTTP;
                                cl->query.i_version = atoi( p2+7 );
                            }
                            else if( !strncasecmp( p2, "RTSP/1.", 7 ) )
                            {
                                cl->query.i_proto = HTTPD_PROTO_RTSP;
                                cl->query.i_version = atoi( p2+7 );
                            }
                        }
                        p = p2;
                    }
                }
                if( p )
                {
                    p = strchr( p, '\n' );
                }
                if( p )
                {
                    while( *p == '\n' || *p == '\r' )
                    {
                        p++;
                    }
                    while( p && *p != '\0' )
                    {
                        char *line = p;
                        char *eol = p = strchr( p, '\n' );
                        char *colon;

                        while( eol && eol >= line && ( *eol == '\n' || *eol == '\r' ) )
                        {
                            *eol-- = '\0';
                        }

                        if( ( colon = strchr( line, ':' ) ) )
                        {
                            char *name;
                            char *value;

                            *colon++ = '\0';
                            while( *colon == ' ' )
                            {
                                colon++;
                            }
                            name = strdup( line );
                            value = strdup( colon );

                            TAB_APPEND( cl->query.i_name, cl->query.name, name );
                            TAB_APPEND( cl->query.i_value,cl->query.value,value);

                            if( !strcasecmp( name, "Content-Length" ) )
                            {
                                cl->query.i_body = atol( value );
                            }
                        }

                        if( p )
                        {
                            p++;
                            while( *p == '\n' || *p == '\r' )
                            {
                                p++;
                            }
                        }
                    }
                }
                if( cl->query.i_body > 0 )
                {
                    /* TODO Mhh, handle the case client will only send a request and close the connection
                     * to mark and of body (probably only RTSP) */
                    cl->query.p_body = malloc( cl->query.i_body );
                    cl->i_buffer = 0;
                }
                else
                {
                    cl->i_state = HTTPD_CLIENT_RECEIVE_DONE;
                }
            }
        }
    }

    /* check if the client is to be set to dead */
#if defined( WIN32 ) || defined( UNDER_CE )
    if( ( i_len < 0 && WSAGetLastError() != WSAEWOULDBLOCK ) || ( i_len == 0 ) )
#else
    if( ( i_len < 0 && errno != EAGAIN && errno != EINTR ) || ( i_len == 0 ) )
#endif
    {
        if( cl->query.i_proto != HTTPD_PROTO_NONE && cl->query.i_type != HTTPD_MSG_NONE )
        {
            /* connection closed -> end of data */
            if( cl->query.i_body > 0 )
            {
                cl->query.i_body = cl->i_buffer;
            }
            cl->i_state = HTTPD_CLIENT_RECEIVE_DONE;
        }
        else
        {
            cl->i_state = HTTPD_CLIENT_DEAD;
        }
    }
    cl->i_activity_date = mdate();

    /* XXX: for QT I have to disable timeout. Try to find why */
    if( cl->query.i_proto == HTTPD_PROTO_RTSP )
        cl->i_activity_timeout = 0;

    /* Debugging only */
    if( cl->i_state == HTTPD_CLIENT_RECEIVE_DONE )
    {
        int i;

        fprintf( stderr, "received new request\n" );
        fprintf( stderr, "  - proto=%s\n",
                 cl->query.i_proto == HTTPD_PROTO_HTTP ? "HTTP" : "RTSP" );
        fprintf( stderr, "  - version=%d\n", cl->query.i_version );
        fprintf( stderr, "  - msg=%d\n", cl->query.i_type );
        if( cl->query.i_type == HTTPD_MSG_ANSWER )
        {
            fprintf( stderr, "  - answer=%d '%s'\n", cl->query.i_status,
                     cl->query.psz_status );
        }
        else if( cl->query.i_type != HTTPD_MSG_NONE )
        {
            fprintf( stderr, "  - url=%s\n", cl->query.psz_url );
        }
        for( i = 0; i < cl->query.i_name; i++ )
        {
            fprintf( stderr, "  - option name='%s' value='%s'\n",
                     cl->query.name[i], cl->query.value[i] );
        }
    }
}


static void httpd_ClientSend( httpd_client_t *cl )
{
    int i;
    int i_len;

    if( cl->i_buffer < 0 )
    {
        /* We need to create the header */
        int i_size = 0;
        char *p;

        i_size = strlen( "HTTP/1.") + 10 + 10 +
                 strlen( cl->answer.psz_status ? cl->answer.psz_status : "" ) + 5;
        for( i = 0; i < cl->answer.i_name; i++ )
        {
            i_size += strlen( cl->answer.name[i] ) + 2 +
                      strlen( cl->answer.value[i] ) + 2;
        }

        if( cl->i_buffer_size < i_size )
        {
            cl->i_buffer_size = i_size;
            free( cl->p_buffer );
            cl->p_buffer = malloc( i_size );
        }
        p = cl->p_buffer;

        p += sprintf( p, "%s/1.%d %d %s\r\n",
                      cl->answer.i_proto ==  HTTPD_PROTO_HTTP ? "HTTP" : "RTSP",
                      cl->answer.i_version,
                      cl->answer.i_status, cl->answer.psz_status );
        for( i = 0; i < cl->answer.i_name; i++ )
        {
            p += sprintf( p, "%s: %s\r\n", cl->answer.name[i],
                          cl->answer.value[i] );
        }
        p += sprintf( p, "\r\n" );

        cl->i_buffer = 0;
        cl->i_buffer_size = (uint8_t*)p - cl->p_buffer;

        fprintf( stderr, "sending answer\n" );
        fprintf( stderr, "%s",  cl->p_buffer );
    }

    i_len = httpd_NetSend( cl, &cl->p_buffer[cl->i_buffer],
                           cl->i_buffer_size - cl->i_buffer );
    if( i_len > 0 )
    {
        cl->i_activity_date = mdate();
        cl->i_buffer += i_len;

        if( cl->i_buffer >= cl->i_buffer_size )
        {
            if( cl->answer.i_body == 0  && cl->answer.i_body_offset > 0 &&
                !cl->b_read_waiting )
            {
                /* catch more body data */
                int     i_msg = cl->query.i_type;
                int64_t i_offset = cl->answer.i_body_offset;

                httpd_MsgClean( &cl->answer );
                cl->answer.i_body_offset = i_offset;

                cl->url->catch[i_msg].cb( cl->url->catch[i_msg].p_sys, cl,
                                          &cl->answer, &cl->query );
            }

            if( cl->answer.i_body > 0 )
            {
                /* send the body data */
                free( cl->p_buffer );
                cl->p_buffer = cl->answer.p_body;
                cl->i_buffer_size = cl->answer.i_body;
                cl->i_buffer = 0;

                cl->answer.i_body = 0;
                cl->answer.p_body = NULL;
            }
            else
            {
                /* send finished */
                cl->i_state = HTTPD_CLIENT_SEND_DONE;
            }
        }
    }
    else
    {
#if defined( WIN32 ) || defined( UNDER_CE )
        if( ( i_len < 0 && WSAGetLastError() != WSAEWOULDBLOCK ) || ( i_len == 0 ) )
#else
        if( ( i_len < 0 && errno != EAGAIN && errno != EINTR ) || ( i_len == 0 ) )
#endif
        {
            /* error */
            cl->i_state = HTTPD_CLIENT_DEAD;
        }
    }
}

static void httpd_HostThread( httpd_host_t *host )
{
    tls_session_t *p_tls = NULL;

    while( !host->b_die )
    {
        struct timeval  timeout;
        fd_set          fds_read;
        fd_set          fds_write;
        int             i_handle_max = 0;
        int             i_ret;
        int             i_client;
        int             b_low_delay = 0;

        if( host->i_url <= 0 )
        {
            /* 0.2s */
            msleep( 200000 );
            continue;
        }

        /* built a set of handle to select */
        FD_ZERO( &fds_read );
        FD_ZERO( &fds_write );

        FD_SET( host->fd, &fds_read );
        i_handle_max = host->fd;

        /* prepare a new TLS session */
        if( ( p_tls == NULL ) && ( host->p_tls != NULL ) )
            p_tls = tls_ServerSessionPrepare( host->p_tls );

        /* add all socket that should be read/write and close dead connection */
        vlc_mutex_lock( &host->lock );
        for( i_client = 0; i_client < host->i_client; i_client++ )
        {
            httpd_client_t *cl = host->client[i_client];

            if( cl->i_ref < 0 || ( cl->i_ref == 0 &&
                ( cl->i_state == HTTPD_CLIENT_DEAD ||
                  ( cl->i_activity_timeout > 0 &&
                    cl->i_activity_date+cl->i_activity_timeout < mdate()) ) ) )
            {
                char *ip;

                // FIXME: it sucks to allocate memory on the stack for debug
                ip = httpd_ClientIP( cl );
                msg_Dbg( host, "connection closed(%s)",
                         (ip != NULL) ? ip : "unknown" );
                free( ip );

                httpd_ClientClean( cl );
                TAB_REMOVE( host->i_client, host->client, cl );
                free( cl );
                i_client--;
                continue;
            }
            else if( cl->i_state == HTTPD_CLIENT_RECEIVING )
            {
                FD_SET( cl->fd, &fds_read );
                i_handle_max = __MAX( i_handle_max, cl->fd );
            }
            else if( cl->i_state == HTTPD_CLIENT_SENDING )
            {
                FD_SET( cl->fd, &fds_write );
                i_handle_max = __MAX( i_handle_max, cl->fd );
            }
            else if( cl->i_state == HTTPD_CLIENT_RECEIVE_DONE )
            {
                httpd_message_t *answer = &cl->answer;
                httpd_message_t *query  = &cl->query;
                int i_msg = query->i_type;

                httpd_MsgInit( answer );

                /* Handle what we received */
                if( cl->i_mode != HTTPD_CLIENT_BIDIR &&
                    (i_msg == HTTPD_MSG_ANSWER || i_msg == HTTPD_MSG_CHANNEL) )
                {
                    /* we can only receive request from client when not
                     * in BIDIR mode */
                    cl->url     = NULL;
                    cl->i_state = HTTPD_CLIENT_DEAD;
                }
                else if( i_msg == HTTPD_MSG_ANSWER )
                {
                    /* We are in BIDIR mode, trigger the callback and then
                     * check for new data */
                    if( cl->url && cl->url->catch[i_msg].cb )
                    {
                        cl->url->catch[i_msg].cb( cl->url->catch[i_msg].p_sys,
                                                  cl, NULL, query );
                    }
                    cl->i_state = HTTPD_CLIENT_WAITING;
                }
                else if( i_msg == HTTPD_MSG_CHANNEL )
                {
                    /* We are in BIDIR mode, trigger the callback and then
                     * check for new data */
                    if( cl->url && cl->url->catch[i_msg].cb )
                    {
                        cl->url->catch[i_msg].cb( cl->url->catch[i_msg].p_sys,
                                                  cl, NULL, query );
                    }
                    cl->i_state = HTTPD_CLIENT_WAITING;
                }
                else if( i_msg == HTTPD_MSG_OPTIONS )
                {
                    int i_cseq;

                    /* unimplemented */
                    answer->i_proto  = query->i_proto ;
                    answer->i_type   = HTTPD_MSG_ANSWER;
                    answer->i_version= 0;
                    answer->i_status = 200;
                    answer->psz_status = strdup( "Ok" );

                    answer->i_body = 0;
                    answer->p_body = NULL;

                    i_cseq = atoi( httpd_MsgGet( query, "Cseq" ) );
                    httpd_MsgAdd( answer, "Cseq", "%d", i_cseq );
                    httpd_MsgAdd( answer, "Server", "VLC Server" );
                    httpd_MsgAdd( answer, "Public", "DESCRIBE, SETUP, "
                                 "TEARDOWN, PLAY, PAUSE" );
                    httpd_MsgAdd( answer, "Content-Length", "%d",
                                  answer->i_body );

                    cl->i_buffer = -1;  /* Force the creation of the answer in
                                         * httpd_ClientSend */
                    cl->i_state = HTTPD_CLIENT_SENDING;
                }
                else if( i_msg == HTTPD_MSG_NONE )
                {
                    if( query->i_proto == HTTPD_PROTO_NONE )
                    {
                        cl->url = NULL;
                        cl->i_state = HTTPD_CLIENT_DEAD;
                    }
                    else
                    {
                        uint8_t *p;

                        /* unimplemented */
                        answer->i_proto  = query->i_proto ;
                        answer->i_type   = HTTPD_MSG_ANSWER;
                        answer->i_version= 0;
                        answer->i_status = 501;
                        answer->psz_status = strdup( "Unimplemented" );

                        p = answer->p_body = malloc( 1000 );

                        p += sprintf( p, "<html>\n" );
                        p += sprintf( p, "<head>\n" );
                        p += sprintf( p, "<title>Error 501</title>\n" );
                        p += sprintf( p, "</head>\n" );
                        p += sprintf( p, "<body>\n" );
                        p += sprintf( p, "<h1><center> 501 Unimplemented</center></h1>\n" );
                        p += sprintf( p, "<hr />\n" );
                        p += sprintf( p, "<a href=\"http://www.videolan.org\">VideoLAN</a>\n" );
                        p += sprintf( p, "</body>\n" );
                        p += sprintf( p, "</html>\n" );

                        answer->i_body = p - answer->p_body;
                        httpd_MsgAdd( answer, "Content-Length", "%d", answer->i_body );

                        cl->i_buffer = -1;  /* Force the creation of the answer in httpd_ClientSend */
                        cl->i_state = HTTPD_CLIENT_SENDING;
                    }
                }
                else
                {
                    vlc_bool_t b_auth_failed = VLC_FALSE;
                    int i;

                    /* Search the url and trigger callbacks */
                    for( i = 0; i < host->i_url; i++ )
                    {
                        httpd_url_t *url = host->url[i];

                        if( !strcmp( url->psz_url, query->psz_url ) )
                        {
                            if( url->catch[i_msg].cb )
                            {
                                if( answer && ( *url->psz_user || *url->psz_password ) )
                                {
                                    /* create the headers */
                                    char *b64 = httpd_MsgGet( query, "Authorization" ); /* BASIC id */
                                    char *auth;
                                    char *id;

                                    asprintf( &id, "%s:%s", url->psz_user, url->psz_password );
                                    auth = malloc( strlen(b64) + 1 );

                                    if( !strncasecmp( b64, "BASIC", 5 ) )
                                    {
                                        b64 += 5;
                                        while( *b64 == ' ' )
                                        {
                                            b64++;
                                        }
                                        b64_decode( auth, b64 );
                                    }
                                    else
                                    {
                                        strcpy( auth, "" );
                                    }
                                    if( strcmp( id, auth ) )
                                    {
                                        httpd_MsgAdd( answer, "WWW-Authenticate", "Basic realm=\"%s\"", url->psz_user );
                                        /* We fail for all url */
                                        b_auth_failed = VLC_TRUE;
                                        free( id );
                                        free( auth );
                                        break;
                                    }

                                    free( id );
                                    free( auth );
                                }

                                if( !url->catch[i_msg].cb( url->catch[i_msg].p_sys, cl, answer, query ) )
                                {
                                    /* only one url can answer */
                                    answer = NULL;
                                    if( cl->url == NULL )
                                    {
                                        cl->url = url;
                                    }
                                }
                            }
                        }
                    }
                    if( answer )
                    {
                        uint8_t *p;

                        answer->i_proto  = query->i_proto;
                        answer->i_type   = HTTPD_MSG_ANSWER;
                        answer->i_version= 0;
                        p = answer->p_body = malloc( 1000 + strlen(query->psz_url) );

                        if( b_auth_failed )
                        {
                            answer->i_status = 401;
                            answer->psz_status = strdup( "Authorization Required" );

                            p += sprintf( p, "<html>\n" );
                            p += sprintf( p, "<head>\n" );
                            p += sprintf( p, "<title>Error 401</title>\n" );
                            p += sprintf( p, "</head>\n" );
                            p += sprintf( p, "<body>\n" );
                            p += sprintf( p, "<h1><center> 401 Authorization Required (%s)</center></h1>\n", query->psz_url );
                            p += sprintf( p, "<hr />\n" );
                            p += sprintf( p, "<a href=\"http://www.videolan.org\">VideoLAN</a>\n" );
                            p += sprintf( p, "</body>\n" );
                            p += sprintf( p, "</html>\n" );
                        }
                        else
                        {
                            /* no url registered */
                            answer->i_status = 404;
                            answer->psz_status = strdup( "Not found" );

                            p += sprintf( p, "<html>\n" );
                            p += sprintf( p, "<head>\n" );
                            p += sprintf( p, "<title>Error 404</title>\n" );
                            p += sprintf( p, "</head>\n" );
                            p += sprintf( p, "<body>\n" );
                            p += sprintf( p, "<h1><center> 404 Resource not found(%s)</center></h1>\n", query->psz_url );
                            p += sprintf( p, "<hr />\n" );
                            p += sprintf( p, "<a href=\"http://www.videolan.org\">VideoLAN</a>\n" );
                            p += sprintf( p, "</body>\n" );
                            p += sprintf( p, "</html>\n" );
                        }

                        answer->i_body = p - answer->p_body;
                        httpd_MsgAdd( answer, "Content-Length", "%d", answer->i_body );
                    }
                    cl->i_buffer = -1;  /* Force the creation of the answer in httpd_ClientSend */
                    cl->i_state = HTTPD_CLIENT_SENDING;
                }
            }
            else if( cl->i_state == HTTPD_CLIENT_SEND_DONE )
            {
                if( cl->i_mode == HTTPD_CLIENT_FILE || cl->answer.i_body_offset == 0 )
                {
                    cl->url = NULL;
                    if( ( cl->query.i_proto == HTTPD_PROTO_HTTP &&
                          ( ( cl->answer.i_version == 0 && !strcasecmp( httpd_MsgGet( &cl->answer, "Connection" ), "Keep-Alive" ) ) ||
                            ( cl->answer.i_version == 1 &&  strcasecmp( httpd_MsgGet( &cl->answer, "Connection" ), "Close" ) ) ) ) ||
                        ( cl->query.i_proto == HTTPD_PROTO_RTSP &&
                          strcasecmp( httpd_MsgGet( &cl->query, "Connection" ), "Close" ) &&
                          strcasecmp( httpd_MsgGet( &cl->answer, "Connection" ), "Close" ) ) )
                    {
                        httpd_MsgClean( &cl->query );
                        httpd_MsgInit( &cl->query );

                        cl->i_buffer = 0;
                        cl->i_buffer_size = 1000;
                        free( cl->p_buffer );
                        cl->p_buffer = malloc( cl->i_buffer_size );
                        cl->i_state = HTTPD_CLIENT_RECEIVING;
                    }
                    else
                    {
                        cl->i_state = HTTPD_CLIENT_DEAD;
                    }
                    httpd_MsgClean( &cl->answer );
                }
                else if( cl->b_read_waiting )
                {
                    /* we have a message waiting for us to read it */
                    httpd_MsgClean( &cl->answer );
                    httpd_MsgClean( &cl->query );

                    cl->i_buffer = 0;
                    cl->i_buffer_size = 1000;
                    free( cl->p_buffer );
                    cl->p_buffer = malloc( cl->i_buffer_size );
                    cl->i_state = HTTPD_CLIENT_RECEIVING;
                    cl->b_read_waiting = VLC_FALSE;
                }
                else
                {
                    int64_t i_offset = cl->answer.i_body_offset;
                    httpd_MsgClean( &cl->answer );

                    cl->answer.i_body_offset = i_offset;
                    free( cl->p_buffer );
                    cl->p_buffer = NULL;
                    cl->i_buffer = 0;
                    cl->i_buffer_size = 0;

                    cl->i_state = HTTPD_CLIENT_WAITING;
                }
            }
            else if( cl->i_state == HTTPD_CLIENT_WAITING )
            {
                int64_t i_offset = cl->answer.i_body_offset;
                int     i_msg = cl->query.i_type;

                httpd_MsgInit( &cl->answer );
                cl->answer.i_body_offset = i_offset;

                cl->url->catch[i_msg].cb( cl->url->catch[i_msg].p_sys, cl,
                                          &cl->answer, &cl->query );
                if( cl->answer.i_type != HTTPD_MSG_NONE )
                {
                    /* we have new data, so reenter send mode */
                    cl->i_buffer      = 0;
                    cl->p_buffer      = cl->answer.p_body;
                    cl->i_buffer_size = cl->answer.i_body;
                    cl->answer.p_body = NULL;
                    cl->answer.i_body = 0;
                    cl->i_state = HTTPD_CLIENT_SENDING;
                }
                else
                {
                    /* we shouldn't wait too long */
                    b_low_delay = VLC_TRUE;
                }
            }

            /* Special for BIDIR mode we also check reading */
            if( cl->i_mode == HTTPD_CLIENT_BIDIR &&
                cl->i_state == HTTPD_CLIENT_SENDING )
            {
                FD_SET( cl->fd, &fds_read );
                i_handle_max = __MAX( i_handle_max, cl->fd );
            }
        }
        vlc_mutex_unlock( &host->lock );

        /* we will wait 100ms or 20ms (not too big 'cause HTTPD_CLIENT_WAITING) */
        timeout.tv_sec = 0;
        timeout.tv_usec = b_low_delay ? 20000 : 100000;

        i_ret = select( i_handle_max + 1,
                        &fds_read, &fds_write, NULL, &timeout );

        if( i_ret == -1 && errno != EINTR )
        {
            msg_Warn( host, "cannot select sockets" );
            msleep( 1000 );
            continue;
        }
        else if( i_ret <= 0 )
        {
            continue;
        }

        /* accept new connections */
        if( FD_ISSET( host->fd, &fds_read ) )
        {
            int     i_sock_size = sizeof( struct sockaddr_storage );
            struct  sockaddr_storage sock;
            int     fd;

            fd = accept( host->fd, (struct sockaddr *)&sock, &i_sock_size );
            if( fd >= 0 )
            {
                if( p_tls != NULL)
                {
                    p_tls = tls_SessionHandshake( p_tls, fd );
                    if ( p_tls == NULL )
                    {
                        msg_Err( host, "Rejecting TLS connection" );
                        net_Close( fd );
                        fd = -1;
                    }
                }
                
                if( fd >= 0 )
                {
                    char *ip;
                    httpd_client_t *cl;

                    cl = httpd_ClientNew( fd, &sock, i_sock_size, p_tls );
                    p_tls = NULL;
                    vlc_mutex_lock( &host->lock );
                    TAB_APPEND( host->i_client, host->client, cl );
                    vlc_mutex_unlock( &host->lock );

    
                    // FIXME: it sucks to allocate memory for debug
                    ip = httpd_ClientIP( cl );
                    msg_Dbg( host, "new connection (%s)",
                             ip != NULL ? ip : "unknown" );
                    if( ip != NULL)
                        free( ip );
                }
            }
        }
        /* now try all others socket */
        vlc_mutex_lock( &host->lock );
        for( i_client = 0; i_client < host->i_client; i_client++ )
        {
            httpd_client_t *cl = host->client[i_client];
            if( cl->i_state == HTTPD_CLIENT_RECEIVING )
            {
                httpd_ClientRecv( cl );
            }
            else if( cl->i_state == HTTPD_CLIENT_SENDING )
            {
                httpd_ClientSend( cl );
            }

            if( cl->i_mode == HTTPD_CLIENT_BIDIR &&
                cl->i_state == HTTPD_CLIENT_SENDING &&
                FD_ISSET( cl->fd, &fds_read ) )
            {
                cl->b_read_waiting = VLC_TRUE;
            }
        }
        vlc_mutex_unlock( &host->lock );
    }
}

#ifndef HAVE_GETADDRINFO
static int BuildAddr( struct sockaddr_in * p_socket,
                      const char * psz_address, int i_port )
{
    /* Reset struct */
    memset( p_socket, 0, sizeof( struct sockaddr_in ) );
    p_socket->sin_family = AF_INET;                                /* family */
    p_socket->sin_port = htons( (uint16_t)i_port );
    if( !*psz_address )
    {
        p_socket->sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        struct hostent    * p_hostent;

        /* Try to convert address directly from in_addr - this will work if
         * psz_address is dotted decimal. */
#ifdef HAVE_ARPA_INET_H
        if( !inet_aton( psz_address, &p_socket->sin_addr ) )
#else
        p_socket->sin_addr.s_addr = inet_addr( psz_address );

/*        if( p_socket->sin_addr.s_addr == INADDR_NONE )*/
        if( p_socket->sin_addr.s_addr == INADDR_BROADCAST )
#endif
        {
            /* We have a fqdn, try to find its address */
            if ( (p_hostent = gethostbyname( psz_address )) == NULL )
            {
                return( -1 );
            }

            /* Copy the first address of the host in the socket address */
            memcpy( &((struct sockaddr_in *)p_socket)->sin_addr, p_hostent->h_addr_list[0],
                     p_hostent->h_length );
        }
    }
    return( 0 );
}
#endif

static int GetAddrPort( const struct sockaddr_storage *p_ss )
{
    int i_port = 0;

    switch (p_ss->ss_family)
    {
#ifdef AF_INET6
        case AF_INET6:
            i_port = ((const struct sockaddr_in6 *)p_ss)->sin6_port;
            break;
#endif

        case AF_INET:
            i_port = ((const struct sockaddr_in *)p_ss)->sin_port;
            break;
            
        default:
            return -1;
    }
    
    return ntohs( i_port );
}

#else /* ENABLE_HTTPD */

/* We just define an empty wrapper */
httpd_host_t *httpd_TLSHostNew( vlc_object_t *a, char *b, int c,
                                tls_server_t *d )
{
    msg_Err( a, "HTTP daemon support is disabled" );
    return 0;
}
httpd_host_t *httpd_HostNew( vlc_object_t *a, char *b, int c )
{
    msg_Err( a, "HTTP daemon support is disabled" );
    return 0;
}
void httpd_HostDelete( httpd_host_t *a ){}
httpd_url_t *httpd_UrlNew( httpd_host_t *a, char *b ){ return 0; }
httpd_url_t *httpd_UrlNewUnique( httpd_host_t *a, char *b, char *c,
                                 char *d ){ return 0; }
int httpd_UrlCatch( httpd_url_t *a, int b, httpd_callback_t c,
                    httpd_callback_sys_t *d ){ return 0; }
void httpd_UrlDelete( httpd_url_t *a ){}

char *httpd_ClientIP( httpd_client_t *a ){ return 0; }
void httpd_ClientModeStream( httpd_client_t *a ){}
void httpd_ClientModeBidir( httpd_client_t *a ){}

void httpd_FileDelete( httpd_file_t *a ){}
httpd_file_t *httpd_FileNew( httpd_host_t *a, char *b, char *c, char *d,
                             char *e, httpd_file_callback_t f,
                             httpd_file_sys_t *g ){ return 0; }

void httpd_RedirectDelete( httpd_redirect_t *a ){}
httpd_redirect_t *httpd_RedirectNew( httpd_host_t *a,
                                     char *b, char *c ){ return 0; }

void httpd_StreamDelete( httpd_stream_t *a ){}
int  httpd_StreamHeader( httpd_stream_t *a, uint8_t *b, int c ){ return 0; }
int  httpd_StreamSend  ( httpd_stream_t *a, uint8_t *b, int c ){ return 0; }
httpd_stream_t *httpd_StreamNew( httpd_host_t *a, char *b, char *c,
                                 char *d, char *e ){ return 0; }

void httpd_MsgInit ( httpd_message_t *a ){}
void httpd_MsgAdd  ( httpd_message_t *a, char *b, char *c, ... ){}
char *httpd_MsgGet ( httpd_message_t *a, char *b ){ return 0; }
void httpd_MsgClean( httpd_message_t *a ){}

#endif /* ENABLE_HTTPD */
