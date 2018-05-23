/*****************************************************************************
 * ctrl_resize.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: ctrl_resize.hpp 6964 2004-03-05 20:56:39Z ipkiss $
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

#ifndef CTRL_RESIZE_HPP
#define CTRL_RESIZE_HPP

#include "ctrl_flat.hpp"
#include "../commands/cmd_generic.hpp"
#include "../utils/fsm.hpp"

class GenericLayout;


/// Control decorator for resizing windows
class CtrlResize: public CtrlFlat
{
    public:
        CtrlResize( intf_thread_t *pIntf, CtrlFlat &rCtrl,
                    GenericLayout &rLayout, const UString &rHelp,
                    VarBool *pVisible );
        virtual ~CtrlResize() {}

        /// Handle an event
        virtual void handleEvent( EvtGeneric &rEvent );

        /// Check whether coordinates are inside the decorated control
        virtual bool mouseOver( int x, int y ) const;

        /// Draw the control on the given graphics
        virtual void draw( OSGraphics &rImage, int xDest, int yDest );

        /// Set the position and the associated layout of the decorated control
        virtual void setLayout( GenericLayout *pLayout,
                                const Position &rPosition );

        /// Get the position of the decorated control in the layout, if any
        virtual const Position *getPosition() const;

        static void transOutStill( SkinObject *pCtrl );
        static void transStillOut( SkinObject *pCtrl );
        static void transStillStill( SkinObject *pCtrl );
        static void transStillResize( SkinObject *pCtrl );
        static void transResizeStill( SkinObject *pCtrl );
        static void transResizeResize( SkinObject *pCtrl );

    private:
        FSM m_fsm;
        /// Decorated CtrlFlat
        CtrlFlat &m_rCtrl;
        /// The layout resized by this control
        GenericLayout &m_rLayout;
        /// The last received event
        EvtGeneric *m_pEvt;
        /// Position of the click that started the resizing
        int m_xPos, m_yPos;
        /// Callbacks
        Callback m_cmdOutStill;
        Callback m_cmdStillOut;
        Callback m_cmdStillStill;
        Callback m_cmdStillResize;
        Callback m_cmdResizeStill;
        Callback m_cmdResizeResize;

        // Size of the layout, before resizing
        int m_width, m_height;
};

#endif

