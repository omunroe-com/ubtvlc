/*****************************************************************************
 * cmd_show_window.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: cmd_show_window.hpp 8966 2004-10-10 10:08:44Z ipkiss $
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

#ifndef CMD_SHOW_WINDOW_HPP
#define CMD_SHOW_WINDOW_HPP

#include "cmd_generic.hpp"
#include "../src/top_window.hpp"
#include "../src/window_manager.hpp"


/// Command to show a window
class CmdShowWindow: public CmdGeneric
{
    public:
        CmdShowWindow( intf_thread_t *pIntf, WindowManager &rWinManager,
                       TopWindow &rWin ):
            CmdGeneric( pIntf ), m_rWinManager( rWinManager ), m_rWin( rWin ) {}
        virtual ~CmdShowWindow() {}

        /// This method does the real job of the command
        virtual void execute() { m_rWinManager.show( m_rWin ); }

        /// Return the type of the command
        virtual string getType() const { return "show window"; }

    private:
        /// Reference to the window manager
        WindowManager &m_rWinManager;
        /// Reference to the window
        TopWindow &m_rWin;
};


/// Command to hide a window
class CmdHideWindow: public CmdGeneric
{
    public:
        CmdHideWindow( intf_thread_t *pIntf, WindowManager &rWinManager,
                       TopWindow &rWin ):
            CmdGeneric( pIntf ), m_rWinManager( rWinManager ), m_rWin( rWin ) {}
        virtual ~CmdHideWindow() {}

        /// This method does the real job of the command
        virtual void execute() { m_rWinManager.hide( m_rWin ); }

        /// Return the type of the command
        virtual string getType() const { return "hide window"; }

    private:
        /// Reference to the window manager
        WindowManager &m_rWinManager;
        /// Reference to the window
        TopWindow &m_rWin;
};


/// Command to raise all windows
class CmdRaiseAll: public CmdGeneric
{
    public:
        CmdRaiseAll( intf_thread_t *pIntf, WindowManager &rWinManager ):
            CmdGeneric( pIntf ), m_rWinManager( rWinManager ) {}
        virtual ~CmdRaiseAll() {}

        /// This method does the real job of the command
        virtual void execute() { m_rWinManager.raiseAll(); }

        /// Return the type of the command
        virtual string getType() const { return "raise all windows"; }

    private:
        /// Reference to the window manager
        WindowManager &m_rWinManager;
};

#endif
