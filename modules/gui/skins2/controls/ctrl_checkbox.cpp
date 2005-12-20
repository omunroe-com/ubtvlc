/*****************************************************************************
 * ctrl_checkbox.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: ctrl_checkbox.cpp 8079 2004-06-27 19:27:01Z gbazin $
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

#include "ctrl_checkbox.hpp"
#include "../events/evt_generic.hpp"
#include "../commands/cmd_generic.hpp"
#include "../src/generic_bitmap.hpp"
#include "../src/os_factory.hpp"
#include "../src/os_graphics.hpp"
#include "../utils/var_bool.hpp"


CtrlCheckbox::CtrlCheckbox( intf_thread_t *pIntf,
                            const GenericBitmap &rBmpUp1,
                            const GenericBitmap &rBmpOver1,
                            const GenericBitmap &rBmpDown1,
                            const GenericBitmap &rBmpUp2,
                            const GenericBitmap &rBmpOver2,
                            const GenericBitmap &rBmpDown2,
                            CmdGeneric &rCommand1, CmdGeneric &rCommand2,
                            const UString &rTooltip1,
                            const UString &rTooltip2,
                            VarBool &rVariable, const UString &rHelp,
                            VarBool *pVisible ):
    CtrlGeneric( pIntf, rHelp, pVisible ), m_fsm( pIntf ),
    m_rVariable( rVariable ),
    m_rCommand1( rCommand1 ), m_rCommand2( rCommand2 ),
    m_tooltip1( rTooltip1 ), m_tooltip2( rTooltip2 ),
    m_cmdUpOverDownOver( this, &transUpOverDownOver ),
    m_cmdDownOverUpOver( this, &transDownOverUpOver ),
    m_cmdDownOverDown( this, &transDownOverDown ),
    m_cmdDownDownOver( this, &transDownDownOver ),
    m_cmdUpOverUp( this, &transUpOverUp ),
    m_cmdUpUpOver( this, &transUpUpOver ),
    m_cmdDownUp( this, &transDownUp ),
    m_cmdUpHidden( this, &transUpHidden ),
    m_cmdHiddenUp( this, &transHiddenUp )
{
    // Build the images of the checkbox
    OSFactory *pOsFactory = OSFactory::instance( pIntf );
    m_pImgUp1 = pOsFactory->createOSGraphics( rBmpUp1.getWidth(),
                                              rBmpUp1.getHeight() );
    m_pImgUp1->drawBitmap( rBmpUp1, 0, 0 );
    m_pImgDown1 = pOsFactory->createOSGraphics( rBmpDown1.getWidth(),
                                                rBmpDown1.getHeight() );
    m_pImgDown1->drawBitmap( rBmpDown1, 0, 0 );
    m_pImgOver1 = pOsFactory->createOSGraphics( rBmpOver1.getWidth(),
                                                rBmpOver1.getHeight() );
    m_pImgOver1->drawBitmap( rBmpOver1, 0, 0 );

    m_pImgUp2 = pOsFactory->createOSGraphics( rBmpUp2.getWidth(),
                                              rBmpUp2.getHeight() );
    m_pImgUp2->drawBitmap( rBmpUp2, 0, 0 );
    m_pImgDown2 = pOsFactory->createOSGraphics( rBmpDown2.getWidth(),
                                                rBmpDown2.getHeight() );
    m_pImgDown2->drawBitmap( rBmpDown2, 0, 0 );
    m_pImgOver2 = pOsFactory->createOSGraphics( rBmpOver2.getWidth(),
                                                rBmpOver2.getHeight() );
    m_pImgOver2->drawBitmap( rBmpOver2, 0, 0 );

    // States
    m_fsm.addState( "up" );
    m_fsm.addState( "down" );
    m_fsm.addState( "upOver" );
    m_fsm.addState( "downOver" );
    m_fsm.addState( "hidden" );

    // Transitions
    m_fsm.addTransition( "upOver", "mouse:left:down", "downOver",
                         &m_cmdUpOverDownOver );
    m_fsm.addTransition( "upOver", "mouse:left:dblclick", "downOver",
                         &m_cmdUpOverDownOver );
    m_fsm.addTransition( "downOver", "mouse:left:up", "upOver",
                         &m_cmdDownOverUpOver );
    m_fsm.addTransition( "downOver", "leave", "down", &m_cmdDownOverDown );
    m_fsm.addTransition( "down", "enter", "downOver", &m_cmdDownDownOver );
    m_fsm.addTransition( "upOver", "leave", "up", &m_cmdUpOverUp );
    m_fsm.addTransition( "up", "enter", "upOver", &m_cmdUpUpOver );
    m_fsm.addTransition( "down", "mouse:left:up", "up", &m_cmdDownUp );
    // XXX: It would be easy to use a "ANY" initial state to handle these
    // four lines in only one. But till now it isn't worthwhile...
    m_fsm.addTransition( "up", "special:hide", "hidden", &m_cmdUpHidden );
    m_fsm.addTransition( "down", "special:hide", "hidden", &m_cmdUpHidden );
    m_fsm.addTransition( "upOver", "special:hide", "hidden", &m_cmdUpHidden );
    m_fsm.addTransition( "downOver", "special:hide", "hidden", &m_cmdUpHidden );
    m_fsm.addTransition( "hidden", "special:show", "up", &m_cmdHiddenUp );

    // Observe the variable
    m_rVariable.addObserver( this );

    // Initial state
    m_fsm.setState( "up" );
    if( !m_rVariable.get() )
    {
        m_pImgUp = m_pImgUp1;
        m_pImgOver = m_pImgOver1;
        m_pImgDown = m_pImgDown1;
        m_pImgCurrent = m_pImgUp;
        m_pCommand = &m_rCommand1;
        m_pTooltip = &m_tooltip1;
    }
    else
    {
        m_pImgUp = m_pImgUp2;
        m_pImgOver = m_pImgOver2;
        m_pImgDown = m_pImgDown2;
        m_pImgCurrent = m_pImgDown;
        m_pCommand = &m_rCommand2;
        m_pTooltip = &m_tooltip2;
    }
}


CtrlCheckbox::~CtrlCheckbox()
{
    m_rVariable.delObserver( this );
    SKINS_DELETE( m_pImgUp1 );
    SKINS_DELETE( m_pImgDown1 );
    SKINS_DELETE( m_pImgOver1 );
    SKINS_DELETE( m_pImgUp2 );
    SKINS_DELETE( m_pImgDown2 );
    SKINS_DELETE( m_pImgOver2 );
}


void CtrlCheckbox::handleEvent( EvtGeneric &rEvent )
{
    m_fsm.handleTransition( rEvent.getAsString() );
}


bool CtrlCheckbox::mouseOver( int x, int y ) const
{
    if( m_pImgCurrent )
    {
        return m_pImgCurrent->hit( x, y );
    }
    else
    {
        return false;
    }
}


void CtrlCheckbox::draw( OSGraphics &rImage, int xDest, int yDest )
{
    if( m_pImgCurrent )
    {
        // Draw the current image
        rImage.drawGraphics( *m_pImgCurrent, 0, 0, xDest, yDest );
    }
}


void CtrlCheckbox::transUpOverDownOver( SkinObject *pCtrl )
{
    CtrlCheckbox *pThis = (CtrlCheckbox*)pCtrl;
    pThis->captureMouse();
    pThis->m_pImgCurrent = pThis->m_pImgDown;
    pThis->notifyLayout();
}


void CtrlCheckbox::transDownOverUpOver( SkinObject *pCtrl )
{
    CtrlCheckbox *pThis = (CtrlCheckbox*)pCtrl;
    pThis->releaseMouse();

    // Invert the state variable
    pThis->m_pImgCurrent = pThis->m_pImgUp;
    pThis->notifyLayout();

    // Execute the command
    pThis->m_pCommand->execute();
}


void CtrlCheckbox::transDownOverDown( SkinObject *pCtrl )
{
    CtrlCheckbox *pThis = (CtrlCheckbox*)pCtrl;
    pThis->m_pImgCurrent = pThis->m_pImgUp;
    pThis->notifyLayout();
}


void CtrlCheckbox::transDownDownOver( SkinObject *pCtrl )
{
    CtrlCheckbox *pThis = (CtrlCheckbox*)pCtrl;
    pThis->m_pImgCurrent = pThis->m_pImgDown;
    pThis->notifyLayout();
}


void CtrlCheckbox::transUpUpOver( SkinObject *pCtrl )
{
    CtrlCheckbox *pThis = (CtrlCheckbox*)pCtrl;
    pThis->m_pImgCurrent = pThis->m_pImgOver;
    pThis->notifyLayout();
}


void CtrlCheckbox::transUpOverUp( SkinObject *pCtrl )
{
    CtrlCheckbox *pThis = (CtrlCheckbox*)pCtrl;
    pThis->m_pImgCurrent = pThis->m_pImgUp;
    pThis->notifyLayout();
}


void CtrlCheckbox::transDownUp( SkinObject *pCtrl )
{
    CtrlCheckbox *pThis = (CtrlCheckbox*)pCtrl;
    pThis->releaseMouse();
}


void CtrlCheckbox::transUpHidden( SkinObject *pCtrl )
{
    CtrlCheckbox *pThis = (CtrlCheckbox*)pCtrl;
    pThis->m_pImgCurrent = NULL;
    pThis->notifyLayout();
}


void CtrlCheckbox::transHiddenUp( SkinObject *pCtrl )
{
    CtrlCheckbox *pThis = (CtrlCheckbox*)pCtrl;
    pThis->m_pImgCurrent = pThis->m_pImgUp;
    pThis->notifyLayout();
}


void CtrlCheckbox::onVarBoolUpdate( VarBool &rVariable )
{
    changeButton();
}


void CtrlCheckbox::changeButton()
{
    // Are we using the first set of images or the second one?
    if( m_pImgUp == m_pImgUp1 )
    {
        m_pImgUp = m_pImgUp2;
        m_pImgOver = m_pImgOver2;
        m_pImgDown = m_pImgDown2;
        m_pTooltip = &m_tooltip2;
        m_pCommand = &m_rCommand2;
    }
    else
    {
        m_pImgUp = m_pImgUp1;
        m_pImgOver = m_pImgOver1;
        m_pImgDown = m_pImgDown1;
        m_pTooltip = &m_tooltip1;
        m_pCommand = &m_rCommand1;
    }
    // XXX: We assume that the checkbox is up
    m_pImgCurrent = m_pImgUp;

    // Notify the window the tooltip has changed
    notifyTooltipChange();
    // Refresh
    notifyLayout();
}

