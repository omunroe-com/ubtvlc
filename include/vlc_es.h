/*****************************************************************************
 * vlc_es.h: Elementary stream formats descriptions
 *****************************************************************************
 * Copyright (C) 1999-2001 the VideoLAN team
 * $Id: ad45797bfa591d6a4f77f7d33900f7dabdde1bc1 $
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

#ifndef VLC_ES_H
#define VLC_ES_H 1

/* FIXME: i'm not too sure about this include but it fixes compilation of
 * video chromas -- dionoea */
#include "vlc_common.h"

/**
 * \file
 * This file defines the elementary streams format types
 */

/**
 * video palette data
 * \see video_format_t
 * \see subs_format_t
 */
struct video_palette_t
{
    int i_entries;      /**< to keep the compatibility with ffmpeg's palette */
    uint8_t palette[256][4];                   /**< 4-byte RGBA/YUVA palette */
};

/**
 * audio replay gain description
 */
#define AUDIO_REPLAY_GAIN_MAX (2)
#define AUDIO_REPLAY_GAIN_TRACK (0)
#define AUDIO_REPLAY_GAIN_ALBUM (1)
typedef struct
{
    /* true if we have the peak value */
    bool pb_peak[AUDIO_REPLAY_GAIN_MAX];
    /* peak value where 1.0 means full sample value */
    float      pf_peak[AUDIO_REPLAY_GAIN_MAX];

    /* true if we have the gain value */
    bool pb_gain[AUDIO_REPLAY_GAIN_MAX];
    /* gain value in dB */
    float      pf_gain[AUDIO_REPLAY_GAIN_MAX];
} audio_replay_gain_t;

/**
 * audio format description
 */
struct audio_format_t
{
    vlc_fourcc_t i_format;                          /**< audio format fourcc */
    unsigned int i_rate;                              /**< audio sample-rate */

    /* Describes the channels configuration of the samples (ie. number of
     * channels which are available in the buffer, and positions). */
    uint32_t     i_physical_channels;

    /* Describes from which original channels, before downmixing, the
     * buffer is derived. */
    uint32_t     i_original_channels;

    /* Optional - for A/52, SPDIF and DTS types : */
    /* Bytes used by one compressed frame, depends on bitrate. */
    unsigned int i_bytes_per_frame;

    /* Number of sampleframes contained in one compressed frame. */
    unsigned int i_frame_length;
    /* Please note that it may be completely arbitrary - buffers are not
     * obliged to contain a integral number of so-called "frames". It's
     * just here for the division :
     * buffer_size = i_nb_samples * i_bytes_per_frame / i_frame_length
     */

    /* FIXME ? (used by the codecs) */
    unsigned     i_bitspersample;
    unsigned     i_blockalign;
    uint8_t      i_channels; /* must be <=32 */
    uint8_t      i_flavor;
};

#ifdef WORDS_BIGENDIAN
#   define AUDIO_FMT_S16_NE VLC_FOURCC('s','1','6','b')
#   define AUDIO_FMT_U16_NE VLC_FOURCC('u','1','6','b')
#else
#   define AUDIO_FMT_S16_NE VLC_FOURCC('s','1','6','l')
#   define AUDIO_FMT_U16_NE VLC_FOURCC('u','1','6','l')
#endif

/**
 * video format description
 */
struct video_format_t
{
    vlc_fourcc_t i_chroma;                               /**< picture chroma */
    unsigned int i_aspect;                                 /**< aspect ratio */

    unsigned int i_width;                                 /**< picture width */
    unsigned int i_height;                               /**< picture height */
    unsigned int i_x_offset;               /**< start offset of visible area */
    unsigned int i_y_offset;               /**< start offset of visible area */
    unsigned int i_visible_width;                 /**< width of visible area */
    unsigned int i_visible_height;               /**< height of visible area */

    unsigned int i_bits_per_pixel;             /**< number of bits per pixel */

    unsigned int i_sar_num;                   /**< sample/pixel aspect ratio */
    unsigned int i_sar_den;

    unsigned int i_frame_rate;                     /**< frame rate numerator */
    unsigned int i_frame_rate_base;              /**< frame rate denominator */

    int i_rmask, i_gmask, i_bmask;          /**< color masks for RGB chroma */
    int i_rrshift, i_lrshift;
    int i_rgshift, i_lgshift;
    int i_rbshift, i_lbshift;
    video_palette_t *p_palette;              /**< video palette from demuxer */
};

/**
 * subtitles format description
 */
struct subs_format_t
{
    /* the character encoding of the text of the subtitle.
     * all gettext recognized shorts can be used */
    char *psz_encoding;


    int  i_x_origin; /**< x coordinate of the subtitle. 0 = left */
    int  i_y_origin; /**< y coordinate of the subtitle. 0 = top */

    struct
    {
        /*  */
        uint32_t palette[16+1];

        /* the width of the original movie the spu was extracted from */
        int i_original_frame_width;
        /* the height of the original movie the spu was extracted from */
        int i_original_frame_height;
    } spu;

    struct
    {
        int i_id;
    } dvb;
};

/**
 * ES definition
 */
typedef struct extra_languages_t
{
        char *psz_language;
        char *psz_description;
} extra_languages_t;


struct es_format_t
{
    int             i_cat;
    vlc_fourcc_t    i_codec;

    int             i_id;       /* -1: let the core mark the right id
                                   >=0: valid id */
    int             i_group;    /* -1 : standalone
                                   >= 0 then a "group" (program) is created
                                        for each value */
    int             i_priority; /*  -2 : mean not selectable by the users
                                    -1 : mean not selected by default even
                                        when no other stream
                                    >=0: priority */

    char            *psz_language;
    char            *psz_description;
    int             i_extra_languages;
    extra_languages_t *p_extra_languages;

    audio_format_t  audio;
    audio_replay_gain_t audio_replay_gain;
    video_format_t video;
    subs_format_t  subs;

    unsigned int   i_bitrate;

    bool     b_packetized; /* wether the data is packetized
                                    (ie. not truncated) */
    int     i_extra;
    void    *p_extra;

};

/* ES Categories */
enum es_format_category_e
{
    UNKNOWN_ES = 0x00,
    VIDEO_ES   = 0x01,
    AUDIO_ES   = 0x02,
    SPU_ES     = 0x03,
    NAV_ES     = 0x04,
};

/**
 * This function will fill all RGB shift from RGB masks.
 */
VLC_EXPORT( void, video_format_FixRgb, ( video_format_t * ) );

/**
 * This funtion will initialize a es_format_t structure.
 */
VLC_EXPORT( void, es_format_Init, ( es_format_t *, int i_cat, vlc_fourcc_t i_codec ) );

/**
 * This functions will copy a es_format_t.
 */
VLC_EXPORT( int, es_format_Copy, ( es_format_t *p_dst, const es_format_t *p_src ) );

/**
 * This function will clean up a es_format_t and relasing all associated
 * resources.
 * You can call it multiple times on the same structure.
 */
VLC_EXPORT( void, es_format_Clean, ( es_format_t *fmt ) );

#endif

