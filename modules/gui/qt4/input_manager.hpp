/*****************************************************************************
 * input_manager.hpp : Manage an input and interact with its GUI elements
 ****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 * $Id: input_manager.hpp 16069 2006-07-18 14:33:18Z zorglub $
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
 *****************************************************************************/

#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <QObject>
#include <vlc/vlc.h>

class InputManager : public QObject
{
    Q_OBJECT;
public:
    InputManager( QObject *, intf_thread_t *);
    virtual ~InputManager();

private:
    intf_thread_t *p_intf;
    input_thread_t *p_input;
    int i_old_playing_status;

public slots:
    void togglePlayPause();
    void update(); ///< Periodic updates
    void setInput( input_thread_t * ); ///< Our controlled input changed
    void sliderUpdate( float ); ///< User dragged the slider. We get new pos
signals:
    /// Send new position, new time and new length
    void positionUpdated( float , int, int );
    void nameChanged( QString );
    void navigationChanged( int );
    void statusChanged( int );
};

class MainInputManager : public QObject
{
    Q_OBJECT;
public:
    static MainInputManager *getInstance( intf_thread_t *_p_intf )
    {
        if( !instance )
            instance = new MainInputManager( _p_intf );
        return instance;
    }

    input_thread_t *getInput() { return p_input; };
    InputManager *getIM() { return im; };

private:
    InputManager *im;
    intf_thread_t *p_intf;
    input_thread_t *p_input;
    static MainInputManager *instance;
    MainInputManager( intf_thread_t *);
public slots:
    void togglePlayPause();
private slots:
    void updateInput();
signals:
    void inputChanged( input_thread_t *);
};


#endif
