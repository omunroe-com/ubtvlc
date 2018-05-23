/*****************************************************************************
 * mpeg_ts.c : Transport Stream input module for vlc
 *****************************************************************************
 * Copyright (C) 2000-2004 VideoLAN
 * $Id: ts.c 7421 2004-04-22 03:25:47Z fenrir $
 *
 * Authors: Henri Fallon <henri@via.ecp.fr>
 *          Johan Bilien <jobi@via.ecp.fr>
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

#include "iso_lang.h"

#if defined MODULE_NAME_IS_ts_dvbpsi
#   ifdef HAVE_DVBPSI_DR_H
#       include <dvbpsi/dvbpsi.h>
#       include <dvbpsi/descriptor.h>
#       include <dvbpsi/pat.h>
#       include <dvbpsi/pmt.h>
#       include <dvbpsi/dr.h>
#   else
#       include "dvbpsi.h"
#       include "descriptor.h"
#       include "tables/pat.h"
#       include "tables/pmt.h"
#       include "descriptors/dr.h"
#   endif
#endif

#include "system.h"
#include "codecs.h"

/*****************************************************************************
 * Constants
 *****************************************************************************/
#define TS_READ_ONCE 200

/*****************************************************************************
 * Private structure
 *****************************************************************************/
struct demux_sys_t
{
    module_t *   p_module;
    mpeg_demux_t mpeg;
};


#define local_iso639_getlang(p1, p2)                                         \
{                                                                           \
    const iso639_lang_t * p_iso;                                            \
    p_iso = GetLang_2T((char*)(p1));                                        \
    if( p_iso && strcmp(p_iso->psz_native_name,"Unknown"))                  \
    {                                                                       \
        if( p_iso->psz_native_name[0] )                                     \
            strncpy( (p2), p_iso->psz_native_name, 20 );                    \
        else                                                                \
            strncpy( (p2), p_iso->psz_eng_name, 20 );                       \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        p_iso = GetLang_2B((char*)(p1));                                    \
        if ( p_iso )                                                        \
        {                                                                   \
            if( p_iso->psz_native_name[0] )                                 \
                strncpy( (p2), p_iso->psz_native_name, 20 );                \
            else                                                            \
                strncpy( (p2), p_iso->psz_eng_name, 20 );                   \
        }                                                                   \
        else                                                                \
        {                                                                   \
            strncpy( (p2), p1, 3 );                                         \
        }                                                                   \
    }                                                                       \
}

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int  Activate   ( vlc_object_t * );
static void Deactivate ( vlc_object_t * );
static int  Demux      ( input_thread_t * );

#if defined MODULE_NAME_IS_ts
static void TSDemuxPSI ( input_thread_t *, data_packet_t *,
                          es_descriptor_t *, vlc_bool_t );
static void TSDecodePAT( input_thread_t *, es_descriptor_t *);
static void TSDecodePMT( input_thread_t *, es_descriptor_t *);
#define PSI_CALLBACK TSDemuxPSI
#elif defined MODULE_NAME_IS_ts_dvbpsi
static void TS_DVBPSI_DemuxPSI  ( input_thread_t *, data_packet_t *,
                                  es_descriptor_t *, vlc_bool_t );
static void TS_DVBPSI_HandlePAT ( input_thread_t *, dvbpsi_pat_t * );
static void TS_DVBPSI_HandlePMT ( input_thread_t *, dvbpsi_pmt_t * );
#define PSI_CALLBACK TS_DVBPSI_DemuxPSI
#endif

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define VLS_BACKWARDS_COMPAT_TEXT N_("Compatibility with pre-0.4 VLS")
#define VLS_BACKWARDS_COMPAT_LONGTEXT N_( \
    "The protocol for transmitting A/52 audio streams changed between VLC " \
    "0.3.x and 0.4. By default VLC assumes you have the latest VLS. In case " \
    "you're using an old version, select this option.")

#define BUGGY_PSI_TEXT N_("Buggy PSI")
#define BUGGY_PSI_LONGTEXT N_( \
    "If you have a stream whose PSI packets do not feature incremented " \
    "continuity counters, select this option.")

vlc_module_begin();
#if defined MODULE_NAME_IS_ts
    set_description( _("ISO 13818-1 MPEG Transport Stream input") );
    set_capability( "demux", 160 );
    add_shortcut( "ts" );
#elif defined MODULE_NAME_IS_ts_dvbpsi
    set_description( _("ISO 13818-1 MPEG Transport Stream input (libdvbpsi)") );
    set_capability( "demux", 170 );
    add_shortcut( "ts_dvbpsi" );
#endif

    add_bool( "vls-backwards-compat", 0, NULL,
              VLS_BACKWARDS_COMPAT_TEXT, VLS_BACKWARDS_COMPAT_LONGTEXT, VLC_TRUE );
    add_bool( "buggy-psi", 0, NULL, BUGGY_PSI_TEXT, BUGGY_PSI_LONGTEXT, VLC_TRUE );
    set_callbacks( Activate, Deactivate );
vlc_module_end();

/*****************************************************************************
 * Activate: initialize TS structures
 *****************************************************************************/
static int Activate( vlc_object_t * p_this )
{
    input_thread_t *    p_input = (input_thread_t *)p_this;
    demux_sys_t *       p_demux;
    es_descriptor_t *   p_pat_es;
    es_ts_data_t *      p_demux_data;
    stream_ts_data_t *  p_stream_data;
    byte_t *            p_peek;
    vlc_bool_t          b_force = VLC_FALSE;
    int                 i_sync_pos;

    /* Set the demux function */
    p_input->pf_demux = Demux;
    p_input->pf_demux_control = demux_vaControlDefault;

    /* Have a peep at the show */
    if( input_Peek( p_input, &p_peek, TS_PACKET_SIZE ) < TS_PACKET_SIZE )
    {
        msg_Err( p_input, "cannot peek()" );
        return VLC_EGENERIC;
    }

    if( *p_input->psz_demux && ( !strncmp( p_input->psz_demux, "ts", 3 )
         || !strncmp( p_input->psz_demux, "ts_dvbpsi", 10 ) ) )
        b_force = VLC_TRUE;

    /* In a TS_PACKET_SIZE buffer we should find a sync byte */
    for( i_sync_pos = 0; i_sync_pos < TS_PACKET_SIZE &&
         p_peek[i_sync_pos] != TS_SYNC_CODE; i_sync_pos++ );

    if( i_sync_pos >= TS_PACKET_SIZE )
    {
        if( b_force )
        {
            /* User forced */
            msg_Err( p_input, "this does not look like a TS stream, "
                     "continuing anyway" );
        }
        else
        {
            msg_Warn( p_input, "TS module discarded (no sync)" );
            return VLC_EGENERIC;
        }
    }

    /* Now that we have the first TS_SYNC_CODE, check the following
     * TS_SYNC_CODEs are where they are supposed to be (one byte sync code
     * is not enough to ensure the sync). */
    if( !b_force )
    {
        if( input_Peek( p_input, &p_peek, TS_PACKET_SIZE * TS_SYNC_CODES_MIN )
            < TS_PACKET_SIZE * TS_SYNC_CODES_MIN )
        {
            msg_Err( p_input, "cannot peek()" );
            return VLC_EGENERIC;
        }

        for( ; i_sync_pos < TS_PACKET_SIZE * TS_SYNC_CODES_MIN;
             i_sync_pos += TS_PACKET_SIZE )
        {
            if( p_peek[i_sync_pos] != TS_SYNC_CODE )
            {
                msg_Warn( p_input, "TS module discarded (lost sync)" );
                return VLC_EGENERIC;
            }
        }
    }

    /* Adapt the bufsize for our only use. */
    if( p_input->i_mtu != 0 )
    {
        /* Have minimum granularity to avoid bottlenecks at the input level. */
        p_input->i_bufsize = (p_input->i_mtu / TS_PACKET_SIZE) * TS_PACKET_SIZE;
    }

    p_demux = p_input->p_demux_data = malloc( sizeof(demux_sys_t ) );
    if( p_demux == NULL )
    {
        return -1;
    }

    p_input->p_private = (void*)&p_demux->mpeg;
    p_demux->p_module = module_Need( p_input, "mpeg-system", NULL, 0 );
    if( p_demux->p_module == NULL )
    {
        free( p_input->p_demux_data );
        return -1;
    }

    vlc_mutex_lock( &p_input->stream.stream_lock );

    if( input_InitStream( p_input, sizeof( stream_ts_data_t ) ) == -1 )
    {
        module_Unneed( p_input, p_demux->p_module );
        free( p_input->p_demux_data );
        return -1;
    }

    p_stream_data = (stream_ts_data_t *)p_input->stream.p_demux_data;
    p_stream_data->i_pat_version = PAT_UNINITIALIZED ;
    p_stream_data->b_buggy_psi = config_GetInt( p_input, "buggy-psi" );

#ifdef MODULE_NAME_IS_ts_dvbpsi
    p_stream_data->p_pat_handle = (dvbpsi_handle *)
      dvbpsi_AttachPAT( (dvbpsi_pat_callback) &TS_DVBPSI_HandlePAT, p_input );

    if( p_stream_data->p_pat_handle == NULL )
    {
        msg_Err( p_input, "could not create PAT decoder" );
        module_Unneed( p_input, p_demux->p_module );
        free( p_input->p_demux_data );
        return -1;
    }
#endif

    /* We'll have to catch the PAT in order to continue
     * Then the input will catch the PMT and then the others ES
     * The PAT es is indepedent of any program. */
    p_pat_es = input_AddES( p_input, NULL, 0x00,
                            UNKNOWN_ES, NULL, sizeof( es_ts_data_t ) );
    p_pat_es->i_fourcc = VLC_FOURCC( 'p', 'a', 't', ' ' );
    p_demux_data = (es_ts_data_t *)p_pat_es->p_demux_data;
    p_demux_data->b_psi = 1;
    p_demux_data->i_psi_type = PSI_IS_PAT;
    p_demux_data->p_psi_section = malloc(sizeof(psi_section_t));
    p_demux_data->p_psi_section->b_is_complete = 1;
    p_demux_data->i_continuity_counter = 0xFF;

    vlc_mutex_unlock( &p_input->stream.stream_lock );

    return 0;
}

/*****************************************************************************
 * Deactivate: deinitialize TS structures
 *****************************************************************************/
static void Deactivate( vlc_object_t * p_this )
{
    input_thread_t *    p_input = (input_thread_t *)p_this;

    module_Unneed( p_input, p_input->p_demux_data->p_module );
    free( p_input->p_demux_data );
}

/*****************************************************************************
 * Demux: reads and demuxes data packets
 *****************************************************************************
 * Returns -1 in case of error, 0 in case of EOF, otherwise the number of
 * packets.
 *****************************************************************************/
static int Demux( input_thread_t * p_input )
{
    demux_sys_t *   p_demux = p_input->p_demux_data;
    int             i_read_once = (p_input->i_mtu ?
                                   p_input->i_bufsize / TS_PACKET_SIZE :
                                   TS_READ_ONCE);
    int             i;

    for( i = 0; i < i_read_once; i++ )
    {
        data_packet_t *     p_data;
        ssize_t             i_result;

        i_result = p_demux->mpeg.pf_read_ts( p_input, &p_data );

        if( i_result <= 0 )
        {
            return i_result;
        }

        p_demux->mpeg.pf_demux_ts( p_input, p_data,
                                   (psi_callback_t) &PSI_CALLBACK );
    }

    return i_read_once;
}


#if defined MODULE_NAME_IS_ts
/*
 * PSI demultiplexing and decoding without libdvbpsi
 */

/*****************************************************************************
 * DemuxPSI : makes up complete PSI data
 *****************************************************************************/
static void TSDemuxPSI( input_thread_t * p_input, data_packet_t * p_data,
        es_descriptor_t * p_es, vlc_bool_t b_unit_start )
{
    es_ts_data_t  * p_demux_data;

    p_demux_data = (es_ts_data_t *)p_es->p_demux_data;

#define p_psi (p_demux_data->p_psi_section)
#define p (p_data->p_payload_start)

    if( b_unit_start )
    {
        /* unit_start set to 1 -> presence of a pointer field
         * (see ISO/IEC 13818 (2.4.4.2) which should be set to 0x00 */
        if( (uint8_t)p[0] != 0x00 )
        {
            msg_Warn( p_input,
                      "non-zero pointer field found, trying to continue" );
            p+=(uint8_t)p[0];
        }
        else
        {
            p++;
        }

        /* This is the begining of a new section */

        if( ((uint8_t)(p[1]) & 0xc0) != 0x80 )
        {
            msg_Warn( p_input, "invalid PSI packet" );
            p_psi->b_trash = 1;
        }
        else
        {
            p_psi->i_section_length = ((p[1] & 0xF) << 8) | p[2];
            p_psi->b_section_complete = 0;
            p_psi->i_read_in_section = 0;
            p_psi->i_section_number = (uint8_t)p[6];

            if( p_psi->b_is_complete || p_psi->i_section_number == 0 )
            {
                /* This is a new PSI packet */
                p_psi->b_is_complete = 0;
                p_psi->b_trash = 0;
                p_psi->i_version_number = ( p[5] >> 1 ) & 0x1f;
                p_psi->i_last_section_number = (uint8_t)p[7];

                /* We'll write at the begining of the buffer */
                p_psi->p_current = p_psi->buffer;
            }
            else
            {
                if( p_psi->b_section_complete )
                {
                    /* New Section of an already started PSI */
                    p_psi->b_section_complete = 0;

                    if( p_psi->i_version_number != (( p[5] >> 1 ) & 0x1f) )
                    {
                        msg_Warn( p_input,
                                  "PSI version differs inside same PAT" );
                        p_psi->b_trash = 1;
                    }
                    if( p_psi->i_section_number + 1 != (uint8_t)p[6] )
                    {
                        msg_Warn( p_input,
                                  "PSI Section discontinuity, packet lost?" );
                        p_psi->b_trash = 1;
                    }
                    else
                        p_psi->i_section_number++;
                }
                else
                {
                    msg_Warn( p_input, "got unexpected new PSI section" );
                    p_psi->b_trash = 1;
                }
            }
        }
    } /* b_unit_start */

    if( !p_psi->b_trash )
    {
        /* read */
        if( (p_data->p_payload_end - p) >=
            ( p_psi->i_section_length - p_psi->i_read_in_section ) )
        {
            /* The end of the section is in this TS packet */
            memcpy( p_psi->p_current, p,
            (p_psi->i_section_length - p_psi->i_read_in_section) );

            p_psi->b_section_complete = 1;
            p_psi->p_current +=
                (p_psi->i_section_length - p_psi->i_read_in_section);

            if( p_psi->i_section_number == p_psi->i_last_section_number )
            {
                /* This was the last section of PSI */
                p_psi->b_is_complete = 1;

                switch( p_demux_data->i_psi_type)
                {
                case PSI_IS_PAT:
                    TSDecodePAT( p_input, p_es );
                    break;
                case PSI_IS_PMT:
                    TSDecodePMT( p_input, p_es );
                    break;
                default:
                    msg_Warn( p_input, "received unknown PSI in DemuxPSI" );
                }
            }
        }
        else
        {
            memcpy( p_psi->buffer, p, p_data->p_payload_end - p );
            p_psi->i_read_in_section += p_data->p_payload_end - p;

            p_psi->p_current += p_data->p_payload_end - p;
        }
    }

#undef p_psi
#undef p

    input_DeletePacket( p_input->p_method_data, p_data );

    return ;
}

/*****************************************************************************
 * DecodePAT : Decodes Programm association table and deal with it
 *****************************************************************************/
static void TSDecodePAT( input_thread_t * p_input, es_descriptor_t * p_es )
{
    stream_ts_data_t  * p_stream_data;
    es_ts_data_t      * p_demux_data;

    pgrm_descriptor_t * p_pgrm;
    es_descriptor_t   * p_current_es;
    byte_t            * p_current_data;

    int                 i_section_length, i_program_id, i_pmt_pid;
    int                 i_loop, i_current_section;

    vlc_bool_t          b_changed = 0;

    p_demux_data = (es_ts_data_t *)p_es->p_demux_data;
    p_stream_data = (stream_ts_data_t *)p_input->stream.p_demux_data;

#define p_psi (p_demux_data->p_psi_section)

    /* Not so fast, Mike ! If the PAT version has changed, we first check
     * that its content has really changed before doing anything */
    if( p_stream_data->i_pat_version != p_psi->i_version_number )
    {
        int i_programs = p_input->stream.i_pgrm_number;

        p_current_data = p_psi->buffer;

        do
        {
            i_section_length = ((uint32_t)(p_current_data[1] & 0xF) << 8) |
                                 p_current_data[2];
            i_current_section = (uint8_t)p_current_data[6];

            for( i_loop = 0;
                 ( i_loop < (i_section_length - 9) / 4 ) && !b_changed;
                 i_loop++ )
            {
                i_program_id = ( (uint32_t)*(p_current_data + i_loop * 4 + 8) << 8 )
                                 | *(p_current_data + i_loop * 4 + 9);
                i_pmt_pid = ( ((uint32_t)*(p_current_data + i_loop * 4 + 10) & 0x1F)
                                    << 8 )
                               | *(p_current_data + i_loop * 4 + 11);

                if( i_program_id )
                {
                    if( (p_pgrm = input_FindProgram( p_input, i_program_id ))
                        && (p_current_es = input_FindES( p_input, i_pmt_pid ))
                        && p_current_es->p_pgrm == p_pgrm
                        && p_current_es->i_id == i_pmt_pid
                        && ((es_ts_data_t *)p_current_es->p_demux_data)->b_psi
                        && ((es_ts_data_t *)p_current_es->p_demux_data)
                            ->i_psi_type == PSI_IS_PMT )
                    {
                        i_programs--;
                    }
                    else
                    {
                        b_changed = 1;
                    }
                }
            }

            p_current_data += 3 + i_section_length;

        } while( ( i_current_section < p_psi->i_last_section_number )
                  && !b_changed );

        /* If we didn't find the expected amount of programs, the PAT has
         * changed. Otherwise, it only changed if b_changed is already != 0 */
        b_changed = b_changed || i_programs;
    }

    if( b_changed )
    {
        /* PAT has changed. We are going to delete all programs and
         * create new ones. We chose not to only change what was needed
         * as a PAT change may mean the stream is radically changing and
         * this is a secure method to avoid crashes */
        es_ts_data_t      * p_es_demux;
        pgrm_ts_data_t    * p_pgrm_demux;

        p_current_data = p_psi->buffer;

        /* Delete all programs */
        while( p_input->stream.i_pgrm_number )
        {
            input_DelProgram( p_input, p_input->stream.pp_programs[0] );
        }

        do
        {
            i_section_length = ((uint32_t)(p_current_data[1] & 0xF) << 8) |
                                 p_current_data[2];
            i_current_section = (uint8_t)p_current_data[6];

            for( i_loop = 0; i_loop < (i_section_length - 9) / 4 ; i_loop++ )
            {
                i_program_id = ( (uint32_t)*(p_current_data + i_loop * 4 + 8) << 8 )
                                 | *(p_current_data + i_loop * 4 + 9);
                i_pmt_pid = ( ((uint32_t)*(p_current_data + i_loop * 4 + 10) & 0x1F)
                                    << 8 )
                               | *(p_current_data + i_loop * 4 + 11);

                /* If program = 0, we're having info about NIT not PMT */
                if( i_program_id )
                {
                    /* Add this program */
                    p_pgrm = input_AddProgram( p_input, i_program_id,
                                               sizeof( pgrm_ts_data_t ) );

                    /* whatis the PID of the PMT of this program */
                    p_pgrm_demux = (pgrm_ts_data_t *)p_pgrm->p_demux_data;
                    p_pgrm_demux->i_pmt_version = PMT_UNINITIALIZED;

                    /* Add the PMT ES to this program */
                    p_current_es = input_AddES( p_input, p_pgrm,(uint16_t)i_pmt_pid,
                        UNKNOWN_ES, NULL, sizeof( es_ts_data_t) );
                    p_current_es->i_fourcc = VLC_FOURCC( 'p', 'm', 't', ' ' );
                    p_es_demux = (es_ts_data_t *)p_current_es->p_demux_data;
                    p_es_demux->b_psi = 1;
                    p_es_demux->i_psi_type = PSI_IS_PMT;

                    p_es_demux->p_psi_section =
                                            malloc( sizeof( psi_section_t ) );
                    p_es_demux->p_psi_section->b_is_complete = 0;
                    p_es_demux->i_continuity_counter = 0xFF;
                }
            }

            p_current_data += 3 + i_section_length;

        } while( i_current_section < p_psi->i_last_section_number );

        /* Go to the beginning of the next section */
        p_stream_data->i_pat_version = p_psi->i_version_number;

    }
#undef p_psi

}

/*****************************************************************************
 * DecodePMT : decode a given Program Stream Map
 * ***************************************************************************
 * When the PMT changes, it may mean a deep change in the stream, and it is
 * careful to delete the ES and add them again. If the PMT doesn't change,
 * there no need to do anything.
 *****************************************************************************/
static void TSDecodePMT( input_thread_t * p_input, es_descriptor_t * p_es )
{

    pgrm_ts_data_t            * p_pgrm_data;
    es_ts_data_t              * p_demux_data;
    vlc_bool_t b_vls_compat = config_GetInt( p_input, "vls-backwards-compat" );

    p_demux_data = (es_ts_data_t *)p_es->p_demux_data;
    p_pgrm_data = (pgrm_ts_data_t *)p_es->p_pgrm->p_demux_data;

#define p_psi (p_demux_data->p_psi_section)

    if( p_psi->i_version_number != p_pgrm_data->i_pmt_version )
    {
        es_descriptor_t   * p_new_es;
        es_ts_data_t      * p_es_demux;
        byte_t            * p_current_data, * p_current_section;
        int                 i_section_length,i_current_section;
        int                 i_prog_info_length, i_loop;
        int                 i_es_info_length, i_pid, i_stream_type;

        p_current_section = p_psi->buffer;
        p_current_data = p_psi->buffer;

        p_pgrm_data->i_pcr_pid = ( ((uint32_t)*(p_current_section + 8) & 0x1F) << 8 ) |
                                    *(p_current_section + 9);


        /* Lock stream information */
        vlc_mutex_lock( &p_input->stream.stream_lock );

        /* Delete all ES in this program  except the PSI. We start from the
         * end because i_es_number gets decremented after each deletion. */
        for( i_loop = p_es->p_pgrm->i_es_number ; i_loop ; )
        {
            i_loop--;
            p_es_demux = (es_ts_data_t *)
                         p_es->p_pgrm->pp_es[i_loop]->p_demux_data;
            if ( ! p_es_demux->b_psi )
            {
                input_DelES( p_input, p_es->p_pgrm->pp_es[i_loop] );
            }
        }

        /* Then add what we received in this PMT */
        do
        {
            i_section_length = ( ((uint32_t)*(p_current_data + 1) & 0xF) << 8 ) |
                                  *(p_current_data + 2);
            i_current_section = (uint8_t)p_current_data[6];
            i_prog_info_length = ( ((uint32_t)*(p_current_data + 10) & 0xF) << 8 ) |
                                    *(p_current_data + 11);

            /* For the moment we ignore program descriptors */
            p_current_data += 12 + i_prog_info_length;

            /* The end of the section, before the CRC is at
             * p_current_section + i_section_length -1 */
            while( p_current_data < p_current_section + i_section_length -1 )
            {
                i_stream_type = (int)p_current_data[0];
                i_pid = ( ((uint32_t)*(p_current_data + 1) & 0x1F) << 8 ) |
                           *(p_current_data + 2);
                i_es_info_length = ( ((uint32_t)*(p_current_data + 3) & 0xF) << 8 ) |
                                      *(p_current_data + 4);

                /* Tell the interface what kind of stream it is and select
                 * the required ones */
                {
                    int i_fourcc, i_cat, i_stream_id;

                    switch( i_stream_type )
                    {
                        case MPEG1_VIDEO_ES:
                        case MPEG2_VIDEO_ES:
                        case MPEG2_MOTO_VIDEO_ES:
                            /* This isn't real, but we don't actually use
                             * it. */
                            i_stream_id = 0xE0;
                            i_fourcc = VLC_FOURCC('m','p','g','v');
                            i_cat = VIDEO_ES;
                            break;
                        case MPEG1_AUDIO_ES:
                        case MPEG2_AUDIO_ES:
                            /* This isn't real, but we don't actually use
                             * it. */
                            i_stream_id = 0xC0;
                            i_fourcc = VLC_FOURCC('m','p','g','a');
                            i_cat = AUDIO_ES;
                            break;
                        case A52_AUDIO_ES:
                            if ( !b_vls_compat )
                                i_fourcc = VLC_FOURCC('a','5','2',' ');
                            else
                                i_fourcc = VLC_FOURCC('a','5','2','b');
                            i_stream_id = 0xBD;
                            i_cat = AUDIO_ES;
                            break;
                        case LPCM_AUDIO_ES:
                            i_fourcc = VLC_FOURCC('l','p','c','m');
                            i_stream_id = 0xBD;
                            i_cat = AUDIO_ES;
                            break;
                        case DVD_SPU_ES:
                            if ( !b_vls_compat )
                                i_fourcc = VLC_FOURCC('s','p','u',' ');
                            else
                                i_fourcc = VLC_FOURCC('s','p','u','b');
                            i_stream_id = 0xBD;
                            i_cat = SPU_ES;
                            break;
                        case SDDS_AUDIO_ES:
                            i_fourcc = VLC_FOURCC('s','d','d','s');
                            i_stream_id = 0xBD;
                            i_cat = AUDIO_ES;
                            break;
                        case DTS_AUDIO_ES:
                            i_fourcc = VLC_FOURCC('d','t','s',' ');
                            i_stream_id = 0xBD;
                            i_cat = AUDIO_ES;
                            break;
                        /* 'b' stands for 'buggy' */
                        case A52B_AUDIO_ES:
                            i_fourcc = VLC_FOURCC('a','5','2','b');
                            i_stream_id = 0xBD;
                            i_cat = AUDIO_ES;
                            break;
                        case LPCMB_AUDIO_ES:
                            i_fourcc = VLC_FOURCC('l','p','c','b');
                            i_stream_id = 0xBD;
                            i_cat = AUDIO_ES;
                            break;
                        case DVDB_SPU_ES:
                            i_fourcc = VLC_FOURCC('s','p','u','b');
                            i_stream_id = 0xBD;
                            i_cat = SPU_ES;
                            break;
                        case AAC_ADTS_AUDIO_ES:
                            i_fourcc = VLC_FOURCC('m','p','4','a');
                            i_cat = AUDIO_ES;
                            i_stream_id = 0xfa;
                            break;

                        default :
                            i_stream_id = 0;
                            i_fourcc = 0;
                            i_cat = UNKNOWN_ES;
                            break;
                    }

                    /* Add this ES to the program */
                    p_new_es = input_AddES( p_input, p_es->p_pgrm, (uint16_t)i_pid,
                                   i_cat, NULL, sizeof( es_ts_data_t ) );

                    ((es_ts_data_t *)p_new_es->p_demux_data)->i_continuity_counter = 0xFF;

                    p_new_es->i_stream_id = i_stream_id;
                    p_new_es->i_fourcc = i_fourcc;

                }

                p_current_data += 5 + i_es_info_length;
            }

            /* Go to the beginning of the next section*/
            p_current_data += 3 + i_section_length;

            p_current_section++;

        } while( i_current_section < p_psi->i_last_section_number );

        p_pgrm_data->i_pmt_version = p_psi->i_version_number;

        /* if no program is selected :*/
        if( !p_input->stream.p_selected_program )
        {
            pgrm_descriptor_t *     p_pgrm_to_select;
            uint16_t i_id = (uint16_t)config_GetInt( p_input, "program" );

            if( i_id != 0 ) /* if user specified a program */
            {
                p_pgrm_to_select = input_FindProgram( p_input, i_id );

                if( p_pgrm_to_select && p_pgrm_to_select == p_es->p_pgrm )
                    p_input->pf_set_program( p_input, p_pgrm_to_select );
            }
            else
                    p_input->pf_set_program( p_input, p_es->p_pgrm );
        }

        /* if the pmt belongs to the currently selected program, we
         * reselect it to update its ES */
        else if( p_es->p_pgrm == p_input->stream.p_selected_program )
        {
            p_input->pf_set_program( p_input, p_es->p_pgrm );
        }

        /* inform interface that stream has changed */
        p_input->stream.b_changed = 1;
        /*  Remove lock */
        vlc_mutex_unlock( &p_input->stream.stream_lock );
    }

#undef p_psi
}

#elif defined MODULE_NAME_IS_ts_dvbpsi
/*
 * PSI Decoding using libdvbpsi
 */

/*****************************************************************************
 * DemuxPSI : send the PSI to the right libdvbpsi decoder
 *****************************************************************************/
static void TS_DVBPSI_DemuxPSI( input_thread_t  * p_input,
                                data_packet_t   * p_data,
                                es_descriptor_t * p_es,
                                vlc_bool_t        b_unit_start )
{
    es_ts_data_t        * p_es_demux_data;
    pgrm_ts_data_t      * p_pgrm_demux_data;
    stream_ts_data_t    * p_stream_demux_data;

    p_es_demux_data = (es_ts_data_t *)p_es->p_demux_data;
    p_stream_demux_data = (stream_ts_data_t *) p_input->stream.p_demux_data;

    switch( p_es_demux_data->i_psi_type)
    {
        case PSI_IS_PAT:
            dvbpsi_PushPacket(
                    (dvbpsi_handle)p_stream_demux_data->p_pat_handle,
                    p_data->p_demux_start );
            break;
        case PSI_IS_PMT:
            p_pgrm_demux_data = ( pgrm_ts_data_t * )p_es->p_pgrm->p_demux_data;
            dvbpsi_PushPacket(
                    (dvbpsi_handle)p_pgrm_demux_data->p_pmt_handle,
                    p_data->p_demux_start );
            break;
        default:
            msg_Warn( p_input, "received unknown PSI in DemuxPSI" );
    }

    input_DeletePacket( p_input->p_method_data, p_data );
}
/*****************************************************************************
 * MP4 specific functions
 *****************************************************************************/
static int  MP4_DescriptorLength( int *pi_data, uint8_t **pp_data )
{
    unsigned int i_b;
    unsigned int i_len = 0;
    do
    {
        i_b = **pp_data;
        (*pp_data)++;
        (*pi_data)--;
        i_len = ( i_len << 7 ) + ( i_b&0x7f );

    } while( i_b&0x80 );

    return( i_len );
}
static int MP4_GetByte( int *pi_data, uint8_t **pp_data )
{
    if( *pi_data > 0 )
    {
        int i_b = **pp_data;
        (*pp_data)++;
        (*pi_data)--;
        return( i_b );
    }
    else
    {
        return( 0 );
    }
}

static int MP4_GetWord( int *pi_data, uint8_t **pp_data )
{
    int i1, i2;
    i1 = MP4_GetByte( pi_data, pp_data );
    i2 = MP4_GetByte( pi_data, pp_data );
    return( ( i1 << 8 ) | i2 );
}
static int MP4_Get3Bytes( int *pi_data, uint8_t **pp_data )
{
    int i1, i2, i3;
    i1 = MP4_GetByte( pi_data, pp_data );
    i2 = MP4_GetByte( pi_data, pp_data );
    i3 = MP4_GetByte( pi_data, pp_data );
    return( ( i1 << 16 ) | ( i2 << 8) | i3 );
}

static uint32_t MP4_GetDWord( int *pi_data, uint8_t **pp_data )
{
    uint32_t i1, i2;
    i1 = MP4_GetWord( pi_data, pp_data );
    i2 = MP4_GetWord( pi_data, pp_data );
    return( ( i1 << 16 ) | i2 );
}

static char* MP4_GetURL( int *pi_data, uint8_t **pp_data )
{
    char *url;
    int i_url_len, i;

    i_url_len = MP4_GetByte( pi_data, pp_data );
    url = malloc( i_url_len + 1 );
    for( i = 0; i < i_url_len; i++ )
    {
        url[i] = MP4_GetByte( pi_data, pp_data );
    }
    url[i_url_len] = '\0';
    return( url );
}

static void MP4_IODParse( iod_descriptor_t *p_iod, int i_data, uint8_t *p_data )
{
    int i;
    int i_es_index;
    uint8_t     i_flags;
    vlc_bool_t  b_url;
    int         i_iod_length;

    fprintf( stderr, "\n************ IOD ************" );
    for( i = 0; i < 255; i++ )
    {
        p_iod->es_descr[i].b_ok = 0;
    }
    i_es_index = 0;

    if( i_data < 3 )
    {
        return;
    }

    p_iod->i_iod_label = MP4_GetByte( &i_data, &p_data );
    fprintf( stderr, "\n* iod_label:%d", p_iod->i_iod_label );
    fprintf( stderr, "\n* ===========" );
    fprintf( stderr, "\n* tag:0x%x", p_data[0] );

    if( MP4_GetByte( &i_data, &p_data ) != 0x02 )
    {
        fprintf( stderr, "\n ERR: tag != 0x02" );
        return;
    }

    i_iod_length = MP4_DescriptorLength( &i_data, &p_data );
    fprintf( stderr, "\n* length:%d", i_iod_length );
    if( i_iod_length > i_data )
    {
        i_iod_length = i_data;
    }

    p_iod->i_od_id = ( MP4_GetByte( &i_data, &p_data ) << 2 );
    i_flags = MP4_GetByte( &i_data, &p_data );
    p_iod->i_od_id |= i_flags >> 6;
    b_url = ( i_flags >> 5  )&0x01;

    fprintf( stderr, "\n* od_id:%d", p_iod->i_od_id );
    fprintf( stderr, "\n* url flag:%d", b_url );
    fprintf( stderr, "\n* includeInlineProfileLevel flag:%d", ( i_flags >> 4 )&0x01 );

    if( b_url )
    {
        p_iod->psz_url = MP4_GetURL( &i_data, &p_data );
        fprintf( stderr, "\n* url string:%s", p_iod->psz_url );
        fprintf( stderr, "\n*****************************\n" );
        return;
    }
    else
    {
        p_iod->psz_url = NULL;
    }

    p_iod->i_ODProfileLevelIndication = MP4_GetByte( &i_data, &p_data );
    p_iod->i_sceneProfileLevelIndication = MP4_GetByte( &i_data, &p_data );
    p_iod->i_audioProfileLevelIndication = MP4_GetByte( &i_data, &p_data );
    p_iod->i_visualProfileLevelIndication = MP4_GetByte( &i_data, &p_data );
    p_iod->i_graphicsProfileLevelIndication = MP4_GetByte( &i_data, &p_data );

    fprintf( stderr, "\n* ODProfileLevelIndication:%d", p_iod->i_ODProfileLevelIndication );
    fprintf( stderr, "\n* sceneProfileLevelIndication:%d", p_iod->i_sceneProfileLevelIndication );
    fprintf( stderr, "\n* audioProfileLevelIndication:%d", p_iod->i_audioProfileLevelIndication );
    fprintf( stderr, "\n* visualProfileLevelIndication:%d", p_iod->i_visualProfileLevelIndication );
    fprintf( stderr, "\n* graphicsProfileLevelIndication:%d", p_iod->i_graphicsProfileLevelIndication );


    while( i_data > 0 && i_es_index < 255)
    {
        int i_tag, i_length;
        int     i_data_sav;
        uint8_t *p_data_sav;

        i_tag = MP4_GetByte( &i_data, &p_data );
        i_length = MP4_DescriptorLength( &i_data, &p_data );

        i_data_sav = i_data;
        p_data_sav = p_data;

        i_data = i_length;

        switch( i_tag )
        {
            case 0x03:
                {
#define es_descr    p_iod->es_descr[i_es_index]
                    int i_decoderConfigDescr_length;
                    fprintf( stderr, "\n* - ES_Descriptor length:%d", i_length );
                    es_descr.b_ok = 1;

                    es_descr.i_es_id = MP4_GetWord( &i_data, &p_data );
                    i_flags = MP4_GetByte( &i_data, &p_data );
                    es_descr.b_streamDependenceFlag = ( i_flags >> 7 )&0x01;
                    b_url = ( i_flags >> 6 )&0x01;
                    es_descr.b_OCRStreamFlag = ( i_flags >> 5 )&0x01;
                    es_descr.i_streamPriority = i_flags & 0x1f;
                    fprintf( stderr, "\n*   * streamDependenceFlag:%d", es_descr.b_streamDependenceFlag );
                    fprintf( stderr, "\n*   * OCRStreamFlag:%d", es_descr.b_OCRStreamFlag );
                    fprintf( stderr, "\n*   * streamPriority:%d", es_descr.i_streamPriority );

                    if( es_descr.b_streamDependenceFlag )
                    {
                        es_descr.i_dependOn_es_id = MP4_GetWord( &i_data, &p_data );
                        fprintf( stderr, "\n*   * dependOn_es_id:%d", es_descr.i_dependOn_es_id );
                    }

                    if( b_url )
                    {
                        es_descr.psz_url = MP4_GetURL( &i_data, &p_data );
                        fprintf( stderr, "\n* url string:%s", es_descr.psz_url );
                    }
                    else
                    {
                        es_descr.psz_url = NULL;
                    }

                    if( es_descr.b_OCRStreamFlag )
                    {
                        es_descr.i_OCR_es_id = MP4_GetWord( &i_data, &p_data );
                        fprintf( stderr, "\n*   * OCR_es_id:%d", es_descr.i_OCR_es_id );
                    }

                    if( MP4_GetByte( &i_data, &p_data ) != 0x04 )
                    {
                        fprintf( stderr, "\n* ERR missing DecoderConfigDescr" );
                        es_descr.b_ok = 0;
                        break;
                    }
                    i_decoderConfigDescr_length = MP4_DescriptorLength( &i_data, &p_data );

                    fprintf( stderr, "\n*   - DecoderConfigDesc length:%d", i_decoderConfigDescr_length );
#define dec_descr   es_descr.dec_descr
                    dec_descr.i_objectTypeIndication = MP4_GetByte( &i_data, &p_data );
                    i_flags = MP4_GetByte( &i_data, &p_data );
                    dec_descr.i_streamType = i_flags >> 2;
                    dec_descr.b_upStream = ( i_flags >> 1 )&0x01;
                    dec_descr.i_bufferSizeDB = MP4_Get3Bytes( &i_data, &p_data );
                    dec_descr.i_maxBitrate = MP4_GetDWord( &i_data, &p_data );
                    dec_descr.i_avgBitrate = MP4_GetDWord( &i_data, &p_data );
                    fprintf( stderr, "\n*     * objectTypeIndication:0x%x", dec_descr.i_objectTypeIndication  );
                    fprintf( stderr, "\n*     * streamType:0x%x", dec_descr.i_streamType );
                    fprintf( stderr, "\n*     * upStream:%d", dec_descr.b_upStream );
                    fprintf( stderr, "\n*     * bufferSizeDB:%d", dec_descr.i_bufferSizeDB );
                    fprintf( stderr, "\n*     * maxBitrate:%d", dec_descr.i_maxBitrate );
                    fprintf( stderr, "\n*     * avgBitrate:%d", dec_descr.i_avgBitrate );
                    if( i_decoderConfigDescr_length > 13 && MP4_GetByte( &i_data, &p_data ) == 0x05 )
                    {
                        int i;
                        dec_descr.i_decoder_specific_info_len =
                            MP4_DescriptorLength( &i_data, &p_data );
                        if( dec_descr.i_decoder_specific_info_len > 0 )
                        {
                            dec_descr.p_decoder_specific_info =
                                malloc( dec_descr.i_decoder_specific_info_len );
                        }
                        for( i = 0; i < dec_descr.i_decoder_specific_info_len; i++ )
                        {
                            dec_descr.p_decoder_specific_info[i] = MP4_GetByte( &i_data, &p_data );
                        }
                    }
                    else
                    {
                        dec_descr.i_decoder_specific_info_len = 0;
                        dec_descr.p_decoder_specific_info = NULL;
                    }
                }
#undef  dec_descr
#define sl_descr    es_descr.sl_descr
                {
                    int i_SLConfigDescr_length;
                    int i_predefined;

                    if( MP4_GetByte( &i_data, &p_data ) != 0x06 )
                    {
                        fprintf( stderr, "\n* ERR missing SLConfigDescr" );
                        es_descr.b_ok = 0;
                        break;
                    }
                    i_SLConfigDescr_length = MP4_DescriptorLength( &i_data, &p_data );

                    fprintf( stderr, "\n*   - SLConfigDescr length:%d", i_SLConfigDescr_length );
                    i_predefined = MP4_GetByte( &i_data, &p_data );
                    fprintf( stderr, "\n*     * i_predefined:0x%x", i_predefined  );
                    switch( i_predefined )
                    {
                        case 0x01:
                            {
                                sl_descr.b_useAccessUnitStartFlag   = 0;
                                sl_descr.b_useAccessUnitEndFlag     = 0;
                                sl_descr.b_useRandomAccessPointFlag = 0;
                                //sl_descr.b_useRandomAccessUnitsOnlyFlag = 0;
                                sl_descr.b_usePaddingFlag           = 0;
                                sl_descr.b_useTimeStampsFlags       = 0;
                                sl_descr.b_useIdleFlag              = 0;
                                sl_descr.b_durationFlag     = 0;    // FIXME FIXME
                                sl_descr.i_timeStampResolution      = 1000;
                                sl_descr.i_OCRResolution    = 0;    // FIXME FIXME
                                sl_descr.i_timeStampLength          = 32;
                                sl_descr.i_OCRLength        = 0;    // FIXME FIXME
                                sl_descr.i_AU_Length                = 0;
                                sl_descr.i_instantBitrateLength= 0; // FIXME FIXME
                                sl_descr.i_degradationPriorityLength= 0;
                                sl_descr.i_AU_seqNumLength          = 0;
                                sl_descr.i_packetSeqNumLength       = 0;
                                if( sl_descr.b_durationFlag )
                                {
                                    sl_descr.i_timeScale            = 0;    // FIXME FIXME
                                    sl_descr.i_accessUnitDuration   = 0;    // FIXME FIXME
                                    sl_descr.i_compositionUnitDuration= 0;    // FIXME FIXME
                                }
                                if( !sl_descr.b_useTimeStampsFlags )
                                {
                                    sl_descr.i_startDecodingTimeStamp   = 0;    // FIXME FIXME
                                    sl_descr.i_startCompositionTimeStamp= 0;    // FIXME FIXME
                                }
                            }
                            break;
                        default:
                            fprintf( stderr, "\n* ERR unsupported SLConfigDescr predefined" );
                            es_descr.b_ok = 0;
                            break;
                    }
                }
                break;
#undef  sl_descr
#undef  es_descr
            default:
                fprintf( stderr, "\n* - OD tag:0x%x length:%d (Unsupported)", i_tag, i_length );
                break;
        }

        p_data = p_data_sav + i_length;
        i_data = i_data_sav - i_length;
        i_es_index++;
    }


    fprintf( stderr, "\n*****************************\n" );
}

static void MP4_IODClean( iod_descriptor_t *p_iod )
{
    int i;

    if( p_iod->psz_url )
    {
        free( p_iod->psz_url );
        p_iod->psz_url = NULL;
        return;
    }

    for( i = 0; i < 255; i++ )
    {
#define es_descr p_iod->es_descr[i]
        if( es_descr.b_ok )
        {
            if( es_descr.psz_url )
            {
                free( es_descr.psz_url );
                es_descr.psz_url = NULL;
            }
            else
            {
                if( es_descr.dec_descr.p_decoder_specific_info != NULL )
                {
                    free( es_descr.dec_descr.p_decoder_specific_info );
                    es_descr.dec_descr.p_decoder_specific_info = NULL;
                    es_descr.dec_descr.i_decoder_specific_info_len = 0;
                }
            }
        }
        es_descr.b_ok = 0;
#undef  es_descr
    }
}

/*****************************************************************************
 * HandlePAT: will treat a PAT returned by dvbpsi
 *****************************************************************************/
static void TS_DVBPSI_HandlePAT( input_thread_t * p_input,
                                 dvbpsi_pat_t * p_new_pat )
{
    dvbpsi_pat_program_t *      p_pgrm;
    pgrm_descriptor_t *         p_new_pgrm;
    pgrm_ts_data_t *            p_pgrm_demux;
    es_descriptor_t *           p_current_es;
    es_ts_data_t *              p_es_demux;
    stream_ts_data_t *          p_stream_data;

    vlc_mutex_lock( &p_input->stream.stream_lock );

    p_stream_data = (stream_ts_data_t *)p_input->stream.p_demux_data;

    if( ( p_new_pat->b_current_next &&
          ( p_new_pat->i_version != p_stream_data->i_pat_version ) ) ||
        p_stream_data->i_pat_version == PAT_UNINITIALIZED  )
    {
        msg_Dbg( p_input, "processing PAT version %d", p_new_pat->i_version );

        /* Delete all programs */
        while( p_input->stream.i_pgrm_number )
        {
            pgrm_ts_data_t *p_pgrm_demux_old =
                (pgrm_ts_data_t *)p_input->stream.pp_programs[0]->p_demux_data;

            if( p_pgrm_demux_old->b_mpeg4 )
            {
                MP4_IODClean( &p_pgrm_demux_old->iod );
            }

            /* Delete old PMT decoder */
            if( p_pgrm_demux_old->p_pmt_handle )
                dvbpsi_DetachPMT( p_pgrm_demux_old->p_pmt_handle );

            input_DelProgram( p_input, p_input->stream.pp_programs[0] );
        }

        /* treat the new programs list */
        p_pgrm = p_new_pat->p_first_program;

        while( p_pgrm )
        {
            msg_Dbg( p_input, "new program: %d", p_pgrm->i_number );

            /* If program = 0, we're having info about NIT not PMT */
            if( p_pgrm->i_number )
            {
                /* Add this program */
                p_new_pgrm = input_AddProgram( p_input, p_pgrm->i_number,
                                            sizeof( pgrm_ts_data_t ) );

                p_pgrm_demux = (pgrm_ts_data_t *)p_new_pgrm->p_demux_data;
                p_pgrm_demux->i_pmt_version = PMT_UNINITIALIZED;

                /* Add the PMT ES to this program */
                p_current_es = input_AddES( p_input, p_new_pgrm,
                                            (uint16_t)p_pgrm->i_pid, UNKNOWN_ES,
                                            NULL, sizeof(es_ts_data_t) );
                p_current_es->i_fourcc = VLC_FOURCC( 'p', 'm', 't', ' ' );
                p_es_demux = (es_ts_data_t *)p_current_es->p_demux_data;
                p_es_demux->b_psi = 1;
                p_es_demux->i_psi_type = PSI_IS_PMT;
                p_es_demux->p_psi_section = NULL;
                p_es_demux->i_continuity_counter = 0xFF;

                /* Create a PMT decoder */
                p_pgrm_demux->p_pmt_handle = (dvbpsi_handle *)
                    dvbpsi_AttachPMT( p_pgrm->i_number,
                            (dvbpsi_pmt_callback) &TS_DVBPSI_HandlePMT,
                            p_input );

                if( p_pgrm_demux->p_pmt_handle == NULL )
                {
                    msg_Err( p_input, "could not create PMT decoder" );
                    p_input->b_error = 1;
                    return;
                }

            }
            p_pgrm = p_pgrm->p_next;
        }

        p_stream_data->i_pat_version = p_new_pat->i_version;
    }
    vlc_mutex_unlock( &p_input->stream.stream_lock );
}


/*****************************************************************************
 * HandlePMT: will treat a PMT returned by dvbpsi
 *****************************************************************************/
static void TS_DVBPSI_HandlePMT( input_thread_t * p_input,
                                 dvbpsi_pmt_t * p_new_pmt )
{
    dvbpsi_pmt_es_t *       p_es;
    pgrm_descriptor_t *     p_pgrm;
    es_descriptor_t *       p_new_es;
    pgrm_ts_data_t *        p_pgrm_demux;
    vlc_bool_t b_vls_compat = config_GetInt( p_input, "vls-backwards-compat" );

    vlc_mutex_lock( &p_input->stream.stream_lock );

    p_pgrm = input_FindProgram( p_input, p_new_pmt->i_program_number );

    if( p_pgrm == NULL )
    {
        msg_Warn( p_input, "PMT of unreferenced program found" );
        return;
    }

    p_pgrm_demux = (pgrm_ts_data_t *)p_pgrm->p_demux_data;
    p_pgrm_demux->i_pcr_pid = p_new_pmt->i_pcr_pid;

    if( ( p_new_pmt->b_current_next &&
          ( p_new_pmt->i_version != p_pgrm_demux->i_pmt_version ) ) ||
          p_pgrm_demux->i_pmt_version == PMT_UNINITIALIZED )
    {
        dvbpsi_descriptor_t *p_dr = p_new_pmt->p_first_descriptor;
        int i_loop;

        msg_Dbg( p_input, "processing PMT for program %d version %d",
                 p_new_pmt->i_program_number, p_new_pmt->i_version );

        /* Delete all ES in this program  except the PSI. We start from the
         * end because i_es_number gets decremented after each deletion. */
        for( i_loop = p_pgrm->i_es_number ; i_loop > 0 ; )
        {
            es_ts_data_t *              p_es_demux;

            i_loop--;
            p_es_demux = (es_ts_data_t *)
                         p_pgrm->pp_es[i_loop]->p_demux_data;
            if ( !p_es_demux->b_psi )
            {
                input_DelES( p_input, p_pgrm->pp_es[i_loop] );
            }
        }

        /* IOD */
        while( p_dr && ( p_dr->i_tag != 0x1d ) )
            p_dr = p_dr->p_next;
        if( p_dr)
        {
            msg_Warn( p_input, "found IOD descriptor" );
            MP4_IODParse( &p_pgrm_demux->iod, p_dr->i_length, p_dr->p_data );
        }

        p_es = p_new_pmt->p_first_es;
        while( p_es )
        {
            vlc_fourcc_t i_fourcc;
            int i_size, i_cat, i_stream_id = 0;
            es_ts_data_t demux_data;
            BITMAPINFOHEADER *p_bih = NULL;
            WAVEFORMATEX *p_wf = NULL;
            char psz_desc[30];

            memset( &demux_data, 0, sizeof(es_ts_data_t) );
            *psz_desc = 0;
            msg_Dbg( p_input, "new PID 0x%x stream type 0x%x",
                     p_es->i_pid, p_es->i_type );

            switch( p_es->i_type )
            {
                case MPEG1_VIDEO_ES:
                case MPEG2_VIDEO_ES:
                case MPEG2_MOTO_VIDEO_ES:
                    i_fourcc = VLC_FOURCC('m','p','g','v');
                    i_cat = VIDEO_ES;
                    break;
                case MPEG1_AUDIO_ES:
                case MPEG2_AUDIO_ES:
                    i_fourcc = VLC_FOURCC('m','p','g','a');
                    i_cat = AUDIO_ES;
                    break;
                case A52_AUDIO_ES:
                    if ( !b_vls_compat )
                        i_fourcc = VLC_FOURCC('a','5','2',' ');
                    else
                        i_fourcc = VLC_FOURCC('a','5','2','b');
                    i_cat = AUDIO_ES;
                    i_stream_id = 0xBD;
                    break;
                case DVD_SPU_ES:
                    if ( !b_vls_compat )
                        i_fourcc = VLC_FOURCC('s','p','u',' ');
                    else
                        i_fourcc = VLC_FOURCC('s','p','u','b');
                    i_cat = SPU_ES;
                    i_stream_id = 0xBD;
                    break;
                case LPCM_AUDIO_ES:
                    i_fourcc = VLC_FOURCC('l','p','c','m');
                    i_cat = AUDIO_ES;
                    i_stream_id = 0xBD;
                    break;
                case SDDS_AUDIO_ES:
                    i_fourcc = VLC_FOURCC('s','d','d','s');
                    i_stream_id = 0xBD;
                    i_cat = AUDIO_ES;
                    break;
                case DTS_AUDIO_ES:
                    i_fourcc = VLC_FOURCC('d','t','s',' ');
                    i_stream_id = 0xBD;
                    i_cat = AUDIO_ES;
                    break;
                case A52B_AUDIO_ES:
                    i_fourcc = VLC_FOURCC('a','5','2','b');
                    i_cat = AUDIO_ES;
                    i_stream_id = 0xBD;
                    break;
                case DVDB_SPU_ES:
                    i_fourcc = VLC_FOURCC('s','p','u','b');
                    i_cat = SPU_ES;
                    i_stream_id = 0xBD;
                    break;
                case LPCMB_AUDIO_ES:
                    i_fourcc = VLC_FOURCC('l','p','c','b');
                    i_cat = AUDIO_ES;
                    i_stream_id = 0xBD;
                    break;
                case MPEG4_VIDEO_ES:
                    i_fourcc = VLC_FOURCC('m','p','4','v');
                    i_cat = VIDEO_ES;
                    i_stream_id = 0xfa;
                    break;
                case MPEG4_AUDIO_ES:
                case AAC_ADTS_AUDIO_ES:
                    i_fourcc = VLC_FOURCC('m','p','4','a');
                    i_cat = AUDIO_ES;
                    i_stream_id = 0xfa;
                    break;
                case H264_VIDEO_ES:
                    i_fourcc = VLC_FOURCC('h','2','6','4');
                    i_cat = VIDEO_ES;
                    break;
                case MSCODEC_VIDEO_ES:
                    i_fourcc = VLC_FOURCC(0,0,0,0);   /* fixed later */
                    i_cat = VIDEO_ES;
                    i_stream_id = 0xa0;
                    break;
                case PES_PRIVATE_ES:
                    /* We need to check a descriptor to find the real codec */
                    i_fourcc = VLC_FOURCC(0,0,0,0);   /* fixed later */
                    i_cat = UNKNOWN_ES;
                    i_stream_id = 0xbd;
                    break;
                default:
                    i_fourcc = 0;
                    i_cat = UNKNOWN_ES;
                    i_stream_id = 0;
            }

            if( p_es->i_type == MPEG4_VIDEO_ES ||
                p_es->i_type == MPEG4_AUDIO_ES )
            {
                /* mpeg4 stream, search sl_descriptor */
                dvbpsi_descriptor_t *p_dr = p_es->p_first_descriptor;

                while( p_dr && ( p_dr->i_tag != 0x1f ) ) p_dr = p_dr->p_next;

                if( p_dr && p_dr->i_length == 2 )
                {
                    int i_es_descr_index;

                    demux_data.i_es_id =
                        ( p_dr->p_data[0] << 8 ) | p_dr->p_data[1];
                    demux_data.p_es_descr = NULL;

                    msg_Warn( p_input, "found SL_descriptor" );
                    for( i_es_descr_index = 0; i_es_descr_index < 255;
                         i_es_descr_index++ )
                    {
                        if( p_pgrm_demux->iod.es_descr[i_es_descr_index].b_ok &&
                            p_pgrm_demux->iod.es_descr[i_es_descr_index].i_es_id == demux_data.i_es_id )
                        {
                            demux_data.p_es_descr =
                                &p_pgrm_demux->iod.es_descr[i_es_descr_index];
                            break;
                        }
                    }
                }

                if( demux_data.p_es_descr != NULL )
                {
#define DESCR demux_data.p_es_descr->dec_descr
                    demux_data.b_mpeg4 = 1;

                    /* fix fourcc */
                    switch( DESCR.i_streamType )
                    {
                    case 0x04:  /* VisualStream */
                        i_cat = VIDEO_ES;
                        switch( DESCR.i_objectTypeIndication )
                        {
                        case 0x20:
                            i_fourcc = VLC_FOURCC('m','p','4','v');    // mpeg4
                            break;
                        case 0x60:
                        case 0x61:
                        case 0x62:
                        case 0x63:
                        case 0x64:
                        case 0x65:
                            i_fourcc = VLC_FOURCC( 'm','p','g','v' );  // mpeg2
                            break;
                        case 0x6a:
                            i_fourcc = VLC_FOURCC( 'm','p','g','v' );  // mpeg1
                            break;
                        case 0x6c:
                            i_fourcc = VLC_FOURCC( 'j','p','e','g' );  // mpeg1
                            break;
                        default:
                            i_fourcc = 0;
                            break;
                        }
                        break;
                    case 0x05:  /* AudioStream */
                        i_cat = AUDIO_ES;
                        switch( DESCR.i_objectTypeIndication )
                        {
                        case 0x40:
                            i_fourcc = VLC_FOURCC('m','p','4','a');    // mpeg4
                            break;
                        case 0x66:
                        case 0x67:
                        case 0x68:
                            i_fourcc = VLC_FOURCC('m','p','4','a');// mpeg2 aac
                            break;
                        case 0x69:
                            i_fourcc = VLC_FOURCC('m','p','g','a');    // mpeg2
                            break;
                        case 0x6b:
                            i_fourcc = VLC_FOURCC('m','p','g','a');    // mpeg1
                            break;
                        default:
                            i_fourcc = 0;
                            break;
                        }
                        break;
                    default:
                        i_cat = UNKNOWN_ES;
                        i_fourcc = 0;
                        break;
                    }

                    switch( i_cat )
                    {
                    case VIDEO_ES:
                        i_size = sizeof( BITMAPINFOHEADER ) +
                                 DESCR.i_decoder_specific_info_len;
                        p_bih = malloc( i_size );
                        p_bih->biSize = i_size;
                        p_bih->biWidth = 0;
                        p_bih->biHeight = 0;
                        p_bih->biPlanes = 1;
                        p_bih->biBitCount = 0;
                        p_bih->biCompression = 0;
                        p_bih->biSizeImage = 0;
                        p_bih->biXPelsPerMeter = 0;
                        p_bih->biYPelsPerMeter = 0;
                        p_bih->biClrUsed = 0;
                        p_bih->biClrImportant = 0;
                        memcpy( &p_bih[1],
                                DESCR.p_decoder_specific_info,
                                DESCR.i_decoder_specific_info_len );
                        break;
                    case AUDIO_ES:
                        i_size = sizeof( WAVEFORMATEX ) +
                                 DESCR.i_decoder_specific_info_len;
                        p_wf = malloc( i_size );
                        p_wf->wFormatTag = 0xffff;
                        p_wf->nChannels = 0;
                        p_wf->nSamplesPerSec = 0;
                        p_wf->nAvgBytesPerSec = 0;
                        p_wf->nBlockAlign = 1;
                        p_wf->wBitsPerSample = 0;
                        p_wf->cbSize = DESCR.i_decoder_specific_info_len;
                        memcpy( &p_wf[1],
                                DESCR.p_decoder_specific_info,
                                DESCR.i_decoder_specific_info_len );
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    msg_Warn( p_input,
                              "mpeg4 stream without (valid) sl_descriptor" );
                    demux_data.b_mpeg4 = 0;
                }

            }
            else if( p_es->i_type == MSCODEC_VIDEO_ES )
            {
                /* crapy ms codec stream, search private descriptor */
                dvbpsi_descriptor_t *p_dr = p_es->p_first_descriptor;

                while( p_dr && ( p_dr->i_tag != 0xa0 ) ) p_dr = p_dr->p_next;

                if( p_dr && p_dr->i_length >= 8 )
                {
                    int i_bih_size;
                    i_fourcc = VLC_FOURCC( p_dr->p_data[0], p_dr->p_data[1],
                                           p_dr->p_data[2], p_dr->p_data[3] );

                    i_bih_size = (p_dr->p_data[8] << 8) | p_dr->p_data[9];
                    i_size = sizeof( BITMAPINFOHEADER ) + i_bih_size;

                    p_bih = malloc( i_size );
                    p_bih->biSize = i_size;
                    p_bih->biWidth = ( p_dr->p_data[4] << 8 )|p_dr->p_data[5];
                    p_bih->biHeight = ( p_dr->p_data[6] << 8 )|p_dr->p_data[7];
                    p_bih->biPlanes = 1;
                    p_bih->biBitCount = 0;
                    p_bih->biCompression = 0;
                    p_bih->biSizeImage = 0;
                    p_bih->biXPelsPerMeter = 0;
                    p_bih->biYPelsPerMeter = 0;
                    p_bih->biClrUsed = 0;
                    p_bih->biClrImportant = 0;
                    memcpy( &p_bih[1], &p_dr->p_data[10], i_bih_size );
                }
                else
                {
                    msg_Warn( p_input, "private ms-codec stream without bih "
                              "private sl_descriptor" );
                    i_fourcc = 0;
                    i_cat = UNKNOWN_ES;
                }
            }
            else if( p_es->i_type == PES_PRIVATE_ES )
            {
                dvbpsi_descriptor_t *p_dr = p_es->p_first_descriptor;
                /* We have to find a descriptor giving the right codec */

                for(p_dr = p_es->p_first_descriptor; p_dr; p_dr = p_dr->p_next)
                {
                    if( p_dr->i_tag == 0x6a )
                    {
                        /* A52 */
                        i_fourcc = VLC_FOURCC( 'a', '5', '2', ' ' );
                        i_cat    = AUDIO_ES;
                    }
#ifdef _DVBPSI_DR_59_H_
                    else if( p_dr->i_tag == 0x59 )
                    {
                        uint16_t                n;
                        es_descriptor_t *       p_dvbsub_es;
                        es_ts_data_t            dvbsub_demux_data;
                        dvb_spuinfo_t*          p_info;
                        dvbpsi_subtitling_dr_t* sub;
                        demux_data.b_dvbsub = 1;
                        demux_data.i_dvbsub_es_count = 0;

                        /* DVB subtitle */
                        i_fourcc = VLC_FOURCC( 'd', 'v', 'b', 's' );
                        i_cat    = SPU_ES;

                        sub = dvbpsi_DecodeSubtitlingDr( p_dr );
                        for( n = 0; n < sub->i_subtitles_number; n++)
                        {
                            /* As each subtitle ES contains n languages, 
                             * We are going to create n fake ES for the n
                             * tracks */
                            local_iso639_getlang(
                                    sub->p_subtitle[n].i_iso6392_language_code,
                                    psz_desc);
                            p_dvbsub_es = input_AddES( p_input,
                                                       p_pgrm,
                                                       0xfe12+n,
                                                       SPU_ES,
                                                       psz_desc,
                                                       sizeof(es_ts_data_t) );
                            if( p_dvbsub_es == NULL )
                            {
                                msg_Err( p_input, "could not add ES %d",
                                                                p_es->i_pid );
                                p_input->b_error = 1;
                                return;
                            }

                            p_dvbsub_es->i_fourcc = i_fourcc;
                            p_dvbsub_es->i_stream_id = i_stream_id;
                            p_info = malloc(sizeof(dvb_spuinfo_t));
                            p_info->i_id =
                                    sub->p_subtitle[n].i_composition_page_id;
                            p_dvbsub_es->p_spuinfo = (void*) p_info;
                            memcpy( p_dvbsub_es->p_demux_data,
                                    &dvbsub_demux_data,
                                    sizeof(es_ts_data_t) );
                            ((es_ts_data_t *)p_dvbsub_es->p_demux_data)
                                                ->i_continuity_counter = 0xFF;
                            // Finaly we add this stream to the index
                            demux_data.p_dvbsub_es[
                                demux_data.i_dvbsub_es_count++] = p_dvbsub_es;
                            i_cat = UNKNOWN_ES;
                        }

                    }
#endif /* _DVBPSI_DR_59_H_ */
                }
                if( i_fourcc == VLC_FOURCC(0,0,0,0) )
                {
                    msg_Warn( p_input,
                              "unknown codec/type for Private PES stream" );
                }
            }

            if( i_cat == AUDIO_ES || i_cat == SPU_ES )
            {
                dvbpsi_descriptor_t *p_dr = p_es->p_first_descriptor;
                while( p_dr && ( p_dr->i_tag != 0x0a ) ) p_dr = p_dr->p_next;

                if( p_dr )
                {
                    dvbpsi_iso639_dr_t *p_decoded =
                                                dvbpsi_DecodeISO639Dr( p_dr );
                    if( p_decoded != NULL )
                        local_iso639_getlang(p_decoded->i_iso_639_code, psz_desc);
#if 0
                    if( p_decoded->i_code_count > 0 )
                    {
                        const iso639_lang_t * p_iso;
                        p_iso = GetLang_2T((char*)p_decoded->i_iso_639_code);

                        if( p_iso && strcmp(p_iso->psz_native_name,"Unknown"))
                        {
                            if( p_iso->psz_native_name[0] )
                                strncpy( psz_desc,
                                         p_iso->psz_native_name, 20 );
                            else
                                strncpy( psz_desc,
                                         p_iso->psz_eng_name, 20 );
                        }
                        else
                        {
                            p_iso = GetLang_2B(
                                        (char*)p_decoded->i_iso_639_code);
                            if ( p_iso )
                            {
                              if( p_iso->psz_native_name[0] )
                                strncpy( psz_desc,
                                         p_iso->psz_native_name, 20 );
                              else
                                strncpy( psz_desc,
                                         p_iso->psz_eng_name, 20 );
                            }
                            else
                            {
                              strncpy( psz_desc, p_decoded->i_iso_639_code, 3 );
                            }
                        }
                    }
#endif
                }
                switch( p_es->i_type )
                {
                    case MPEG1_AUDIO_ES:
                    case MPEG2_AUDIO_ES:
                        strcat( psz_desc, " (mpeg)" );
                        break;
                    case LPCM_AUDIO_ES:
                    case LPCMB_AUDIO_ES:
                        strcat( psz_desc, " (lpcm)" );
                        break;
                    case A52_AUDIO_ES:
                    case A52B_AUDIO_ES:
                        strcat( psz_desc, " (A52)" );
                        break;
                    case MPEG4_AUDIO_ES:
                    case AAC_ADTS_AUDIO_ES:
                        strcat( psz_desc, " (aac)" );
                        break;
                }
            }

            /* Add this ES */
            p_new_es = input_AddES( p_input, p_pgrm, (uint16_t)p_es->i_pid,
                                    i_cat, psz_desc, sizeof( es_ts_data_t ) );
            if( p_new_es == NULL )
            {
                msg_Err( p_input, "could not add ES %d", p_es->i_pid );
                p_input->b_error = 1;
                return;
            }
            p_new_es->i_fourcc = i_fourcc;
            p_new_es->i_stream_id = i_stream_id;
            p_new_es->p_bitmapinfoheader = (void*)p_bih;
            p_new_es->p_waveformatex = (void*)p_wf;
            memcpy( p_new_es->p_demux_data, &demux_data,
                    sizeof(es_ts_data_t) );

            ((es_ts_data_t *)p_new_es->p_demux_data)->i_continuity_counter =
                0xFF;

            p_es = p_es->p_next;
        }

        /* if no program is selected :*/
        if( !p_input->stream.p_selected_program )
        {
            pgrm_descriptor_t *     p_pgrm_to_select;
            uint16_t i_id = (uint16_t)config_GetInt( p_input, "program" );

            if( i_id != 0 ) /* if user specified a program */
            {
                p_pgrm_to_select = input_FindProgram( p_input, i_id );

                if( p_pgrm_to_select && p_pgrm_to_select == p_pgrm )
                    p_input->pf_set_program( p_input, p_pgrm_to_select );
            }
            else
                p_input->pf_set_program( p_input, p_pgrm );
        }
        /* if the pmt belongs to the currently selected program, we
         * reselect it to update its ES */
        else if( p_pgrm == p_input->stream.p_selected_program )
        {
            p_input->pf_set_program( p_input, p_pgrm );
        }

        p_pgrm_demux->i_pmt_version = p_new_pmt->i_version;
        p_input->stream.b_changed = 1;
    }
    vlc_mutex_unlock( &p_input->stream.stream_lock );
}
#endif
