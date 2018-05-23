/*****************************************************************************
 * input_dec.c: Functions for the management of decoders
 *****************************************************************************
 * Copyright (C) 1999-2004 VideoLAN
 * $Id: input_dec.c 7606 2004-05-06 17:35:26Z gbazin $
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *          Gildas Bazin <gbazin@netcourrier.com>
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
#include <string.h>                                    /* memcpy(), memset() */

#include <vlc/vlc.h>
#include <vlc/decoder.h>
#include <vlc/vout.h>

#include "stream_output.h"

#include "input_ext-intf.h"
#include "input_ext-plugins.h"

#include "codecs.h"

static void input_NullPacket( input_thread_t *, es_descriptor_t * );

static decoder_t * CreateDecoder( input_thread_t *, es_descriptor_t *, int );
static int         DecoderThread( decoder_t * );
static int         DecoderDecode( decoder_t * p_dec, block_t *p_block );
static void        DeleteDecoder( decoder_t * );

/* Buffers allocation callbacks for the decoders */
static aout_buffer_t *aout_new_buffer( decoder_t *, int );
static void aout_del_buffer( decoder_t *, aout_buffer_t * );

static picture_t *vout_new_buffer( decoder_t * );
static void vout_del_buffer( decoder_t *, picture_t * );
static void vout_link_picture( decoder_t *, picture_t * );
static void vout_unlink_picture( decoder_t *, picture_t * );

static es_format_t null_es_format = {0};

struct decoder_owner_sys_t
{
    vlc_bool_t      b_own_thread;

    input_thread_t  *p_input;

    aout_instance_t *p_aout;
    aout_input_t    *p_aout_input;

    vout_thread_t   *p_vout;

    sout_instance_t         *p_sout;
    sout_packetizer_input_t *p_sout_input;

    /* Some decoders require already packetized data (ie. not truncated) */
    decoder_t *p_packetizer;

    /* Current format in use by the output */
    video_format_t video;
    audio_format_t audio;
    es_format_t    sout;

    /* fifo */
    block_fifo_t *p_fifo;

    /* */
    input_buffers_t *p_method_data;
    es_descriptor_t *p_es_descriptor;
};


/**
 * Spawns a new decoder thread
 *
 * \param p_input the input thread
 * \param p_es the es descriptor
 * \return the spawned decoder object
 */
decoder_t * input_RunDecoder( input_thread_t * p_input, es_descriptor_t * p_es )
{
    decoder_t   *p_dec = NULL;
    vlc_value_t val;

    /* If we are in sout mode, search for packetizer module */
    if( !p_es->b_force_decoder && p_input->stream.p_sout )
    {
        /* Create the decoder configuration structure */
        p_dec = CreateDecoder( p_input, p_es, VLC_OBJECT_PACKETIZER );
        if( p_dec == NULL )
        {
            msg_Err( p_input, "could not create packetizer" );
            return NULL;
        }
    }
    else
    {
        /* Create the decoder configuration structure */
        p_dec = CreateDecoder( p_input, p_es, VLC_OBJECT_DECODER );
        if( p_dec == NULL )
        {
            msg_Err( p_input, "could not create decoder" );
            return NULL;
        }
    }

    if( !p_dec->p_module )
    {
        msg_Err( p_dec, "no suitable decoder module for fourcc `%4.4s'.\n"
                 "VLC probably does not support this sound or video format.",
                 (char*)&p_dec->fmt_in.i_codec );

        DeleteDecoder( p_dec );
        vlc_object_destroy( p_dec );
        return NULL;
    }

    if( !p_es->b_force_decoder && p_input->stream.p_sout && p_input->stream.b_pace_control )
    {
        msg_Dbg( p_input, "stream out mode -> no decoder thread" );
        p_dec->p_owner->b_own_thread = VLC_FALSE;
    }
    else
    {
        var_Get( p_input, "minimize-threads", &val );
        p_dec->p_owner->b_own_thread = !val.b_bool;
    }

    if( p_dec->p_owner->b_own_thread )
    {
        int i_priority;
        if ( p_es->i_cat == AUDIO_ES )
        {
            i_priority = VLC_THREAD_PRIORITY_AUDIO;
        }
        else
        {
            i_priority = VLC_THREAD_PRIORITY_VIDEO;
        }

        /* Spawn the decoder thread */
        if( vlc_thread_create( p_dec, "decoder", DecoderThread,
                               i_priority, VLC_FALSE ) )
        {
            msg_Err( p_dec, "cannot spawn decoder thread \"%s\"",
                             p_dec->p_module->psz_object_name );
            module_Unneed( p_dec, p_dec->p_module );
            DeleteDecoder( p_dec );
            vlc_object_destroy( p_dec );
            return NULL;
        }
    }

    /* Select a new ES */
    INSERT_ELEM( p_input->stream.pp_selected_es,
                 p_input->stream.i_selected_es_number,
                 p_input->stream.i_selected_es_number,
                 p_es );

    p_input->stream.b_changed = 1;

    return p_dec;
}

/**
 * Kills a decoder thread and waits until it's finished
 *
 * \param p_input the input thread
 * \param p_es the es descriptor
 * \return nothing
 */
void input_EndDecoder( input_thread_t * p_input, es_descriptor_t * p_es )
{
    decoder_t *p_dec = p_es->p_dec;

    p_dec->b_die = VLC_TRUE;

    if( p_dec->p_owner->b_own_thread )
    {
        /* Make sure the thread leaves the function by
         * sending it an empty block. */
        block_t *p_block = block_New( p_dec, 0 );
        input_DecodeBlock( p_dec, p_block );

        vlc_thread_join( p_dec );

        /* Don't module_Unneed() here because of the dll loader that wants
         * close() in the same thread than open()/decode() */
    }
    else
    {
        module_Unneed( p_dec, p_dec->p_module );
    }

    /* Delete decoder configuration */
    DeleteDecoder( p_dec );

    /* Delete the decoder */
    vlc_object_destroy( p_dec );

    /* Tell the input there is no more decoder */
    p_es->p_dec = NULL;

    p_input->stream.b_changed = 1;
}

/**
 * Put a PES in the decoder's fifo.
 *
 * \param p_dec the decoder object
 * \param p_pes the pes packet
 * \return nothing
 */
void input_DecodePES( decoder_t * p_dec, pes_packet_t * p_pes )
{
    data_packet_t *p_data;
    int     i_size = 0;

    for( p_data = p_pes->p_first; p_data != NULL; p_data = p_data->p_next )
    {
        i_size += p_data->p_payload_end - p_data->p_payload_start;
    }
    if( i_size > 0 )
    {
        block_t *p_block = block_New( p_dec, i_size );
        if( p_block )
        {
            uint8_t *p_buffer = p_block->p_buffer;

            for( p_data = p_pes->p_first; p_data; p_data = p_data->p_next )
            {
                int i_copy = p_data->p_payload_end - p_data->p_payload_start;

                memcpy( p_buffer, p_data->p_payload_start, i_copy );

                p_buffer += i_copy;
            }
            p_block->i_pts = p_pes->i_pts;
            p_block->i_dts = p_pes->i_dts;
            if( p_pes->b_discontinuity )
                p_block->i_flags |= BLOCK_FLAG_DISCONTINUITY;
            p_block->i_rate = p_pes->i_rate;

            input_DecodeBlock( p_dec, p_block );
        }
    }

    input_DeletePES( p_dec->p_owner->p_method_data, p_pes );
}

/**
 * Put a block_t in the decoder's fifo.
 *
 * \param p_dec the decoder object
 * \param p_block the data block
 */
void input_DecodeBlock( decoder_t * p_dec, block_t *p_block )
{
    if( p_dec->p_owner->b_own_thread )
    {
        block_FifoPut( p_dec->p_owner->p_fifo, p_block );

        if( p_dec->p_owner->p_input->b_out_pace_control )
        {
            /* FIXME !!!!! */
            while( !p_dec->b_die && !p_dec->b_error &&
                   p_dec->p_owner->p_fifo->i_depth > 10 )
            {
                msleep( 1000 );
            }
        }
    }
    else
    {
        if( p_dec->b_error || p_block->i_buffer <= 0 )
        {
            block_Release( p_block );
        }
        else
        {
            DecoderDecode( p_dec, p_block );
        }
    }
}

/**
 * Create a NULL packet for padding in case of a data loss
 *
 * \param p_input the input thread
 * \param p_es es descriptor
 * \return nothing
 */
static void input_NullPacket( input_thread_t * p_input,
                              es_descriptor_t * p_es )
{
    block_t *p_block = block_New( p_input, PADDING_PACKET_SIZE );
    if( p_block )
    {
        memset( p_block->p_buffer, 0, PADDING_PACKET_SIZE );
        p_block->i_flags |= BLOCK_FLAG_DISCONTINUITY;

        block_FifoPut( p_es->p_dec->p_owner->p_fifo, p_block );
    }
}

/**
 * Send a NULL packet to the decoders
 *
 * \param p_input the input thread
 * \return nothing
 */
void input_EscapeDiscontinuity( input_thread_t * p_input )
{
    unsigned int i_es, i;

    for( i_es = 0; i_es < p_input->stream.i_selected_es_number; i_es++ )
    {
        es_descriptor_t * p_es = p_input->stream.pp_selected_es[i_es];

        if( p_es->p_dec != NULL )
        {
            for( i = 0; i < PADDING_PACKET_NUMBER; i++ )
            {
                input_NullPacket( p_input, p_es );
            }
        }
    }
}

/**
 * Send a NULL packet to the audio decoders
 *
 * \param p_input the input thread
 * \return nothing
 */
void input_EscapeAudioDiscontinuity( input_thread_t * p_input )
{
    unsigned int i_es, i;

    for( i_es = 0; i_es < p_input->stream.i_selected_es_number; i_es++ )
    {
        es_descriptor_t * p_es = p_input->stream.pp_selected_es[i_es];

        if( p_es->p_dec != NULL && p_es->i_cat == AUDIO_ES )
        {
            for( i = 0; i < PADDING_PACKET_NUMBER; i++ )
            {
                input_NullPacket( p_input, p_es );
            }
        }
    }
}

/**
 * Create a decoder object
 *
 * \param p_input the input thread
 * \param p_es the es descriptor
 * \param i_object_type Object type as define in include/vlc_objects.h
 * \return the decoder object
 */
static decoder_t * CreateDecoder( input_thread_t * p_input,
                                  es_descriptor_t * p_es, int i_object_type )
{
    decoder_t *p_dec;

    p_dec = vlc_object_create( p_input, i_object_type );
    if( p_dec == NULL )
    {
        msg_Err( p_input, "out of memory" );
        return NULL;
    }

    p_dec->pf_decode_audio = 0;
    p_dec->pf_decode_video = 0;
    p_dec->pf_decode_sub = 0;
    p_dec->pf_packetize = 0;

    /* Initialize the decoder fifo */
    p_dec->p_module = NULL;

    es_format_Copy( &p_dec->fmt_in, &p_es->fmt );

    if( p_es->p_waveformatex )
    {
#define p_wf ((WAVEFORMATEX *)p_es->p_waveformatex)
        p_dec->fmt_in.audio.i_channels = p_wf->nChannels;
        p_dec->fmt_in.audio.i_rate = p_wf->nSamplesPerSec;
        p_dec->fmt_in.i_bitrate = p_wf->nAvgBytesPerSec * 8;
        p_dec->fmt_in.audio.i_blockalign = p_wf->nBlockAlign;
        p_dec->fmt_in.audio.i_bitspersample = p_wf->wBitsPerSample;
        p_dec->fmt_in.i_extra = p_wf->cbSize;
        p_dec->fmt_in.p_extra = NULL;
        if( p_wf->cbSize )
        {
            p_dec->fmt_in.p_extra = malloc( p_wf->cbSize );
            memcpy( p_dec->fmt_in.p_extra, &p_wf[1], p_wf->cbSize );
        }
    }

    if( p_es->p_bitmapinfoheader )
    {
#define p_bih ((BITMAPINFOHEADER *) p_es->p_bitmapinfoheader)
        p_dec->fmt_in.i_extra = p_bih->biSize - sizeof(BITMAPINFOHEADER);
        p_dec->fmt_in.p_extra = NULL;
        if( p_dec->fmt_in.i_extra )
        {
            p_dec->fmt_in.p_extra = malloc( p_dec->fmt_in.i_extra );
            memcpy( p_dec->fmt_in.p_extra, &p_bih[1], p_dec->fmt_in.i_extra );
        }

        p_dec->fmt_in.video.i_width = p_bih->biWidth;
        p_dec->fmt_in.video.i_height = p_bih->biHeight;
    }

    /* FIXME
     *  - 1: beurk
     *  - 2: I'm not sure there isn't any endian problem here (spu)... */
    if( p_es->i_cat == SPU_ES && p_es->p_demux_data )
    {
        if( ( p_es->i_fourcc == VLC_FOURCC( 's', 'p', 'u', ' ' ) ||
              p_es->i_fourcc == VLC_FOURCC( 's', 'p', 'u', 'b' ) ) &&
            *((uint32_t*)p_es->p_demux_data) == 0xBeef )
        {
            memcpy( p_dec->fmt_in.subs.spu.palette,
                    p_es->p_demux_data, 17 * 4 );
        }
        else if( p_es->i_fourcc == VLC_FOURCC( 'd', 'v', 'b', 's' ) &&
                 p_es->p_spuinfo )
        {
            dvb_spuinfo_t *p_dvbs = (dvb_spuinfo_t*)p_es->p_spuinfo;
            p_dec->fmt_in.subs.dvb.i_id = p_dvbs->i_id;
        }
    }

    p_dec->fmt_in.i_cat = p_es->i_cat;
    p_dec->fmt_in.i_codec = p_es->i_fourcc;

    p_dec->fmt_out = null_es_format;

    /* Allocate our private structure for the decoder */
    p_dec->p_owner = (decoder_owner_sys_t*)malloc(sizeof(decoder_owner_sys_t));
    if( p_dec->p_owner == NULL )
    {
        msg_Err( p_dec, "out of memory" );
        return NULL;
    }
    p_dec->p_owner->b_own_thread = VLC_TRUE;
    p_dec->p_owner->p_input = p_input;
    p_dec->p_owner->p_aout = NULL;
    p_dec->p_owner->p_aout_input = NULL;
    p_dec->p_owner->p_vout = NULL;
    p_dec->p_owner->p_sout = p_input->stream.p_sout;
    p_dec->p_owner->p_sout_input = NULL;
    p_dec->p_owner->p_packetizer = NULL;
    p_dec->p_owner->p_es_descriptor = p_es;


    /* decoder fifo */
    if( ( p_dec->p_owner->p_fifo = block_FifoNew( p_dec ) ) == NULL )
    {
        msg_Err( p_dec, "out of memory" );
        return NULL;
    }
    p_dec->p_owner->p_method_data = p_input->p_method_data;

    /* Set buffers allocation callbacks for the decoders */
    p_dec->pf_aout_buffer_new = aout_new_buffer;
    p_dec->pf_aout_buffer_del = aout_del_buffer;
    p_dec->pf_vout_buffer_new = vout_new_buffer;
    p_dec->pf_vout_buffer_del = vout_del_buffer;
    p_dec->pf_picture_link    = vout_link_picture;
    p_dec->pf_picture_unlink  = vout_unlink_picture;

    vlc_object_attach( p_dec, p_input );

    /* Find a suitable decoder/packetizer module */
    if( i_object_type == VLC_OBJECT_DECODER )
        p_dec->p_module = module_Need( p_dec, "decoder", "$codec", 0 );
    else
        p_dec->p_module = module_Need( p_dec, "packetizer", "$packetizer", 0 );

    /* Check if decoder requires already packetized data */
    if( i_object_type == VLC_OBJECT_DECODER &&
        p_dec->b_need_packetized && !p_dec->fmt_in.b_packetized )
    {
        p_dec->p_owner->p_packetizer =
            vlc_object_create( p_input, VLC_OBJECT_PACKETIZER );
        if( p_dec->p_owner->p_packetizer )
        {
            p_dec->p_owner->p_packetizer->fmt_in = null_es_format;
            p_dec->p_owner->p_packetizer->fmt_out = null_es_format;
            es_format_Copy( &p_dec->p_owner->p_packetizer->fmt_in,
                            &p_dec->fmt_in );

            vlc_object_attach( p_dec->p_owner->p_packetizer, p_input );

            p_dec->p_owner->p_packetizer->p_module =
                module_Need( p_dec->p_owner->p_packetizer,
                             "packetizer", "$packetizer", 0 );

            if( !p_dec->p_owner->p_packetizer->p_module )
            {
                es_format_Clean( &p_dec->p_owner->p_packetizer->fmt_in );
                vlc_object_detach( p_dec->p_owner->p_packetizer );
                vlc_object_destroy( p_dec->p_owner->p_packetizer );
            }
        }
    }

    return p_dec;
}

/**
 * The decoding main loop
 *
 * \param p_dec the decoder
 * \return 0
 */
static int DecoderThread( decoder_t * p_dec )
{
    block_t *p_block;

    /* The decoder's main loop */
    while( !p_dec->b_die && !p_dec->b_error )
    {
        if( ( p_block = block_FifoGet( p_dec->p_owner->p_fifo ) ) == NULL )
        {
            p_dec->b_error = 1;
            break;
        }
        if( DecoderDecode( p_dec, p_block ) != VLC_SUCCESS )
        {
            break;
        }
    }

    while( !p_dec->b_die )
    {
        /* Trash all received PES packets */
        p_block = block_FifoGet( p_dec->p_owner->p_fifo );
        if( p_block ) block_Release( p_block );
    }

    /* We do it here because of the dll loader that wants close() in the
     * same thread than open()/decode() */
    module_Unneed( p_dec, p_dec->p_module );

    return 0;
}

/**
 * Decode a block
 *
 * \param p_dec the decoder object
 * \param p_block the block to decode
 * \return VLC_SUCCESS or an error code
 */
static int DecoderDecode( decoder_t *p_dec, block_t *p_block )
{
    if( p_block->i_buffer <= 0 )
    {
        block_Release( p_block );
        return VLC_SUCCESS;
    }

    if( p_dec->i_object_type == VLC_OBJECT_PACKETIZER )
    {
        block_t *p_sout_block;

        while( (p_sout_block = p_dec->pf_packetize( p_dec, &p_block )) )
        {
            if( !p_dec->p_owner->p_sout_input )
            {
                es_format_Copy( &p_dec->p_owner->sout, &p_dec->fmt_out );
                if( p_dec->p_owner->p_es_descriptor->p_pgrm )
                {
                    p_dec->p_owner->sout.i_group =
                        p_dec->p_owner->p_es_descriptor->p_pgrm->i_number;
                }
                p_dec->p_owner->sout.i_id =
                    p_dec->p_owner->p_es_descriptor->i_id - 1;
                if( p_dec->fmt_in.psz_language )
                {
                    p_dec->p_owner->sout.psz_language =
                        strdup( p_dec->fmt_in.psz_language );
                }

                p_dec->p_owner->p_sout_input =
                    sout_InputNew( p_dec->p_owner->p_sout,
                                   &p_dec->p_owner->sout );

                if( p_dec->p_owner->p_sout_input == NULL )
                {
                    msg_Err( p_dec, "cannot create packetizer output" );
                    p_dec->b_error = VLC_TRUE;

                    while( p_sout_block )
                    {
                        block_t *p_next = p_sout_block->p_next;
                        block_Release( p_sout_block );
                        p_sout_block = p_next;
                    }
                    break;
                }
            }

            while( p_sout_block )
            {
                block_t       *p_next = p_sout_block->p_next;

                p_sout_block->p_next = NULL;

                sout_InputSendBuffer( p_dec->p_owner->p_sout_input,
                                      p_sout_block );

                p_sout_block = p_next;
            }

            /* For now it's enough, as only sout inpact on this flag */
            if( p_dec->p_owner->p_sout->i_out_pace_nocontrol > 0 &&
                p_dec->p_owner->p_input->b_out_pace_control )
            {
                msg_Dbg( p_dec, "switching to synch mode" );
                p_dec->p_owner->p_input->b_out_pace_control = VLC_FALSE;
            }
            else if( p_dec->p_owner->p_sout->i_out_pace_nocontrol <= 0 &&
                     !p_dec->p_owner->p_input->b_out_pace_control )
            {
                msg_Dbg( p_dec, "switching to asynch mode" );
                p_dec->p_owner->p_input->b_out_pace_control = VLC_TRUE;
            }
        }
    }
    else if( p_dec->fmt_in.i_cat == AUDIO_ES )
    {
        aout_buffer_t *p_aout_buf;

        if( p_dec->p_owner->p_packetizer )
        {
            block_t *p_packetized_block;
            decoder_t *p_packetizer = p_dec->p_owner->p_packetizer;

            while( (p_packetized_block =
                    p_packetizer->pf_packetize( p_packetizer, &p_block )) )
            {
                while( (p_aout_buf =
                        p_dec->pf_decode_audio( p_dec, &p_packetized_block )) )
                {
                    aout_DecPlay( p_dec->p_owner->p_aout,
                                  p_dec->p_owner->p_aout_input, p_aout_buf );
                }
            }
        }
        else while( (p_aout_buf = p_dec->pf_decode_audio( p_dec, &p_block )) )
        {
            aout_DecPlay( p_dec->p_owner->p_aout,
                          p_dec->p_owner->p_aout_input, p_aout_buf );
        }
    }
    else if( p_dec->fmt_in.i_cat == VIDEO_ES )
    {
        picture_t *p_pic;

        if( p_dec->p_owner->p_packetizer )
        {
            block_t *p_packetized_block;
            decoder_t *p_packetizer = p_dec->p_owner->p_packetizer;

            while( (p_packetized_block =
                    p_packetizer->pf_packetize( p_packetizer, &p_block )) )
            {
                while( (p_pic =
                        p_dec->pf_decode_video( p_dec, &p_packetized_block )) )
                {
                    vout_DatePicture( p_dec->p_owner->p_vout, p_pic,
                                      p_pic->date );
                    vout_DisplayPicture( p_dec->p_owner->p_vout, p_pic );
                }
            }
        }
        else while( (p_pic = p_dec->pf_decode_video( p_dec, &p_block )) )
        {
            vout_DatePicture( p_dec->p_owner->p_vout, p_pic, p_pic->date );
            vout_DisplayPicture( p_dec->p_owner->p_vout, p_pic );
        }
    }
    else if( p_dec->fmt_in.i_cat == SPU_ES )
    {
        p_dec->pf_decode_sub( p_dec, &p_block );
    }
    else
    {
        msg_Err( p_dec, "unknown ES format" );
        p_dec->b_error = 1;
    }

    return p_dec->b_error ? VLC_EGENERIC : VLC_SUCCESS;
}

/**
 * Destroys a decoder object
 *
 * \param p_dec the decoder object
 * \return nothing
 */
static void DeleteDecoder( decoder_t * p_dec )
{
    vlc_object_detach( p_dec );

    msg_Dbg( p_dec, "killing decoder fourcc `%4.4s', %d PES in FIFO",
             (char*)&p_dec->fmt_in.i_codec,
             p_dec->p_owner->p_fifo->i_depth );

    /* Free all packets still in the decoder fifo. */
    block_FifoEmpty( p_dec->p_owner->p_fifo );
    block_FifoRelease( p_dec->p_owner->p_fifo );

   /* Cleanup */
    if( p_dec->p_owner->p_aout_input )
        aout_DecDelete( p_dec->p_owner->p_aout, p_dec->p_owner->p_aout_input );

    if( p_dec->p_owner->p_vout )
    {
        int i_pic;

#define p_pic p_dec->p_owner->p_vout->render.pp_picture[i_pic]
        /* Hack to make sure all the the pictures are freed by the decoder */
        for( i_pic = 0; i_pic < p_dec->p_owner->p_vout->render.i_pictures;
             i_pic++ )
        {
            if( p_pic->i_status == RESERVED_PICTURE )
                vout_DestroyPicture( p_dec->p_owner->p_vout, p_pic );
            if( p_pic->i_refcount > 0 )
                vout_UnlinkPicture( p_dec->p_owner->p_vout, p_pic );
        }
#undef p_pic

        /* We are about to die. Reattach video output to p_vlc. */
        vout_Request( p_dec, p_dec->p_owner->p_vout, 0, 0, 0, 0 );
    }

    if( p_dec->p_owner->p_sout_input )
    {
        sout_InputDelete( p_dec->p_owner->p_sout_input );
        es_format_Clean( &p_dec->p_owner->sout );
    }

    es_format_Clean( &p_dec->fmt_in );
    es_format_Clean( &p_dec->fmt_out );

    if( p_dec->p_owner->p_packetizer )
    {
        module_Unneed( p_dec->p_owner->p_packetizer,
                       p_dec->p_owner->p_packetizer->p_module );
        es_format_Clean( &p_dec->p_owner->p_packetizer->fmt_in );
        es_format_Clean( &p_dec->p_owner->p_packetizer->fmt_out );
        vlc_object_detach( p_dec->p_owner->p_packetizer );
        vlc_object_destroy( p_dec->p_owner->p_packetizer );
    }

    free( p_dec->p_owner );
}

/*****************************************************************************
 * Buffers allocation callbacks for the decoders
 *****************************************************************************/
static aout_buffer_t *aout_new_buffer( decoder_t *p_dec, int i_samples )
{
    decoder_owner_sys_t *p_sys = (decoder_owner_sys_t *)p_dec->p_owner;
    aout_buffer_t *p_buffer;

    if( p_sys->p_aout_input != NULL &&
        ( p_dec->fmt_out.audio.i_rate != p_sys->audio.i_rate ||
          p_dec->fmt_out.audio.i_original_channels !=
              p_sys->audio.i_original_channels ||
          p_dec->fmt_out.audio.i_bytes_per_frame !=
              p_sys->audio.i_bytes_per_frame ) )
    {
        /* Parameters changed, restart the aout */
        aout_DecDelete( p_sys->p_aout, p_sys->p_aout_input );
        p_sys->p_aout_input = NULL;
    }

    if( p_sys->p_aout_input == NULL )
    {
        p_dec->fmt_out.audio.i_format = p_dec->fmt_out.i_codec;
        p_sys->audio = p_dec->fmt_out.audio;
        p_sys->p_aout_input =
            aout_DecNew( p_dec, &p_sys->p_aout, &p_sys->audio );
        if( p_sys->p_aout_input == NULL )
        {
            msg_Err( p_dec, "failed to create audio output" );
            p_dec->b_error = VLC_TRUE;
            return NULL;
        }
        p_dec->fmt_out.audio.i_bytes_per_frame =
            p_sys->audio.i_bytes_per_frame;
    }

    p_buffer = aout_DecNewBuffer( p_sys->p_aout, p_sys->p_aout_input,
                                  i_samples );

    return p_buffer;
}

static void aout_del_buffer( decoder_t *p_dec, aout_buffer_t *p_buffer )
{
    aout_DecDeleteBuffer( p_dec->p_owner->p_aout,
                          p_dec->p_owner->p_aout_input, p_buffer );
}

static picture_t *vout_new_buffer( decoder_t *p_dec )
{
    decoder_owner_sys_t *p_sys = (decoder_owner_sys_t *)p_dec->p_owner;
    picture_t *p_pic;

    if( p_sys->p_vout == NULL ||
        p_dec->fmt_out.video.i_width != p_sys->video.i_width ||
        p_dec->fmt_out.video.i_height != p_sys->video.i_height ||
        p_dec->fmt_out.video.i_chroma != p_sys->video.i_chroma ||
        p_dec->fmt_out.video.i_aspect != p_sys->video.i_aspect )
    {
        if( !p_dec->fmt_out.video.i_width ||
            !p_dec->fmt_out.video.i_height )
        {
            /* Can't create a new vout without display size */
            return NULL;
        }

        p_dec->fmt_out.video.i_chroma = p_dec->fmt_out.i_codec;
        p_sys->video = p_dec->fmt_out.video;

        p_sys->p_vout = vout_Request( p_dec, p_sys->p_vout,
                                      p_sys->video.i_width,
                                      p_sys->video.i_height,
                                      p_sys->video.i_chroma,
                                      p_sys->video.i_aspect );

        if( p_sys->p_vout == NULL )
        {
            msg_Err( p_dec, "failed to create video output" );
            p_dec->b_error = VLC_TRUE;
            return NULL;
        }
    }

    /* Get a new picture */
    while( !(p_pic = vout_CreatePicture( p_sys->p_vout, 0, 0, 0 ) ) )
    {
        int i_pic, i_ready_pic = 0;

        if( p_dec->b_die || p_dec->b_error )
        {
            return NULL;
        }

#define p_pic p_dec->p_owner->p_vout->render.pp_picture[i_pic]
        /* Check the decoder doesn't leak pictures */
        for( i_pic = 0; i_pic < p_dec->p_owner->p_vout->render.i_pictures;
             i_pic++ )
        {
            if( p_pic->i_status == READY_PICTURE && i_ready_pic++ > 0 ) break;

            if( p_pic->i_status != DISPLAYED_PICTURE &&
                p_pic->i_status != RESERVED_PICTURE &&
                p_pic->i_status != READY_PICTURE ) break;

            if( !p_pic->i_refcount ) break;
        }
        if( i_pic == p_dec->p_owner->p_vout->render.i_pictures )
        {
            msg_Err( p_dec, "decoder is leaking pictures, resetting the heap" );

            /* Just free all the pictures */
            for( i_pic = 0; i_pic < p_dec->p_owner->p_vout->render.i_pictures;
                 i_pic++ )
            {
                if( p_pic->i_status == RESERVED_PICTURE )
                    vout_DestroyPicture( p_dec->p_owner->p_vout, p_pic );
                if( p_pic->i_refcount > 0 )
                vout_UnlinkPicture( p_dec->p_owner->p_vout, p_pic );
            }
        }
#undef p_pic

        msleep( VOUT_OUTMEM_SLEEP );
    }

    return p_pic;
}

static void vout_del_buffer( decoder_t *p_dec, picture_t *p_pic )
{
    vout_DestroyPicture( p_dec->p_owner->p_vout, p_pic );
}

static void vout_link_picture( decoder_t *p_dec, picture_t *p_pic )
{
    vout_LinkPicture( p_dec->p_owner->p_vout, p_pic );
}

static void vout_unlink_picture( decoder_t *p_dec, picture_t *p_pic )
{
    vout_UnlinkPicture( p_dec->p_owner->p_vout, p_pic );
}
