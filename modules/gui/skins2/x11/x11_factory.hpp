/*****************************************************************************
 * x11_factory.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: x11_factory.hpp 8116 2004-07-04 22:22:10Z adn $
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

#ifndef X11_FACTORY_HPP
#define X11_FACTORY_HPP

#include <X11/Xlib.h>

#include "../src/os_factory.hpp"
#include <map>

class X11Display;
class X11DragDrop;
class X11TimerLoop;


/// Class used to instanciate X11 specific objects
class X11Factory: public OSFactory
{
    public:
        /// Map to find the GenericWindow associated to a X11Window
        map<Window, GenericWindow*> m_windowMap;
        /// Map to find the Dnd object associated to a X11Window
        map<Window, X11DragDrop*> m_dndMap;

        X11Factory( intf_thread_t *pIntf );
        virtual ~X11Factory();

        /// Initialization method
        virtual bool init();

        /// Instantiate an object OSGraphics.
        virtual OSGraphics *createOSGraphics( int width, int height );

        /// Get the instance of the singleton OSLoop.
        virtual OSLoop *getOSLoop();

        /// Destroy the instance of OSLoop.
        virtual void destroyOSLoop();

        /// Instantiate an OSTimer with the given callback
        virtual OSTimer *createOSTimer( const Callback &rCallback );

        ///
        virtual void minimize();

        /// Instantiate an OSWindow object
        virtual OSWindow *createOSWindow( GenericWindow &rWindow,
                                          bool dragDrop, bool playOnDrop,
                                          OSWindow *pParent );

        /// Instantiate an object OSTooltip.
        virtual OSTooltip *createOSTooltip();

        /// Get the directory separator
        virtual const string &getDirSeparator() const { return m_dirSep; }

        /// Get the resource path
        virtual const list<string> &getResourcePath() const
            { return m_resourcePath; }

        /// Get the screen size
        virtual int getScreenWidth() const;
        virtual int getScreenHeight() const;

        /// Get the work area (screen area without taskbars)
        virtual Rect getWorkArea() const;

        /// Get the position of the mouse
        virtual void getMousePos( int &rXPos, int &rYPos ) const;

        /// Change the cursor
        virtual void changeCursor( CursorType_t type ) const { /*TODO*/ }

        /// Delete a directory recursively
        virtual void rmDir( const string &rPath );

        /// Get the timer loop
        X11TimerLoop *getTimerLoop() const { return m_pTimerLoop; }

    private:
        /// X11 display
        X11Display *m_pDisplay;
        /// Timer loop
        X11TimerLoop *m_pTimerLoop;
        /// Directory separator
        const string m_dirSep;
        /// Resource path
        list<string> m_resourcePath;
};

#endif
