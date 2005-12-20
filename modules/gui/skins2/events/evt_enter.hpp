/*****************************************************************************
 * evt_enter.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: evt_enter.hpp 6961 2004-03-05 17:34:23Z sam $
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

#ifndef EVT_ENTER_HPP
#define EVT_ENTER_HPP

#include "evt_input.hpp"


/// Mouse enter event
class EvtEnter: public EvtInput
{
    public:
        EvtEnter( intf_thread_t *pIntf ): EvtInput( pIntf ) {}
        virtual ~EvtEnter() {}

        /// Return the type of event
        virtual const string getAsString() const { return "enter"; }
};


#endif
