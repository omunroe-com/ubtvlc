/*****************************************************************************
 * transcode.c: transcoding stream output module
 *****************************************************************************
 * Copyright (C) 2003-2004 VideoLAN
 * $Id: transcode.c 9268 2004-11-10 13:01:48Z gbazin $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Gildas Bazin <gbazin@videolan.org>
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
#include <vlc/input.h>
#include <vlc/sout.h>
#include <vlc/vout.h>
#include <vlc/decoder.h>
#include "vlc_filter.h"
#include "osd.h"

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define VENC_TEXT N_("Video encoder")
#define VENC_LONGTEXT N_( \
    "Allows you to specify the video encoder to use and its associated " \
    "options." )
#define VCODEC_TEXT N_("Destination video codec")
#define VCODEC_LONGTEXT N_( \
    "Allows you to specify the destination video codec used for the " \
    "streaming output." )
#define VB_TEXT N_("Video bitrate")
#define VB_LONGTEXT N_( \
    "Allows you to specify the video bitrate used for the streaming " \
    "output." )
#define SCALE_TEXT N_("Video scaling")
#define SCALE_LONGTEXT N_( \
    "Allows you to scale the video before encoding." )
#define FPS_TEXT N_("Video frame-rate")
#define FPS_LONGTEXT N_( \
    "Allows you to specify an output frame rate for the video." )
#define DEINTERLACE_TEXT N_("Deinterlace video")
#define DEINTERLACE_LONGTEXT N_( \
    "Allows you to deinterlace the video before encoding." )
#define WIDTH_TEXT N_("Video width")
#define WIDTH_LONGTEXT N_( \
    "Allows you to specify the output video width." )
#define HEIGHT_TEXT N_("Video height")
#define HEIGHT_LONGTEXT N_( \
    "Allows you to specify the output video height." )

#define CROPTOP_TEXT N_("Video crop top")
#define CROPTOP_LONGTEXT N_( \
    "Allows you to specify the top coordinate for the video cropping." )
#define CROPLEFT_TEXT N_("Video crop left")
#define CROPLEFT_LONGTEXT N_( \
    "Allows you to specify the left coordinate for the video cropping." )
#define CROPBOTTOM_TEXT N_("Video crop bottom")
#define CROPBOTTOM_LONGTEXT N_( \
    "Allows you to specify the bottom coordinate for the video cropping." )
#define CROPRIGHT_TEXT N_("Video crop right")
#define CROPRIGHT_LONGTEXT N_( \
    "Allows you to specify the right coordinate for the video cropping." )

#define AENC_TEXT N_("Audio encoder")
#define AENC_LONGTEXT N_( \
    "Allows you to specify the audio encoder to use and its associated " \
    "options." )
#define ACODEC_TEXT N_("Destination audio codec")
#define ACODEC_LONGTEXT N_( \
    "Allows you to specify the destination audio codec used for the " \
    "streaming output." )
#define AB_TEXT N_("Audio bitrate")
#define AB_LONGTEXT N_( \
    "Allows you to specify the audio bitrate used for the streaming " \
    "output." )
#define ARATE_TEXT N_("Audio sample rate")
#define ARATE_LONGTEXT N_( \
    "Allows you to specify the audio sample rate used for the streaming " \
    "output." )
#define ACHANS_TEXT N_("Audio channels")
#define ACHANS_LONGTEXT N_( \
    "Allows you to specify the number of audio channels used for the " \
    "streaming output." )

#define SENC_TEXT N_("Subtitles encoder")
#define SENC_LONGTEXT N_( \
    "Allows you to specify the subtitles encoder to use and its associated " \
    "options." )
#define SCODEC_TEXT N_("Destination subtitles codec")
#define SCODEC_LONGTEXT N_( \
    "Allows you to specify the destination subtitles codec used for the " \
    "streaming output." )
#define SFILTER_TEXT N_("Subpictures filter")
#define SFILTER_LONGTEXT N_( \
    "Allows you to specify subpictures filters used during the video " \
    "transcoding. The subpictures produced by the filters will be overlayed " \
    "directly onto the video." )

#define THREADS_TEXT N_("Number of threads")
#define THREADS_LONGTEXT N_( \
    "Allows you to specify the number of threads used for the transcoding." )

#define ASYNC_TEXT N_("Synchronise on audio track")
#define ASYNC_LONGTEXT N_( \
    "This option will drop/duplicate video frames to synchronise the video " \
    "track on the audio track." )

static int  Open ( vlc_object_t * );
static void Close( vlc_object_t * );

#define SOUT_CFG_PREFIX "sout-transcode-"

vlc_module_begin();
    set_description( _("Transcode stream output") );
    set_capability( "sout stream", 50 );
    add_shortcut( "transcode" );
    set_callbacks( Open, Close );

    add_string( SOUT_CFG_PREFIX "venc", NULL, NULL, VENC_TEXT,
                VENC_LONGTEXT, VLC_FALSE );
    add_string( SOUT_CFG_PREFIX "vcodec", NULL, NULL, VCODEC_TEXT,
                VCODEC_LONGTEXT, VLC_FALSE );
    add_integer( SOUT_CFG_PREFIX "vb", 800 * 1000, NULL, VB_TEXT,
                 VB_LONGTEXT, VLC_FALSE );
    add_float( SOUT_CFG_PREFIX "scale", 1, NULL, SCALE_TEXT,
               SCALE_LONGTEXT, VLC_FALSE );
    add_float( SOUT_CFG_PREFIX "fps", 0, NULL, FPS_TEXT,
               FPS_LONGTEXT, VLC_FALSE );
    add_bool( SOUT_CFG_PREFIX "deinterlace", 0, NULL, DEINTERLACE_TEXT,
              DEINTERLACE_LONGTEXT, VLC_FALSE );
    add_integer( SOUT_CFG_PREFIX "width", 0, NULL, WIDTH_TEXT,
                 WIDTH_LONGTEXT, VLC_TRUE );
    add_integer( SOUT_CFG_PREFIX "height", 0, NULL, HEIGHT_TEXT,
                 HEIGHT_LONGTEXT, VLC_TRUE );

    add_integer( SOUT_CFG_PREFIX "croptop", 0, NULL, CROPTOP_TEXT,
                 CROPTOP_LONGTEXT, VLC_TRUE );
    add_integer( SOUT_CFG_PREFIX "cropleft", 0, NULL, CROPLEFT_TEXT,
                 CROPLEFT_LONGTEXT, VLC_TRUE );
    add_integer( SOUT_CFG_PREFIX "cropbottom", 0, NULL, CROPBOTTOM_TEXT,
                 CROPBOTTOM_LONGTEXT, VLC_TRUE );
    add_integer( SOUT_CFG_PREFIX "cropright", 0, NULL, CROPRIGHT_TEXT,
                 CROPRIGHT_LONGTEXT, VLC_TRUE );

    add_string( SOUT_CFG_PREFIX "aenc", NULL, NULL, AENC_TEXT,
                AENC_LONGTEXT, VLC_FALSE );
    add_string( SOUT_CFG_PREFIX "acodec", NULL, NULL, ACODEC_TEXT,
                ACODEC_LONGTEXT, VLC_FALSE );
    add_integer( SOUT_CFG_PREFIX "ab", 64000, NULL, AB_TEXT,
                 AB_LONGTEXT, VLC_FALSE );
    add_integer( SOUT_CFG_PREFIX "channels", 0, NULL, ACHANS_TEXT,
                 ACHANS_LONGTEXT, VLC_FALSE );
    add_integer( SOUT_CFG_PREFIX "samplerate", 0, NULL, ARATE_TEXT,
                 ARATE_LONGTEXT, VLC_TRUE );

    add_string( SOUT_CFG_PREFIX "senc", NULL, NULL, SENC_TEXT,
                SENC_LONGTEXT, VLC_FALSE );
    add_string( SOUT_CFG_PREFIX "scodec", NULL, NULL, SCODEC_TEXT,
                SCODEC_LONGTEXT, VLC_FALSE );
    add_bool( SOUT_CFG_PREFIX "soverlay", 0, NULL, SCODEC_TEXT,
               SCODEC_LONGTEXT, VLC_FALSE );
    add_string( SOUT_CFG_PREFIX "sfilter", NULL, NULL, SFILTER_TEXT,
                SFILTER_LONGTEXT, VLC_FALSE );

    add_integer( SOUT_CFG_PREFIX "threads", 0, NULL, THREADS_TEXT,
                 THREADS_LONGTEXT, VLC_TRUE );

    add_bool( SOUT_CFG_PREFIX "audio-sync", 0, NULL, ASYNC_TEXT,
              ASYNC_LONGTEXT, VLC_FALSE );
vlc_module_end();

static const char *ppsz_sout_options[] = {
    "venc", "vcodec", "vb", "croptop", "cropbottom", "cropleft", "cropright",
    "scale", "fps", "width", "height", "deinterlace", "threads",
    "aenc", "acodec", "ab", "samplerate", "channels",
    "senc", "scodec", "soverlay", "sfilter",
    "audio-sync", NULL
};

/*****************************************************************************
 * Exported prototypes
 *****************************************************************************/
static sout_stream_id_t *Add ( sout_stream_t *, es_format_t * );
static int               Del ( sout_stream_t *, sout_stream_id_t * );
static int               Send( sout_stream_t *, sout_stream_id_t *, block_t* );

static int  transcode_audio_new    ( sout_stream_t *, sout_stream_id_t * );
static void transcode_audio_close  ( sout_stream_t *, sout_stream_id_t * );
static int  transcode_audio_process( sout_stream_t *, sout_stream_id_t *,
                                     block_t *, block_t ** );

static aout_buffer_t *audio_new_buffer( decoder_t *, int );
static void audio_del_buffer( decoder_t *, aout_buffer_t * );

static int  transcode_video_new    ( sout_stream_t *, sout_stream_id_t * );
static void transcode_video_close  ( sout_stream_t *, sout_stream_id_t * );
static int  transcode_video_encoder_open( sout_stream_t *, sout_stream_id_t *);
static int  transcode_video_process( sout_stream_t *, sout_stream_id_t *,
                                     block_t *, block_t ** );

static void video_del_buffer( vlc_object_t *, picture_t * );
static picture_t *video_new_buffer_decoder( decoder_t * );
static void video_del_buffer_decoder( decoder_t *, picture_t * );
static void video_link_picture_decoder( decoder_t *, picture_t * );
static void video_unlink_picture_decoder( decoder_t *, picture_t * );
static picture_t *video_new_buffer_filter( filter_t * );
static void video_del_buffer_filter( filter_t *, picture_t * );

static int  transcode_spu_new    ( sout_stream_t *, sout_stream_id_t * );
static void transcode_spu_close  ( sout_stream_t *, sout_stream_id_t * );
static int  transcode_spu_process( sout_stream_t *, sout_stream_id_t *,
                                   block_t *, block_t ** );

static int  EncoderThread( struct sout_stream_sys_t * p_sys );

static int pi_channels_maps[6] =
{
    0,
    AOUT_CHAN_CENTER,   AOUT_CHAN_LEFT | AOUT_CHAN_RIGHT,
    AOUT_CHAN_CENTER | AOUT_CHAN_LEFT | AOUT_CHAN_RIGHT,
    AOUT_CHAN_LEFT | AOUT_CHAN_RIGHT | AOUT_CHAN_REARLEFT
     | AOUT_CHAN_REARRIGHT,
    AOUT_CHAN_LEFT | AOUT_CHAN_RIGHT | AOUT_CHAN_CENTER
     | AOUT_CHAN_REARLEFT | AOUT_CHAN_REARRIGHT
};

#define PICTURE_RING_SIZE 64
#define SUBPICTURE_RING_SIZE 20

struct sout_stream_sys_t
{
    VLC_COMMON_MEMBERS

    sout_stream_t   *p_out;
    sout_stream_id_t *id_video;
    block_t         *p_buffers;
    vlc_mutex_t     lock_out;
    vlc_cond_t      cond;
    picture_t *     pp_pics[PICTURE_RING_SIZE];
    int             i_first_pic, i_last_pic;

    /* Audio */
    vlc_fourcc_t    i_acodec;   /* codec audio (0 if not transcode) */
    char            *psz_aenc;
    sout_cfg_t      *p_audio_cfg;
    int             i_sample_rate;
    int             i_channels;
    int             i_abitrate;

    /* Video */
    vlc_fourcc_t    i_vcodec;   /* codec video (0 if not transcode) */
    char            *psz_venc;
    sout_cfg_t      *p_video_cfg;
    int             i_vbitrate;
    double          f_scale;
    double          f_fps;
    int             i_width;
    int             i_height;
    vlc_bool_t      b_deinterlace;
    int             i_threads;

    int             i_crop_top;
    int             i_crop_bottom;
    int             i_crop_right;
    int             i_crop_left;

    /* SPU */
    vlc_fourcc_t    i_scodec;   /* codec spu (0 if not transcode) */
    char            *psz_senc;
    vlc_bool_t      b_soverlay;
    sout_cfg_t      *p_spu_cfg;
    spu_t           *p_spu;

    /* Sync */
    vlc_bool_t      b_master_sync;
    mtime_t         i_master_drift;
};

struct decoder_owner_sys_t
{
    picture_t *pp_pics[PICTURE_RING_SIZE];
};
struct filter_owner_sys_t
{
    picture_t *pp_pics[PICTURE_RING_SIZE];
};

/*****************************************************************************
 * Open:
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    sout_stream_t     *p_stream = (sout_stream_t*)p_this;
    sout_stream_sys_t *p_sys;
    vlc_value_t       val;

    p_sys = vlc_object_create( p_this, sizeof( sout_stream_sys_t ) );

    p_sys->p_out = sout_StreamNew( p_stream->p_sout, p_stream->psz_next );
    if( !p_sys->p_out )
    {
        msg_Err( p_stream, "cannot create chain" );
        free( p_sys );
        return VLC_EGENERIC;
    }

    p_sys->i_master_drift = 0;

    sout_CfgParse( p_stream, SOUT_CFG_PREFIX, ppsz_sout_options,
                   p_stream->p_cfg );

    /* Audio transcoding parameters */
    var_Get( p_stream, SOUT_CFG_PREFIX "aenc", &val );
    p_sys->psz_aenc = NULL;
    p_sys->p_audio_cfg = NULL;
    if( val.psz_string && *val.psz_string )
    {
        char *psz_next;
        psz_next = sout_CfgCreate( &p_sys->psz_aenc, &p_sys->p_audio_cfg,
                                   val.psz_string );
        if( psz_next ) free( psz_next );
    }
    if( val.psz_string ) free( val.psz_string );

    var_Get( p_stream, SOUT_CFG_PREFIX "acodec", &val );
    p_sys->i_acodec = 0;
    if( val.psz_string && *val.psz_string )
    {
        char fcc[4] = "    ";
        memcpy( fcc, val.psz_string, __MIN( strlen( val.psz_string ), 4 ) );
        p_sys->i_acodec = VLC_FOURCC( fcc[0], fcc[1], fcc[2], fcc[3] );
    }
    if( val.psz_string ) free( val.psz_string );

    var_Get( p_stream, SOUT_CFG_PREFIX "ab", &val );
    p_sys->i_abitrate = val.i_int;
    if( p_sys->i_abitrate < 4000 ) p_sys->i_abitrate *= 1000;

    var_Get( p_stream, SOUT_CFG_PREFIX "samplerate", &val );
    p_sys->i_sample_rate = val.i_int;

    var_Get( p_stream, SOUT_CFG_PREFIX "channels", &val );
    p_sys->i_channels = val.i_int;

    if( p_sys->i_acodec )
    {
        msg_Dbg( p_stream, "codec audio=%4.4s %dHz %d channels %dKb/s",
                 (char *)&p_sys->i_acodec, p_sys->i_sample_rate,
                 p_sys->i_channels, p_sys->i_abitrate / 1000 );
    }

    /* Video transcoding parameters */
    var_Get( p_stream, SOUT_CFG_PREFIX "venc", &val );
    p_sys->psz_venc = NULL;
    p_sys->p_video_cfg = NULL;
    if( val.psz_string && *val.psz_string )
    {
        char *psz_next;
        psz_next = sout_CfgCreate( &p_sys->psz_venc, &p_sys->p_video_cfg,
                                   val.psz_string );
        if( psz_next ) free( psz_next );
    }
    if( val.psz_string ) free( val.psz_string );

    var_Get( p_stream, SOUT_CFG_PREFIX "vcodec", &val );
    p_sys->i_vcodec = 0;
    if( val.psz_string && *val.psz_string )
    {
        char fcc[4] = "    ";
        memcpy( fcc, val.psz_string, __MIN( strlen( val.psz_string ), 4 ) );
        p_sys->i_vcodec = VLC_FOURCC( fcc[0], fcc[1], fcc[2], fcc[3] );
    }
    if( val.psz_string ) free( val.psz_string );

    var_Get( p_stream, SOUT_CFG_PREFIX "vb", &val );
    p_sys->i_vbitrate = val.i_int;
    if( p_sys->i_vbitrate < 16000 ) p_sys->i_vbitrate *= 1000;

    var_Get( p_stream, SOUT_CFG_PREFIX "scale", &val );
    p_sys->f_scale = val.f_float;

    var_Get( p_stream, SOUT_CFG_PREFIX "fps", &val );
    p_sys->f_fps = val.f_float;

    var_Get( p_stream, SOUT_CFG_PREFIX "width", &val );
    p_sys->i_width = val.i_int;

    var_Get( p_stream, SOUT_CFG_PREFIX "height", &val );
    p_sys->i_height = val.i_int;

    var_Get( p_stream, SOUT_CFG_PREFIX "deinterlace", &val );
    p_sys->b_deinterlace = val.b_bool;

    var_Get( p_stream, SOUT_CFG_PREFIX "croptop", &val );
    p_sys->i_crop_top = val.i_int;
    var_Get( p_stream, SOUT_CFG_PREFIX "cropbottom", &val );
    p_sys->i_crop_bottom = val.i_int;
    var_Get( p_stream, SOUT_CFG_PREFIX "cropleft", &val );
    p_sys->i_crop_left = val.i_int;
    var_Get( p_stream, SOUT_CFG_PREFIX "cropright", &val );
    p_sys->i_crop_right = val.i_int;

    var_Get( p_stream, SOUT_CFG_PREFIX "threads", &val );
    p_sys->i_threads = val.i_int;

    if( p_sys->i_vcodec )
    {
        msg_Dbg( p_stream, "codec video=%4.4s %dx%d scaling: %f %dkb/s",
                 (char *)&p_sys->i_vcodec, p_sys->i_width, p_sys->i_height,
                 p_sys->f_scale, p_sys->i_vbitrate / 1000 );
    }

    /* Subpictures transcoding parameters */
    p_sys->p_spu = 0;
    p_sys->psz_senc = NULL;
    p_sys->p_spu_cfg = NULL;
    p_sys->i_scodec = 0;

    var_Get( p_stream, SOUT_CFG_PREFIX "senc", &val );
    if( val.psz_string && *val.psz_string )
    {
        char *psz_next;
        psz_next = sout_CfgCreate( &p_sys->psz_senc, &p_sys->p_spu_cfg,
                                   val.psz_string );
        if( psz_next ) free( psz_next );
    }
    if( val.psz_string ) free( val.psz_string );

    var_Get( p_stream, SOUT_CFG_PREFIX "scodec", &val );
    if( val.psz_string && *val.psz_string )
    {
        char fcc[4] = "    ";
        memcpy( fcc, val.psz_string, __MIN( strlen( val.psz_string ), 4 ) );
        p_sys->i_scodec = VLC_FOURCC( fcc[0], fcc[1], fcc[2], fcc[3] );
    }
    if( val.psz_string ) free( val.psz_string );

    if( p_sys->i_scodec )
    {
        msg_Dbg( p_stream, "codec spu=%4.4s", (char *)&p_sys->i_acodec );
    }

    var_Get( p_stream, SOUT_CFG_PREFIX "soverlay", &val );
    p_sys->b_soverlay = val.b_bool;

    var_Get( p_stream, SOUT_CFG_PREFIX "sfilter", &val );
    if( val.psz_string && *val.psz_string )
    {
        p_sys->p_spu = spu_Create( p_stream );
        var_Create( p_sys->p_spu, "sub-filter", VLC_VAR_STRING );
        var_Set( p_sys->p_spu, "sub-filter", val );
        spu_Init( p_sys->p_spu );
    }
    if( val.psz_string ) free( val.psz_string );

    var_Get( p_stream, SOUT_CFG_PREFIX "audio-sync", &val );
    p_sys->b_master_sync = val.b_bool;
    if( p_sys->f_fps > 0 ) p_sys->b_master_sync = VLC_TRUE;

    p_stream->pf_add    = Add;
    p_stream->pf_del    = Del;
    p_stream->pf_send   = Send;
    p_stream->p_sys     = p_sys;

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close:
 *****************************************************************************/
static void Close( vlc_object_t * p_this )
{
    sout_stream_t       *p_stream = (sout_stream_t*)p_this;
    sout_stream_sys_t   *p_sys = p_stream->p_sys;

    sout_StreamDelete( p_sys->p_out );

    while( p_sys->p_audio_cfg != NULL )
    {
        sout_cfg_t *p_next = p_sys->p_audio_cfg->p_next;

        if( p_sys->p_audio_cfg->psz_name )
            free( p_sys->p_audio_cfg->psz_name );
        if( p_sys->p_audio_cfg->psz_value )
            free( p_sys->p_audio_cfg->psz_value );
        free( p_sys->p_audio_cfg );

        p_sys->p_audio_cfg = p_next;
    }
    if( p_sys->psz_aenc ) free( p_sys->psz_aenc );

    while( p_sys->p_video_cfg != NULL )
    {
        sout_cfg_t *p_next = p_sys->p_video_cfg->p_next;

        if( p_sys->p_video_cfg->psz_name )
            free( p_sys->p_video_cfg->psz_name );
        if( p_sys->p_video_cfg->psz_value )
            free( p_sys->p_video_cfg->psz_value );
        free( p_sys->p_video_cfg );

        p_sys->p_video_cfg = p_next;
    }
    if( p_sys->psz_venc ) free( p_sys->psz_venc );

    while( p_sys->p_spu_cfg != NULL )
    {
        sout_cfg_t *p_next = p_sys->p_spu_cfg->p_next;

        if( p_sys->p_spu_cfg->psz_name )
            free( p_sys->p_spu_cfg->psz_name );
        if( p_sys->p_spu_cfg->psz_value )
            free( p_sys->p_spu_cfg->psz_value );
        free( p_sys->p_spu_cfg );

        p_sys->p_spu_cfg = p_next;
    }
    if( p_sys->psz_senc ) free( p_sys->psz_senc );

    if( p_sys->p_spu ) spu_Destroy( p_sys->p_spu );

    vlc_object_destroy( p_sys );
}

struct sout_stream_id_t
{
    vlc_fourcc_t  b_transcode;

    /* id of the out stream */
    void *id;

    /* Decoder */
    decoder_t       *p_decoder;

    /* Filters */
    filter_t        *pp_filter[10];
    int             i_filter;

    /* Encoder */
    encoder_t       *p_encoder;

    /* Sync */
    date_t          interpolated_pts;
};


static sout_stream_id_t *Add( sout_stream_t *p_stream, es_format_t *p_fmt )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;
    sout_stream_id_t *id;

    id = malloc( sizeof( sout_stream_id_t ) );
    memset( id, 0, sizeof(sout_stream_id_t) );

    id->id = NULL;
    id->p_decoder = NULL;
    id->p_encoder = NULL;

    /* Create decoder object */
    id->p_decoder = vlc_object_create( p_stream, VLC_OBJECT_DECODER );
    if( !id->p_decoder )
    {
        msg_Err( p_stream, "out of memory" );
        goto error;
    }
    vlc_object_attach( id->p_decoder, p_stream );
    id->p_decoder->p_module = NULL;
    id->p_decoder->fmt_in = *p_fmt;
    id->p_decoder->fmt_out = *p_fmt;
    id->p_decoder->fmt_out.i_extra = 0;
    id->p_decoder->fmt_out.p_extra = 0;
    id->p_decoder->b_pace_control = VLC_TRUE;

    /* Create encoder object */
    id->p_encoder = vlc_object_create( p_stream, VLC_OBJECT_ENCODER );
    if( !id->p_encoder )
    {
        msg_Err( p_stream, "out of memory" );
        goto error;
    }
    vlc_object_attach( id->p_encoder, p_stream );
    id->p_encoder->p_module = NULL;

    /* Create destination format */
    es_format_Init( &id->p_encoder->fmt_out, p_fmt->i_cat, 0 );
    id->p_encoder->fmt_out.i_id    = p_fmt->i_id;
    id->p_encoder->fmt_out.i_group = p_fmt->i_group;
    if( p_fmt->psz_language )
        id->p_encoder->fmt_out.psz_language = strdup( p_fmt->psz_language );

    if( p_fmt->i_cat == AUDIO_ES && (p_sys->i_acodec || p_sys->psz_aenc) )
    {
        msg_Dbg( p_stream,
                 "creating audio transcoding from fcc=`%4.4s' to fcc=`%4.4s'",
                 (char*)&p_fmt->i_codec, (char*)&p_sys->i_acodec );

        /* Complete destination format */
        id->p_encoder->fmt_out.i_codec = p_sys->i_acodec;
        id->p_encoder->fmt_out.audio.i_rate = p_sys->i_sample_rate > 0 ?
            p_sys->i_sample_rate : (int)p_fmt->audio.i_rate;
        id->p_encoder->fmt_out.audio.i_channels = p_sys->i_channels > 0 ?
            p_sys->i_channels : p_fmt->audio.i_channels;
        id->p_encoder->fmt_out.i_bitrate = p_sys->i_abitrate;
        id->p_encoder->fmt_out.audio.i_bitspersample =
            p_fmt->audio.i_bitspersample;

        /* Build decoder -> filter -> encoder chain */
        if( transcode_audio_new( p_stream, id ) )
        {
            msg_Err( p_stream, "cannot create audio chain" );
            goto error;
        }

        /* Open output stream */
        id->id = p_sys->p_out->pf_add( p_sys->p_out, &id->p_encoder->fmt_out );
        id->b_transcode = VLC_TRUE;

        if( !id->id ) goto error;

        date_Init( &id->interpolated_pts, p_fmt->audio.i_rate, 1 );
    }
    else if( p_fmt->i_cat == VIDEO_ES &&
             (p_sys->i_vcodec != 0 || p_sys->psz_venc) )
    {
        msg_Dbg( p_stream,
                 "creating video transcoding from fcc=`%4.4s' to fcc=`%4.4s'",
                 (char*)&p_fmt->i_codec, (char*)&p_sys->i_vcodec );

        /* Complete destination format */
        id->p_encoder->fmt_out.i_codec = p_sys->i_vcodec;
        id->p_encoder->fmt_out.video.i_width  = p_sys->i_width;
        id->p_encoder->fmt_out.video.i_height = p_sys->i_height;
        id->p_encoder->fmt_out.i_bitrate = p_sys->i_vbitrate;

        /* Build decoder -> filter -> encoder chain */
        if( transcode_video_new( p_stream, id ) )
        {
            msg_Err( p_stream, "cannot create video chain" );
            goto error;
        }

        /* Stream will be added later on because we don't know
         * all the characteristics of the decoded stream yet */
        id->b_transcode = VLC_TRUE;

        if( p_sys->f_fps > 0 )
        {
            id->p_encoder->fmt_out.video.i_frame_rate = p_sys->f_fps * 1000;
            id->p_encoder->fmt_out.video.i_frame_rate_base = 1000;
        }
    }
    else if( p_fmt->i_cat == SPU_ES && (p_sys->i_scodec || p_sys->psz_senc) )
    {
        msg_Dbg( p_stream, "creating subtitles transcoding from fcc=`%4.4s' "
                 "to fcc=`%4.4s'", (char*)&p_fmt->i_codec,
                 (char*)&p_sys->i_scodec );

        /* Complete destination format */
        id->p_encoder->fmt_out.i_codec = p_sys->i_scodec;

        /* build decoder -> filter -> encoder */
        if( transcode_spu_new( p_stream, id ) )
        {
            msg_Err( p_stream, "cannot create subtitles chain" );
            goto error;
        }

        /* open output stream */
        id->id = p_sys->p_out->pf_add( p_sys->p_out, &id->p_encoder->fmt_out );
        id->b_transcode = VLC_TRUE;

        if( !id->id ) goto error;
    }
    else if( p_fmt->i_cat == SPU_ES && p_sys->b_soverlay )
    {
        msg_Dbg( p_stream, "subtitles (fcc=`%4.4s') overlaying",
                 (char*)&p_fmt->i_codec );

        id->b_transcode = VLC_TRUE;

        /* Build decoder -> filter -> overlaying chain */
        if( transcode_spu_new( p_stream, id ) )
        {
            msg_Err( p_stream, "cannot create subtitles chain" );
            goto error;
        }
    }
    else
    {
        msg_Dbg( p_stream, "not transcoding a stream (fcc=`%4.4s')",
                 (char*)&p_fmt->i_codec );
        id->id = p_sys->p_out->pf_add( p_sys->p_out, p_fmt );
        id->b_transcode = VLC_FALSE;

        if( !id->id ) goto error;
    }

    return id;

 error:
    if( id->p_decoder )
    {
        vlc_object_detach( id->p_decoder );
        vlc_object_destroy( id->p_decoder );
    }

    if( id->p_encoder )
    {
        vlc_object_detach( id->p_encoder );
        vlc_object_destroy( id->p_encoder );
    }

    free( id );
    return NULL;
}

static int Del( sout_stream_t *p_stream, sout_stream_id_t *id )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;

    if( id->b_transcode )
    {
        switch( id->p_decoder->fmt_in.i_cat )
        {
        case AUDIO_ES:
            transcode_audio_close( p_stream, id );
            break;
        case VIDEO_ES:
            transcode_video_close( p_stream, id );
            break;
        case SPU_ES:
            transcode_spu_close( p_stream, id );
            break;
        }
    }

    if( id->id ) p_sys->p_out->pf_del( p_sys->p_out, id->id );

    if( id->p_decoder )
    {
        vlc_object_detach( id->p_decoder );
        vlc_object_destroy( id->p_decoder );
    }

    if( id->p_encoder )
    {
        vlc_object_detach( id->p_encoder );
        vlc_object_destroy( id->p_encoder );
    }

    free( id );

    return VLC_SUCCESS;
}

static int Send( sout_stream_t *p_stream, sout_stream_id_t *id,
                 block_t *p_buffer )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;
    block_t *p_out;

    if( !id->b_transcode && id->id )
    {
        if( p_sys->b_master_sync && p_sys->i_master_drift )
        {
            if( p_buffer->i_dts > 0 )
            {
                p_buffer->i_dts -= p_sys->i_master_drift;
                if( p_buffer->i_dts < 0 )
                {
                    block_Release( p_buffer );
                    return VLC_EGENERIC;
                }
            }
            if( p_buffer->i_pts > 0 )
            {
                p_buffer->i_pts -= p_sys->i_master_drift;
                if( p_buffer->i_pts < 0 )
                {
                    block_Release( p_buffer );
                    return VLC_EGENERIC;
                }
            }
        }

        return p_sys->p_out->pf_send( p_sys->p_out, id->id, p_buffer );
    }
    else if( !id->b_transcode )
    {
        block_Release( p_buffer );
        return VLC_EGENERIC;
    }

    switch( id->p_decoder->fmt_in.i_cat )
    {
    case AUDIO_ES:
        transcode_audio_process( p_stream, id, p_buffer, &p_out );
        break;

    case VIDEO_ES:
        if( transcode_video_process( p_stream, id, p_buffer, &p_out )
            != VLC_SUCCESS )
        {
            return VLC_EGENERIC;
        }
        break;

    case SPU_ES:
        if( transcode_spu_process( p_stream, id, p_buffer, &p_out ) !=
            VLC_SUCCESS )
        {
            return VLC_EGENERIC;
        }
        break;

    default:
        block_Release( p_buffer );
        break;
    }

    if( p_out ) return p_sys->p_out->pf_send( p_sys->p_out, id->id, p_out );
    return VLC_SUCCESS;
}

/****************************************************************************
 * decoder reencoder part
 ****************************************************************************/
int audio_BitsPerSample( vlc_fourcc_t i_format )
{
    switch( i_format )
    {
    case VLC_FOURCC('u','8',' ',' '):
    case VLC_FOURCC('s','8',' ',' '):
        return 8;

    case VLC_FOURCC('u','1','6','l'):
    case VLC_FOURCC('s','1','6','l'):
    case VLC_FOURCC('u','1','6','b'):
    case VLC_FOURCC('s','1','6','b'):
        return 16;

    case VLC_FOURCC('u','2','4','l'):
    case VLC_FOURCC('s','2','4','l'):
    case VLC_FOURCC('u','2','4','b'):
    case VLC_FOURCC('s','2','4','b'):
        return 24;

    case VLC_FOURCC('u','3','2','l'):
    case VLC_FOURCC('s','3','2','l'):
    case VLC_FOURCC('u','3','2','b'):
    case VLC_FOURCC('s','3','2','b'):
    case VLC_FOURCC('f','l','3','2'):
    case VLC_FOURCC('f','i','3','2'):
        return 32;

    case VLC_FOURCC('f','l','6','4'):
        return 64;
    }

    return 0;
}

static int transcode_audio_new( sout_stream_t *p_stream,
                                sout_stream_id_t *id )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;

    /*
     * Open decoder
     */

    /* Initialization of decoder structures */
    id->p_decoder->pf_decode_audio = 0;
    id->p_decoder->pf_aout_buffer_new = audio_new_buffer;
    id->p_decoder->pf_aout_buffer_del = audio_del_buffer;
    //id->p_decoder->p_cfg = p_sys->p_video_cfg;

    id->p_decoder->p_module =
        module_Need( id->p_decoder, "decoder", "$codec", 0 );

    if( !id->p_decoder->p_module )
    {
        msg_Err( p_stream, "cannot find decoder" );
        return VLC_EGENERIC;
    }
    id->p_decoder->fmt_out.audio.i_bitspersample = 
        audio_BitsPerSample( id->p_decoder->fmt_out.i_codec );

    /*
     * Open encoder
     */

    /* Initialization of encoder format structures */
    es_format_Init( &id->p_encoder->fmt_in, id->p_decoder->fmt_in.i_cat,
                    id->p_decoder->fmt_out.i_codec );
    id->p_encoder->fmt_in.audio.i_format = id->p_decoder->fmt_out.i_codec;

    /* Sanity check for audio channels */
    id->p_encoder->fmt_out.audio.i_channels =
        __MIN( id->p_encoder->fmt_out.audio.i_channels,
               id->p_decoder->fmt_out.audio.i_channels );
    if( id->p_decoder->fmt_out.audio.i_channels ==
        id->p_encoder->fmt_out.audio.i_channels )
        id->p_encoder->fmt_out.audio.i_physical_channels =
            id->p_encoder->fmt_out.audio.i_original_channels =
                id->p_decoder->fmt_out.audio.i_physical_channels;
    else
        id->p_encoder->fmt_out.audio.i_physical_channels =
            id->p_encoder->fmt_out.audio.i_original_channels =
                pi_channels_maps[id->p_encoder->fmt_out.audio.i_channels];

    /* Initialization of encoder format structures */
    es_format_Init( &id->p_encoder->fmt_in, AUDIO_ES, AOUT_FMT_S16_NE );
    id->p_encoder->fmt_in.audio.i_format = AOUT_FMT_S16_NE;
    id->p_encoder->fmt_in.audio.i_rate = id->p_encoder->fmt_out.audio.i_rate;
    id->p_encoder->fmt_in.audio.i_physical_channels =
        id->p_encoder->fmt_in.audio.i_original_channels =
            id->p_encoder->fmt_out.audio.i_physical_channels;
    id->p_encoder->fmt_in.audio.i_channels =
        id->p_encoder->fmt_out.audio.i_channels;
    id->p_encoder->fmt_in.audio.i_bitspersample =
        audio_BitsPerSample( id->p_encoder->fmt_in.i_codec );

    id->p_encoder->p_cfg = p_stream->p_sys->p_audio_cfg;

    id->p_encoder->p_module =
        module_Need( id->p_encoder, "encoder", p_sys->psz_aenc, VLC_TRUE );
    if( !id->p_encoder->p_module )
    {
        msg_Err( p_stream, "cannot find encoder" );
        module_Unneed( id->p_decoder, id->p_decoder->p_module );
        id->p_decoder->p_module = 0;
        return VLC_EGENERIC;
    }
    id->p_encoder->fmt_in.audio.i_format = id->p_encoder->fmt_in.i_codec;
    id->p_encoder->fmt_in.audio.i_bitspersample =
        audio_BitsPerSample( id->p_encoder->fmt_in.i_codec );

    /* Check if we need a filter for chroma conversion or resizing */
    if( id->p_decoder->fmt_out.i_codec !=
        id->p_encoder->fmt_in.i_codec )
    {
        id->pp_filter[0] =
            vlc_object_create( p_stream, VLC_OBJECT_FILTER );
        vlc_object_attach( id->pp_filter[0], p_stream );

        id->pp_filter[0]->pf_audio_buffer_new = __block_New;

        id->pp_filter[0]->fmt_in = id->p_decoder->fmt_out;
        id->pp_filter[0]->fmt_out = id->p_encoder->fmt_in;
        id->pp_filter[0]->p_module =
            module_Need( id->pp_filter[0], "audio filter2", 0, 0 );
        if( id->pp_filter[0]->p_module ) id->i_filter++;
        else
        {
            msg_Dbg( p_stream, "no audio filter found (%4.4s->%4.4s)",
                     (char *)&id->pp_filter[0]->fmt_in,
                     (char *)&id->pp_filter[0]->fmt_out );
            vlc_object_detach( id->pp_filter[0] );
            vlc_object_destroy( id->pp_filter[0] );
            module_Unneed( id->p_decoder, id->p_decoder->p_module );
            id->p_decoder->p_module = 0;
            module_Unneed( id->p_encoder, id->p_encoder->p_module );
            id->p_encoder->p_module = 0;
            return VLC_EGENERIC;
        }

        id->pp_filter[0]->fmt_out.audio.i_bitspersample = 
            audio_BitsPerSample( id->pp_filter[0]->fmt_out.i_codec );

        /* Try a 2 stage conversion */
        if( id->pp_filter[0]->fmt_out.i_codec !=
            id->p_encoder->fmt_in.i_codec )
        {
            id->pp_filter[1] =
                vlc_object_create( p_stream, VLC_OBJECT_FILTER );
            vlc_object_attach( id->pp_filter[1], p_stream );

            id->pp_filter[1]->pf_audio_buffer_new = __block_New;

            id->pp_filter[1]->fmt_in = id->pp_filter[0]->fmt_out;
            id->pp_filter[1]->fmt_out = id->p_encoder->fmt_in;
            id->pp_filter[1]->p_module =
              module_Need( id->pp_filter[1], "audio filter2", 0, 0 );
            if( !id->pp_filter[1]->p_module ||
                id->pp_filter[1]->fmt_out.i_codec !=
                  id->p_encoder->fmt_in.i_codec )
            {
                msg_Dbg( p_stream, "no audio filter found (%4.4s->%4.4s)",
                         (char *)&id->pp_filter[1]->fmt_in,
                         (char *)&id->pp_filter[1]->fmt_out );
                module_Unneed( id->pp_filter[0], id->pp_filter[0]->p_module );
                vlc_object_detach( id->pp_filter[0] );
                vlc_object_destroy( id->pp_filter[0] );
                if( id->pp_filter[1]->p_module )
                module_Unneed( id->pp_filter[0], id->pp_filter[0]->p_module );
                vlc_object_detach( id->pp_filter[1] );
                vlc_object_destroy( id->pp_filter[1] );
                module_Unneed( id->p_decoder, id->p_decoder->p_module );
                id->p_decoder->p_module = 0;
                module_Unneed( id->p_encoder, id->p_encoder->p_module );
                id->p_encoder->p_module = 0;
                return VLC_EGENERIC;
            }
            else id->i_filter++;
        }
    }

    /* FIXME: Hack for mp3 transcoding support */
    if( id->p_encoder->fmt_out.i_codec == VLC_FOURCC( 'm','p','3',' ' ) )
        id->p_encoder->fmt_out.i_codec = VLC_FOURCC( 'm','p','g','a' );

    return VLC_SUCCESS;
}

static void transcode_audio_close( sout_stream_t *p_stream,
                                   sout_stream_id_t *id )
{
    int i;

    /* Close decoder */
    if( id->p_decoder->p_module )
        module_Unneed( id->p_decoder, id->p_decoder->p_module );

    /* Close encoder */
    if( id->p_encoder->p_module )
        module_Unneed( id->p_encoder, id->p_encoder->p_module );

    /* Close filters */
    for( i = 0; i < id->i_filter; i++ )
    {
        vlc_object_detach( id->pp_filter[i] );
        if( id->pp_filter[i]->p_module )
            module_Unneed( id->pp_filter[i], id->pp_filter[i]->p_module );
        vlc_object_destroy( id->pp_filter[i] );
    }
}

static int transcode_audio_process( sout_stream_t *p_stream,
                                    sout_stream_id_t *id,
                                    block_t *in, block_t **out )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;
    aout_buffer_t *p_audio_buf;
    block_t *p_block, *p_audio_block;
    int i;
    *out = NULL;

    while( (p_audio_buf = id->p_decoder->pf_decode_audio( id->p_decoder,
                                                          &in )) )
    {
        if( p_sys->b_master_sync )
        {
            mtime_t i_dts = date_Get( &id->interpolated_pts ) + 1;
            p_sys->i_master_drift = p_audio_buf->start_date - i_dts;
            date_Increment( &id->interpolated_pts, p_audio_buf->i_nb_samples );
            p_audio_buf->start_date -= p_sys->i_master_drift;
            p_audio_buf->end_date -= p_sys->i_master_drift;
        }

        p_audio_block = p_audio_buf->p_sys;
        p_audio_block->i_buffer = p_audio_buf->i_nb_bytes;
        p_audio_block->i_dts = p_audio_block->i_pts =
            p_audio_buf->start_date;
        p_audio_block->i_length = p_audio_buf->end_date -
            p_audio_buf->start_date;
        p_audio_block->i_samples = p_audio_buf->i_nb_samples;

        /* Run filter chain */
        for( i = 0; i < id->i_filter; i++ )
        {
            p_audio_block =
                id->pp_filter[i]->pf_audio_filter( id->pp_filter[i],
                                                   p_audio_block );
        }

        p_audio_buf->p_buffer = p_audio_block->p_buffer;
        p_audio_buf->i_nb_bytes = p_audio_block->i_buffer;
        p_audio_buf->i_nb_samples = p_audio_block->i_samples;
        p_audio_buf->start_date = p_audio_block->i_dts;
        p_audio_buf->end_date = p_audio_block->i_dts + p_audio_block->i_length;

        p_block = id->p_encoder->pf_encode_audio( id->p_encoder, p_audio_buf );
        block_ChainAppend( out, p_block );
        block_Release( p_audio_block );
        free( p_audio_buf );
    }

    return VLC_SUCCESS;
}

static void audio_release_buffer( aout_buffer_t *p_buffer )
{
    if( p_buffer && p_buffer->p_sys ) block_Release( p_buffer->p_sys );
    if( p_buffer ) free( p_buffer );
}

static aout_buffer_t *audio_new_buffer( decoder_t *p_dec, int i_samples )
{
    aout_buffer_t *p_buffer;
    block_t *p_block;
    int i_size;

    if( p_dec->fmt_out.audio.i_bitspersample )
    {
        i_size = i_samples * p_dec->fmt_out.audio.i_bitspersample / 8 *
            p_dec->fmt_out.audio.i_channels;
    }
    else if( p_dec->fmt_out.audio.i_bytes_per_frame &&
             p_dec->fmt_out.audio.i_frame_length )
    {
        i_size = i_samples * p_dec->fmt_out.audio.i_bytes_per_frame /
            p_dec->fmt_out.audio.i_frame_length;
    }
    else
    {
        i_size = i_samples * 4 * p_dec->fmt_out.audio.i_channels;
    }

    p_buffer = malloc( sizeof(aout_buffer_t) );
    p_buffer->pf_release = audio_release_buffer;
    p_buffer->p_sys = p_block = block_New( p_dec, i_size );

    p_buffer->p_buffer = p_block->p_buffer;
    p_buffer->i_size = p_buffer->i_nb_bytes = p_block->i_buffer;
    p_buffer->i_nb_samples = i_samples;
    p_block->i_samples = i_samples;

    return p_buffer;
}

static void audio_del_buffer( decoder_t *p_dec, aout_buffer_t *p_buffer )
{
    if( p_buffer && p_buffer->p_sys ) block_Release( p_buffer->p_sys );
    if( p_buffer ) free( p_buffer );
}

/*
 * video
 */
static int transcode_video_new( sout_stream_t *p_stream, sout_stream_id_t *id )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;
    int i;

    /*
     * Open decoder
     */

    /* Initialization of decoder structures */
    id->p_decoder->pf_decode_video = 0;
    id->p_decoder->pf_vout_buffer_new = video_new_buffer_decoder;
    id->p_decoder->pf_vout_buffer_del = video_del_buffer_decoder;
    id->p_decoder->pf_picture_link    = video_link_picture_decoder;
    id->p_decoder->pf_picture_unlink  = video_unlink_picture_decoder;
    id->p_decoder->p_owner = malloc( sizeof(decoder_owner_sys_t) );
    for( i = 0; i < PICTURE_RING_SIZE; i++ )
        id->p_decoder->p_owner->pp_pics[i] = 0;
    //id->p_decoder->p_cfg = p_sys->p_video_cfg;

    id->p_decoder->p_module =
        module_Need( id->p_decoder, "decoder", "$codec", 0 );

    if( !id->p_decoder->p_module )
    {
        msg_Err( p_stream, "cannot find decoder" );
        return VLC_EGENERIC;
    }

    /*
     * Open encoder.
     * Because some info about the decoded input will only be available
     * once the first frame is decoded, we actually only test the availability
     * of the encoder here.
     */

    /* Initialization of encoder format structures */
    es_format_Init( &id->p_encoder->fmt_in, id->p_decoder->fmt_in.i_cat,
                    id->p_decoder->fmt_out.i_codec );
    id->p_encoder->fmt_in.video.i_chroma = id->p_decoder->fmt_out.i_codec;

    /* The dimensions will be set properly later on.
     * Just put sensible values so we can test an encoder is available. */
    id->p_encoder->fmt_in.video.i_width =
        id->p_encoder->fmt_out.video.i_width ?
        id->p_encoder->fmt_out.video.i_width :
        id->p_decoder->fmt_in.video.i_width ?
        id->p_decoder->fmt_in.video.i_width : 16;
    id->p_encoder->fmt_in.video.i_height =
        id->p_encoder->fmt_out.video.i_height ?
        id->p_encoder->fmt_out.video.i_height :
        id->p_decoder->fmt_in.video.i_height ?
        id->p_decoder->fmt_in.video.i_height : 16;
    id->p_encoder->fmt_in.video.i_frame_rate = 25;
    id->p_encoder->fmt_in.video.i_frame_rate_base = 1;

    id->p_encoder->i_threads = p_sys->i_threads;
    id->p_encoder->p_cfg = p_sys->p_video_cfg;

    id->p_encoder->p_module =
        module_Need( id->p_encoder, "encoder", p_sys->psz_venc, VLC_TRUE );
    if( !id->p_encoder->p_module )
    {
        msg_Err( p_stream, "cannot find encoder" );
        module_Unneed( id->p_decoder, id->p_decoder->p_module );
        id->p_decoder->p_module = 0;
        return VLC_EGENERIC;
    }

    /* Close the encoder.
     * We'll open it only when we have the first frame. */
    module_Unneed( id->p_encoder, id->p_encoder->p_module );
    id->p_encoder->p_module = NULL;

    if( p_sys->i_threads >= 1 )
    {
        p_sys->id_video = id;
        vlc_mutex_init( p_stream, &p_sys->lock_out );
        vlc_cond_init( p_stream, &p_sys->cond );
        memset( p_sys->pp_pics, 0, sizeof(p_sys->pp_pics) );
        p_sys->i_first_pic = 0;
        p_sys->i_last_pic = 0;
        p_sys->p_buffers = NULL;
        p_sys->b_die = p_sys->b_error = 0;
        if( vlc_thread_create( p_sys, "encoder", EncoderThread,
                               VLC_THREAD_PRIORITY_VIDEO, VLC_FALSE ) )
        {
            msg_Err( p_stream, "cannot spawn encoder thread" );
            module_Unneed( id->p_decoder, id->p_decoder->p_module );
            id->p_decoder->p_module = 0;
            return VLC_EGENERIC;
        }
    }

    date_Set( &id->interpolated_pts, 0 );

    return VLC_SUCCESS;
}

static int transcode_video_encoder_open( sout_stream_t *p_stream,
                                         sout_stream_id_t *id )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;

    /* Hack because of the copy packetizer which can fail to detect the
     * proper size (which forces us to wait until the 1st frame
     * is decoded) */
    int i_width = id->p_decoder->fmt_out.video.i_width -
        p_sys->i_crop_left - p_sys->i_crop_right;
    int i_height = id->p_decoder->fmt_out.video.i_height -
        p_sys->i_crop_top - p_sys->i_crop_bottom;

    if( id->p_encoder->fmt_out.video.i_width <= 0 &&
        id->p_encoder->fmt_out.video.i_height <= 0 && p_sys->f_scale )
    {
        /* Apply the scaling */
        id->p_encoder->fmt_out.video.i_width = i_width * p_sys->f_scale;
        id->p_encoder->fmt_out.video.i_height = i_height * p_sys->f_scale;
    }
    else if( id->p_encoder->fmt_out.video.i_width > 0 &&
             id->p_encoder->fmt_out.video.i_height <= 0 )
    {
        id->p_encoder->fmt_out.video.i_height =
            id->p_encoder->fmt_out.video.i_width / (double)i_width * i_height;
    }
    else if( id->p_encoder->fmt_out.video.i_width <= 0 &&
             id->p_encoder->fmt_out.video.i_height > 0 )
    {
        id->p_encoder->fmt_out.video.i_width =
            id->p_encoder->fmt_out.video.i_height / (double)i_height * i_width;
    }

    /* Make sure the size is at least a multiple of 2 */
    id->p_encoder->fmt_out.video.i_width =
        (id->p_encoder->fmt_out.video.i_width + 1) >> 1 << 1;
    id->p_encoder->fmt_out.video.i_height =
        (id->p_encoder->fmt_out.video.i_height + 1) >> 1 << 1;

    id->p_encoder->fmt_in.video.i_width =
        id->p_encoder->fmt_out.video.i_width;
    id->p_encoder->fmt_in.video.i_height =
        id->p_encoder->fmt_out.video.i_height;

    if( !id->p_encoder->fmt_out.video.i_frame_rate ||
        !id->p_encoder->fmt_out.video.i_frame_rate_base )
    {
        if( id->p_decoder->fmt_out.video.i_frame_rate &&
            id->p_decoder->fmt_out.video.i_frame_rate_base )
        {
            id->p_encoder->fmt_out.video.i_frame_rate =
                id->p_decoder->fmt_out.video.i_frame_rate;
            id->p_encoder->fmt_out.video.i_frame_rate_base =
                id->p_decoder->fmt_out.video.i_frame_rate_base;
        }
        else
        {
            /* Pick a sensible default value */
            id->p_encoder->fmt_out.video.i_frame_rate = 25;
            id->p_encoder->fmt_out.video.i_frame_rate_base = 1;
        }
    }

    id->p_encoder->fmt_in.video.i_frame_rate =
        id->p_encoder->fmt_out.video.i_frame_rate;
    id->p_encoder->fmt_in.video.i_frame_rate_base =
        id->p_encoder->fmt_out.video.i_frame_rate_base;

    date_Init( &id->interpolated_pts,
               id->p_encoder->fmt_out.video.i_frame_rate,
               id->p_encoder->fmt_out.video.i_frame_rate_base );

    /* Check whether a particular aspect ratio was requested */
    if( !id->p_encoder->fmt_out.video.i_aspect )
    {
        id->p_encoder->fmt_out.video.i_aspect =
            id->p_decoder->fmt_out.video.i_aspect;
    }
    id->p_encoder->fmt_in.video.i_aspect =
        id->p_encoder->fmt_out.video.i_aspect;

    id->p_encoder->p_module =
        module_Need( id->p_encoder, "encoder", p_sys->psz_venc, VLC_TRUE );
    if( !id->p_encoder->p_module )
    {
        msg_Err( p_stream, "cannot find encoder" );
        return VLC_EGENERIC;
    }

    id->p_encoder->fmt_in.video.i_chroma = id->p_encoder->fmt_in.i_codec;

    /* Hack for mp2v/mp1v transcoding support */
    if( id->p_encoder->fmt_out.i_codec == VLC_FOURCC('m','p','1','v') ||
        id->p_encoder->fmt_out.i_codec == VLC_FOURCC('m','p','2','v') )
    {
        id->p_encoder->fmt_out.i_codec = VLC_FOURCC('m','p','g','v');
    }

    id->id = p_stream->p_sys->p_out->pf_add( p_stream->p_sys->p_out,
                                             &id->p_encoder->fmt_out );
    if( !id->id )
    {
        msg_Err( p_stream, "cannot add this stream" );
        return VLC_EGENERIC;
    }

    return VLC_SUCCESS;
}

static void transcode_video_close( sout_stream_t *p_stream,
                                   sout_stream_id_t *id )
{
    int i, j;

    if( p_stream->p_sys->i_threads >= 1 )
    {
        vlc_mutex_lock( &p_stream->p_sys->lock_out );
        p_stream->p_sys->b_die = 1;
        vlc_cond_signal( &p_stream->p_sys->cond );
        vlc_mutex_unlock( &p_stream->p_sys->lock_out );
        vlc_thread_join( p_stream->p_sys );
        vlc_mutex_destroy( &p_stream->p_sys->lock_out );
        vlc_cond_destroy( &p_stream->p_sys->cond );
    }

    /* Close decoder */
    if( id->p_decoder->p_module )
        module_Unneed( id->p_decoder, id->p_decoder->p_module );

    if( id->p_decoder->p_owner )
    {
        /* Clean-up pictures ring buffer */
        for( i = 0; i < PICTURE_RING_SIZE; i++ )
        {
            if( id->p_decoder->p_owner->pp_pics[i] )
                video_del_buffer( VLC_OBJECT(id->p_decoder),
                                  id->p_decoder->p_owner->pp_pics[i] );
        }
        free( id->p_decoder->p_owner );
    }

    /* Close encoder */
    if( id->p_encoder->p_module )
        module_Unneed( id->p_encoder, id->p_encoder->p_module );

    /* Close filters */
    for( i = 0; i < id->i_filter; i++ )
    {
        vlc_object_detach( id->pp_filter[i] );
        if( id->pp_filter[i]->p_module )
            module_Unneed( id->pp_filter[i], id->pp_filter[i]->p_module );

        /* Clean-up pictures ring buffer */
        for( j = 0; j < PICTURE_RING_SIZE; j++ )
        {
            if( id->pp_filter[i]->p_owner->pp_pics[j] )
                video_del_buffer( VLC_OBJECT(id->pp_filter[i]),
                                  id->pp_filter[i]->p_owner->pp_pics[j] );
        }
        free( id->pp_filter[i]->p_owner );

        vlc_object_destroy( id->pp_filter[i] );
    }
}

static int transcode_video_process( sout_stream_t *p_stream,
                                    sout_stream_id_t *id,
                                    block_t *in, block_t **out )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;
    int i_duplicate = 1, i;
    picture_t *p_pic;
    *out = NULL;

    while( (p_pic = id->p_decoder->pf_decode_video( id->p_decoder, &in )) )
    {
        subpicture_t *p_subpic = 0;

        if( p_sys->b_master_sync )
        {
            mtime_t i_video_drift;
            mtime_t i_master_drift = p_sys->i_master_drift;
            mtime_t i_pts;

            if( !i_master_drift )
            {
                /* No audio track ? */
                p_sys->i_master_drift = i_master_drift = p_pic->date;
            }

            i_pts = date_Get( &id->interpolated_pts ) + 1;
            i_video_drift = p_pic->date - i_pts;
            i_duplicate = 1;

            /* Set the pts of the frame being encoded */
            p_pic->date = i_pts;

            if( i_video_drift < i_master_drift - 50000 )
            {
#if 0
                msg_Dbg( p_stream, "dropping frame (%i)",
                         (int)(i_video_drift - i_master_drift) );
#endif
                p_pic->pf_release( p_pic );
                return VLC_EGENERIC;
            }
            else if( i_video_drift > i_master_drift + 50000 )
            {
#if 0
                msg_Dbg( p_stream, "adding frame (%i)",
                         (int)(i_video_drift - i_master_drift) );
#endif
                i_duplicate = 2;
            }
        }

        if( !id->p_encoder->p_module )
        {
            if( transcode_video_encoder_open( p_stream, id ) != VLC_SUCCESS )
            {
                transcode_video_close( p_stream, id );
                id->b_transcode = VLC_FALSE;
                p_pic->pf_release( p_pic );
                return VLC_EGENERIC;
            }

            /* Deinterlace */
            if( p_stream->p_sys->b_deinterlace )
            {
                id->pp_filter[id->i_filter] =
                    vlc_object_create( p_stream, VLC_OBJECT_FILTER );
                vlc_object_attach( id->pp_filter[id->i_filter], p_stream );

                id->pp_filter[id->i_filter]->pf_vout_buffer_new =
                    video_new_buffer_filter;
                id->pp_filter[id->i_filter]->pf_vout_buffer_del =
                    video_del_buffer_filter;

                id->pp_filter[id->i_filter]->fmt_in = id->p_decoder->fmt_out;
                id->pp_filter[id->i_filter]->fmt_out = id->p_decoder->fmt_out;
                id->pp_filter[id->i_filter]->p_module =
                    module_Need( id->pp_filter[id->i_filter],
                                 "video filter2", "deinterlace", 0 );
                if( id->pp_filter[id->i_filter]->p_module )
                {
                    id->pp_filter[id->i_filter]->p_owner =
                        malloc( sizeof(filter_owner_sys_t) );
                    for( i = 0; i < PICTURE_RING_SIZE; i++ )
                        id->pp_filter[id->i_filter]->p_owner->pp_pics[i] = 0;

                    id->i_filter++;
                }
                else
                {
                    msg_Dbg( p_stream, "no video filter found" );
                    vlc_object_detach( id->pp_filter[id->i_filter] );
                    vlc_object_destroy( id->pp_filter[id->i_filter] );
                }
            }

            /* Check if we need a filter for chroma conversion or resizing */
            if( id->p_decoder->fmt_out.video.i_chroma !=
                id->p_encoder->fmt_in.video.i_chroma ||
                id->p_decoder->fmt_out.video.i_width !=
                id->p_encoder->fmt_out.video.i_width ||
                id->p_decoder->fmt_out.video.i_height !=
                id->p_encoder->fmt_out.video.i_height ||
                p_sys->i_crop_top > 0 || p_sys->i_crop_bottom > 0 ||
                p_sys->i_crop_left > 0 || p_sys->i_crop_right > 0 )
            {
                id->pp_filter[id->i_filter] =
                    vlc_object_create( p_stream, VLC_OBJECT_FILTER );
                vlc_object_attach( id->pp_filter[id->i_filter], p_stream );

                id->pp_filter[id->i_filter]->pf_vout_buffer_new =
                    video_new_buffer_filter;
                id->pp_filter[id->i_filter]->pf_vout_buffer_del =
                    video_del_buffer_filter;

                id->pp_filter[id->i_filter]->fmt_in = id->p_decoder->fmt_out;
                id->pp_filter[id->i_filter]->fmt_out = id->p_encoder->fmt_in;
                id->pp_filter[id->i_filter]->p_module =
                    module_Need( id->pp_filter[id->i_filter],
                                 "video filter2", 0, 0 );
                if( id->pp_filter[id->i_filter]->p_module )
                {
                    id->pp_filter[id->i_filter]->p_owner =
                        malloc( sizeof(filter_owner_sys_t) );
                    for( i = 0; i < PICTURE_RING_SIZE; i++ )
                        id->pp_filter[id->i_filter]->p_owner->pp_pics[i] = 0;

                    id->i_filter++;
                }
                else
                {
                    msg_Dbg( p_stream, "no video filter found" );
                    vlc_object_detach( id->pp_filter[id->i_filter] );
                    vlc_object_destroy( id->pp_filter[id->i_filter] );

                    transcode_video_close( p_stream, id );
                    id->b_transcode = VLC_FALSE;
                    p_pic->pf_release( p_pic );
                    return VLC_EGENERIC;
                }
            }
        }

        /* Run filter chain */
        for( i = 0; i < id->i_filter; i++ )
        {
            p_pic = id->pp_filter[i]->pf_video_filter(id->pp_filter[i], p_pic);
        }

        /*
         * Encoding
         */

        /* Check if we have a subpicture to overlay */
        if( p_sys->p_spu )
        {
            p_subpic = spu_SortSubpictures( p_sys->p_spu, p_pic->date );
            /* TODO: get another pic */
        }

        /* Overlay subpicture */
        if( p_subpic )
        {
            int i_scale_width, i_scale_height;
            video_format_t *p_fmt;

            i_scale_width = id->p_encoder->fmt_in.video.i_width * 1000 /
                id->p_decoder->fmt_out.video.i_width;
            i_scale_height = id->p_encoder->fmt_in.video.i_height * 1000 /
                id->p_decoder->fmt_out.video.i_height;

            if( p_pic->i_refcount && !id->i_filter )
            {
                /* We can't modify the picture, we need to duplicate it */
                picture_t *p_tmp = video_new_buffer_decoder( id->p_decoder );
                if( p_tmp )
                {
                    vout_CopyPicture( p_stream, p_tmp, p_pic );
                    p_pic->pf_release( p_pic );
                    p_pic = p_tmp;
                }
            }

            if( id->i_filter )
                p_fmt = &id->pp_filter[id->i_filter -1]->fmt_out.video;
            else
                p_fmt = &id->p_decoder->fmt_out.video;

            /* FIXME (shouldn't have to be done here) */
            p_fmt->i_sar_num = p_fmt->i_aspect *
                p_fmt->i_height / p_fmt->i_width;
            p_fmt->i_sar_den = VOUT_ASPECT_FACTOR;

            spu_RenderSubpictures( p_sys->p_spu, p_fmt, p_pic, p_pic, p_subpic,
                                   i_scale_width, i_scale_height );
        }

        if( p_sys->i_threads >= 1 )
        {
            vlc_mutex_lock( &p_sys->lock_out );
            p_sys->pp_pics[p_sys->i_last_pic++] = p_pic;
            p_sys->i_last_pic %= PICTURE_RING_SIZE;
            *out = p_sys->p_buffers;
            p_sys->p_buffers = NULL;
            vlc_cond_signal( &p_sys->cond );
            vlc_mutex_unlock( &p_sys->lock_out );
        }
        else
        {
            block_t *p_block;
            p_block = id->p_encoder->pf_encode_video( id->p_encoder, p_pic );
            block_ChainAppend( out, p_block );

            if( p_sys->b_master_sync )
                date_Increment( &id->interpolated_pts, 1 );

            if( p_sys->b_master_sync && i_duplicate > 1 )
            {
                mtime_t i_pts = date_Get( &id->interpolated_pts ) + 1;
                date_Increment( &id->interpolated_pts, 1 );
                p_pic->date = i_pts;
                p_block = id->p_encoder->pf_encode_video(id->p_encoder, p_pic);
                block_ChainAppend( out, p_block );
            }

            p_pic->pf_release( p_pic );
        }
    }

    return VLC_SUCCESS;
}

static int EncoderThread( sout_stream_sys_t *p_sys )
{
    sout_stream_id_t *id = p_sys->id_video;
    picture_t *p_pic;
    int i_plane;

    while( !p_sys->b_die && !p_sys->b_error )
    {
        block_t *p_block;

        vlc_mutex_lock( &p_sys->lock_out );
        while( p_sys->i_last_pic == p_sys->i_first_pic )
        {
            vlc_cond_wait( &p_sys->cond, &p_sys->lock_out );
            if( p_sys->b_die || p_sys->b_error ) break;
        }
        if( p_sys->b_die || p_sys->b_error )
        {
            vlc_mutex_unlock( &p_sys->lock_out );
            break;
        }

        p_pic = p_sys->pp_pics[p_sys->i_first_pic++];
        p_sys->i_first_pic %= PICTURE_RING_SIZE;
        vlc_mutex_unlock( &p_sys->lock_out );

        p_block = id->p_encoder->pf_encode_video( id->p_encoder, p_pic );
        vlc_mutex_lock( &p_sys->lock_out );
        block_ChainAppend( &p_sys->p_buffers, p_block );
        vlc_mutex_unlock( &p_sys->lock_out );

        for( i_plane = 0; i_plane < p_pic->i_planes; i_plane++ )
        {
            free( p_pic->p[i_plane].p_pixels );
        }
        free( p_pic );
    }

    while( p_sys->i_last_pic != p_sys->i_first_pic )
    {
        p_pic = p_sys->pp_pics[p_sys->i_first_pic++];
        p_sys->i_first_pic %= PICTURE_RING_SIZE;

        for( i_plane = 0; i_plane < p_pic->i_planes; i_plane++ )
        {
            free( p_pic->p[i_plane].p_pixels );
        }
        free( p_pic );
    }

    block_ChainRelease( p_sys->p_buffers );

    return 0;
}

struct picture_sys_t
{
    vlc_object_t *p_owner;
};

static void video_release_buffer( picture_t *p_pic )
{
    if( p_pic && !p_pic->i_refcount && p_pic->pf_release && p_pic->p_sys )
    {
        video_del_buffer_decoder( (decoder_t *)p_pic->p_sys->p_owner, p_pic );
    }
    else if( p_pic && p_pic->i_refcount > 0 ) p_pic->i_refcount--;
}

static picture_t *video_new_buffer( vlc_object_t *p_this, picture_t **pp_ring )
{
    decoder_t *p_dec = (decoder_t *)p_this;
    picture_t *p_pic;
    int i;

    /* Find an empty space in the picture ring buffer */
    for( i = 0; i < PICTURE_RING_SIZE; i++ )
    {
        if( pp_ring[i] != 0 && pp_ring[i]->i_status == DESTROYED_PICTURE )
        {
            pp_ring[i]->i_status = RESERVED_PICTURE;
            return pp_ring[i];
        }
    }
    for( i = 0; i < PICTURE_RING_SIZE; i++ )
    {
        if( pp_ring[i] == 0 ) break;
    }

    if( i == PICTURE_RING_SIZE )
    {
        msg_Err( p_this, "decoder/filter is leaking pictures, "
                 "resetting its ring buffer" );

        for( i = 0; i < PICTURE_RING_SIZE; i++ )
        {
            pp_ring[i]->pf_release( pp_ring[i] );
        }

        i = 0;
    }

    p_pic = malloc( sizeof(picture_t) );
    p_dec->fmt_out.video.i_chroma = p_dec->fmt_out.i_codec;
    vout_AllocatePicture( VLC_OBJECT(p_dec), p_pic,
                          p_dec->fmt_out.video.i_chroma,
                          p_dec->fmt_out.video.i_width,
                          p_dec->fmt_out.video.i_height,
                          p_dec->fmt_out.video.i_aspect );

    if( !p_pic->i_planes )
    {
        free( p_pic );
        return 0;
    }

    p_pic->pf_release = video_release_buffer;
    p_pic->p_sys = malloc( sizeof(picture_sys_t) );
    p_pic->p_sys->p_owner = p_this;
    p_pic->i_status = RESERVED_PICTURE;

    pp_ring[i] = p_pic;

    return p_pic;
}

static picture_t *video_new_buffer_decoder( decoder_t *p_dec )
{
    return video_new_buffer( VLC_OBJECT(p_dec),
                             p_dec->p_owner->pp_pics );
}

static picture_t *video_new_buffer_filter( filter_t *p_filter )
{
    return video_new_buffer( VLC_OBJECT(p_filter),
                             p_filter->p_owner->pp_pics );
}

static void video_del_buffer( vlc_object_t *p_this, picture_t *p_pic )
{
    if( p_pic && p_pic->p_data_orig ) free( p_pic->p_data_orig );
    if( p_pic && p_pic->p_sys ) free( p_pic->p_sys );
    if( p_pic ) free( p_pic );
}

static void video_del_buffer_decoder( decoder_t *p_decoder, picture_t *p_pic )
{
    p_pic->i_refcount = 0;
    p_pic->i_status = DESTROYED_PICTURE;
}

static void video_del_buffer_filter( filter_t *p_filter, picture_t *p_pic )
{
    p_pic->i_refcount = 0;
    p_pic->i_status = DESTROYED_PICTURE;
}

static void video_link_picture_decoder( decoder_t *p_dec, picture_t *p_pic )
{
    p_pic->i_refcount++;
}

static void video_unlink_picture_decoder( decoder_t *p_dec, picture_t *p_pic )
{
    video_release_buffer( p_pic );
}

/*
 * SPU
 */
static subpicture_t *spu_new_buffer( decoder_t * );
static void spu_del_buffer( decoder_t *, subpicture_t * );

static int transcode_spu_new( sout_stream_t *p_stream, sout_stream_id_t *id )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;

    /*
     * Open decoder
     */

    /* Initialization of decoder structures */
    id->p_decoder->pf_spu_buffer_new = spu_new_buffer;
    id->p_decoder->pf_spu_buffer_del = spu_del_buffer;
    id->p_decoder->p_owner = (decoder_owner_sys_t *)p_stream;
    //id->p_decoder->p_cfg = p_sys->p_spu_cfg;

    id->p_decoder->p_module =
        module_Need( id->p_decoder, "decoder", "$codec", 0 );

    if( !id->p_decoder->p_module )
    {
        msg_Err( p_stream, "cannot find decoder" );
        return VLC_EGENERIC;
    }

    if( !p_sys->b_soverlay )
    {
        /*
         * Open encoder
         */

        /* Initialization of encoder format structures */
        es_format_Init( &id->p_encoder->fmt_in, id->p_decoder->fmt_in.i_cat,
                        id->p_decoder->fmt_in.i_codec );

        id->p_encoder->p_cfg = p_sys->p_spu_cfg;

        id->p_encoder->p_module =
            module_Need( id->p_encoder, "encoder", p_sys->psz_senc, VLC_TRUE );

        if( !id->p_encoder->p_module )
        {
            module_Unneed( id->p_decoder, id->p_decoder->p_module );
            msg_Err( p_stream, "cannot find encoder" );
            return VLC_EGENERIC;
        }
    }

    if( !p_sys->p_spu )
    {
        p_sys->p_spu = spu_Create( p_stream );
        spu_Init( p_sys->p_spu );
    }

    return VLC_SUCCESS;
}

static void transcode_spu_close( sout_stream_t *p_stream, sout_stream_id_t *id)
{
    /* Close decoder */
    if( id->p_decoder->p_module )
        module_Unneed( id->p_decoder, id->p_decoder->p_module );

    /* Close encoder */
    if( id->p_encoder->p_module )
        module_Unneed( id->p_encoder, id->p_encoder->p_module );
}

static int transcode_spu_process( sout_stream_t *p_stream,
                                  sout_stream_id_t *id,
                                  block_t *in, block_t **out )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;
    subpicture_t *p_subpic;
    *out = NULL;

    p_subpic = id->p_decoder->pf_decode_sub( id->p_decoder, &in );
    if( !p_subpic ) return VLC_EGENERIC;

    if( p_sys->b_master_sync && p_sys->i_master_drift )
    {
        p_subpic->i_start -= p_sys->i_master_drift;
        if( p_subpic->i_stop ) p_subpic->i_stop -= p_sys->i_master_drift;
    }

    if( p_sys->b_soverlay )
    {
        spu_DisplaySubpicture( p_sys->p_spu, p_subpic );
    }
    else
    {
        block_t *p_block;

        p_block = id->p_encoder->pf_encode_sub( id->p_encoder, p_subpic );
        spu_del_buffer( id->p_decoder, p_subpic );

        if( p_block )
        {
            block_ChainAppend( out, p_block );
            return VLC_SUCCESS;
        }
    }

    return VLC_EGENERIC;
}

static subpicture_t *spu_new_buffer( decoder_t *p_dec )
{
    sout_stream_t *p_stream = (sout_stream_t *)p_dec->p_owner;
    return spu_CreateSubpicture( p_stream->p_sys->p_spu );
}

static void spu_del_buffer( decoder_t *p_dec, subpicture_t *p_subpic )
{
    sout_stream_t *p_stream = (sout_stream_t *)p_dec->p_owner;
    spu_DestroySubpicture( p_stream->p_sys->p_spu, p_subpic );
}
