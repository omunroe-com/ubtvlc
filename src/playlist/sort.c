/*****************************************************************************
 * sort.c : Playlist sorting functions
 *****************************************************************************
 * Copyright (C) 1999-2004 VideoLAN
 * $Id: sort.c 8109 2004-07-01 12:37:53Z sigmunau $
 *
 * Authors: Cl�ment Stenac <zorglub@videolan.org>
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
#include <stdlib.h>                                      /* free(), strtol() */
#include <stdio.h>                                              /* sprintf() */
#include <string.h>                                            /* strerror() */

#include <vlc/vlc.h>
#include <vlc/input.h>
#include <vlc/vout.h>
#include <vlc/sout.h>

#include "vlc_playlist.h"

/**
 * Sort the playlist
 * \param p_playlist the playlist
 * \param i_mode: SORT_ID, SORT_TITLE, SORT_GROUP, SORT_AUTHOR, SORT_RANDOM
 * \param i_type: ORDER_NORMAL or ORDER_REVERSE (reversed order)
 * \return VLC_SUCCESS on success
 */
int playlist_Sort( playlist_t * p_playlist , int i_mode, int i_type )
{
    int i , i_small , i_position;
    playlist_item_t *p_temp;
    vlc_value_t val;
    val.b_bool = VLC_TRUE;

    vlc_mutex_lock( &p_playlist->object_lock );

    p_playlist->i_sort = i_mode;
    p_playlist->i_order = i_type;
    /* playlist with one or less items are allways sorted in all
       manners, quit fast. */
    if( p_playlist->i_size <= 1 )
    {
        vlc_mutex_unlock( &p_playlist->object_lock );

        /* Notify the interfaces, is this necessary? */
        var_Set( p_playlist, "intf-change", val );

        return VLC_SUCCESS;
    }

    if( i_mode == SORT_RANDOM )
    {
        for( i_position = 0; i_position < p_playlist->i_size ; i_position ++ )
        {
            int i_new  = rand() % (p_playlist->i_size - 1);

            /* Keep the correct current index */
            if( i_new == p_playlist->i_index )
                p_playlist->i_index = i_position;
            else if( i_position == p_playlist->i_index )
                p_playlist->i_index = i_new;

            p_temp = p_playlist->pp_items[i_position];
            p_playlist->pp_items[i_position] = p_playlist->pp_items[i_new];
            p_playlist->pp_items[i_new] = p_temp;
        }
        vlc_mutex_unlock( &p_playlist->object_lock );

        /* Notify the interfaces */
        var_Set( p_playlist, "intf-change", val );

        return VLC_SUCCESS;
    }

    for( i_position = 0; i_position < p_playlist->i_size -1 ; i_position ++ )
    {
        i_small  = i_position;
        for( i = i_position + 1 ; i<  p_playlist->i_size ; i++)
        {
            int i_test = 0;

            if( i_mode == SORT_ID )
            {
                i_test = p_playlist->pp_items[i]->i_id -
                                 p_playlist->pp_items[i_small]->i_id;
            }
            else if( i_mode == SORT_TITLE )
            {
                i_test = strcasecmp( p_playlist->pp_items[i]->input.psz_name,
                             p_playlist->pp_items[i_small]->input.psz_name );
            }
            else if( i_mode == SORT_GROUP )
            {
                i_test = p_playlist->pp_items[i]->i_group -
                             p_playlist->pp_items[i_small]->i_group;
            }
            else if( i_mode == SORT_DURATION )
            {
                i_test = p_playlist->pp_items[i]->input.i_duration -
                             p_playlist->pp_items[i_small]->input.i_duration;
            }
            else if( i_mode == SORT_AUTHOR )
            {
                 i_test = strcasecmp(
                          playlist_GetInfo( p_playlist, i,
                                            _("General") , _("Author") ),
                          playlist_GetInfo( p_playlist, i_small,
                                            _("General") , _("Author") ) );
            }

            if( ( i_type == ORDER_NORMAL  && i_test < 0 ) ||
                ( i_type == ORDER_REVERSE && i_test > 0 ) )
            {
                i_small = i;
            }
        }
        /* Keep the correct current index */
        if( i_small == p_playlist->i_index )
            p_playlist->i_index = i_position;
        else if( i_position == p_playlist->i_index )
            p_playlist->i_index = i_small;

        p_temp = p_playlist->pp_items[i_position];
        p_playlist->pp_items[i_position] = p_playlist->pp_items[i_small];
        p_playlist->pp_items[i_small] = p_temp;
    }
    vlc_mutex_unlock( &p_playlist->object_lock );

    /* Notify the interfaces  */
    var_Set( p_playlist, "intf-change", val );

    return VLC_SUCCESS;
}
