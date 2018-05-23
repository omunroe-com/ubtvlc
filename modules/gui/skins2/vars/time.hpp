/*****************************************************************************
 * time.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: time.hpp 7574 2004-05-01 14:23:40Z asmax $
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

#ifndef TIME_HPP
#define TIME_HPP

#include "../utils/var_percent.hpp"
#include <string>

/// Variable for VLC strem time
class StreamTime: public VarPercent
{
    public:
        StreamTime( intf_thread_t *pIntf ): VarPercent( pIntf ) {}
        virtual ~StreamTime() {}

        virtual void set( float percentage, bool updateVLC );

        virtual void set( float percentage ) { set( percentage, true ); }

        /// Return a string containing a value from 0 to 100
        virtual const string getAsStringPercent() const;
        /// Return the current time formatted as a time display (h:mm:ss)
        virtual const string getAsStringCurrTime() const;
        /// Return the time left formatted as a time display (h:mm:ss)
        virtual const string getAsStringTimeLeft() const;
        /// Return the duration formatted as a time display (h:mm:ss)
        virtual const string getAsStringDuration() const;

    private:
        /// Convert a number of seconds into "h:mm:ss" format
        const string formatTime( int seconds ) const;
};

#endif
