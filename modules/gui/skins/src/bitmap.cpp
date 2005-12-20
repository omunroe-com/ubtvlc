/*****************************************************************************
 * bitmap.cpp: Bitmap class
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: bitmap.cpp 6961 2004-03-05 17:34:23Z sam $
 *
 * Authors: Olivier Teuli�re <ipkiss@via.ecp.fr>
 *          Emmanuel Puig    <karibu@via.ecp.fr>
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111,
 * USA.
 *****************************************************************************/


//--- VLC -------------------------------------------------------------------
#include <vlc/intf.h>

//--- SKIN ------------------------------------------------------------------
#include "bitmap.h"



//---------------------------------------------------------------------------
//   Bitmap
//---------------------------------------------------------------------------
Bitmap::Bitmap( intf_thread_t *_p_intf, string FileName, int AColor )
{
    p_intf     = _p_intf;
    AlphaColor = AColor;
}
//---------------------------------------------------------------------------
Bitmap::Bitmap( intf_thread_t *_p_intf, Graphics *from, int x, int y,
                int w, int h, int AColor )
{
    p_intf     = _p_intf;
    AlphaColor = AColor;
}
//---------------------------------------------------------------------------
Bitmap::Bitmap( intf_thread_t *_p_intf, Bitmap *c )
{
    p_intf = _p_intf;
}
//---------------------------------------------------------------------------
Bitmap::~Bitmap()
{
}
//---------------------------------------------------------------------------
void Bitmap::GetSize( int &w, int &h )
{
    w = Width;
    h = Height;
}
//---------------------------------------------------------------------------

