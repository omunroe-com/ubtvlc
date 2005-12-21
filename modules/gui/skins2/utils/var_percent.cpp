/*****************************************************************************
 * var_percent.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 * $Id: var_percent.cpp 11786 2005-07-18 23:57:41Z asmax $
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

#include "var_percent.hpp"


const string VarPercent::m_type = "percent";


void VarPercent::set( float percentage )
{
    if( percentage < 0 )
    {
        percentage = 0;
    }
    if( percentage > 1 )
    {
        percentage = 1;
    }

    // If the value has changed, notify the observers
    if( m_value != percentage )
    {
        m_value = percentage;
        notify();
    }
}

