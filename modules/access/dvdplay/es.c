/*****************************************************************************
 * es.c: functions to handle elementary streams.
 *****************************************************************************
 * Copyright (C) 2001 VideoLAN
 * $Id: es.c 6961 2004-03-05 17:34:23Z sam $
 *
 * Author: St�phane Borel <stef@via.ecp.fr>
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
#include <stdio.h>
#include <stdlib.h>

#include <vlc/vlc.h>
#include <vlc/input.h>

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#ifdef STRNCASECMP_IN_STRINGS_H
#   include <strings.h>
#endif

#include "dvd.h"
#include "iso_lang.h"

void dvdplay_LaunchDecoders( input_thread_t * p_input );

/*****************************************************************************
 * dvdplay_DeleteES:
 *****************************************************************************/
void dvdplay_DeleteES( input_thread_t* p_input )
{
    free( p_input->stream.pp_selected_es );

    p_input->stream.pp_selected_es = NULL;
    p_input->stream.i_selected_es_number = 0;

    while( p_input->stream.i_es_number )
    {
        input_DelES( p_input, p_input->stream.pp_es[0] );
    }

    free( p_input->stream.pp_es );

    p_input->stream.pp_es = NULL;
    p_input->stream.i_es_number = 0;

}

#define ADDES( id, fourcc, cat, lang, descr, size )                     \
    msg_Dbg( p_input, "new es 0x%x", i_id );                            \
    {                                                                   \
        char *psz_descr;                                                \
        psz_descr = malloc( strlen(DecodeLanguage( lang )) +            \
                            strlen(descr) + 1 );                        \
        if( psz_descr ) {strcpy( psz_descr, DecodeLanguage( lang ) );   \
            strcat( psz_descr, descr );}                                \
        p_es = input_AddES( p_input, NULL, id, cat,                     \
                            psz_descr, size );                          \
        if( psz_descr ) free( psz_descr );                              \
    }                                                                   \
    p_es->i_stream_id = i_id & 0xff;                                    \
    p_es->i_fourcc = (fourcc);

/*****************************************************************************
 * dvdplay_Video: read video ES
 *****************************************************************************/
void dvdplay_Video( input_thread_t * p_input )
{
    dvd_data_t *            p_dvd;
    es_descriptor_t *       p_es;
    video_attr_t *          p_attr;
    int                     i_id;

    p_dvd = (dvd_data_t*)(p_input->p_access_data);
    p_attr = dvdplay_video_attr( p_dvd->vmg );

    /* ES 0 -> video MPEG2 */
    i_id = 0xe0;

    if( p_attr->display_aspect_ratio )
    {
        ADDES( 0xe0, VLC_FOURCC('m','p','g','v'), VIDEO_ES, 0,
               "", sizeof(int) );
        *(int*)(p_es->p_demux_data) = p_attr->display_aspect_ratio;
    }
    else
    {
        ADDES( 0xe0, VLC_FOURCC('m','p','g','v'), VIDEO_ES, 0, "", 0 );
    }

}

/*****************************************************************************
 * dvdplay_Audio: read audio ES
 *****************************************************************************/
void dvdplay_Audio( input_thread_t * p_input )
{
    dvd_data_t *            p_dvd;
    es_descriptor_t *       p_es;
    audio_attr_t *          p_attr;
    int                     i_audio_nr  = -1;
    int                     i_audio     = -1;
    int                     i_channels;
    int                     i_lang;
    int                     i_id;
    int                     i;

    p_dvd = (dvd_data_t*)(p_input->p_access_data);
    p_dvd->i_audio_nb = 0;
    dvdplay_audio_info( p_dvd->vmg, &i_audio_nr, &i_audio );

    /* Audio ES, in the order they appear in .ifo */
    for( i = 1 ; i <= i_audio_nr ; i++ )
    {
        if( ( i_id = dvdplay_audio_id( p_dvd->vmg, i-1 ) ) > 0 )
        {
            p_attr     = dvdplay_audio_attr( p_dvd->vmg, i-1 );
            i_channels = p_attr->channels;
            i_lang     = p_attr->lang_code;

            ++p_dvd->i_audio_nb;

            switch( p_attr->audio_format )
            {
            case 0x00:              /* A52 */
                ADDES( i_id, VLC_FOURCC('a','5','2','b'), AUDIO_ES, i_lang,
                       " (A52)", 0 );

                break;
            case 0x02:
            case 0x03:              /* MPEG audio */
                ADDES( i_id, VLC_FOURCC('m','p','g','a'), AUDIO_ES, i_lang,
                       " (mpeg)", 0 );

                break;
            case 0x04:              /* LPCM */
                ADDES( i_id, VLC_FOURCC('l','p','c','b'), AUDIO_ES, i_lang,
                       " (lpcm)", 0 );

                break;
            case 0x05:              /* SDDS */
                ADDES( i_id, VLC_FOURCC('s','d','d','b'), AUDIO_ES, i_lang,
                       " (sdds)", 0 );

                break;
            case 0x06:              /* DTS */
                ADDES( i_id, VLC_FOURCC('d','t','s','b'), AUDIO_ES, i_lang,
                       " (dts)", 0 );

                break;
            default:
                i_id = 0;
                msg_Warn( p_input, "unknown audio type %.2x",
                             p_attr->audio_format );
            }
        }
    }
}

/*****************************************************************************
 * dvdplay_Subp: read subpictures ES
 *****************************************************************************/
void dvdplay_Subp( input_thread_t * p_input )
{
    dvd_data_t *            p_dvd;
    es_descriptor_t *       p_es;
    subp_attr_t *           p_attr;
    uint32_t *              pi_palette;
    int                     i_subp_nr   = -1;
    int                     i_subp      = -1;
    int                     i_id;
    int                     i;

    p_dvd = (dvd_data_t*)(p_input->p_access_data);
    p_dvd->i_spu_nb = 0;

    dvdplay_subp_info( p_dvd->vmg, &i_subp_nr, &i_subp );
    pi_palette = dvdplay_subp_palette( p_dvd->vmg );

    for( i = 1 ; i <= i_subp_nr; i++ )
    {
        if( ( i_id = dvdplay_subp_id( p_dvd->vmg, i-1 ) ) >= 0 )
        {
            p_attr = dvdplay_subp_attr( p_dvd->vmg, i-1 );
            ++p_dvd->i_spu_nb;

            if( pi_palette )
            {
                ADDES( i_id, VLC_FOURCC('s','p','u','b'), SPU_ES,
                       p_attr->lang_code, "", sizeof(int) + 16*sizeof(uint32_t) );
                *(int*)p_es->p_demux_data = 0xBeeF;
                memcpy( (void*)p_es->p_demux_data + sizeof(int),
                        pi_palette, 16*sizeof(uint32_t) );
            }
            else
            {
                ADDES( i_id, VLC_FOURCC('s','p','u','b'), SPU_ES,
                       p_attr->lang_code, "", 0 );
            }
        }
    }
}

/*****************************************************************************
 * dvdplay_LaunchDecoders
 *****************************************************************************/
void dvdplay_LaunchDecoders( input_thread_t * p_input )
{
    dvd_data_t *            p_dvd;
    int                     i_audio_nr  = -1;
    int                     i_audio     = -1;
    int                     i_subp_nr   = -1;
    int                     i_subp      = -1;

    p_dvd = (dvd_data_t*)(p_input->p_access_data);

    /* For audio: check user settings first, then check dvdplay settings. */
    i_audio = config_GetInt( p_input, "audio-channel" );
    if( i_audio <= 0 || i_audio > p_dvd->i_audio_nb )
    {
        /* if i_audio = -1 dvdplay_audio_info() will select a default channel,
         * otherwise it warns libdvdplay that we have chosen another stream. */
        i_audio = -1;
    }
    dvdplay_audio_info( p_dvd->vmg, &i_audio_nr, &i_audio );

    /* For spu: check user settings first, the check dvdplay settings. */
    i_subp = config_GetInt( p_input, "spu-channel" );
    if( i_subp <= 0 || i_subp > p_dvd->i_spu_nb )
    {
        i_subp = -1;
    }
    dvdplay_subp_info( p_dvd->vmg, &i_subp_nr, &i_subp );

    input_SelectES( p_input, p_input->stream.pp_es[0] );

    if( i_audio > p_dvd->i_audio_nb ) i_audio = 1;
    if( ( i_audio > 0 ) && ( p_dvd->i_audio_nb > 0 ) )
    {
        if( config_GetInt( p_input, "audio-type" ) == REQUESTED_A52 )
        {
            int     i_a52 = i_audio;

            while( ( i_a52 < p_dvd->i_audio_nb ) &&
                   ( p_input->stream.pp_es[i_a52]->i_fourcc !=
                        VLC_FOURCC('a','5','2','b') ) )
            {
                i_a52++;
            }
            if( p_input->stream.pp_es[i_a52]->i_fourcc ==
                    VLC_FOURCC('a','5','2','b') )
            {
                input_SelectES( p_input,
                                p_input->stream.pp_es[i_a52] );

                /* warn libdvdplay that we have chosen another stream */
                dvdplay_audio_info( p_dvd->vmg, &i_audio_nr, &i_a52 );
            }
            else
            {
                /* none found, select the default one */
                input_SelectES( p_input,
                                p_input->stream.pp_es[i_audio] );
            }
        }
        else
        {
            input_SelectES( p_input,
                            p_input->stream.pp_es[i_audio] );
        }
    }

    if( i_subp > p_dvd->i_spu_nb ) i_subp = -1;
    if( ( i_subp > 0 ) && ( p_dvd->i_spu_nb > 0 ) )
    {
        i_subp += p_dvd->i_audio_nb;
        input_SelectES( p_input, p_input->stream.pp_es[i_subp] );
    }
}

/*****************************************************************************
 * dvdplay_ES:
 *****************************************************************************/
void dvdplay_ES( input_thread_t * p_input )
{
    dvdplay_DeleteES      ( p_input );
    dvdplay_Video         ( p_input );
    dvdplay_Audio         ( p_input );
    dvdplay_Subp          ( p_input );
    dvdplay_LaunchDecoders( p_input );
}


