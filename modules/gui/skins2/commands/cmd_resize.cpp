/*****************************************************************************
 * cmd_resize.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: cmd_resize.cpp 6961 2004-03-05 17:34:23Z sam $
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

#include "cmd_resize.hpp"
#include "../src/generic_layout.hpp"


CmdResize::CmdResize( intf_thread_t *pIntf, GenericLayout &rLayout, int width,
                      int height ):
    CmdGeneric( pIntf ), m_rLayout( rLayout ), m_width( width ),
    m_height( height )
{
}


void CmdResize::execute()
{
    // Resize the layout
    m_rLayout.resize( m_width, m_height );
}
