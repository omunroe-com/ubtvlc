/*
 * Copyright (C) 2004the VideoLAN team
 *  $Id: snapshot.h 14956 2006-03-28 22:34:41Z xtophe $
 *
 * Authors: Olivier Aubert <oaubert 47 videolan d07 org>
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
 **************************************************************************** */



typedef struct snapshot_t {
  char *p_data;  /* Data area */

  int i_width;       /* In pixels */
  int i_height;      /* In pixels */
  int i_datasize;    /* In bytes */
  mtime_t date;      /* Presentation time */
} snapshot_t;
