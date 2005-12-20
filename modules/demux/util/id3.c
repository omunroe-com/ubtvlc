/*****************************************************************************
 * id3.c: simple id3 tag skipper
 *****************************************************************************
 * Copyright (C) 2001 VideoLAN
 * $Id: id3.c 6961 2004-03-05 17:34:23Z sam $
 *
 * Authors: Sigmund Augdal <sigmunau@idi.ntnu.no>
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

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <stdlib.h>                                      /* malloc(), free() */
#include <string.h>

#include <vlc/vlc.h>
#include <vlc/input.h>

#include "ninput.h"

#include <sys/types.h>

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int  SkipID3Tag ( vlc_object_t * );

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
vlc_module_begin();
    set_description( _("Simple id3 tag skipper" ) );
    set_capability( "id3", 50 );
    set_callbacks( SkipID3Tag, NULL );
vlc_module_end();

/****************************************************************************
 * SkipID3Tag : check if an ID3 tag is present, and skip it if it is
 ****************************************************************************/
static int SkipID3Tag( vlc_object_t *p_this )
{
    input_thread_t *p_input = NULL;
    uint8_t *p_peek;
    int i_size;
    uint8_t version, revision;
    int b_footer;

    if ( p_this->i_object_type == VLC_OBJECT_INPUT )
    {
        p_input = (input_thread_t *)p_this;
    }
    if( p_input == NULL )
    {
        p_input = vlc_object_find( p_this, VLC_OBJECT_INPUT, FIND_ANYWHERE );
        if( p_input == NULL )
        {
            return VLC_EGENERIC;
        }
    }

    msg_Dbg( p_input, "checking for ID3 tag" );

    /* get 10 byte id3 header */
    if( stream_Peek( p_input->s, &p_peek, 10 ) < 10 )
    {
        msg_Err( p_input, "cannot peek()" );
        vlc_object_release( p_input );
        return VLC_EGENERIC;
    }

    if( p_peek[0] != 'I' || p_peek[1] != 'D' || p_peek[2] != '3' )
    {
        vlc_object_release( p_input );
        return VLC_SUCCESS;
    }

    version = p_peek[3];  /* These may become usfull later, */
    revision = p_peek[4]; /* but we ignore them for now */
    b_footer = p_peek[5] & 0x10;
    i_size = (p_peek[6] << 21) +
             (p_peek[7] << 14) +
             (p_peek[8] << 7) +
             p_peek[9];
    if ( b_footer )
    {
        i_size += 10;
    }
    i_size += 10;

    /* Skip the entire tag */
    stream_Read( p_input->s, NULL, i_size );

    msg_Dbg( p_input, "ID3v2.%d revision %d tag found, skiping %d bytes",
             version, revision, i_size );

    vlc_object_release( p_input );
    return VLC_SUCCESS;
}
