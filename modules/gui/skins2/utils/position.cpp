/*****************************************************************************
 * position.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: position.cpp 6961 2004-03-05 17:34:23Z sam $
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

#include "position.hpp"


Rect::Rect( int left, int top, int right, int bottom ):
    m_left( left ), m_top( top ), m_right( right ), m_bottom( bottom )
{
}


Position::Position( int left, int top, int right, int bottom, const Box &rBox,
                    Ref_t refLeftTop, Ref_t refRightBottom ):
    m_left( left ), m_top( top ), m_right( right ), m_bottom( bottom ),
    m_rBox( rBox ), m_refLeftTop( refLeftTop ),
    m_refRighBottom( refRightBottom )
{
}


int Position::getLeft() const
{
    switch( m_refLeftTop )
    {
        case kLeftTop:
        case kLeftBottom:
            return m_left;
            break;
        case kRightTop:
        case kRightBottom:
            return m_rBox.getWidth() + m_left - 1;
            break;
    }
    // Avoid a warning
    return 0;
}


int Position::getTop() const
{
    switch( m_refLeftTop )
    {
        case kLeftTop:
        case kRightTop:
            return m_top;
            break;
        case kRightBottom:
        case kLeftBottom:
            return m_rBox.getHeight() + m_top - 1;
            break;
    }
    // Avoid a warning
    return 0;
}


int Position::getRight() const
{
    switch( m_refRighBottom )
    {
        case kLeftTop:
        case kLeftBottom:
            return m_right;
            break;
        case kRightTop:
        case kRightBottom:
            return m_rBox.getWidth() + m_right - 1;
            break;
    }
    // Avoid a warning
    return 0;
}


int Position::getBottom() const
{
    switch( m_refRighBottom )
    {
        case kLeftTop:
        case kRightTop:
            return m_bottom;
            break;
        case kLeftBottom:
        case kRightBottom:
            return m_rBox.getHeight() + m_bottom - 1;
            break;
    }
    // Avoid a warning
    return 0;
}


int Position::getWidth() const
{
    return getRight() - getLeft() + 1;
}


int Position::getHeight() const
{
    return getBottom() - getTop() + 1;
}

