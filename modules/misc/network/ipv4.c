/*****************************************************************************
 * ipv4.c: IPv4 network abstraction layer
 *****************************************************************************
 * Copyright (C) 2001, 2002 VideoLAN
 * $Id: ipv4.c 7687 2004-05-16 17:44:44Z gbazin $
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *          Mathias Kretschmer <mathias@research.att.com>
 *          Alexis de Lattre <alexis@via.ecp.fr>
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
#include <string.h>
#include <vlc/vlc.h>

#ifdef HAVE_SYS_TYPES_H
#   include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#   include <sys/stat.h>
#endif
#ifdef HAVE_ERRNO_H
#   include <errno.h>
#endif
#ifdef HAVE_FCNTL_H
#   include <fcntl.h>
#endif

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif

#if defined( UNDER_CE )
#   include <winsock.h>
#elif defined( WIN32 )
#   include <winsock2.h>
#   include <ws2tcpip.h>
#   define close closesocket
#else
#   include <netdb.h>                                         /* hostent ... */
#   include <sys/socket.h>
#   include <netinet/in.h>
#   ifdef HAVE_ARPA_INET_H
#       include <arpa/inet.h>                    /* inet_ntoa(), inet_aton() */
#   endif
#endif

#include "network.h"

#ifndef INADDR_ANY
#   define INADDR_ANY  0x00000000
#endif
#ifndef INADDR_NONE
#   define INADDR_NONE 0xFFFFFFFF
#endif
#ifndef IN_MULTICAST
#   define IN_MULTICAST(a) IN_CLASSD(a)
#endif


/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int NetOpen( vlc_object_t * );

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define TIMEOUT_TEXT N_("TCP connection timeout in ms")
#define TIMEOUT_LONGTEXT N_( \
    "Allows you to modify the default TCP connection timeout. This " \
    "value should be set in millisecond units." )

vlc_module_begin();
    set_description( _("IPv4 network abstraction layer") );
    set_capability( "network", 50 );
    set_callbacks( NetOpen, NULL );

    add_integer( "ipv4-timeout", 5 * 1000, NULL, TIMEOUT_TEXT,
                 TIMEOUT_LONGTEXT, VLC_TRUE );
vlc_module_end();

/*****************************************************************************
 * BuildAddr: utility function to build a struct sockaddr_in
 *****************************************************************************/
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
        if( p_socket->sin_addr.s_addr == INADDR_NONE )
#endif
        {
            /* We have a fqdn, try to find its address */
            if ( (p_hostent = gethostbyname( psz_address )) == NULL )
            {
                return( -1 );
            }

            /* Copy the first address of the host in the socket address */
            memcpy( &p_socket->sin_addr, p_hostent->h_addr_list[0],
                     p_hostent->h_length );
        }
    }
    return( 0 );
}

/*****************************************************************************
 * OpenUDP: open a UDP socket
 *****************************************************************************
 * psz_bind_addr, i_bind_port : address and port used for the bind()
 *   system call. If psz_bind_addr == "", the socket is bound to
 *   INADDR_ANY and broadcast reception is enabled. If i_bind_port == 0,
 *   1234 is used. If psz_bind_addr is a multicast (class D) address,
 *   join the multicast group.
 * psz_server_addr, i_server_port : address and port used for the connect()
 *   system call. It can avoid receiving packets from unauthorized IPs.
 *   Its use leads to great confusion and is currently discouraged.
 * This function returns -1 in case of error.
 *****************************************************************************/
static int OpenUDP( vlc_object_t * p_this, network_socket_t * p_socket )
{
    char * psz_bind_addr = p_socket->psz_bind_addr;
    int i_bind_port = p_socket->i_bind_port;
    char * psz_server_addr = p_socket->psz_server_addr;
    int i_server_port = p_socket->i_server_port;

    int i_handle, i_opt;
    socklen_t i_opt_size;
    struct sockaddr_in sock;
    vlc_value_t val;

    /* If IP_ADD_SOURCE_MEMBERSHIP is not defined in the headers
       (because it's not in glibc for example), we have to define the
       headers required for IGMPv3 here */
#ifndef IP_ADD_SOURCE_MEMBERSHIP
    #define IP_ADD_SOURCE_MEMBERSHIP  39
    struct ip_mreq_source {
        struct in_addr  imr_multiaddr;
        struct in_addr  imr_interface;
        struct in_addr  imr_sourceaddr;
     };
#endif

    /* Open a SOCK_DGRAM (UDP) socket, in the AF_INET domain, automatic (0)
     * protocol */
    if( (i_handle = socket( AF_INET, SOCK_DGRAM, 0 )) == -1 )
    {
#ifdef HAVE_ERRNO_H
        msg_Warn( p_this, "cannot create socket (%s)", strerror(errno) );
#else
        msg_Warn( p_this, "cannot create socket" );
#endif
        return( -1 );
    }

    /* We may want to reuse an already used socket */
    i_opt = 1;
    if( setsockopt( i_handle, SOL_SOCKET, SO_REUSEADDR,
                    (void *) &i_opt, sizeof( i_opt ) ) == -1 )
    {
#ifdef HAVE_ERRNO_H
        msg_Warn( p_this, "cannot configure socket (SO_REUSEADDR: %s)",
                          strerror(errno));
#else
        msg_Warn( p_this, "cannot configure socket (SO_REUSEADDR)" );
#endif
        close( i_handle );
        return( -1 );
    }

    /* Increase the receive buffer size to 1/2MB (8Mb/s during 1/2s) to avoid
     * packet loss caused by scheduling problems */
    i_opt = 0x80000;
#if !defined( SYS_BEOS )
    if( setsockopt( i_handle, SOL_SOCKET, SO_RCVBUF, (void *) &i_opt, sizeof( i_opt ) ) == -1 )
    {
#ifdef HAVE_ERRNO_H
        msg_Dbg( p_this, "cannot configure socket (SO_RCVBUF: %s)",
                          strerror(errno));
#else
        msg_Warn( p_this, "cannot configure socket (SO_RCVBUF)" );
#endif
    }
#endif

#if !defined( SYS_BEOS )
    /* Check if we really got what we have asked for, because Linux, etc.
     * will silently limit the max buffer size to net.core.rmem_max which
     * is typically only 65535 bytes */
    i_opt = 0;
    i_opt_size = sizeof( i_opt );
    if( getsockopt( i_handle, SOL_SOCKET, SO_RCVBUF, (void*) &i_opt, &i_opt_size ) == -1 )
    {
#ifdef HAVE_ERRNO_H
        msg_Warn( p_this, "cannot query socket (SO_RCVBUF: %s)",
                          strerror(errno) );
#else
        msg_Warn( p_this, "cannot query socket (SO_RCVBUF)" );
#endif
    }
    else if( i_opt < 0x80000 )
    {
        msg_Dbg( p_this, "socket buffer size is 0x%x instead of 0x%x",
                         i_opt, 0x80000 );
    }
#endif


    /* Build the local socket */

#if defined( WIN32 ) && !defined( UNDER_CE )
    /* Under Win32 and for multicasting, we bind to INADDR_ANY,
     * so let's call BuildAddr with "" instead of psz_bind_addr */
    if( BuildAddr( &sock, IN_MULTICAST( ntohl( inet_addr(psz_bind_addr) ) ) ?
                   "" : psz_bind_addr, i_bind_port ) == -1 )
#else
    if( BuildAddr( &sock, psz_bind_addr, i_bind_port ) == -1 )
#endif
    {
        msg_Dbg( p_this, "could not build local address" );
        close( i_handle );
        return( -1 );
    }

    /* Bind it */
    if( bind( i_handle, (struct sockaddr *)&sock, sizeof( sock ) ) < 0 )
    {
#ifdef HAVE_ERRNO_H
        msg_Warn( p_this, "cannot bind socket (%s)", strerror(errno) );
#else
        msg_Warn( p_this, "cannot bind socket" );
#endif
        close( i_handle );
        return( -1 );
    }

#if defined( WIN32 ) && !defined( UNDER_CE )
    /* Restore the sock struct so we can spare a few #ifdef WIN32 later on */
    if( IN_MULTICAST( ntohl( inet_addr(psz_bind_addr) ) ) )
    {
        if ( BuildAddr( &sock, psz_bind_addr, i_bind_port ) == -1 )
        {
            msg_Dbg( p_this, "could not build local address" );
            close( i_handle );
            return( -1 );
        }
    }
#endif

#if !defined( SYS_BEOS )
    /* Allow broadcast reception if we bound on INADDR_ANY */
    if( !*psz_bind_addr )
    {
        i_opt = 1;
        if( setsockopt( i_handle, SOL_SOCKET, SO_BROADCAST, (void*) &i_opt, sizeof( i_opt ) ) == -1 )
        {
#ifdef HAVE_ERRNO_H
            msg_Warn( p_this, "cannot configure socket (SO_BROADCAST: %s)",
                       strerror(errno) );
#else
            msg_Warn( p_this, "cannot configure socket (SO_BROADCAST)" );
#endif
        }
    }
#endif

#if !defined( UNDER_CE ) && !defined( SYS_BEOS )
    /* Join the multicast group if the socket is a multicast address */
    if( IN_MULTICAST( ntohl(sock.sin_addr.s_addr) ) )
    {
        /* Determine interface to be used for multicast */
        char * psz_if_addr = config_GetPsz( p_this, "iface-addr" );

        /* If we have a source address, we use IP_ADD_SOURCE_MEMBERSHIP
           so that IGMPv3 aware OSes running on IGMPv3 aware networks
           will do an IGMPv3 query on the network */
        if( *psz_server_addr )
        {
            struct ip_mreq_source imr;

            imr.imr_multiaddr.s_addr = sock.sin_addr.s_addr;
            imr.imr_sourceaddr.s_addr = inet_addr(psz_server_addr);

            if( psz_if_addr != NULL && *psz_if_addr
                && inet_addr(psz_if_addr) != INADDR_NONE )
            {
                imr.imr_interface.s_addr = inet_addr(psz_if_addr);
            }
            else
            {
                imr.imr_interface.s_addr = INADDR_ANY;
            }
            if( psz_if_addr != NULL ) free( psz_if_addr );

            msg_Dbg( p_this, "IP_ADD_SOURCE_MEMBERSHIP multicast request" );
            /* Join Multicast group with source filter */
            if( setsockopt( i_handle, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP,
                         (char*)&imr,
                         sizeof(struct ip_mreq_source) ) == -1 )
            {
#ifdef HAVE_ERRNO_H
                msg_Err( p_this, "failed to join IP multicast group (%s)",
                                  strerror(errno) );
                msg_Err( p_this, "are you sure your OS supports IGMPv3?" );
#else
                msg_Err( p_this, "failed to join IP multicast group" );
                msg_Err( p_this, "are you sure your OS supports IGMPv3?" );
#endif
                close( i_handle );
                return( -1 );
            }
         }
         /* If there is no source address, we use IP_ADD_MEMBERSHIP */
         else
         {
             struct ip_mreq imr;

             imr.imr_multiaddr.s_addr = sock.sin_addr.s_addr;
             if( psz_if_addr != NULL && *psz_if_addr
                && inet_addr(psz_if_addr) != INADDR_NONE )
            {
                imr.imr_interface.s_addr = inet_addr(psz_if_addr);
            }
            else
            {
                imr.imr_interface.s_addr = INADDR_ANY;
            }
            if( psz_if_addr != NULL ) free( psz_if_addr );

            msg_Dbg( p_this, "IP_ADD_MEMBERSHIP multicast request" );
            /* Join Multicast group without source filter */
            if( setsockopt( i_handle, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                            (char*)&imr, sizeof(struct ip_mreq) ) == -1 )
            {
#ifdef HAVE_ERRNO_H
                msg_Err( p_this, "failed to join IP multicast group (%s)",
                                  strerror(errno) );
#else
                msg_Err( p_this, "failed to join IP multicast group" );
#endif
                close( i_handle );
                return( -1 );
            }
         }
    }
#endif

    if( *psz_server_addr )
    {
        /* Build socket for remote connection */
        if ( BuildAddr( &sock, psz_server_addr, i_server_port ) == -1 )
        {
            msg_Warn( p_this, "cannot build remote address" );
            close( i_handle );
            return( -1 );
        }

        /* Connect the socket */
        if( connect( i_handle, (struct sockaddr *) &sock,
                     sizeof( sock ) ) == (-1) )
        {
#ifdef HAVE_ERRNO_H
            msg_Warn( p_this, "cannot connect socket (%s)", strerror(errno) );
#else
            msg_Warn( p_this, "cannot connect socket" );
#endif
            close( i_handle );
            return( -1 );
        }

#if !defined( UNDER_CE ) && !defined( SYS_BEOS )
        if( IN_MULTICAST( ntohl(inet_addr(psz_server_addr) ) ) )
        {
            /* set the time-to-live */
            int ttl = p_socket->i_ttl;
            if( ttl < 1 )
            {
                ttl = config_GetInt( p_this, "ttl" );
            }
            if( ttl < 1 ) ttl = 1;

            if( setsockopt( i_handle, IPPROTO_IP, IP_MULTICAST_TTL,
                            (void *) &ttl, sizeof( ttl ) ) < 0 )
            {
#ifdef HAVE_ERRNO_H
                msg_Err( p_this, "failed to set ttl (%s)", strerror(errno) );
#else
                msg_Err( p_this, "failed to set ttl" );
#endif
                close( i_handle );
                return( -1 );
            }
        }
#endif
    }

    p_socket->i_handle = i_handle;

    var_Create( p_this, "mtu", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Get( p_this, "mtu", &val );
    p_socket->i_mtu = val.i_int;
    return( 0 );
}

/*****************************************************************************
 * OpenTCP: open a TCP socket
 *****************************************************************************
 * psz_server_addr, i_server_port : address and port used for the connect()
 *   system call. If i_server_port == 0, 80 is used.
 * Other parameters are ignored.
 * This function returns -1 in case of error.
 *****************************************************************************/
static int OpenTCP( vlc_object_t * p_this, network_socket_t * p_socket )
{
    char * psz_server_addr = p_socket->psz_server_addr;
    int i_server_port = p_socket->i_server_port;

    int i_handle;
    struct sockaddr_in sock;

    if( i_server_port == 0 )
    {
        i_server_port = 80;
    }

    /* Open a SOCK_STREAM (TCP) socket, in the AF_INET domain, automatic (0)
     * protocol */
    if( (i_handle = socket( AF_INET, SOCK_STREAM, 0 )) == -1 )
    {
#ifdef HAVE_ERRNO_H
        msg_Warn( p_this, "cannot create socket (%s)", strerror(errno) );
#else
        msg_Warn( p_this, "cannot create socket" );
#endif
        goto error;
    }

    /* Build remote address */
    if ( BuildAddr( &sock, psz_server_addr, i_server_port ) == -1 )
    {
        msg_Dbg( p_this, "could not build local address" );
        goto error;
    }

    /* Set to non-blocking */
#if defined( WIN32 ) || defined( UNDER_CE )
    {
        unsigned long i_dummy = 1;
        if( ioctlsocket( i_handle, FIONBIO, &i_dummy ) != 0 )
        {
            msg_Err( p_this, "cannot set socket to non-blocking mode" );
        }
    }
#elif defined( HAVE_ERRNO_H )
    {
        int i_flags;
        if( ( i_flags = fcntl( i_handle, F_GETFL, 0 ) ) < 0 ||
            fcntl( i_handle, F_SETFL, i_flags | O_NONBLOCK ) < 0 )
        {
            msg_Err( p_this, "cannot set socket to non-blocking mode" );
        }
    }
#endif

    /* Connect the socket */
    if( connect( i_handle, (struct sockaddr *) &sock, sizeof( sock ) ) == -1 )
    {
#if defined( WIN32 ) || defined( UNDER_CE )
        if( WSAGetLastError() == WSAEWOULDBLOCK )
#elif defined( HAVE_ERRNO_H )
        if( errno == EINPROGRESS )
#else
        if( 0 )
#endif
        {
            int i_ret, i_opt, i_opt_size = sizeof( i_opt ), i_max_count;
            struct timeval timeout;
            vlc_value_t val;
            fd_set fds;

            if( !var_Type( p_this, "ipv4-timeout" ) )
            {
                var_Create( p_this, "ipv4-timeout",
                            VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
            }
            var_Get( p_this, "ipv4-timeout", &val );
            i_max_count = val.i_int * 1000 / 100000 /* timeout.tv_usec */;

            msg_Dbg( p_this, "connection in progress" );
            do
            {
                if( p_this->b_die || i_max_count <= 0 )
                {
                    msg_Dbg( p_this, "connection aborted" );
                    goto error;
                }

                i_max_count--;

                /* Initialize file descriptor set */
                FD_ZERO( &fds );
                FD_SET( i_handle, &fds );

                /* We'll wait 0.1 second if nothing happens */
                timeout.tv_sec = 0;
                timeout.tv_usec = 100000;

            } while( ( i_ret = select( i_handle + 1, NULL, &fds, NULL,
                                       &timeout ) ) == 0 ||
#if defined( WIN32 ) || defined( UNDER_CE )
                     ( i_ret < 0 && WSAGetLastError() == WSAEWOULDBLOCK ) );
#elif defined( HAVE_ERRNO_H )
                     ( i_ret < 0 && errno == EINTR ) );
#else
                     ( i_ret < 0 ) );
#endif

            if( i_ret < 0 )
            {
                msg_Warn( p_this, "cannot connect socket (select failed)" );
                goto error;
            }

#if !defined( SYS_BEOS )
            if( getsockopt( i_handle, SOL_SOCKET, SO_ERROR, (void*)&i_opt,
                            &i_opt_size ) == -1 || i_opt != 0 )
            {
                msg_Warn( p_this, "cannot connect socket (SO_ERROR)" );
                goto error;
            }
#endif
        }
        else
        {
#if defined( HAVE_ERRNO_H )
            msg_Warn( p_this, "cannot connect socket (%s)", strerror(errno) );
#else
            msg_Warn( p_this, "cannot connect socket" );
#endif
            goto error;
        }
    }

    p_socket->i_handle = i_handle;
    p_socket->i_mtu = 0; /* There is no MTU notion in TCP */
    return VLC_SUCCESS;

error:
    if( i_handle > 0 )
    {
        close( i_handle );
    }
    return VLC_EGENERIC;
}

/*****************************************************************************
 * NetOpen: wrapper around OpenUDP and OpenTCP
 *****************************************************************************/
static int NetOpen( vlc_object_t * p_this )
{
    network_socket_t * p_socket = p_this->p_private;

    if( p_socket->i_type == NETWORK_UDP )
    {
        return OpenUDP( p_this, p_socket );
    }
    else
    {
        return OpenTCP( p_this, p_socket );
    }
}
