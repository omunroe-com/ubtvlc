/*****************************************************************************
 * intf.h: send info to intf.
 *****************************************************************************
 * Copyright (C) 2001 VideoLAN
 * $Id: intf.h 6961 2004-03-05 17:34:23Z sam $
 *
 * Author: St�phane Borel <stef@via.ecp.fr>
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
 * intf_sys_t: description and status of interface
 *****************************************************************************/
struct intf_sys_t
{
  input_thread_t    * p_input;
  thread_vcd_data_t * p_vcd;

  vlc_bool_t          b_still;        /* True if we are in a still frame */
  vlc_bool_t          b_inf_still;    /* True if still wait time is infinite */
  mtime_t             m_still_time;   /* Time in microseconds remaining
                                         to wait in still frame.
                                       */
#if FINISHED
  vcdplay_ctrl_t      control;
#else
  int                 control;
#endif
  vlc_bool_t          b_click, b_move, b_key_pressed;
};

int vcdIntfStillTime( struct intf_thread_t *, int );
int vcdIntfResetStillTime( intf_thread_t *p_intf );

