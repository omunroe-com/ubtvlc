/*****************************************************************************
 * mkv.cpp : matroska demuxer
 *****************************************************************************
 * Copyright (C) 2003-2004 VideoLAN
 * $Id: mkv.cpp 7722 2004-05-19 21:01:58Z hartman $
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
#ifdef HAVE_MATROSKA_KAXATTACHMENTS_H
#include "matroska/KaxAttachments.h"
#else
#include "matroska/KaxAttachements.h"
#endif
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

#include "ebml/StdIOCallback.h"

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

    virtual uint32_t read            ( void *p_buffer, size_t i_size);
    virtual void     setFilePointer  ( int64_t i_offset, seek_mode mode = seek_beginning );
    virtual size_t   write           ( const void *p_buffer, size_t i_size);
    virtual uint64_t getFilePointer  ( void );
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
    EbmlElement *m_el[6];

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
};

#define MKVD_TIMECODESCALE 1000000

static void IndexAppendCluster  ( demux_t *p_demux, KaxCluster *cluster );
static char *UTF8ToStr          ( const UTFstring &u );
static void LoadCues            ( demux_t * );
static void InformationsCreate  ( demux_t * );

/*****************************************************************************
 * Open: initializes matroska demux structures
 *****************************************************************************/
static int Open( vlc_object_t * p_this )
{
    demux_t     *p_demux = (demux_t*)p_this;
    demux_sys_t *p_sys;
    uint8_t     *p_peek;

    int          i_track;

    EbmlElement *el = NULL, *el1 = NULL, *el2 = NULL, *el3 = NULL, *el4 = NULL;

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
    msg_Dbg( p_demux, "+ Segment" );
    p_sys->segment = (KaxSegment*)el;
    p_sys->cluster = NULL;

    p_sys->ep = new EbmlParser( p_sys->es, el );

    while( ( el1 = p_sys->ep->Get() ) != NULL )
    {
        if( EbmlId( *el1 ) == KaxInfo::ClassInfos.GlobalId )
        {
            msg_Dbg( p_demux, "|   + Informations" );

            p_sys->ep->Down();
            while( ( el2 = p_sys->ep->Get() ) != NULL )
            {
                if( EbmlId( *el2 ) == KaxTimecodeScale::ClassInfos.GlobalId )
                {
                    KaxTimecodeScale &tcs = *(KaxTimecodeScale*)el2;

                    tcs.ReadData( p_sys->es->I_O() );
                    p_sys->i_timescale = uint64(tcs);

                    msg_Dbg( p_demux, "|   |   + TimecodeScale="I64Fd,
                             p_sys->i_timescale );
                }
                else if( EbmlId( *el2 ) == KaxDuration::ClassInfos.GlobalId )
                {
                    KaxDuration &dur = *(KaxDuration*)el2;

                    dur.ReadData( p_sys->es->I_O() );
                    p_sys->f_duration = float(dur);

                    msg_Dbg( p_demux, "|   |   + Duration=%f",
                             p_sys->f_duration );
                }
                else if( EbmlId( *el2 ) == KaxMuxingApp::ClassInfos.GlobalId )
                {
                    KaxMuxingApp &mapp = *(KaxMuxingApp*)el2;

                    mapp.ReadData( p_sys->es->I_O() );

                    p_sys->psz_muxing_application = UTF8ToStr( UTFstring( mapp ) );

                    msg_Dbg( p_demux, "|   |   + Muxing Application=%s",
                             p_sys->psz_muxing_application );
                }
                else if( EbmlId( *el2 ) == KaxWritingApp::ClassInfos.GlobalId )
                {
                    KaxWritingApp &wapp = *(KaxWritingApp*)el2;

                    wapp.ReadData( p_sys->es->I_O() );

                    p_sys->psz_writing_application = UTF8ToStr( UTFstring( wapp ) );

                    msg_Dbg( p_demux, "|   |   + Writing Application=%s",
                             p_sys->psz_writing_application );
                }
                else if( EbmlId( *el2 ) == KaxSegmentFilename::ClassInfos.GlobalId )
                {
                    KaxSegmentFilename &sfn = *(KaxSegmentFilename*)el2;

                    sfn.ReadData( p_sys->es->I_O() );

                    p_sys->psz_segment_filename = UTF8ToStr( UTFstring( sfn ) );

                    msg_Dbg( p_demux, "|   |   + Segment Filename=%s",
                             p_sys->psz_segment_filename );
                }
                else if( EbmlId( *el2 ) == KaxTitle::ClassInfos.GlobalId )
                {
                    KaxTitle &title = *(KaxTitle*)el2;

                    title.ReadData( p_sys->es->I_O() );

                    p_sys->psz_title = UTF8ToStr( UTFstring( title ) );

                    msg_Dbg( p_demux, "|   |   + Title=%s", p_sys->psz_title );
                }
#if defined( HAVE_GMTIME_R ) && !defined( SYS_DARWIN )
                else if( EbmlId( *el2 ) == KaxDateUTC::ClassInfos.GlobalId )
                {
                    KaxDateUTC &date = *(KaxDateUTC*)el2;
                    time_t i_date;
                    struct tm tmres;
                    char   buffer[256];

                    date.ReadData( p_sys->es->I_O() );

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
                    msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid(*el2).name() );
                }
            }
            p_sys->ep->Up();
        }
        else if( EbmlId( *el1 ) == KaxTracks::ClassInfos.GlobalId )
        {
            msg_Dbg( p_demux, "|   + Tracks" );

            p_sys->ep->Down();
            while( ( el2 = p_sys->ep->Get() ) != NULL )
            {
                if( EbmlId( *el2 ) == KaxTrackEntry::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   |   + Track Entry" );

                    p_sys->i_track++;
                    p_sys->track = (mkv_track_t*)realloc( p_sys->track, sizeof( mkv_track_t ) * (p_sys->i_track + 1 ) );
#define tk  p_sys->track[p_sys->i_track - 1]
                    memset( &tk, 0, sizeof( mkv_track_t ) );

                    es_format_Init( &tk.fmt, UNKNOWN_ES, 0 );
                    tk.fmt.psz_language = strdup("English");
                    tk.fmt.psz_description = NULL;

                    tk.b_default = VLC_TRUE;
                    tk.b_enabled = VLC_TRUE;
                    tk.i_number = p_sys->i_track - 1;
                    tk.i_extra_data = 0;
                    tk.p_extra_data = NULL;
                    tk.psz_codec = NULL;
                    tk.i_default_duration = 0;
                    tk.f_timecodescale = 1.0;

                    tk.b_inited = VLC_FALSE;
                    tk.i_data_init = 0;
                    tk.p_data_init = NULL;

                    tk.psz_codec_name = NULL;
                    tk.psz_codec_settings = NULL;
                    tk.psz_codec_info_url = NULL;
                    tk.psz_codec_download_url = NULL;

                    p_sys->ep->Down();

                    while( ( el3 = p_sys->ep->Get() ) != NULL )
                    {
                        if( EbmlId( *el3 ) == KaxTrackNumber::ClassInfos.GlobalId )
                        {
                            KaxTrackNumber &tnum = *(KaxTrackNumber*)el3;
                            tnum.ReadData( p_sys->es->I_O() );

                            tk.i_number = uint32( tnum );
                            msg_Dbg( p_demux, "|   |   |   + Track Number=%u",
                                     uint32( tnum ) );
                        }
                        else  if( EbmlId( *el3 ) == KaxTrackUID::ClassInfos.GlobalId )
                        {
                            KaxTrackUID &tuid = *(KaxTrackUID*)el3;
                            tuid.ReadData( p_sys->es->I_O() );

                            msg_Dbg( p_demux, "|   |   |   + Track UID=%u",
                                     uint32( tuid ) );
                        }
                        else  if( EbmlId( *el3 ) == KaxTrackType::ClassInfos.GlobalId )
                        {
                            char *psz_type;
                            KaxTrackType &ttype = *(KaxTrackType*)el3;
                            ttype.ReadData( p_sys->es->I_O() );
                            switch( uint8(ttype) )
                            {
                                case track_audio:
                                    psz_type = "audio";
                                    tk.fmt.i_cat = AUDIO_ES;
                                    break;
                                case track_video:
                                    psz_type = "video";
                                    tk.fmt.i_cat = VIDEO_ES;
                                    break;
                                case track_subtitle:
                                    psz_type = "subtitle";
                                    tk.fmt.i_cat = SPU_ES;
                                    break;
                                default:
                                    psz_type = "unknown";
                                    tk.fmt.i_cat = UNKNOWN_ES;
                                    break;
                            }

                            msg_Dbg( p_demux, "|   |   |   + Track Type=%s",
                                     psz_type );
                        }
//                         else  if( EbmlId( *el3 ) == KaxTrackFlagEnabled::ClassInfos.GlobalId )
//                         {
//                             KaxTrackFlagEnabled &fenb = *(KaxTrackFlagEnabled*)el3;
//                             fenb.ReadData( p_sys->es->I_O() );

//                             tk.b_enabled = uint32( fenb );
//                             msg_Dbg( p_demux, "|   |   |   + Track Enabled=%u",
//                                      uint32( fenb )  );
//                         }
                        else  if( EbmlId( *el3 ) == KaxTrackFlagDefault::ClassInfos.GlobalId )
                        {
                            KaxTrackFlagDefault &fdef = *(KaxTrackFlagDefault*)el3;
                            fdef.ReadData( p_sys->es->I_O() );

                            tk.b_default = uint32( fdef );
                            msg_Dbg( p_demux, "|   |   |   + Track Default=%u",
                                     uint32( fdef )  );
                        }
                        else  if( EbmlId( *el3 ) == KaxTrackFlagLacing::ClassInfos.GlobalId )
                        {
                            KaxTrackFlagLacing &lac = *(KaxTrackFlagLacing*)el3;
                            lac.ReadData( p_sys->es->I_O() );

                            msg_Dbg( p_demux, "|   |   |   + Track Lacing=%d",
                                     uint32( lac ) );
                        }
                        else  if( EbmlId( *el3 ) == KaxTrackMinCache::ClassInfos.GlobalId )
                        {
                            KaxTrackMinCache &cmin = *(KaxTrackMinCache*)el3;
                            cmin.ReadData( p_sys->es->I_O() );

                            msg_Dbg( p_demux, "|   |   |   + Track MinCache=%d",
                                     uint32( cmin ) );
                        }
                        else  if( EbmlId( *el3 ) == KaxTrackMaxCache::ClassInfos.GlobalId )
                        {
                            KaxTrackMaxCache &cmax = *(KaxTrackMaxCache*)el3;
                            cmax.ReadData( p_sys->es->I_O() );

                            msg_Dbg( p_demux, "|   |   |   + Track MaxCache=%d",
                                     uint32( cmax ) );
                        }
                        else  if( EbmlId( *el3 ) == KaxTrackDefaultDuration::ClassInfos.GlobalId )
                        {
                            KaxTrackDefaultDuration &defd = *(KaxTrackDefaultDuration*)el3;
                            defd.ReadData( p_sys->es->I_O() );

                            tk.i_default_duration = uint64(defd);
                            msg_Dbg( p_demux, "|   |   |   + Track Default Duration="I64Fd, uint64(defd) );
                        }
                        else  if( EbmlId( *el3 ) == KaxTrackTimecodeScale::ClassInfos.GlobalId )
                        {
                            KaxTrackTimecodeScale &ttcs = *(KaxTrackTimecodeScale*)el3;
                            ttcs.ReadData( p_sys->es->I_O() );

                            tk.f_timecodescale = float( ttcs );
                            msg_Dbg( p_demux, "|   |   |   + Track TimeCodeScale=%f", tk.f_timecodescale );
                        }
                        else if( EbmlId( *el3 ) == KaxTrackName::ClassInfos.GlobalId )
                        {
                            KaxTrackName &tname = *(KaxTrackName*)el3;
                            tname.ReadData( p_sys->es->I_O() );

                            tk.fmt.psz_description = UTF8ToStr( UTFstring( tname ) );
                            msg_Dbg( p_demux, "|   |   |   + Track Name=%s",
                                     tk.fmt.psz_description );
                        }
                        else  if( EbmlId( *el3 ) == KaxTrackLanguage::ClassInfos.GlobalId )
                        {
                            KaxTrackLanguage &lang = *(KaxTrackLanguage*)el3;
                            lang.ReadData( p_sys->es->I_O() );

                            tk.fmt.psz_language = strdup( string( lang ).c_str() );
                            msg_Dbg( p_demux,
                                     "|   |   |   + Track Language=`%s'",
                                     tk.fmt.psz_language );
                        }
                        else  if( EbmlId( *el3 ) == KaxCodecID::ClassInfos.GlobalId )
                        {
                            KaxCodecID &codecid = *(KaxCodecID*)el3;
                            codecid.ReadData( p_sys->es->I_O() );

                            tk.psz_codec = strdup( string( codecid ).c_str() );
                            msg_Dbg( p_demux, "|   |   |   + Track CodecId=%s",
                                     string( codecid ).c_str() );
                        }
                        else  if( EbmlId( *el3 ) == KaxCodecPrivate::ClassInfos.GlobalId )
                        {
                            KaxCodecPrivate &cpriv = *(KaxCodecPrivate*)el3;
                            cpriv.ReadData( p_sys->es->I_O(), SCOPE_ALL_DATA );

                            tk.i_extra_data = cpriv.GetSize();
                            if( tk.i_extra_data > 0 )
                            {
                                tk.p_extra_data = (uint8_t*)malloc( tk.i_extra_data );
                                memcpy( tk.p_extra_data, cpriv.GetBuffer(), tk.i_extra_data );
                            }
                            msg_Dbg( p_demux, "|   |   |   + Track CodecPrivate size="I64Fd, cpriv.GetSize() );
                        }
                        else if( EbmlId( *el3 ) == KaxCodecName::ClassInfos.GlobalId )
                        {
                            KaxCodecName &cname = *(KaxCodecName*)el3;
                            cname.ReadData( p_sys->es->I_O() );

                            tk.psz_codec_name = UTF8ToStr( UTFstring( cname ) );
                            msg_Dbg( p_demux, "|   |   |   + Track Codec Name=%s", tk.psz_codec_name );
                        }
//                         else if( EbmlId( *el3 ) == KaxCodecSettings::ClassInfos.GlobalId )
//                         {
//                             KaxCodecSettings &cset = *(KaxCodecSettings*)el3;
//                             cset.ReadData( p_sys->es->I_O() );

//                             tk.psz_codec_settings = UTF8ToStr( UTFstring( cset ) );
//                             msg_Dbg( p_demux, "|   |   |   + Track Codec Settings=%s", tk.psz_codec_settings );
//                         }
//                         else if( EbmlId( *el3 ) == KaxCodecInfoURL::ClassInfos.GlobalId )
//                         {
//                             KaxCodecInfoURL &ciurl = *(KaxCodecInfoURL*)el3;
//                             ciurl.ReadData( p_sys->es->I_O() );

//                             tk.psz_codec_info_url = strdup( string( ciurl ).c_str() );
//                             msg_Dbg( p_demux, "|   |   |   + Track Codec Info URL=%s", tk.psz_codec_info_url );
//                         }
//                         else if( EbmlId( *el3 ) == KaxCodecDownloadURL::ClassInfos.GlobalId )
//                         {
//                             KaxCodecDownloadURL &cdurl = *(KaxCodecDownloadURL*)el3;
//                             cdurl.ReadData( p_sys->es->I_O() );

//                             tk.psz_codec_download_url = strdup( string( cdurl ).c_str() );
//                             msg_Dbg( p_demux, "|   |   |   + Track Codec Info URL=%s", tk.psz_codec_download_url );
//                         }
//                         else if( EbmlId( *el3 ) == KaxCodecDecodeAll::ClassInfos.GlobalId )
//                         {
//                             KaxCodecDecodeAll &cdall = *(KaxCodecDecodeAll*)el3;
//                             cdall.ReadData( p_sys->es->I_O() );

//                             msg_Dbg( p_demux, "|   |   |   + Track Codec Decode All=%u <== UNUSED", uint8( cdall ) );
//                         }
//                         else if( EbmlId( *el3 ) == KaxTrackOverlay::ClassInfos.GlobalId )
//                         {
//                             KaxTrackOverlay &tovr = *(KaxTrackOverlay*)el3;
//                             tovr.ReadData( p_sys->es->I_O() );

//                             msg_Dbg( p_demux, "|   |   |   + Track Overlay=%u <== UNUSED", uint32( tovr ) );
//                         }
                        else  if( EbmlId( *el3 ) == KaxTrackVideo::ClassInfos.GlobalId )
                        {
                            msg_Dbg( p_demux, "|   |   |   + Track Video" );
                            tk.f_fps = 0.0;

                            p_sys->ep->Down();

                            while( ( el4 = p_sys->ep->Get() ) != NULL )
                            {
//                                 if( EbmlId( *el4 ) == KaxVideoFlagInterlaced::ClassInfos.GlobalId )
//                                 {
//                                     KaxVideoFlagInterlaced &fint = *(KaxVideoFlagInterlaced*)el4;
//                                     fint.ReadData( p_sys->es->I_O() );

//                                     msg_Dbg( p_demux, "|   |   |   |   + Track Video Interlaced=%u", uint8( fint ) );
//                                 }
//                                 else if( EbmlId( *el4 ) == KaxVideoStereoMode::ClassInfos.GlobalId )
//                                 {
//                                     KaxVideoStereoMode &stereo = *(KaxVideoStereoMode*)el4;
//                                     stereo.ReadData( p_sys->es->I_O() );

//                                     msg_Dbg( p_demux, "|   |   |   |   + Track Video Stereo Mode=%u", uint8( stereo ) );
//                                 }
//                                 else 
                              if( EbmlId( *el4 ) == KaxVideoPixelWidth::ClassInfos.GlobalId )
                                {
                                    KaxVideoPixelWidth &vwidth = *(KaxVideoPixelWidth*)el4;
                                    vwidth.ReadData( p_sys->es->I_O() );

                                    tk.fmt.video.i_width = uint16( vwidth );
                                    msg_Dbg( p_demux, "|   |   |   |   + width=%d", uint16( vwidth ) );
                                }
                                else if( EbmlId( *el4 ) == KaxVideoPixelHeight::ClassInfos.GlobalId )
                                {
                                    KaxVideoPixelWidth &vheight = *(KaxVideoPixelWidth*)el4;
                                    vheight.ReadData( p_sys->es->I_O() );

                                    tk.fmt.video.i_height = uint16( vheight );
                                    msg_Dbg( p_demux, "|   |   |   |   + height=%d", uint16( vheight ) );
                                }
                                else if( EbmlId( *el4 ) == KaxVideoDisplayWidth::ClassInfos.GlobalId )
                                {
                                    KaxVideoDisplayWidth &vwidth = *(KaxVideoDisplayWidth*)el4;
                                    vwidth.ReadData( p_sys->es->I_O() );

                                    tk.fmt.video.i_visible_width = uint16( vwidth );
                                    msg_Dbg( p_demux, "|   |   |   |   + display width=%d", uint16( vwidth ) );
                                }
                                else if( EbmlId( *el4 ) == KaxVideoDisplayHeight::ClassInfos.GlobalId )
                                {
                                    KaxVideoDisplayWidth &vheight = *(KaxVideoDisplayWidth*)el4;
                                    vheight.ReadData( p_sys->es->I_O() );

                                    tk.fmt.video.i_visible_height = uint16( vheight );
                                    msg_Dbg( p_demux, "|   |   |   |   + display height=%d", uint16( vheight ) );
                                }
                                else if( EbmlId( *el4 ) == KaxVideoFrameRate::ClassInfos.GlobalId )
                                {
                                    KaxVideoFrameRate &vfps = *(KaxVideoFrameRate*)el4;
                                    vfps.ReadData( p_sys->es->I_O() );

                                    tk.f_fps = float( vfps );
                                    msg_Dbg( p_demux, "   |   |   |   + fps=%f", float( vfps ) );
                                }
//                                 else if( EbmlId( *el4 ) == KaxVideoDisplayUnit::ClassInfos.GlobalId )
//                                 {
//                                      KaxVideoDisplayUnit &vdmode = *(KaxVideoDisplayUnit*)el4;
//                                     vdmode.ReadData( p_sys->es->I_O() );

//                                     msg_Dbg( p_demux, "|   |   |   |   + Track Video Display Unit=%s",
//                                              uint8( vdmode ) == 0 ? "pixels" : ( uint8( vdmode ) == 1 ? "centimeters": "inches" ) );
//                                 }
//                                 else if( EbmlId( *el4 ) == KaxVideoAspectRatio::ClassInfos.GlobalId )
//                                 {
//                                     KaxVideoAspectRatio &ratio = *(KaxVideoAspectRatio*)el4;
//                                     ratio.ReadData( p_sys->es->I_O() );

//                                     msg_Dbg( p_demux, "   |   |   |   + Track Video Aspect Ratio Type=%u", uint8( ratio ) );
//                                 }
//                                 else if( EbmlId( *el4 ) == KaxVideoGamma::ClassInfos.GlobalId )
//                                 {
//                                     KaxVideoGamma &gamma = *(KaxVideoGamma*)el4;
//                                     gamma.ReadData( p_sys->es->I_O() );

//                                     msg_Dbg( p_demux, "   |   |   |   + fps=%f", float( gamma ) );
//                                 }
                                else
                                {
                                    msg_Dbg( p_demux, "|   |   |   |   + Unknown (%s)", typeid(*el4).name() );
                                }
                            }
                            p_sys->ep->Up();
                        }
                        else  if( EbmlId( *el3 ) == KaxTrackAudio::ClassInfos.GlobalId )
                        {
                            msg_Dbg( p_demux, "|   |   |   + Track Audio" );

                            p_sys->ep->Down();

                            while( ( el4 = p_sys->ep->Get() ) != NULL )
                            {
                                if( EbmlId( *el4 ) == KaxAudioSamplingFreq::ClassInfos.GlobalId )
                                {
                                    KaxAudioSamplingFreq &afreq = *(KaxAudioSamplingFreq*)el4;
                                    afreq.ReadData( p_sys->es->I_O() );

                                    tk.fmt.audio.i_rate = (int)float( afreq );
                                    msg_Dbg( p_demux, "|   |   |   |   + afreq=%d", tk.fmt.audio.i_rate );
                                }
                                else if( EbmlId( *el4 ) == KaxAudioChannels::ClassInfos.GlobalId )
                                {
                                    KaxAudioChannels &achan = *(KaxAudioChannels*)el4;
                                    achan.ReadData( p_sys->es->I_O() );

                                    tk.fmt.audio.i_channels = uint8( achan );
                                    msg_Dbg( p_demux, "|   |   |   |   + achan=%u", uint8( achan ) );
                                }
                                else if( EbmlId( *el4 ) == KaxAudioBitDepth::ClassInfos.GlobalId )
                                {
                                    KaxAudioBitDepth &abits = *(KaxAudioBitDepth*)el4;
                                    abits.ReadData( p_sys->es->I_O() );

                                    tk.fmt.audio.i_bitspersample = uint8( abits );
                                    msg_Dbg( p_demux, "|   |   |   |   + abits=%u", uint8( abits ) );
                                }
                                else
                                {
                                    msg_Dbg( p_demux, "|   |   |   |   + Unknown (%s)", typeid(*el4).name() );
                                }
                            }
                            p_sys->ep->Up();
                        }
                        else
                        {
                            msg_Dbg( p_demux, "|   |   |   + Unknown (%s)",
                                     typeid(*el3).name() );
                        }
                    }
                    p_sys->ep->Up();
                }
                else
                {
                    msg_Dbg( p_demux, "|   |   + Unknown (%s)",
                             typeid(*el2).name() );
                }
#undef tk
            }
            p_sys->ep->Up();
        }
        else if( EbmlId( *el1 ) == KaxSeekHead::ClassInfos.GlobalId )
        {
            msg_Dbg( p_demux, "|   + Seek head" );
            p_sys->ep->Down();
            while( ( el = p_sys->ep->Get() ) != NULL )
            {
                if( EbmlId( *el ) == KaxSeek::ClassInfos.GlobalId )
                {
                    EbmlId id = EbmlVoid::ClassInfos.GlobalId;
                    int64_t i_pos = -1;

                    //msg_Dbg( p_demux, "|   |   + Seek" );
                    p_sys->ep->Down();
                    while( ( el = p_sys->ep->Get() ) != NULL )
                    {
                        if( EbmlId( *el ) == KaxSeekID::ClassInfos.GlobalId )
                        {
                            KaxSeekID &sid = *(KaxSeekID*)el;

                            sid.ReadData( p_sys->es->I_O(), SCOPE_ALL_DATA );

                            id = EbmlId( sid.GetBuffer(), sid.GetSize() );
                        }
                        else  if( EbmlId( *el ) == KaxSeekPosition::ClassInfos.GlobalId )
                        {
                            KaxSeekPosition &spos = *(KaxSeekPosition*)el;

                            spos.ReadData( p_sys->es->I_O(), SCOPE_ALL_DATA );

                            i_pos = uint64( spos );
                        }
                        else
                        {
                            msg_Dbg( p_demux, "|   |   |   + Unknown (%s)",
                                     typeid(*el).name() );
                        }
                    }
                    p_sys->ep->Up();

                    if( i_pos >= 0 )
                    {
                        if( id == KaxCues::ClassInfos.GlobalId )
                        {
                            msg_Dbg( p_demux, "|   |   |   = cues at "I64Fd,
                                     i_pos );
                            p_sys->i_cues_position = p_sys->segment->GetGlobalPosition( i_pos );
                        }
                        else if( id == KaxChapters::ClassInfos.GlobalId )
                        {
                            msg_Dbg( p_demux, "|   |   |   = chapters at "I64Fd,
                                     i_pos );
                            p_sys->i_chapters_position = p_sys->segment->GetGlobalPosition( i_pos );
                        }
                        else if( id == KaxTags::ClassInfos.GlobalId )
                        {
                            msg_Dbg( p_demux, "|   |   |   = tags at "I64Fd,
                                     i_pos );
                            p_sys->i_tags_position = p_sys->segment->GetGlobalPosition( i_pos );
                        }

                    }
                }
                else
                {
                    msg_Dbg( p_demux, "|   |   + Unknown (%s)",
                             typeid(*el).name() );
                }
            }
            p_sys->ep->Up();
        }
        else if( EbmlId( *el1 ) == KaxCues::ClassInfos.GlobalId )
        {
            msg_Dbg( p_demux, "|   + Cues" );
        }
        else if( EbmlId( *el1 ) == KaxCluster::ClassInfos.GlobalId )
        {
            msg_Dbg( p_demux, "|   + Cluster" );

            p_sys->cluster = (KaxCluster*)el1;

            p_sys->ep->Down();
            /* stop parsing the stream */
            break;
        }
#ifdef HAVE_MATROSKA_KAXATTACHMENTS_H
        else if( EbmlId( *el1 ) == KaxAttachments::ClassInfos.GlobalId )
#else
        else if( EbmlId( *el1 ) == KaxAttachements::ClassInfos.GlobalId )
#endif
        {
            msg_Dbg( p_demux, "|   + Attachments FIXME TODO (but probably never supported)" );
        }
        else if( EbmlId( *el1 ) == KaxChapters::ClassInfos.GlobalId )
        {
            msg_Dbg( p_demux, "|   + Chapters FIXME TODO" );
        }
        else if( EbmlId( *el1 ) == KaxTag::ClassInfos.GlobalId )
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

    if( p_sys->i_chapters_position >= 0 )
    {
        msg_Warn( p_demux, "chapters unsupported" );
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
            tk.fmt.i_codec = VLC_FOURCC( 'v', 'o', 'r', 'b' );
            tk.i_data_init = tk.i_extra_data;
            tk.p_data_init = tk.p_extra_data;
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
        }
        else
        {
            msg_Err( p_demux, "unknow codec id=`%s'", tk.psz_codec );
            tk.fmt.i_codec = VLC_FOURCC( 'u', 'n', 'd', 'f' );
        }

        tk.p_es = es_out_Add( p_demux->out, &tk.fmt );
#undef tk
    }

    /* add informations */
    InformationsCreate( p_demux );

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
            return VLC_EGENERIC;

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
                idx.i_time        = (*pp_block)->GlobalTimecode() * (mtime_t) 1000 / p_sys->i_timescale;
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
            if( EbmlId( *el ) == KaxCluster::ClassInfos.GlobalId )
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
            else if( EbmlId( *el ) == KaxCues::ClassInfos.GlobalId )
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
            if( EbmlId( *el ) == KaxClusterTimecode::ClassInfos.GlobalId )
            {
                KaxClusterTimecode &ctc = *(KaxClusterTimecode*)el;

                ctc.ReadData( p_sys->es->I_O(), SCOPE_ALL_DATA );
                p_sys->cluster->InitTimecode( uint64( ctc ), p_sys->i_timescale );
            }
            else if( EbmlId( *el ) == KaxBlockGroup::ClassInfos.GlobalId )
            {
                p_sys->ep->Down();
            }
        }
        else if( i_level == 3 )
        {
            if( EbmlId( *el ) == KaxBlock::ClassInfos.GlobalId )
            {
                *pp_block = (KaxBlock*)el;

                (*pp_block)->ReadData( p_sys->es->I_O() );
                (*pp_block)->SetParent( *p_sys->cluster );

                p_sys->ep->Keep();
            }
            else if( EbmlId( *el ) == KaxBlockDuration::ClassInfos.GlobalId )
            {
                KaxBlockDuration &dur = *(KaxBlockDuration*)el;

                dur.ReadData( p_sys->es->I_O() );
                *pi_duration = uint64( dur );
            }
            else if( EbmlId( *el ) == KaxReferenceBlock::ClassInfos.GlobalId )
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

static void BlockDecode( demux_t *p_demux, KaxBlock *block, mtime_t i_pts, mtime_t i_duration )
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

        if( tk.fmt.i_codec == VLC_FOURCC( 'v', 'o', 'r', 'b' ) )
        {
            int i;
            int i_offset = 1;
            int i_size[3];

            /* XXX hack split the 3 headers */
            if( tk.p_data_init[0] != 0x02 )
            {
                msg_Err( p_demux, "invalid vorbis header" );
            }

            for( i = 0; i < 2; i++ )
            {
                i_size[i] = 0;
                while( i_offset < tk.i_data_init )
                {
                    i_size[i] += tk.p_data_init[i_offset];
                    if( tk.p_data_init[i_offset++] != 0xff )
                    {
                        break;
                    }
                }
            }
            i_size[0] = __MIN( i_size[0], tk.i_data_init - i_offset );
            i_size[1] = __MIN( i_size[1], tk.i_data_init - i_offset - i_size[0] );
            i_size[2] = tk.i_data_init - i_offset - i_size[0] - i_size[1];

            p_init = MemToBlock( p_demux, &tk.p_data_init[i_offset], i_size[0] );
            if( p_init )
            {
                es_out_Send( p_demux->out, tk.p_es, p_init );
            }
            p_init = MemToBlock( p_demux, &tk.p_data_init[i_offset+i_size[0]], i_size[1] );
            if( p_init )
            {
                es_out_Send( p_demux->out, tk.p_es, p_init );
            }
            p_init = MemToBlock( p_demux, &tk.p_data_init[i_offset+i_size[0]+i_size[1]], i_size[2] );
            if( p_init )
            {
                es_out_Send( p_demux->out, tk.p_es, p_init );
            }
        }
        else
        {
            p_init = MemToBlock( p_demux, tk.p_data_init, tk.i_data_init );
            if( p_init )
            {
                es_out_Send( p_demux->out, tk.p_es, p_init );
            }
        }
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
                if( EbmlId( *el ) == KaxCluster::ClassInfos.GlobalId )
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

        p_sys->i_pts = block->GlobalTimecode() * (mtime_t) 1000 / p_sys->i_timescale + 1;

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

        p_sys->i_pts = block->GlobalTimecode() * (mtime_t) 1000 / p_sys->i_timescale + 1;

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

uint32_t vlc_stream_io_callback::read( void *p_buffer, size_t i_size )
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
uint64_t vlc_stream_io_callback::getFilePointer( void )
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
 *  * InformationsCreate : create all informations, load tags if present
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
        if( EbmlId( *el ) == KaxCuePoint::ClassInfos.GlobalId )
        {
#define idx p_sys->index[p_sys->i_index]

            idx.i_track       = -1;
            idx.i_block_number= -1;
            idx.i_position    = -1;
            idx.i_time        = -1;
            idx.b_key         = VLC_TRUE;

            ep->Down();
            while( ( el = ep->Get() ) != NULL )
            {
                if( EbmlId( *el ) == KaxCueTime::ClassInfos.GlobalId )
                {
                    KaxCueTime &ctime = *(KaxCueTime*)el;

                    ctime.ReadData( p_sys->es->I_O() );

                    idx.i_time = uint64( ctime ) * (mtime_t)1000000000 / p_sys->i_timescale;
                }
                else if( EbmlId( *el ) == KaxCueTrackPositions::ClassInfos.GlobalId )
                {
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        if( EbmlId( *el ) == KaxCueTrack::ClassInfos.GlobalId )
                        {
                            KaxCueTrack &ctrack = *(KaxCueTrack*)el;

                            ctrack.ReadData( p_sys->es->I_O() );
                            idx.i_track = uint16( ctrack );
                        }
                        else if( EbmlId( *el ) == KaxCueClusterPosition::ClassInfos.GlobalId )
                        {
                            KaxCueClusterPosition &ccpos = *(KaxCueClusterPosition*)el;

                            ccpos.ReadData( p_sys->es->I_O() );
                            idx.i_position = p_sys->segment->GetGlobalPosition( uint64( ccpos ) );
                        }
                        else if( EbmlId( *el ) == KaxCueBlockNumber::ClassInfos.GlobalId )
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
        if( EbmlId( *el ) == KaxTag::ClassInfos.GlobalId )
        {
            msg_Dbg( p_demux, "+ Tag" );
            ep->Down();
            while( ( el = ep->Get() ) != NULL )
            {
                if( EbmlId( *el ) == KaxTagTargets::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   + Targets" );
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid( *el ).name() );
                    }
                    ep->Up();
                }
                else if( EbmlId( *el ) == KaxTagGeneral::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   + General" );
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid( *el ).name() );
                    }
                    ep->Up();
                }
                else if( EbmlId( *el ) == KaxTagGenres::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   + Genres" );
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid( *el ).name() );
                    }
                    ep->Up();
                }
                else if( EbmlId( *el ) == KaxTagAudioSpecific::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   + Audio Specific" );
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid( *el ).name() );
                    }
                    ep->Up();
                }
                else if( EbmlId( *el ) == KaxTagImageSpecific::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   + Images Specific" );
                    ep->Down();
                    while( ( el = ep->Get() ) != NULL )
                    {
                        msg_Dbg( p_demux, "|   |   + Unknown (%s)", typeid( *el ).name() );
                    }
                    ep->Up();
                }
                else if( EbmlId( *el ) == KaxTagMultiComment::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   + Multi Comment" );
                }
                else if( EbmlId( *el ) == KaxTagMultiCommercial::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   + Multi Commercial" );
                }
                else if( EbmlId( *el ) == KaxTagMultiDate::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   + Multi Date" );
                }
                else if( EbmlId( *el ) == KaxTagMultiEntity::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   + Multi Entity" );
                }
                else if( EbmlId( *el ) == KaxTagMultiIdentifier::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   + Multi Identifier" );
                }
                else if( EbmlId( *el ) == KaxTagMultiLegal::ClassInfos.GlobalId )
                {
                    msg_Dbg( p_demux, "|   + Multi Legal" );
                }
                else if( EbmlId( *el ) == KaxTagMultiTitle::ClassInfos.GlobalId )
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

static void InformationsCreate( demux_t *p_demux )
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

