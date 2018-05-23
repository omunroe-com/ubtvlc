/*****************************************************************************
 * x11_font.cpp: X11 implementation of the Font class
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: x11_font.cpp 6961 2004-03-05 17:34:23Z sam $
 *
 * Authors: Cyril Deguet     <asmax@videolan.org>
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

#ifdef X11_SKINS

//--- X11 -----------------------------------------------------------------
#include <X11/Xlib.h>

//--- VLC -------------------------------------------------------------------
#include <vlc/intf.h>

//--- SKIN ------------------------------------------------------------------
#include "../src/skin_common.h"
#include "../src/graphics.h"
#include "../os_graphics.h"
#include "../src/font.h"
#include "../os_font.h"
#include "../src/theme.h"
#include "../os_theme.h"

//---------------------------------------------------------------------------
// Font object
//---------------------------------------------------------------------------
X11Font::X11Font( intf_thread_t *_p_intf, string fontname, int size,
    int color, int weight, bool italic, bool underline )
    : SkinFont( _p_intf, fontname, size, color, weight, italic, underline )
{
    display = g_pIntf->p_sys->display;
    Underline = underline;

    char name[256];
    char slant = ( italic ? 'i' : 'r' );
    // FIXME: a lot of work...
    size = ( size < 10 ? 8 : 12 );
    snprintf( name, 256, "-*-helvetica-bold-%c-*-*-*-%i-*-*-*-*-*-*", 
              slant, 10 * size );
    XLOCK;
    FontInfo = XLoadQueryFont( display, name );
    font = FontInfo->fid;
    Ascent = FontInfo->max_bounds.ascent;
    Descent = FontInfo->max_bounds.descent;
    XUNLOCK;
}
//---------------------------------------------------------------------------
X11Font::~X11Font()
{
    XLOCK;
    XFreeFont( display, FontInfo );
    XUNLOCK;
}
//---------------------------------------------------------------------------
void X11Font::AssignFont( Graphics *dest )
{
}
//---------------------------------------------------------------------------
void X11Font::GetSize( string text, int &w, int &h )
{
    int direction, fontAscent, fontDescent;
    XCharStruct overall;
    
    XLOCK;
    XQueryTextExtents( display, font, text.c_str(), text.size(), 
                        &direction, &fontAscent, &fontDescent, &overall );
    XUNLOCK;

    w = overall.rbearing - overall.lbearing;
    h = Ascent + Descent;
}
//---------------------------------------------------------------------------
void X11Font::GenericPrint( Graphics *dest, string text, int x, int y,
                                 int w, int h, int align, int color )
{
    // Get handles
    Drawable drawable = ( (X11Graphics *)dest )->GetImage();
    GC gc = ( (X11Graphics *)dest )->GetGC();

    XGCValues gcVal;
    // Change color to avoid transparency
    gcVal.foreground = (color < 10 ? 10 : color);
    gcVal.font = font;
    XRectangle rect;
    rect.x = x;
    rect.y = y;
    rect.width = w;
    rect.height = h;

    XLOCK;
    XChangeGC( display, gc, GCForeground|GCFont, &gcVal );
    // Set the clipping region
    XSetClipRectangles( display, gc, 0, 0, &rect, 1, Unsorted );  
    
    // Render text no the drawable
    XDrawString( display, drawable, gc, x, y+Ascent, text.c_str(), text.size());
    if( Underline )
    {
        XDrawLine( display, drawable, gc, x, y+Ascent+1, x+w, y+Ascent+1 );
    }
    
    // Reset the clip mask
    XSetClipMask( display, gc, None );
    XUNLOCK;
}

//---------------------------------------------------------------------------
void X11Font::Print( Graphics *dest, string text, int x, int y, int w,
                       int h, int align )
{
    GenericPrint( dest, text, x, y, w, h, align, Color );
}
//---------------------------------------------------------------------------
void X11Font::PrintColor( Graphics *dest, string text, int x, int y, int w,
                            int h, int align, int color )
{
    GenericPrint( dest, text, x, y, w, h, align, color );
}
//---------------------------------------------------------------------------

#endif
