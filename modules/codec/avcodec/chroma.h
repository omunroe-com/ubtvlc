/*****************************************************************************
 * chroma.h: libavutil <-> libvlc conversion routines
 *****************************************************************************
 * Copyright (C) 1999-2008 the VideoLAN team
 * $Id: 3b1d67c30713dd33a73229770c78af4009a6f887 $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Gildas Bazin <gbazin@videolan.org>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 021100301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Chroma fourcc -> ffmpeg_id mapping
 *****************************************************************************/
#if defined(WORDS_BIGENDIAN)
#   define VLC_RGB_ES( fcc, leid, beid ) \
    { fcc, beid, 0, 0, 0 },
#else
#   define VLC_RGB_ES( fcc, leid, beid ) \
    { fcc, leid, 0, 0, 0 },
#endif

#define VLC_RGB( fcc, leid, beid, rmask, gmask, bmask ) \
    { fcc, leid, rmask, gmask, bmask }, \
    { fcc, beid, bmask, gmask, rmask }, \
    VLC_RGB_ES( fcc, leid, beid )


static const struct
{
    vlc_fourcc_t  i_chroma;
    int           i_chroma_id;
    uint32_t      i_rmask;
    uint32_t      i_gmask;
    uint32_t      i_bmask;

} chroma_table[] =
{
    /* Planar YUV formats */
    {VLC_FOURCC('I','4','4','4'), PIX_FMT_YUV444P, 0, 0, 0 },
    {VLC_FOURCC('J','4','4','4'), PIX_FMT_YUVJ444P, 0, 0, 0 },

#if LIBAVUTIL_VERSION_INT >= ((49<<16)+(5<<8)+0)
    {VLC_FOURCC('I','4','4','0'), PIX_FMT_YUV440P, 0, 0, 0 },
    {VLC_FOURCC('J','4','4','0'), PIX_FMT_YUVJ440P, 0, 0, 0 },
#endif

    {VLC_FOURCC('I','4','2','2'), PIX_FMT_YUV422P, 0, 0, 0 },
    {VLC_FOURCC('J','4','2','2'), PIX_FMT_YUVJ422P, 0, 0, 0 },

    {VLC_FOURCC('I','4','2','0'), PIX_FMT_YUV420P, 0, 0, 0 },
    {VLC_FOURCC('Y','V','1','2'), PIX_FMT_YUV420P, 0, 0, 0 },
    {VLC_FOURCC('I','Y','U','V'), PIX_FMT_YUV420P, 0, 0, 0 },
    {VLC_FOURCC('J','4','2','0'), PIX_FMT_YUVJ420P, 0, 0, 0 },
    {VLC_FOURCC('I','4','1','1'), PIX_FMT_YUV411P, 0, 0, 0 },
    {VLC_FOURCC('I','4','1','0'), PIX_FMT_YUV410P, 0, 0, 0 },
    {VLC_FOURCC('Y','V','U','9'), PIX_FMT_YUV410P, 0, 0, 0 },

    {VLC_FOURCC('N','V','1','2'), PIX_FMT_NV12, 0, 0, 0 },
    {VLC_FOURCC('N','V','2','1'), PIX_FMT_NV21, 0, 0, 0 },

    /* Packed YUV formats */
    {VLC_FOURCC('Y','U','Y','2'), PIX_FMT_YUYV422, 0, 0, 0 },
    {VLC_FOURCC('Y','U','Y','V'), PIX_FMT_YUYV422, 0, 0, 0 },
    {VLC_FOURCC('U','Y','V','Y'), PIX_FMT_UYVY422, 0, 0, 0 },
    {VLC_FOURCC('Y','4','1','1'), PIX_FMT_UYYVYY411, 0, 0, 0 },

    /* Packed RGB formats */
    VLC_RGB( VLC_FOURCC('R','G','B','4'), PIX_FMT_RGB4, PIX_FMT_BGR4, 0x10, 0x06, 0x01 )
    VLC_RGB( VLC_FOURCC('R','G','B','8'), PIX_FMT_RGB8, PIX_FMT_BGR8, 0xC0, 0x38, 0x07 )

    VLC_RGB( VLC_FOURCC('R','V','1','5'), PIX_FMT_BGR555, PIX_FMT_RGB555, 0x7c00, 0x03e0, 0x001f )
    VLC_RGB( VLC_FOURCC('R','V','1','6'), PIX_FMT_BGR565, PIX_FMT_RGB565, 0xf800, 0x07e0, 0x001f )
    VLC_RGB( VLC_FOURCC('R','V','2','4'), PIX_FMT_BGR24, PIX_FMT_RGB24, 0xff0000, 0x00ff00, 0x0000ff )

    VLC_RGB( VLC_FOURCC('R','V','3','2'), PIX_FMT_RGB32, PIX_FMT_BGR32, 0x00ff0000, 0x0000ff00, 0x000000ff )
    VLC_RGB( VLC_FOURCC('R','V','3','2'), PIX_FMT_RGB32_1, PIX_FMT_BGR32_1, 0xff000000, 0x00ff0000, 0x0000ff00 )

#if defined(PIX_FMT_RGBA)
    {VLC_FOURCC('R','G','B','A'), PIX_FMT_RGBA, 0xff000000, 0x00ff0000, 0x0000ff00},
#endif
    {VLC_FOURCC('G','R','E','Y'), PIX_FMT_GRAY8, 0, 0, 0},

    { 0, 0, 0, 0, 0 }
};

static inline int TestFfmpegChroma( const int i_ffmpeg_id, const vlc_fourcc_t i_vlc_fourcc )
{
    for( int i = 0; chroma_table[i].i_chroma != 0; i++ )
    {
        if( chroma_table[i].i_chroma == i_vlc_fourcc || chroma_table[i].i_chroma_id == i_ffmpeg_id )
            return VLC_SUCCESS;
    }
    return VLC_EGENERIC;
}

/* FIXME special case the RGB formats */
static inline int GetFfmpegChroma( int *i_ffmpeg_chroma, const video_format_t fmt )
{
    for( int i = 0; chroma_table[i].i_chroma != 0; i++ )
    {
        if( chroma_table[i].i_chroma == fmt.i_chroma )
        {
            if( chroma_table[i].i_rmask == fmt.i_rmask &&
                chroma_table[i].i_gmask == fmt.i_gmask &&
                chroma_table[i].i_bmask == fmt.i_bmask )
            {
                *i_ffmpeg_chroma = chroma_table[i].i_chroma_id;
                return VLC_SUCCESS;
            }
        }
    }
    return VLC_EGENERIC;
}

static inline int GetVlcChroma( video_format_t *fmt, const int i_ffmpeg_chroma )
{
    /* TODO FIXME for rgb format we HAVE to set rgb mask/shift */
    for( int i = 0; chroma_table[i].i_chroma != 0; i++ )
    {
        if( chroma_table[i].i_chroma_id == i_ffmpeg_chroma )
        {
            fmt->i_rmask = chroma_table[i].i_rmask;
            fmt->i_gmask = chroma_table[i].i_gmask;
            fmt->i_bmask = chroma_table[i].i_bmask;
            fmt->i_chroma = chroma_table[i].i_chroma;
            return VLC_SUCCESS;
        }
    }
    return VLC_EGENERIC;
}
