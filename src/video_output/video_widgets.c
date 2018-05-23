/*****************************************************************************
 * video_widgets.c : OSD widgets manipulation functions
 *****************************************************************************
 * Copyright (C) 2004-2005 the VideoLAN team
 * $Id: video_widgets.c 12487 2005-09-08 13:55:25Z hartman $
 *
 * Author: Yoann Peronneau <yoann@videolan.org>
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
#include <stdlib.h>                                                /* free() */
#include <vlc/vout.h>
#include <vlc_osd.h>

#include "vlc_video.h"
#include "vlc_filter.h"

/*****************************************************************************
 * Displays an OSD slider.
 * Types are: OSD_HOR_SLIDER and OSD_VERT_SLIDER.
 *****************************************************************************/
void vout_OSDSlider( vlc_object_t *p_caller, int i_channel, int i_position,
                     short i_type )
{
    vout_thread_t *p_vout = vlc_object_find( p_caller, VLC_OBJECT_VOUT,
                                             FIND_ANYWHERE );

    if( p_vout && ( config_GetInt( p_caller, "osd" ) || ( i_position >= 0 ) ) )
    {
        osd_Slider( p_caller, p_vout->p_spu, p_vout->render.i_width,
            p_vout->render.i_height, i_channel, i_position, i_type );
    }
    vlc_object_release( p_vout );
}

/*****************************************************************************
 * Displays an OSD icon.
 * Types are: OSD_PLAY_ICON, OSD_PAUSE_ICON, OSD_SPEAKER_ICON, OSD_MUTE_ICON
 *****************************************************************************/
void vout_OSDIcon( vlc_object_t *p_caller, int i_channel, short i_type )
{
    vout_thread_t *p_vout = vlc_object_find( p_caller, VLC_OBJECT_VOUT,
                                             FIND_ANYWHERE );

    if( !p_vout ) return;

    if( config_GetInt( p_caller, "osd" ) )
    {
        osd_Icon( p_caller, p_vout->p_spu, p_vout->render.i_width,
            p_vout->render.i_height, i_channel, i_type );
    }
    vlc_object_release( p_vout );
}

