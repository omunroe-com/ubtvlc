/*****************************************************************************
 * sub.h
 *****************************************************************************
 * Copyright (C) 2001-2004 VideoLAN
 * $Id: sub.h 7288 2004-04-06 16:04:03Z hartman $
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

#include "../ps.h"

#define SUB_TYPE_MICRODVD   0x00
#define SUB_TYPE_SUBRIP     0x01
#define SUB_TYPE_SSA1       0x02
#define SUB_TYPE_SSA2_4     0x03
#define SUB_TYPE_VPLAYER    0x04
#define SUB_TYPE_SAMI       0x05
#define SUB_TYPE_SUBVIEWER  0x06
#define SUB_TYPE_VOBSUB     0x100
#define SUB_TYPE_UNKNOWN    0xffff

typedef struct subtitle_s
{
    mtime_t i_start;
    mtime_t i_stop;

    char    *psz_text;
    int     i_vobsub_location;

} subtitle_t;

typedef struct subtitle_demux_s
{
    VLC_COMMON_MEMBERS

    module_t        *p_module;

    int     (*pf_open) ( struct subtitle_demux_s *p_sub,
                         input_thread_t*p_input,
                         char *psz_name,
                         mtime_t i_microsecperframe );
    int     (*pf_demux)( struct subtitle_demux_s *p_sub, mtime_t i_maxdate );
    int     (*pf_seek) ( struct subtitle_demux_s *p_sub, mtime_t i_date );
    void    (*pf_close)( struct subtitle_demux_s *p_sub );

    /* *** private *** */
    input_thread_t      *p_input;
    int                 i_sub_type;

    char                *psz_header;
    int                 i_subtitle;
    int                 i_subtitles;
    subtitle_t          *subtitle;
    es_out_id_t         *p_es;
    int                 i_previously_selected; /* to make pf_seek */
    FILE                *p_vobsub_file;
    mtime_t             i_original_mspf;

} subtitle_demux_t;

/*****************************************************************************
 * subtitle_New: Start a new subtitle demux instance (but subtitle ES isn't
 *               selected by default.
 *****************************************************************************
 * Return: NULL if failed, else a pointer on a new subtitle_demux_t.
 *
 * XXX: - if psz_name is NULL then --sub-file is read
 *      - i_microsecperframe is used only for microdvd file. (overriden
 *        by --sub-fps )
 *      - it's at this point that --sub-delay is applied
 *
 *****************************************************************************/
static inline subtitle_demux_t *subtitle_New( input_thread_t *p_input,
                                              char *psz_name,
                                              mtime_t i_microsecperframe )
{
    subtitle_demux_t *p_sub;

    p_sub = vlc_object_create( p_input, sizeof( subtitle_demux_t ) );
    p_sub->psz_object_name = "subtitle demux";
    vlc_object_attach( p_sub, p_input );
    p_sub->p_module = module_Need( p_sub, "subtitle demux", NULL, 0 );

    if( p_sub->p_module &&
        p_sub->pf_open( p_sub, p_input, psz_name, i_microsecperframe ) >=0 )
    {
        msg_Info( p_input, "subtitle started" );

    }
    else
    {
        msg_Warn( p_input, "failed to start subtitle demux" );
        vlc_object_detach( p_sub );
        if( p_sub->p_module )
        {
            module_Unneed( p_sub, p_sub->p_module );
        }
        vlc_object_destroy( p_sub );
        p_sub = NULL;
    }

    return( p_sub );
}

/*****************************************************************************
 * subtitle_Demux: send subtitle to decoder from last date to i_max
 *****************************************************************************/
static inline int subtitle_Demux( subtitle_demux_t *p_sub, mtime_t i_max )
{
    return( p_sub->pf_demux( p_sub, i_max ) );
}

/*****************************************************************************
 * subtitle_Seek: Seek to i_date
 *****************************************************************************/
static inline int subtitle_Seek( subtitle_demux_t *p_sub, mtime_t i_date )
{
    return( p_sub->pf_seek( p_sub, i_date ) );
}

/*****************************************************************************
 * subtitle_Close: Stop ES decoder and free all memory included p_sub.
 *****************************************************************************/
static inline void subtitle_Close( subtitle_demux_t *p_sub )
{
    msg_Info( p_sub, "subtitle stopped" );
    if( p_sub )
    {
        p_sub->pf_close( p_sub );
        vlc_object_detach( p_sub );
        if( p_sub->p_module )
        {
            module_Unneed( p_sub, p_sub->p_module );
        }
        vlc_object_destroy( p_sub );
    }
}

