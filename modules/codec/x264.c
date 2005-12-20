/*****************************************************************************
 * x264.c: h264 video encoder
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 * $Id: x264.c 9280 2004-11-11 12:31:27Z zorglub $
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
#include <vlc/vlc.h>
#include <vlc/vout.h>
#include <vlc/sout.h>
#include <vlc/decoder.h>

#include <x264.h>

#define SOUT_CFG_PREFIX "sout-x264-"

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open ( vlc_object_t * );
static void Close( vlc_object_t * );

#define QP_TEXT N_("Quantizer parameter")
#define QP_LONGTEXT N_( \
    "This selects the quantizer to use (1 to 51). Lower values result in " \
    "better fidelity, but higher bitrates. 26 is a good default value." )

#define QPMIN_TEXT N_("Minimum quantizer parameter")
#define QPMIN_LONGTEXT N_( "Minimum quantizer, 15/35 seems to be a useful " \
    "range." )

#define QPMAX_TEXT N_("Maximum quantizer parameter")
#define QPMAX_LONGTEXT N_( "Maximum quantizer parameter." )

#define CABAC_TEXT N_("Enable CABAC")
#define CABAC_LONGTEXT N_( "Enable CABAC (Context-Adaptive Binary Arithmetic "\
    "Coding). Slightly slows down encoding and decoding, but should save " \
    "10-15% bitrate." )

#define LOOPF_TEXT N_("Enable loop filter")
#define LOOPF_LONGTEXT N_( "Use deblocking loop filter (increases quality).")

#define ANALYSE_TEXT N_("Analyse mode")
#define ANALYSE_LONGTEXT N_( "This selects the analysing mode.")

#define KEYINT_TEXT N_("Sets maximum interval between I frames")
#define KEYINT_LONGTEXT N_( "Larger values save bits, thus improve quality "\
    "for a given bitrate, at the cost of seeking precision." )

#define IDRINT_TEXT N_("IDR frames")
#define IDRINT_LONGTEXT N_("In H.264, I-Frames do not necessarily bound a " \
    "closed GOP because it is allowable for a P-frame to be predicted from " \
    "more frames than just the one frame before it (also see frameref). " \
    "Therefore, I-frames are not necessarily seekable. " \
    "IDR-Frames restrict subsequent P-frames from referring to any frame " \
    "prior to the IDR-Frame." )

#define BFRAMES_TEXT N_("B frames")
#define BFRAMES_LONGTEXT N_( "Number of consecutive B-Frames between I and " \
    "P-frames." )

#define FRAMEREF_TEXT N_("Number of previous frames used as predictors.")
#define FRAMEREF_LONGTEXT N_( "This is effective in Anime, but seems to " \
    "make little difference in live-action source material. Some decoders " \
    "are unable to deal with large frameref values." )

#define SCENE_TEXT N_("Scene-cut detection.")
#define SCENE_LONGTEXT N_( "Controls how aggressively to insert extra " \
    "I-frames. With small values of scenecut, the codec often has to force " \
    "an I-frame when it would exceed keyint. " \
    "Good values of scenecut may find a better location for the I-frame. " \
    "Large values use more I-frames than necessary, thus wasting bits. " \
    "-1 disables scene-cut detection, so I-frames are be inserted only every "\
    "other keyint frames, which probably leads to ugly encoding artifacts." )

static char *enc_analyse_list[] =
  { "", "all", "normal", "fast", "none" };
static char *enc_analyse_list_text[] =
  { N_("default"), N_("all"), N_("normal"), N_("fast"), N_("none") };

vlc_module_begin();
    set_description( _("h264 video encoder using x264 library"));
    set_capability( "encoder", 200 );
    set_callbacks( Open, Close );

    add_integer( SOUT_CFG_PREFIX "qp", 0, NULL, QP_TEXT, QP_LONGTEXT,
                 VLC_FALSE );
        change_integer_range( 0, 51 );
    add_integer( SOUT_CFG_PREFIX "qp-min", 10, NULL, QPMIN_TEXT,
                 QPMIN_LONGTEXT, VLC_FALSE );
        change_integer_range( 0, 51 );
    add_integer( SOUT_CFG_PREFIX "qp-max", 51, NULL, QPMAX_TEXT,
                 QPMAX_LONGTEXT, VLC_FALSE );
        change_integer_range( 0, 51 );

    add_bool( SOUT_CFG_PREFIX "cabac", 1, NULL, CABAC_TEXT, CABAC_LONGTEXT,
              VLC_FALSE );

    add_bool( SOUT_CFG_PREFIX "loopfilter", 1, NULL, LOOPF_TEXT,
              LOOPF_LONGTEXT, VLC_FALSE );

    add_string( SOUT_CFG_PREFIX "analyse", "", NULL, ANALYSE_TEXT,
                ANALYSE_LONGTEXT, VLC_FALSE );
        change_string_list( enc_analyse_list, enc_analyse_list_text, 0 );

    add_integer( SOUT_CFG_PREFIX "keyint", 250, NULL, KEYINT_TEXT,
                 KEYINT_LONGTEXT, VLC_FALSE );

    add_integer( SOUT_CFG_PREFIX "idrint", 2, NULL, IDRINT_TEXT,
                 IDRINT_LONGTEXT, VLC_FALSE );

    add_integer( SOUT_CFG_PREFIX "bframes", 0, NULL, BFRAMES_TEXT,
                 BFRAMES_LONGTEXT, VLC_FALSE );
        change_integer_range( 0, 16 );

    add_integer( SOUT_CFG_PREFIX "frameref", 1, NULL, FRAMEREF_TEXT,
                 FRAMEREF_LONGTEXT, VLC_FALSE );
        change_integer_range( 1, 15 );

    add_integer( SOUT_CFG_PREFIX "scenecut", 40, NULL, SCENE_TEXT,
                 SCENE_LONGTEXT, VLC_FALSE );
        change_integer_range( -1, 100 );

vlc_module_end();

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static const char *ppsz_sout_options[] = {
    "qp", "qp-min", "qp-max", "cabac", "loopfilter", "analyse",
    "keyint", "idrint", "bframes", "frameref", "scenecut", NULL
};

static block_t *Encode( encoder_t *, picture_t * );

struct encoder_sys_t
{
    x264_t          *h;
    x264_param_t    param;

    int             i_buffer;
    uint8_t         *p_buffer;
};

/*****************************************************************************
 * Open: probe the encoder
 *****************************************************************************/
static int  Open ( vlc_object_t *p_this )
{
    encoder_t     *p_enc = (encoder_t *)p_this;
    encoder_sys_t *p_sys;
    vlc_value_t    val;
    int i_qmin = 0, i_qmax = 0;

    if( p_enc->fmt_out.i_codec != VLC_FOURCC( 'h', '2', '6', '4' ) &&
        !p_enc->b_force )
    {
        return VLC_EGENERIC;
    }

    if( p_enc->fmt_in.video.i_width % 16 != 0 ||
        p_enc->fmt_in.video.i_height % 16!= 0 )
    {
        msg_Warn( p_enc, "invalid size %ix%i",
                  p_enc->fmt_in.video.i_width,
                  p_enc->fmt_in.video.i_height );
        return VLC_EGENERIC;
    }

    sout_CfgParse( p_enc, SOUT_CFG_PREFIX, ppsz_sout_options, p_enc->p_cfg );

    p_enc->fmt_out.i_codec = VLC_FOURCC( 'h', '2', '6', '4' );
    p_enc->fmt_in.i_codec = VLC_FOURCC('I','4','2','0');

    p_enc->pf_encode_video = Encode;
    p_enc->pf_encode_audio = NULL;
    p_enc->p_sys = p_sys = malloc( sizeof( encoder_sys_t ) );

    x264_param_default( &p_sys->param );
    p_sys->param.i_width  = p_enc->fmt_in.video.i_width;
    p_sys->param.i_height = p_enc->fmt_in.video.i_height;

    var_Get( p_enc, SOUT_CFG_PREFIX "qp-min", &val );
    if( val.i_int >= 1 && val.i_int <= 51 ) i_qmin = val.i_int;
    var_Get( p_enc, SOUT_CFG_PREFIX "qp-max", &val );
    if( val.i_int >= 1 && val.i_int <= 51 ) i_qmax = val.i_int;

    var_Get( p_enc, SOUT_CFG_PREFIX "qp", &val );
    if( val.i_int >= 1 && val.i_int <= 51 )
    {
        if( i_qmin > val.i_int ) i_qmin = val.i_int;
        if( i_qmax < val.i_int ) i_qmax = val.i_int;

#if X264_BUILD >= 0x000a
        p_sys->param.rc.i_qp_constant = val.i_int;
        p_sys->param.rc.i_qp_min = i_qmin;
        p_sys->param.rc.i_qp_max = i_qmax;
#else
        p_sys->param.i_qp_constant = val.i_int;
#endif
    }
    else
    {
        /* No QP -> constant bitrate */
#if X264_BUILD >= 0x000a
        p_sys->param.rc.b_cbr = 1;
        p_sys->param.rc.i_bitrate = p_enc->fmt_out.i_bitrate / 1000;
        p_sys->param.rc.i_rc_buffer_size = p_sys->param.rc.i_bitrate;
        p_sys->param.rc.i_rc_init_buffer = p_sys->param.rc.i_bitrate / 4;
#endif
    }

    var_Get( p_enc, SOUT_CFG_PREFIX "cabac", &val );
    p_sys->param.b_cabac = val.b_bool;

    var_Get( p_enc, SOUT_CFG_PREFIX "loopfilter", &val );
    p_sys->param.b_deblocking_filter = val.b_bool;

    var_Get( p_enc, SOUT_CFG_PREFIX "keyint", &val );
    if( val.i_int > 0 ) p_sys->param.i_iframe = val.i_int;

    var_Get( p_enc, SOUT_CFG_PREFIX "idrint", &val );
    if( val.i_int > 0 ) p_sys->param.i_idrframe = val.i_int;

    var_Get( p_enc, SOUT_CFG_PREFIX "bframes", &val );
    if( val.i_int >= 0 && val.i_int <= 16 ) p_sys->param.i_bframe = val.i_int;

    var_Get( p_enc, SOUT_CFG_PREFIX "frameref", &val );
    if( val.i_int > 0 && val.i_int <= 15 )
        p_sys->param.i_frame_reference = val.i_int;

    var_Get( p_enc, SOUT_CFG_PREFIX "scenecut", &val );
#if X264_BUILD >= 0x000b
    if( val.i_int >= -1 && val.i_int <= 100 )
        p_sys->param.i_scenecut_threshold = val.i_int;
#endif

    var_Get( p_enc, SOUT_CFG_PREFIX "analyse", &val );
    if( !strcmp( val.psz_string, "none" ) )
    {
        p_sys->param.analyse.inter = 0;
    }
    else if( !strcmp( val.psz_string, "fast" ) )
    {
        p_sys->param.analyse.inter = X264_ANALYSE_I4x4;
    }
    else if( !strcmp( val.psz_string, "normal" ) )
    {
        p_sys->param.analyse.inter =
            X264_ANALYSE_I4x4 | X264_ANALYSE_PSUB16x16;
    }
    else if( !strcmp( val.psz_string, "all" ) )
    {
        p_sys->param.analyse.inter =
            X264_ANALYSE_I4x4 | X264_ANALYSE_PSUB16x16 | X264_ANALYSE_PSUB8x8;
    }

    if( p_enc->fmt_in.video.i_aspect > 0 )
    {
        int64_t i_num, i_den;
        int i_dst_num, i_dst_den;

        i_num = p_enc->fmt_in.video.i_aspect *
            (int64_t)p_enc->fmt_in.video.i_height;
        i_den = VOUT_ASPECT_FACTOR * p_enc->fmt_in.video.i_width;
        vlc_reduce( &i_dst_num, &i_dst_den, i_num, i_den, 0 );

        p_sys->param.vui.i_sar_width = i_dst_num;
        p_sys->param.vui.i_sar_height = i_dst_den;
    }
    if( p_enc->fmt_in.video.i_frame_rate_base > 0 )
    {
        p_sys->param.i_fps_num = p_enc->fmt_in.video.i_frame_rate;
        p_sys->param.i_fps_den = p_enc->fmt_in.video.i_frame_rate_base;
    }
    if( !(p_enc->p_libvlc->i_cpu & CPU_CAPABILITY_MMX) )
    {
        p_sys->param.cpu &= ~X264_CPU_MMX;
    }
    if( !(p_enc->p_libvlc->i_cpu & CPU_CAPABILITY_MMXEXT) )
    {
        p_sys->param.cpu &= ~X264_CPU_MMXEXT;
    }
    if( !(p_enc->p_libvlc->i_cpu & CPU_CAPABILITY_SSE) )
    {
        p_sys->param.cpu &= ~X264_CPU_SSE;
    }
    if( !(p_enc->p_libvlc->i_cpu & CPU_CAPABILITY_SSE2) )
    {
        p_sys->param.cpu &= ~X264_CPU_SSE2;
    }

    /* Open the encoder */
    p_sys->h = x264_encoder_open( &p_sys->param );

    /* alloc mem */
    p_sys->i_buffer = 4 * p_enc->fmt_in.video.i_width *
        p_enc->fmt_in.video.i_height + 1000;
    p_sys->p_buffer = malloc( p_sys->i_buffer );

    /* get the globals headers */
    p_enc->fmt_out.i_extra = 0;
    p_enc->fmt_out.p_extra = NULL;

#if 0
    x264_encoder_headers( p_sys->h, &nal, &i_nal );
    for( i = 0; i < i_nal; i++ )
    {
        int i_size = p_sys->i_buffer;

        x264_nal_encode( p_sys->p_buffer, &i_size, 1, &nal[i] );

        p_enc->fmt_out.p_extra = realloc( p_enc->fmt_out.p_extra, p_enc->fmt_out.i_extra + i_size );

        memcpy( p_enc->fmt_out.p_extra + p_enc->fmt_out.i_extra,
                p_sys->p_buffer, i_size );

        p_enc->fmt_out.i_extra += i_size;
    }
#endif

    return VLC_SUCCESS;
}

/****************************************************************************
 * Encode:
 ****************************************************************************/
static block_t *Encode( encoder_t *p_enc, picture_t *p_pict )
{
    encoder_sys_t *p_sys = p_enc->p_sys;
    x264_picture_t  pic;
    int        i_nal;
    x264_nal_t *nal;
    block_t *p_block;
    int i_out;
    int i;

    /* init pic */
    memset( &pic, 0, sizeof( x264_picture_t ) );
    pic.img.i_csp = X264_CSP_I420;
    pic.img.i_plane = p_pict->i_planes;
    for( i = 0; i < p_pict->i_planes; i++ )
    {
        pic.img.plane[i] = p_pict->p[i].p_pixels;
        pic.img.i_stride[i] = p_pict->p[i].i_pitch;
    }

    x264_encoder_encode( p_sys->h, &nal, &i_nal, &pic );
    for( i = 0, i_out = 0; i < i_nal; i++ )
    {
        int i_size = p_sys->i_buffer - i_out;
        x264_nal_encode( p_sys->p_buffer + i_out, &i_size, 1, &nal[i] );

        i_out += i_size;
    }

    p_block = block_New( p_enc, i_out );
    p_block->i_dts = p_pict->date;
    p_block->i_pts = p_pict->date;
    memcpy( p_block->p_buffer, p_sys->p_buffer, i_out );

    if( pic.i_type == X264_TYPE_IDR || pic.i_type == X264_TYPE_I )
        p_block->i_flags |= BLOCK_FLAG_TYPE_I;
    else if( pic.i_type == X264_TYPE_P )
        p_block->i_flags |= BLOCK_FLAG_TYPE_P;
    else if( pic.i_type == X264_TYPE_B )
        p_block->i_flags |= BLOCK_FLAG_TYPE_B;

    return p_block;
}

/*****************************************************************************
 * CloseEncoder: ffmpeg encoder destruction
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    encoder_t     *p_enc = (encoder_t *)p_this;
    encoder_sys_t *p_sys = p_enc->p_sys;

    x264_encoder_close( p_sys->h );
    free( p_sys->p_buffer );
    free( p_sys );
}
