/*****************************************************************************
 * pointer.hpp
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: pointer.hpp 6961 2004-03-05 17:34:23Z sam $
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

#ifndef POINTER_HPP
#define POINTER_HPP


/// Reference couting pointer
template <class T> class CountedPtr
{
    public:
        explicit CountedPtr( T *pPtr = 0 ): m_pCounter( 0 )
        {
            if( pPtr ) m_pCounter = new Counter( pPtr );
        }

        ~CountedPtr() { release(); }

        CountedPtr(const CountedPtr &rPtr ) { acquire( rPtr.m_pCounter ); }

        CountedPtr &operator=( const CountedPtr &rPtr )
        {
            if( this != &rPtr )
            {
                release();
                acquire( rPtr.m_pCounter );
            }
            return *this;
        }

        T &operator*() const { return *m_pCounter->m_pPtr; }

        T *operator->() const {return m_pCounter->m_pPtr; }

        T *get() const { return m_pCounter ? m_pCounter->m_pPtr : 0; }

        bool unique() const
        {
            return ( m_pCounter ? m_pCounter->m_count == 1 : true );
        }

    private:
        struct Counter
        {
            Counter( T* pPtr = 0, unsigned int c = 1 ):
                m_pPtr( pPtr ), m_count( c ) {}
            T* m_pPtr;
            unsigned int m_count;
        } *m_pCounter;

        void acquire( Counter* pCount )
        {
            m_pCounter = pCount;
            if( pCount ) ++pCount->m_count;
        }

        void release()
        {
            if( m_pCounter )
            {
                if( --m_pCounter->m_count == 0 )
                {
                    delete m_pCounter->m_pPtr;
                    delete m_pCounter;
                }
                m_pCounter = 0;
            }
        }
};


#endif
