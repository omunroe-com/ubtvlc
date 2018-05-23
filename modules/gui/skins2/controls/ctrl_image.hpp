/*****************************************************************************
 * ctrl_image.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: ctrl_image.hpp 6961 2004-03-05 17:34:23Z sam $
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

#ifndef CTRL_IMAGE_HPP
#define CTRL_IMAGE_HPP

#include "../commands/cmd_generic.hpp"
#include "ctrl_flat.hpp"

class GenericBitmap;
class OSGraphics;


/// Control image
class CtrlImage: public CtrlFlat
{
    public:
        // Create an image with the given bitmap (which is NOT copied)
        CtrlImage( intf_thread_t *pIntf, const GenericBitmap &rBitmap,
                   const UString &rHelp, VarBool *pVisible );
        virtual ~CtrlImage();

        /// Handle an event on the control
        virtual void handleEvent( EvtGeneric &rEvent );

        /// Check whether coordinates are inside the control
        virtual bool mouseOver( int x, int y ) const;

        /// Draw the control on the given graphics
        virtual void draw( OSGraphics &rImage, int xDest, int yDest );

    private:
        /// Bitmap
        const GenericBitmap &m_rBitmap;
        /// Buffer to stored the rendered bitmap
        OSGraphics *m_pImage;
};

#endif
