/*****************************************************************************
 * window_manager.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: window_manager.hpp 8966 2004-10-10 10:08:44Z ipkiss $
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

#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include "skin_common.hpp"
#include "top_window.hpp"
#include <list>
#include <map>
#include <set>
#include <utility>


class GenericFont;
class GenericLayout;
class Anchor;
class Tooltip;


/// Window manager for skin windows
class WindowManager: public SkinObject
{
    public:
        /// Constructor
        WindowManager( intf_thread_t *pIntf);

        /// Destructor
        virtual ~WindowManager();

        /// Add a window to the list of known windows. Necessary if you want
        /// your window to be movable...
        void registerWindow( TopWindow &rWindow );

        /// Remove a previously registered window
        void unregisterWindow( TopWindow &rWindow );

        /// Tell the window manager that a move is initiated for pWindow.
        void startMove( TopWindow &rWindow );

        /// Tell the window manager that the current move ended.
        void stopMove();

        /// Move the pWindow window to (left, top), and move all its
        /// anchored windows.
        /// If a new anchoring is detected, the windows will move accordingly.
        void move( TopWindow &rWindow, int left, int top ) const;

        /// Raise all the registered windows
        void raiseAll() const;

        /// Show all the registered windows
        void showAll() const;

        /// Hide all the registered windows
        void hideAll() const;

        /// Synchronize the windows with their visibility variable
        void synchVisibility() const;

        /// Raise the given window
        void raise( TopWindow &rWindow ) const { rWindow.raise(); }

        /// Show the given window
        void show( TopWindow &rWindow ) const { rWindow.show(); }

        /// Hide the given window
        void hide( TopWindow &rWindow ) const { rWindow.hide(); }

        /// Toggle all the windows on top
        void toggleOnTop();

        /// Set the magnetism of screen edges
        void setMagnetValue( int magnet ) { m_magnet = magnet; }

        /// Set the alpha value of the static windows
        void setAlphaValue( int alpha ) { m_alpha = alpha; }

        /// Set the alpha value of the moving windows
        void setMoveAlphaValue( int moveAlpha ) { m_moveAlpha = moveAlpha; }

        /// Create the tooltip window
        void createTooltip( const GenericFont &rTipFont );

        /// Show the tooltip window
        void showTooltip();

        /// Hide the tooltip window
        void hideTooltip();

        /// Add a layout of the given window. This new layout will be the
        /// active one.
        void addLayout( TopWindow &rWindow, GenericLayout &rLayout );

        /// Change the active layout of the given window
        void setActiveLayout( TopWindow &rWindow, GenericLayout &rLayout );

    private:
        /// Some useful typedefs for lazy people like me
        typedef set<TopWindow*> WinSet_t;
        typedef list<Anchor*> AncList_t;

        /// This map represents the graph of anchored windows: it associates
        /// to a given window all the windows that are directly anchored by it.
        /// This is not transitive, i.e. if a is in m_dep[b] and if b is in
        /// m_dep[c], it doesn't mean that a is in m_dep[c] (in fact, it
        /// would be extremely rare...)
        map<TopWindow*, WinSet_t> m_dependencies;
        /// Store all the windows
        WinSet_t m_allWindows;
        /// Store the moving windows; this set is updated at every start of
        /// move.
        WinSet_t m_movingWindows;
        /// Indicate whether the windows are currently on top
        VariablePtr m_cVarOnTop;
        /// Magnetism of the screen edges (= scope of action)
        int m_magnet;
        /// Alpha value of the static windows
        int m_alpha;
        /// Alpha value of the moving windows
        int m_moveAlpha;
        /// Tooltip
        Tooltip *m_pTooltip;

        /// Recursively build a set of windows anchored to the one given.
        void buildDependSet( WinSet_t &rWinSet, TopWindow *pWindow );

        /// Check anchoring: this function updates xOffset and yOffset,
        /// to take care of a new anchoring (if any)
        void checkAnchors( TopWindow *pWindow,
                           int &xOffset, int &yOffset ) const;
};


#endif
