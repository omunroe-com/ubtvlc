/*****************************************************************************
 * cmd_input.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: cmd_input.hpp 7707 2004-05-17 20:48:39Z ipkiss $
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teuli�re <ipkiss@via.ecp.fr>
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

#ifndef CMD_INPUT_HPP
#define CMD_INPUT_HPP

#include "cmd_generic.hpp"

/// Commands to control the input
DEFINE_COMMAND( Play, "play" )
DEFINE_COMMAND( Pause, "pause" )
DEFINE_COMMAND( Stop, "stop" )
DEFINE_COMMAND( Slower, "slower" )
DEFINE_COMMAND( Faster, "faster" )
DEFINE_COMMAND( Mute, "mute" )


#endif
