/*****************************************************************************
 * record.c: record stream output module
 *****************************************************************************
 * Copyright (C) 2008 the VideoLAN team
 * $Id$
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <limits.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_block.h>
#include <vlc_sout.h>
#include <vlc_charset.h>
#include <assert.h>

/*****************************************************************************
 * Exported prototypes
 *****************************************************************************/
static int      Open    ( vlc_object_t * );
static void     Close   ( vlc_object_t * );

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define DST_PREFIX_TEXT N_("Destination prefix")
#define DST_PREFIX_LONGTEXT N_( \
    "Prefix of the destination file automatically generated" )

#define SOUT_CFG_PREFIX "sout-record-"

vlc_module_begin ()
    set_description( N_("Record stream output") )
    set_capability( "sout stream", 0 )
    add_shortcut( "record" )
    set_shortname( N_("Record") )

    set_category( CAT_SOUT )
    set_subcategory( SUBCAT_SOUT_STREAM )

    add_string( SOUT_CFG_PREFIX "dst-prefix", "", NULL, DST_PREFIX_TEXT,
                DST_PREFIX_LONGTEXT, true )

    set_callbacks( Open, Close )
vlc_module_end ()

/* */
static const char *const ppsz_sout_options[] = {
    "dst-prefix",
    NULL
};

/* */
static sout_stream_id_t *Add ( sout_stream_t *, es_format_t * );
static int               Del ( sout_stream_t *, sout_stream_id_t * );
static int               Send( sout_stream_t *, sout_stream_id_t *, block_t* );

/* */
struct sout_stream_id_t
{
    es_format_t fmt;

    block_t *p_first;
    block_t **pp_last;

    sout_stream_id_t *id;

    bool b_wait_key;
    bool b_wait_start;
};

struct sout_stream_sys_t
{
    char *psz_prefix;

    sout_stream_t *p_out;

    mtime_t     i_date_start;
    size_t      i_size;

    mtime_t     i_max_wait;
    size_t      i_max_size;

    bool        b_drop;

    int              i_id;
    sout_stream_id_t **id;
    mtime_t     i_dts_start;
};

static void OutputStart( sout_stream_t *p_stream );
static void OutputSend( sout_stream_t *p_stream, sout_stream_id_t *id, block_t * );

/*****************************************************************************
 * Open:
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    sout_stream_t *p_stream = (sout_stream_t*)p_this;
    sout_stream_sys_t *p_sys;

    p_stream->pf_add    = Add;
    p_stream->pf_del    = Del;
    p_stream->pf_send   = Send;

    p_stream->p_sys = p_sys = malloc( sizeof(*p_sys) );
    if( !p_sys )
        return VLC_ENOMEM;

    config_ChainParse( p_stream, SOUT_CFG_PREFIX, ppsz_sout_options, p_stream->p_cfg );

    p_sys->p_out = NULL;
    p_sys->psz_prefix = var_GetNonEmptyString( p_stream, SOUT_CFG_PREFIX "dst-prefix" );
    if( !p_sys->psz_prefix  )
    {
        p_sys->psz_prefix = strdup( "sout-record-" );
        if( !p_sys->psz_prefix )
        {
            free( p_sys );
            return VLC_ENOMEM;
        }
    }

    p_sys->i_date_start = -1;
    p_sys->i_size = 0;
#ifdef OPTIMIZE_MEMORY
    p_sys->i_max_wait = 5*1000000; /* 5s */
    p_sys->i_max_size = 1*1000000; /* 1 Mbyte */
#else
    p_sys->i_max_wait = 30*1000000; /* 30s */
    p_sys->i_max_size = 20*1000000; /* 20 Mbyte */
#endif
    p_sys->b_drop = false;
    p_sys->i_dts_start = 0;
    TAB_INIT( p_sys->i_id, p_sys->id );

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close:
 *****************************************************************************/
static void Close( vlc_object_t * p_this )
{
    sout_stream_t *p_stream = (sout_stream_t*)p_this;
    sout_stream_sys_t *p_sys = p_stream->p_sys;

    if( p_sys->p_out )
        sout_StreamDelete( p_sys->p_out );

    TAB_CLEAN( p_sys->i_id, p_sys->id );
    free( p_sys->psz_prefix );
    free( p_sys );
}

/*****************************************************************************
 *
 *****************************************************************************/
static sout_stream_id_t *Add( sout_stream_t *p_stream, es_format_t *p_fmt )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;
    sout_stream_id_t *id;

    id = malloc( sizeof(*id) );
    if( !id )
        return NULL;

    es_format_Copy( &id->fmt, p_fmt );
    id->p_first = NULL;
    id->pp_last = &id->p_first;
    id->id = NULL;
    id->b_wait_key = true;
    id->b_wait_start = true;

    TAB_APPEND( p_sys->i_id, p_sys->id, id );

    return id;
}

static int Del( sout_stream_t *p_stream, sout_stream_id_t *id )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;

    if( !p_sys->p_out )
        OutputStart( p_stream );

    if( id->p_first )
        block_ChainRelease( id->p_first );

    assert( !id->id || p_sys->p_out );
    if( id->id )
        sout_StreamIdDel( p_sys->p_out, id->id );

    es_format_Clean( &id->fmt );

    TAB_REMOVE( p_sys->i_id, p_sys->id, id );

    if( p_sys->i_id <= 0 )
    {
        if( !p_sys->p_out )
            p_sys->b_drop = false;
    }

    free( id );

    return VLC_SUCCESS;
}

static int Send( sout_stream_t *p_stream, sout_stream_id_t *id,
                 block_t *p_buffer )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;

    if( p_sys->i_date_start < 0 )
        p_sys->i_date_start = mdate();
    if( !p_sys->p_out &&
        ( mdate() - p_sys->i_date_start > p_sys->i_max_wait ||
          p_sys->i_size > p_sys->i_max_size ) )
    {
        msg_Dbg( p_stream, "Starting recording, waited %ds and %dbyte",
                 (int)((mdate() - p_sys->i_date_start)/1000000), (int)p_sys->i_size );
        OutputStart( p_stream );
    }

    OutputSend( p_stream, id, p_buffer );

    return VLC_SUCCESS;
}

/*****************************************************************************
 *
 *****************************************************************************/
typedef struct
{
    const char  *psz_muxer;
    const char  *psz_extension;
    int         i_es_max;
    vlc_fourcc_t codec[128];
} muxer_properties_t;

#define M(muxer, ext, count, ... ) { .psz_muxer = muxer, .psz_extension = ext, .i_es_max = count, .codec = { __VA_ARGS__, 0 } }
/* Table of native codec support,
 * Do not do non native and non standard association !
 * Muxer will be probe if no entry found */
static const muxer_properties_t p_muxers[] = {
    M( "raw", "mp3", 1,         VLC_FOURCC('m','p','g','a') ),
    M( "raw", "a52", 1,         VLC_FOURCC('a','5','2',' ') ),
    M( "raw", "dts", 1,         VLC_FOURCC('d','t','s',' ') ),
    M( "raw", "mpc", 1,         VLC_FOURCC('m','p','c',' ') ),
    M( "raw", "ape", 1,         VLC_FOURCC('A','P','E',' ') ),

    M( "wav", "wav", 1,         VLC_FOURCC('a','r','a','w'), VLC_FOURCC('u','8',' ',' '), VLC_FOURCC('s','1','6','l'),
                                VLC_FOURCC('s','2','4','l'), VLC_FOURCC('s','3','2','l'), VLC_FOURCC('f','l','3','2') ),

    //M( "ffmpeg{mux=flac}", "flac", 1, VLC_FOURCC('f','l','a','c') ), BROKEN

    M( "ogg", "ogg", INT_MAX,   VLC_FOURCC('v','o','r','b'), VLC_FOURCC('s','p','x',' '), VLC_FOURCC('f','l','a','c'),
                                VLC_FOURCC('s','u','b','t'), VLC_FOURCC('t','h','e','o'), VLC_FOURCC('d','r','a','c')  ),

    M( "asf", "asf", 127,       VLC_FOURCC('w','m','a','1'), VLC_FOURCC('w','m','a','2'), VLC_FOURCC('w','m','a',' '),
                                VLC_FOURCC('w','m','a','p'), VLC_FOURCC('w','m','a','l'),
                                VLC_FOURCC('W','M','V','1'), VLC_FOURCC('W','M','V','2'), VLC_FOURCC('W','M','V','3'),
                                VLC_FOURCC('W','V','C','1')),

    M( "mp4", "mp4", INT_MAX,   VLC_FOURCC('m','p','4','a'), VLC_FOURCC('h','2','6','4'), VLC_FOURCC('m','p','4','v'),
                                VLC_FOURCC('s','u','b','t') ),

    M( "ps", "mpg", 16/* FIXME*/,VLC_FOURCC('m','p','g','v'), VLC_FOURCC('m','p','1','v'), VLC_FOURCC('m','p','2','v'),
                                VLC_FOURCC('m','p','g','a'), VLC_FOURCC('l','p','c','m'), VLC_FOURCC('a','5','2',' '),
                                VLC_FOURCC('d','t','s',' '),
                                VLC_FOURCC('s','p','u',' ') ),

    M( "ts", "ts", 8000,        VLC_FOURCC('m','p','g','v'), VLC_FOURCC('m','p','1','v'), VLC_FOURCC('m','p','2','v'),
                                VLC_FOURCC('h','2','6','4'),
                                VLC_FOURCC('m','p','g','a'), VLC_FOURCC('l','p','c','m'), VLC_FOURCC('a','5','2',' '),
                                VLC_FOURCC('d','t','s',' '), VLC_FOURCC('m','p','4','a'),
                                VLC_FOURCC('d','v','b','s'), VLC_FOURCC('t','e','l','x') ),

    M( NULL, NULL, 0, 0 )
};
#undef M

static int OutputNew( sout_stream_t *p_stream,
                      const char *psz_muxer, const char *psz_prefix, const char *psz_extension  )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;
    char *psz_output;
    int i_count;

    if( asprintf( &psz_output, "std{access=file,mux='%s',dst='%s%s%s'}",
                  psz_muxer, psz_prefix, psz_extension ? "." : "", psz_extension ? psz_extension : "" ) < 0 )
        return -1;

    /* Create the output */
    msg_Dbg( p_stream, "Using record output `%s'", psz_output );
    p_sys->p_out = sout_StreamNew( p_stream->p_sout, psz_output );

    free( psz_output );

    if( !p_sys->p_out )
        return -1;

    /* Add es */
    i_count = 0;
    for( int i = 0; i < p_sys->i_id; i++ )
    {
        sout_stream_id_t *id = p_sys->id[i];

        id->id = sout_StreamIdAdd( p_sys->p_out, &id->fmt );
        if( id->id )
            i_count++;
    }

    return i_count;
}

static void OutputStart( sout_stream_t *p_stream )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;

    /* */
    if( p_sys->b_drop )
        return;

    /* From now on drop packet that cannot be handled */
    p_sys->b_drop = true;

    /* Detect streams to smart select muxer */
    const char *psz_muxer = NULL;
    const char *psz_extension = NULL;

    /* Look for prefered muxer
     * TODO we could insert transcode in a few cases like
     * s16l <-> s16b
     */
    for( int i = 0; p_muxers[i].psz_muxer != NULL; i++ )
    {
        bool b_ok;
        if( p_sys->i_id > p_muxers[i].i_es_max )
            continue;

        b_ok = true;
        for( int j = 0; j < p_sys->i_id; j++ )
        {
            es_format_t *p_fmt = &p_sys->id[j]->fmt;

            b_ok = false;
            for( int k = 0; p_muxers[i].codec[k] != 0; k++ )
            {
                if( p_fmt->i_codec == p_muxers[i].codec[k] )
                {
                    b_ok = true;
                    break;
                }
            }
            if( !b_ok )
                break;
        }
        if( !b_ok )
            continue;

        psz_muxer = p_muxers[i].psz_muxer;
        psz_extension = p_muxers[i].psz_extension;
        break;
    }

    /* If failed, brute force our demuxers and select the one that
     * keeps most of our stream */
    if( !psz_muxer || !psz_extension )
    {
        static const char *ppsz_muxers[][2] = {
            { "avi", "avi" }, { "mp4", "mp4" }, { "ogg", "ogg" },
            { "asf", "asf" }, {  "ts",  "ts" }, {  "ps", "mpg" },
#if 0
            // XXX ffmpeg sefault really easily if you try an unsupported codec
            // mov and avi at least segfault
            { "ffmpeg{mux=avi}", "avi" },
            { "ffmpeg{mux=mov}", "mov" },
            { "ffmpeg{mux=mp4}", "mp4" },
            { "ffmpeg{mux=nsv}", "nsv" },
            { "ffmpeg{mux=flv}", "flv" },
#endif
            { NULL, NULL }
        };
        int i_best = 0;
        int i_best_es = 0;

        msg_Warn( p_stream, "failed to find an adequate muxer, probing muxers" );
        for( int i = 0; ppsz_muxers[i][0] != NULL; i++ )
        {
            char *psz_file;
            int i_es;

            psz_file = tempnam( NULL, "vlc" );
            if( !psz_file )
                continue;

            msg_Dbg( p_stream, "probing muxer %s", ppsz_muxers[i][0] );
            i_es = OutputNew( p_stream, ppsz_muxers[i][0], psz_file, NULL );

            if( i_es < 0 )
            {
                utf8_unlink( psz_file );
                free( psz_file );
                continue;
            }

            /* */
            for( int i = 0; i < p_sys->i_id; i++ )
            {
                sout_stream_id_t *id = p_sys->id[i];

                if( id->id )
                    sout_StreamIdDel( p_sys->p_out, id->id );
                id->id = NULL;
            }
            if( p_sys->p_out )
                sout_StreamDelete( p_sys->p_out );
            p_sys->p_out = NULL;

            if( i_es > i_best_es )
            {
                i_best_es = i_es;
                i_best = i;

                if( i_best_es >= p_sys->i_id )
                    break;
            }
            utf8_unlink( psz_file );
            free( psz_file );
        }

        /* */
        psz_muxer = ppsz_muxers[i_best][0];
        psz_extension = ppsz_muxers[i_best][1];
        msg_Dbg( p_stream, "using muxer %s with extension %s (%d/%d streams accepted)",
                 psz_muxer, psz_extension, i_best_es, p_sys->i_id );
    }

    /* Create the output */
    if( OutputNew( p_stream, psz_muxer, p_sys->psz_prefix, psz_extension ) < 0 )
    {
        msg_Err( p_stream, "failed to open output");
        return;
    }

    /* Compute highest timestamp of first I over all streams */
    p_sys->i_dts_start = 0;
    for( int i = 0; i < p_sys->i_id; i++ )
    {
        sout_stream_id_t *id = p_sys->id[i];
        block_t *p_block;

        if( !id->id || !id->p_first )
            continue;

        mtime_t i_dts = id->p_first->i_dts;
        for( p_block = id->p_first; p_block != NULL; p_block = p_block->p_next )
        {
            if( p_block->i_flags & BLOCK_FLAG_TYPE_I )
            {
                i_dts = p_block->i_dts;
                break;
            }
        }

        if( i_dts > p_sys->i_dts_start )
            p_sys->i_dts_start = i_dts;
    }

    /* Send buffered data */
    for( int i = 0; i < p_sys->i_id; i++ )
    {
        sout_stream_id_t *id = p_sys->id[i];

        if( !id->id )
            continue;

        block_t *p_block = id->p_first;
        while( p_block )
        {
            block_t *p_next = p_block->p_next;

            p_block->p_next = NULL;

            OutputSend( p_stream, id, p_block );

            p_block = p_next;
        }

        id->p_first = NULL;
        id->pp_last = &id->p_first;
    }
}

static void OutputSend( sout_stream_t *p_stream, sout_stream_id_t *id, block_t *p_block )
{
    sout_stream_sys_t *p_sys = p_stream->p_sys;

    if( id->id )
    {
        /* We wait until the first key frame (if needed) and
         * to be beyong i_dts_start (for stream without key frame) */
        if( id->b_wait_key )
        {
            if( p_block->i_flags & BLOCK_FLAG_TYPE_I )
            {
                id->b_wait_key = false;
                id->b_wait_start = false;
            }

            if( ( p_block->i_flags & BLOCK_FLAG_TYPE_MASK ) == 0 )
                id->b_wait_key = false;
        }
        if( id->b_wait_start )
        {
            if( p_block->i_dts >=p_sys->i_dts_start )
                id->b_wait_start = false;
        }
        if( id->b_wait_key || id->b_wait_start )
            block_ChainRelease( p_block );
        else
            sout_StreamIdSend( p_sys->p_out, id->id, p_block );
    }
    else if( p_sys->b_drop )
    {
        block_ChainRelease( p_block );
    }
    else
    {
        size_t i_size;

        block_ChainProperties( p_block, NULL, &i_size, NULL );
        p_sys->i_size += i_size;
        block_ChainLastAppend( &id->pp_last, p_block );
    }
}

