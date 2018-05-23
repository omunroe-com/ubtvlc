/*****************************************************************************
 * anchor.h: Anchor class
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: anchor.h 6961 2004-03-05 17:34:23Z sam $
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


#ifndef VLC_SKIN_ANCHOR
#define VLC_SKIN_ANCHOR

//--- GENERAL ---------------------------------------------------------------
#include <list>
using namespace std;

//---------------------------------------------------------------------------
struct intf_thread_t;
class SkinWindow;
//---------------------------------------------------------------------------
class Anchor
{
    private:
        // Position parameters
        int Left;
        int Top;

        // Ray of action
        int Len;

        // Priority
        int Priority;

        // Parent window
        SkinWindow *Parent;

        // Interface thread
        intf_thread_t *p_intf;

    public:
        // Constructor
        Anchor( intf_thread_t *_p_intf, int x, int y, int len, int priority,
                SkinWindow *parent );

        // Hang to anchor if in neighbourhood
        bool Hang( Anchor *anc, int mx, int my );
        void Add( Anchor *anc );
        void Remove( Anchor *anc );
        bool IsInList( Anchor *anc );

        // List of windows actually magnetized
        list<Anchor *> HangList;

        // Get position of anchor
        void GetPos( int &x, int &y );

        // Getters
        int GetPriority()       { return Priority; }
        SkinWindow *GetParent()     { return Parent; }
};
//---------------------------------------------------------------------------

#endif
