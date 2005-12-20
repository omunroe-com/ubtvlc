/*****************************************************************************
 * font.h: Font class
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: font.h 6961 2004-03-05 17:34:23Z sam $
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


#ifndef VLC_SKIN_FONT
#define VLC_SKIN_FONT

//--- GENERAL ---------------------------------------------------------------
#include <string>
using namespace std;

//---------------------------------------------------------------------------
struct intf_thread_t;
class Graphics;

//---------------------------------------------------------------------------
class SkinFont
{
    private:
        // Assign font to Device Context
        virtual void AssignFont( Graphics *dest ) = 0;

        // Helper function
        virtual void GenericPrint( Graphics *dest, string text, int x, int y,
                                   int w, int h, int align, int color ) = 0;
    protected:
        int Weight;
        string FontName;
        bool Italic;
        bool Underline;
        int Size;
        int Color;
        intf_thread_t *p_intf;

    public:
        // Constructor
        SkinFont( intf_thread_t *_p_intf, string fontname, int size, int color,
              int weight, bool italic, bool underline );

        // Destructor
        virtual ~SkinFont();

        // Get size of text
        virtual void GetSize( string text, int &w, int &h ) = 0;

        // Draw text with boundaries
        virtual void Print( Graphics *dest, string text, int x, int y, int w,
                            int h, int align ) = 0;

        virtual void PrintColor( Graphics *dest, string text, int x, int y,
                                 int w, int h, int align, int color ) = 0;

};
//---------------------------------------------------------------------------

#endif
