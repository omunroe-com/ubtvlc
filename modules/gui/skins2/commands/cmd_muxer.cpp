/*****************************************************************************
 * cmd_muxer.cpp
 *****************************************************************************
 * Copyright (C) 2005 VideoLAN
 * $Id: cmd_muxer.cpp 10770 2005-04-22 22:25:10Z ipkiss $
 *
 * Authors: Olivier Teuli�re <ipkiss@via.ecp.fr>
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

#include "cmd_muxer.hpp"


void CmdMuxer::execute()
{
    list<CmdGeneric*>::const_iterator it;
    for( it = m_list.begin(); it != m_list.end(); it++ )
    {
        (*it)->execute();
    }
}
