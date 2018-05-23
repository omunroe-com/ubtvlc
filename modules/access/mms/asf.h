/*****************************************************************************
 * asf.h: MMS access plug-in
 *****************************************************************************
 * Copyright (C) 2001-2004 VideoLAN
 * $Id: asf.h 7903 2004-06-05 21:53:24Z fenrir $
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

/****************************************************************************
 * XXX:
 *  Definitions and data duplicated from asf demuxers but I want access
 * and demux plugins to be independant
 *
 ****************************************************************************/

#define ASF_STREAM_VIDEO    0x0001
#define ASF_STREAM_AUDIO    0x0002
#define ASF_STREAM_UNKNOWN  0xffff

typedef struct
{
    int i_cat;      /* ASF_STREAM_VIDEO, ASF_STREAM_AUDIO */
    int i_bitrate;  /* -1 if unknown */
    int i_selected;
} asf_stream_t;

typedef struct
{
    int64_t      i_file_size;
    int64_t      i_data_packets_count;
    int32_t      i_min_data_packet_size;

    asf_stream_t stream[128];

} asf_header_t;


typedef struct guid_s
{
    uint32_t v1; /* le */
    uint16_t v2; /* le */
    uint16_t v3; /* le */
    uint8_t  v4[8];
} guid_t;


void E_( GenerateGuid )     ( guid_t * );
void E_( asf_HeaderParse )  ( asf_header_t *, uint8_t *, int );
void E_( asf_StreamSelect ) ( asf_header_t *,
                              int i_bitrate_max, vlc_bool_t b_all, vlc_bool_t b_audio,
                              vlc_bool_t b_video );


#define GUID_FMT "%8.8x-%4.4x-%4.4x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x"
#define GUID_PRINT( guid )  \
    (guid).v1,              \
    (guid).v2,              \
    (guid).v3,              \
    (guid).v4[0],(guid).v4[1],(guid).v4[2],(guid).v4[3],    \
    (guid).v4[4],(guid).v4[5],(guid).v4[6],(guid).v4[7]

static const guid_t asf_object_header_guid =
{
    0x75B22630,
    0x668E,
    0x11CF,
    { 0xA6,0xD9, 0x00,0xAA,0x00,0x62,0xCE,0x6C }
};

static const guid_t asf_object_file_properties_guid =
{
    0x8cabdca1,
    0xa947,
    0x11cf,
    { 0x8e,0xe4, 0x00,0xC0,0x0C,0x20,0x53,0x65 }
};

static const guid_t asf_object_stream_properties_guid =
{
    0xB7DC0791,
    0xA9B7,
    0x11CF,
    { 0x8E,0xE6, 0x00,0xC0,0x0C,0x20,0x53,0x65 }
};

static const guid_t asf_object_stream_type_audio =
{
    0xF8699E40,
    0x5B4D,
    0x11CF,
    { 0xA8,0xFD, 0x00,0x80,0x5F,0x5C,0x44,0x2B }
};

static const guid_t asf_object_stream_type_video =
{
    0xbc19efc0,
    0x5B4D,
    0x11CF,
    { 0xA8,0xFD, 0x00,0x80,0x5F,0x5C,0x44,0x2B }
};

static const guid_t asf_object_bitrate_properties_guid =
{
    0x7BF875CE,
    0x468D,
    0x11D1,
    { 0x8D,0x82,0x00,0x60,0x97,0xC9,0xA2,0xB2 }
};

static const guid_t asf_object_bitrate_mutual_exclusion_guid =
{
    0xD6E229DC,
    0x35DA,
    0x11D1,
    { 0x90,0x34,0x00,0xA0,0xC9,0x03,0x49,0xBE }
};


