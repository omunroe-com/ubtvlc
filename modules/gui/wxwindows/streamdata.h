/*****************************************************************************
 * streamdata.h: streaming/transcoding data
 *****************************************************************************
 * Copyright (C) 2000-2004 VideoLAN
 * $Id: wizard.cpp 7826 2004-05-30 14:43:12Z zorglub $
 *
 * Authors: Cl�ment Stenac <zorglub@videolan.org>
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


#define MUXERS_NUMBER 9

// Do not count dummy here !
#define VCODECS_NUMBER 13
#define ACODECS_NUMBER 9

#define MUX_PS          0
#define MUX_TS          1
#define MUX_MPEG        2
#define MUX_OGG         3
#define MUX_RAW         4
#define MUX_ASF         5
#define MUX_AVI         6
#define MUX_MP4         7
#define MUX_MOV         8
#define MUX_WAV         9

/* Muxer / Codecs / Access_out compatibility tables */


struct codec {
    char *psz_display;
    char *psz_codec;
    char *psz_descr;
    int muxers[MUXERS_NUMBER];
};

static struct codec vcodecs_array[] =
{
    { "MPEG-1 Video" , "mp1v" , N_("MPEG-1 Video codec"),
//       {MUX_PS, MUX_TS, MUX_MPEG, MUX_OGG, MUX_AVI, MUX_RAW, -1,-1,-1 } },
       {MUX_PS, MUX_TS, MUX_MPEG, MUX_OGG, MUX_RAW, -1,-1,-1,-1 } },
    { "MPEG-2 Video" , "mp2v" , N_("MPEG-2 Video codec"),
//       {MUX_PS, MUX_TS, MUX_MPEG, MUX_OGG, MUX_AVI, MUX_RAW, -1,-1,-1 } },
       {MUX_PS, MUX_TS, MUX_MPEG, MUX_OGG, MUX_RAW, -1,-1,-1,-1 } },
    { "MPEG-4 Video" , "mp4v" , N_("MPEG-4 Video codec"),
//       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_MP4,MUX_OGG,MUX_AVI,MUX_RAW, -1} },
       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_MP4,MUX_OGG,MUX_RAW, -1,-1} },
    { "DIVX 1" ,"DIV1",N_("DivX first version") ,
//       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , MUX_AVI , -1,-1,-1,-1 } },
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , -1 , -1,-1,-1,-1 } },
    { "DIVX 2" ,"DIV2",N_("DivX second version") ,
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , -1 , -1,-1,-1,-1 } },
    { "DIVX 3" ,"DIV3",N_("DivX third version") ,
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , -1 , -1,-1,-1,-1 } },
    { "H 263" , "H263" , N_("H263 is a video codec optimized for videoconference (low rates)") ,
       { MUX_TS, MUX_AVI, -1,-1,-1,-1,-1,-1,-1 } },
    { "H 264" , "H264" , N_("H264 is a new video codec") ,
       { MUX_TS, MUX_AVI, -1,-1,-1,-1,-1,-1,-1 } },
    { "I 263", "I263", N_("I263 is an Intel conferencing codec") ,
       { MUX_TS, MUX_AVI, -1,-1,-1,-1,-1,-1,-1 } },
    { "WMV 1" , "WMV1", N_("WMV (Windows Media Video) 1") ,
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , -1 , -1,-1,-1,-1 } },
    { "WMV 2" , "WMV2", N_("WMV (Windows Media Video) 2") ,
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , -1 , -1,-1,-1,-1 } },
    { "MJPEG" , "MJPG", N_("MJPEG consists of a series of JPEG pictures") ,
       {MUX_TS , MUX_MPEG , MUX_ASF , MUX_OGG , -1 , -1,-1,-1,-1 } },
    { "Theora" , "theo", N_("Theora is a free general-purpose codec"),
       {MUX_TS, -1,-1,-1,-1,-1,-1,-1,-1} },
    { "Dummy", "dummy", N_("Dummy codec (do not transcode)") ,
      {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_MP4,MUX_OGG,MUX_WAV,MUX_RAW,MUX_MOV}},
    { NULL,NULL,NULL , {-1,-1,-1,-1,-1,-1,-1,-1,-1}} /* Do not remove me */
};

static struct codec acodecs_array[] =
{
    { "MPEG Audio" , "mpga" , N_("The standard MPEG audio (1/2) format") ,
//       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_OGG,MUX_AVI,MUX_RAW, -1,-1} },
       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_OGG,MUX_RAW, -1,-1,-1} },
    { "MP3" , "mp3" , N_("MPEG Audio Layer 3") ,
//       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_OGG,MUX_AVI,MUX_RAW, -1,-1} },
       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_OGG,MUX_RAW, -1,-1, -1} },
    { "MPEG 4 Audio" , "mp4a" , N_("Audio format for MPEG4") ,
       {MUX_TS, MUX_MP4, -1,-1,-1,-1,-1,-1,-1 } },
    { "A/52" , "a52" , N_("DVD audio format") ,
//       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_OGG,MUX_AVI,MUX_RAW, -1,-1} },
       {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_OGG,MUX_RAW, -1,-1,-1} },
    { "Vorbis" , "vorb" , N_("Vorbis is a free audio codec") ,
       {MUX_OGG, -1,-1,-1,-1,-1,-1,-1,-1} },
    { "FLAC" , "flac" , N_("FLAC is a lossless audio codec") ,
       {MUX_OGG , MUX_RAW, -1,-1,-1,-1,-1,-1,-1} },
    { "Speex" , "spx" , N_("A free audio codec dedicated to compression of voice") ,
       {MUX_OGG, -1,-1,-1,-1,-1,-1,-1,-1} },
    { "Uncompressed, integer" , "s16l" , N_("Uncompressed audio samples"),
       {MUX_WAV, -1,-1,-1,-1,-1,-1,-1,-1} },
    { "Uncompressed, floating" , "fl32" , N_("Uncompressed audio samples"),
       {MUX_WAV, -1,-1,-1,-1,-1,-1,-1,-1} },
    { "Dummy", "dummy", N_("Dummy codec (do not transcode)") ,
//     {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_MP4,MUX_OGG,MUX_AVI,MUX_RAW,MUX_MOV}},
     {MUX_PS,MUX_TS,MUX_MPEG,MUX_ASF,MUX_MP4,MUX_OGG,MUX_RAW,MUX_MOV,MUX_WAV}},
    { NULL,NULL,NULL , {-1,-1,-1,-1,-1,-1,-1,-1,-1}} /* Do not remove me */
};

struct method {
    char *psz_access;
    char *psz_method;
    char *psz_descr;
    char *psz_address;
    int   muxers[MUXERS_NUMBER];
};

static struct method methods_array[] =
{
    {"udp:",N_("UDP Unicast"), N_("Use this to stream to a single computer"),
     N_("Enter the address of the computer to stream to"),
     { MUX_TS, -1,-1,-1,-1,-1,-1,-1,-1 } },
    {"udp:",N_("UDP Multicast"),
     N_("Use this to stream to a dynamic group of computers on a "
     "multicast-enabled network. This is the most efficient method "
     "to stream to several computers, but it does not work over Internet."),
     N_("Enter the multicast address to stream to in this field. "
     "This must be an IP address between 224.0.0.0 an 239.255.255.255 "
     "For a private use, enter an address beginning with 239.255."),
     { MUX_TS, -1,-1,-1,-1,-1,-1,-1,-1 } },
    {"http://",N_("HTTP"),
     N_("Use this to stream to several computers. This method is "
     "less efficient, as the server needs to send several times the "
     "stream."),
     N_("Enter the local addresses you want to listen to. Do not enter "
     "anything if you want to listen to all adresses or if you don't "
     "understand. This is generally the best thing to do. Other computers "
     "can then access the stream at http://yourip:8080 by default"),
     { MUX_TS, MUX_PS, MUX_MPEG, MUX_OGG, MUX_RAW, MUX_ASF, -1,-1,-1} },
    { NULL, NULL,NULL,NULL , {-1,-1,-1,-1,-1,-1,-1,-1,-1}} /* Do not remove me */
};

struct encap {
    int   id;
    char *psz_mux;
    char *psz_encap;
    char *psz_descr;
};

static struct encap encaps_array[] =
{
    { MUX_PS, "ps","MPEG PS", N_("MPEG Program Stream") },
    { MUX_TS, "ts","MPEG TS", N_("MPEG Transport Stream") },
    { MUX_MPEG, "ps", "MPEG 1", N_("MPEG 1 Format") },
    { MUX_OGG, "ogg", "OGG", N_("OGG") },
    { MUX_RAW, "raw", "RAW", N_("RAW") },
    { MUX_ASF, "asf","ASF", N_("ASF") },
//    { MUX_AVI, "avi","AVI", N_("AVI") },
    { MUX_MP4, "mp4","MP4", N_("MPEG4") },
    { MUX_MOV, "mov","MOV", N_("MOV") },
    { MUX_WAV, "wav","WAV", N_("WAV") },
    { -1 , NULL,NULL , NULL } /* Do not remove me */
};


/* Bitrates arrays */
    static const wxString vbitrates_array[] =
    {
        wxT("3072"),
        wxT("2048"),
        wxT("1024"),
        wxT("768"),
        wxT("512"),
        wxT("384"),
        wxT("256"),
        wxT("192"),
        wxT("128"),
        wxT("96"),
        wxT("64"),
        wxT("32"),
        wxT("16")
    };
    static const wxString abitrates_array[] =
    {
        wxT("512"),
        wxT("256"),
        wxT("192"),
        wxT("128"),
        wxT("96"),
        wxT("64"),
        wxT("32"),
        wxT("16")
    };

