/*****************************************************************************
 * access.h : access headers for CD digital audio input module
 *****************************************************************************
 * Copyright (C) 2004 the VideoLAN team
 * $Id: 5ba6adb6c6f7f06ed023fe49ab2f469cead1810a $
 *
 * Authors: Rocky Bernstein <rocky@panix.com>
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

/*****************************************************************************
 * Open: open cdda device or image file and initialize structures
 * for subsequent operations.
 *****************************************************************************/
int  CDDAOpen     ( vlc_object_t * );

/*****************************************************************************
 * CDDAClose: closes cdda and frees any resources associded with it.
 *****************************************************************************/
void CDDAClose    ( vlc_object_t * );
