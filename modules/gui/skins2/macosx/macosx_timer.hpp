/*****************************************************************************
 * macosx_timer.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: macosx_timer.hpp 9711 2005-01-09 19:46:39Z asmax $
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

#ifndef MACOSX_TIMER_HPP
#define MACOSX_TIMER_HPP

#include "../src/os_timer.hpp"

// Forward declaration
class MacOSXTimerLoop;


// MacOSX specific timer
class MacOSXTimer: public OSTimer
{
    public:
        MacOSXTimer( intf_thread_t *pIntf, const Callback &rCallback );
        virtual ~MacOSXTimer();

        /// (Re)start the timer with the given delay (in ms). If oneShot is
        /// true, stop it after the first execution of the callback.
        virtual void start( int delay, bool oneShot );

        /// Stop the timer
        virtual void stop();

    private:
        /// Callback to execute
        Callback m_callback;
};


#endif
