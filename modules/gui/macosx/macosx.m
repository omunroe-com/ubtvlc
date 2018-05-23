/*****************************************************************************
 * macosx.m: Mac OS X module for vlc
 *****************************************************************************
 * Copyright (C) 2001-2003 VideoLAN
 * $Id: macosx.m 8446 2004-08-17 15:17:04Z titer $
 *
 * Authors: Colin Delacroix <colin@zoy.org>
 *          Eugenio Jarosiewicz <ej0@cise.ufl.edu>
 *          Florian G. Pflug <fgp@phlo.org>
 *          Jon Lech Johansen <jon-vl@nanocrew.net>
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

#include <vlc/vlc.h>

/*****************************************************************************
 * External prototypes
 *****************************************************************************/
int  E_(OpenIntf)     ( vlc_object_t * );
void E_(CloseIntf)    ( vlc_object_t * );

int  E_(OpenVideoQT)  ( vlc_object_t * );
void E_(CloseVideoQT) ( vlc_object_t * );

int  E_(OpenVideoGL)  ( vlc_object_t * );
void E_(CloseVideoGL) ( vlc_object_t * );

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define VDEV_TEXT N_("Video device")
#define VDEV_LONGTEXT N_("Choose a number corresponding to " \
    "a screen in you video device selection menu and this screen " \
    "will be used by default as the screen for 'fullscreen'.")

#define OPAQUENESS_TEXT N_("Opaqueness")
#define OPAQUENESS_LONGTEXT N_( \
    "Set the transparency of the video output. 1 is non-transparent (default) " \
    "0 is fully transparent.")
    
#define STRETCH_TEXT N_("Stretch Aspect Ratio")
#define STRETCH_LONGTEXT N_("Instead of keeping the aspect ratio " \
        "of the movie when resizing the video, stretch the video " \
        "to fill the entire window." )

#define FILL_TEXT N_("Fill fullscreen")
#define FILL_LONGTEXT N_("In fullscreen mode, crop the picture if " \
        "necessary in order to fill the screen without black " \
        "borders (OpenGL only)." )

vlc_module_begin();
    set_description( _("Mac OS X interface, sound and video") );
    set_capability( "interface", 100 );
    set_callbacks( E_(OpenIntf), E_(CloseIntf) );
    add_submodule();
        set_capability( "video output", 100 );
        set_callbacks( E_(OpenVideoQT), E_(CloseVideoQT) );
        add_integer( "macosx-vdev", 0, NULL, VDEV_TEXT, VDEV_LONGTEXT,
                     VLC_FALSE );
        add_bool( "macosx-stretch", 0, NULL, STRETCH_TEXT, STRETCH_LONGTEXT,
                     VLC_FALSE );
        add_float_with_range( "macosx-opaqueness", 1, 0, 1, NULL,
                OPAQUENESS_TEXT, OPAQUENESS_LONGTEXT, VLC_TRUE );
        add_bool( "macosx-fill", 0, NULL, FILL_TEXT, FILL_LONGTEXT,
                  VLC_TRUE );
    add_submodule();
        set_capability( "opengl provider", 100 );
        set_callbacks( E_(OpenVideoGL), E_(CloseVideoGL) );
vlc_module_end();

