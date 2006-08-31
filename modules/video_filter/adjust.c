/*****************************************************************************
 * adjust.c : Contrast/Hue/Saturation/Brightness video plugin for vlc
 *****************************************************************************
 * Copyright (C) 2000-2006 the VideoLAN team
 * $Id: adjust.c 16044 2006-07-15 14:53:58Z dionoea $
 *
 * Authors: Simon Latapie <garf@via.ecp.fr>
 *          Antoine Cellerier <dionoea -at- videolan d0t org>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <errno.h>
#include <stdlib.h>                                      /* malloc(), free() */
#include <string.h>
#include <math.h>

#include <vlc/vlc.h>
#include <vlc/sout.h>
#include <vlc/decoder.h>

#include "vlc_filter.h"

#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

#define eight_times( x )    x x x x x x x x

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int  Create    ( vlc_object_t * );
static void Destroy   ( vlc_object_t * );

static picture_t *Filter( filter_t *, picture_t * );

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/

#define THRES_TEXT N_("Brightness threshold")
#define THRES_LONGTEXT N_("When this mode is enabled, pixels will be " \
        "shown as black or white. The threshold value will be the brighness " \
        "defined below." )
#define CONT_TEXT N_("Image contrast (0-2)")
#define CONT_LONGTEXT N_("Set the image contrast, between 0 and 2. Defaults to 1.")
#define HUE_TEXT N_("Image hue (0-360)")
#define HUE_LONGTEXT N_("Set the image hue, between 0 and 360. Defaults to 0.")
#define SAT_TEXT N_("Image saturation (0-3)")
#define SAT_LONGTEXT N_("Set the image saturation, between 0 and 3. Defaults to 1.")
#define LUM_TEXT N_("Image brightness (0-2)")
#define LUM_LONGTEXT N_("Set the image brightness, between 0 and 2. Defaults to 1.")
#define GAMMA_TEXT N_("Image gamma (0-10)")
#define GAMMA_LONGTEXT N_("Set the image gamma, between 0.01 and 10. Defaults to 1.")

vlc_module_begin();
    set_description( _("Image properties filter") );
    set_shortname( N_("Image adjust" ));
    set_category( CAT_VIDEO );
    set_subcategory( SUBCAT_VIDEO_VFILTER2 );
    set_capability( "video filter2", 0 );

    add_float_with_range( "contrast", 1.0, 0.0, 2.0, NULL,
                          CONT_TEXT, CONT_LONGTEXT, VLC_FALSE );
    add_float_with_range( "brightness", 1.0, 0.0, 2.0, NULL,
                           LUM_TEXT, LUM_LONGTEXT, VLC_FALSE );
    add_integer_with_range( "hue", 0, 0, 360, NULL,
                            HUE_TEXT, HUE_LONGTEXT, VLC_FALSE );
    add_float_with_range( "saturation", 1.0, 0.0, 3.0, NULL,
                          SAT_TEXT, SAT_LONGTEXT, VLC_FALSE );
    add_float_with_range( "gamma", 1.0, 0.01, 10.0, NULL,
                          GAMMA_TEXT, GAMMA_LONGTEXT, VLC_FALSE );

    add_bool( "brightness-threshold", 0, NULL,
              THRES_TEXT, THRES_LONGTEXT, VLC_FALSE );

    add_shortcut( "adjust" );
    set_callbacks( Create, Destroy );
vlc_module_end();

static const char *ppsz_filter_options[] = {
    "contrast", "brightness", "hue", "saturation", "gamma",
    "brightness-threshold", NULL
};

/*****************************************************************************
 * filter_sys_t: adjust filter method descriptor
 *****************************************************************************/
struct filter_sys_t
{
};

inline static int32_t clip( int32_t a )
{
    return (a > 255) ? 255 : (a < 0) ? 0 : a;
}

/*****************************************************************************
 * Create: allocates adjust video thread output method
 *****************************************************************************
 * This function allocates and initializes a adjust vout method.
 *****************************************************************************/
static int Create( vlc_object_t *p_this )
{
    filter_t *p_filter = (filter_t *)p_this;

    /* XXX: we might need to add/remove some FOURCCs ... */
    if(   p_filter->fmt_in.video.i_chroma != VLC_FOURCC('I','4','2','0')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('I','Y','U','V')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('J','4','2','0')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('Y','V','1','2')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('I','U','Y','V')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('U','Y','V','Y')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('U','Y','N','V')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('Y','4','2','2')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('c','y','u','v')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('Y','U','Y','2')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('Y','U','N','V')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('Y','V','Y','U')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('I','4','1','1')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('I','4','1','0')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('Y','V','U','9')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('Y','M','G','A')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('I','4','2','2')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('J','4','2','2')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('I','4','4','4')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('J','4','4','4')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('Y','U','V','P')
       && p_filter->fmt_in.video.i_chroma != VLC_FOURCC('Y','U','V','A') )
    {
        msg_Err( p_filter, "Unsupported input chroma (%4s)",
                 (char*)&(p_filter->fmt_in.video.i_chroma) );
        return VLC_EGENERIC;
    }

    if( p_filter->fmt_in.video.i_chroma != p_filter->fmt_out.video.i_chroma )
    {
        msg_Err( p_filter, "Input and output chromas don't match" );
        return VLC_EGENERIC;
    }

    /* Allocate structure */
    p_filter->p_sys = malloc( sizeof( filter_sys_t ) );
    if( p_filter->p_sys == NULL )
    {
        msg_Err( p_filter, "out of memory" );
        return VLC_ENOMEM;
    }

    p_filter->pf_video_filter = Filter;

    /* needed to get options passed in transcode using the
     * adjust{name=value} syntax */
    sout_CfgParse( p_filter, "", ppsz_filter_options,
                   p_filter->p_cfg );

    var_Create( p_filter, "contrast",
                VLC_VAR_FLOAT | VLC_VAR_DOINHERIT );
    var_Create( p_filter, "brightness",
                VLC_VAR_FLOAT | VLC_VAR_DOINHERIT );
    var_Create( p_filter, "hue",
                VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_filter, "saturation",
                VLC_VAR_FLOAT | VLC_VAR_DOINHERIT );
    var_Create( p_filter, "gamma",
                VLC_VAR_FLOAT | VLC_VAR_DOINHERIT );
    var_Create( p_filter, "brightness-threshold",
                VLC_VAR_BOOL | VLC_VAR_DOINHERIT );

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Destroy: destroy adjust video thread output method
 *****************************************************************************
 * Terminate an output method created by adjustCreateOutputMethod
 *****************************************************************************/
static void Destroy( vlc_object_t *p_this )
{
    filter_t *p_filter = (filter_t *)p_this;
    free( p_filter->p_sys );
}

/*****************************************************************************
 * Render: displays previously rendered output
 *****************************************************************************
 * This function send the currently rendered image to adjust modified image,
 * waits until it is displayed and switch the two rendering buffers, preparing
 * next frame.
 *****************************************************************************/
static picture_t *Filter( filter_t *p_filter, picture_t *p_pic )
{
    int pi_luma[256];
    int pi_gamma[256];

    picture_t *p_outpic;
    uint8_t *p_in, *p_in_v, *p_in_end, *p_line_end;
    uint8_t *p_out, *p_out_v;

    vlc_bool_t b_thres;
    double  f_hue;
    double  f_gamma;
    int32_t i_cont, i_lum;
    int i_sat, i_sin, i_cos, i_x, i_y;
    int i;
    vlc_value_t val;

    if( !p_pic ) return NULL;

    p_outpic = p_filter->pf_vout_buffer_new( p_filter );
    if( !p_outpic )
    {
        msg_Warn( p_filter, "can't get output picture" );
        if( p_pic->pf_release )
            p_pic->pf_release( p_pic );
        return NULL;
    }

    /* Getvariables */
    var_Get( p_filter, "contrast", &val );
    i_cont = (int) ( val.f_float * 255 );
    var_Get( p_filter, "brightness", &val );
    i_lum = (int) (( val.f_float - 1.0 ) * 255 );
    var_Get( p_filter, "hue", &val );
    f_hue = (float) ( val.i_int * M_PI / 180 );
    var_Get( p_filter, "saturation", &val );
    i_sat = (int) (val.f_float * 256 );
    var_Get( p_filter, "gamma", &val );
    f_gamma = 1.0 / val.f_float;
    var_Get( p_filter, "brightness-threshold", &val );
    b_thres = (vlc_bool_t) ( val.b_bool );

    /*
     * Threshold mode drops out everything about luma, contrast and gamma.
     */
    if( b_thres != VLC_TRUE )
    {

        /* Contrast is a fast but kludged function, so I put this gap to be
         * cleaner :) */
        i_lum += 128 - i_cont / 2;

        /* Fill the gamma lookup table */
        for( i = 0 ; i < 256 ; i++ )
        {
          pi_gamma[ i ] = clip( pow(i / 255.0, f_gamma) * 255.0);
        }

        /* Fill the luma lookup table */
        for( i = 0 ; i < 256 ; i++ )
        {
            pi_luma[ i ] = pi_gamma[clip( i_lum + i_cont * i / 256)];
        }
    }
    else
    {
        /*
         * We get luma as threshold value: the higher it is, the darker is
         * the image. Should I reverse this?
         */
        for( i = 0 ; i < 256 ; i++ )
        {
            pi_luma[ i ] = (i < i_lum) ? 0 : 255;
        }

        /*
         * Desaturates image to avoid that strange yellow halo...
         */
        i_sat = 0;
    }

    /*
     * Do the Y plane
     */

    p_in = p_pic->p[Y_PLANE].p_pixels;
    p_in_end = p_in + p_pic->p[Y_PLANE].i_visible_lines
                      * p_pic->p[Y_PLANE].i_pitch - 8;

    p_out = p_outpic->p[Y_PLANE].p_pixels;

    for( ; p_in < p_in_end ; )
    {
        p_line_end = p_in + p_pic->p[Y_PLANE].i_visible_pitch - 8;

        for( ; p_in < p_line_end ; )
        {
            /* Do 8 pixels at a time */
            *p_out++ = pi_luma[ *p_in++ ]; *p_out++ = pi_luma[ *p_in++ ];
            *p_out++ = pi_luma[ *p_in++ ]; *p_out++ = pi_luma[ *p_in++ ];
            *p_out++ = pi_luma[ *p_in++ ]; *p_out++ = pi_luma[ *p_in++ ];
            *p_out++ = pi_luma[ *p_in++ ]; *p_out++ = pi_luma[ *p_in++ ];
        }

        p_line_end += 8;

        for( ; p_in < p_line_end ; )
        {
            *p_out++ = pi_luma[ *p_in++ ];
        }

        p_in += p_pic->p[Y_PLANE].i_pitch
              - p_pic->p[Y_PLANE].i_visible_pitch;
        p_out += p_outpic->p[Y_PLANE].i_pitch
               - p_outpic->p[Y_PLANE].i_visible_pitch;
    }

    /*
     * Do the U and V planes
     */

    p_in = p_pic->p[U_PLANE].p_pixels;
    p_in_v = p_pic->p[V_PLANE].p_pixels;
    p_in_end = p_in + p_pic->p[U_PLANE].i_visible_lines
                      * p_pic->p[U_PLANE].i_pitch - 8;

    p_out = p_outpic->p[U_PLANE].p_pixels;
    p_out_v = p_outpic->p[V_PLANE].p_pixels;

    i_sin = sin(f_hue) * 256;
    i_cos = cos(f_hue) * 256;

    i_x = ( cos(f_hue) + sin(f_hue) ) * 32768;
    i_y = ( cos(f_hue) - sin(f_hue) ) * 32768;

    if ( i_sat > 256 )
    {
#define WRITE_UV_CLIP() \
    i_u = *p_in++ ; i_v = *p_in_v++ ; \
    *p_out++ = clip( (( ((i_u * i_cos + i_v * i_sin - i_x) >> 8) \
                           * i_sat) >> 8) + 128); \
    *p_out_v++ = clip( (( ((i_v * i_cos - i_u * i_sin - i_y) >> 8) \
                           * i_sat) >> 8) + 128)

        uint8_t i_u, i_v;

        for( ; p_in < p_in_end ; )
        {
            p_line_end = p_in + p_pic->p[U_PLANE].i_visible_pitch - 8;

            for( ; p_in < p_line_end ; )
            {
                /* Do 8 pixels at a time */
                WRITE_UV_CLIP(); WRITE_UV_CLIP();
                WRITE_UV_CLIP(); WRITE_UV_CLIP();
                WRITE_UV_CLIP(); WRITE_UV_CLIP();
                WRITE_UV_CLIP(); WRITE_UV_CLIP();
            }

            p_line_end += 8;

            for( ; p_in < p_line_end ; )
            {
                WRITE_UV_CLIP();
            }

            p_in += p_pic->p[U_PLANE].i_pitch
                  - p_pic->p[U_PLANE].i_visible_pitch;
            p_in_v += p_pic->p[V_PLANE].i_pitch
                    - p_pic->p[V_PLANE].i_visible_pitch;
            p_out += p_outpic->p[U_PLANE].i_pitch
                   - p_outpic->p[U_PLANE].i_visible_pitch;
            p_out_v += p_outpic->p[V_PLANE].i_pitch
                     - p_outpic->p[V_PLANE].i_visible_pitch;
        }
    }
    else
    {
#define WRITE_UV() \
    i_u = *p_in++ ; i_v = *p_in_v++ ; \
    *p_out++ = (( ((i_u * i_cos + i_v * i_sin - i_x) >> 8) \
                       * i_sat) >> 8) + 128; \
    *p_out_v++ = (( ((i_v * i_cos - i_u * i_sin - i_y) >> 8) \
                       * i_sat) >> 8) + 128

        uint8_t i_u, i_v;

        for( ; p_in < p_in_end ; )
        {
            p_line_end = p_in + p_pic->p[U_PLANE].i_visible_pitch - 8;

            for( ; p_in < p_line_end ; )
            {
                /* Do 8 pixels at a time */
                WRITE_UV(); WRITE_UV(); WRITE_UV(); WRITE_UV();
                WRITE_UV(); WRITE_UV(); WRITE_UV(); WRITE_UV();
            }

            p_line_end += 8;

            for( ; p_in < p_line_end ; )
            {
                WRITE_UV();
            }

            p_in += p_pic->p[U_PLANE].i_pitch
                  - p_pic->p[U_PLANE].i_visible_pitch;
            p_in_v += p_pic->p[V_PLANE].i_pitch
                    - p_pic->p[V_PLANE].i_visible_pitch;
            p_out += p_outpic->p[U_PLANE].i_pitch
                   - p_outpic->p[U_PLANE].i_visible_pitch;
            p_out_v += p_outpic->p[V_PLANE].i_pitch
                     - p_outpic->p[V_PLANE].i_visible_pitch;
        }
    }

    p_outpic->date = p_pic->date;
    p_outpic->b_force = p_pic->b_force;
    p_outpic->i_nb_fields = p_pic->i_nb_fields;
    p_outpic->b_progressive = p_pic->b_progressive;
    p_outpic->b_top_field_first = p_pic->b_top_field_first;

    if( p_pic->pf_release )
        p_pic->pf_release( p_pic );

    return p_outpic;
}
