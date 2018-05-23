/*****************************************************************************
 * win32_graphics.h: Win32 implementation of the Graphics and Region classes
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: win32_graphics.h 6961 2004-03-05 17:34:23Z sam $
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

#ifndef VLC_SKIN_WIN32_GRAPHICS
#define VLC_SKIN_WIN32_GRAPHICS

//---------------------------------------------------------------------------
class SkinRegion;
class SkinWindow;

//---------------------------------------------------------------------------
class Win32Graphics : public Graphics
{
    protected:
        HDC Image;

    public:
        // Constructor
        Win32Graphics( intf_thread_t *p_intf, int w, int h, SkinWindow *from = NULL );
        // Destructor
        virtual ~Win32Graphics();
        // Drawing methods
        virtual void CopyFrom( int dx, int dy, int dw, int dh, Graphics *Src,
                              int sx, int sy, int Flag );
        //virtual void CopyTo(  Graphics *Dest, int dx, int dy, int dw, int dh,
        //                      int sx, int sy, int Flag );
        virtual void DrawRect( int x, int y, int w, int h, int color );

        // Clipping methods
        virtual void SetClipRegion( SkinRegion *rgn );
        virtual void ResetClipRegion();

        // Specific win32 methods
        HDC GetImageHandle()    { return Image; };
};
//---------------------------------------------------------------------------
class Win32Region : public SkinRegion
{
    private:
        HRGN Rgn;
    public:
        // Constructor
        Win32Region();
        Win32Region( int x, int y, int w, int h );
        // Destructor
        ~Win32Region();
        // Modify region
        virtual void AddPoint( int x, int y );
        virtual void AddRectangle( int x, int y, int w, int h );
        virtual void AddElipse( int x, int y, int w, int h );
        virtual void Move( int x, int y );

        virtual bool Hit( int x, int y );

        // Specific win32 methods
        HRGN GetHandle() { return Rgn; };
};
//---------------------------------------------------------------------------

#endif

#endif
