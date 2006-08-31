/*****************************************************************************
 * playlist.hpp: Playlist dialog
 ****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 * $Id: playlist.hpp 16235 2006-08-06 17:12:01Z zorglub $
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA. *****************************************************************************/

#ifndef _PLAYLIST_DIALOG_H_
#define _PLAYLIST_DIALOG_H_

#include <QModelIndex>
#include "util/qvlcframe.hpp"

class PLSelector;
class PLPanel;

class PlaylistDialog : public QVLCMW
{
    Q_OBJECT;
public:
    static PlaylistDialog * getInstance( intf_thread_t *p_intf )
    {
        if( !instance) instance = new PlaylistDialog( p_intf );
        return instance;
    }
    virtual ~PlaylistDialog();
private:
    PlaylistDialog( intf_thread_t * );
    static PlaylistDialog *instance;

    PLSelector *selector;
    PLPanel *rightPanel;
};


#endif
