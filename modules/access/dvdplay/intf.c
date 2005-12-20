/*****************************************************************************
 * intf.c: interface for DVD video manager
 *****************************************************************************
 * Copyright (C) 2002 VideoLAN
 * $Id: intf.c 6961 2004-03-05 17:34:23Z sam $
 *
 * Authors: St�phane Borel <stef@via.ecp.fr>
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
#include <string.h>
#include <unistd.h>

#include <vlc/vlc.h>
#include <vlc/intf.h>

#include "stream_control.h"
#include "input_ext-intf.h"
#include "input_ext-dec.h"

#include "vlc_keys.h"

#include "dvd.h"

/*****************************************************************************
 * intf_sys_t: description and status of interface
 *****************************************************************************/
struct intf_sys_t
{
    input_thread_t *    p_input;
    dvd_data_t *        p_dvd;

    vlc_bool_t          b_still;
    vlc_bool_t          b_inf_still;
    mtime_t             m_still_time;

    dvdplay_ctrl_t      control;
    vlc_bool_t          b_click, b_move, b_key_pressed;
};

/*****************************************************************************
 * Local prototypes.
 *****************************************************************************/
static int  InitThread     ( intf_thread_t *p_intf );
static int  MouseEvent     ( vlc_object_t *, char const *,
                             vlc_value_t, vlc_value_t, void * );
static int  KeyEvent       ( vlc_object_t *, char const *,
                             vlc_value_t, vlc_value_t, void * );

/* Exported functions */
static void RunIntf        ( intf_thread_t *p_intf );

/*****************************************************************************
 * OpenIntf: initialize dummy interface
 *****************************************************************************/
int E_(OpenIntf) ( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t *)p_this;

    /* Allocate instance and initialize some members */
    p_intf->p_sys = malloc( sizeof( intf_sys_t ) );
    if( p_intf->p_sys == NULL )
    {
        return( 1 );
    };

    p_intf->pf_run = RunIntf;
    
    var_AddCallback( p_intf->p_vlc, "key-pressed", KeyEvent, p_intf );
    p_intf->p_sys->m_still_time = 0;
    p_intf->p_sys->b_inf_still = 0;
    p_intf->p_sys->b_still = 0;

    return( 0 );
}

/*****************************************************************************
 * CloseIntf: destroy dummy interface
 *****************************************************************************/
void E_(CloseIntf) ( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t *)p_this;
    var_DelCallback( p_intf->p_vlc, "key-pressed", KeyEvent, p_intf );
    /* Destroy structure */
    free( p_intf->p_sys );
}


/*****************************************************************************
 * RunIntf: main loop
 *****************************************************************************/
static void RunIntf( intf_thread_t *p_intf )
{
    vlc_object_t *      p_vout = NULL;
    mtime_t             mtime = 0;
    mtime_t             mlast = 0;

    if( InitThread( p_intf ) < 0 )
    {
        msg_Err( p_intf, "can't initialize intf" );
        return;
    }
    msg_Dbg( p_intf, "intf initialized" );

    /* Main loop */
    while( !p_intf->b_die )
    {
        vlc_mutex_lock( &p_intf->change_lock );

        /*
         * still images
         */
#if 1
        if( p_intf->p_sys->b_still && !p_intf->p_sys->b_inf_still )
        {
            if( p_intf->p_sys->m_still_time > 0 )
            {
                /* update remaining still time */
                mtime = mdate();
                if( mlast )
                {
                    p_intf->p_sys->m_still_time -= mtime - mlast;
                }

                mlast = mtime;
            }
            else
            {
                /* still time elasped */
                input_SetStatus( p_intf->p_sys->p_input,
                                 INPUT_STATUS_PLAY );
                p_intf->p_sys->m_still_time = 0;
                p_intf->p_sys->b_still = 0;
                mlast = 0;
            }
        }
#else
        if( p_intf->p_sys->m_still_time != (mtime_t)(-1) )
        {
            if( p_intf->p_sys->m_still_time )
            {
                mtime = mdate();
                if( mlast )
                {
                    p_intf->p_sys->m_still_time -= mtime - mlast;
                }
                if( !p_intf->p_sys->m_still_time )
                {
                    input_SetStatus( p_intf->p_sys->p_input,
                                     INPUT_STATUS_PLAY );
                }
                mlast = mtime;
            }

        }
#endif

        /* 
         * mouse cursor
         */
        if( p_vout && ( p_intf->p_sys->b_click || p_intf->p_sys->b_move ) )
        {
            vlc_value_t val;
            int i_activate;

            var_Get( p_vout, "mouse-x", &val );
            p_intf->p_sys->control.mouse.i_x = val.i_int;
            var_Get( p_vout, "mouse-y", &val );
            p_intf->p_sys->control.mouse.i_y = val.i_int;

            if( p_intf->p_sys->b_click )
            {
                p_intf->p_sys->control.type = DVDCtrlMouseActivate;
                p_intf->p_sys->b_click = VLC_FALSE;
            }
            else
            {
                p_intf->p_sys->control.type = DVDCtrlMouseSelect;
                p_intf->p_sys->b_move = VLC_FALSE;
            }

            /* we can safely interact with libdvdplay
             * with the stream lock */
            vlc_mutex_lock( &p_intf->p_sys->p_input->stream.stream_lock );

            i_activate = dvdplay_button( p_intf->p_sys->p_dvd->vmg,
                                         &p_intf->p_sys->control );

            vlc_mutex_unlock( &p_intf->p_sys->p_input->stream.stream_lock );

            if( i_activate && p_intf->p_sys->b_still )
            {
                input_SetStatus( p_intf->p_sys->p_input, INPUT_STATUS_PLAY );
                p_intf->p_sys->b_still = 0;
                p_intf->p_sys->b_inf_still = 0;
                p_intf->p_sys->m_still_time = 0;
            }
        }

        /*
         * keyboard event
         */
        if( p_intf->p_sys->b_key_pressed )
        {
            vlc_value_t val;
            int i, i_activate, i_action = -1;
            struct hotkey *p_hotkeys = p_intf->p_vlc->p_hotkeys;

            p_intf->p_sys->b_key_pressed = VLC_FALSE;
            
            /* Find action triggered by hotkey (if any) */
            var_Get( p_intf->p_vlc, "key-pressed", &val );
            for( i = 0; p_hotkeys[i].psz_action != NULL; i++ )
            {
                if( p_hotkeys[i].i_key == val.i_int )
                {
                    i_action = p_hotkeys[i].i_action;
                }
            }

            if( i_action )
            {
                if( i_action == ACTIONID_NAV_LEFT )
                {
                    p_intf->p_sys->control.type = DVDCtrlLeftButtonSelect;
                }
                else if( i_action == ACTIONID_NAV_RIGHT )
                {
                    p_intf->p_sys->control.type = DVDCtrlRightButtonSelect;
                }
                else if( i_action == ACTIONID_NAV_UP )
                {
                    p_intf->p_sys->control.type = DVDCtrlUpperButtonSelect;
                }
                else if( i_action == ACTIONID_NAV_DOWN )
                {
                    p_intf->p_sys->control.type = DVDCtrlLowerButtonSelect;
                }
                else if( i_action == ACTIONID_NAV_ACTIVATE )
                {
                    p_intf->p_sys->control.type = DVDCtrlButtonActivate;
                }
                /* we can safely interact with libdvdplay
                 * with the stream lock */
                vlc_mutex_lock( &p_intf->p_sys->p_input->stream.stream_lock );
                
                i_activate = dvdplay_button( p_intf->p_sys->p_dvd->vmg,
                                             &p_intf->p_sys->control );
                
                vlc_mutex_unlock( &p_intf->p_sys->p_input->stream.stream_lock );
                
                if( i_activate && p_intf->p_sys->b_still )
                {
                    input_SetStatus( p_intf->p_sys->p_input, INPUT_STATUS_PLAY );
                    p_intf->p_sys->b_still = 0;
                    p_intf->p_sys->b_inf_still = 0;
                    p_intf->p_sys->m_still_time = 0;
                }
            }
        }
                

        vlc_mutex_unlock( &p_intf->change_lock );

        /* 
         * video output
         */
        if( p_vout && p_vout->b_die )
        {
            var_DelCallback( p_vout, "mouse-moved", MouseEvent, p_intf );
            var_DelCallback( p_vout, "mouse-clicked", MouseEvent, p_intf );
            vlc_object_release( p_vout );
            p_vout = NULL;
        }

        if( p_vout == NULL )
        {
            p_vout = vlc_object_find( p_intf->p_sys->p_input,
                                      VLC_OBJECT_VOUT, FIND_CHILD );
            if( p_vout )
            {
                var_AddCallback( p_vout, "mouse-moved", MouseEvent, p_intf );
                var_AddCallback( p_vout, "mouse-clicked", MouseEvent, p_intf );
            }
        }

        /* Wait a bit */
        msleep( INTF_IDLE_SLEEP );
    }

    if( p_vout )
    {
        var_DelCallback( p_vout, "mouse-moved", MouseEvent, p_intf );
        var_DelCallback( p_vout, "mouse-clicked", MouseEvent, p_intf );
        vlc_object_release( p_vout );
    }

    vlc_object_release( p_intf->p_sys->p_input );
}

/*****************************************************************************
 * InitThread:
 *****************************************************************************/
static int InitThread( intf_thread_t * p_intf )
{
    /* We might need some locking here */
    if( !p_intf->b_die )
    {
        input_thread_t * p_input;
        dvd_data_t * p_dvd;

        p_input = vlc_object_find( p_intf, VLC_OBJECT_INPUT, FIND_PARENT );

        /* Maybe the input just died */
        if( p_input == NULL )
        {
            return VLC_EGENERIC;
        }

        p_dvd = (dvd_data_t*)p_input->p_access_data;
        p_dvd->p_intf = p_intf;

        vlc_mutex_lock( &p_intf->change_lock );

        p_intf->p_sys->p_input = p_input;
        p_intf->p_sys->p_dvd = p_dvd;

        p_intf->p_sys->b_move = VLC_FALSE;
        p_intf->p_sys->b_click = VLC_FALSE;
        p_intf->p_sys->b_key_pressed = VLC_FALSE;

        vlc_mutex_unlock( &p_intf->change_lock );

        return VLC_SUCCESS;
    }
    else
    {
        return VLC_EGENERIC;
    }
}

/*****************************************************************************
 * MouseEvent: callback for mouse events
 *****************************************************************************/
static int MouseEvent( vlc_object_t *p_this, char const *psz_var,
                       vlc_value_t oldval, vlc_value_t newval, void *p_data )
{
    intf_thread_t *p_intf = (intf_thread_t *)p_data;

    vlc_mutex_lock( &p_intf->change_lock );

    if( psz_var[6] == 'c' ) /* "mouse-clicked" */
    {
        p_intf->p_sys->b_click = VLC_TRUE;
    }
    else if( psz_var[6] == 'm' ) /* "mouse-moved" */
    {
        p_intf->p_sys->b_move = VLC_TRUE;
    }

    vlc_mutex_unlock( &p_intf->change_lock );

    return VLC_SUCCESS;
}

/*****************************************************************************
 * KeyEvent: callback for keyboard events
 *****************************************************************************/
static int KeyEvent( vlc_object_t *p_this, char const *psz_var,
                       vlc_value_t oldval, vlc_value_t newval, void *p_data )
{
    intf_thread_t *p_intf = (intf_thread_t *)p_data;
    vlc_mutex_lock( &p_intf->change_lock );

    p_intf->p_sys->b_key_pressed = VLC_TRUE;
    
    vlc_mutex_unlock( &p_intf->change_lock );

    return VLC_SUCCESS;
}

/*****************************************************************************
 * dvdIntfStillTime: function provided to demux plugin to request
 * still images
 *****************************************************************************/
int dvdIntfStillTime( intf_thread_t *p_intf, int i_sec )
{
    vlc_mutex_lock( &p_intf->change_lock );
#if 1

    if( i_sec == 0xff )
    {
        p_intf->p_sys->b_still = 1;
        p_intf->p_sys->b_inf_still = 1;
    }
    else if( i_sec > 0 )
    {
        p_intf->p_sys->b_still = 1;
        p_intf->p_sys->m_still_time = 1000000 * i_sec;
    }
#else
    if( i_sec > 0 )
    {
        if( i_sec == 0xff )
        {
            p_intf->p_sys->m_still_time = (mtime_t)(-1);
            msg_Warn( p_intf, I64Fd, p_intf->p_sys->m_still_time );
        }
        else
        {
            p_intf->p_sys->m_still_time = 1000000 * i_sec;
        }
    }
#endif
    vlc_mutex_unlock( &p_intf->change_lock );

    return VLC_SUCCESS;
}

/*****************************************************************************
 * dvdIntfStillTime: function provided to reset still image
 *****************************************************************************/
int dvdIntfResetStillTime( intf_thread_t *p_intf )
{
    vlc_mutex_lock( &p_intf->change_lock );
    p_intf->p_sys->m_still_time = 0;
    input_SetStatus( p_intf->p_sys->p_input, INPUT_STATUS_PLAY );
    vlc_mutex_unlock( &p_intf->change_lock );

    return VLC_SUCCESS;
}
