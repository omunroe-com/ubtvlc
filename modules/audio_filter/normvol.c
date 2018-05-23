/*****************************************************************************
 * normvol.c :  volume normalizer
 *****************************************************************************
 * Copyright (C) 2001 VideoLAN
 * $Id: normvol.c 9126 2004-11-03 00:07:38Z gbazin $
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

/*
 * TODO:
 *
 * We should detect fast power increases and react faster to these
 * This way, we can increase the buffer size to get a more stable filter */


/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <stdlib.h>                                      /* malloc(), free() */
#include <string.h>

#include <errno.h>                                                 /* ENOMEM */
#include <stdio.h>
#include <ctype.h>
#include <signal.h>

#include <math.h>


#include <vlc/vlc.h>
#include <vlc/aout.h>

#include <aout_internal.h>

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/

static int  Open     ( vlc_object_t * );
static void Close    ( vlc_object_t * );
static void DoWork   ( aout_instance_t * , aout_filter_t *,
                aout_buffer_t * , aout_buffer_t *);

typedef struct aout_filter_sys_t
{
    int i_nb;
    float *p_last;
    float f_max;
} aout_filter_sys_t;

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define BUFF_TEXT N_("Number of audio buffers" )
#define BUFF_LONGTEXT N_("This is the number of audio buffers on which the " \
                "power measurement is made. A higher number of buffers will " \
                "increase the response time of the filter to a high " \
                "power but will make it less sensitive to short variations." )

#define LEVEL_TEXT N_("Max level" )
#define LEVEL_LONGTEXT N_("If the average power over the last N buffers " \
               "is higher than this value, the volume will be normalized. " \
               "This value is a positive floating point number. A value " \
               "between 0.5 and 10 seems sensible." )

vlc_module_begin();
    set_description( _("Volume normalizer") );
    add_shortcut( "volnorm" );
    add_integer( "norm-buff-size", 20 ,NULL ,BUFF_TEXT, BUFF_LONGTEXT,
                 VLC_TRUE);
    add_float( "norm-max-level", 2.0, NULL, LEVEL_TEXT,
               LEVEL_LONGTEXT, VLC_TRUE );
    set_capability( "audio filter", 0 );
    set_callbacks( Open, Close );
vlc_module_end();

/*****************************************************************************
 * Open: initialize and create stuff
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    aout_filter_t *p_filter = (aout_filter_t*)p_this;
    int i_channels;
    aout_filter_sys_t *p_sys = p_filter->p_sys =
        malloc( sizeof( aout_filter_sys_t ) );

    if( p_filter->input.i_format != VLC_FOURCC('f','l','3','2' ) ||
        p_filter->output.i_format != VLC_FOURCC('f','l','3','2') )
    {
            msg_Warn( p_filter, "Bad input or output format" );
            return VLC_EGENERIC;
    }

    if ( !AOUT_FMTS_SIMILAR( &p_filter->input, &p_filter->output ) )
    {
        msg_Warn( p_filter, "input and output formats are not similar" );
        return VLC_EGENERIC;
    }

    p_filter->pf_do_work = DoWork;
    p_filter->b_in_place = VLC_TRUE;

    i_channels = aout_FormatNbChannels( &p_filter->input );

    p_sys->i_nb = var_CreateGetInteger( p_filter->p_parent, "norm-buff-size" );
    p_sys->f_max = var_CreateGetFloat( p_filter->p_parent, "norm-max-level" );

    if( p_sys->f_max <= 0 ) p_sys->f_max = 0.01;

    /* We need to store (nb_buffers+1)*nb_channels floats */
    p_sys->p_last = malloc( sizeof( float ) * (i_channels) *
                            (p_filter->p_sys->i_nb + 2) );
    memset( p_sys->p_last, 0 ,sizeof( float ) * (i_channels) *
            (p_filter->p_sys->i_nb + 2) );
    return VLC_SUCCESS;
}

/*****************************************************************************
 * DoWork : normalizes and sends a buffer
 *****************************************************************************/
 static void DoWork( aout_instance_t *p_aout, aout_filter_t *p_filter,
                     aout_buffer_t *p_in_buf, aout_buffer_t *p_out_buf )
{
    float *pf_sum;
    float *pf_gain;
    float f_average = 0;
    int i, i_chan;

    int i_samples = p_in_buf->i_nb_samples;
    int i_channels = aout_FormatNbChannels( &p_filter->input );
    float *p_out = (float*)p_out_buf->p_buffer;
    float *p_in =  (float*)p_in_buf->p_buffer;

    struct aout_filter_sys_t *p_sys = p_filter->p_sys;

    pf_sum = (float *)malloc( sizeof(float) * i_channels );
    memset( pf_sum, 0, sizeof(float) * i_channels );

    pf_gain = (float *)malloc( sizeof(float) * i_channels );

    p_out_buf->i_nb_samples = p_in_buf->i_nb_samples;
    p_out_buf->i_nb_bytes = p_in_buf->i_nb_bytes;

    /* Calculate the average power level on this buffer */
    for( i = 0 ; i < i_samples; i++ )
    {
        for( i_chan = 0; i_chan < i_channels; i_chan++ )
        {
            float f_sample = p_in[i_chan];
            float f_square = pow( f_sample, 2 );
            pf_sum[i_chan] += f_square;
        }
        p_in += i_channels;
    }

    /* sum now contains for each channel the sigma(value�) */
    for( i_chan = 0; i_chan < i_channels; i_chan++ )
    {
        /* Shift our lastbuff */
        memmove( &p_sys->p_last[ i_chan * p_sys->i_nb],
                        &p_sys->p_last[i_chan * p_sys->i_nb + 1],
                 (p_sys->i_nb-1) * sizeof( float ) );

        /* Insert the new average : sqrt(sigma(value�)) */
        p_sys->p_last[ i_chan * p_sys->i_nb + p_sys->i_nb - 1] =
                sqrt( pf_sum[i_chan] );

        pf_sum[i_chan] = 0;

        /* Get the average power on the lastbuff */
        f_average = 0;
        for( i = 0; i < p_sys->i_nb ; i++)
        {
            f_average += p_sys->p_last[ i_chan * p_sys->i_nb + i ];
        }
        f_average = f_average / p_sys->i_nb;

        /* Seuil arbitraire */
        p_sys->f_max = var_GetFloat( p_aout, "norm-max-level" );

        //fprintf(stderr,"Average %f, max %f\n", f_average, p_sys->f_max );
        if( f_average > p_sys->f_max )
        {
             pf_gain[i_chan] = f_average / p_sys->f_max;
        }
        else
        {
           pf_gain[i_chan] = 1;
        }
    }

    /* Apply gain */
    for( i = 0; i < i_samples; i++)
    {
        for( i_chan = 0; i_chan < i_channels; i_chan++ )
        {
            p_out[i_chan] /= pf_gain[i_chan];
        }
        p_out += i_channels;
    }

    free( pf_sum );
    free( pf_gain );

    return;
}

/**********************************************************************
 * Close
 **********************************************************************/
static void Close( vlc_object_t *p_this )
{
    aout_filter_t *p_filter = (aout_filter_t*)p_this;
    aout_filter_sys_t *p_sys = p_filter->p_sys;

    if( p_sys )
    {
        if( p_sys->p_last) free( p_sys->p_last );
        free( p_sys );
    }
}
