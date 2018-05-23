/*****************************************************************************
 * x11_tooltip.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: x11_tooltip.hpp 6961 2004-03-05 17:34:23Z sam $
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

#ifndef X11_TOOLTIP_HPP
#define X11_TOOLTIP_HPP

#include <X11/Xlib.h>

#include "../src/os_tooltip.hpp"

class X11Display;


/// X11 implementation of OSTooltip
class X11Tooltip: public OSTooltip
{
    public:
        X11Tooltip( intf_thread_t *pIntf, X11Display &rDisplay );

        virtual ~X11Tooltip();

        // Show the tooltip
        virtual void show( int left, int top, OSGraphics &rText );

        // Hide the tooltip
        virtual void hide();

    private:
        /// X11 display
        X11Display &m_rDisplay;
        /// Window ID
        Window m_wnd;
};


#endif
