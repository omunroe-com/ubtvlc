/*****************************************************************************
 * cmd_vars.cpp
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 * $Id: cmd_vars.cpp 7261 2004-04-03 13:57:46Z asmax $
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
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

#include "cmd_vars.hpp"
#include "../src/vlcproc.hpp"
#include "../vars/stream.hpp"
#include "../vars/playlist.hpp"


void CmdNotifyPlaylist::execute()
{
    // Notify the playlist variable
    Playlist &rVar = VlcProc::instance( getIntf() )->getPlaylistVar();
    rVar.onChange();
}


void CmdSetStream::execute()
{
    // Change the stream variable
    m_rStream.set( m_name, m_updateVLC );
}

