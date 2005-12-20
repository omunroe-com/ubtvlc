/*****************************************************************************
 * skin_parser.hpp
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 * $Id: skin_parser.hpp 7369 2004-04-18 18:11:51Z ipkiss $
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
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

#ifndef SKIN_PARSER_HPP
#define SKIN_PARSER_HPP

#include "xmlparser.hpp"
#include "builder_data.hpp"
#include <set>


/// Parser for the skin DTD
class SkinParser: public XMLParser
{
    public:
        SkinParser( intf_thread_t *pIntf, const string &rFileName,
                    const string &rPath );
        virtual ~SkinParser() {}

        const BuilderData &getData() const { return m_data; }

    private:
        // Static variable to avoid initializing catalogs twice
        static bool m_initialized;
        /// Container for mapping data from the XML
        BuilderData m_data;
        /// Current IDs
        string m_curWindowId;
        string m_curLayoutId;
        string m_curListId;
        /// Current offset of the controls
        int m_xOffset, m_yOffset;
        list<int> m_xOffsetList, m_yOffsetList;
        /// Layer of the current control in the layout
        int m_curLayer;
        /// Set of used id
        set<string> m_idSet;
        /// Path of the XML file being parsed
        const string m_path;

        /// Callbacks
        virtual void handleBeginElement( const string &rName,
                                         AttrList_t &attr );
        virtual void handleEndElement( const string &rName );

        /// Helper functions
        //@{
        bool convertBoolean( const char *value ) const;
        int convertColor( const char *transcolor ) const;
        string convertFileName( const char *fileName ) const;
        /// Transform to int, and check that it is in the given range (if not,
        /// the closest range boundary will be used)
        int convertInRange( const char *value, int minValue, int maxValue,
                            const string &rAttribute ) const;
        //@}

        /// Generate a new id
        const string generateId() const;

        /// Check if the id is unique, and if not generate a new one
        const string uniqueId( const string &id );
};

#endif
