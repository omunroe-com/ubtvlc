/*****************************************************************************
 * shout.c:  Shoutcast services discovery module
 *****************************************************************************
 * Copyright (C) 2005 the VideoLAN team
 * $Id: shout.c 15555 2006-05-06 12:50:08Z xtophe $
 *
 * Authors: Sigmund Augdal Helberg <dnumgis@videolan.org>
 *          Antoine Cellerier <dionoea -@T- videolan -d.t- org>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Includes
 *****************************************************************************/
#include <stdlib.h>                                      /* malloc(), free() */

#include <vlc/vlc.h>
#include <vlc/intf.h>
#include <vlc_interaction.h>

#include <vlc/input.h>

#include "network.h"

#include <errno.h>                                                 /* ENOMEM */

#ifdef HAVE_UNISTD_H
#    include <unistd.h>
#endif
#ifdef HAVE_SYS_TIME_H
#    include <sys/time.h>
#endif

/************************************************************************
 * Macros and definitions
 ************************************************************************/

#define MAX_LINE_LENGTH 256
#define SHOUTCAST_BASE_URL "http/shout-winamp://www.shoutcast.com/sbin/newxml.phtml"

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/

/* Callbacks */
    static int  Open ( vlc_object_t * );
    static void Close( vlc_object_t * );

vlc_module_begin();
    set_shortname( "Shoutcast");
    set_description( _("Shoutcast radio listings") );
    add_shortcut( "shoutcast" );
    set_category( CAT_PLAYLIST );
    set_subcategory( SUBCAT_PLAYLIST_SD );

    add_suppressed_integer( "shoutcast-limit" );

    set_capability( "services_discovery", 0 );
    set_callbacks( Open, Close );

vlc_module_end();


/*****************************************************************************
 * Local structures
 *****************************************************************************/

struct services_discovery_sys_t
{
    playlist_item_t *p_item;
    vlc_bool_t b_dialog;
};

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/

/* Main functions */
    static void Run    ( services_discovery_t *p_intf );

/*****************************************************************************
 * Open: initialize and create stuff
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    services_discovery_t *p_sd = ( services_discovery_t* )p_this;
    services_discovery_sys_t *p_sys  = malloc(
                                    sizeof( services_discovery_sys_t ) );

    vlc_value_t         val;
    playlist_t          *p_playlist;
    playlist_view_t     *p_view;
    playlist_item_t     *p_item;

    p_sd->pf_run = Run;
    p_sd->p_sys  = p_sys;

    /* Create our playlist node */
    p_playlist = (playlist_t *)vlc_object_find( p_sd, VLC_OBJECT_PLAYLIST,
                                                FIND_ANYWHERE );
    if( !p_playlist )
    {
        msg_Warn( p_sd, "unable to find playlist, cancelling");
        return VLC_EGENERIC;
    }

    p_view = playlist_ViewFind( p_playlist, VIEW_CATEGORY );

    p_sys->p_item =
    p_item = playlist_ItemNew( p_playlist, SHOUTCAST_BASE_URL, _("Shoutcast") );
    playlist_NodeAddItem( p_playlist, p_item, p_view->i_id,
                          p_view->p_root, PLAYLIST_APPEND,
                          PLAYLIST_END );

    p_sys->p_item->i_flags |= PLAYLIST_RO_FLAG;

    val.b_bool = VLC_TRUE;
    var_Set( p_playlist, "intf-change", val );

    vlc_object_release( p_playlist );

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close:
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    services_discovery_t *p_sd = ( services_discovery_t* )p_this;
    services_discovery_sys_t *p_sys  = p_sd->p_sys;
    playlist_t *p_playlist =  (playlist_t *) vlc_object_find( p_sd,
                                 VLC_OBJECT_PLAYLIST, FIND_ANYWHERE );
    if( p_playlist )
    {
        playlist_NodeDelete( p_playlist, p_sys->p_item, VLC_TRUE, VLC_TRUE );
        vlc_object_release( p_playlist );
    }
    free( p_sys );
}

/*****************************************************************************
 * Run: main thread
 *****************************************************************************/
static void Run( services_discovery_t *p_sd )
{
    services_discovery_sys_t *p_sys  = p_sd->p_sys;
    int i_id = input_Read( p_sd, &p_sys->p_item->input, VLC_FALSE );
    int i_dialog_id;

    i_dialog_id = intf_UserProgress( p_sd, "Shoutcast" , "Connecting...", 0.0 );

    p_sys->b_dialog = VLC_TRUE;
    while( !p_sd->b_die )
    {
        input_thread_t *p_input = (input_thread_t *)vlc_object_get( p_sd,
                                                                    i_id );

        /* The Shoutcast server does not return a content-length so we
         * can't know where we are. Use the number of inserted items
         * as a hint */
        if( p_input != NULL )
        {
            int i_state = var_GetInteger( p_input, "state" );
            if( i_state == PLAYING_S )
            {
                float f_pos = (float)(p_sys->p_item->i_children)* 2 *100.0 /
                              260 /* gruiiik FIXME */;
                intf_UserProgressUpdate( p_sd, i_dialog_id, "Downloading",
                                         f_pos );
            }
            vlc_object_release( p_input );
        }
        else if( p_sys->b_dialog )
        {
            p_sys->b_dialog  = VLC_FALSE;
            intf_UserHide( p_sd, i_dialog_id );
        }
        msleep( 10000 );
    }
}
