/*****************************************************************************
 * subtitle.c: Demux vobsub files.
 *****************************************************************************
 * Copyright (C) 1999-2004 VideoLAN
 * $Id: vobsub.c 9003 2004-10-17 13:38:22Z hartman $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Derk-Jan Hartman <hartman at videolan dot org>
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

#include <errno.h>
#include <sys/types.h>

#include <vlc/vlc.h>
#include <vlc/input.h>
#include "vlc_video.h"

#include "ps.h"

#define MAX_LINE 8192

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open ( vlc_object_t *p_this );
static void Close( vlc_object_t *p_this );

vlc_module_begin();
    set_description( _("Vobsub subtitles demux") );
    set_capability( "demux2", 1 );
    
    set_callbacks( Open, Close );

    add_shortcut( "vobsub" );
    add_shortcut( "subtitle" );
vlc_module_end();

/*****************************************************************************
 * Prototypes:
 *****************************************************************************/

typedef struct
{
    int     i_line_count;
    int     i_line;
    char    **line;
} text_t;
static int  TextLoad( text_t *, stream_t *s );
static void TextUnload( text_t * );

typedef struct
{
    int64_t i_start;
    int     i_vobsub_location;
} subtitle_t;

typedef struct
{
    es_format_t fmt;
    es_out_id_t *p_es;
    int         i_track_id;
    
    int         i_current_subtitle;
    int         i_subtitles;
    subtitle_t  *p_subtitles;
} vobsub_track_t;

struct demux_sys_t
{
    int64_t     i_next_demux_date;
    int64_t     i_length;

    text_t      txt;
    FILE        *p_vobsub_file;
    
    /* all tracks */
    int            i_tracks;
    vobsub_track_t *track;
    
    int         i_original_frame_width;
    int         i_original_frame_height;
};

static int Demux( demux_t * );
static int Control( demux_t *, int, va_list );

static int ParseVobSubIDX( demux_t * );
static int DemuxVobSub( demux_t *, block_t *);

/*****************************************************************************
 * Module initializer
 *****************************************************************************/
static int Open ( vlc_object_t *p_this )
{
    demux_t     *p_demux = (demux_t*)p_this;
    demux_sys_t *p_sys;
    char *psz_vobname, *s;
    int i_len;

    if( ( s = stream_ReadLine( p_demux->s ) ) != NULL )
    {
        if( !strcasestr( s, "# VobSub index file" ) )
        {
            msg_Dbg( p_demux, "this doesn't seem to be a vobsub file" );
            free( s );
            if( stream_Seek( p_demux->s, 0 ) )
            {
                msg_Warn( p_demux, "failed to rewind" );
            }
            return VLC_EGENERIC;
        }
        free( s );

    }
    else
    {
        msg_Dbg( p_demux, "could not read vobsub IDX file" );
        return VLC_EGENERIC;
    }

    p_demux->pf_demux = Demux;
    p_demux->pf_control = Control;
    p_demux->p_sys = p_sys = malloc( sizeof( demux_sys_t ) );
    p_sys->i_length = 0;
    p_sys->p_vobsub_file = NULL;
    p_sys->i_tracks = 0;
    p_sys->track = (vobsub_track_t *)malloc( sizeof( vobsub_track_t ) );
    p_sys->i_original_frame_width = -1;
    p_sys->i_original_frame_height = -1;

    /* Load the whole file */
    TextLoad( &p_sys->txt, p_demux->s );

    /* Parse it */
    ParseVobSubIDX( p_demux );

    /* Unload */
    TextUnload( &p_sys->txt );

    /* Find the total length of the vobsubs */
    if( p_sys->i_tracks > 0 )
    {
        int i;
        for( i = 0; i < p_sys->i_tracks; i++ )
        {
            if( p_sys->track[i].i_subtitles > 1 )
            {
                if( p_sys->track[i].p_subtitles[p_sys->track[i].i_subtitles-1].i_start > p_sys->i_length )
                    p_sys->i_length = (int64_t) p_sys->track[i].p_subtitles[p_sys->track[i].i_subtitles-1].i_start + ( 1 *1000 *1000 );
            }
        }
    }

    i_len = strlen( p_demux->psz_path );
    psz_vobname = strdup( p_demux->psz_path );

    strcpy( psz_vobname + i_len - 4, ".sub" );

    /* open file */
    if( !( p_sys->p_vobsub_file = fopen( psz_vobname, "rb" ) ) )
    {
        msg_Err( p_demux, "couldn't open .sub Vobsub file: %s",
                 psz_vobname );
        free( p_sys );
        free( psz_vobname );
        return VLC_EGENERIC;
    }
    free( psz_vobname );

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close: Close subtitle demux
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    demux_t *p_demux = (demux_t*)p_this;
    demux_sys_t *p_sys = p_demux->p_sys;

    /* Clean all subs from all tracks
    if( p_sys->subtitle )
        free( p_sys->subtitle );
*/
    if( p_sys->p_vobsub_file )
        fclose( p_sys->p_vobsub_file );

    free( p_sys );
}

/*****************************************************************************
 * Control:
 *****************************************************************************/
static int Control( demux_t *p_demux, int i_query, va_list args )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    int64_t *pi64, i64;
    int i;
    double *pf, f;

    switch( i_query )
    {
        case DEMUX_GET_LENGTH:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            *pi64 = (int64_t) p_sys->i_length;
            return VLC_SUCCESS;

        case DEMUX_GET_TIME:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            for( i = 0; i < p_sys->i_tracks; i++ )
            {
                vlc_bool_t b_selected;
                /* Check the ES is selected */
                es_out_Control( p_demux->out, ES_OUT_GET_ES_STATE,
                                p_sys->track[i].p_es, &b_selected );
                if( b_selected ) break;
            }
            if( i < p_sys->i_tracks && p_sys->track[i].i_current_subtitle < p_sys->track[i].i_subtitles )
            {
                *pi64 = p_sys->track[i].p_subtitles[p_sys->track[i].i_current_subtitle].i_start;
                return VLC_SUCCESS;
            }
            return VLC_EGENERIC;

        case DEMUX_SET_TIME:
            i64 = (int64_t)va_arg( args, int64_t );
            for( i = 0; i < p_sys->i_tracks; i++ )
            {
                p_sys->track[i].i_current_subtitle = 0;
                while( p_sys->track[i].i_current_subtitle < p_sys->track[i].i_subtitles &&
                       p_sys->track[i].p_subtitles[p_sys->track[i].i_current_subtitle].i_start < i64 )
                {
                    p_sys->track[i].i_current_subtitle++;
                }

                if( p_sys->track[i].i_current_subtitle >= p_sys->track[i].i_subtitles )
                    return VLC_EGENERIC;
            }
            return VLC_SUCCESS;

        case DEMUX_GET_POSITION:
            pf = (double*)va_arg( args, double * );
            for( i = 0; i < p_sys->i_tracks; i++ )
            {
                vlc_bool_t b_selected;
                /* Check the ES is selected */
                es_out_Control( p_demux->out, ES_OUT_GET_ES_STATE,
                                p_sys->track[i].p_es, &b_selected );
                if( b_selected ) break;
            }
            if( p_sys->track[i].i_current_subtitle >= p_sys->track[i].i_subtitles )
            {
                *pf = 1.0;
            }
            else if( p_sys->track[i].i_subtitles > 0 )
            {
                *pf = (double)p_sys->track[i].p_subtitles[p_sys->track[i].i_current_subtitle].i_start /
                      (double)p_sys->i_length;
            }
            else
            {
                *pf = 0.0;
            }
            return VLC_SUCCESS;

        case DEMUX_SET_POSITION:
            f = (double)va_arg( args, double );
            i64 = (int64_t) f * p_sys->i_length;

            for( i = 0; i < p_sys->i_tracks; i++ )
            {
                p_sys->track[i].i_current_subtitle = 0;
                while( p_sys->track[i].i_current_subtitle < p_sys->track[i].i_subtitles &&
                       p_sys->track[i].p_subtitles[p_sys->track[i].i_current_subtitle].i_start < i64 )
                {
                    p_sys->track[i].i_current_subtitle++;
                }
                if( p_sys->track[i].i_current_subtitle >= p_sys->track[i].i_subtitles )
                    return VLC_EGENERIC;
            }
            return VLC_SUCCESS;

        case DEMUX_SET_NEXT_DEMUX_TIME:
            p_sys->i_next_demux_date = (int64_t)va_arg( args, int64_t );
            return VLC_SUCCESS;

        case DEMUX_GET_FPS:
        case DEMUX_GET_META:
        case DEMUX_GET_TITLE_INFO:
            return VLC_EGENERIC;

        default:
            msg_Err( p_demux, "unknown query in subtitle control" );
            return VLC_EGENERIC;
    }
}

/*****************************************************************************
 * Demux: Send subtitle to decoder
 *****************************************************************************/
static int Demux( demux_t *p_demux )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    int64_t i_maxdate;
    int i;

    for( i = 0; i < p_sys->i_tracks; i++ )
    {
#define tk p_sys->track[i]
        if( tk.i_current_subtitle >= tk.i_subtitles )
            continue;

        i_maxdate = (int64_t) p_sys->i_next_demux_date;
        if( i_maxdate <= 0 && tk.i_current_subtitle < tk.i_subtitles )
        {
            /* Should not happen */
            i_maxdate = (int64_t) tk.p_subtitles[tk.i_current_subtitle].i_start + 1;
        }

        while( tk.i_current_subtitle < tk.i_subtitles &&
               tk.p_subtitles[tk.i_current_subtitle].i_start < i_maxdate )
        {
            int i_pos = tk.p_subtitles[tk.i_current_subtitle].i_vobsub_location;
            block_t *p_block;
            int i_size = 0;

            /* first compute SPU size */
            if( tk.i_current_subtitle + 1 < tk.i_subtitles )
            {
                i_size = tk.p_subtitles[tk.i_current_subtitle+1].i_vobsub_location - i_pos;
            }
            if( i_size <= 0 ) i_size = 65535;   /* Invalid or EOF */

            /* Seek at the right place */
            if( fseek( p_sys->p_vobsub_file, i_pos, SEEK_SET ) )
            {
                msg_Warn( p_demux,
                          "cannot seek at right vobsub location %d", i_pos );
                tk.i_current_subtitle++;
                continue;
            }

            /* allocate a packet */
            if( ( p_block = block_New( p_demux, i_size ) ) == NULL )
            {
                tk.i_current_subtitle++;
                continue;
            }

            /* read data */
            p_block->i_buffer = fread( p_block->p_buffer, 1, i_size,
                                       p_sys->p_vobsub_file );
            if( p_block->i_buffer <= 6 )
            {
                block_Release( p_block );
                tk.i_current_subtitle++;
                continue;
            }

            /* pts */
            p_block->i_pts = tk.p_subtitles[tk.i_current_subtitle].i_start;

            /* demux this block */
            DemuxVobSub( p_demux, p_block );

            tk.i_current_subtitle++;
        }
#undef tk
    }

    /* */
    p_sys->i_next_demux_date = 0;

    return 1;
}

static int TextLoad( text_t *txt, stream_t *s )
{
    int   i_line_max;

    /* init txt */
    i_line_max          = 500;
    txt->i_line_count   = 0;
    txt->i_line         = 0;
    txt->line           = calloc( i_line_max, sizeof( char * ) );

    /* load the complete file */
    for( ;; )
    {
        char *psz = stream_ReadLine( s );

        if( psz == NULL )
            break;

        txt->line[txt->i_line_count++] = psz;
        if( txt->i_line_count >= i_line_max )
        {
            i_line_max += 100;
            txt->line = realloc( txt->line, i_line_max * sizeof( char*) );
        }
    }

    if( txt->i_line_count <= 0 )
    {
        free( txt->line );
        return VLC_EGENERIC;
    }

    return VLC_SUCCESS;
}
static void TextUnload( text_t *txt )
{
    int i;

    for( i = 0; i < txt->i_line_count; i++ )
    {
        free( txt->line[i] );
    }
    free( txt->line );
    txt->i_line       = 0;
    txt->i_line_count = 0;
}

static char *TextGetLine( text_t *txt )
{
    if( txt->i_line >= txt->i_line_count )
        return( NULL );

    return txt->line[txt->i_line++];
}

static int ParseVobSubIDX( demux_t *p_demux )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    text_t      *txt = &p_sys->txt;
    char        *line;
    vobsub_track_t *current_tk;

    for( ;; )
    {
        if( ( line = TextGetLine( txt ) ) == NULL )
        {
            return( VLC_EGENERIC );
        }
        
        if( *line == 0 || *line == '\r' || *line == '\n' || *line == '#' ) 
            continue;
        else if( !strncmp( "size:", line, 5 ) )
        {
            /* Store the original size of the video */
            if( sscanf( line, "size: %dx%d",
                        &p_sys->i_original_frame_width, &p_sys->i_original_frame_height ) == 2 )
            {
                msg_Dbg( p_demux, "original frame size: %dx%d", p_sys->i_original_frame_width, p_sys->i_original_frame_height );
            }
            else
            {
                msg_Warn( p_demux, "reading original frame size failed" );
            }
        }
        else if( !strncmp( "id:", line, 3 ) )
        {
            char language[20];
            int i_track_id;
            es_format_t fmt;

            /* Lets start a new track */
            if( sscanf( line, "id: %2s, index: %d",
                        language, &i_track_id ) == 2 )
            {
                p_sys->i_tracks++;
                p_sys->track = (vobsub_track_t*)realloc( p_sys->track, sizeof( vobsub_track_t ) * (p_sys->i_tracks + 1 ) );

                /* Init the track */
                current_tk = &p_sys->track[p_sys->i_tracks - 1];
                memset( current_tk, 0, sizeof( vobsub_track_t ) );
                current_tk->i_current_subtitle = 0;
                current_tk->i_subtitles = 0;
                current_tk->p_subtitles = (subtitle_t*)malloc( sizeof( subtitle_t ) );;
                current_tk->i_track_id = i_track_id;

                es_format_Init( &fmt, SPU_ES, VLC_FOURCC( 's','p','u',' ' ) );
                fmt.subs.spu.i_original_frame_width = p_sys->i_original_frame_width;
                fmt.subs.spu.i_original_frame_height = p_sys->i_original_frame_height;
                fmt.psz_language = strdup( language );
                current_tk->p_es = es_out_Add( p_demux->out, &fmt );

                msg_Dbg( p_demux, "new vobsub track detected" );
            }
            else
            {
                msg_Warn( p_demux, "reading new track failed" );
            }
        }
        else if( !strncmp( line, "timestamp:", 10 ) )
        {
            /*
             * timestamp: hh:mm:ss:mss, filepos: loc
             * loc is the hex location of the spu in the .sub file
             *
             */
            int h, m, s, ms, loc;
            int64_t i_start, i_location = 0;
            
            vobsub_track_t *current_tk = &p_sys->track[p_sys->i_tracks - 1];

            if( sscanf( line, "timestamp: %d:%d:%d:%d, filepos: %x",
                        &h, &m, &s, &ms, &loc ) == 5 )
            {
                subtitle_t *current_sub;
                
                i_start = (int64_t) ( h * 3600*1000 +
                            m * 60*1000 +
                            s * 1000 +
                            ms ) * 1000;
                i_location = loc;
                
                current_tk->i_subtitles++;
                current_tk->p_subtitles = (subtitle_t*)realloc( current_tk->p_subtitles, sizeof( subtitle_t ) * (current_tk->i_subtitles + 1 ) );
                current_sub = &current_tk->p_subtitles[current_tk->i_subtitles - 1];
                
                current_sub->i_start = (int64_t) i_start;
                current_sub->i_vobsub_location = i_location;
            }
        }
    }
    return( 0 );
}

static int DemuxVobSub( demux_t *p_demux, block_t *p_bk )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    uint8_t     *p = p_bk->p_buffer;
    uint8_t     *p_end = &p_bk->p_buffer[p_bk->i_buffer];
    int i;

    while( p < p_end )
    {
        int i_size = ps_pkt_size( p, p_end - p );
        block_t *p_pkt;
        int      i_id;
        int      i_spu;

        if( i_size <= 0 )
        {
            break;
        }
        if( p[0] != 0 || p[1] != 0 || p[2] != 0x01 )
        {
            msg_Warn( p_demux, "invalid PES" );
            break;
        }

        if( p[3] != 0xbd )
        {
            /* msg_Dbg( p_demux, "we don't need these ps packets (id=0x1%2.2x)", p[3] ); */
            p += i_size;
            continue;
        }

        /* Create a block */
        p_pkt = block_New( p_demux, i_size );
        memcpy( p_pkt->p_buffer, p, i_size);
        p += i_size;

        i_id = ps_pkt_id( p_pkt );
        if( (i_id&0xffe0) != 0xbd20 ||
            ps_pkt_parse_pes( p_pkt, 1 ) )
        {
            block_Release( p_pkt );
            continue;
        }
        i_spu = i_id&0x1f;
        /* msg_Dbg( p_demux, "SPU track %d size %d", i_spu, i_size ); */

        for( i = 0; i < p_sys->i_tracks; i++ )
        {
#define tk p_sys->track[i]
            p_pkt->i_dts = p_pkt->i_pts = p_bk->i_pts;
            p_pkt->i_length = 0;
            
            if( tk.p_es && tk.i_track_id == i_spu )
            {
                es_out_Send( p_demux->out, tk.p_es, p_pkt );
                p_bk->i_pts = 0;     /*only first packet has a pts */
                break;
            }
            else if( i == p_sys->i_tracks - 1 )
            {
                block_Release( p_pkt );
            }
#undef tk
        }
    }

    return VLC_SUCCESS;
}
