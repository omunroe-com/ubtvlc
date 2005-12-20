/*****************************************************************************
 * async_queue.cpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: async_queue.cpp 8562 2004-08-29 09:00:03Z asmax $
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

#include "async_queue.hpp"
#include "../src/os_factory.hpp"
#include "../src/os_timer.hpp"


AsyncQueue::AsyncQueue( intf_thread_t *pIntf ): SkinObject( pIntf )
{
    // Initialize the mutex
    vlc_mutex_init( pIntf, &m_lock );

    // Create a timer
    OSFactory *pOsFactory = OSFactory::instance( pIntf );
    m_pTimer = pOsFactory->createOSTimer( Callback( this, &doFlush ) );

    // Flush the queue every 10 ms
    m_pTimer->start( 10, false );
}


AsyncQueue::~AsyncQueue()
{
    delete( m_pTimer );
    vlc_mutex_destroy( &m_lock );
}


AsyncQueue *AsyncQueue::instance( intf_thread_t *pIntf )
{
    if( ! pIntf->p_sys->p_queue )
    {
        AsyncQueue *pQueue;
        pQueue = new AsyncQueue( pIntf );
        if( pQueue )
        {
             // Initialization succeeded
             pIntf->p_sys->p_queue = pQueue;
        }
     }
     return pIntf->p_sys->p_queue;
}


void AsyncQueue::destroy( intf_thread_t *pIntf )
{
    if( pIntf->p_sys->p_queue )
    {
        delete pIntf->p_sys->p_queue;
        pIntf->p_sys->p_queue = NULL;
    }
}


void AsyncQueue::push( const CmdGenericPtr &rcCommand )
{
    m_cmdList.push_back( rcCommand );
}


void AsyncQueue::remove( const string &rType )
{
    vlc_mutex_lock( &m_lock );

    list<CmdGenericPtr>::iterator it;
    for( it = m_cmdList.begin(); it != m_cmdList.end(); it++ )
    {
        // Remove the command if it is of the given type
        if( (*it).get()->getType() == rType )
        {
            list<CmdGenericPtr>::iterator itNew = it;
            itNew++;
            m_cmdList.erase( it );
            it = itNew;
        }
    }

    vlc_mutex_unlock( &m_lock );
}


void AsyncQueue::flush()
{
    vlc_mutex_lock( &m_lock );

    while( m_cmdList.size() > 0 )
    {
        // Pop the first command from the queue
        CmdGenericPtr cCommand = m_cmdList.front();
        m_cmdList.pop_front();
        // And execute it
        cCommand.get()->execute();
    }

    vlc_mutex_unlock( &m_lock );
}


void AsyncQueue::doFlush( SkinObject *pObj )
{
    AsyncQueue *pThis = (AsyncQueue*)pObj;
    // Flush the queue
    pThis->flush();
}
