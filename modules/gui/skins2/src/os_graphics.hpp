/*****************************************************************************
 * os_graphics.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: os_graphics.hpp 10861 2005-05-01 13:19:04Z asmax $
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

#ifndef OS_GRAPHICS_HPP
#define OS_GRAPHICS_HPP

#include "skin_common.hpp"
#include "../utils/pointer.hpp"

class GenericBitmap;
class OSWindow;


/// OS specific graphics class
class OSGraphics: public SkinObject
{
    public:
        virtual ~OSGraphics() {}

        /// Clear the graphics
        virtual void clear() = 0;

        /// Draw another graphics on this one
        virtual void drawGraphics( const OSGraphics &rGraphics, int xSrc = 0,
                                   int ySrc = 0, int xDest = 0, int yDest = 0,
                                   int width = -1, int height = -1 ) = 0;

        /// Render a bitmap on this graphics
        virtual void drawBitmap( const GenericBitmap &rBitmap, int xSrc = 0,
                                 int ySrc = 0, int xDest = 0, int yDest = 0,
                                 int width = -1, int height = -1,
                                 bool blend = false) = 0;

        /// Draw a filled rectangle on the grahics (color is #RRGGBB)
        virtual void fillRect( int left, int top, int width, int height,
                               uint32_t color ) = 0;

        /// Draw an empty rectangle on the grahics (color is #RRGGBB)
        virtual void drawRect( int left, int top, int width, int height,
                               uint32_t color ) = 0;


        /// Set the shape of a window with the mask of this graphics.
        virtual void applyMaskToWindow( OSWindow &rWindow ) = 0;

        /// Copy the graphics on a window
        virtual void copyToWindow( OSWindow &rWindow, int xSrc,
                                   int ySrc, int width, int height,
                                   int xDest, int yDest ) = 0;

        /// Tell whether the pixel at the given position is visible
        virtual bool hit( int x, int y ) const = 0;

        /// Getters
        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;

    protected:
        OSGraphics( intf_thread_t *pIntf ): SkinObject( pIntf ) {}
};

typedef CountedPtr<OSGraphics> OSGraphicsPtr;

#endif
