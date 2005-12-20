/*****************************************************************************
 * ft2_font.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: ft2_font.hpp 7141 2004-03-22 20:38:15Z asmax $
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

#ifndef FT2_FONT_HPP
#define FT2_FONT_HPP

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <string>

#include "generic_font.hpp"

class UString;


/// Freetype2 font
class FT2Font: public GenericFont
{
    public:
        FT2Font( intf_thread_t *pIntf, const string &rName, int size );
        virtual ~FT2Font();

        /// Initalize the object. Returns false if it failed
        virtual bool init();

        /// Render a string on a bitmap.
        /// If maxWidth != -1, the text is truncated with '...'
        virtual GenericBitmap *drawString( const UString &rString,
            uint32_t color, int maxWidth = -1 ) const;

        /// Get the text height
        virtual int getSize() const { return m_height; }

    private:
        /// File name
        const string m_name;
        /// Buffer to store the font
        void *m_buffer;
        /// Pixel size of the font
        int m_size;
        /// Handle to FT library
        FT_Library m_lib;
        /// Font face
        FT_Face m_face;
        /// Font metrics
        int m_height, m_ascender, m_descender;
        /// Index, glyph, width and advance of the dot symbol
        int m_dotIndex;
        FT_Glyph m_dotGlyph;
        int m_dotWidth;
        int m_dotAdvance;
};


#endif
