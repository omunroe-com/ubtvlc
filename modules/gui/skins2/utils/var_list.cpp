/*****************************************************************************
 * var_list.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: var_list.cpp 6961 2004-03-05 17:34:23Z sam $
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

#include "var_list.hpp"


const string VarList::m_type = "list";


VarList::VarList( intf_thread_t *pIntf ): Variable( pIntf )
{
    // Create the position variable
    m_cPosition = VariablePtr( new VarPercent( pIntf ) );
    getPositionVar().set( 1.0 );
}


VarList::~VarList()
{
}


void VarList::add( const UStringPtr &rcString )
{
    m_list.push_back( Elem_t( rcString ) );
    notify();
}


void VarList::delSelected()
{
    Iterator it = begin();
    while( it != end() )
    {
        if( (*it).m_selected )
        {
            Iterator oldIt = it;
            it++;
            m_list.erase( oldIt );
        }
        else
        {
            it++;
        }
    }
    notify();
}


void VarList::clear()
{
    m_list.clear();
}


VarList::Iterator VarList::operator[]( int n )
{
    Iterator it = begin();
    for( int i = 0; i < n; i++ )
    {
        if( it != end() )
        {
            it++;
        }
        else
        {
            break;
        }
    }
    return it;
}


VarList::ConstIterator VarList::operator[]( int n ) const
{
    ConstIterator it = begin();
    for( int i = 0; i < n; i++ )
    {
        if( it != end() )
        {
            it++;
        }
        else
        {
            break;
        }
    }
    return it;
}

