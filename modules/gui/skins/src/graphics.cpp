/*****************************************************************************
 * graphics.cpp: Graphics class
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: graphics.cpp 6961 2004-03-05 17:34:23Z sam $
 *
 * Authors: Olivier Teulière <ipkiss@via.ecp.fr>
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


//--- SKIN ------------------------------------------------------------------
#include "graphics.h"



//---------------------------------------------------------------------------
Graphics::Graphics( int w, int h )
{
    Width  = w;
    Height = h;
}
//---------------------------------------------------------------------------
Graphics::~Graphics()
{
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// REGION
//---------------------------------------------------------------------------
SkinRegion::SkinRegion()
{
}
//---------------------------------------------------------------------------
SkinRegion::SkinRegion( int x, int y, int w, int h )
{
}
//---------------------------------------------------------------------------
SkinRegion::~SkinRegion()
{
}
//---------------------------------------------------------------------------
