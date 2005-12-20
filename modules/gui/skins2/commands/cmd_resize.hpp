/*****************************************************************************
 * cmd_resize.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: cmd_resize.hpp 6961 2004-03-05 17:34:23Z sam $
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

#ifndef CMD_RESIZE_HPP
#define CMD_RESIZE_HPP

#include "cmd_generic.hpp"

class GenericLayout;


/// Command to resize a layout
class CmdResize: public CmdGeneric
{
    public:
        /// Resize the given layout
        CmdResize( intf_thread_t *pIntf, GenericLayout &rLayout, int width,
                   int height );
        virtual ~CmdResize() {}

        /// This method does the real job of the command
        virtual void execute();

        /// Return the type of the command
        virtual string getType() const { return "resize"; }

    private:
        GenericLayout &m_rLayout;
        int m_width, m_height;
};

#endif
