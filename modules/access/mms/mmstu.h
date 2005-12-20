/*****************************************************************************
 * mms.h: MMS access plug-in
 *****************************************************************************
 * Copyright (C) 2001, 2002 VideoLAN
 * $Id: mmstu.h 6961 2004-03-05 17:34:23Z sam $
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

#if 0
/* url: [/]host[:port][/path] */
typedef struct url_s
{
    char    *psz_server_addr;
    int     i_server_port;

    char    *psz_bind_addr;
    int     i_bind_port;

    char    *psz_path;

    /* private */
    char *psz_private;
} url_t;
#endif

#define MMS_PACKET_ANY          0
#define MMS_PACKET_CMD          1
#define MMS_PACKET_HEADER       2
#define MMS_PACKET_MEDIA        3
#define MMS_PACKET_UDP_TIMING   4


#define MMS_CMD_HEADERSIZE  48

#if 0
#define MMS_STREAM_VIDEO    0x0001
#define MMS_STREAM_AUDIO    0x0002
#define MMS_STREAM_UNKNOWN  0xffff
typedef struct mms_stream_s
{
    int i_id;       /* 1 -> 127 */
    int i_cat;      /* MMS_STREAM_VIDEO, MMS_STREAM_AUDIO */
    int i_bitrate;  /* -1 if unknown */
    int i_selected;

} mms_stream_t;
#endif

#define MMS_BUFFER_SIZE 100000
struct access_sys_t
{
    int                 i_proto;        /* MMS_PROTO_TCP, MMS_PROTO_UDP */
    input_socket_t      socket_tcp;     /* TCP socket for communication with server */
    input_socket_t      socket_udp;     /* Optional UDP socket for data(media/header packet) */
                                        /* send by server */
    char                *psz_bind_addr; /* used by udp */

    url_t               *p_url;         /* connect to this server */

    //asf_stream_t        stream[128];    /* in asf never more than 1->127 streams */
    asf_header_t        asfh;

    off_t               i_pos;          /* position of next byte to be read */

    /* */
    uint8_t             buffer_tcp[MMS_BUFFER_SIZE];
    int                 i_buffer_tcp;

    uint8_t             buffer_udp[MMS_BUFFER_SIZE];
    int                 i_buffer_udp;

    /* data necessary to send data to server */
    guid_t      guid;
    int         i_command_level;
    int         i_seq_num;
    uint32_t    i_header_packet_id_type;
    uint32_t    i_media_packet_id_type;

    int         i_packet_seq_num;

    uint8_t     *p_cmd;     /* latest command read */
    int         i_cmd;      /* allocated at the begining */

    uint8_t     *p_header;  /* allocated by mms_ReadPacket */
    int         i_header;

    uint8_t     *p_media;   /* allocated by mms_ReadPacket */
    size_t      i_media;
    size_t      i_media_used;

    /* extracted informations */
    int         i_command;
    int         i_eos;

    /* from 0x01 answer (not yet set) */
    char        *psz_server_version;
    char        *psz_tool_version;
    char        *psz_update_player_url;
    char        *psz_encryption_type;

    /* from 0x06 answer */
    uint32_t    i_flags_broadcast;
    uint32_t    i_media_length;
    size_t      i_packet_length;
    uint32_t    i_packet_count;
    int         i_max_bit_rate;
    int         i_header_size;

};

