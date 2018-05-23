/*****************************************************************************
 * xmlparser.hpp
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 * $Id: xmlparser.hpp 6961 2004-03-05 17:34:23Z sam $
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

#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include "../src/skin_common.hpp"
#include <libxml/xmlreader.h>
#include <map>

/// XML parser using libxml2 text reader API
class XMLParser: public SkinObject
{
    public:
        XMLParser( intf_thread_t *pIntf, const string &rFileName );
        virtual ~XMLParser();

        /// Parse the file. Returns true on success
        bool parse();

    protected:
        // Key comparison function for type "const char*"
        struct ltstr
        {
            bool operator()(const char* s1, const char* s2) const
            {
                return strcmp(s1, s2) < 0;
            }
        };
        /// Type for attribute lists
        typedef map<const char*, const char*, ltstr> AttrList_t;

        /// Flag for validation errors
        bool m_errors;

        /// Callbacks
        virtual void handleBeginElement( const string &rName, AttrList_t &attr ) {}
        virtual void handleEndElement( const string &rName ) {}

    private:
        /// Reader context
        xmlTextReaderPtr m_pReader;

        /// Callback for validation errors
        static void handleError( void *pArg,  const char *pMsg,
                                 xmlParserSeverities severity,
                                 xmlTextReaderLocatorPtr locator);
};

#endif
