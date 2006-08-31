/*****************************************************************************
 * playlist.cpp : Playlist dialog
 ****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 * $Id: playlist.cpp 16235 2006-08-06 17:12:01Z zorglub $
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 ******************************************************************************/

#include "dialogs/playlist.hpp"
#include "util/qvlcframe.hpp"
#include "qt4.hpp"
#include "components/playlist/panels.hpp"
#include "components/playlist/selector.hpp"
#include <QHBoxLayout>
#include "menus.hpp"

PlaylistDialog *PlaylistDialog::instance = NULL;

PlaylistDialog::PlaylistDialog( intf_thread_t *_p_intf ) : QVLCMW( _p_intf )
{
    setWindowTitle( qtr( "Playlist" ) );
    QWidget *main = new QWidget( this );
    setCentralWidget( main );
    QVLCMenu::createPlMenuBar( menuBar(), p_intf );

    selector = new PLSelector( centralWidget(), p_intf, THEPL );
    selector->setMaximumWidth( 140 );

    rightPanel = qobject_cast<PLPanel *>(new StandardPLPanel( centralWidget(),
                              p_intf, THEPL, THEPL->p_local_category ) );
    connect( selector, SIGNAL( activated( int ) ),
             rightPanel, SLOT( setRoot( int ) ) );

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget( selector, 0 );
    layout->addWidget( rightPanel, 10 );
    centralWidget()->setLayout( layout );
    readSettings( "playlist", QSize( 600,300 ) );
}

PlaylistDialog::~PlaylistDialog()
{
    writeSettings( "playlist" );
}
