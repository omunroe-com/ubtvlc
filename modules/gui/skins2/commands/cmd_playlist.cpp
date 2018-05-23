/*****************************************************************************
 * cmd_playlist.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 * $Id: cmd_playlist.cpp 15998 2006-07-08 21:19:17Z xtophe $
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
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

#include "cmd_playlist.hpp"
#include "../src/vlcproc.hpp"
#include "../utils/var_bool.hpp"


void CmdPlaylistDel::execute()
{
    m_rList.delSelected();
}


void CmdPlaylistNext::execute()
{
    playlist_t *pPlaylist = getIntf()->p_sys->p_playlist;
    if( pPlaylist != NULL )
    {
        playlist_Next( pPlaylist );
    }
}


void CmdPlaylistPrevious::execute()
{
    playlist_t *pPlaylist = getIntf()->p_sys->p_playlist;
    if( pPlaylist != NULL )
    {
        playlist_Prev( pPlaylist );
    }
}


void CmdPlaylistRandom::execute()
{
    playlist_t *pPlaylist = getIntf()->p_sys->p_playlist;
    if( pPlaylist != NULL )
    {
        vlc_value_t val;
        val.b_bool = m_value;
        var_Set( pPlaylist , "random", val);
    }
}


void CmdPlaylistLoop::execute()
{
    playlist_t *pPlaylist = getIntf()->p_sys->p_playlist;
    if( pPlaylist != NULL )
    {
        vlc_value_t val;
        val.b_bool = m_value;
        var_Set( pPlaylist , "loop", val);
    }
}


void CmdPlaylistRepeat::execute()
{
    playlist_t *pPlaylist = getIntf()->p_sys->p_playlist;
    if( pPlaylist != NULL )
    {
        vlc_value_t val;
        val.b_bool = m_value;
        var_Set( pPlaylist , "repeat", val);
    }
}


void CmdPlaylistLoad::execute()
{
    playlist_t *pPlaylist = getIntf()->p_sys->p_playlist;
    if( pPlaylist != NULL )
    {
        /*FIXME: Where do we want ot insert ?*/
        playlist_Import( pPlaylist, m_file.c_str(),
                         pPlaylist->p_local_category, VLC_TRUE );
    }
}


void CmdPlaylistSave::execute()
{
    playlist_t *pPlaylist = getIntf()->p_sys->p_playlist;
    if( pPlaylist != NULL )
    {
        // FIXME: when the PLS export will be working, we'll need to remove
        // this hardcoding...
        msg_Err( getIntf(), "need to fix playlist save" );
//        playlist_Export( pPlaylist, m_file.c_str(), "export-m3u" );
    }
}

