/*****************************************************************************
 * live.cpp : live.com support.
 *****************************************************************************
 * Copyright (C) 2003-2004 VideoLAN
 * $Id: livedotcom.cpp 9298 2004-11-12 22:23:11Z hartman $
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

#include <iostream>

#if defined( WIN32 )
#   include <winsock2.h>
#endif

#include "BasicUsageEnvironment.hh"
#include "GroupsockHelper.hh"
#include "liveMedia.hh"

extern "C" {
#include "../access/mms/asf.h"  /* Who said ugly ? */
}

#if (LIVEMEDIA_LIBRARY_VERSION_INT < 1089936000)
#define RECLAIM_ENV(env) delete (env)
#else
#define RECLAIM_ENV(env) (env)->reclaim()
#endif


using namespace std;

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open ( vlc_object_t * );
static void Close( vlc_object_t * );

#define CACHING_TEXT N_("Caching value (ms)")
#define CACHING_LONGTEXT N_( \
    "Allows you to modify the default caching value for RTSP streams. This " \
    "value should be set in millisecond units." )

#define KASENNA_TEXT N_( "Kasenna RTSP dialect")
#define KASENNA_LONGTEXT N_( "Kasenna server speak an old and unstandard dialect of RTSP " \
    "When you set this parameter, VLC will try this dialect for communication. In " \
    "this mode you cannot talk to normal RTSP servers." )

vlc_module_begin();
    set_description( _("live.com (RTSP/RTP/SDP) demuxer" ) );
    set_capability( "demux2", 50 );
    set_callbacks( Open, Close );
    add_shortcut( "live" );

    add_submodule();
        set_description( _("RTSP/RTP access and demux") );
        add_shortcut( "rtsp" );
        add_shortcut( "sdp" );
        set_capability( "access_demux", 0 );
        set_callbacks( Open, Close );
        add_bool( "rtsp-tcp", 0, NULL,
                  N_("Use RTP over RTSP (TCP)"),
                  N_("Use RTP over RTSP (TCP)"), VLC_TRUE );
        add_integer( "rtsp-caching", 4 * DEFAULT_PTS_DELAY / 1000, NULL,
            CACHING_TEXT, CACHING_LONGTEXT, VLC_TRUE );
        add_bool( "rtsp-kasenna", VLC_FALSE, NULL, KASENNA_TEXT,
                  KASENNA_LONGTEXT, VLC_TRUE );
vlc_module_end();

/* TODO:
 *  - Improve support of PS/TS
 *  - Support X-QT/X-QUICKTIME generic codec for audio.
 *
 *  - Check memory leak, delete/free -> still one when using rtsp-tcp but I'm
 *  not sure if it comes from me.
 *
 */

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
typedef struct
{
    demux_t     *p_demux;

    vlc_bool_t   b_quicktime;
    vlc_bool_t   b_muxed;
    vlc_bool_t   b_asf;

    es_format_t  fmt;
    es_out_id_t  *p_es;

    stream_t     *p_out_muxed;    /* for muxed stream */

    RTPSource    *rtpSource;
    FramedSource *readSource;
    vlc_bool_t   b_rtcp_sync;

    uint8_t      *p_buffer;
    unsigned int  i_buffer;

    char         waiting;

    mtime_t      i_pts;
} live_track_t;

struct demux_sys_t
{
    char         *p_sdp;    /* XXX mallocated */

    MediaSession     *ms;
    TaskScheduler    *scheduler;
    UsageEnvironment *env ;
    RTSPClient       *rtsp;

    /* */
    int              i_track;
    live_track_t     **track;   /* XXX mallocated */
    mtime_t          i_pcr;
    mtime_t          i_pcr_start;

    /* Asf */
    asf_header_t     asfh;
    stream_t         *p_out_asf;

    /* */
    mtime_t          i_length;
    mtime_t          i_start;

    char             event;
};

static int Demux  ( demux_t * );
static int Control( demux_t *, int, va_list );

static int ParseASF( demux_t * );

static void StreamRead( void *p_private, unsigned int i_size, unsigned int i_truncated_bytes, struct timeval pts, unsigned int i_duration );
static void StreamClose( void *p_private );
static void TaskInterrupt( void *p_private );


/*****************************************************************************
 * DemuxOpen:
 *****************************************************************************/
static int  Open ( vlc_object_t *p_this )
{
    demux_t     *p_demux = (demux_t*)p_this;
    demux_sys_t *p_sys;

    MediaSubsessionIterator *iter;
    MediaSubsession *sub;

    vlc_bool_t b_rtsp_tcp;
    uint8_t *p_peek;

    int     i_sdp;
    int     i_sdp_max;
    uint8_t *p_sdp;

    if( p_demux->s )
    {
        /* See if it looks like a SDP
           v, o, s fields are mandatory and in this order */
        if( stream_Peek( p_demux->s, &p_peek, 7 ) < 7 )
        {
            msg_Err( p_demux, "cannot peek" );
            return VLC_EGENERIC;
        }
        if( strncmp( (char*)p_peek, "v=0\r\n", 5 ) && strncmp( (char*)p_peek, "v=0\n", 4 ) &&
            ( p_peek[0] < 'a' || p_peek[0] > 'z' || p_peek[1] != '=' ) )
        {
            msg_Warn( p_demux, "SDP module discarded" );
            return VLC_EGENERIC;
        }
    }
    else
    {
        var_Create( p_demux, "rtsp-caching", VLC_VAR_INTEGER|VLC_VAR_DOINHERIT );
    }

    p_demux->pf_demux  = Demux;
    p_demux->pf_control= Control;
    p_demux->p_sys     = p_sys = (demux_sys_t*)malloc( sizeof( demux_sys_t ) );
    p_sys->p_sdp = NULL;
    p_sys->scheduler = NULL;
    p_sys->env = NULL;
    p_sys->ms = NULL;
    p_sys->rtsp = NULL;
    p_sys->i_track = 0;
    p_sys->track   = NULL;
    p_sys->i_pcr   = 0;
    p_sys->i_pcr_start = 0;
    p_sys->i_length = 0;
    p_sys->i_start = 0;
    p_sys->p_out_asf = NULL;


    if( ( p_sys->scheduler = BasicTaskScheduler::createNew() ) == NULL )
    {
        msg_Err( p_demux, "BasicTaskScheduler::createNew failed" );
        goto error;
    }
    if( ( p_sys->env = BasicUsageEnvironment::createNew(*p_sys->scheduler) ) == NULL )
    {
        msg_Err( p_demux, "BasicUsageEnvironment::createNew failed" );
        goto error;
    }

    if( p_demux->s == NULL && !strcasecmp( p_demux->psz_access, "rtsp" ) )
    {
        char *psz_url;
        char *psz_options;

        if( ( p_sys->rtsp = RTSPClient::createNew(*p_sys->env, 1/*verbose*/, "VLC Media Player" ) ) == NULL )
        {
            msg_Err( p_demux, "RTSPClient::createNew failed (%s)", p_sys->env->getResultMsg() );
            goto error;
        }
        psz_url = (char*)malloc( strlen( p_demux->psz_path ) + 8 );
        sprintf( psz_url, "rtsp://%s", p_demux->psz_path );

        /* Add kasenna option */
        psz_options = p_sys->rtsp->sendOptionsCmd( psz_url );
        if( psz_options )
            delete [] psz_options;

        p_sdp = (uint8_t*)p_sys->rtsp->describeURL( psz_url,
                              NULL, var_CreateGetBool( p_demux, "rtsp-kasenna" ) );
        if( p_sdp == NULL )
        {
            msg_Err( p_demux, "describeURL failed (%s)", p_sys->env->getResultMsg() );

            free( psz_url );
            goto error;
        }
        free( psz_url );

        /* malloc-ated copy */
        p_sys->p_sdp = strdup( (char*)p_sdp );
        delete[] p_sdp;
        fprintf( stderr, "sdp=%s\n", p_sys->p_sdp );
    }
    else if( p_demux->s == NULL && !strcasecmp( p_demux->psz_access, "sdp" ) )
    {
        p_sys->p_sdp = strdup( p_demux->psz_path );
    }
    else
    {
        /* Gather the complete sdp file */
        i_sdp = 0;
        i_sdp_max = 1000;
        p_sdp = (uint8_t*)malloc( i_sdp_max );
        for( ;; )
        {
            int i_read = stream_Read( p_demux->s, &p_sdp[i_sdp], i_sdp_max - i_sdp - 1 );

            if( i_read < 0 )
            {
                msg_Err( p_demux, "failed to read SDP" );
                free( p_sys );
                return VLC_EGENERIC;
            }

            i_sdp += i_read;

            if( i_read < i_sdp_max - i_sdp - 1 )
            {
                p_sdp[i_sdp] = '\0';
                break;
            }

            i_sdp_max += 1000;
            p_sdp = (uint8_t*)realloc( p_sdp, i_sdp_max );
        }
        p_sys->p_sdp = (char*)p_sdp;

        msg_Dbg( p_demux, "sdp=%s\n", p_sys->p_sdp );
    }
    if( ( p_sys->ms = MediaSession::createNew(*p_sys->env, p_sys->p_sdp ) ) == NULL )
    {
        msg_Err( p_demux, "MediaSession::createNew failed" );
        goto error;
    }

    b_rtsp_tcp = var_CreateGetBool( p_demux, "rtsp-tcp" );

    /* Initialise each media subsession */
    iter = new MediaSubsessionIterator( *p_sys->ms );
    while( ( sub = iter->next() ) != NULL )
    {
        unsigned int i_buffer = 0;
        Boolean bInit;

        /* Value taken from mplayer */
        if( !strcmp( sub->mediumName(), "audio" ) )
            i_buffer = 100000;
        else if( !strcmp( sub->mediumName(), "video" ) )
            i_buffer = 2000000;
        else
            continue;

        if( !strcmp( sub->codecName(), "X-ASF-PF" ) )
            bInit = sub->initiate( 4 ); /* Constant ? */
        else
            bInit = sub->initiate();

        if( !bInit )
        {
            msg_Warn( p_demux, "RTP subsession '%s/%s' failed(%s)", sub->mediumName(), sub->codecName(), p_sys->env->getResultMsg() );
        }
        else
        {
            if( sub->rtpSource() )
            {
                int fd = sub->rtpSource()->RTPgs()->socketNum();
                /* Increase the buffer size */
                increaseReceiveBufferTo( *p_sys->env, fd, i_buffer );
            }

            msg_Dbg( p_demux, "RTP subsession '%s/%s'", sub->mediumName(), sub->codecName() );

            /* Issue the SETUP */
            if( p_sys->rtsp )
            {
                p_sys->rtsp->setupMediaSubsession( *sub, False, b_rtsp_tcp ? True : False );
            }
        }
    }

    if( p_sys->rtsp )
    {
        /* The PLAY */
        if( !p_sys->rtsp->playMediaSession( *p_sys->ms ) )
        {
            msg_Err( p_demux, "PLAY failed %s", p_sys->env->getResultMsg() );
            goto error;
        }
    }

    /* Create all es struct */
    iter->reset();
    while( ( sub = iter->next() ) != NULL )
    {
        live_track_t *tk;

        if( sub->readSource() == NULL )
        {
            continue;
        }

        tk = (live_track_t*)malloc( sizeof( live_track_t ) );
        tk->p_demux = p_demux;
        tk->waiting = 0;
        tk->i_pts   = 0;
        tk->b_quicktime = VLC_FALSE;
        tk->b_muxed     = VLC_FALSE;
        tk->b_asf       = VLC_FALSE;
        tk->b_rtcp_sync = VLC_FALSE;
        tk->p_out_muxed = NULL;
        tk->p_es        = NULL;
        tk->i_buffer    = 65536;
        tk->p_buffer    = (uint8_t *)malloc( 65536 );

        /* Value taken from mplayer */
        if( !strcmp( sub->mediumName(), "audio" ) )
        {
            es_format_Init( &tk->fmt, AUDIO_ES, VLC_FOURCC( 'u', 'n', 'd', 'f' ) );
            tk->fmt.audio.i_channels = sub->numChannels();
            tk->fmt.audio.i_rate = sub->rtpSource()->timestampFrequency();

            if( !strcmp( sub->codecName(), "MPA" ) ||
                !strcmp( sub->codecName(), "MPA-ROBUST" ) ||
                !strcmp( sub->codecName(), "X-MP3-DRAFT-00" ) )
            {
                tk->fmt.i_codec = VLC_FOURCC( 'm', 'p', 'g', 'a' );
                tk->fmt.audio.i_rate = 0;
            }
            else if( !strcmp( sub->codecName(), "AC3" ) )
            {
                tk->fmt.i_codec = VLC_FOURCC( 'a', '5', '2', ' ' );
                tk->fmt.audio.i_rate = 0;
            }
            else if( !strcmp( sub->codecName(), "L16" ) )
            {
                tk->fmt.i_codec = VLC_FOURCC( 't', 'w', 'o', 's' );
                tk->fmt.audio.i_bitspersample = 16;
            }
            else if( !strcmp( sub->codecName(), "L8" ) )
            {
                tk->fmt.i_codec = VLC_FOURCC( 'a', 'r', 'a', 'w' );
                tk->fmt.audio.i_bitspersample = 8;
            }
            else if( !strcmp( sub->codecName(), "PCMU" ) )
            {
                tk->fmt.i_codec = VLC_FOURCC( 'u', 'l', 'a', 'w' );
            }
            else if( !strcmp( sub->codecName(), "PCMA" ) )
            {
                tk->fmt.i_codec = VLC_FOURCC( 'a', 'l', 'a', 'w' );
            }
            else if( !strcmp( sub->codecName(), "MP4A-LATM" ) )
            {
                unsigned int i_extra;
                uint8_t      *p_extra;

                tk->fmt.i_codec = VLC_FOURCC( 'm', 'p', '4', 'a' );

                if( ( p_extra = parseStreamMuxConfigStr( sub->fmtp_config(), i_extra ) ) )
                {
                    tk->fmt.i_extra = i_extra;
                    tk->fmt.p_extra = malloc( i_extra );
                    memcpy( tk->fmt.p_extra, p_extra, i_extra );
                    delete[] p_extra;
                }
            }
            else if( !strcmp( sub->codecName(), "MPEG4-GENERIC" ) )
            {
                unsigned int i_extra;
                uint8_t      *p_extra;

                tk->fmt.i_codec = VLC_FOURCC( 'm', 'p', '4', 'a' );

                if( ( p_extra = parseGeneralConfigStr( sub->fmtp_config(), i_extra ) ) )
                {
                    tk->fmt.i_extra = i_extra;
                    tk->fmt.p_extra = malloc( i_extra );
                    memcpy( tk->fmt.p_extra, p_extra, i_extra );
                    delete[] p_extra;
                }
            }
            else if( !strcmp( sub->codecName(), "X-ASF-PF" ) )
            {
                tk->b_asf = VLC_TRUE;
                if( p_sys->p_out_asf == NULL )
                    p_sys->p_out_asf = stream_DemuxNew( p_demux, "asf", p_demux->out );;
            }
        }
        else if( !strcmp( sub->mediumName(), "video" ) )
        {
            es_format_Init( &tk->fmt, VIDEO_ES, VLC_FOURCC( 'u', 'n', 'd', 'f' ) );
            if( !strcmp( sub->codecName(), "MPV" ) )
            {
                tk->fmt.i_codec = VLC_FOURCC( 'm', 'p', 'g', 'v' );
            }
            else if( !strcmp( sub->codecName(), "H263" ) ||
                     !strcmp( sub->codecName(), "H263-1998" ) ||
                     !strcmp( sub->codecName(), "H263-2000" ) )
            {
                tk->fmt.i_codec = VLC_FOURCC( 'H', '2', '6', '3' );
            }
            else if( !strcmp( sub->codecName(), "H261" ) )
            {
                tk->fmt.i_codec = VLC_FOURCC( 'H', '2', '6', '1' );
            }
            else if( !strcmp( sub->codecName(), "JPEG" ) )
            {
                tk->fmt.i_codec = VLC_FOURCC( 'M', 'J', 'P', 'G' );
            }
            else if( !strcmp( sub->codecName(), "X-SV3V-ES" ) )
            {
                tk->fmt.i_codec = VLC_FOURCC( 'S', 'V', 'Q', '3' );
            }
            else if( !strcmp( sub->codecName(), "MP4V-ES" ) )
            {
                unsigned int i_extra;
                uint8_t      *p_extra;

                tk->fmt.i_codec = VLC_FOURCC( 'm', 'p', '4', 'v' );

                if( ( p_extra = parseGeneralConfigStr( sub->fmtp_config(), i_extra ) ) )
                {
                    tk->fmt.i_extra = i_extra;
                    tk->fmt.p_extra = malloc( i_extra );
                    memcpy( tk->fmt.p_extra, p_extra, i_extra );
                    delete[] p_extra;
                }
            }
            else if( !strcmp( sub->codecName(), "X-QT" ) || !strcmp( sub->codecName(), "X-QUICKTIME" ) )
            {
                tk->b_quicktime = VLC_TRUE;
            }
            else if( !strcmp( sub->codecName(), "MP2T" ) )
            {
                tk->b_muxed = VLC_TRUE;
                tk->p_out_muxed = stream_DemuxNew( p_demux, "ts", p_demux->out );
            }
            else if( !strcmp( sub->codecName(), "MP2P" ) || !strcmp( sub->codecName(), "MP1S" ) )
            {
                tk->b_muxed = VLC_TRUE;
                tk->p_out_muxed = stream_DemuxNew( p_demux, "ps", p_demux->out );
            }
            else if( !strcmp( sub->codecName(), "X-ASF-PF" ) )
            {
                tk->b_asf = VLC_TRUE;
                if( p_sys->p_out_asf == NULL )
                    p_sys->p_out_asf = stream_DemuxNew( p_demux, "asf", p_demux->out );;
            }
        }

        if( tk->fmt.i_codec != VLC_FOURCC( 'u', 'n', 'd', 'f' ) )
        {
            tk->p_es = es_out_Add( p_demux->out, &tk->fmt );
        }

        if( sub->rtcpInstance() != NULL )
        {
            sub->rtcpInstance()->setByeHandler( StreamClose, tk );
        }

        if( tk->p_es || tk->b_quicktime || tk->b_muxed || tk->b_asf )
        {
            tk->readSource = sub->readSource();
            tk->rtpSource  = sub->rtpSource();

            /* Append */
            p_sys->track = (live_track_t**)realloc( p_sys->track, sizeof( live_track_t ) * ( p_sys->i_track + 1 ) );
            p_sys->track[p_sys->i_track++] = tk;
        }
        else
        {
            free( tk );
        }
    }

    delete iter;

    if( p_sys->p_out_asf && ParseASF( p_demux ) )
    {
        msg_Err( p_demux, "cannot find a usable asf header" );
        /* TODO Clean tracks */
        goto error;
    }

    p_sys->i_length = (mtime_t)(p_sys->ms->playEndTime() * 1000000.0);
    if( p_sys->i_length < 0 )
    {
        p_sys->i_length = 0;
    }
    else if( p_sys->i_length > 0 )
    {
        /* FIXME */
        /* p_input->stream.p_selected_area->i_size = 1000;*/ /* needed for now */
    }

    if( p_sys->i_track <= 0 )
    {
        msg_Err( p_demux, "no codec supported, aborting" );
        goto error;
    }

    return VLC_SUCCESS;

error:
    if( p_sys->p_out_asf )
    {
        stream_DemuxDelete( p_sys->p_out_asf );
    }
    if( p_sys->ms )
    {
        Medium::close( p_sys->ms );
    }
    if( p_sys->rtsp )
    {
        Medium::close( p_sys->rtsp );
    }
    if( p_sys->env )
    {
        RECLAIM_ENV(p_sys->env);
    }
    if( p_sys->scheduler )
    {
        delete p_sys->scheduler;
    }
    if( p_sys->p_sdp )
    {
        free( p_sys->p_sdp );
    }
    free( p_sys );
    return VLC_EGENERIC;
}



/*****************************************************************************
 * DemuxClose:
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    demux_t *p_demux = (demux_t*)p_this;
    demux_sys_t    *p_sys = p_demux->p_sys;
    int            i;

    for( i = 0; i < p_sys->i_track; i++ )
    {
        live_track_t *tk = p_sys->track[i];

        if( tk->b_muxed )
        {
            stream_DemuxDelete( tk->p_out_muxed );
        }
        free( tk->p_buffer );
        free( tk );
    }
    if( p_sys->i_track )
    {
        free( p_sys->track );
    }
    if( p_sys->p_out_asf )
    {
        stream_DemuxDelete( p_sys->p_out_asf );
    }

    if( p_sys->rtsp && p_sys->ms )
    {
        /* TEARDOWN */
        p_sys->rtsp->teardownMediaSession( *p_sys->ms );
    }
    Medium::close( p_sys->ms );
    if( p_sys->rtsp )
    {
        Medium::close( p_sys->rtsp );
    }

    if( p_sys->env )
    {
        RECLAIM_ENV(p_sys->env);
    }
    if( p_sys->scheduler )
    {
        delete p_sys->scheduler;
    }
    if( p_sys->p_sdp )
    {
        free( p_sys->p_sdp );
    }
    free( p_sys );
}

/*****************************************************************************
 * Demux:
 *****************************************************************************/
static int Demux( demux_t *p_demux )
{
    demux_sys_t    *p_sys = p_demux->p_sys;
    TaskToken      task;

    vlc_bool_t      b_send_pcr = VLC_TRUE;
    mtime_t         i_pcr = 0;
    int             i;

    for( i = 0; i < p_sys->i_track; i++ )
    {
        live_track_t *tk = p_sys->track[i];

        if( tk->b_asf || tk->b_muxed )
            b_send_pcr = VLC_FALSE;

        if( i_pcr == 0 )
        {
            i_pcr = tk->i_pts;
        }
        else if( tk->i_pts != 0 && i_pcr > tk->i_pts )
        {
            i_pcr = tk->i_pts ;
        }
    }
    if( i_pcr != p_sys->i_pcr && i_pcr > 0 )
    {
        p_sys->i_pcr = i_pcr;

        if( b_send_pcr )
            es_out_Control( p_demux->out, ES_OUT_SET_PCR, i_pcr );
        if( p_sys->i_pcr_start <= 0 || p_sys->i_pcr_start > i_pcr ||
            ( p_sys->i_length > 0 && i_pcr - p_sys->i_pcr_start > p_sys->i_length ) )
        {
            p_sys->i_pcr_start = i_pcr;
        }
    }

    /* First warm we want to read data */
    p_sys->event = 0;
    for( i = 0; i < p_sys->i_track; i++ )
    {
        live_track_t *tk = p_sys->track[i];

        if( tk->waiting == 0 )
        {
            tk->waiting = 1;
            tk->readSource->getNextFrame( tk->p_buffer, tk->i_buffer,
                                          StreamRead, tk,
                                          StreamClose, tk );
        }
    }
    /* Create a task that will be called if we wait more than 300ms */
    task = p_sys->scheduler->scheduleDelayedTask( 300000, TaskInterrupt, p_demux );

    /* Do the read */
    p_sys->scheduler->doEventLoop( &p_sys->event );

    /* remove the task */
    p_sys->scheduler->unscheduleDelayedTask( task );

    /* Check for gap in pts value */
    for( i = 0; i < p_sys->i_track; i++ )
    {
        live_track_t *tk = p_sys->track[i];

        if( !tk->b_muxed && !tk->b_rtcp_sync && tk->rtpSource->hasBeenSynchronizedUsingRTCP() )
        {
            msg_Dbg( p_demux, "tk->rtpSource->hasBeenSynchronizedUsingRTCP()" );

            es_out_Control( p_demux->out, ES_OUT_RESET_PCR );
            tk->b_rtcp_sync = VLC_TRUE;

            /* reset PCR and PCR start, mmh won't work well for multi-stream I fear */
            tk->i_pts = 0;
            p_sys->i_pcr_start = 0;
            p_sys->i_pcr = 0;
            i_pcr = 0;
        }
    }

    return p_demux->b_error ? 0 : 1;
}

/*****************************************************************************
 * Control:
 *****************************************************************************/
static int Control( demux_t *p_demux, int i_query, va_list args )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    int64_t *pi64;
    double  *pf, f;
    vlc_bool_t *pb, b_bool;
    int i;

    switch( i_query )
    {
        case DEMUX_GET_TIME:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            *pi64 = p_sys->i_pcr - p_sys->i_pcr_start + p_sys->i_start;
            return VLC_SUCCESS;

        case DEMUX_GET_LENGTH:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            *pi64 = p_sys->i_length;
            return VLC_SUCCESS;

        case DEMUX_GET_POSITION:
            pf = (double*)va_arg( args, double* );
            if( p_sys->i_length > 0 )
            {
                *pf = (double)( p_sys->i_pcr - p_sys->i_pcr_start + p_sys->i_start)/
                      (double)(p_sys->i_length);
            }
            else
            {
                *pf = 0;
            }
            return VLC_SUCCESS;

        case DEMUX_SET_POSITION:
        {
            float time;

            f = (double)va_arg( args, double );
            time = f * (double)p_sys->i_length / 1000000.0;   /* in second */

            if( p_sys->rtsp && p_sys->i_length > 0 )
            {
                MediaSubsessionIterator *iter = new MediaSubsessionIterator( *p_sys->ms );
                MediaSubsession         *sub;
                int i;

                while( ( sub = iter->next() ) != NULL )
                {
                    p_sys->rtsp->playMediaSubsession( *sub, time );
                }
                delete iter;
                p_sys->i_start = (mtime_t)(f * (double)p_sys->i_length);
                p_sys->i_pcr_start = 0;
                p_sys->i_pcr       = 0;
                
                for( i = 0; i < p_sys->i_track; i++ )
                {
                    p_sys->track[i]->i_pts = 0;
                }
                return VLC_SUCCESS;
            }
            return VLC_SUCCESS;
        }

        /* Special for access_demux */
        case DEMUX_CAN_PAUSE:
            pb = (vlc_bool_t*)va_arg( args, vlc_bool_t * );
            if( p_sys->rtsp && p_sys->i_length )
                *pb = VLC_TRUE; /* Not always true, but will be handled in SET_PAUSE_STATE */
            else
                *pb = VLC_FALSE;
            return VLC_SUCCESS;

        case DEMUX_CAN_CONTROL_PACE:
            pb = (vlc_bool_t*)va_arg( args, vlc_bool_t * );
            *pb = VLC_FALSE;
            return VLC_SUCCESS;

        case DEMUX_SET_PAUSE_STATE:
            double d_npt;
            MediaSubsessionIterator *iter;
            MediaSubsession *sub;

            d_npt = ( (double)( p_sys->i_pcr - p_sys->i_pcr_start + p_sys->i_start ) ) / 1000000.00;

            b_bool = (vlc_bool_t)va_arg( args, vlc_bool_t );
            if( p_sys->rtsp == NULL )
                return VLC_EGENERIC;

            iter = new MediaSubsessionIterator( *p_sys->ms );
            while( ( sub = iter->next() ) != NULL )
            {
                if( ( b_bool && !p_sys->rtsp->pauseMediaSubsession( *sub ) ) ||
                    ( !b_bool && !p_sys->rtsp->playMediaSubsession( *sub, d_npt > 0 ? d_npt : -1 ) ) )
                {
                    delete iter;
                    return VLC_EGENERIC;
                }
            }
            delete iter;
#if 0
            /* reset PCR and PCR start, mmh won't work well for multi-stream I fear */
            for( i = 0; i < p_sys->i_track; i++ )
            {
                p_sys->track[i]->i_pts = 0;
            }
            p_sys->i_pcr_start = 0; /* FIXME Wrong */
            p_sys->i_pcr = 0;
#endif
            return VLC_SUCCESS;

        case DEMUX_GET_TITLE_INFO:
        case DEMUX_SET_TITLE:
        case DEMUX_SET_SEEKPOINT:
            return VLC_EGENERIC;

        case DEMUX_GET_PTS_DELAY:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            *pi64 = (int64_t)var_GetInteger( p_demux, "rtsp-caching" ) * I64C(1000);
            return VLC_SUCCESS;

        default:
            return VLC_EGENERIC;
    }
}

/*****************************************************************************
 *
 *****************************************************************************/
static void StreamRead( void *p_private, unsigned int i_size, unsigned int i_truncated_bytes, struct timeval pts, unsigned int duration )
{
    live_track_t   *tk = (live_track_t*)p_private;
    demux_t        *p_demux = tk->p_demux;
    demux_sys_t    *p_sys = p_demux->p_sys;
    block_t        *p_block;

    mtime_t i_pts = (uint64_t)pts.tv_sec * UI64C(1000000) + (uint64_t)pts.tv_usec;

    /* XXX Beurk beurk beurk Avoid having negative value XXX */
    i_pts &= UI64C(0x00ffffffffffffff);

    if( tk->b_quicktime && tk->p_es == NULL )
    {
        QuickTimeGenericRTPSource *qtRTPSource = (QuickTimeGenericRTPSource*)tk->rtpSource;
        QuickTimeGenericRTPSource::QTState &qtState = qtRTPSource->qtState;
        uint8_t *sdAtom = (uint8_t*)&qtState.sdAtom[4];

        if( qtState.sdAtomSize < 16 + 32 )
        {
            /* invalid */
            p_sys->event = 0xff;
            tk->waiting = 0;
            return;
        }
        tk->fmt.i_codec = VLC_FOURCC( sdAtom[0], sdAtom[1], sdAtom[2], sdAtom[3] );
        tk->fmt.video.i_width  = (sdAtom[28] << 8) | sdAtom[29];
        tk->fmt.video.i_height = (sdAtom[30] << 8) | sdAtom[31];

        tk->fmt.i_extra        = qtState.sdAtomSize - 16;
        tk->fmt.p_extra        = malloc( tk->fmt.i_extra );
        memcpy( tk->fmt.p_extra, &sdAtom[12], tk->fmt.i_extra );

        tk->p_es = es_out_Add( p_demux->out, &tk->fmt );
    }

#if 0
    fprintf( stderr, "StreamRead size=%d pts=%lld\n",
             i_size,
             pts.tv_sec * 1000000LL + pts.tv_usec );
#endif

    /* grow buffer if it looks like buffer is too small, but don't eat
     * up all the memory on strange streams */
    if( i_truncated_bytes > 0 && tk->i_buffer < 2000000 )
    {
        void *p_tmp;
        msg_Dbg( p_demux, "lost %d bytes", i_truncated_bytes );
        msg_Dbg( p_demux, "increasing buffer size to %d", tk->i_buffer * 2 );
        tk->i_buffer *= 2;
        p_tmp = realloc( tk->p_buffer, tk->i_buffer );
        if (p_tmp == NULL)
        {
            msg_Warn( p_demux, "realloc failed" );
        }
        else
        {
            tk->p_buffer = (uint8_t*)p_tmp;
        }
    }
    if( i_size > tk->i_buffer )
    {
        msg_Warn( p_demux, "buffer overflow" );
    }
    /* FIXME could i_size be > buffer size ? */
    if( tk->fmt.i_codec == VLC_FOURCC('h','2','6','1') )
    {
#if LIVEMEDIA_LIBRARY_VERSION_INT >= 1081468800
        H261VideoRTPSource *h261Source = (H261VideoRTPSource*)tk->rtpSource;
        uint32_t header = h261Source->lastSpecialHeader();
#else
        uint32_t header = 0;
        msg_Warn( p_demux, "need livemedia library >= \"2004.04.09\"" );
#endif
        p_block = block_New( p_demux, i_size + 4 );
        memcpy( p_block->p_buffer, &header, 4 );
        memcpy( p_block->p_buffer + 4, tk->p_buffer, i_size );
    }
    else if( tk->b_asf )
    {
        int i_copy = __MIN( p_sys->asfh.i_min_data_packet_size, i_size );
        p_block = block_New( p_demux, p_sys->asfh.i_min_data_packet_size );

        memcpy( p_block->p_buffer, tk->p_buffer, i_copy );
    }
    else
    {
        p_block = block_New( p_demux, i_size );
        memcpy( p_block->p_buffer, tk->p_buffer, i_size );
    }
    if( tk->fmt.i_codec == VLC_FOURCC('h','2','6','1') &&
        tk->rtpSource->curPacketMarkerBit() )
    {
        p_block->i_flags |= BLOCK_FLAG_END_OF_FRAME;
    }
    //p_block->i_rate = p_input->stream.control.i_rate;

    if( i_pts != tk->i_pts && !tk->b_muxed )
    {
        p_block->i_dts = i_pts;
        p_block->i_pts = i_pts;
    }
    //fprintf( stderr, "tk -> dpts=%lld\n", i_pts - tk->i_pts );

    if( tk->b_muxed )
    {
        stream_DemuxSend( tk->p_out_muxed, p_block );
    }
    else if( tk->b_asf )
    {
        stream_DemuxSend( p_sys->p_out_asf, p_block );
    }
    else
    {
        es_out_Send( p_demux->out, tk->p_es, p_block );
    }

    /* warm that's ok */
    p_sys->event = 0xff;

    /* we have read data */
    tk->waiting = 0;

    if( i_pts > 0 && !tk->b_muxed )
    {
        tk->i_pts = i_pts;
    }
}

/*****************************************************************************
 *
 *****************************************************************************/
static void StreamClose( void *p_private )
{
    live_track_t   *tk = (live_track_t*)p_private;
    demux_t        *p_demux = tk->p_demux;
    demux_sys_t    *p_sys = p_demux->p_sys;

    fprintf( stderr, "StreamClose\n" );

    p_sys->event = 0xff;
    p_demux->b_error = VLC_TRUE;
}


/*****************************************************************************
 *
 *****************************************************************************/
static void TaskInterrupt( void *p_private )
{
    demux_t *p_demux = (demux_t*)p_private;

    fprintf( stderr, "TaskInterrupt\n" );

    /* Avoid lock */
    p_demux->p_sys->event = 0xff;
}

/*****************************************************************************
 *
 *****************************************************************************/
static int b64_decode( char *dest, char *src );

static int ParseASF( demux_t *p_demux )
{
    demux_sys_t    *p_sys = p_demux->p_sys;

    const char *psz_marker = "a=pgmpu:data:application/vnd.ms.wms-hdr.asfv1;base64,";
    char *psz_asf = strcasestr( p_sys->p_sdp, psz_marker );
    char *psz_end;
    block_t *p_header;

    /* Parse the asf header */
    if( psz_asf == NULL )
        return VLC_EGENERIC;

    psz_asf += strlen( psz_marker );
    psz_asf = strdup( psz_asf );    /* Duplicate it */
    psz_end = strchr( psz_asf, '\n' );

    while( psz_end > psz_asf && ( *psz_end == '\n' || *psz_end == '\r' ) )
        *psz_end-- = '\0';

    if( psz_asf >= psz_end )
    {
        free( psz_asf );
        return VLC_EGENERIC;
    }

    /* Always smaller */
    p_header = block_New( p_demux, psz_end - psz_asf );
    p_header->i_buffer = b64_decode( (char*)p_header->p_buffer, psz_asf );
    fprintf( stderr, "Size=%d Hdrb64=%s\n", p_header->i_buffer, psz_asf );
    if( p_header->i_buffer <= 0 )
    {
        free( psz_asf );
        return VLC_EGENERIC;
    }

    /* Parse it to get packet size */
    E_(asf_HeaderParse)( &p_sys->asfh, p_header->p_buffer, p_header->i_buffer );

    /* Send it to demuxer */
    stream_DemuxSend( p_sys->p_out_asf, p_header );

    free( psz_asf );
    return VLC_SUCCESS;
}
/*char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";*/
static int b64_decode( char *dest, char *src )
{
    const char *dest_start = dest;
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

    return dest - dest_start;
}

