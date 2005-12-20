/*****************************************************************************
 * udp.c: raw UDP & RTP input module
 *****************************************************************************
 * Copyright (C) 2001-2004 VideoLAN
 * $Id: udp.c 8606 2004-08-31 18:32:54Z hartman $
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *          Tristan Leteurtre <tooney@via.ecp.fr>
 *          Laurent Aimar <fenrir@via.ecp.fr>
 *
 * Reviewed: 23 October 2003, Jean-Paul Saman <jpsaman@wxs.nl>
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

#include "network.h"

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define CACHING_TEXT N_("Caching value in ms")
#define CACHING_LONGTEXT N_( \
    "Allows you to modify the default caching value for UDP streams. This " \
    "value should be set in millisecond units." )

#define AUTO_MTU_TEXT N_("Autodetection of MTU")
#define AUTO_MTU_LONGTEXT N_( \
    "Allows growing the MTU if truncated packets are found" )

static int  Open ( vlc_object_t * );
static void Close( vlc_object_t * );

vlc_module_begin();
    set_description( _("UDP/RTP input") );

    add_integer( "udp-caching", DEFAULT_PTS_DELAY / 1000, NULL, CACHING_TEXT,
                 CACHING_LONGTEXT, VLC_TRUE );
    add_bool( "udp-auto-mtu", 1, NULL,
              AUTO_MTU_TEXT, AUTO_MTU_LONGTEXT, VLC_TRUE );

    set_capability( "access2", 0 );
    add_shortcut( "udp" );
    add_shortcut( "udpstream" );
    add_shortcut( "udp4" );
    add_shortcut( "udp6" );
    add_shortcut( "rtp" );
    add_shortcut( "rtp4" );
    add_shortcut( "rtp6" );
    set_callbacks( Open, Close );
vlc_module_end();

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
#define RTP_HEADER_LEN 12

static block_t *BlockUDP( access_t * );
static block_t *BlockRTP( access_t * );
static block_t *BlockChoose( access_t * );
static int Control( access_t *, int, va_list );

struct access_sys_t
{
    int fd;

    int i_mtu;
    vlc_bool_t b_auto_mtu;
};

/*****************************************************************************
 * Open: open the socket
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    access_t     *p_access = (access_t*)p_this;
    access_sys_t *p_sys;

    char *psz_name = strdup( p_access->psz_path );
    char *psz_parser = psz_name;
    char *psz_server_addr = "";
    char *psz_server_port = "";
    char *psz_bind_addr = "";
    char *psz_bind_port = "";
    int  i_bind_port = 0;
    int  i_server_port = 0;


    /* First set ipv4/ipv6 */
    var_Create( p_access, "ipv4", VLC_VAR_BOOL | VLC_VAR_DOINHERIT );
    var_Create( p_access, "ipv6", VLC_VAR_BOOL | VLC_VAR_DOINHERIT );

    if( *p_access->psz_access )
    {
        vlc_value_t val;
        /* Find out which shortcut was used */
        if( !strncmp( p_access->psz_access, "udp4", 6 ) ||
            !strncmp( p_access->psz_access, "rtp4", 6 ))
        {
            val.b_bool = VLC_TRUE;
            var_Set( p_access, "ipv4", val );

            val.b_bool = VLC_FALSE;
            var_Set( p_access, "ipv6", val );
        }
        else if( !strncmp( p_access->psz_access, "udp6", 6 ) ||
                 !strncmp( p_access->psz_access, "rtp6", 6 ) )
        {
            val.b_bool = VLC_TRUE;
            var_Set( p_access, "ipv6", val );

            val.b_bool = VLC_FALSE;
            var_Set( p_access, "ipv4", val );
        }
    }

    /* Parse psz_name syntax :
     * [serveraddr[:serverport]][@[bindaddr]:[bindport]] */
    if( *psz_parser && *psz_parser != '@' )
    {
        /* Found server */
        psz_server_addr = psz_parser;

        while( *psz_parser && *psz_parser != ':' && *psz_parser != '@' )
        {
            if( *psz_parser == '[' )
            {
                /* IPv6 address */
                while( *psz_parser && *psz_parser != ']' )
                {
                    psz_parser++;
                }
            }
            psz_parser++;
        }

        if( *psz_parser == ':' )
        {
            /* Found server port */
            *psz_parser++ = '\0'; /* Terminate server name */
            psz_server_port = psz_parser;

            while( *psz_parser && *psz_parser != '@' )
            {
                psz_parser++;
            }
        }
    }

    if( *psz_parser == '@' )
    {
        /* Found bind address or bind port */
        *psz_parser++ = '\0'; /* Terminate server port or name if necessary */

        if( *psz_parser && *psz_parser != ':' )
        {
            /* Found bind address */
            psz_bind_addr = psz_parser;

            while( *psz_parser && *psz_parser != ':' )
            {
                if( *psz_parser == '[' )
                {
                    /* IPv6 address */
                    while( *psz_parser && *psz_parser != ']' )
                    {
                        psz_parser++;
                    }
                }
                psz_parser++;
            }
        }

        if( *psz_parser == ':' )
        {
            /* Found bind port */
            *psz_parser++ = '\0'; /* Terminate bind address if necessary */
            psz_bind_port = psz_parser;
        }
    }

    i_server_port = strtol( psz_server_port, NULL, 10 );
    if( ( i_bind_port   = strtol( psz_bind_port,   NULL, 10 ) ) == 0 )
    {
        i_bind_port = var_CreateGetInteger( p_access, "server-port" );
    }

    msg_Dbg( p_access, "opening server=%s:%d local=%s:%d",
             psz_server_addr, i_server_port, psz_bind_addr, i_bind_port );

    /* Set up p_access */
    p_access->pf_read = NULL;
    p_access->pf_block = BlockChoose;
    p_access->pf_control = Control;
    p_access->pf_seek = NULL;
    p_access->info.i_update = 0;
    p_access->info.i_size = 0;
    p_access->info.i_pos = 0;
    p_access->info.b_eof = VLC_FALSE;
    p_access->info.i_title = 0;
    p_access->info.i_seekpoint = 0;

    p_access->p_sys = p_sys = malloc( sizeof( access_sys_t ) );
    p_sys->fd = net_OpenUDP( p_access, psz_bind_addr, i_bind_port,
                                      psz_server_addr, i_server_port );
    if( p_sys->fd < 0 )
    {
        msg_Err( p_access, "cannot open socket" );
        free( psz_name );
        free( p_sys );
        return VLC_EGENERIC;
    }
    free( psz_name );

    /* FIXME */
    p_sys->i_mtu = var_CreateGetInteger( p_access, "mtu" );
    if( p_sys->i_mtu <= 1 )
        p_sys->i_mtu  = 1500;   /* Avoid problem */

    p_sys->b_auto_mtu = var_CreateGetBool( p_access, "udp-auto-mtu" );;

    /* Update default_pts to a suitable value for udp access */
    var_Create( p_access, "udp-caching", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close: free unused data structures
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    access_t     *p_access = (access_t*)p_this;
    access_sys_t *p_sys = p_access->p_sys;

    net_Close( p_sys->fd );
    free( p_sys );
}

/*****************************************************************************
 * Control:
 *****************************************************************************/
static int Control( access_t *p_access, int i_query, va_list args )
{
    access_sys_t *p_sys = p_access->p_sys;
    vlc_bool_t   *pb_bool;
    int          *pi_int;
    int64_t      *pi_64;

    switch( i_query )
    {
        /* */
        case ACCESS_CAN_SEEK:
        case ACCESS_CAN_FASTSEEK:
        case ACCESS_CAN_PAUSE:
        case ACCESS_CAN_CONTROL_PACE:
            pb_bool = (vlc_bool_t*)va_arg( args, vlc_bool_t* );
            *pb_bool = VLC_FALSE;
            break;
        /* */
        case ACCESS_GET_MTU:
            pi_int = (int*)va_arg( args, int * );
            *pi_int = p_sys->i_mtu;
            break;

        case ACCESS_GET_PTS_DELAY:
            pi_64 = (int64_t*)va_arg( args, int64_t * );
            *pi_64 = var_GetInteger( p_access, "udp-caching" ) * 1000;
            break;

        /* */
        case ACCESS_SET_PAUSE_STATE:
        case ACCESS_GET_TITLE_INFO:
        case ACCESS_SET_TITLE:
        case ACCESS_SET_SEEKPOINT:
        case ACCESS_SET_PRIVATE_ID_STATE:
            return VLC_EGENERIC;

        default:
            msg_Warn( p_access, "unimplemented query in control" );
            return VLC_EGENERIC;

    }
    return VLC_SUCCESS;
}

/*****************************************************************************
 * BlockUDP:
 *****************************************************************************/
static block_t *BlockUDP( access_t *p_access )
{
    access_sys_t *p_sys = p_access->p_sys;
    block_t      *p_block;

    /* Read data */
    p_block = block_New( p_access, p_sys->i_mtu );
    p_block->i_buffer = net_Read( p_access, p_sys->fd, p_block->p_buffer, p_sys->i_mtu, VLC_FALSE );
    if( p_block->i_buffer <= 0 )
    {
        block_Release( p_block );
        return NULL;
    }

    if( p_block->i_buffer >= p_sys->i_mtu && p_sys->b_auto_mtu &&
        p_sys->i_mtu < 32767 )
    {
        /* Increase by 100% */
        p_sys->i_mtu *= 2;
        msg_Dbg( p_access, "increasing MTU to %d", p_sys->i_mtu );
    }

    return p_block;
}

/*****************************************************************************
 * BlockParseRTP/BlockRTP:
 *****************************************************************************/
static block_t *BlockParseRTP( access_t *p_access, block_t *p_block )
{
    int     i_rtp_version;
    int     i_CSRC_count;
    int     i_payload_type;
    int     i_skip = 0;

    if( p_block->i_buffer < RTP_HEADER_LEN )
        goto trash;

    /* Parse the header and make some verifications.
     * See RFC 1889 & RFC 2250. */
    i_rtp_version  = ( p_block->p_buffer[0] & 0xC0 ) >> 6;
    i_CSRC_count   = ( p_block->p_buffer[0] & 0x0F );
    i_payload_type = ( p_block->p_buffer[1] & 0x7F );

    if ( i_rtp_version != 2 )
        msg_Dbg( p_access, "RTP version is %u, should be 2", i_rtp_version );

    if( i_payload_type == 14 )
        i_skip = 4;
    else if( i_payload_type !=  33 && i_payload_type != 32 )
        msg_Dbg( p_access, "unsupported RTP payload type (%u)", i_payload_type );

    i_skip += RTP_HEADER_LEN + 4*i_CSRC_count;

    /* A CSRC extension field is 32 bits in size (4 bytes) */
    if( i_skip >= p_block->i_buffer )
        goto trash;

    /* Return the packet without the RTP header. */
    p_block->i_buffer -= i_skip;
    p_block->p_buffer += i_skip;

    return p_block;

trash:
    msg_Warn( p_access, "received a too short packet for RTP" );
    block_Release( p_block );
    return NULL;
}

static block_t *BlockRTP( access_t *p_access )
{
    return BlockParseRTP( p_access, BlockUDP( p_access ) );
}

/*****************************************************************************
 * BlockChoose: decide between RTP and UDP
 *****************************************************************************/
static block_t *BlockChoose( access_t *p_access )
{
    block_t *p_block;
    int     i_rtp_version;
    int     i_CSRC_count;
    int     i_payload_type;

    if( ( p_block = BlockUDP( p_access ) ) == NULL )
        return NULL;

    if( p_block->p_buffer[0] == 0x47 )
    {
        msg_Dbg( p_access, "detected TS over raw UDP" );
        p_access->pf_block = BlockUDP;
        return p_block;
    }

    if( p_block->i_buffer < RTP_HEADER_LEN )
        return p_block;

    /* Parse the header and make some verifications.
     * See RFC 1889 & RFC 2250. */

    i_rtp_version  = ( p_block->p_buffer[0] & 0xC0 ) >> 6;
    i_CSRC_count   = ( p_block->p_buffer[0] & 0x0F );
    i_payload_type = ( p_block->p_buffer[1] & 0x7F );

    if( i_rtp_version != 2 )
    {
        msg_Dbg( p_access, "no supported RTP header detected" );
        p_access->pf_block = BlockUDP;
        return p_block;
    }

    switch( i_payload_type )
    {
        case 33:
            msg_Dbg( p_access, "detected TS over RTP" );
            p_access->psz_demux = strdup( "ts" );
            break;

        case 14:
            msg_Dbg( p_access, "detected MPEG audio over RTP" );
            p_access->psz_demux = strdup( "mp3" );
            break;

        case 32:
            msg_Dbg( p_access, "detected MPEG video over RTP" );
            p_access->psz_demux = strdup( "es" );
            break;

        default:
            msg_Dbg( p_access, "no RTP header detected" );
            p_access->pf_block = BlockUDP;
            return p_block;
    }

    p_access->pf_block = BlockRTP;

    return BlockParseRTP( p_access, p_block );
}
