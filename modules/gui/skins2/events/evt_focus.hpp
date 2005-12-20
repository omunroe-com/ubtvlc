/*****************************************************************************
 * evt_focus.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: evt_focus.hpp 6961 2004-03-05 17:34:23Z sam $
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

#ifndef EVT_FOCUS_HPP
#define EVT_FOCUS_HPP

#include "evt_generic.hpp"


/// Focus change event
class EvtFocus: public EvtGeneric
{
    public:
        EvtFocus( intf_thread_t *pIntf, bool focus ): EvtGeneric( pIntf ),
            m_focus( focus ) {}
        virtual ~EvtFocus() {}

        /// Return the type of event
        virtual const string getAsString() const
        {
            return ( m_focus ? "focus:in" : "focus:out" );
        }

    private:
        /// true for a focus in, and false for a focus out
        bool m_focus;
};


#endif
