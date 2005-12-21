/*****************************************************************************
 * vout_window.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: vout_window.hpp 9469 2004-12-07 14:18:47Z zorglub $
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

#ifndef VOUT_WINDOW_HPP
#define VOUT_WINDOW_HPP

#include "generic_window.hpp"

class OSGraphics;


/// Class to handle a video output window
class VoutWindow: private GenericWindow
{
    public:
        VoutWindow( intf_thread_t *pIntf, int xPos, int yPos,
                    bool dragDrop, bool playOnDrop, GenericWindow &rParent );
        virtual ~VoutWindow();

        /// Make some functions public
        //@{
        using GenericWindow::show;
        using GenericWindow::hide;
        using GenericWindow::move;
        //@}

        /// Resize the window
        virtual void resize( int width, int height );

        /// Refresh an area of the window
        virtual void refresh( int left, int top, int width, int height );

        virtual string getType() const { return "Vout"; }

    private:
        /// Image when there is no video
        OSGraphics *m_pImage;
};

typedef CountedPtr<VoutWindow> VoutWindowPtr;

#endif
