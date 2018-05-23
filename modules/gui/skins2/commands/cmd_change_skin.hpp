/*****************************************************************************
 * cmd_change_skin.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: cmd_change_skin.hpp 8524 2004-08-25 21:32:15Z ipkiss $
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

#ifndef CMD_CHANGE_SKIN_HPP
#define CMD_CHANGE_SKIN_HPP

#include "cmd_generic.hpp"


/// "Change Skin" command
class CmdChangeSkin: public CmdGeneric
{
    public:
        CmdChangeSkin( intf_thread_t *pIntf, const string &rFile ):
            CmdGeneric( pIntf ), m_file( rFile ) {}
        virtual ~CmdChangeSkin() {}

        /// This method does the real job of the command
        virtual void execute();

        /// Return the type of the command
        virtual string getType() const { return "change skin"; }

    private:
        /// Skin file to load
        string m_file;
};

#endif
