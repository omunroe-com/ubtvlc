/*****************************************************************************
 * macosx_dragdrop.cpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 * $Id: macosx_dragdrop.cpp 11664 2005-07-09 06:17:09Z courmisch $
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
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

#ifdef MACOSX_SKINS

#include "macosx_dragdrop.hpp"


MacOSXDragDrop::MacOSXDragDrop( intf_thread_t *pIntf, bool playOnDrop ):
    SkinObject( pIntf ), m_playOnDrop( playOnDrop )
{
    // TODO
}


#endif
