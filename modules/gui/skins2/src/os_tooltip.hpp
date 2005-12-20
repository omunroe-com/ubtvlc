/*****************************************************************************
 * os_tooltip.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: os_tooltip.hpp 6961 2004-03-05 17:34:23Z sam $
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

#ifndef OS_TOOLTIP_HPP
#define OS_TOOLTIP_HPP

#include "skin_common.hpp"

class OSGraphics;


/// Base class for OS specific Tooltip Windows
class OSTooltip: public SkinObject
{
    public:
        virtual ~OSTooltip() {}

        // Show the tooltip
        virtual void show( int left, int top, OSGraphics &rText ) = 0;

        // Hide the tooltip
        virtual void hide() = 0;

    protected:
        OSTooltip( intf_thread_t *pIntf ): SkinObject( pIntf ) {}
};

#endif
