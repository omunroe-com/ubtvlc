/*****************************************************************************
 * intf.h: interface header for vlc
 *****************************************************************************
 * Copyright (C) 2002 VideoLAN
 * $Id: intf.h 8740 2004-09-19 19:22:04Z gbazin $
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

#ifndef _VLC_INTF_H
#define _VLC_INTF_H 1

# ifdef __cplusplus
extern "C" {
# endif

/*****************************************************************************
 * Required public headers
 *****************************************************************************/
#include <vlc/vlc.h>

/*****************************************************************************
 * Required internal headers
 *****************************************************************************/
#include "vlc_interface.h"
#include "vlc_es.h"
#include "vlc_input.h"
#include "intf_eject.h"
#include "vlc_playlist.h"

# ifdef __cplusplus
}
# endif

#endif /* <vlc/intf.h> */
