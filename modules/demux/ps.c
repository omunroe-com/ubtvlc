/*****************************************************************************
 * ps.c
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 * $Id: ps.c 6961 2004-03-05 17:34:23Z sam $
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
#include <stdlib.h>                                      /* malloc(), free() */

#include <vlc/vlc.h>
#include <vlc/input.h>

#include "ps.h"

/* TODO:
 *  - re-add pre-scanning.
 *  - ...
 */

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open    ( vlc_object_t * );
static void Close  ( vlc_object_t * );

vlc_module_begin();
    set_description( _("PS demuxer") );
    set_capability( "demux2", 0 );
    set_callbacks( Open, Close );
    add_shortcut( "ps2" );
vlc_module_end();

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/

struct demux_sys_t
{
    ps_track_t   tk[PS_TK_COUNT];

    int64_t     i_scr;
    int         i_mux_rate;
};

static int Demux  ( demux_t *p_demux );
static int Control( demux_t *p_demux, int i_query, va_list args );

static int      ps_pkt_resynch( stream_t *, uint32_t *pi_code );
static block_t *ps_pkt_read   ( stream_t *, uint32_t i_code );

/*****************************************************************************
 * Open
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    demux_t     *p_demux = (demux_t*)p_this;
    demux_sys_t *p_sys;

    uint8_t     *p_peek;

    if( stream_Peek( p_demux->s, &p_peek, 4 ) < 4 )
    {
        msg_Err( p_demux, "cannot peek" );
        return VLC_EGENERIC;
    }

    if( p_peek[0] != 0 || p_peek[1] != 0 || p_peek[2] != 1 || p_peek[3] < 0xb9 )
    {
        msg_Warn( p_demux, "this does not look like an MPEG PS stream, continuing anyway" );
    }

    /* Fill p_demux field */
    p_demux->pf_demux = Demux;
    p_demux->pf_control = Control;
    p_demux->p_sys = p_sys = malloc( sizeof( demux_sys_t ) );

    /* Init p_sys */
    p_sys->i_mux_rate = 0;
    p_sys->i_scr      = -1;

    ps_track_init( p_sys->tk );

    /* TODO prescanning of ES */

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    demux_t     *p_demux = (demux_t*)p_this;
    demux_sys_t *p_sys = p_demux->p_sys;

    free( p_sys );
}

/*****************************************************************************
 * Demux:
 *****************************************************************************/
static int Demux( demux_t *p_demux )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    int i_ret;
    uint32_t i_code;
    block_t  *p_pkt;

    i_ret = ps_pkt_resynch( p_demux->s, &i_code );
    if( i_ret < 0 )
    {
        return 0;
    }
    else if( i_ret == 0 )
    {
        msg_Warn( p_demux, "garbage at input" );
        return 1;
    }

    if( ( p_pkt = ps_pkt_read( p_demux->s, i_code ) ) == NULL )
    {
        return 0;
    }

    switch( i_code )
    {
        case 0x1b9:
            block_Release( p_pkt );
            break;
        case 0x1ba:
        {
            int i_mux_rate;

            if( !ps_pkt_parse_pack( p_pkt, &p_sys->i_scr, &i_mux_rate ) )
            {
                es_out_Control( p_demux->out, ES_OUT_SET_PCR, p_sys->i_scr );
                if( i_mux_rate > 0 )
                {
                    p_sys->i_mux_rate = i_mux_rate;
                }
            }
            block_Release( p_pkt );
            break;
        }
        case 0x1bb:
            if( !ps_pkt_parse_system( p_pkt, p_sys->tk ) )
            {
                int i;
                for( i = 0; i < PS_TK_COUNT; i++ )
                {
                    ps_track_t *tk = &p_sys->tk[i];

                    if( tk->b_seen && !tk->es && tk->fmt.i_cat != UNKNOWN_ES )
                    {
                        tk->es = es_out_Add( p_demux->out, &tk->fmt );
                    }
                }
            }
            block_Release( p_pkt );
            break;
        case 0x1bc:
            /* TODO PSM */
            block_Release( p_pkt );
            break;

        default:
        {
            int        i_id = ps_pkt_id( p_pkt );
            if( i_id >= 0xc0 )
            {
                ps_track_t *tk = &p_sys->tk[PS_ID_TO_TK(i_id)];

                if( !tk->b_seen )
                {
                    if( !ps_track_fill( tk, i_id ) )
                    {
                        tk->es = es_out_Add( p_demux->out, &tk->fmt );
                    }
                    tk->b_seen = VLC_TRUE;
                }
                if( tk->b_seen && tk->es && !ps_pkt_parse_pes( p_pkt, tk->i_skip ) )
                {
                    es_out_Send( p_demux->out, tk->es, p_pkt );
                }
                else
                {
                    block_Release( p_pkt );
                }
            }
            else
            {
                block_Release( p_pkt );
            }
            break;
        }
    }
    return 1;
}

/*****************************************************************************
 * Control:
 *****************************************************************************/
static int Control( demux_t *p_demux, int i_query, va_list args )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    double f, *pf;
    int64_t i64, *pi64;

    switch( i_query )
    {
        case DEMUX_GET_POSITION:
            pf = (double*) va_arg( args, double* );
            i64 = stream_Size( p_demux->s );
            if( i64 > 0 )
            {
                *pf = (double)stream_Tell( p_demux->s ) / (double)i64;
            }
            else
            {
                *pf = 0.0;
            }
            return VLC_SUCCESS;
        case DEMUX_SET_POSITION:
            f = (double) va_arg( args, double );
            i64 = stream_Size( p_demux->s );

            es_out_Control( p_demux->out, ES_OUT_RESET_PCR );

            return stream_Seek( p_demux->s, (int64_t)(i64 * f) );

        case DEMUX_GET_TIME:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            if( p_sys->i_mux_rate > 0 )
            {
                *pi64 = (int64_t)1000000 * ( stream_Tell( p_demux->s ) / 50 ) / p_sys->i_mux_rate;
                return VLC_SUCCESS;
            }
            *pi64 = 0;
            return VLC_EGENERIC;

        case DEMUX_GET_LENGTH:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            if( p_sys->i_mux_rate > 0 )
            {
                *pi64 = (int64_t)1000000 * ( stream_Size( p_demux->s ) / 50 ) / p_sys->i_mux_rate;
                return VLC_SUCCESS;
            }
            *pi64 = 0;
            return VLC_EGENERIC;

        case DEMUX_SET_TIME:
        case DEMUX_GET_FPS:
        default:
            return VLC_EGENERIC;
    }
}

/*****************************************************************************
 * Divers:
 *****************************************************************************/

/* PSResynch: resynch on a systeme starcode
 *  It doesn't skip more than 512 bytes
 *  -1 -> error, 0 -> not synch, 1 -> ok
 */
static int ps_pkt_resynch( stream_t *s, uint32_t *pi_code )
{
    uint8_t *p_peek;
    int     i_peek;
    int     i_skip;

    if( stream_Peek( s, &p_peek, 4 ) < 4 )
    {
        return -1;
    }
    if( p_peek[0] == 0 && p_peek[1] == 0 && p_peek[2] == 1 && p_peek[3] >= 0xb9 )
    {
        *pi_code = 0x100 | p_peek[3];
        return 1;
    }

    if( ( i_peek = stream_Peek( s, &p_peek, 512 ) ) < 4 )
    {
        return -1;
    }
    i_skip = 0;

    for( ;; )
    {
        if( i_peek < 4 )
        {
            break;
        }
        if( p_peek[0] == 0 && p_peek[1] == 0 && p_peek[2] == 1 && p_peek[3] >= 0xb9 )
        {
            *pi_code = 0x100 | p_peek[3];
            return stream_Read( s, NULL, i_skip ) == i_skip ? 1 : -1;
        }

        p_peek++;
        i_peek--;
        i_skip++;
    }
    return stream_Read( s, NULL, i_skip ) == i_skip ? 0 : -1;
}

static block_t *ps_pkt_read( stream_t *s, uint32_t i_code )
{
    uint8_t *p_peek;
    int      i_peek = stream_Peek( s, &p_peek, 14 );
    int      i_size = ps_pkt_size( p_peek, i_peek );

    if( i_size > 0 )
    {
        return  stream_Block( s, i_size );
    }
    return NULL;
}
