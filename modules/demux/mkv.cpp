/*****************************************************************************
 * mkv.cpp : matroska demuxer
 *****************************************************************************
 * Copyright (C) 2003-2004 VideoLAN
 * $Id: mkv.cpp 8892 2004-10-02 20:07:35Z hartman $
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
#include <stdlib.h>                                      /* malloc(), free() */

#include <vlc/vlc.h>

#ifdef HAVE_TIME_H
#   include <time.h>                                               /* time() */
#endif

#include <vlc/input.h>

#include <codecs.h>                        /* BITMAPINFOHEADER, WAVEFORMATEX */
#include "iso_lang.h"
#include "vlc_meta.h"

#include <iostream>
#include <cassert>
#include <typeinfo>

/* libebml and matroska */
#include "ebml/EbmlHead.h"
#include "ebml/EbmlSubHead.h"
#include "ebml/EbmlStream.h"
#include "ebml/EbmlContexts.h"
#include "ebml/EbmlVersion.h"
#include "ebml/EbmlVoid.h"

#include "matroska/FileKax.h"
#include "matroska/KaxAttachments.h"
#include "matroska/KaxBlock.h"
#include "matroska/KaxBlockData.h"
#include "matroska/KaxChapters.h"
#include "matroska/KaxCluster.h"
#include "matroska/KaxClusterData.h"
#include "matroska/KaxContexts.h"
#include "matroska/KaxCues.h"
#include "matroska/KaxCuesData.h"
#include "matroska/KaxInfo.h"
#include "matroska/KaxInfoData.h"
#include "matroska/KaxSeekHead.h"
#include "matroska/KaxSegment.h"
#include "matroska/KaxTag.h"
#include "matroska/KaxTags.h"
#include "matroska/KaxTagMulti.h"
#include "matroska/KaxTracks.h"
#include "matroska/KaxTrackAudio.h"
#include "matroska/KaxTrackVideo.h"
#include "matroska/KaxTrackEntryData.h"
#include "matroska/KaxContentEncoding.h"

#include "ebml/StdIOCallback.h"

extern "C" {
   #include "mp4/libmp4.h"
}
#ifdef HAVE_ZLIB_H
#   include <zlib.h>
#endif

#define MATROSKA_COMPRESSION_NONE 0
#define MATROSKA_COMPRESSION_ZLIB 1

using namespace LIBMATROSKA_NAMESPACE;
using namespace std;

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open ( vlc_object_t * );
static void Close( vlc_object_t * );

vlc_module_begin();
    set_description( _("Matroska stream demuxer" ) );
    set_capability( "demux2", 50 );
    set_callbacks( Open, Close );

    add_bool( "mkv-seek-percent", 1, NULL,
            N_("Seek based on percent not time"),
            N_("Seek based on percent not time"), VLC_TRUE );

    add_shortcut( "mka" );
    add_shortcut( "mkv" );
vlc_module_end();

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int  Demux  ( demux_t * );
static int  Control( demux_t *, int, va_list );
static void Seek   ( demux_t *, mtime_t i_date, int i_percent );

#ifdef HAVE_ZLIB_H
block_t *block_zlib_decompress( vlc_object_t *p_this, block_t *p_in_block ) {
    int result, dstsize, n;
    unsigned char *dst;
    block_t *p_block;
    z_stream d_stream;

    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    result = inflateInit(&d_stream);
    if( result != Z_OK )
    {
        msg_Dbg( p_this, "inflateInit() failed. Result: %d", result );
        return NULL;
    }

    d_stream.next_in = (Bytef *)p_in_block->p_buffer;
    d_stream.avail_in = p_in_block->i_buffer;
    n = 0;
    p_block = block_New( p_this, 0 );
    dst = NULL;
    do
    {
        n++;
        p_block = block_Realloc( p_block, 0, n * 1000 );
        dst = (unsigned char *)p_block->p_buffer;
        d_stream.next_out = (Bytef *)&dst[(n - 1) * 1000];
        d_stream.avail_out = 1000;
        result = inflate(&d_stream, Z_NO_FLUSH);
        if( ( result != Z_OK ) && ( result != Z_STREAM_END ) )
        {
            msg_Dbg( p_this, "Zlib decompression failed. Result: %d", result );
            return NULL;
        }
    }
    while( ( d_stream.avail_out == 0 ) && ( d_stream.avail_in != 0 ) &&
           ( result != Z_STREAM_END ) );

    dstsize = d_stream.total_out;
    inflateEnd( &d_stream );

    p_block = block_Realloc( p_block, 0, dstsize );
    p_block->i_buffer = dstsize;
    block_Release( p_in_block );

    return p_block;
}
#endif

/**
 * Helper function to print the mkv parse tree
 */
static void MkvTree( demux_t *p_this, int i_level, char *psz_format, ... )
{
    va_list args;
    if( i_level > 9 )
    {
        msg_Err( p_this, "too deep tree" );
        return;
    }
    va_start( args, psz_format );
    static char *psz_foo = "|   |   |   |   |   |   |   |   |   |";
    char *psz_foo2 = (char*)malloc( ( i_level * 4 + 3 + strlen( psz_format ) ) * sizeof(char) );
    strncpy( psz_foo2, psz_foo, 4 * i_level );
    psz_foo2[ 4 * i_level ] = '+';
    psz_foo2[ 4 * i_level + 1 ] = ' ';
    strcpy( &psz_foo2[ 4 * i_level + 2 ], psz_format );
    __msg_GenericVa( VLC_OBJECT(p_this), VLC_MSG_DBG, "mkv", psz_foo2, args );
    free( psz_foo2 );
    va_end( args );
}
    
/*****************************************************************************
 * Stream managment
 *****************************************************************************/
class vlc_stream_io_callback: public IOCallback
{
  private:
    stream_t       *s;
    vlc_bool_t     mb_eof;

  public:
    vlc_stream_io_callback( stream_t * );

    virtual uint32   read            ( void *p_buffer, size_t i_size);
    virtual void     setFilePointer  ( int64_t i_offset, seek_mode mode = seek_beginning );
    virtual size_t   write           ( const void *p_buffer, size_t i_size);
    virtual uint64   getFilePointer  ( void );
    virtual void     close           ( void );
};

/*****************************************************************************
 * Ebml Stream parser
 *****************************************************************************/
class EbmlParser
{
  public:
    EbmlParser( EbmlStream *es, EbmlElement *el_start );
    ~EbmlParser( void );

    void Up( void );
    void Down( void );
    EbmlElement *Get( void );
    void        Keep( void );

    int GetLevel( void );

  private:
    EbmlStream  *m_es;
    int         mi_level;
    EbmlElement *m_el[10];

    EbmlElement *m_got;

    int         mi_user_level;
    vlc_bool_t  mb_keep;
};


/*****************************************************************************
 * Some functions to manipulate memory
 *****************************************************************************/
#define GetFOURCC( p )  __GetFOURCC( (uint8_t*)p )
static vlc_fourcc_t __GetFOURCC( uint8_t *p )
{
    return VLC_FOURCC( p[0], p[1], p[2], p[3] );
}

/*****************************************************************************
 * definitions of structures and functions used by this plugins
 *****************************************************************************/
typedef struct
{
    vlc_bool_t  b_default;
    vlc_bool_t  b_enabled;
    int         i_number;

    int         i_extra_data;
    uint8_t     *p_extra_data;

    char         *psz_codec;

    uint64_t     i_default_duration;
    float        f_timecodescale;

    /* video */
    es_format_t fmt;
    float       f_fps;
    es_out_id_t *p_es;

    vlc_bool_t      b_inited;
    /* data to be send first */
    int             i_data_init;
    uint8_t         *p_data_init;

    /* hack : it's for seek */
    vlc_bool_t      b_search_keyframe;

    /* informative */
    char         *psz_codec_name;
    char         *psz_codec_settings;
    char         *psz_codec_info_url;
    char         *psz_codec_download_url;
    
    /* encryption/compression */
    int           i_compression_type;

} mkv_track_t;

typedef struct
{
    int     i_track;
    int     i_block_number;

    int64_t i_position;
    int64_t i_time;

    vlc_bool_t b_key;
} mkv_index_t;

struct demux_sys_t
{
    vlc_stream_io_callback  *in;
    EbmlStream              *es;
    EbmlParser              *ep;

    /* time scale */
    uint64_t                i_timescale;

    /* duration of the segment */
    float                   f_duration;

    /* all tracks */
    int                     i_track;
    mkv_track_t             *track;

    /* from seekhead */
    int64_t                 i_cues_position;
    int64_t                 i_chapters_position;
    int64_t                 i_tags_position;

    /* current data */
    KaxSegment              *segment;
    KaxCluster              *cluster;

    mtime_t                 i_pts;

    vlc_bool_t              b_cues;
    int                     i_index;
    int                     i_index_max;
    mkv_index_t             *index;

    /* info */
    char                    *psz_muxing_application;
    char                    *psz_writing_application;
    char                    *psz_segment_filename;
    char                    *psz_title;
    char                    *psz_date_utc;

    vlc_meta_t              *meta;

    input_title_t           *title;
};

#define MKVD_TIMECODESCALE 1000000

#define MKV_IS_ID( el, C ) ( EbmlId( (*el) ) == C::ClassInfos.GlobalId )

static void IndexAppendCluster  ( demux_t *p_demux, KaxCluster *cluster );
static char *UTF8ToStr          ( const UTFstring &u );
static void LoadCues            ( demux_t * );
static void InformationCreate  ( demux_t * );

static void ParseInfo( demux_t *, EbmlElement *info );
static void ParseTracks( demux_t *, EbmlElement *tracks );
static void ParseSeekHead( demux_t *, EbmlElement *seekhead );
static void ParseChapters( demux_t *, EbmlElement *chapters );

/*****************************************************************************
 * Open: initializes matroska demux structures
 *****************************************************************************/
static int Open( vlc_object_t * p_this )
{
    demux_t     *p_demux = (demux_t*)p_this;
    demux_sys_t *p_sys;
    uint8_t     *p_peek;

    int          i_track;

    EbmlElement *el = NULL, *el1 = NULL;

    /* peek the begining */
    if( stream_Peek( p_demux->s, &p_peek, 4 ) < 4 )
    {
        msg_Warn( p_demux, "cannot peek" );
        return VLC_EGENERIC;
    }

    /* is a valid file */
    if( p_peek[0] != 0x1a || p_peek[1] != 0x45 ||
        p_peek[2] != 0xdf || p_peek[3] != 0xa3 )
    {
        msg_Warn( p_demux, "matroska module discarded "
                           "(invalid header 0x%.2x%.2x%.2x%.2x)",
                           p_peek[0], p_peek[1], p_peek[2], p_peek[3] );
        return VLC_EGENERIC;
    }

    /* Set the demux function */
    p_demux->pf_demux   = Demux;
    p_demux->pf_control = Control;
    p_demux->p_sys      = p_sys = (demux_sys_t*)malloc(sizeof( demux_sys_t ));

    memset( p_sys, 0, sizeof( demux_sys_t ) );
    p_sys->in = new vlc_stream_io_callback( p_demux->s );
    p_sys->es = new EbmlStream( *p_sys->in );
    p_sys->f_duration   = -1;
    p_sys->i_timescale     = MKVD_TIMECODESCALE;
    p_sys->i_track      = 0;
    p_sys->track        = (mkv_track_t*)malloc( sizeof( mkv_track_t ) );
    p_sys->i_pts   = 0;
    p_sys->i_cues_position = -1;
    p_sys->i_chapters_position = -1;
    p_sys->i_tags_position = -1;

    p_sys->b_cues       = VLC_FALSE;
    p_sys->i_index      = 0;
    p_sys->i_index_max  = 1024;
    p_sys->index        = (mkv_index_t*)malloc( sizeof( mkv_index_t ) *
                                                p_sys->i_index_max );

    p_sys->psz_muxing_application = NULL;
    p_sys->psz_writing_application = NULL;
    p_sys->psz_segment_filename = NULL;
    p_sys->psz_title = NULL;
    p_sys->psz_date_utc = NULL;;
    p_sys->meta = NULL;
    p_sys->title = NULL;

    if( p_sys->es == NULL )
    {
        msg_Err( p_demux, "failed to create EbmlStream" );
        delete p_sys->in;
        free( p_sys );
        return VLC_EGENERIC;
    }
    /* Find the EbmlHead element */
    el = p_sys->es->FindNextID(EbmlHead::ClassInfos, 0xFFFFFFFFL);
    if( el == NULL )
    {
        msg_Err( p_demux, "cannot find EbmlHead" );
        goto error;
    }
    msg_Dbg( p_demux, "EbmlHead" );
    /* skip it */
    el->SkipData( *p_sys->es, el->Generic().Context );
    delete el;

    /* Find a segment */
    el = p_sys->es->FindNextID( KaxSegment::ClassInfos, 0xFFFFFFFFL);
    if( el == NULL )
    {
        msg_Err( p_demux, "cannot find KaxSegment" );
        goto error;
    }
    MkvTree( p_demux, 0, "Segment" );
    p_sys->segment = (KaxSegment*)el;
    p_sys->cluster = NULL;

    p_sys->ep = new EbmlParser( p_sys->es, el );

    while( ( el1 = p_sys->ep->Get() ) != NULL )
    {
        if( MKV_IS_ID( el1, KaxInfo ) )
        {
            ParseInfo( p_demux, el1 );
        }
        else if( MKV_IS_ID( el1, KaxTracks ) )
        {
            ParseTracks( p_demux, el1 );
        }
        else if( MKV_IS_ID( el1, KaxSeekHead ) )
        {
            ParseSeekHead( p_demux, el1 );
        }
        else if( MKV_IS_ID( el1, KaxCues ) )
        {
            msg_Dbg( p_demux, "|   + Cues" );
        }
        else if( MKV_IS_ID( el1, KaxCluster ) )
        {
            msg_Dbg( p_demux, "|   + Cluster" );

            p_sys->cluster = (KaxCluster*)el1;

            p_sys->ep->Down();
            /* stop parsing the stream */
            break;
        }
        else if( MKV_IS_ID( el1, KaxAttachments ) )
        {
            msg_Dbg( p_demux, "|   + Attachments FIXME TODO (but probably never supported)" );
        }
        else if( MKV_IS_ID( el1, KaxChapters ) )
        {
            msg_Dbg( p_demux, "|   + Chapters" );
            ParseChapters( p_demux, el1 );
        }
        else if( MKV_IS_ID( el1, KaxTag ) )
        {
            msg_Dbg( p_demux, "|   + Tags FIXME TODO" );
        }
        else
        {
            msg_Dbg( p_demux, "|   + Unknown (%s)", typeid(*el1).name() );
        }
    }

    if( p_sys->cluster == NULL )
    {
        msg_Err( p_demux, "cannot find any cluster, damaged file ?" );
        goto error;
    }

    /* *** Load the cue if found *** */
    if( p_sys->i_cues_position >= 0 )
    {
        vlc_bool_t b_seekable;

        stream_Control( p_demux->s, STREAM_CAN_FASTSEEK, &b_seekable );
        if( b_seekable )
        {
            LoadCues( p_demux );
        }
    }

    if( !p_sys->b_cues || p_sys->i_index <= 0 )
    {
        msg_Warn( p_demux, "no cues/empty cues found->seek won't be precise" );

        IndexAppendCluster( p_demux, p_sys->cluster );

        p_sys->b_cues = VLC_FALSE;
    }

    /* add all es */
    msg_Dbg( p_demux, "found %d es", p_sys->i_track );
    for( i_track = 0; i_track < p_sys->i_track; i_track++ )
    {
#define tk  p_sys->track[i_track]
        if( tk.fmt.i_cat == UNKNOWN_ES )
        {
            msg_Warn( p_demux, "invalid track[%d, n=%d]", i_track, tk.i_number );
            tk.p_es = NULL;
            continue;
        }

        if( !strcmp( tk.psz_codec, "V_MS/VFW/FOURCC" ) )
        {
            if( tk.i_extra_data < (int)sizeof( BITMAPINFOHEADER ) )
            {
                msg_Err( p_demux, "missing/invalid BITMAPINFOHEADER" );
                tk.fmt.i_codec = VLC_FOURCC( 'u', 'n', 'd', 'f' );
            }
            else
            {
                BITMAPINFOHEADER *p_bih = (BITMAPINFOHEADER*)tk.p_extra_data;

                tk.fmt.video.i_width = GetDWLE( &p_bih->biWidth );
                tk.fmt.video.i_height= GetDWLE( &p_bih->biHeight );
                tk.fmt.i_codec       = GetFOURCC( &p_bih->biCompression );

                tk.fmt.i_extra       = GetDWLE( &p_bih->biSize ) - sizeof( BITMAPINFOHEADER );
                if( tk.fmt.i_extra > 0 )
                {
                    tk.fmt.p_extra = malloc( tk.fmt.i_extra );
                    memcpy( tk.fmt.p_extra, &p_bih[1], tk.fmt.i_extra );
                }
            }
        }
        else if( !strcmp( tk.psz_codec, "V_MPEG1" ) ||
                 !strcmp( tk.psz_codec, "V_MPEG2" ) )
        {
            tk.fmt.i_codec = VLC_FOURCC( 'm', 'p', 'g', 'v' );
        }
        else if( !strncmp( tk.psz_codec, "V_MPEG4", 7 ) )
        {
            if( !strcmp( tk.psz_codec, "V_MPEG4/MS/V3" ) )
            {
                tk.fmt.i_codec = VLC_FOURCC( 'D', 'I', 'V', '3' );
            }
            else
            {
                tk.fmt.i_codec = VLC_FOURCC( 'm', 'p', '4', 'v' );
            }
        }
        else if( !strcmp( tk.psz_codec, "V_QUICKTIME" ) )
        {
            MP4_Box_t *p_box = (MP4_Box_t*)malloc( sizeof( MP4_Box_t ) );
            MP4_Stream_t *p_mp4_stream = MP4_MemoryStream( p_demux->s,
                                                           tk.i_extra_data,
                                                           tk.p_extra_data );
            MP4_ReadBoxCommon( p_mp4_stream, p_box );
            MP4_ReadBox_sample_vide( p_mp4_stream, p_box );
            tk.fmt.i_codec = p_box->i_type;
            tk.fmt.video.i_width = p_box->data.p_sample_vide->i_width;
            tk.fmt.video.i_height = p_box->data.p_sample_vide->i_height;
            tk.fmt.i_extra = p_box->data.p_sample_vide->i_qt_image_description;
            tk.fmt.p_extra = malloc( tk.fmt.i_extra );
            memcpy( tk.fmt.p_extra, p_box->data.p_sample_vide->p_qt_image_description, tk.fmt.i_extra );
            MP4_FreeBox_sample_vide( p_box );
            free( p_box );
            free( p_mp4_stream );
        }
        else if( !strcmp( tk.psz_codec, "A_MS/ACM" ) )
        {
            if( tk.i_extra_data < (int)sizeof( WAVEFORMATEX ) )
            {
                msg_Err( p_demux, "missing/invalid WAVEFORMATEX" );
                tk.fmt.i_codec = VLC_FOURCC( 'u', 'n', 'd', 'f' );
            }
            else
            {
                WAVEFORMATEX *p_wf = (WAVEFORMATEX*)tk.p_extra_data;

                wf_tag_to_fourcc( GetWLE( &p_wf->wFormatTag ), &tk.fmt.i_codec, NULL );

                tk.fmt.audio.i_channels   = GetWLE( &p_wf->nChannels );
                tk.fmt.audio.i_rate = GetDWLE( &p_wf->nSamplesPerSec );
                tk.fmt.i_bitrate    = GetDWLE( &p_wf->nAvgBytesPerSec ) * 8;
                tk.fmt.audio.i_blockalign = GetWLE( &p_wf->nBlockAlign );;
                tk.fmt.audio.i_bitspersample = GetWLE( &p_wf->wBitsPerSample );

                tk.fmt.i_extra            = GetWLE( &p_wf->cbSize );
                if( tk.fmt.i_extra > 0 )
                {
                    tk.fmt.p_extra = malloc( tk.fmt.i_extra );
                    memcpy( tk.fmt.p_extra, &p_wf[1], tk.fmt.i_extra );
                }
            }
        }
        else if( !strcmp( tk.psz_codec, "A_MPEG/L3" ) ||
                 !strcmp( tk.psz_codec, "A_MPEG/L2" ) ||
                 !strcmp( tk.psz_codec, "A_MPEG/L1" ) )
        {
            tk.fmt.i_codec = VLC_FOURCC( 'm', 'p', 'g', 'a' );
        }
        else if( !strcmp( tk.psz_codec, "A_AC3" ) )
        {
            tk.fmt.i_codec = VLC_FOURCC( 'a', '5', '2', ' ' );
        }
        else if( !strcmp( tk.psz_codec, "A_DTS" ) )
        {
            tk.fmt.i_codec = VLC_FOURCC( 'd', 't', 's', ' ' );
        }
        else if( !strcmp( tk.psz_codec, "A_FLAC" ) )
        {
            tk.fmt.i_codec = VLC_FOURCC( 'f', 'l', 'a', 'c' );
            tk.fmt.i_extra = tk.i_extra_data;
            tk.fmt.p_extra = malloc( tk.i_extra_data );
            memcpy( tk.fmt.p_extra,tk.p_extra_data, tk.i_extra_data );
        }
        else if( !strcmp( tk.psz_codec, "A_VORBIS" ) )
        {
            int i, i_offset = 1, i_size[3], i_extra;
            uint8_t *p_extra;

            tk.fmt.i_codec = VLC_FOURCC( 'v', 'o', 'r', 'b' );

            /* Split the 3 headers */
            if( tk.p_extra_data[0] != 0x02 )
                msg_Err( p_demux, "invalid vorbis header" );

            for( i = 0; i < 2; i++ )
            {
                i_size[i] = 0;
                while( i_offset < tk.i_extra_data )
                {
                    i_size[i] += tk.p_extra_data[i_offset];
                    if( tk.p_extra_data[i_offset++] != 0xff ) break;
                }
            }

            i_size[0] = __MIN(i_size[0], tk.i_extra_data - i_offset);
            i_size[1] = __MIN(i_size[1], tk.i_extra_data -i_offset -i_size[0]);
            i_size[2] = tk.i_extra_data - i_offset - i_size[0] - i_size[1];

            tk.fmt.i_extra = 3 * 2 + i_size[0] + i_size[1] + i_size[2];
            tk.fmt.p_extra = malloc( tk.fmt.i_extra );
            p_extra = (uint8_t *)tk.fmt.p_extra; i_extra = 0;
            for( i = 0; i < 3; i++ )
            {
                *(p_extra++) = i_size[i] >> 8;
                *(p_extra++) = i_size[i] & 0xFF;
                memcpy( p_extra, tk.p_extra_data + i_offset + i_extra,
                        i_size[i] );
                p_extra += i_size[i];
                i_extra += i_size[i];
            }
        }
        else if( !strncmp( tk.psz_codec, "A_AAC/MPEG2/", strlen( "A_AAC/MPEG2/" ) ) ||
                 !strncmp( tk.psz_codec, "A_AAC/MPEG4/", strlen( "A_AAC/MPEG4/" ) ) )
        {
            int i_profile, i_srate;
            static unsigned int i_sample_rates[] =
            {
                    96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050,
                        16000, 12000, 11025, 8000,  7350,  0,     0,     0
            };

            tk.fmt.i_codec = VLC_FOURCC( 'm', 'p', '4', 'a' );
            /* create data for faad (MP4DecSpecificDescrTag)*/

            if( !strcmp( &tk.psz_codec[12], "MAIN" ) )
            {
                i_profile = 0;
            }
            else if( !strcmp( &tk.psz_codec[12], "LC" ) )
            {
                i_profile = 1;
            }
            else if( !strcmp( &tk.psz_codec[12], "SSR" ) )
            {
                i_profile = 2;
            }
            else
            {
                i_profile = 3;
            }

            for( i_srate = 0; i_srate < 13; i_srate++ )
            {
                if( i_sample_rates[i_srate] == tk.fmt.audio.i_rate )
                {
                    break;
                }
            }
            msg_Dbg( p_demux, "profile=%d srate=%d", i_profile, i_srate );

            tk.fmt.i_extra = 2;
            tk.fmt.p_extra = malloc( tk.fmt.i_extra );
            ((uint8_t*)tk.fmt.p_extra)[0] = ((i_profile + 1) << 3) | ((i_srate&0xe) >> 1);
            ((uint8_t*)tk.fmt.p_extra)[1] = ((i_srate & 0x1) << 7) | (tk.fmt.audio.i_channels << 3);
        }
        else if( !strcmp( tk.psz_codec, "A_PCM/INT/BIG" ) ||
                 !strcmp( tk.psz_codec, "A_PCM/INT/LIT" ) ||
                 !strcmp( tk.psz_codec, "A_PCM/FLOAT/IEEE" ) )
        {
            if( !strcmp( tk.psz_codec, "A_PCM/INT/BIG" ) )
            {
                tk.fmt.i_codec = VLC_FOURCC( 't', 'w', 'o', 's' );
            }
            else
            {
                tk.fmt.i_codec = VLC_FOURCC( 'a', 'r', 'a', 'w' );
            }
            tk.fmt.audio.i_blockalign = ( tk.fmt.audio.i_bitspersample + 7 ) / 8 * tk.fmt.audio.i_channels;
        }
        else if( !strcmp( tk.psz_codec, "S_TEXT/UTF8" ) )
        {
            tk.fmt.i_codec = VLC_FOURCC( 's', 'u', 'b', 't' );
            tk.fmt.subs.psz_encoding = strdup( "UTF-8" );
        }
        else if( !strcmp( tk.psz_codec, "S_TEXT/SSA" ) ||
                 !strcmp( tk.psz_codec, "S_TEXT/ASS" ) ||
                 !strcmp( tk.psz_codec, "S_SSA" ) ||
                 !strcmp( tk.psz_codec, "S_ASS" ))
        {
            tk.fmt.i_codec = VLC_FOURCC( 's', 's', 'a', ' ' );
            tk.fmt.subs.psz_encoding = strdup( "UTF-8" );
        }
        else if( !strcmp( tk.psz_codec, "S_VOBSUB" ) )
        {
            tk.fmt.i_codec = VLC_FOURCC( 's','p','u',' ' );
            if( tk.i_extra_data )
            {
                char *p_start;
                char *p_buf = (char *)malloc( tk.i_extra_data + 1);
                memcpy( p_buf, tk.p_extra_data , tk.i_extra_data );
                p_buf[tk.i_extra_data] = '\0';
                
                p_start = strstr( p_buf, "size:" );
                if( sscanf( p_start, "size: %dx%d",
                        &tk.fmt.subs.spu.i_original_frame_width, &tk.fmt.subs.spu.i_original_frame_height ) == 2 )
                {
                    msg_Dbg( p_demux, "original frame size vobsubs: %dx%d", tk.fmt.subs.spu.i_original_frame_width, tk.fmt.subs.spu.i_original_frame_height );
                }
                else
                {
                    msg_Warn( p_demux, "reading original frame size for vobsub failed" );
                }
                free( p_buf );
            }
        }
        else
        {
            msg_Err( p_demux, "unknow codec id=`%s'", tk.psz_codec );
            tk.fmt.i_codec = VLC_FOURCC( 'u', 'n', 'd', 'f' );
        }
        if( tk.b_default )
        {
            tk.fmt.i_priority = 1000;
        }

        tk.p_es = es_out_Add( p_demux->out, &tk.fmt );
#undef tk
    }

    /* add information */
    InformationCreate( p_demux );

    return VLC_SUCCESS;

error:
    delete p_sys->es;
    delete p_sys->in;
    free( p_sys );
    return VLC_EGENERIC;
}

/*****************************************************************************
 * Close: frees unused data
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    demux_t     *p_demux = (demux_t*)p_this;
    demux_sys_t *p_sys   = p_demux->p_sys;
    int         i_track;

    for( i_track = 0; i_track < p_sys->i_track; i_track++ )
    {
#define tk  p_sys->track[i_track]
        if( tk.fmt.psz_description )
        {
            free( tk.fmt.psz_description );
        }
        if( tk.psz_codec )
        {
            free( tk.psz_codec );
        }
        if( tk.fmt.psz_language )
        {
            free( tk.fmt.psz_language );
        }
#undef tk
    }
    free( p_sys->track );

    if( p_sys->psz_writing_application  )
    {
        free( p_sys->psz_writing_application );
    }
    if( p_sys->psz_muxing_application  )
    {
        free( p_sys->psz_muxing_application );
    }

    delete p_sys->segment;

    delete p_sys->ep;
    delete p_sys->es;
    delete p_sys->in;

    free( p_sys );
}

/*****************************************************************************
 * Control:
 *****************************************************************************/
static int Control( demux_t *p_demux, int i_query, va_list args )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    int64_t     *pi64;
    double      *pf, f;

    vlc_meta_t **pp_meta;

    switch( i_query )
    {
        case DEMUX_GET_META:
            pp_meta = (vlc_meta_t**)va_arg( args, vlc_meta_t** );
            *pp_meta = vlc_meta_Duplicate( p_sys->meta );
            return VLC_SUCCESS;

        case DEMUX_GET_LENGTH:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            if( p_sys->f_duration > 0.0 )
            {
                *pi64 = (int64_t)(p_sys->f_duration * 1000);
                return VLC_SUCCESS;
            }
            return VLC_EGENERIC;

        case DEMUX_GET_POSITION:
            pf = (double*)va_arg( args, double * );
            *pf = (double)p_sys->in->getFilePointer() / (double)stream_Size( p_demux->s );
            return VLC_SUCCESS;

        case DEMUX_SET_POSITION:
            f = (double)va_arg( args, double );
            Seek( p_demux, -1, (int)(100.0 * f) );
            return VLC_SUCCESS;

        case DEMUX_GET_TIME:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            if( p_sys->f_duration > 0.0 )
            {
                mtime_t i_duration = (mtime_t)( p_sys->f_duration / 1000 );

                /* FIXME */
                *pi64 = (mtime_t)1000000 *
                        (mtime_t)i_duration*
                        (mtime_t)p_sys->in->getFilePointer() /
                        (mtime_t)stream_Size( p_demux->s );
                return VLC_SUCCESS;
            }
            return VLC_EGENERIC;

        case DEMUX_GET_TITLE_INFO:
            if( p_sys->title && p_sys->title->i_seekpoint > 0 )
            {
                input_title_t ***ppp_title = (input_title_t***)va_arg( args, input_title_t*** );
                int *pi_int    = (int*)va_arg( args, int* );

                *pi_int = 1;
                *ppp_title = (input_title_t**)malloc( sizeof( input_title_t**) );

                (*ppp_title)[0] = vlc_input_title_Duplicate( p_sys->title );

                return VLC_SUCCESS;
            }
            return VLC_EGENERIC;

        case DEMUX_SET_TITLE:
            if( p_sys->title && p_sys->title->i_seekpoint > 0 )
            {
                return VLC_SUCCESS;
            }
            return VLC_EGENERIC;

        case DEMUX_SET_SEEKPOINT:
            /* FIXME do a better implementation */
            if( p_sys->title && p_sys->title->i_seekpoint > 0 )
            {
                int i_skp = (int)va_arg( args, int );

                Seek( p_demux, (int64_t)p_sys->title->seekpoint[i_skp]->i_time_offset, -1);
                return VLC_SUCCESS;
            }
            return VLC_EGENERIC;


        case DEMUX_SET_TIME:
        case DEMUX_GET_FPS:
        default:
            return VLC_EGENERIC;
    }
}

static int BlockGet( demux_t *p_demux, KaxBlock **pp_block, int64_t *pi_ref1, int64_t *pi_ref2, int64_t *pi_duration )
{
    demux_sys_t *p_sys = p_demux->p_sys;

    *pp_block = NULL;
    *pi_ref1  = -1;
    *pi_ref2  = -1;

    for( ;; )
    {
        EbmlElement *el;
        int         i_level;

        if( p_demux->b_die )
        {
            return VLC_EGENERIC;
        }

        el = p_sys->ep->Get();
        i_level = p_sys->ep->GetLevel();

        if( el == NULL && *pp_block != NULL )
        {
            /* update the index */
#define idx p_sys->index[p_sys->i_index - 1]
            if( p_sys->i_index > 0 && idx.i_time == -1 )
            {
                idx.i_time        = (*pp_block)->GlobalTimecode() / (mtime_t)1000;
                idx.b_key         = *pi_ref1 == -1 ? VLC_TRUE : VLC_FALSE;
            }
#undef idx
            return VLC_SUCCESS;
        }

        if( el == NULL )
        {
            if( p_sys->ep->GetLevel() > 1 )
            {
                p_sys->ep->Up();
                continue;
            }
            msg_Warn( p_demux, "EOF" );
            return VLC_EGENERIC;
        }

        /* do parsing */
        if( i_level == 1 )
        {
            if( MKV_IS_ID( el, KaxCluster ) )
            {
                p_sys->cluster = (KaxCluster*)el;

                /* add it to the index */
                if( p_sys->i_index == 0 ||
                    ( p_sys->i_index > 0 && p_sys->index[p_sys->i_index - 1].i_position < (int64_t)p_sys->cluster->GetElementPosition() ) )
                {
                    IndexAppendCluster( p_demux, p_sys->cluster );
                }

                p_sys->ep->Down();
            }
            else if( MKV_IS_ID( el, KaxCues ) )
            {
                msg_Warn( p_demux, "find KaxCues FIXME" );
                return VLC_EGENERIC;
            }
            else
            {
                msg_Dbg( p_demux, "unknown (%s)", typeid( el ).name() );
            }
        }
        else if( i_level == 2 )
        {
            if( MKV_IS_ID( el, KaxClusterTimecode ) )
            {
                KaxClusterTimecode &ctc = *(KaxClusterTimecode*)el;

                ctc.ReadData( p_sys->es->I_O(), SCOPE_ALL_DATA );
                p_sys->cluster->InitTimecode( uint64( ctc ), p_sys->i_timescale );
            }
            else if( MKV_IS_ID( el, KaxBlockGroup ) )
            {
                p_sys->ep->Down();
            }
        }
        else if( i_level == 3 )
        {
            if( MKV_IS_ID( el, KaxBlock ) )
            {
                *pp_block = (KaxBlock*)el;

                (*pp_block)->ReadData( p_sys->es->I_O() );
                (*pp_block)->SetParent( *p_sys->cluster );

                p_sys->ep->Keep();
            }
            else if( MKV_IS_ID( el, KaxBlockDuration ) )
            {
                KaxBlockDuration &dur = *(KaxBlockDuration*)el;

                dur.ReadData( p_sys->es->I_O() );
                *pi_duration = uint64( dur );
            }
            else if( MKV_IS_ID( el, KaxReferenceBlock ) )
            {
                KaxReferenceBlock &ref = *(KaxReferenceBlock*)el;

                ref.ReadData( p_sys->es->I_O() );
                if( *pi_ref1 == -1 )
                {
                    *pi_ref1 = int64( ref );
                }
                else
                {
                    *pi_ref2 = int64( ref );
                }
            }
        }
        else
        {
            msg_Err( p_demux, "invalid level = %d", i_level );
            return VLC_EGENERIC;
        }
    }
}

static block_t *MemToBlock( demux_t *p_demux, uint8_t *p_mem, int i_mem)
{
    block_t *p_block;
    if( !(p_block = block_New( p_demux, i_mem ) ) ) return NULL;
    memcpy( p_block->p_buffer, p_mem, i_mem );
    //p_block->i_rate = p_input->stream.control.i_rate;
    return p_block;
}

static void BlockDecode( demux_t *p_demux, KaxBlock *block, mtime_t i_pts,
                         mtime_t i_duration )
{
    demux_sys_t *p_sys = p_demux->p_sys;

    int             i_track;
    unsigned int    i;
    vlc_bool_t      b;

#define tk  p_sys->track[i_track]
    for( i_track = 0; i_track < p_sys->i_track; i_track++ )
    {
        if( tk.i_number == block->TrackNum() )
        {
            break;
        }
    }

    if( i_track >= p_sys->i_track )
    {
        msg_Err( p_demux, "invalid track number=%d", block->TrackNum() );
        return;
    }

    es_out_Control( p_demux->out, ES_OUT_GET_ES_STATE, tk.p_es, &b );
    if( !b )
    {
        tk.b_inited = VLC_FALSE;
        return;
    }

    /* First send init data */
    if( !tk.b_inited && tk.i_data_init > 0 )
    {
        block_t *p_init;

        msg_Dbg( p_demux, "sending header (%d bytes)", tk.i_data_init );
        p_init = MemToBlock( p_demux, tk.p_data_init, tk.i_data_init );
        if( p_init ) es_out_Send( p_demux->out, tk.p_es, p_init );
    }
    tk.b_inited = VLC_TRUE;


    for( i = 0; i < block->NumberFrames(); i++ )
    {
        block_t *p_block;
        DataBuffer &data = block->GetBuffer(i);

        p_block = MemToBlock( p_demux, data.Buffer(), data.Size() );

        if( p_block == NULL )
        {
            break;
        }

#if defined(HAVE_ZLIB_H)
        if( tk.i_compression_type )
        {
            p_block = block_zlib_decompress( VLC_OBJECT(p_demux), p_block );
        }
#endif

        if( tk.fmt.i_cat != VIDEO_ES )
            p_block->i_dts = p_block->i_pts = i_pts;
        else
        {
            p_block->i_dts = i_pts;
            p_block->i_pts = 0;
        }

        if( tk.fmt.i_cat == SPU_ES && strcmp( tk.psz_codec, "S_VOBSUB" ) )
        {
            p_block->i_length = i_duration * 1000;
        }
        es_out_Send( p_demux->out, tk.p_es, p_block );

        /* use time stamp only for first block */
        i_pts = 0;
    }

#undef tk
}

static void Seek( demux_t *p_demux, mtime_t i_date, int i_percent)
{
    demux_sys_t *p_sys = p_demux->p_sys;

    KaxBlock    *block;
    int64_t     i_block_duration;
    int64_t     i_block_ref1;
    int64_t     i_block_ref2;

    int         i_index;
    int         i_track_skipping;
    int         i_track;

    msg_Dbg( p_demux, "seek request to "I64Fd" (%d%%)", i_date, i_percent );
    if( i_date < 0 && i_percent < 0 )
    {
        return;
    }
    if( i_percent > 100 ) i_percent = 100;

    delete p_sys->ep;
    p_sys->ep = new EbmlParser( p_sys->es, p_sys->segment );
    p_sys->cluster = NULL;

    /* seek without index or without date */
    if( config_GetInt( p_demux, "mkv-seek-percent" ) || !p_sys->b_cues || i_date < 0 )
    {
        int64_t i_pos = i_percent * stream_Size( p_demux->s ) / 100;

        msg_Dbg( p_demux, "inacurate way of seeking" );
        for( i_index = 0; i_index < p_sys->i_index; i_index++ )
        {
            if( p_sys->index[i_index].i_position >= i_pos)
            {
                break;
            }
        }
        if( i_index == p_sys->i_index )
        {
            i_index--;
        }

        p_sys->in->setFilePointer( p_sys->index[i_index].i_position,
                                   seek_beginning );

        if( p_sys->index[i_index].i_position < i_pos )
        {
            EbmlElement *el;

            msg_Warn( p_demux, "searching for cluster, could take some time" );

            /* search a cluster */
            while( ( el = p_sys->ep->Get() ) != NULL )
            {
                if( MKV_IS_ID( el, KaxCluster ) )
                {
                    KaxCluster *cluster = (KaxCluster*)el;

                    /* add it to the index */
                    IndexAppendCluster( p_demux, cluster );

                    if( (int64_t)cluster->GetElementPosition() >= i_pos )
                    {
                        p_sys->cluster = cluster;
                        p_sys->ep->Down();
                        break;
                    }
                }
            }
        }
    }
    else
    {
        for( i_index = 0; i_index < p_sys->i_index; i_index++ )
        {
            if( p_sys->index[i_index].i_time >= i_date )
            {
                break;
            }
        }

        if( i_index > 0 )
        {
            i_index--;
        }

        msg_Dbg( p_demux, "seek got "I64Fd" (%d%%)",
                 p_sys->index[i_index].i_time,
                 (int)( 100 * p_sys->index[i_index].i_position /
                        stream_Size( p_demux->s ) ) );

        p_sys->in->setFilePointer( p_sys->index[i_index].i_position,
                                   seek_beginning );
    }

    /* now parse until key frame */
#define tk  p_sys->track[i_track]
    i_track_skipping = 0;
    for( i_track = 0; i_track < p_sys->i_track; i_track++ )
    {
        if( tk.fmt.i_cat == VIDEO_ES )
        {
            tk.b_search_keyframe = VLC_TRUE;
            i_track_skipping++;
        }
    }

    while( i_track_skipping > 0 )
    {
        if( BlockGet( p_demux, &block, &i_block_ref1, &i_block_ref2, &i_block_duration ) )
        {
            msg_Warn( p_demux, "cannot get block EOF?" );

            return;
        }

        p_sys->i_pts = block->GlobalTimecode() / (mtime_t) 1000 + 1;

        for( i_track = 0; i_track < p_sys->i_track; i_track++ )
        {
            if( tk.i_number == block->TrackNum() )
            {
                break;
            }
        }

        if( i_track < p_sys->i_track )
        {
            if( tk.fmt.i_cat == VIDEO_ES && i_block_ref1 == -1 && tk.b_search_keyframe )
            {
                tk.b_search_keyframe = VLC_FALSE;
                i_track_skipping--;
            }
            if( tk.fmt.i_cat == VIDEO_ES && !tk.b_search_keyframe )
            {
                BlockDecode( p_demux, block, 0, 0 );
            }
        }

        delete block;
    }
#undef tk
}

/*****************************************************************************
 * Demux: reads and demuxes data packets
 *****************************************************************************
 * Returns -1 in case of error, 0 in case of EOF, 1 otherwise
 *****************************************************************************/
static int Demux( demux_t *p_demux)
{
    demux_sys_t *p_sys = p_demux->p_sys;
    mtime_t        i_start_pts;
    int            i_block_count = 0;

    KaxBlock *block;
    int64_t i_block_duration;
    int64_t i_block_ref1;
    int64_t i_block_ref2;

    i_start_pts = -1;

    for( ;; )
    {
        if( BlockGet( p_demux, &block, &i_block_ref1, &i_block_ref2, &i_block_duration ) )
        {
            msg_Warn( p_demux, "cannot get block EOF?" );

            return 0;
        }

        p_sys->i_pts = block->GlobalTimecode() / (mtime_t) 1000 + 1;

        if( p_sys->i_pts > 0 )
        {
            es_out_Control( p_demux->out, ES_OUT_SET_PCR, p_sys->i_pts );
        }

        BlockDecode( p_demux, block, p_sys->i_pts, i_block_duration );

        delete block;
        i_block_count++;

        if( i_start_pts == -1 )
        {
            i_start_pts = p_sys->i_pts;
        }
        else if( p_sys->i_pts > i_start_pts + (mtime_t)100000 || i_block_count > 5 )
        {
            return 1;
        }
    }
}



/*****************************************************************************
 * Stream managment
 *****************************************************************************/
vlc_stream_io_callback::vlc_stream_io_callback( stream_t *s_ )
{
    s = s_;
    mb_eof = VLC_FALSE;
}

uint32 vlc_stream_io_callback::read( void *p_buffer, size_t i_size )
{
    if( i_size <= 0 || mb_eof )
    {
        return 0;
    }

    return stream_Read( s, p_buffer, i_size );
}
void vlc_stream_io_callback::setFilePointer(int64_t i_offset, seek_mode mode )
{
    int64_t i_pos;

    switch( mode )
    {
        case seek_beginning:
            i_pos = i_offset;
            break;
        case seek_end:
            i_pos = stream_Size( s ) - i_offset;
            break;
        default:
            i_pos= stream_Tell( s ) + i_offset;
            break;
    }

    if( i_pos < 0 || i_pos >= stream_Size( s ) )
    {
        mb_eof = VLC_TRUE;
        return;
    }

    mb_eof = VLC_FALSE;
    if( stream_Seek( s, i_pos ) )
    {
        mb_eof = VLC_TRUE;
    }
    return;
}
size_t vlc_stream_io_callback::write( const void *p_buffer, size_t i_size )
{
    return 0;
}
uint64 vlc_stream_io_callback::getFilePointer( void )
{
    return stream_Tell( s );
}
void vlc_stream_io_callback::close( void )
{
    return;
}


/*****************************************************************************
 * Ebml Stream parser
 *****************************************************************************/
EbmlParser::EbmlParser( EbmlStream *es, EbmlElement *el_start )
{
    int i;

    m_es = es;
    m_got = NULL;
    m_el[0] = el_start;

    for( i = 1; i < 6; i++ )
    {
        m_el[i] = NULL;
    }
    mi_level = 1;
    mi_user_level = 1;
    mb_keep = VLC_FALSE;
}

EbmlParser::~EbmlParser( void )
{
    int i;

    for( i = 1; i < mi_level; i++ )
    {
        if( !mb_keep )
        {
            delete m_el[i];
        }
        mb_keep = VLC_FALSE;
    }
}

void EbmlParser::Up( void )
{
    if( mi_user_level == mi_level )
    {
        fprintf( stderr," arrrrrrrrrrrrrg Up cannot escape itself\n" );
    }

    mi_user_level--;
}

void EbmlParser::Down( void )
{
    mi_user_level++;
    mi_level++;
}

void EbmlParser::Keep( void )
{
    mb_keep = VLC_TRUE;
}

int EbmlParser::GetLevel( void )
{
    return mi_user_level;
}

EbmlElement *EbmlParser::Get( void )
{
    int i_ulev = 0;

    if( mi_user_level != mi_level )
    {
        return NULL;
    }
    if( m_got )
    {
        EbmlElement *ret = m_got;
        m_got = NULL;

        return ret;
    }

    if( m_el[mi_level] )
    {
        m_el[mi_level]->SkipData( *m_es, m_el[mi_level]->Generic().Context );
        if( !mb_keep )
        {
            delete m_el[mi_level];
        }
        mb_keep = VLC_FALSE;
    }

    m_el[mi_level] = m_es->FindNextElement( m_el[mi_level - 1]->Generic().Context, i_ulev, 0xFFFFFFFFL, true, 1 );
    if( i_ulev > 0 )
    {
        while( i_ulev > 0 )
        {
            if( mi_level == 1 )
            {
                mi_level = 0;
                return NULL;
            }

            delete m_el[mi_level - 1];
            m_got = m_el[mi_level -1] = m_el[mi_level];
            m_el[mi_level] = NULL;

            mi_level--;
            i_ulev--;
        }
        return NULL;
    }
    else if( m_el[mi_level] == NULL )
    {
        fprintf( stderr," m_el[mi_level] == NULL\n" );
    }

    return m_el[mi_level];
}


/*****************************************************************************
 * Tools
 *  * LoadCues : load the cues element and update index
 *
 *  * LoadTags : load ... the tags element
 *
 *  * InformationCreate : create all information, load tags if present
 *
 *****************************************************************************/
static void LoadCues( demux_t *p_demux )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    int64_t     i_sav_position = p_sys->in->getFilePointer();
    EbmlParser  *ep;
    EbmlElement *el, *cues;

    msg_Dbg( p_demux, "loading cues" );
    p_sys->in->setFilePointer( p_sys->i_cues_position, seek_beginning );
    cues = p_sys->es->FindNextID( KaxCues::ClassInfos, 0xFFFFFFFFL);

    if( cues == NULL )
    {
        msg_Err( p_demux, "cannot load cues (broken seekhead or file)" );
        p_sys->in->setFilePointer( i_sav_position, seek_beginning );
        return;
    }

    ep = new EbmlParser( p_sys->es, cues );
    while( ( el = ep->Get() ) != NULL )
    {
        if( MKV_IS_ID( el, KaxCuePoint ) )
        {
#define idx p_sys->index[p_sys->i_index]

            idx.i_track       = -1;
            idx.i_block_number= -1;
            idx.i_position    = -1;
            idx.i_time        = 0;
            idx.b_key         = VLC_TRUE;

            ep->Down();
            while( ( el = ep->Get() ) != NULL )
            {
                if( MKV_IS_ID( el, KaxCueTime ) )
                {
                    KaxCueTime &ctime = *(KaxCueTime*)el;

                    ctime.ReadData( p_sys->es->I_O() );

                    idx.i_time = uint64( ctime ) * p_sys->i_timescale / (mtime_t)1000;
                }
                else if( MKV_IS_ID( el, KaxCueTrackPositions ) )
                {
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        if( MKV_IS_ID( el, KaxCueTrack ) )
                        {
                            KaxCueTrack &ctrack = *(KaxCueTrack*)el;

                            ctrack.ReadData( p_sys->es->I_O() );
                            idx.i_track = uint16( ctrack );
                        }
                        else if( MKV_IS_ID( el, KaxCueClusterPosition ) )
                        {
                            KaxCueClusterPosition &ccpos = *(KaxCueClusterPosition*)el;

                            ccpos.ReadData( p_sys->es->I_O() );
                            idx.i_position = p_sys->segment->GetGlobalPosition( uint64( ccpos ) );
                        }
                        else if( MKV_IS_ID( el, KaxCueBlockNumber ) )
                        {
                            KaxCueBlockNumber &cbnum = *(KaxCueBlockNumber*)el;

                            cbnum.ReadData( p_sys->es->I_O() );
                            idx.i_block_number = uint32( cbnum );
                        }
                        else
                        {
                            msg_Dbg( p_demux, "         * Unknown (%s)", typeid(*el).name() );
                        }
                    }
                    ep->Up();
                }
                else
                {
                    msg_Dbg( p_demux, "     * Unknown (%s)", typeid(*el).name() );
                }
            }
            ep->Up();

#if 0
            msg_Dbg( p_demux, " * added time="I64Fd" pos="I64Fd
                     " track=%d bnum=%d", idx.i_time, idx.i_position,
                     idx.i_track, idx.i_block_number );
#endif

            p_sys->i_index++;
            if( p_sys->i_index >= p_sys->i_index_max )
            {
                p_sys->i_index_max += 1024;
                p_sys->index = (mkv_index_t*)realloc( p_sys->index, sizeof( mkv_index_t ) * p_sys->i_index_max );
            }
#undef idx
        }
        else
        {
            msg_Dbg( p_demux, " * Unknown (%s)", typeid(*el).name() );
        }
    }
    delete ep;
    delete cues;

    p_sys->b_cues = VLC_TRUE;

    msg_Dbg( p_demux, "loading cues done." );
    p_sys->in->setFilePointer( i_sav_position, seek_beginning );
}

static void LoadTags( demux_t *p_demux )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    int64_t     i_sav_position = p_sys->in->getFilePointer();
    EbmlParser  *ep;
    EbmlElement *el, *tags;

    msg_Dbg( p_demux, "loading tags" );
    p_sys->in->setFilePointer( p_sys->i_tags_position, seek_beginning );
    tags = p_sys->es->FindNextID( KaxTags::ClassInfos, 0xFFFFFFFFL);

    if( tags == NULL )
    {
        msg_Err( p_demux, "cannot load tags (broken seekhead or file)" );
        p_sys->in->setFilePointer( i_sav_position, seek_beginning );
        return;
    }

    msg_Dbg( p_demux, "Tags" );
    ep = new EbmlParser( p_sys->es, tags );
    while( ( el = ep->Get() ) != NULL )
    {
        if( MKV_IS_ID( el, KaxTag ) )
        {
            msg_Dbg( p_demux, "+ Tag" );
            ep->Down();
            while( ( el = ep->Get() ) != NULL )
            {
                if( MKV_IS_ID( el, KaxTagTargets ) )
                {
                    msg_Dbg( p_demux, "|   + Targets" );
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid( *el ).name() );
                    }
                    ep->Up();
                }
                else if( MKV_IS_ID( el, KaxTagGeneral ) )
                {
                    msg_Dbg( p_demux, "|   + General" );
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid( *el ).name() );
                    }
                    ep->Up();
                }
                else if( MKV_IS_ID( el, KaxTagGenres ) )
                {
                    msg_Dbg( p_demux, "|   + Genres" );
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid( *el ).name() );
                    }
                    ep->Up();
                }
                else if( MKV_IS_ID( el, KaxTagAudioSpecific ) )
                {
                    msg_Dbg( p_demux, "|   + Audio Specific" );
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid( *el ).name() );
                    }
                    ep->Up();
                }
                else if( MKV_IS_ID( el, KaxTagImageSpecific ) )
                {
                    msg_Dbg( p_demux, "|   + Images Specific" );
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid( *el ).name() );
                    }
                    ep->Up();
                }
                else if( MKV_IS_ID( el, KaxTagMultiComment ) )
                {
                    msg_Dbg( p_demux, "|   + Multi Comment" );
                }
                else if( MKV_IS_ID( el, KaxTagMultiCommercial ) )
                {
                    msg_Dbg( p_demux, "|   + Multi Commercial" );
                }
                else if( MKV_IS_ID( el, KaxTagMultiDate ) )
                {
                    msg_Dbg( p_demux, "|   + Multi Date" );
                }
                else if( MKV_IS_ID( el, KaxTagMultiEntity ) )
                {
                    msg_Dbg( p_demux, "|   + Multi Entity" );
                }
                else if( MKV_IS_ID( el, KaxTagMultiIdentifier ) )
                {
                    msg_Dbg( p_demux, "|   + Multi Identifier" );
                }
                else if( MKV_IS_ID( el, KaxTagMultiLegal ) )
                {
                    msg_Dbg( p_demux, "|   + Multi Legal" );
                }
                else if( MKV_IS_ID( el, KaxTagMultiTitle ) )
                {
                    msg_Dbg( p_demux, "|   + Multi Title" );
                }
                else
                {
                    msg_Dbg( p_demux, "|   + Unknown (%s)", typeid( *el ).name() );
                }
            }
            ep->Up();
        }
        else
        {
            msg_Dbg( p_demux, "+ Unknown (%s)", typeid( *el ).name() );
        }
    }
    delete ep;
    delete tags;

    msg_Dbg( p_demux, "loading tags done." );
    p_sys->in->setFilePointer( i_sav_position, seek_beginning );
}

/*****************************************************************************
 * ParseInfo:
 *****************************************************************************/
static void ParseSeekHead( demux_t *p_demux, EbmlElement *seekhead )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    EbmlElement *el;
    EbmlMaster  *m;
    unsigned int i;
    int i_upper_level = 0;

    msg_Dbg( p_demux, "|   + Seek head" );

    /* Master elements */
    m = static_cast<EbmlMaster *>(seekhead);
    m->Read( *p_sys->es, seekhead->Generic().Context, i_upper_level, el, true );

    for( i = 0; i < m->ListSize(); i++ )
    {
        EbmlElement *l = (*m)[i];

        if( MKV_IS_ID( l, KaxSeek ) )
        {
            EbmlMaster *sk = static_cast<EbmlMaster *>(l);
            EbmlId id = EbmlVoid::ClassInfos.GlobalId;
            int64_t i_pos = -1;

            unsigned int j;

            for( j = 0; j < sk->ListSize(); j++ )
            {
                EbmlElement *l = (*sk)[j];

                if( MKV_IS_ID( l, KaxSeekID ) )
                {
                    KaxSeekID &sid = *(KaxSeekID*)l;
                    id = EbmlId( sid.GetBuffer(), sid.GetSize() );
                }
                else if( MKV_IS_ID( l, KaxSeekPosition ) )
                {
                    KaxSeekPosition &spos = *(KaxSeekPosition*)l;
                    i_pos = uint64( spos );
                }
                else
                {
                    msg_Dbg( p_demux, "|   |   |   + Unknown (%s)", typeid(*l).name() );
                }
            }

            if( i_pos >= 0 )
            {
                if( id == KaxCues::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   |   |   = cues at "I64Fd, i_pos );
                    p_sys->i_cues_position = p_sys->segment->GetGlobalPosition( i_pos );
                }
                else if( id == KaxChapters::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   |   |   = chapters at "I64Fd, i_pos );
                    p_sys->i_chapters_position = p_sys->segment->GetGlobalPosition( i_pos );
                }
                else if( id == KaxTags::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   |   |   = tags at "I64Fd, i_pos );
                    p_sys->i_tags_position = p_sys->segment->GetGlobalPosition( i_pos );
                }
            }
        }
        else
        {
            msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid(*l).name() );
        }
    }
}

/*****************************************************************************
 * ParseTracks:
 *****************************************************************************/
static void ParseTrackEntry( demux_t *p_demux, EbmlMaster *m )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    unsigned int i;

    mkv_track_t *tk;

    msg_Dbg( p_demux, "|   |   + Track Entry" );

    p_sys->i_track++;
    p_sys->track = (mkv_track_t*)realloc( p_sys->track, sizeof( mkv_track_t ) * (p_sys->i_track + 1 ) );

    /* Init the track */
    tk = &p_sys->track[p_sys->i_track - 1];

    memset( tk, 0, sizeof( mkv_track_t ) );

    es_format_Init( &tk->fmt, UNKNOWN_ES, 0 );
    tk->fmt.psz_language = strdup("English");
    tk->fmt.psz_description = NULL;

    tk->b_default = VLC_TRUE;
    tk->b_enabled = VLC_TRUE;
    tk->i_number = p_sys->i_track - 1;
    tk->i_extra_data = 0;
    tk->p_extra_data = NULL;
    tk->psz_codec = NULL;
    tk->i_default_duration = 0;
    tk->f_timecodescale = 1.0;

    tk->b_inited = VLC_FALSE;
    tk->i_data_init = 0;
    tk->p_data_init = NULL;

    tk->psz_codec_name = NULL;
    tk->psz_codec_settings = NULL;
    tk->psz_codec_info_url = NULL;
    tk->psz_codec_download_url = NULL;
    
    tk->i_compression_type = MATROSKA_COMPRESSION_NONE;

    for( i = 0; i < m->ListSize(); i++ )
    {
        EbmlElement *l = (*m)[i];

        if( MKV_IS_ID( l, KaxTrackNumber ) )
        {
            KaxTrackNumber &tnum = *(KaxTrackNumber*)l;

            tk->i_number = uint32( tnum );
            msg_Dbg( p_demux, "|   |   |   + Track Number=%u", uint32( tnum ) );
        }
        else  if( MKV_IS_ID( l, KaxTrackUID ) )
        {
            KaxTrackUID &tuid = *(KaxTrackUID*)l;

            msg_Dbg( p_demux, "|   |   |   + Track UID=%u",  uint32( tuid ) );
        }
        else  if( MKV_IS_ID( l, KaxTrackType ) )
        {
            char *psz_type;
            KaxTrackType &ttype = *(KaxTrackType*)l;

            switch( uint8(ttype) )
            {
                case track_audio:
                    psz_type = "audio";
                    tk->fmt.i_cat = AUDIO_ES;
                    break;
                case track_video:
                    psz_type = "video";
                    tk->fmt.i_cat = VIDEO_ES;
                    break;
                case track_subtitle:
                    psz_type = "subtitle";
                    tk->fmt.i_cat = SPU_ES;
                    break;
                default:
                    psz_type = "unknown";
                    tk->fmt.i_cat = UNKNOWN_ES;
                    break;
            }

            msg_Dbg( p_demux, "|   |   |   + Track Type=%s", psz_type );
        }
//        else  if( EbmlId( *l ) == KaxTrackFlagEnabled::ClassInfos.GlobalId )
//        {
//            KaxTrackFlagEnabled &fenb = *(KaxTrackFlagEnabled*)l;

//            tk->b_enabled = uint32( fenb );
//            msg_Dbg( p_demux, "|   |   |   + Track Enabled=%u",
//                     uint32( fenb )  );
//        }
        else  if( MKV_IS_ID( l, KaxTrackFlagDefault ) )
        {
            KaxTrackFlagDefault &fdef = *(KaxTrackFlagDefault*)l;

            tk->b_default = uint32( fdef );
            msg_Dbg( p_demux, "|   |   |   + Track Default=%u", uint32( fdef )  );
        }
        else  if( MKV_IS_ID( l, KaxTrackFlagLacing ) )
        {
            KaxTrackFlagLacing &lac = *(KaxTrackFlagLacing*)l;

            msg_Dbg( p_demux, "|   |   |   + Track Lacing=%d", uint32( lac ) );
        }
        else  if( MKV_IS_ID( l, KaxTrackMinCache ) )
        {
            KaxTrackMinCache &cmin = *(KaxTrackMinCache*)l;

            msg_Dbg( p_demux, "|   |   |   + Track MinCache=%d", uint32( cmin ) );
        }
        else  if( MKV_IS_ID( l, KaxTrackMaxCache ) )
        {
            KaxTrackMaxCache &cmax = *(KaxTrackMaxCache*)l;

            msg_Dbg( p_demux, "|   |   |   + Track MaxCache=%d", uint32( cmax ) );
        }
        else  if( MKV_IS_ID( l, KaxTrackDefaultDuration ) )
        {
            KaxTrackDefaultDuration &defd = *(KaxTrackDefaultDuration*)l;

            tk->i_default_duration = uint64(defd);
            msg_Dbg( p_demux, "|   |   |   + Track Default Duration="I64Fd, uint64(defd) );
        }
        else  if( MKV_IS_ID( l, KaxTrackTimecodeScale ) )
        {
            KaxTrackTimecodeScale &ttcs = *(KaxTrackTimecodeScale*)l;

            tk->f_timecodescale = float( ttcs );
            msg_Dbg( p_demux, "|   |   |   + Track TimeCodeScale=%f", tk->f_timecodescale );
        }
        else if( MKV_IS_ID( l, KaxTrackName ) )
        {
            KaxTrackName &tname = *(KaxTrackName*)l;

            tk->fmt.psz_description = UTF8ToStr( UTFstring( tname ) );
            msg_Dbg( p_demux, "|   |   |   + Track Name=%s", tk->fmt.psz_description );
        }
        else  if( MKV_IS_ID( l, KaxTrackLanguage ) )
        {
            KaxTrackLanguage &lang = *(KaxTrackLanguage*)l;

            tk->fmt.psz_language = strdup( string( lang ).c_str() );
            msg_Dbg( p_demux,
                     "|   |   |   + Track Language=`%s'", tk->fmt.psz_language );
        }
        else  if( MKV_IS_ID( l, KaxCodecID ) )
        {
            KaxCodecID &codecid = *(KaxCodecID*)l;

            tk->psz_codec = strdup( string( codecid ).c_str() );
            msg_Dbg( p_demux, "|   |   |   + Track CodecId=%s", string( codecid ).c_str() );
        }
        else  if( MKV_IS_ID( l, KaxCodecPrivate ) )
        {
            KaxCodecPrivate &cpriv = *(KaxCodecPrivate*)l;

            tk->i_extra_data = cpriv.GetSize();
            if( tk->i_extra_data > 0 )
            {
                tk->p_extra_data = (uint8_t*)malloc( tk->i_extra_data );
                memcpy( tk->p_extra_data, cpriv.GetBuffer(), tk->i_extra_data );
            }
            msg_Dbg( p_demux, "|   |   |   + Track CodecPrivate size="I64Fd, cpriv.GetSize() );
        }
        else if( MKV_IS_ID( l, KaxCodecName ) )
        {
            KaxCodecName &cname = *(KaxCodecName*)l;

            tk->psz_codec_name = UTF8ToStr( UTFstring( cname ) );
            msg_Dbg( p_demux, "|   |   |   + Track Codec Name=%s", tk->psz_codec_name );
        }
        else if( MKV_IS_ID( l, KaxContentEncodings ) )
        {
            EbmlMaster *cencs = static_cast<EbmlMaster*>(l);
            MkvTree( p_demux, 3, "Content Encodings" );
            for( unsigned int i = 0; i < cencs->ListSize(); i++ )
            {
                EbmlElement *l2 = (*cencs)[i];
                if( MKV_IS_ID( l2, KaxContentEncoding ) )
                {
                    MkvTree( p_demux, 4, "Content Encoding" );
                    EbmlMaster *cenc = static_cast<EbmlMaster*>(l2);
                    for( unsigned int i = 0; i < cenc->ListSize(); i++ )
                    {
                        EbmlElement *l3 = (*cenc)[i];
                        if( MKV_IS_ID( l3, KaxContentEncodingOrder ) )
                        {
                            KaxContentEncodingOrder &encord = *(KaxContentEncodingOrder*)l3;
                            MkvTree( p_demux, 5, "Order: %i", uint32( encord ) );
                        }
                        else if( MKV_IS_ID( l3, KaxContentEncodingScope ) )
                        {
                            KaxContentEncodingScope &encscope = *(KaxContentEncodingScope*)l3;
                            MkvTree( p_demux, 5, "Scope: %i", uint32( encscope ) );
                        }
                        else if( MKV_IS_ID( l3, KaxContentEncodingType ) )
                        {
                            KaxContentEncodingType &enctype = *(KaxContentEncodingType*)l3;
                            MkvTree( p_demux, 5, "Type: %i", uint32( enctype ) );
                        }
                        else if( MKV_IS_ID( l3, KaxContentCompression ) )
                        {
                            EbmlMaster *compr = static_cast<EbmlMaster*>(l3);
                            MkvTree( p_demux, 5, "Content Compression" );
                            for( unsigned int i = 0; i < compr->ListSize(); i++ )
                            {
                                EbmlElement *l4 = (*compr)[i];
                                if( MKV_IS_ID( l4, KaxContentCompAlgo ) )
                                {
                                    KaxContentCompAlgo &compalg = *(KaxContentCompAlgo*)l4;
                                    MkvTree( p_demux, 6, "Compression Algorithm: %i", uint32(compalg) );
                                    if( uint32( compalg ) == 0 )
                                    {
                                        tk->i_compression_type = MATROSKA_COMPRESSION_ZLIB;
                                    }
                                }
                                else
                                {
                                    MkvTree( p_demux, 6, "Unknown (%s)", typeid(*l4).name() );
                                }
                            }
                        }

                        else
                        {
                            MkvTree( p_demux, 5, "Unknown (%s)", typeid(*l3).name() );
                        }
                    }
                    
                }
                else
                {
                    MkvTree( p_demux, 4, "Unknown (%s)", typeid(*l2).name() );
                }
            }
                
        }
//        else if( EbmlId( *l ) == KaxCodecSettings::ClassInfos.GlobalId )
//        {
//            KaxCodecSettings &cset = *(KaxCodecSettings*)l;

//            tk->psz_codec_settings = UTF8ToStr( UTFstring( cset ) );
//            msg_Dbg( p_demux, "|   |   |   + Track Codec Settings=%s", tk->psz_codec_settings );
//        }
//        else if( EbmlId( *l ) == KaxCodecInfoURL::ClassInfos.GlobalId )
//        {
//            KaxCodecInfoURL &ciurl = *(KaxCodecInfoURL*)l;

//            tk->psz_codec_info_url = strdup( string( ciurl ).c_str() );
//            msg_Dbg( p_demux, "|   |   |   + Track Codec Info URL=%s", tk->psz_codec_info_url );
//        }
//        else if( EbmlId( *l ) == KaxCodecDownloadURL::ClassInfos.GlobalId )
//        {
//            KaxCodecDownloadURL &cdurl = *(KaxCodecDownloadURL*)l;

//            tk->psz_codec_download_url = strdup( string( cdurl ).c_str() );
//            msg_Dbg( p_demux, "|   |   |   + Track Codec Info URL=%s", tk->psz_codec_download_url );
//        }
//        else if( EbmlId( *l ) == KaxCodecDecodeAll::ClassInfos.GlobalId )
//        {
//            KaxCodecDecodeAll &cdall = *(KaxCodecDecodeAll*)l;

//            msg_Dbg( p_demux, "|   |   |   + Track Codec Decode All=%u <== UNUSED", uint8( cdall ) );
//        }
//        else if( EbmlId( *l ) == KaxTrackOverlay::ClassInfos.GlobalId )
//        {
//            KaxTrackOverlay &tovr = *(KaxTrackOverlay*)l;

//            msg_Dbg( p_demux, "|   |   |   + Track Overlay=%u <== UNUSED", uint32( tovr ) );
//        }
        else  if( MKV_IS_ID( l, KaxTrackVideo ) )
        {
            EbmlMaster *tkv = static_cast<EbmlMaster*>(l);
            unsigned int j;

            msg_Dbg( p_demux, "|   |   |   + Track Video" );
            tk->f_fps = 0.0;

            for( j = 0; j < tkv->ListSize(); j++ )
            {
                EbmlElement *l = (*tkv)[j];
//                if( EbmlId( *el4 ) == KaxVideoFlagInterlaced::ClassInfos.GlobalId )
//                {
//                    KaxVideoFlagInterlaced &fint = *(KaxVideoFlagInterlaced*)el4;

//                    msg_Dbg( p_demux, "|   |   |   |   + Track Video Interlaced=%u", uint8( fint ) );
//                }
//                else if( EbmlId( *el4 ) == KaxVideoStereoMode::ClassInfos.GlobalId )
//                {
//                    KaxVideoStereoMode &stereo = *(KaxVideoStereoMode*)el4;

//                    msg_Dbg( p_demux, "|   |   |   |   + Track Video Stereo Mode=%u", uint8( stereo ) );
//                }
//                else
                if( MKV_IS_ID( l, KaxVideoPixelWidth ) )
                {
                    KaxVideoPixelWidth &vwidth = *(KaxVideoPixelWidth*)l;

                    tk->fmt.video.i_width = uint16( vwidth );
                    msg_Dbg( p_demux, "|   |   |   |   + width=%d", uint16( vwidth ) );
                }
                else if( MKV_IS_ID( l, KaxVideoPixelHeight ) )
                {
                    KaxVideoPixelWidth &vheight = *(KaxVideoPixelWidth*)l;

                    tk->fmt.video.i_height = uint16( vheight );
                    msg_Dbg( p_demux, "|   |   |   |   + height=%d", uint16( vheight ) );
                }
                else if( MKV_IS_ID( l, KaxVideoDisplayWidth ) )
                {
                    KaxVideoDisplayWidth &vwidth = *(KaxVideoDisplayWidth*)l;

                    tk->fmt.video.i_visible_width = uint16( vwidth );
                    msg_Dbg( p_demux, "|   |   |   |   + display width=%d", uint16( vwidth ) );
                }
                else if( MKV_IS_ID( l, KaxVideoDisplayHeight ) )
                {
                    KaxVideoDisplayWidth &vheight = *(KaxVideoDisplayWidth*)l;

                    tk->fmt.video.i_visible_height = uint16( vheight );
                    msg_Dbg( p_demux, "|   |   |   |   + display height=%d", uint16( vheight ) );
                }
                else if( MKV_IS_ID( l, KaxVideoFrameRate ) )
                {
                    KaxVideoFrameRate &vfps = *(KaxVideoFrameRate*)l;

                    tk->f_fps = float( vfps );
                    msg_Dbg( p_demux, "   |   |   |   + fps=%f", float( vfps ) );
                }
//                else if( EbmlId( *l ) == KaxVideoDisplayUnit::ClassInfos.GlobalId )
//                {
//                     KaxVideoDisplayUnit &vdmode = *(KaxVideoDisplayUnit*)l;

//                    msg_Dbg( p_demux, "|   |   |   |   + Track Video Display Unit=%s",
//                             uint8( vdmode ) == 0 ? "pixels" : ( uint8( vdmode ) == 1 ? "centimeters": "inches" ) );
//                }
//                else if( EbmlId( *l ) == KaxVideoAspectRatio::ClassInfos.GlobalId )
//                {
//                    KaxVideoAspectRatio &ratio = *(KaxVideoAspectRatio*)l;

//                    msg_Dbg( p_demux, "   |   |   |   + Track Video Aspect Ratio Type=%u", uint8( ratio ) );
//                }
//                else if( EbmlId( *l ) == KaxVideoGamma::ClassInfos.GlobalId )
//                {
//                    KaxVideoGamma &gamma = *(KaxVideoGamma*)l;

//                    msg_Dbg( p_demux, "   |   |   |   + fps=%f", float( gamma ) );
//                }
                else
                {
                    msg_Dbg( p_demux, "|   |   |   |   + Unknown (%s)", typeid(*l).name() );
                }
            }
        }
        else  if( MKV_IS_ID( l, KaxTrackAudio ) )
        {
            EbmlMaster *tka = static_cast<EbmlMaster*>(l);
            unsigned int j;

            msg_Dbg( p_demux, "|   |   |   + Track Audio" );

            for( j = 0; j < tka->ListSize(); j++ )
            {
                EbmlElement *l = (*tka)[j];

                if( MKV_IS_ID( l, KaxAudioSamplingFreq ) )
                {
                    KaxAudioSamplingFreq &afreq = *(KaxAudioSamplingFreq*)l;

                    tk->fmt.audio.i_rate = (int)float( afreq );
                    msg_Dbg( p_demux, "|   |   |   |   + afreq=%d", tk->fmt.audio.i_rate );
                }
                else if( MKV_IS_ID( l, KaxAudioChannels ) )
                {
                    KaxAudioChannels &achan = *(KaxAudioChannels*)l;

                    tk->fmt.audio.i_channels = uint8( achan );
                    msg_Dbg( p_demux, "|   |   |   |   + achan=%u", uint8( achan ) );
                }
                else if( MKV_IS_ID( l, KaxAudioBitDepth ) )
                {
                    KaxAudioBitDepth &abits = *(KaxAudioBitDepth*)l;

                    tk->fmt.audio.i_bitspersample = uint8( abits );
                    msg_Dbg( p_demux, "|   |   |   |   + abits=%u", uint8( abits ) );
                }
                else
                {
                    msg_Dbg( p_demux, "|   |   |   |   + Unknown (%s)", typeid(*l).name() );
                }
            }
        }
        else
        {
            msg_Dbg( p_demux, "|   |   |   + Unknown (%s)",
                     typeid(*l).name() );
        }
    }
}

static void ParseTracks( demux_t *p_demux, EbmlElement *tracks )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    EbmlElement *el;
    EbmlMaster  *m;
    unsigned int i;
    int i_upper_level = 0;

    msg_Dbg( p_demux, "|   + Tracks" );

    /* Master elements */
    m = static_cast<EbmlMaster *>(tracks);
    m->Read( *p_sys->es, tracks->Generic().Context, i_upper_level, el, true );

    for( i = 0; i < m->ListSize(); i++ )
    {
        EbmlElement *l = (*m)[i];

        if( MKV_IS_ID( l, KaxTrackEntry ) )
        {
            ParseTrackEntry( p_demux, static_cast<EbmlMaster *>(l) );
        }
        else
        {
            msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid(*l).name() );
        }
    }
}

/*****************************************************************************
 * ParseInfo:
 *****************************************************************************/
static void ParseInfo( demux_t *p_demux, EbmlElement *info )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    EbmlElement *el;
    EbmlMaster  *m;
    unsigned int i;
    int i_upper_level = 0;

    msg_Dbg( p_demux, "|   + Information" );

    /* Master elements */
    m = static_cast<EbmlMaster *>(info);
    m->Read( *p_sys->es, info->Generic().Context, i_upper_level, el, true );

    for( i = 0; i < m->ListSize(); i++ )
    {
        EbmlElement *l = (*m)[i];

        if( MKV_IS_ID( l, KaxSegmentUID ) )
        {
            KaxSegmentUID &uid = *(KaxSegmentUID*)l;

            msg_Dbg( p_demux, "|   |   + UID=%d", uint32(uid) );
        }
        else if( MKV_IS_ID( l, KaxTimecodeScale ) )
        {
            KaxTimecodeScale &tcs = *(KaxTimecodeScale*)l;

            p_sys->i_timescale = uint64(tcs);

            msg_Dbg( p_demux, "|   |   + TimecodeScale="I64Fd,
                     p_sys->i_timescale );
        }
        else if( MKV_IS_ID( l, KaxDuration ) )
        {
            KaxDuration &dur = *(KaxDuration*)l;

            p_sys->f_duration = float(dur);

            msg_Dbg( p_demux, "|   |   + Duration=%f",
                     p_sys->f_duration );
        }
        else if( MKV_IS_ID( l, KaxMuxingApp ) )
        {
            KaxMuxingApp &mapp = *(KaxMuxingApp*)l;

            p_sys->psz_muxing_application = UTF8ToStr( UTFstring( mapp ) );

            msg_Dbg( p_demux, "|   |   + Muxing Application=%s",
                     p_sys->psz_muxing_application );
        }
        else if( MKV_IS_ID( l, KaxWritingApp ) )
        {
            KaxWritingApp &wapp = *(KaxWritingApp*)l;

            p_sys->psz_writing_application = UTF8ToStr( UTFstring( wapp ) );

            msg_Dbg( p_demux, "|   |   + Writing Application=%s",
                     p_sys->psz_writing_application );
        }
        else if( MKV_IS_ID( l, KaxSegmentFilename ) )
        {
            KaxSegmentFilename &sfn = *(KaxSegmentFilename*)l;

            p_sys->psz_segment_filename = UTF8ToStr( UTFstring( sfn ) );

            msg_Dbg( p_demux, "|   |   + Segment Filename=%s",
                     p_sys->psz_segment_filename );
        }
        else if( MKV_IS_ID( l, KaxTitle ) )
        {
            KaxTitle &title = *(KaxTitle*)l;

            p_sys->psz_title = UTF8ToStr( UTFstring( title ) );

            msg_Dbg( p_demux, "|   |   + Title=%s", p_sys->psz_title );
        }
#if defined( HAVE_GMTIME_R ) && !defined( SYS_DARWIN )
        else if( MKV_IS_ID( l, KaxDateUTC ) )
        {
            KaxDateUTC &date = *(KaxDateUTC*)l;
            time_t i_date;
            struct tm tmres;
            char   buffer[256];

            i_date = date.GetEpochDate();
            memset( buffer, 0, 256 );
            if( gmtime_r( &i_date, &tmres ) &&
                asctime_r( &tmres, buffer ) )
            {
                buffer[strlen( buffer)-1]= '\0';
                p_sys->psz_date_utc = strdup( buffer );
                msg_Dbg( p_demux, "|   |   + Date=%s", p_sys->psz_date_utc );
            }
        }
#endif
        else
        {
            msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid(*l).name() );
        }
    }

    p_sys->f_duration = p_sys->f_duration * p_sys->i_timescale / 1000000.0;
}


/*****************************************************************************
 * ParseChapterAtom
 *****************************************************************************/
static void ParseChapterAtom( demux_t *p_demux, int i_level, EbmlMaster *ca )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    unsigned int i;
    seekpoint_t *sk;

    if( p_sys->title == NULL )
    {
        p_sys->title = vlc_input_title_New();
    }
    sk = vlc_seekpoint_New();

    msg_Dbg( p_demux, "|   |   |   + ChapterAtom (level=%d)", i_level );
    for( i = 0; i < ca->ListSize(); i++ )
    {
        EbmlElement *l = (*ca)[i];

        if( MKV_IS_ID( l, KaxChapterUID ) )
        {
            KaxChapterUID &uid = *(KaxChapterUID*)l;
            uint32_t i_uid = uint32( uid );
            msg_Dbg( p_demux, "|   |   |   |   + ChapterUID: 0x%x", i_uid );
        }
        else if( MKV_IS_ID( l, KaxChapterTimeStart ) )
        {
            KaxChapterTimeStart &start =*(KaxChapterTimeStart*)l;
            sk->i_time_offset = uint64( start ) / I64C(1000);

            msg_Dbg( p_demux, "|   |   |   |   + ChapterTimeStart: %lld", sk->i_time_offset );
        }
        else if( MKV_IS_ID( l, KaxChapterTimeEnd ) )
        {
            KaxChapterTimeEnd &end =*(KaxChapterTimeEnd*)l;
            int64_t i_end = uint64( end );

            msg_Dbg( p_demux, "|   |   |   |   + ChapterTimeEnd: %lld", i_end );
        }
        else if( MKV_IS_ID( l, KaxChapterDisplay ) )
        {
            EbmlMaster *cd = static_cast<EbmlMaster *>(l);
            unsigned int j;

            msg_Dbg( p_demux, "|   |   |   |   + ChapterDisplay" );
            for( j = 0; j < cd->ListSize(); j++ )
            {
                EbmlElement *l= (*cd)[j];

                if( MKV_IS_ID( l, KaxChapterString ) )
                {
                    KaxChapterString &name =*(KaxChapterString*)l;
                    char *psz = UTF8ToStr( UTFstring( name ) );
                    sk->psz_name = strdup( psz );
                    msg_Dbg( p_demux, "|   |   |   |   |    + ChapterString '%s'", psz );
                }
                else if( MKV_IS_ID( l, KaxChapterLanguage ) )
                {
                    KaxChapterLanguage &lang =*(KaxChapterLanguage*)l;
                    const char *psz = string( lang ).c_str();

                    msg_Dbg( p_demux, "|   |   |   |   |    + ChapterLanguage '%s'", psz );
                }
                else if( MKV_IS_ID( l, KaxChapterCountry ) )
                {
                    KaxChapterCountry &ct =*(KaxChapterCountry*)l;
                    const char *psz = string( ct ).c_str();

                    msg_Dbg( p_demux, "|   |   |   |   |    + ChapterCountry '%s'", psz );
                }
            }
        }
        else if( MKV_IS_ID( l, KaxChapterAtom ) )
        {
            ParseChapterAtom( p_demux, i_level+1, static_cast<EbmlMaster *>(l) );
        }
    }
    // A start time of '0' is ok. A missing ChapterTime element is ok, too, because '0' is its default value.
    p_sys->title->i_seekpoint++;
    p_sys->title->seekpoint = (seekpoint_t**)realloc( p_sys->title->seekpoint, p_sys->title->i_seekpoint * sizeof( seekpoint_t* ) );
    p_sys->title->seekpoint[p_sys->title->i_seekpoint-1] = sk;
}

/*****************************************************************************
 * ParseChapters:
 *****************************************************************************/
static void ParseChapters( demux_t *p_demux, EbmlElement *chapters )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    EbmlElement *el;
    EbmlMaster  *m;
    unsigned int i;
    int i_upper_level = 0;


    /* Master elements */
    m = static_cast<EbmlMaster *>(chapters);
    m->Read( *p_sys->es, chapters->Generic().Context, i_upper_level, el, true );

    for( i = 0; i < m->ListSize(); i++ )
    {
        EbmlElement *l = (*m)[i];

        if( MKV_IS_ID( l, KaxEditionEntry ) )
        {
            EbmlMaster *E = static_cast<EbmlMaster *>(l );
            unsigned int j;
            msg_Dbg( p_demux, "|   |   + EditionEntry" );
            for( j = 0; j < E->ListSize(); j++ )
            {
                EbmlElement *l = (*E)[j];

                if( MKV_IS_ID( l, KaxChapterAtom ) )
                {
                    ParseChapterAtom( p_demux, 0, static_cast<EbmlMaster *>(l) );
                }
                else
                {
                    msg_Dbg( p_demux, "|   |   |   + Unknown (%s)", typeid(*l).name() );
                }
            }
        }
        else
        {
            msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid(*l).name() );
        }
    }
}

/*****************************************************************************
 * InformationCreate:
 *****************************************************************************/
static void InformationCreate( demux_t *p_demux )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    int         i_track;

    p_sys->meta = vlc_meta_New();

    if( p_sys->psz_title )
    {
        vlc_meta_Add( p_sys->meta, VLC_META_TITLE, p_sys->psz_title );
    }
    if( p_sys->psz_date_utc )
    {
        vlc_meta_Add( p_sys->meta, VLC_META_DATE, p_sys->psz_date_utc );
    }
    if( p_sys->psz_segment_filename )
    {
        vlc_meta_Add( p_sys->meta, _("Segment filename"), p_sys->psz_segment_filename );
    }
    if( p_sys->psz_muxing_application )
    {
        vlc_meta_Add( p_sys->meta, _("Muxing application"), p_sys->psz_muxing_application );
    }
    if( p_sys->psz_writing_application )
    {
        vlc_meta_Add( p_sys->meta, _("Writing application"), p_sys->psz_writing_application );
    }

    for( i_track = 0; i_track < p_sys->i_track; i_track++ )
    {
        mkv_track_t *tk = &p_sys->track[i_track];
        vlc_meta_t *mtk = vlc_meta_New();

        p_sys->meta->track = (vlc_meta_t**)realloc( p_sys->meta->track,
                                                    sizeof( vlc_meta_t * ) * ( p_sys->meta->i_track + 1 ) );
        p_sys->meta->track[p_sys->meta->i_track++] = mtk;

        if( tk->fmt.psz_description )
        {
            vlc_meta_Add( p_sys->meta, VLC_META_DESCRIPTION, tk->fmt.psz_description );
        }
        if( tk->psz_codec_name )
        {
            vlc_meta_Add( p_sys->meta, VLC_META_CODEC_NAME, tk->psz_codec_name );
        }
        if( tk->psz_codec_settings )
        {
            vlc_meta_Add( p_sys->meta, VLC_META_SETTING, tk->psz_codec_settings );
        }
        if( tk->psz_codec_info_url )
        {
            vlc_meta_Add( p_sys->meta, VLC_META_CODEC_DESCRIPTION, tk->psz_codec_info_url );
        }
        if( tk->psz_codec_download_url )
        {
            vlc_meta_Add( p_sys->meta, VLC_META_URL, tk->psz_codec_download_url );
        }
    }

    if( p_sys->i_tags_position >= 0 )
    {
        vlc_bool_t b_seekable;

        stream_Control( p_demux->s, STREAM_CAN_FASTSEEK, &b_seekable );
        if( b_seekable )
        {
            LoadTags( p_demux );
        }
    }
}


/*****************************************************************************
 * Divers
 *****************************************************************************/

static void IndexAppendCluster( demux_t *p_demux, KaxCluster *cluster )
{
    demux_sys_t *p_sys = p_demux->p_sys;

#define idx p_sys->index[p_sys->i_index]
    idx.i_track       = -1;
    idx.i_block_number= -1;
    idx.i_position    = cluster->GetElementPosition();
    idx.i_time        = -1;
    idx.b_key         = VLC_TRUE;

    p_sys->i_index++;
    if( p_sys->i_index >= p_sys->i_index_max )
    {
        p_sys->i_index_max += 1024;
        p_sys->index = (mkv_index_t*)realloc( p_sys->index, sizeof( mkv_index_t ) * p_sys->i_index_max );
    }
#undef idx
}

static char * UTF8ToStr( const UTFstring &u )
{
    int     i_src;
    const wchar_t *src;
    char *dst, *p;

    i_src = u.length();
    src   = u.c_str();

    p = dst = (char*)malloc( i_src + 1);
    while( i_src > 0 )
    {
        if( *src < 255 )
        {
            *p++ = (char)*src;
        }
        else
        {
            *p++ = '?';
        }
        src++;
        i_src--;
    }
    *p++= '\0';

    return dst;
}

