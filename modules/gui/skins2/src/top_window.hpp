/*****************************************************************************
 * top_window.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: top_window.hpp 7259 2004-04-03 11:30:26Z ipkiss $
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

#ifndef TOP_WINDOW_HPP
#define TOP_WINDOW_HPP

#include "generic_window.hpp"
#include "../utils/pointer.hpp"
#include <list>

class OSWindow;
class OSGraphics;
class GenericLayout;
class CtrlGeneric;
class WindowManager;


/// Class to handle top-level windows
class TopWindow: public GenericWindow
{
    private:
        friend class WindowManager;
    public:
        TopWindow( intf_thread_t *pIntf, int xPos, int yPos,
                   WindowManager &rWindowManager,
                   bool dragDrop, bool playOnDrop );
        virtual ~TopWindow();

        /// Methods to process OS events.
        virtual void processEvent( EvtFocus &rEvtFocus );
        virtual void processEvent( EvtMotion &rEvtMotion );
        virtual void processEvent( EvtMouse &rEvtMouse );
        virtual void processEvent( EvtLeave &rEvtLeave );
        virtual void processEvent( EvtKey &rEvtKey );
        virtual void processEvent( EvtScroll &rEvtScroll );

        /// Forward an event to a control
        virtual void forwardEvent( EvtGeneric &rEvt, CtrlGeneric &rCtrl );

        // Refresh an area of the window
        virtual void refresh( int left, int top, int width, int height );

        /// Get the active layout
        virtual const GenericLayout& getActiveLayout() const;

        /// Update the shape of the window from the active layout
        virtual void updateShape();

        /// Called by a control that wants to capture the mouse
        virtual void onControlCapture( const CtrlGeneric &rCtrl );

        /// Called by a control that wants to release the mouse
        virtual void onControlRelease( const CtrlGeneric &rCtrl );

        /// Called by a control when its tooltip changed
        virtual void onTooltipChange( const CtrlGeneric &rCtrl );

    protected:
        /// Actually show the window
        virtual void innerShow();

    private:
        /// Change the active layout
        virtual void setActiveLayout( GenericLayout *pLayout );

        /// Window manager
        WindowManager &m_rWindowManager;
        /// Current active layout of the window
        GenericLayout *m_pActiveLayout;
        /// Last control on which the mouse was over
        CtrlGeneric *m_pLastHitControl;
        /// Control that has captured the mouse
        CtrlGeneric *m_pCapturingControl;
        /// Control that has the focus
        CtrlGeneric *m_pFocusControl;
        /// Current key modifier (also used for mouse)
        int m_currModifier;

        /// Find the uppest control in the layout hit by the mouse, and send
        /// it an enter event if needed
        CtrlGeneric *findHitControl( int xPos, int yPos );

        /// Update the lastHitControl pointer and send a leave event to the
        /// right control
        void setLastHit( CtrlGeneric *pNewHitControl );
};

typedef CountedPtr<TopWindow> TopWindowPtr;


#endif
