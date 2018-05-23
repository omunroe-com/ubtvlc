/*****************************************************************************
 * timer.cpp : wxWindows plugin for vlc
 *****************************************************************************
 * Copyright (C) 2000-2003 VideoLAN
 * $Id: timer.cpp 8966 2004-10-10 10:08:44Z ipkiss $
 *
 * Authors: Gildas Bazin <gbazin@videolan.org>
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
#include <errno.h>                                                 /* ENOMEM */
#include <string.h>                                            /* strerror() */
#include <stdio.h>

#include <vlc/vlc.h>
#include <vlc/aout.h>
#include <vlc/intf.h>

#include "wxwindows.h"
#include <wx/timer.h>

//void DisplayStreamDate( wxControl *, intf_thread_t *, int );

/* Callback prototypes */
static int PopupMenuCB( vlc_object_t *p_this, const char *psz_variable,
                        vlc_value_t old_val, vlc_value_t new_val, void *param );
static int IntfShowCB( vlc_object_t *p_this, const char *psz_variable,
                       vlc_value_t old_val, vlc_value_t new_val, void *param );

/*****************************************************************************
 * Constructor.
 *****************************************************************************/
Timer::Timer( intf_thread_t *_p_intf, Interface *_p_main_interface )
{
    p_intf = _p_intf;
    p_main_interface = _p_main_interface;
    b_init = 0;
    i_old_playing_status = PAUSE_S;
    i_old_rate = INPUT_RATE_DEFAULT;

    /* Register callback for the intf-popupmenu variable */
    playlist_t *p_playlist =
        (playlist_t *)vlc_object_find( p_intf, VLC_OBJECT_PLAYLIST,
                                       FIND_ANYWHERE );
    if( p_playlist != NULL )
    {
        var_AddCallback( p_playlist, "intf-popupmenu", PopupMenuCB, p_intf );
        var_AddCallback( p_playlist, "intf-show", IntfShowCB, p_intf );
        vlc_object_release( p_playlist );
    }

    Start( 100 /*milliseconds*/, wxTIMER_CONTINUOUS );
}

Timer::~Timer()
{
    /* Unregister callback */
    playlist_t *p_playlist =
        (playlist_t *)vlc_object_find( p_intf, VLC_OBJECT_PLAYLIST,
                                       FIND_ANYWHERE );
    if( p_playlist != NULL )
    {
        var_DelCallback( p_playlist, "intf-popupmenu", PopupMenuCB, p_intf );
        var_DelCallback( p_playlist, "intf-show", IntfShowCB, p_intf );
        vlc_object_release( p_playlist );
    }
}

/*****************************************************************************
 * Private methods.
 *****************************************************************************/

/*****************************************************************************
 * Manage: manage main thread messages
 *****************************************************************************
 * In this function, called approx. 10 times a second, we check what the
 * main program wanted to tell us.
 *****************************************************************************/
void Timer::Notify()
{
#if defined( __WXMSW__ ) /* Work-around a bug with accelerators */
    if( !b_init )
    {
        p_main_interface->Init();
        b_init = VLC_TRUE;
    }
#endif

    vlc_mutex_lock( &p_intf->change_lock );

    /* Update the input */
    if( p_intf->p_sys->p_input == NULL )
    {
        p_intf->p_sys->p_input =
            (input_thread_t *)vlc_object_find( p_intf, VLC_OBJECT_INPUT,
                                               FIND_ANYWHERE );

        /* Refresh interface */
        if( p_intf->p_sys->p_input )
        {
            p_main_interface->slider->SetValue( 0 );
            b_old_seekable = VLC_FALSE;

            p_main_interface->statusbar->SetStatusText(
                wxU(p_intf->p_sys->p_input->input.p_item->psz_name), 2 );

            p_main_interface->TogglePlayButton( PLAYING_S );
            i_old_playing_status = PLAYING_S;
        }
    }
    else if( p_intf->p_sys->p_input->b_dead )
    {
        /* Hide slider */
        p_main_interface->slider_frame->Hide();
        p_main_interface->frame_sizer->Hide(
            p_main_interface->slider_frame );
        p_main_interface->frame_sizer->Layout();
        p_main_interface->frame_sizer->Fit( p_main_interface );

        p_main_interface->TogglePlayButton( PAUSE_S );
        i_old_playing_status = PAUSE_S;

        p_main_interface->statusbar->SetStatusText( wxT(""), 0 );
        p_main_interface->statusbar->SetStatusText( wxT(""), 2 );

        vlc_object_release( p_intf->p_sys->p_input );
        p_intf->p_sys->p_input = NULL;
    }


    if( p_intf->p_sys->p_input )
    {
        input_thread_t *p_input = p_intf->p_sys->p_input;
        vlc_value_t val;

        if( !p_input->b_die )
        {
            vlc_value_t pos;

            /* New input or stream map change */
            p_intf->p_sys->b_playing = 1;

            /* Manage the slider */
            /* FIXME --fenrir */
            /* Change the name of b_old_seekable into b_show_bar or something like that */
            var_Get( p_input, "position", &pos );

            if( !b_old_seekable )
            {
                if( pos.f_float > 0.0 )
                {
                    /* Done like this, as it's the only way to know if the slider
                     * has to be displayed */
                    b_old_seekable = VLC_TRUE;
                    p_main_interface->slider_frame->Show();
                    p_main_interface->frame_sizer->Show(
                        p_main_interface->slider_frame );
                    p_main_interface->frame_sizer->Layout();
                    p_main_interface->frame_sizer->Fit( p_main_interface );

                }
            }

            if( p_intf->p_sys->b_playing && b_old_seekable )
            {
                /* Update the slider if the user isn't dragging it. */
                if( p_intf->p_sys->b_slider_free )
                {
                    char psz_time[ MSTRTIME_MAX_SIZE ];
                    char psz_total[ MSTRTIME_MAX_SIZE ];
                    vlc_value_t time;
                    mtime_t i_seconds;

                    /* Update the value */
                    if( pos.f_float >= 0.0 )
                    {
                        p_intf->p_sys->i_slider_pos =
                            (int)(SLIDER_MAX_POS * pos.f_float);

                        p_main_interface->slider->SetValue(
                            p_intf->p_sys->i_slider_pos );

                        var_Get( p_intf->p_sys->p_input, "time", &time );
                        i_seconds = time.i_time / 1000000;
                        secstotimestr ( psz_time, i_seconds );

                        var_Get( p_intf->p_sys->p_input, "length",  &time );
                        i_seconds = time.i_time / 1000000;
                        secstotimestr ( psz_total, i_seconds );

                        p_main_interface->statusbar->SetStatusText(
                            wxU(psz_time) + wxString(wxT(" / ")) +
                            wxU(psz_total), 0 );
                    }
                }
            }
#if 0
        vlc_mutex_lock( &p_input->stream.stream_lock );
            if( p_intf->p_sys->p_input->stream.b_seekable && !b_old_seekable )
            {
                /* Done like this because b_seekable is set slightly after
                 * the new input object is available. */
                b_old_seekable = VLC_TRUE;
                p_main_interface->slider_frame->Show();
                p_main_interface->frame_sizer->Show(
                    p_main_interface->slider_frame );
                p_main_interface->frame_sizer->Layout();
                p_main_interface->frame_sizer->Fit( p_main_interface );
            }
            if( p_input->stream.b_seekable && p_intf->p_sys->b_playing )
            {
                /* Update the slider if the user isn't dragging it. */
                if( p_intf->p_sys->b_slider_free )
                {
                    vlc_value_t pos;
                    char psz_time[ MSTRTIME_MAX_SIZE ];
                    char psz_total[ MSTRTIME_MAX_SIZE ];
                    vlc_value_t time;
                    mtime_t i_seconds;

                    /* Update the value */
                    var_Get( p_input, "position", &pos );
                    if( pos.f_float >= 0.0 )
                    {
                        p_intf->p_sys->i_slider_pos =
                            (int)(SLIDER_MAX_POS * pos.f_float);

                        p_main_interface->slider->SetValue(
                            p_intf->p_sys->i_slider_pos );

                        var_Get( p_intf->p_sys->p_input, "time", &time );
                        i_seconds = time.i_time / 1000000;
                        secstotimestr ( psz_time, i_seconds );

                        var_Get( p_intf->p_sys->p_input, "length",  &time );
                        i_seconds = time.i_time / 1000000;
                        secstotimestr ( psz_total, i_seconds );

                        p_main_interface->statusbar->SetStatusText(
                            wxU(psz_time) + wxString(wxT(" / ")) +
                            wxU(psz_total), 0 );
                    }
                }
            }
        vlc_mutex_unlock( &p_input->stream.stream_lock );
#endif
            /* Take care of the volume, etc... */
            p_main_interface->Update();

            /* Manage Playing status */
            var_Get( p_input, "state", &val );
            if( i_old_playing_status != val.i_int )
            {
                if( val.i_int == PAUSE_S )
                {
                    p_main_interface->TogglePlayButton( PAUSE_S );
                }
                else
                {
                    p_main_interface->TogglePlayButton( PLAYING_S );
                }
                i_old_playing_status = val.i_int;
            }

            /* Manage Speed status */
            var_Get( p_input, "rate", &val );
            if( i_old_rate != val.i_int )
            {
                p_main_interface->statusbar->SetStatusText(
                    wxString::Format(wxT("x%.2f"),
                    (float)INPUT_RATE_DEFAULT / val.i_int ), 1 );
                i_old_rate = val.i_int;
            }
        }

    }
    else if( p_intf->p_sys->b_playing && !p_intf->b_die )
    {
        p_intf->p_sys->b_playing = 0;
        p_main_interface->TogglePlayButton( PAUSE_S );
        i_old_playing_status = PAUSE_S;
    }

    /* Show the interface, if requested */
    if( p_intf->p_sys->b_intf_show )
    {
        p_main_interface->Raise();
        p_intf->p_sys->b_intf_show = VLC_FALSE;
    }

    if( p_intf->b_die )
    {
        vlc_mutex_unlock( &p_intf->change_lock );

        /* Prepare to die, young Skywalker */
        p_main_interface->Close(TRUE);
        return;
    }

    vlc_mutex_unlock( &p_intf->change_lock );
}

/*****************************************************************************
 * PopupMenuCB: callback triggered by the intf-popupmenu playlist variable.
 *  We don't show the menu directly here because we don't want the
 *  caller to block for a too long time.
 *****************************************************************************/
static int PopupMenuCB( vlc_object_t *p_this, const char *psz_variable,
                        vlc_value_t old_val, vlc_value_t new_val, void *param )
{
    intf_thread_t *p_intf = (intf_thread_t *)param;

    if( p_intf->p_sys->pf_show_dialog )
    {
        p_intf->p_sys->pf_show_dialog( p_intf, INTF_DIALOG_POPUPMENU,
                                       new_val.b_bool, 0 );
    }

    return VLC_SUCCESS;
}

/*****************************************************************************
 * IntfShowCB: callback triggered by the intf-show playlist variable.
 *****************************************************************************/
static int IntfShowCB( vlc_object_t *p_this, const char *psz_variable,
                       vlc_value_t old_val, vlc_value_t new_val, void *param )
{
    intf_thread_t *p_intf = (intf_thread_t *)param;
    p_intf->p_sys->b_intf_show = VLC_TRUE;

    return VLC_SUCCESS;
}
