/*****************************************************************************
 * cmd_change_skin.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: cmd_change_skin.cpp 8524 2004-08-25 21:32:15Z ipkiss $
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

#include "cmd_change_skin.hpp"
#include "cmd_quit.hpp"
#include "../src/os_factory.hpp"
#include "../src/os_loop.hpp"
#include "../src/theme.hpp"
#include "../src/theme_loader.hpp"
#include "../src/window_manager.hpp"


void CmdChangeSkin::execute()
{
    // Save the old theme to restore it in case of problem
    Theme *pOldTheme = getIntf()->p_sys->p_theme;

    if( pOldTheme )
    {
        pOldTheme->getWindowManager().hideAll();
    }

    ThemeLoader loader( getIntf() );
    if( loader.load( m_file ) )
    {
        // Everything went well
        msg_Dbg( getIntf(), "New theme successfully loaded (%s)",
                 m_file.c_str() );
        if( pOldTheme )
        {
            delete pOldTheme;
        }
    }
    else if( pOldTheme )
    {
        msg_Err( getIntf(), "A problem occurred when loading the new theme,"
                  " restoring the previous one" );
        getIntf()->p_sys->p_theme = pOldTheme;
        pOldTheme->getWindowManager().showAll();
    }
    else
    {
        msg_Err( getIntf(), "Cannot load the theme, aborting" );
        // Quit
        CmdQuit cmd( getIntf() );
        cmd.execute();
    }
}

