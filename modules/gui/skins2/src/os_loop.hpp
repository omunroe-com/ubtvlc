/*****************************************************************************
 * os_loop.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: os_loop.hpp 7270 2004-04-03 23:21:47Z asmax $
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

#ifndef OS_LOOP_HPP
#define OS_LOOP_HPP

#include "skin_common.hpp"


/// Abstract class for the main event loop
class OSLoop: public SkinObject
{
    public:
        virtual ~OSLoop() {}

        /// Enter the main loop
        virtual void run() = 0;

        /// Exit the main loop
        virtual void exit() = 0;

    protected:
        OSLoop( intf_thread_t *pIntf ): SkinObject( pIntf ) {}
};


#endif
