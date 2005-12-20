/*****************************************************************************
 * interpreter.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: interpreter.cpp 8524 2004-08-25 21:32:15Z ipkiss $
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

#include "interpreter.hpp"
#include "expr_evaluator.hpp"
#include "../commands/cmd_playlist.hpp"
#include "../commands/cmd_dialogs.hpp"
#include "../commands/cmd_dummy.hpp"
#include "../commands/cmd_layout.hpp"
#include "../commands/cmd_quit.hpp"
#include "../commands/cmd_minimize.hpp"
#include "../commands/cmd_input.hpp"
#include "../commands/cmd_fullscreen.hpp"
#include "../commands/cmd_on_top.hpp"
#include "../commands/cmd_show_window.hpp"
#include "../src/theme.hpp"
#include "../src/var_manager.hpp"
#include "../src/vlcproc.hpp"


Interpreter::Interpreter( intf_thread_t *pIntf ): SkinObject( pIntf )
{
    /// Create the generic commands
#define REGISTER_CMD( name, cmd ) \
    m_commandMap[name] = CmdGenericPtr( new cmd( getIntf() ) );

    REGISTER_CMD( "none", CmdDummy )
    REGISTER_CMD( "dialogs.changeSkin()", CmdDlgChangeSkin )
    REGISTER_CMD( "dialogs.fileSimple()", CmdDlgFileSimple )
    REGISTER_CMD( "dialogs.file()", CmdDlgFile )
    REGISTER_CMD( "dialogs.disc()", CmdDlgDisc )
    REGISTER_CMD( "dialogs.net()", CmdDlgNet )
    REGISTER_CMD( "dialogs.messages()", CmdDlgMessages )
    REGISTER_CMD( "dialogs.prefs()", CmdDlgPrefs )
    REGISTER_CMD( "dialogs.fileInfo()", CmdDlgFileInfo )
    REGISTER_CMD( "dialogs.popup()", CmdDlgShowPopupMenu )
    REGISTER_CMD( "playlist.load()", CmdDlgPlaylistLoad )
    REGISTER_CMD( "playlist.save()", CmdDlgPlaylistSave )
    REGISTER_CMD( "playlist.add()", CmdDlgAdd )
    VarList &rVar = VlcProc::instance( getIntf() )->getPlaylistVar();
    m_commandMap["playlist.del()"] =
        CmdGenericPtr( new CmdPlaylistDel( getIntf(), rVar ) );
    REGISTER_CMD( "playlist.next()", CmdPlaylistNext )
    REGISTER_CMD( "playlist.previous()", CmdPlaylistPrevious )
    REGISTER_CMD( "playlist.sort()", CmdPlaylistSort )
    m_commandMap["playlist.setRandom(true)"] =
        CmdGenericPtr( new CmdPlaylistRandom( getIntf(), true ) );
    m_commandMap["playlist.setRandom(false)"] =
        CmdGenericPtr( new CmdPlaylistRandom( getIntf(), false ) );
    m_commandMap["playlist.setLoop(true)"] =
        CmdGenericPtr( new CmdPlaylistLoop( getIntf(), true ) );
    m_commandMap["playlist.setLoop(false)"] =
        CmdGenericPtr( new CmdPlaylistLoop( getIntf(), false ) );
    m_commandMap["playlist.setRepeat(true)"] =
        CmdGenericPtr( new CmdPlaylistRepeat( getIntf(), true ) );
    m_commandMap["playlist.setRepeat(false)"] =
        CmdGenericPtr( new CmdPlaylistRepeat( getIntf(), false ) );
    REGISTER_CMD( "vlc.fullscreen()", CmdFullscreen )
    REGISTER_CMD( "vlc.play()", CmdPlay )
    REGISTER_CMD( "vlc.pause()", CmdPause )
    REGISTER_CMD( "vlc.stop()", CmdStop )
    REGISTER_CMD( "vlc.faster()", CmdFaster )
    REGISTER_CMD( "vlc.slower()", CmdSlower )
    REGISTER_CMD( "vlc.mute()", CmdMute )
    REGISTER_CMD( "vlc.minimize()", CmdMinimize )
    REGISTER_CMD( "vlc.onTop()", CmdOnTop )
    REGISTER_CMD( "vlc.quit()", CmdQuit )

    // Register the constant bool variables in the var manager
    VarManager *pVarManager = VarManager::instance( getIntf() );
    VarBool *pVarTrue = new VarBoolTrue( getIntf() );
    pVarManager->registerVar( VariablePtr( pVarTrue ), "true" );
    VarBool *pVarFalse = new VarBoolFalse( getIntf() );
    pVarManager->registerVar( VariablePtr( pVarFalse ), "false" );
}


Interpreter *Interpreter::instance( intf_thread_t *pIntf )
{
    if( ! pIntf->p_sys->p_interpreter )
    {
        Interpreter *pInterpreter;
        pInterpreter = new Interpreter( pIntf );
        if( pInterpreter )
        {
            pIntf->p_sys->p_interpreter = pInterpreter;
        }
    }
    return pIntf->p_sys->p_interpreter;
}


void Interpreter::destroy( intf_thread_t *pIntf )
{
    if( pIntf->p_sys->p_interpreter )
    {
        delete pIntf->p_sys->p_interpreter;
        pIntf->p_sys->p_interpreter = NULL;
    }
}


CmdGeneric *Interpreter::parseAction( const string &rAction, Theme *pTheme )
{
    // Try to find the command in the global command map
    if( m_commandMap.find( rAction ) != m_commandMap.end() )
    {
        return m_commandMap[rAction].get();
    }

    CmdGeneric *pCommand = NULL;

    if( rAction.find( ".setLayout(" ) != string::npos )
    {
        int leftPos = rAction.find( ".setLayout(" );
        string windowId = rAction.substr( 0, leftPos );
        // 11 is the size of ".setLayout("
        int rightPos = rAction.find( ")", windowId.size() + 11 );
        string layoutId = rAction.substr( windowId.size() + 11,
                                          rightPos - (windowId.size() + 11) );
        pCommand = new CmdLayout( getIntf(), windowId, layoutId );
    }
    else if( rAction.find( ".show()" ) != string::npos )
    {
        int leftPos = rAction.find( ".show()" );
        string windowId = rAction.substr( 0, leftPos );
        TopWindow *pWin = pTheme->getWindowById( windowId );
        if( pWin )
        {
            pCommand = new CmdShowWindow( getIntf(), pTheme->getWindowManager(),
                                          *pWin );
        }
        else
        {
            msg_Err( getIntf(), "Unknown window (%s)", windowId.c_str() );
        }
    }
    else if( rAction.find( ".hide()" ) != string::npos )
    {
        int leftPos = rAction.find( ".hide()" );
        string windowId = rAction.substr( 0, leftPos );
        TopWindow *pWin = pTheme->getWindowById( windowId );
        if( pWin )
        {
            pCommand = new CmdHideWindow( getIntf(), pTheme->getWindowManager(),
                                          *pWin );
        }
        else
        {
            msg_Err( getIntf(), "Unknown window (%s)", windowId.c_str() );
        }
    }

    if( pCommand )
    {
        // Add the command in the pool
        pTheme->m_commands.push_back( CmdGenericPtr( pCommand ) );
    }

    return pCommand;
}


VarBool *Interpreter::getVarBool( const string &rName, Theme *pTheme )
{
    VarManager *pVarManager = VarManager::instance( getIntf() );

   // Convert the expression into Reverse Polish Notation
    ExprEvaluator *pEvaluator = new ExprEvaluator( getIntf() );
    pEvaluator->parse( rName );

    list<VarBool*> varStack;

    // Get the first token from the RPN stack
    string token = pEvaluator->getToken();
    while( !token.empty() )
    {
        if( token == "and" )
        {
            // Get the 2 last variables on the stack
            if( varStack.empty() )
            {
                msg_Err( getIntf(), "Invalid boolean expression: %s",
                         rName.c_str());
                return NULL;
            }
            VarBool *pVar1 = varStack.back();
            varStack.pop_back();
            if( varStack.empty() )
            {
                msg_Err( getIntf(), "Invalid boolean expression: %s",
                         rName.c_str());
                return NULL;
            }
            VarBool *pVar2 = varStack.back();
            varStack.pop_back();

            // Create a composite boolean variable
            VarBool *pNewVar = new VarBoolAndBool( getIntf(), *pVar1, *pVar2 );
            varStack.push_back( pNewVar );
            // Register this variable in the manager
            pVarManager->registerVar( VariablePtr( pNewVar ) );
        }
        else if( token == "or" )
        {
            // Get the 2 last variables on the stack
            if( varStack.empty() )
            {
                msg_Err( getIntf(), "Invalid boolean expression: %s",
                         rName.c_str());
                return NULL;
            }
            VarBool *pVar1 = varStack.back();
            varStack.pop_back();
            if( varStack.empty() )
            {
                msg_Err( getIntf(), "Invalid boolean expression: %s",
                         rName.c_str());
                return NULL;
            }
            VarBool *pVar2 = varStack.back();
            varStack.pop_back();

            // Create a composite boolean variable
            VarBool *pNewVar = new VarBoolOrBool( getIntf(), *pVar1, *pVar2 );
            varStack.push_back( pNewVar );
            // Register this variable in the manager
            pVarManager->registerVar( VariablePtr( pNewVar ) );
        }
        else if( token == "not" )
        {
            // Get the last variable on the stack
            if( varStack.empty() )
            {
                msg_Err( getIntf(), "Invalid boolean expression: %s",
                         rName.c_str());
                return NULL;
            }
            VarBool *pVar = varStack.back();
            varStack.pop_back();

            // Create a composite boolean variable
            VarBool *pNewVar = new VarNotBool( getIntf(), *pVar );
            varStack.push_back( pNewVar );
            // Register this variable in the manager
            pVarManager->registerVar( VariablePtr( pNewVar ) );
        }
        else if( token.find( ".isVisible" ) != string::npos )
        {
            int leftPos = token.find( ".isVisible" );
            string windowId = token.substr( 0, leftPos );
            TopWindow *pWin = pTheme->getWindowById( windowId );
            if( pWin )
            {
                // Push the visibility variable on the stack
                varStack.push_back( &pWin->getVisibleVar() );
            }
            else
            {
                msg_Err( getIntf(), "Unknown window (%s)", windowId.c_str() );
                return NULL;
            }
        }
        else
        {
            // Try to get the variable from the variable manager
            VarBool *pVar = (VarBool*)pVarManager->getVar( token, "bool" );
            if( !pVar )
            {
                msg_Err( getIntf(), "Cannot resolve boolean variable: %s",
                         token.c_str());
                return NULL;
            }
            varStack.push_back( pVar );
        }
        // Get the first token from the RPN stack
        token = pEvaluator->getToken();
    }

    // The stack should contain a single variable
    if( varStack.size() != 1 )
    {
        msg_Err( getIntf(), "Invalid boolean expression: %s", rName.c_str() );
        return NULL;
    }
    return varStack.back();
}


VarPercent *Interpreter::getVarPercent( const string &rName, Theme *pTheme )
{
    // Try to get the variable from the variable manager
    VarManager *pVarManager = VarManager::instance( getIntf() );
    VarPercent *pVar = (VarPercent*)pVarManager->getVar( rName, "percent" );
    return pVar;
}


VarList *Interpreter::getVarList( const string &rName, Theme *pTheme )
{
    // Try to get the variable from the variable manager
    VarManager *pVarManager = VarManager::instance( getIntf() );
    VarList *pVar = (VarList*)pVarManager->getVar( rName, "list" );
    return pVar;
}

