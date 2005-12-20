/*****************************************************************************
 * win32_dragdrop.h: Win32 implementation of the drag & drop
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: win32_dragdrop.h 6961 2004-03-05 17:34:23Z sam $
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

#ifdef WIN32

#ifndef VLC_SKIN_WIN32_DRAGDROP
#define VLC_SKIN_WIN32_DRAGDROP

//--- WIN32 -----------------------------------------------------------------
#include <shellapi.h>
#include <ole2.h>

//---------------------------------------------------------------------------
class Win32DropObject : public IDropTarget
{
    public:
       Win32DropObject( bool playondrop );
       virtual ~Win32DropObject();

    protected:
        // IUnknown methods
        STDMETHOD(QueryInterface)( REFIID riid, void FAR* FAR* ppvObj );
        STDMETHOD_(ULONG, AddRef)();
        STDMETHOD_(ULONG, Release)();

        // IDropTarget methods
        STDMETHOD(DragEnter)( LPDATAOBJECT pDataObj, DWORD grfKeyState,
                              POINTL pt, DWORD *pdwEffect );
        STDMETHOD(DragOver)( DWORD grfKeyState, POINTL pt, DWORD *pdwEffect );
        STDMETHOD(DragLeave)();
        STDMETHOD(Drop)( LPDATAOBJECT pDataObj, DWORD grfKeyState,
                         POINTL pt, DWORD *pdwEffect );

    private:
        unsigned long References;
        bool PlayOnDrop;

        // Helper function
        void HandleDrop( HDROP HDrop );
};
//---------------------------------------------------------------------------
#endif

#endif
