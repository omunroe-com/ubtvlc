/*****************************************************************************
 * anchor.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: anchor.cpp 6961 2004-03-05 17:34:23Z sam $
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

#include "anchor.hpp"


bool Anchor::isHanging( const Anchor &rOther ) const
{
    if( m_priority <= rOther.m_priority )
        return false;

    // Compute delta coordinates between anchors, since the Bezier class
    // uses coordinates relative to (0;0)
    int deltaX = getXPosAbs() - rOther.getXPosAbs();
    int deltaY = getYPosAbs() - rOther.getYPosAbs();

    // One of the anchors (at least) must be a point, else it has no meaning
    return (isPoint() && rOther.m_rCurve.getMinDist( deltaX, deltaY ) == 0) ||
           (rOther.isPoint() && m_rCurve.getMinDist( -deltaX, -deltaY ) == 0);
}


bool Anchor::canHang( const Anchor &rOther, int &xOffset, int &yOffset ) const
{
    int deltaX = getXPosAbs() - (rOther.getXPosAbs() + xOffset);
    int deltaY = getYPosAbs() - (rOther.getYPosAbs() + yOffset);

    // One of the anchors (at least) must be a point, else it has no meaning
    if( (isPoint() && rOther.m_rCurve.getMinDist( deltaX, deltaY ) < m_range) )
    {
        // Compute the coordinates of the nearest point of the curve
        int xx, yy;
        float p = rOther.m_rCurve.getNearestPercent( deltaX, deltaY );
        rOther.m_rCurve.getPoint( p, xx, yy );

        xOffset = getXPosAbs() - (rOther.getXPosAbs() + xx);
        yOffset = getYPosAbs() - (rOther.getYPosAbs() + yy);
        return true;
    }
    else if( (rOther.isPoint() &&
              m_rCurve.getMinDist( -deltaX, -deltaY ) < m_range) )
    {
        // Compute the coordinates of the nearest point of the curve
        int xx, yy;
        float p = m_rCurve.getNearestPercent( -deltaX, -deltaY );
        m_rCurve.getPoint( p, xx, yy );

        xOffset = (getXPosAbs() + xx) - rOther.getXPosAbs();
        yOffset = (getYPosAbs() + yy) - rOther.getYPosAbs();
        return true;
    }
    else
    {
        return false;
    }
}
