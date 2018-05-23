/*****************************************************************************
 * input_ext-plugins.c: useful functions for access and demux plug-ins
 *****************************************************************************
 * Copyright (C) 2001-2004 VideoLAN
 * $Id: input_ext-plugins.c 7041 2004-03-11 16:48:27Z gbazin $
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
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

#include "stream_control.h"
#include "input_ext-intf.h"
#include "input_ext-dec.h"
#include "input_ext-plugins.h"


/*
 * Buffers management : internal functions
 *
 * All functions are static, but exported versions with mutex protection
 * start with input_*. Not all of these exported functions are actually used,
 * but they are included here for completeness.
 */

#define BUFFERS_CACHE_SIZE 500
#define DATA_CACHE_SIZE 1000
#define PES_CACHE_SIZE 1000

/*****************************************************************************
 * data_buffer_t: shared data type
 *****************************************************************************/
struct data_buffer_t
{
    data_buffer_t * p_next;

    /* number of data packets this buffer is referenced from - when it falls
     * down to 0, the buffer is freed */
    int i_refcount;

    /* size of the current buffer (starting right after this byte) */
    size_t i_size;
};

/*****************************************************************************
 * input_buffers_t: defines a LIFO per data type to keep
 *****************************************************************************/
#define PACKETS_LIFO( TYPE, NAME )                                          \
struct                                                                      \
{                                                                           \
    TYPE * p_stack;                                                         \
    unsigned int i_depth;                                                   \
} NAME;

struct input_buffers_t
{
    vlc_mutex_t lock;
    PACKETS_LIFO( pes_packet_t, pes )
    PACKETS_LIFO( data_packet_t, data )
    PACKETS_LIFO( data_buffer_t, buffers )
    size_t i_allocated;
};


/*****************************************************************************
 * input_BuffersInit: initialize the cache structures, return a pointer to it
 *****************************************************************************/
void * __input_BuffersInit( vlc_object_t *p_this )
{
    input_buffers_t * p_buffers = malloc( sizeof( input_buffers_t ) );

    if( p_buffers == NULL )
    {
        return NULL;
    }

    memset( p_buffers, 0, sizeof( input_buffers_t ) );
    vlc_mutex_init( p_this, &p_buffers->lock );

    return p_buffers;
}

/*****************************************************************************
 * input_BuffersEnd: free all cached structures
 *****************************************************************************/
#define BUFFERS_END_PACKETS_LOOP                                            \
    while( p_packet != NULL )                                               \
    {                                                                       \
        p_next = p_packet->p_next;                                          \
        free( p_packet );                                                   \
        p_packet = p_next;                                                  \
    }

void input_BuffersEnd( input_thread_t * p_input, input_buffers_t * p_buffers )
{
    if( p_buffers != NULL )
    {
        msg_Dbg( p_input, "pes: %d packets", p_buffers->pes.i_depth );
        msg_Dbg( p_input, "data: %d packets", p_buffers->data.i_depth );
        msg_Dbg( p_input, "buffers: %d packets", p_buffers->buffers.i_depth );

        {
            /* Free PES */
            pes_packet_t * p_next, * p_packet = p_buffers->pes.p_stack;
            BUFFERS_END_PACKETS_LOOP;
        }

        {
            /* Free data packets */
            data_packet_t * p_next, * p_packet = p_buffers->data.p_stack;
            BUFFERS_END_PACKETS_LOOP;
        }

        {
            /* Free buffers */
            data_buffer_t * p_next, * p_buf = p_buffers->buffers.p_stack;
            while( p_buf != NULL )
            {
                p_next = p_buf->p_next;
                p_buffers->i_allocated -= p_buf->i_size;
                free( p_buf );
                p_buf = p_next;
            }
        }

        if( p_buffers->i_allocated )
        {
            msg_Warn( p_input, "%u bytes have not been freed, "
                              "expect memory leak", p_buffers->i_allocated );
        }

        vlc_mutex_destroy( &p_buffers->lock );
        free( p_buffers );
    }
}

/*****************************************************************************
 * input_NewBuffer: return a pointer to a data buffer of the appropriate size
 *****************************************************************************/
static inline data_buffer_t * NewBuffer( input_buffers_t * p_buffers,
                                         size_t i_size )
{
    data_buffer_t * p_buf;

    /* Safety check */
    if( p_buffers->i_allocated > INPUT_MAX_ALLOCATION )
    {
        return NULL;
    }

    if( p_buffers->buffers.p_stack != NULL )
    {
        /* Take the buffer from the cache */
        p_buf = p_buffers->buffers.p_stack;
        p_buffers->buffers.p_stack = p_buf->p_next;
        p_buffers->buffers.i_depth--;

        /* Reallocate the packet if it is too small or too large */
        if( p_buf->i_size < i_size || p_buf->i_size > 3 * i_size )
        {
            p_buffers->i_allocated -= p_buf->i_size;
            free( p_buf );
            p_buf = malloc( sizeof(input_buffers_t) + i_size );
            if( p_buf == NULL )
            {
                return NULL;
            }
            p_buf->i_size = i_size;
            p_buffers->i_allocated += i_size;
        }
    }
    else
    {
        /* Allocate a new buffer */
        p_buf = malloc( sizeof(input_buffers_t) + i_size );
        if( p_buf == NULL )
        {
            return NULL;
        }
        p_buf->i_size = i_size;
        p_buffers->i_allocated += i_size;
    }

    /* Initialize data */
    p_buf->p_next = NULL;
    p_buf->i_refcount = 0;

    return p_buf;
}

data_buffer_t * input_NewBuffer( input_buffers_t * p_buffers, size_t i_size )
{
    data_buffer_t * p_buf;

    vlc_mutex_lock( &p_buffers->lock );
    p_buf = NewBuffer( p_buffers, i_size );
    vlc_mutex_unlock( &p_buffers->lock );

    return p_buf;
}

/*****************************************************************************
 * input_ReleaseBuffer: put a buffer back into the cache
 *****************************************************************************/
static inline void ReleaseBuffer( input_buffers_t * p_buffers,
                                  data_buffer_t * p_buf )
{
    /* Decrement refcount */
    if( --p_buf->i_refcount > 0 )
    {
        return;
    }

    if( p_buffers->buffers.i_depth < BUFFERS_CACHE_SIZE )
    {
        /* Cache not full : store the buffer in it */
        p_buf->p_next = p_buffers->buffers.p_stack;
        p_buffers->buffers.p_stack = p_buf;
        p_buffers->buffers.i_depth++;
    }
    else
    {
        p_buffers->i_allocated -= p_buf->i_size;
        free( p_buf );
    }
}

void input_ReleaseBuffer( input_buffers_t * p_buffers, data_buffer_t * p_buf )
{
    vlc_mutex_lock( &p_buffers->lock );
    ReleaseBuffer( p_buffers, p_buf );
    vlc_mutex_unlock( &p_buffers->lock );
}

/*****************************************************************************
 * input_ShareBuffer: allocate a data_packet_t pointing to a given buffer
 *****************************************************************************/
static inline data_packet_t * ShareBuffer( input_buffers_t * p_buffers,
                                           data_buffer_t * p_buf )
{
    data_packet_t * p_data;

    if( p_buffers->data.p_stack != NULL )
    {
        /* Take the packet from the cache */
        p_data = p_buffers->data.p_stack;
        p_buffers->data.p_stack = p_data->p_next;
        p_buffers->data.i_depth--;
    }
    else
    {
        /* Allocate a new packet */
        p_data = malloc( sizeof(data_packet_t) );
        if( p_data == NULL )
        {
            return NULL;
        }
    }

    p_data->p_buffer = p_buf;
    p_data->p_next = NULL;
    p_data->b_discard_payload = 0;
    p_data->p_payload_start = p_data->p_demux_start
                            = (byte_t *)p_buf + sizeof(input_buffers_t);
    p_data->p_payload_end = p_data->p_demux_start + p_buf->i_size;
    p_buf->i_refcount++;

    return p_data;
}

data_packet_t * input_ShareBuffer( input_buffers_t * p_buffers,
                                   data_buffer_t * p_buf )
{
    data_packet_t * p_data;

    vlc_mutex_lock( &p_buffers->lock );
    p_data = ShareBuffer( p_buffers, p_buf );
    vlc_mutex_unlock( &p_buffers->lock );

    return p_data;
}

/*****************************************************************************
 * input_NewPacket: allocate a packet along with a buffer
 *****************************************************************************/
static inline data_packet_t * NewPacket( input_buffers_t * p_buffers,
                                         size_t i_size )
{
    data_buffer_t * p_buf;
    data_packet_t * p_data;

    p_buf = NewBuffer( p_buffers, i_size );

    if( p_buf == NULL )
    {
        return NULL;
    }

    p_data = ShareBuffer( p_buffers, p_buf );
    if( p_data == NULL )
    {
        ReleaseBuffer( p_buffers, p_buf );
    }
    return p_data;
}

data_packet_t * input_NewPacket( input_buffers_t * p_buffers, size_t i_size )
{
    data_packet_t * p_data;

    vlc_mutex_lock( &p_buffers->lock );
    p_data = NewPacket( p_buffers, i_size );
    vlc_mutex_unlock( &p_buffers->lock );

    return p_data;
}

/*****************************************************************************
 * input_DeletePacket: deallocate a packet and its buffers
 *****************************************************************************/
static inline void DeletePacket( input_buffers_t * p_buffers,
                                 data_packet_t * p_data )
{
    while( p_data != NULL )
    {
        data_packet_t * p_next = p_data->p_next;

        ReleaseBuffer( p_buffers, p_data->p_buffer );

        if( p_buffers->data.i_depth < DATA_CACHE_SIZE )
        {
            /* Cache not full : store the packet in it */
            p_data->p_next = p_buffers->data.p_stack;
            p_buffers->data.p_stack = p_data;
            p_buffers->data.i_depth++;
        }
        else
        {
            free( p_data );
        }

        p_data = p_next;
    }
}

void input_DeletePacket( input_buffers_t * p_buffers, data_packet_t * p_data )
{
    vlc_mutex_lock( &p_buffers->lock );
    DeletePacket( p_buffers, p_data );
    vlc_mutex_unlock( &p_buffers->lock );
}

/*****************************************************************************
 * input_NewPES: return a pointer to a new PES packet
 *****************************************************************************/
static inline pes_packet_t * NewPES( input_buffers_t * p_buffers )
{
    pes_packet_t * p_pes;

    if( p_buffers->pes.p_stack != NULL )
    {
        /* Take the packet from the cache */
        p_pes = p_buffers->pes.p_stack;
        p_buffers->pes.p_stack = p_pes->p_next;
        p_buffers->pes.i_depth--;
    }
    else
    {
        /* Allocate a new packet */
        p_pes = malloc( sizeof(pes_packet_t) );
        if( p_pes == NULL )
        {
            return NULL;
        }
    }

    p_pes->p_next = NULL;
    p_pes->b_data_alignment = p_pes->b_discontinuity = VLC_FALSE;
    p_pes->i_pts = p_pes->i_dts = 0;
    p_pes->p_first = p_pes->p_last = NULL;
    p_pes->i_pes_size = 0;
    p_pes->i_nb_data = 0;

    return p_pes;
}

pes_packet_t * input_NewPES( input_buffers_t * p_buffers )
{
    pes_packet_t * p_pes;

    vlc_mutex_lock( &p_buffers->lock );
    p_pes = NewPES( p_buffers );
    vlc_mutex_unlock( &p_buffers->lock );

    return p_pes;
}

/*****************************************************************************
 * input_DeletePES: put a pes and all data packets and all buffers back into
 *                  the cache
 *****************************************************************************/
static inline void DeletePES( input_buffers_t * p_buffers,
                              pes_packet_t * p_pes )
{
    while( p_pes != NULL )
    {
        pes_packet_t * p_next = p_pes->p_next;

        /* Delete all data packets */
        if( p_pes->p_first != NULL )
        {
            DeletePacket( p_buffers, p_pes->p_first );
        }

        if( p_buffers->pes.i_depth < PES_CACHE_SIZE )
        {
            /* Cache not full : store the packet in it */
            p_pes->p_next = p_buffers->pes.p_stack;
            p_buffers->pes.p_stack = p_pes;
            p_buffers->pes.i_depth++;
        }
        else
        {
            free( p_pes );
        }

        p_pes = p_next;
    }
}

void input_DeletePES( input_buffers_t * p_buffers, pes_packet_t * p_pes )
{
    vlc_mutex_lock( &p_buffers->lock );
    DeletePES( p_buffers, p_pes );
    vlc_mutex_unlock( &p_buffers->lock );
}


/*
 * Buffers management : external functions
 *
 * These functions make the glu between the access plug-in (pf_read) and
 * the demux plug-in (pf_demux). We fill in a large buffer (approx. 10s kB)
 * with a call to pf_read, then allow the demux plug-in to have a peep at
 * it (input_Peek), and to split it in data_packet_t (input_SplitBuffer).
 */
/*****************************************************************************
 * input_FillBuffer: fill in p_data_buffer with data from pf_read
 *****************************************************************************/
ssize_t input_FillBuffer( input_thread_t * p_input )
{
    ptrdiff_t i_remains = p_input->p_last_data - p_input->p_current_data;
    data_buffer_t * p_buf = NULL;
    ssize_t i_ret;

    vlc_mutex_lock( &p_input->p_method_data->lock );

    while( p_buf == NULL )
    {
        p_buf = NewBuffer( p_input->p_method_data,
                           i_remains + p_input->i_bufsize );
        if( p_buf == NULL )
        {
            vlc_mutex_unlock( &p_input->p_method_data->lock );
            msg_Err( p_input,
                     "failed allocating a new buffer (decoder stuck?)" );
            msleep( INPUT_IDLE_SLEEP );

            if( p_input->b_die || p_input->b_error || p_input->b_eof )
            {
                return -1;
            }
            vlc_mutex_lock( &p_input->p_method_data->lock );
        }
    }

    p_buf->i_refcount = 1;

    if( p_input->p_data_buffer != NULL )
    {
        if( i_remains )
        {
            p_input->p_vlc->pf_memcpy( (byte_t *)p_buf + sizeof(data_buffer_t),
                                       p_input->p_current_data,
                                       (size_t)i_remains );
        }
        ReleaseBuffer( p_input->p_method_data, p_input->p_data_buffer );
    }

    p_input->p_data_buffer = p_buf;
    p_input->p_current_data = (byte_t *)p_buf + sizeof(data_buffer_t);
    p_input->p_last_data = p_input->p_current_data + i_remains;

    /* Do not hold the lock during pf_read (blocking call). */
    vlc_mutex_unlock( &p_input->p_method_data->lock );

    i_ret = p_input->pf_read( p_input,
                              (byte_t *)p_buf + sizeof(data_buffer_t)
                               + i_remains,
                              p_input->i_bufsize );
    if( i_ret < 0 && i_remains == 0 )
    {
        /* Our internal buffers are empty, we can signal the error */
        return -1;
    }

    if( i_ret < 0 ) i_ret = 0;

    p_input->p_last_data += i_ret;

    return (ssize_t)i_remains + i_ret;
}

/*****************************************************************************
 * input_Peek: give a pointer to the next available bytes in the buffer
 *             (min. i_size bytes)
 * Returns the number of bytes read, or -1 in case of error
 *****************************************************************************/
ssize_t input_Peek( input_thread_t * p_input, byte_t ** pp_byte,
                    size_t i_size )
{
    ssize_t i_data = p_input->p_last_data - p_input->p_current_data;

    while( i_data < (ssize_t)i_size )
    {
        /* Go to the next buffer */
        ssize_t i_ret = input_FillBuffer( p_input );

        if( i_ret < 0 )
        {
            return -1;
        }

        if( i_ret == i_data )
        {
            /* We didn't get anymore data, must be the EOF */
            i_size = i_data;
            break;
        }

        i_data = i_ret;
    }

    *pp_byte = p_input->p_current_data;
    return i_size;
}

/*****************************************************************************
 * input_SplitBuffer: give a pointer to a data packet containing i_size bytes
 * Returns the number of bytes read, or -1 in case of error
 *****************************************************************************/
ssize_t input_SplitBuffer( input_thread_t * p_input,
                           data_packet_t ** pp_data, size_t i_size )
{
    ssize_t i_data = p_input->p_last_data - p_input->p_current_data;

    while( i_data < (ssize_t)i_size )
    {
        /* Go to the next buffer */
        ssize_t i_ret = input_FillBuffer( p_input );

        if( i_ret < 0 )
        {
            return -1;
        }

        if( i_ret == i_data )
        {
            /* We didn't get anymore data, must be the EOF */
            i_size = i_data;
            break;
        }

        i_data = i_ret;
    }

    if( i_size < 0)
    {
        return 0;
    }

    *pp_data = input_ShareBuffer( p_input->p_method_data,
                                  p_input->p_data_buffer );

    (*pp_data)->p_demux_start = (*pp_data)->p_payload_start
        = p_input->p_current_data;
    (*pp_data)->p_payload_end = (*pp_data)->p_demux_start + i_size;

    p_input->p_current_data += i_size;

    /* Update stream position */
    vlc_mutex_lock( &p_input->stream.stream_lock );
    p_input->stream.p_selected_area->i_tell += i_size;
    vlc_mutex_unlock( &p_input->stream.stream_lock );

    return i_size;
}

/*****************************************************************************
 * input_AccessInit: initialize access plug-in wrapper structures
 *****************************************************************************/
int input_AccessInit( input_thread_t * p_input )
{
    p_input->p_method_data = input_BuffersInit( p_input );
    if( p_input->p_method_data == NULL ) return -1;
    p_input->p_data_buffer = NULL;
    p_input->p_current_data = NULL;
    p_input->p_last_data = NULL;
    return 0;
}

/*****************************************************************************
 * input_AccessReinit: reinit structures before a random seek
 *****************************************************************************/
void input_AccessReinit( input_thread_t * p_input )
{
    if( p_input->p_data_buffer != NULL )
    {
        ReleaseBuffer( p_input->p_method_data, p_input->p_data_buffer );
    }
    p_input->p_data_buffer = NULL;
    p_input->p_current_data = NULL;
    p_input->p_last_data = NULL;
}

/*****************************************************************************
 * input_AccessEnd: free access plug-in wrapper structures
 *****************************************************************************/
void input_AccessEnd( input_thread_t * p_input )
{
    if( p_input->p_data_buffer != NULL )
    {
        ReleaseBuffer( p_input->p_method_data, p_input->p_data_buffer );
    }

    input_BuffersEnd( p_input, p_input->p_method_data );
}

