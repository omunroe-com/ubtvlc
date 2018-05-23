/*****************************************************************************
 * interpreter.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: interpreter.hpp 6961 2004-03-05 17:34:23Z sam $
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

#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "../commands/cmd_generic.hpp"
#include <map>

class Theme;
class VarBool;
class VarList;
class VarPercent;


/// Command interpreter for scripts in the XML
class Interpreter: public SkinObject
{
    public:
        /// Get the instance of Interpreter
        static Interpreter *instance( intf_thread_t *pIntf );

        /// Delete the instance of Interpreter
        static void destroy( intf_thread_t *pIntf );

        /// Parse an action tag and returns a pointer on a command
        /// (the intepreter takes care of deleting it, don't do it
        ///  yourself !)
        CmdGeneric *parseAction( const string &rAction, Theme *pTheme );

        /// Returns the boolean variable corresponding to the given name
        VarBool *getVarBool( const string &rName, Theme *pTheme );


        /// Returns the percent variable corresponding to the given name
        VarPercent *getVarPercent( const string &rName, Theme *pTheme );

        /// Returns the list variable corresponding to the given name
        VarList *getVarList( const string &rName, Theme *pTheme );

    private:
        /// Map of global commands
        map<string, CmdGenericPtr> m_commandMap;

        // Private because it is a singleton
        Interpreter( intf_thread_t *pIntf );
        virtual ~Interpreter() {}
};

#endif
