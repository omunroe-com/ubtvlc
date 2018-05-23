/*****************************************************************************
 * logger.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: logger.cpp 6961 2004-03-05 17:34:23Z sam $
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

#include "logger.hpp"


Logger::Logger( intf_thread_t *pIntf ): SkinObject( pIntf )
{
}


Logger::~Logger()
{
}


Logger *Logger::instance( intf_thread_t *pIntf )
{
    if( ! pIntf->p_sys->p_logger )
    {
        Logger *pLogger = new Logger( pIntf );
        if( pLogger )
        {
            // Initialization succeeded
            pIntf->p_sys->p_logger = pLogger;
        }
    }
    return pIntf->p_sys->p_logger;
}


void Logger::destroy( intf_thread_t *pIntf )
{
    if( pIntf->p_sys->p_logger )
    {
        delete pIntf->p_sys->p_logger;
        pIntf->p_sys->p_logger = NULL;
    }
}


void Logger::error( const string &rMsg )
{
    msg_Err( getIntf(), rMsg.c_str() );
}


void Logger::warn( const string &rMsg )
{
    msg_Warn( getIntf(), rMsg.c_str() );
}

