/*****************************************************************************
 * main.h:
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 * $Id: main.h 8205 2004-07-17 13:55:48Z asmax $
 *
 * Authors: Cyril Deguet <asmax@videolan.org>
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

#ifndef _GALAKTOS_MAIN_H_
#define _GALAKTOS_MAIN_H_

int galaktos_init( galaktos_thread_t *p_thread );
void galaktos_done( galaktos_thread_t *p_thread );
int galaktos_update( galaktos_thread_t *p_thread );

#endif
