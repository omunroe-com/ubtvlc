/*****************************************************************************
 * var_text.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: var_text.cpp 7715 2004-05-18 18:47:02Z ipkiss $
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

#include "var_text.hpp"
#include "../src/vlcproc.hpp"
#include "../src/var_manager.hpp"
#include "../vars/time.hpp"
#include "../vars/volume.hpp"
#include "../vars/stream.hpp"


const string VarText::m_type = "text";


VarText::VarText( intf_thread_t *pIntf ): Variable( pIntf ),
    m_text( pIntf, "" ), m_lastText( pIntf, "" )
{
}


VarText::~VarText()
{
    // Remove the observers
    VlcProc *pVlcProc = VlcProc::instance( getIntf() );
    pVlcProc->getTimeVar().delObserver( this );
    pVlcProc->getVolumeVar().delObserver( this );
    pVlcProc->getStreamVar().delObserver( this );
    VarManager *pVarManager = VarManager::instance( getIntf() );
    pVarManager->getHelpText().delObserver( this );
}


const UString VarText::get() const
{
    uint32_t pos;
    VlcProc *pVlcProc = VlcProc::instance( getIntf() );

    // Fill a temporary UString object, and replace the escape characters
    // ($H for help, $T for current time, $L for time left, $D for duration,
    // $V for volume)
    UString temp( m_text );

    // $H is processed first, in case the help string contains other variables
    // to replace. And it is replaced only once, in case one of these other
    // variables is $H...
    if( (pos = temp.find( "$H" )) != UString::npos )
    {
        VarManager *pVarManager = VarManager::instance( getIntf() );
        // We use .getRaw() to avoid replacing the $H recursively!
        temp.replace( pos, 2, pVarManager->getHelpText().getRaw() );
    }
    while( (pos = temp.find( "$T" )) != UString::npos )
    {
        temp.replace( pos, 2,
                      pVlcProc->getTimeVar().getAsStringCurrTime().c_str() );
    }
    while( (pos = temp.find( "$L" )) != UString::npos )
    {
        temp.replace( pos, 2,
                      pVlcProc->getTimeVar().getAsStringTimeLeft().c_str() );
    }
    while( (pos = temp.find( "$D" )) != UString::npos )
    {
        temp.replace( pos, 2,
                      pVlcProc->getTimeVar().getAsStringDuration().c_str() );
    }
    while( (pos = temp.find( "$V" )) != UString::npos )
    {
        temp.replace( pos, 2,
                      pVlcProc->getVolumeVar().getAsStringPercent().c_str() );
    }
    while( (pos = temp.find( "$N" )) != UString::npos )
    {
        temp.replace( pos, 2,
                      pVlcProc->getStreamVar().getAsStringName().c_str() );
    }
    while( (pos = temp.find( "$F" )) != UString::npos )
    {
        temp.replace( pos, 2,
                      pVlcProc->getStreamVar().getAsStringFullName().c_str() );
    }

    return temp;
}


void VarText::set( const UString &rText )
{
    // Avoid an infinite loop
    if( rText == m_text )
    {
        return;
    }

    // Stop observing other variables
    VlcProc *pVlcProc = VlcProc::instance( getIntf() );
    pVlcProc->getTimeVar().delObserver( this );
    pVlcProc->getVolumeVar().delObserver( this );
    pVlcProc->getStreamVar().delObserver( this );
    VarManager *pVarManager = VarManager::instance( getIntf() );
    pVarManager->getHelpText().delObserver( this );

    m_text = rText;

    // Observe needed variables
    if( m_text.find( "$H" ) != UString::npos )
    {
        pVarManager->getHelpText().addObserver( this );
    }
    if( m_text.find( "$T" ) != UString::npos )
    {
        pVlcProc->getTimeVar().addObserver( this );
    }
    if( m_text.find( "$L" ) != UString::npos )
    {
        pVlcProc->getTimeVar().addObserver( this );
    }
    if( m_text.find( "$D" ) != UString::npos )
    {
        pVlcProc->getTimeVar().addObserver( this );
    }
    if( m_text.find( "$V" ) != UString::npos )
    {
        pVlcProc->getVolumeVar().addObserver( this );
    }
    if( m_text.find( "$N" ) != UString::npos )
    {
        pVlcProc->getStreamVar().addObserver( this );
    }
    if( m_text.find( "$F" ) != UString::npos )
    {
        pVlcProc->getStreamVar().addObserver( this );
    }

    notify();
}


void VarText::onUpdate( Subject<VarPercent> &rVariable )
{
    UString newText = get();
    // If the text has changed, notify the observers
    if( newText != m_lastText )
    {
        m_lastText = newText;
        notify();
    }
}


void VarText::onUpdate( Subject<VarText> &rVariable )
{
    UString newText = get();
    // If the text has changed, notify the observers
    if( newText != m_lastText )
    {
        m_lastText = newText;
        notify();
    }
}
