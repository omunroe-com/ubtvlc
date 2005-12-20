/*****************************************************************************
 * evt_mouse.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: evt_mouse.cpp 6961 2004-03-05 17:34:23Z sam $
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

#include "evt_mouse.hpp"


const string EvtMouse::getAsString() const
{
    string event = "mouse";

    // Add the button
    if( m_button == kLeft )
        event += ":left";
    else if( m_button == kMiddle )
        event += ":middle";
    else if( m_button == kRight )
        event += ":right";
    else
        msg_Warn( getIntf(), "Unknown button type" );

    // Add the action
    if( m_action == kDown )
        event += ":down";
    else if( m_action == kUp )
        event += ":up";
    else if( m_action == kDblClick )
        event += ":dblclick";
    else
        msg_Warn( getIntf(), "Unknown action type" );

    // Add the modifier
    addModifier( event );

    return event;
}

