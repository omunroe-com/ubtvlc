/*****************************************************************************
 * sap.c : SAP announce handler
 *****************************************************************************
 * Copyright (C) 2002-2004 VideoLAN
 * $Id: sap.c 9201 2004-11-06 16:51:46Z yoann $
 *
 * Authors: Cl�ment Stenac <zorglub@videolan.org>
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
#include <stdlib.h>                                                /* free() */
#include <stdio.h>                                              /* sprintf() */
#include <string.h>                                            /* strerror() */

#include <vlc/vlc.h>
#include <vlc/sout.h>

#include <network.h>

#define SAP_IPV4_ADDR "224.2.127.254" /* Standard port and address for SAP */
#define SAP_PORT 9875

#define SAP_IPV6_ADDR_1 "FF0"
#define SAP_IPV6_ADDR_2 "::2:7FFE"

#define DEFAULT_IPV6_SCOPE '8'

#define DEFAULT_PORT "1234"

#undef EXTRA_DEBUG

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static void RunThread( vlc_object_t *p_this);
static int CalculateRate( sap_handler_t *p_sap, sap_address_t *p_address );
static int SDPGenerate( sap_handler_t *p_sap, session_descriptor_t *p_session );

static int announce_SendSAPAnnounce( sap_handler_t *p_sap,
                                     sap_session_t *p_session );


static int announce_SAPAnnounceAdd( sap_handler_t *p_sap,
                             session_descriptor_t *p_session,
                             announce_method_t *p_method );

static int announce_SAPAnnounceDel( sap_handler_t *p_sap,
                             session_descriptor_t *p_session );

#define FREE( p ) if( p ) { free( p ); (p) = NULL; }


/**
 * Create the SAP handler
 *
 * \param p_announce the parent announce_handler
 * \return the newly created SAP handler or NULL on error
 */
sap_handler_t *announce_SAPHandlerCreate( announce_handler_t *p_announce )
{
    sap_handler_t *p_sap;

    p_sap = vlc_object_create( p_announce, sizeof( sap_handler_t ) );

    if( !p_sap )
    {
        msg_Err( p_announce, "out of memory" );
        return NULL;
    }

    vlc_mutex_init( p_sap, &p_sap->object_lock );

    p_sap->pf_add = announce_SAPAnnounceAdd;
    p_sap->pf_del = announce_SAPAnnounceDel;

    p_sap->i_sessions = 0;
    p_sap->i_addresses = 0;
    p_sap->i_current_session = 0;

    p_sap->b_control = config_GetInt( p_sap, "sap-flow-control");

    if( vlc_thread_create( p_sap, "sap handler", RunThread,
                       VLC_THREAD_PRIORITY_LOW, VLC_FALSE ) )
    {
        msg_Dbg( p_announce, "Unable to spawn SAP handler thread");
        free( p_sap );
        return NULL;
    };
    msg_Dbg( p_announce, "thread created, %i sessions", p_sap->i_sessions);
    return p_sap;
}

/**
 *  Destroy the SAP handler
 *  \param p_this the SAP Handler to destroy
 *  \return nothing
 */
void announce_SAPHandlerDestroy( sap_handler_t *p_sap )
{
    int i;

    vlc_mutex_destroy( &p_sap->object_lock );

    /* Free the remaining sessions */
    for( i = 0 ; i< p_sap->i_sessions ; i++)
    {
        sap_session_t *p_session = p_sap->pp_sessions[i];
        FREE( p_session->psz_sdp );
        FREE( p_session->psz_data );
        REMOVE_ELEM( p_sap->pp_sessions, p_sap->i_sessions , i );
        FREE( p_session );
    }

    /* Free the remaining addresses */
    for( i = 0 ; i< p_sap->i_addresses ; i++)
    {
        sap_address_t *p_address = p_sap->pp_addresses[i];
        FREE( p_address->psz_address );
        if( p_address->i_rfd > -1 )
        {
            net_Close( p_address->i_rfd );
        }
        if( p_address->i_wfd > -1 && p_sap->b_control )
        {
            net_Close( p_address->i_wfd );
        }
        REMOVE_ELEM( p_sap->pp_addresses, p_sap->i_addresses, i );
        FREE( p_address );
    }

    /* Free the structure */
    vlc_object_destroy( p_sap );
}

/**
 * main SAP handler thread
 * \param p_this the SAP Handler object
 * \return nothing
 */
static void RunThread( vlc_object_t *p_this)
{
    sap_handler_t *p_sap = (sap_handler_t*)p_this;
    sap_session_t *p_session;

    while( !p_sap->b_die )
    {
        int i;

        /* If needed, get the rate info */
        if( p_sap->b_control == VLC_TRUE )
        {
            for( i = 0 ; i< p_sap->i_addresses ; i++)
            {
                if( p_sap->pp_addresses[i]->b_enabled == VLC_TRUE )
                {
                    CalculateRate( p_sap, p_sap->pp_addresses[i] );
                }
            }
        }

        /* Find the session to announce */
        vlc_mutex_lock( &p_sap->object_lock );
        if( p_sap->i_sessions > p_sap->i_current_session + 1)
        {
            p_sap->i_current_session++;
        }
        else if( p_sap->i_sessions > 0)
        {
            p_sap->i_current_session = 0;
        }
        else
        {
            vlc_mutex_unlock( &p_sap->object_lock );
            msleep( SAP_IDLE );
            continue;
        }
        p_session = p_sap->pp_sessions[p_sap->i_current_session];
        vlc_mutex_unlock( &p_sap->object_lock );

        /* And announce it */
        if( p_session->p_address->b_enabled == VLC_TRUE &&
            p_session->p_address->b_ready == VLC_TRUE )
        {
            announce_SendSAPAnnounce( p_sap, p_session );
        }

        msleep( SAP_IDLE );
    }
}

/* Add a SAP announce */
static int announce_SAPAnnounceAdd( sap_handler_t *p_sap,
                             session_descriptor_t *p_session,
                             announce_method_t *p_method )
{
    int i;
    char *psz_type = "application/sdp";
    int i_header_size;
    char *psz_head;
    vlc_bool_t b_found = VLC_FALSE;
    sap_session_t *p_sap_session;
    mtime_t i_hash;

    vlc_mutex_lock( &p_sap->object_lock );

    /* If needed, build the SDP */
    if( !p_session->psz_sdp )
    {
        if ( SDPGenerate( p_sap, p_session ) != VLC_SUCCESS )
        {
            vlc_mutex_unlock( &p_sap->object_lock );
            return VLC_EGENERIC;
        }
    }

    if( !p_method->psz_address )
    {
        if( p_method->i_ip_version == 6 )
        {
            char sz_scope;
            if( p_method->psz_ipv6_scope != NULL )
            {
                sz_scope = *p_method->psz_ipv6_scope;
            }
            else
            {
                sz_scope = DEFAULT_IPV6_SCOPE;
            }
            p_method->psz_address = (char*)malloc( 30*sizeof(char ));
            sprintf( p_method->psz_address, "%s%c%s",
                            SAP_IPV6_ADDR_1, sz_scope, SAP_IPV6_ADDR_2 );
        }
        else
        {
            /* IPv4 */
            p_method->psz_address = (char*)malloc( 15*sizeof(char) );
            snprintf(p_method->psz_address, 15, SAP_IPV4_ADDR );
        }
    }
    msg_Dbg( p_sap, "using SAP address: %s",p_method->psz_address);

    /* XXX: Check for dupes */
    p_sap_session = (sap_session_t*)malloc(sizeof(sap_session_t));

    p_sap_session->psz_sdp = strdup( p_session->psz_sdp );
    p_sap_session->i_last = 0;

    /* Add the address to the buffer */
    for( i = 0; i< p_sap->i_addresses; i++)
    {
        if( !strcmp( p_method->psz_address,
             p_sap->pp_addresses[i]->psz_address ) )
        {
            p_sap_session->p_address = p_sap->pp_addresses[i];
            b_found = VLC_TRUE;
            break;
        }
    }
    if( b_found == VLC_FALSE )
    {
        sap_address_t *p_address = (sap_address_t *)
                                    malloc( sizeof(sap_address_t) );
        if( !p_address )
        {
            msg_Err( p_sap, "out of memory" );
            return VLC_ENOMEM;
        }
        p_address->psz_address = strdup( p_method->psz_address );
        p_address->i_ip_version = p_method->i_ip_version;
        p_address->i_port  =  9875;
        p_address->i_wfd = net_OpenUDP( p_sap, "", 0,
                                        p_address->psz_address,
                                        p_address->i_port );

        if( p_sap->b_control == VLC_TRUE )
        {
            p_address->i_rfd = net_OpenUDP( p_sap, p_method->psz_address,
                                            p_address->i_port,
                                            "", 0 );
            p_address->i_buff = 0;
            p_address->b_enabled = VLC_TRUE;
            p_address->b_ready = VLC_FALSE;
            p_address->i_limit = 10000; /* 10000 bps */
            p_address->t1 = 0;
        }
        else
        {
            p_address->b_enabled = VLC_TRUE;
            p_address->b_ready = VLC_TRUE;
            p_address->i_interval = config_GetInt( p_sap,"sap-interval");
        }

        if( p_address->i_wfd == -1 || (p_address->i_rfd == -1
                                        && p_sap->b_control ) )
        {
            msg_Warn( p_sap, "disabling address" );
            p_address->b_enabled = VLC_FALSE;
        }

        INSERT_ELEM( p_sap->pp_addresses,
                     p_sap->i_addresses,
                     p_sap->i_addresses,
                     p_address );
        p_sap_session->p_address = p_address;
    }

    /* Build the SAP Headers */
    i_header_size = ( p_method->i_ip_version == 6 ? 20 : 8 ) + strlen( psz_type ) + 1;
    psz_head = (char *) malloc( i_header_size * sizeof( char ) );
    if( ! psz_head )
    {
        msg_Err( p_sap, "out of memory" );
        return VLC_ENOMEM;
    }

    psz_head[0] = 0x20; /* Means SAPv1, IPv4, not encrypted, not compressed */
    psz_head[1] = 0x00; /* No authentification length */

    i_hash = mdate();
    psz_head[2] = (i_hash & 0xFF00) >> 8; /* Msg id hash */
    psz_head[3] = (i_hash & 0xFF);        /* Msg id hash 2 */

    if( p_method->i_ip_version == 6 )
    {
        /* in_addr_t ip_server = inet_addr( ip ); */
        psz_head[0] |= 0x10; /* Set IPv6 */

        psz_head[4] = 0x01; /* Source IP  FIXME: we should get the real address */
        psz_head[5] = 0x02; /* idem */
        psz_head[6] = 0x03; /* idem */
        psz_head[7] = 0x04; /* idem */

        psz_head[8] = 0x01; /* Source IP  FIXME: we should get the real address */
        psz_head[9] = 0x02; /* idem */
        psz_head[10] = 0x03; /* idem */
        psz_head[11] = 0x04; /* idem */

        psz_head[12] = 0x01; /* Source IP  FIXME: we should get the real address */
        psz_head[13] = 0x02; /* idem */
        psz_head[14] = 0x03; /* idem */
        psz_head[15] = 0x04; /* idem */

        psz_head[16] = 0x01; /* Source IP  FIXME: we should get the real address */
        psz_head[17] = 0x02; /* idem */
        psz_head[18] = 0x03; /* idem */
        psz_head[19] = 0x04; /* idem */

        strncpy( psz_head + 20, psz_type, 15 );
    }
    else
    {
        /* in_addr_t ip_server = inet_addr( ip) */
        /* Source IP  FIXME: we should get the real address */
        psz_head[4] = 0x01; /* ip_server */
        psz_head[5] = 0x02; /* ip_server>>8 */
        psz_head[6] = 0x03; /* ip_server>>16 */
        psz_head[7] = 0x04; /* ip_server>>24 */

        strncpy( psz_head + 8, psz_type, 15 );
    }

    psz_head[ i_header_size-1 ] = '\0';
    p_sap_session->i_length = i_header_size + strlen( p_sap_session->psz_sdp);

    p_sap_session->psz_data = (char *)malloc( sizeof(char)*
                                              p_sap_session->i_length );

    /* Build the final message */
    memcpy( p_sap_session->psz_data, psz_head, i_header_size );
    memcpy( p_sap_session->psz_data+i_header_size, p_sap_session->psz_sdp,
            strlen( p_sap_session->psz_sdp) );

    free( psz_head );

    /* Enqueue the announce */
    INSERT_ELEM( p_sap->pp_sessions,
                 p_sap->i_sessions,
                 p_sap->i_sessions,
                 p_sap_session );
    msg_Dbg( p_sap,"Addresses: %i  Sessions: %i",
                   p_sap->i_addresses,p_sap->i_sessions);

    /* Remember the SAP session for later deletion */
    p_session->p_sap = p_sap_session;

    vlc_mutex_unlock( &p_sap->object_lock );

    return VLC_SUCCESS;
}

/* Remove a SAP Announce */
static int announce_SAPAnnounceDel( sap_handler_t *p_sap,
                             session_descriptor_t *p_session )
{
    int i;
    vlc_mutex_lock( &p_sap->object_lock );

    msg_Dbg( p_sap,"removing SAP announce %p",p_session->p_sap);

    /* Dequeue the announce */
    for( i = 0; i< p_sap->i_sessions; i++)
    {
        if( p_session->p_sap == p_sap->pp_sessions[i] )
        {
            REMOVE_ELEM( p_sap->pp_sessions,
                         p_sap->i_sessions,
                         i );
            break;
        }
    }

    /* XXX: Dequeue the address too if it is not used anymore
     * TODO: - address refcount
             - send a SAP deletion packet */

    msg_Dbg( p_sap,"%i announces remaining", p_sap->i_sessions );

    vlc_mutex_unlock( &p_sap->object_lock );

    return VLC_SUCCESS;
}

static int announce_SendSAPAnnounce( sap_handler_t *p_sap,
                                     sap_session_t *p_session )
{
    int i_ret;

    /* This announce has never been sent yet */
    if( p_session->i_last == 0 )
    {
        p_session->i_next = mdate()+ p_session->p_address->i_interval*1000000;
        p_session->i_last = 1;
        return VLC_SUCCESS;
    }

    if( p_session->i_next < mdate() )
    {
#ifdef EXTRA_DEBUG
        msg_Dbg( p_sap, "Sending announce");
#endif
        i_ret = net_Write( p_sap, p_session->p_address->i_wfd,
                           p_session->psz_data,
                           p_session->i_length );
        if( i_ret  != p_session->i_length )
        {
            msg_Warn( p_sap, "SAP send failed on address %s (%i %i)",
                   p_session->p_address->psz_address,
                   i_ret, p_session->i_length );
        }
        p_session->i_last = p_session->i_next;
        p_session->i_next = p_session->i_last
                            + p_session->p_address->i_interval*1000000;
    }
    else
    {
        return VLC_SUCCESS;
    }
    return VLC_SUCCESS;
}

static int SDPGenerate( sap_handler_t *p_sap, session_descriptor_t *p_session )
{
    int64_t i_sdp_id = mdate();
    int     i_sdp_version = 1 + p_sap->i_sessions + (rand()&0xfff);

    /* see the lists in modules/stream_out/rtp.c for compliance stuff */
    p_session->psz_sdp = (char *)malloc(
                            sizeof("v=0\r\n"
                                   "o=- 45383436098 45398  IN IP4 127.0.0.1\r\n" /* FIXME */
                                   "s=\r\n"
                                   "t=0 0\r\n"
                                   "c=IN IP4 /\r\n"
                                   "m=video  udp\r\n"
                                   "a=tool:"PACKAGE_STRING"\r\n"
                                   "a=type:broadcast\r\n")
                           + strlen( p_session->psz_name )
                           + strlen( p_session->psz_uri ) + 300
                           + ( p_session->psz_group ?
                                 strlen( p_session->psz_group ) : 0 ) );

    if( !p_session->psz_sdp )
    {
        msg_Err( p_sap, "out of memory" );
        return VLC_ENOMEM;
    }
    sprintf( p_session->psz_sdp,
                            "v=0\r\n"
                            "o=- "I64Fd" %d IN IP4 127.0.0.1\r\n"
                            "s=%s\r\n"
                            "t=0 0\r\n"
                            "c=IN IP4 %s/%d\r\n"
                            "m=video %d udp %d\r\n"
                            "a=tool:"PACKAGE_STRING"\r\n"
                            "a=type:broadcast\r\n",
                            i_sdp_id, i_sdp_version,
                            p_session->psz_name,
                            p_session->psz_uri, p_session->i_ttl,
                            p_session->i_port, p_session->i_payload );

    if( p_session->psz_group )
    {
        sprintf( p_session->psz_sdp, "%sa=x-plgroup:%s\r\n",
                                     p_session->psz_sdp,
                                     p_session->psz_group );
    }

    msg_Dbg( p_sap, "Generated SDP (%i bytes):\n%s", strlen(p_session->psz_sdp),
                    p_session->psz_sdp );
    return VLC_SUCCESS;
}

static int CalculateRate( sap_handler_t *p_sap, sap_address_t *p_address )
{
    int i_read;
    char buffer[SAP_MAX_BUFFER];
    int i_tot = 0;
    mtime_t i_temp;
    int i_rate;

    if( p_address->t1 == 0 )
    {
        p_address->t1 = mdate();
        return VLC_SUCCESS;
    }
    do
    {
        /* Might be too slow if we have huge data */
        i_read = net_ReadNonBlock( p_sap, p_address->i_rfd, buffer,
                                   SAP_MAX_BUFFER, 0 );
        i_tot += i_read;
    } while( i_read > 0 && i_tot < SAP_MAX_BUFFER );

    i_temp = mdate();

    /* We calculate the rate every 5 seconds */
    if( i_temp - p_address->t1 < 5000000 )
    {
        p_address->i_buff += i_tot;
        return VLC_SUCCESS;
    }

    /* Bits/second */
    i_rate = (int)(8*1000000*((mtime_t)p_address->i_buff + (mtime_t)i_tot ) /
                        (i_temp - p_address->t1 ));

    p_address->i_limit = 10000;

    p_address->i_interval = ((1000*i_rate / p_address->i_limit) *
                            (MAX_INTERVAL - MIN_INTERVAL))/1000 + MIN_INTERVAL;

    if( p_address->i_interval > MAX_INTERVAL || p_address->i_interval < 0 )
    {
        p_address->i_interval = MAX_INTERVAL;
    }
#ifdef EXTRA_DEBUG
    msg_Dbg( p_sap,"%s:%i : Rate=%i, Interval = %i s",
                    p_address->psz_address,p_address->i_port,
                    i_rate,
                    p_address->i_interval );
#endif

    p_address->b_ready = VLC_TRUE;

    p_address->t1 = i_temp;
    p_address->i_buff = 0;

    return VLC_SUCCESS;
}
