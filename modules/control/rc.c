/*****************************************************************************
 * rc.c : remote control stdin/stdout module for vlc
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 * $Id: rc.c 9272 2004-11-10 14:49:52Z fkuehne $
 *
 * Author: Peter Surda <shurdeek@panorama.sth.ac.at>
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

#include <errno.h>                                                 /* ENOMEM */
#include <stdio.h>
#include <ctype.h>
#include <signal.h>

#include <vlc/vlc.h>
#include <vlc/intf.h>
#include <vlc/aout.h>
#include <vlc/vout.h>

#ifdef HAVE_UNISTD_H
#    include <unistd.h>
#endif

#ifdef HAVE_SYS_TIME_H
#    include <sys/time.h>
#endif
#include <sys/types.h>

#include "vlc_error.h"
#include "network.h"

#if defined(PF_UNIX) && !defined(PF_LOCAL)
#    define PF_LOCAL PF_UNIX
#endif
#if defined(AF_UNIX) && !defined(AF_LOCAL)
#    define AF_LOCAL AF_UNIX
#endif

#ifdef PF_LOCAL
#    include <sys/un.h>
#endif

#define MAX_LINE_LENGTH 256

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int  Activate     ( vlc_object_t * );
static void Deactivate   ( vlc_object_t * );
static void Run          ( intf_thread_t * );

static vlc_bool_t ReadCommand( intf_thread_t *, char *, int * );

static playlist_item_t *parse_MRL( intf_thread_t *, char * );

static int  Input        ( vlc_object_t *, char const *,
                           vlc_value_t, vlc_value_t, void * );
static int  Playlist     ( vlc_object_t *, char const *,
                           vlc_value_t, vlc_value_t, void * );
static int  Other        ( vlc_object_t *, char const *,
                           vlc_value_t, vlc_value_t, void * );
static int  Quit         ( vlc_object_t *, char const *,
                           vlc_value_t, vlc_value_t, void * );
static int  Intf         ( vlc_object_t *, char const *,
                           vlc_value_t, vlc_value_t, void * );
static int  Volume       ( vlc_object_t *, char const *,
                           vlc_value_t, vlc_value_t, void * );
static int  VolumeMove   ( vlc_object_t *, char const *,
                           vlc_value_t, vlc_value_t, void * );
static int  AudioConfig  ( vlc_object_t *, char const *,
                           vlc_value_t, vlc_value_t, void * );

struct intf_sys_t
{
    int i_socket_listen;
    int i_socket;
    char *psz_unix_path;
    vlc_bool_t b_extend;
    
#ifdef WIN32
    HANDLE hConsoleIn;
    vlc_bool_t b_quiet;
#endif
};


#ifdef HAVE_VARIADIC_MACROS
#   define msg_rc( psz_format, args... ) \
      __msg_rc( p_intf, psz_format, ## args )
#endif


static void __msg_rc( intf_thread_t *p_intf, const char *psz_fmt, ... )
{
    va_list args;
    va_start( args, psz_fmt );
    if( p_intf->p_sys->i_socket == -1 ) vprintf( psz_fmt, args );
    else
    { net_vaPrintf( p_intf, p_intf->p_sys->i_socket, psz_fmt, args );
      net_Printf( VLC_OBJECT(p_intf), p_intf->p_sys->i_socket, "\r" ); }
    va_end( args );
}

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define POS_TEXT N_("Show stream position")
#define POS_LONGTEXT N_("Show the current position in seconds within the " \
                        "stream from time to time." )

#define TTY_TEXT N_("Fake TTY")
#define TTY_LONGTEXT N_("Force the rc module to use stdin as if it was a TTY.")

#define UNIX_TEXT N_("UNIX socket command input")
#define UNIX_LONGTEXT N_("Accept commands over a Unix socket rather than " \
                         "stdin." )

#define HOST_TEXT N_("TCP command input")
#define HOST_LONGTEXT N_("Accept commands over a socket rather than stdin. " \
            "You can set the address and port the interface will bind to." )
#define EXTEND_TEXT N_("Extended help")
#define EXTEND_LONGTEXT N_("List additional commands.")
            

#ifdef WIN32
#define QUIET_TEXT N_("Do not open a DOS command box interface")
#define QUIET_LONGTEXT N_( \
    "By default the rc interface plugin will start a DOS command box. " \
    "Enabling the quiet mode will not bring this command box but can also " \
    "be pretty annoying when you want to stop VLC and no video window is " \
    "open." )
#endif

vlc_module_begin();
    set_description( _("Remote control interface") );
    add_bool( "rc-show-pos", 0, NULL, POS_TEXT, POS_LONGTEXT, VLC_TRUE );
#ifdef HAVE_ISATTY
    add_bool( "rc-fake-tty", 0, NULL, TTY_TEXT, TTY_LONGTEXT, VLC_TRUE );
#endif
    add_string( "rc-unix", 0, NULL, UNIX_TEXT, UNIX_LONGTEXT, VLC_TRUE );
    add_string( "rc-host", 0, NULL, HOST_TEXT, HOST_LONGTEXT, VLC_TRUE );

#ifdef WIN32
    add_bool( "rc-quiet", 0, NULL, QUIET_TEXT, QUIET_LONGTEXT, VLC_FALSE );
#endif
    add_bool( "rc-extend", 0, NULL, EXTEND_TEXT, EXTEND_LONGTEXT, VLC_FALSE );

    set_capability( "interface", 20 );
    set_callbacks( Activate, Deactivate );
vlc_module_end();

/*****************************************************************************
 * Activate: initialize and create stuff
 *****************************************************************************/
static int Activate( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t*)p_this;
    char *psz_host, *psz_unix_path;
    int i_socket = -1;

#if defined(HAVE_ISATTY) && !defined(WIN32)
    /* Check that stdin is a TTY */
    if( !config_GetInt( p_intf, "rc-fake-tty" ) && !isatty( 0 ) )
    {
        msg_Warn( p_intf, "fd 0 is not a TTY" );
        return VLC_EGENERIC;
    }
#endif

    psz_unix_path = config_GetPsz( p_intf, "rc-unix" );
    if( psz_unix_path )
    {
#ifndef PF_LOCAL
        msg_Warn( p_intf, "your OS doesn't support filesystem sockets" );
        free( psz_unix_path );
        return VLC_EGENERIC;
#else
        struct sockaddr_un addr;
        int i_ret;

        memset( &addr, 0, sizeof(struct sockaddr_un) );

        msg_Dbg( p_intf, "trying UNIX socket" );

        if( (i_socket = socket( PF_LOCAL, SOCK_STREAM, 0 ) ) < 0 )
        {
            msg_Warn( p_intf, "can't open socket: %s", strerror(errno) );
            free( psz_unix_path );
            return VLC_EGENERIC;
        }

        addr.sun_family = AF_LOCAL;
        strncpy( addr.sun_path, psz_unix_path, sizeof( addr.sun_path ) );
        addr.sun_path[sizeof( addr.sun_path ) - 1] = '\0';

        if( (i_ret = bind( i_socket, (struct sockaddr*)&addr,
                           sizeof(struct sockaddr_un) ) ) < 0 )
        {
            msg_Warn( p_intf, "couldn't bind socket to address: %s",
                      strerror(errno) );
            free( psz_unix_path );
            net_Close( i_socket );
            return VLC_EGENERIC;
        }

        if( ( i_ret = listen( i_socket, 1 ) ) < 0 )
        {
            msg_Warn( p_intf, "can't listen on socket: %s", strerror(errno));
            free( psz_unix_path );
            net_Close( i_socket );
            return VLC_EGENERIC;
        }
#endif
    }

    if( ( i_socket == -1) &&
        ( psz_host = config_GetPsz( p_intf, "rc-host" ) ) != NULL )
    {
        vlc_url_t url;

        vlc_UrlParse( &url, psz_host, 0 );

        msg_Dbg( p_intf, "base %s port %d", url.psz_host, url.i_port );

        if( (i_socket = net_ListenTCP(p_this, url.psz_host, url.i_port)) == -1)
        {
            msg_Warn( p_intf, "can't listen to %s port %i",
                      url.psz_host, url.i_port );
            vlc_UrlClean( &url );
            free( psz_host );
            return VLC_EGENERIC;
        }

        vlc_UrlClean( &url );
        free( psz_host );
    }

    p_intf->p_sys = malloc( sizeof( intf_sys_t ) );
    if( !p_intf->p_sys )
    {
        msg_Err( p_intf, "no memory" );
        return VLC_ENOMEM;
    }

    p_intf->p_sys->i_socket_listen = i_socket;
    p_intf->p_sys->i_socket = -1;
    p_intf->p_sys->psz_unix_path = psz_unix_path;

    /* Non-buffered stdout */
    setvbuf( stdout, (char *)NULL, _IOLBF, 0 );

    p_intf->pf_run = Run;

#ifdef WIN32
    p_intf->p_sys->b_quiet = config_GetInt( p_intf, "rc-quiet" );
    if( !p_intf->p_sys->b_quiet ) { CONSOLE_INTRO_MSG; }
#else
    CONSOLE_INTRO_MSG;
#endif

    msg_rc( _("Remote control interface initialized, `h' for help\n") );
    return VLC_SUCCESS;
}

/*****************************************************************************
 * Deactivate: uninitialize and cleanup
 *****************************************************************************/
static void Deactivate( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t*)p_this;

    if( p_intf->p_sys->i_socket_listen != -1 )
        net_Close( p_intf->p_sys->i_socket_listen );
    if( p_intf->p_sys->i_socket != -1 )
        net_Close( p_intf->p_sys->i_socket );
    if( p_intf->p_sys->psz_unix_path != NULL )
    {
#ifdef PF_LOCAL
        unlink( p_intf->p_sys->psz_unix_path );
#endif
        free( p_intf->p_sys->psz_unix_path );
    }
    free( p_intf->p_sys );
}

/*****************************************************************************
 * Run: rc thread
 *****************************************************************************
 * This part of the interface is in a separate thread so that we can call
 * exec() from within it without annoying the rest of the program.
 *****************************************************************************/
static void Run( intf_thread_t *p_intf )
{
    input_thread_t * p_input;
    playlist_t *     p_playlist;

    char       p_buffer[ MAX_LINE_LENGTH + 1 ];
    vlc_bool_t b_showpos = config_GetInt( p_intf, "rc-show-pos" );

    int        i_size = 0;
    int        i_oldpos = 0;
    int        i_newpos;

    p_buffer[0] = 0;
    p_input = NULL;
    p_playlist = NULL;
 
    p_intf->p_sys->b_extend = config_GetInt( p_intf, "rc-extend" );
    /* Register commands that will be cleaned up upon object destruction */
    var_Create( p_intf, "quit", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "quit", Quit, NULL );
    var_Create( p_intf, "intf", VLC_VAR_STRING | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "intf", Intf, NULL );

    var_Create( p_intf, "add", VLC_VAR_STRING | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "add", Playlist, NULL );
    var_Create( p_intf, "playlist", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "playlist", Playlist, NULL );
    var_Create( p_intf, "play", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "play", Playlist, NULL );
    var_Create( p_intf, "stop", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "stop", Playlist, NULL );
    var_Create( p_intf, "prev", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "prev", Playlist, NULL );
    var_Create( p_intf, "next", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "next", Playlist, NULL );
  
    var_Create( p_intf, "marq-marquee", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "marq-marquee", Other, NULL );
    var_Create( p_intf, "marq-x", VLC_VAR_INTEGER | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "marq-x", Other, NULL );
    var_Create( p_intf, "marq-y", VLC_VAR_INTEGER | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "marq-y", Other, NULL );
    var_Create( p_intf, "marq-timeout", VLC_VAR_INTEGER | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "marq-timeout", Other, NULL );

    var_Create( p_intf, "pause", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "pause", Input, NULL );
    var_Create( p_intf, "seek", VLC_VAR_INTEGER | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "seek", Input, NULL );
    var_Create( p_intf, "title", VLC_VAR_STRING | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "title", Input, NULL );
    var_Create( p_intf, "title_n", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "title_n", Input, NULL );
    var_Create( p_intf, "title_p", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "title_p", Input, NULL );
    var_Create( p_intf, "chapter", VLC_VAR_STRING | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "chapter", Input, NULL );
    var_Create( p_intf, "chapter_n", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "chapter_n", Input, NULL );
    var_Create( p_intf, "chapter_p", VLC_VAR_VOID | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "chapter_p", Input, NULL );

    var_Create( p_intf, "volume", VLC_VAR_STRING | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "volume", Volume, NULL );
    var_Create( p_intf, "volup", VLC_VAR_STRING | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "volup", VolumeMove, NULL );
    var_Create( p_intf, "voldown", VLC_VAR_STRING | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "voldown", VolumeMove, NULL );
    var_Create( p_intf, "adev", VLC_VAR_STRING | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "adev", AudioConfig, NULL );
    var_Create( p_intf, "achan", VLC_VAR_STRING | VLC_VAR_ISCOMMAND );
    var_AddCallback( p_intf, "achan", AudioConfig, NULL );

#ifdef WIN32
    /* Get the file descriptor of the console input */
    p_intf->p_sys->hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
    if( p_intf->p_sys->hConsoleIn == INVALID_HANDLE_VALUE )
    {
        msg_Err( p_intf, "Couldn't open STD_INPUT_HANDLE" );
        p_intf->b_die = VLC_TRUE;
    }
#endif

    while( !p_intf->b_die )
    {
        char *psz_cmd, *psz_arg;
        vlc_bool_t b_complete;

        if( p_intf->p_sys->i_socket_listen != - 1 &&
            p_intf->p_sys->i_socket == -1 )
        {
            p_intf->p_sys->i_socket =
                net_Accept( p_intf, p_intf->p_sys->i_socket_listen, 0 );
        }

        b_complete = ReadCommand( p_intf, p_buffer, &i_size );

        /* Manage the input part */
        if( p_input == NULL )
        {
            if( p_playlist )
            {
                p_input = vlc_object_find( p_playlist, VLC_OBJECT_INPUT,
                                                       FIND_CHILD );
            }
            else
            {
                p_input = vlc_object_find( p_intf, VLC_OBJECT_INPUT,
                                                   FIND_ANYWHERE );
                if( p_input )
                {
                    p_playlist = vlc_object_find( p_input, VLC_OBJECT_PLAYLIST,
                                                           FIND_PARENT );
                }
            }
        }
        else if( p_input->b_dead )
        {
            vlc_object_release( p_input );
            p_input = NULL;
        }

        if( p_input && b_showpos )
        {
            i_newpos = 100 * var_GetFloat( p_input, "position" );
            if( i_oldpos != i_newpos )
            {
                i_oldpos = i_newpos;
                msg_rc( "pos: %d%%\n", i_newpos );
            }
        }

        /* Is there something to do? */
        if( !b_complete ) continue;


        /* Skip heading spaces */
        psz_cmd = p_buffer;
        while( *psz_cmd == ' ' )
        {
            psz_cmd++;
        }

        /* Split psz_cmd at the first space and make sure that
         * psz_arg is valid */
        psz_arg = strchr( psz_cmd, ' ' );
        if( psz_arg )
        {
            *psz_arg++ = 0;
            while( *psz_arg == ' ' )
            {
                psz_arg++;
            }
        }
        else
        {
            psz_arg = "";
        }

        /* If the user typed a registered local command, try it */
        if( var_Type( p_intf, psz_cmd ) & VLC_VAR_ISCOMMAND )
        {
            vlc_value_t val;
            int i_ret;

            val.psz_string = psz_arg;
            i_ret = var_Set( p_intf, psz_cmd, val );
            msg_rc( "%s: returned %i (%s)\n",
                    psz_cmd, i_ret, vlc_error( i_ret ) );
        }
        /* Or maybe it's a global command */
        else if( var_Type( p_intf->p_libvlc, psz_cmd ) & VLC_VAR_ISCOMMAND )
        {
            vlc_value_t val;
            int i_ret;

            val.psz_string = psz_arg;
            /* FIXME: it's a global command, but we should pass the
             * local object as an argument, not p_intf->p_libvlc. */
            i_ret = var_Set( p_intf->p_libvlc, psz_cmd, val );
            if( i_ret != 0 )
            {
                msg_rc( "%s: returned %i (%s)\n",
                         psz_cmd, i_ret, vlc_error( i_ret ) );
            }
        }
        else if( !strcmp( psz_cmd, "logout" ) )
        {
            /* Close connection */
            if( p_intf->p_sys->i_socket != -1 )
            {
                net_Close( p_intf->p_sys->i_socket );
            }
            p_intf->p_sys->i_socket = -1;
        }
        else if( !strcmp( psz_cmd, "info" ) )
        {
            if( p_input )
            {
                int i, j;
                vlc_mutex_lock( &p_input->input.p_item->lock );
                for ( i = 0; i < p_input->input.p_item->i_categories; i++ )
                {
                    info_category_t *p_category =
                        p_input->input.p_item->pp_categories[i];

                    msg_rc( "+----[ %s ]\n", p_category->psz_name );
                    msg_rc( "| \n" );
                    for ( j = 0; j < p_category->i_infos; j++ )
                    {
                        info_t *p_info = p_category->pp_infos[j];
                        msg_rc( "| %s: %s\n", p_info->psz_name,
                                p_info->psz_value );
                    }
                    msg_rc( "| \n" );
                }
                msg_rc( "+----[ end of stream info ]\n" );
                vlc_mutex_unlock( &p_input->input.p_item->lock );
            }
            else
            {
                msg_rc( "no input\n" );
            }
        }
        else if( !strcmp( psz_cmd, "is_playing" ) )
        {
            if( ! p_input )
            {
                msg_rc( "0\n" );
            }
            else
            {
                msg_rc( "1\n" );
            }
        }
        else if( !strcmp( psz_cmd, "get_time" ) )
        {
            if( ! p_input )
            {
                msg_rc("0\n");
            }
            else
            {
                vlc_value_t time;
                var_Get( p_input, "time", &time );
                msg_rc( "%i\n", time.i_time / 1000000);
            }
        }
        else if( !strcmp( psz_cmd, "get_length" ) )
        {
            if( ! p_input )
            {
                msg_rc("0\n");
            }
            else
            {
                vlc_value_t time;
                var_Get( p_input, "length", &time );
                msg_rc( "%i\n", time.i_time / 1000000);
            }
        }
        else if( !strcmp( psz_cmd, "get_title" ) )
        {
            if( ! p_input )
            {
                msg_rc("\n");
            }
            else
            {
                msg_rc( "%s\n", p_input->input.p_item->psz_name );
            }
        }
        else switch( psz_cmd[0] )
        {
        case 'f':
        case 'F':
            if( p_input )
            {
                vout_thread_t *p_vout;
                p_vout = vlc_object_find( p_input,
                                          VLC_OBJECT_VOUT, FIND_CHILD );

                if( p_vout )
                {
                    p_vout->i_changes |= VOUT_FULLSCREEN_CHANGE;
                    vlc_object_release( p_vout );
                }
            }
            break;

        case 's':
        case 'S':
            ;
            break;

        case '?':
        case 'h':
        case 'H':
            msg_rc(_("+----[ Remote control commands ]\n"));
            msg_rc("| \n");
            msg_rc(_("| add XYZ  . . . . . . . . . . add XYZ to playlist\n"));
            msg_rc(_("| playlist . . .  show items currently in playlist\n"));
            msg_rc(_("| play . . . . . . . . . . . . . . . . play stream\n"));
            msg_rc(_("| stop . . . . . . . . . . . . . . . . stop stream\n"));
            msg_rc(_("| next . . . . . . . . . . . .  next playlist item\n"));
            msg_rc(_("| prev . . . . . . . . . .  previous playlist item\n"));
            msg_rc(_("| title [X]  . . . . set/get title in current item\n"));
            msg_rc(_("| title_n  . . . . . .  next title in current item\n"));
            msg_rc(_("| title_p  . . . .  previous title in current item\n"));
            msg_rc(_("| chapter [X]  . . set/get chapter in current item\n"));
            msg_rc(_("| chapter_n  . . . .  next chapter in current item\n"));
            msg_rc(_("| chapter_p  . .  previous chapter in current item\n"));
            msg_rc("| \n");
            msg_rc(_("| seek X . seek in seconds, for instance `seek 12'\n"));
            msg_rc(_("| pause  . . . . . . . . . . . . . .  toggle pause\n"));
            msg_rc(_("| f  . . . . . . . . . . . . . . toggle fullscreen\n"));
            msg_rc(_("| info . . .  information about the current stream\n"));
            msg_rc("| \n");
            msg_rc(_("| volume [X] . . . . . . . .  set/get audio volume\n"));
            msg_rc(_("| volup [X]  . . . . .  raise audio volume X steps\n"));
            msg_rc(_("| voldown [X]  . . . .  lower audio volume X steps\n"));
            msg_rc(_("| adev [X] . . . . . . . . .  set/get audio device\n"));
            msg_rc(_("| achan [X]. . . . . . . .  set/get audio channels\n"));
            msg_rc("| \n");
            if (p_intf->p_sys->b_extend)
            {
                   msg_rc(_("| marq-marquee STRING  . . overlay STRING in video\n"));
               msg_rc(_("| marq-x X . . . . . .offset of marquee, from left\n"));
               msg_rc(_("| marq-y Y . . . . . . offset of marquee, from top\n"));
               msg_rc(_("| marq-timeout T. . . . .timeout of marquee, in ms\n"));
               msg_rc("| \n");
            }    
            msg_rc(_("| help . . . . . . . . . . . . . this help message\n"));
            msg_rc(_("| logout . . . . . .exit (if in socket connection)\n"));
            msg_rc(_("| quit . . . . . . . . . . . . . . . . .  quit vlc\n"));
            msg_rc("| \n");
            msg_rc(_("+----[ end of help ]\n"));
            break;

        case '\0':
            /* Ignore empty lines */
            break;

        default:
            msg_rc(_("unknown command `%s', type `help' for help\n"), psz_cmd);
            break;
        }

        /* Command processed */
        i_size = 0; p_buffer[0] = 0;
    }

    if( p_input )
    {
        vlc_object_release( p_input );
        p_input = NULL;
    }

    if( p_playlist )
    {
        vlc_object_release( p_playlist );
        p_playlist = NULL;
    }
}

static int Input( vlc_object_t *p_this, char const *psz_cmd,
                  vlc_value_t oldval, vlc_value_t newval, void *p_data )
{
    intf_thread_t *p_intf = (intf_thread_t*)p_this;
    input_thread_t *p_input;
    vlc_value_t     val;

    p_input = vlc_object_find( p_this, VLC_OBJECT_INPUT, FIND_ANYWHERE );
    if( !p_input ) return VLC_ENOOBJ;

    /* Parse commands that only require an input */
    if( !strcmp( psz_cmd, "pause" ) )
    {
        val.i_int = PAUSE_S;

        var_Set( p_input, "state", val );
        vlc_object_release( p_input );
        return VLC_SUCCESS;
    }
    else if( !strcmp( psz_cmd, "seek" ) )
    {
        if( strlen( newval.psz_string ) > 0 &&
            newval.psz_string[strlen( newval.psz_string ) - 1] == '%' )
        {
            val.f_float = (float)atoi( newval.psz_string ) / 100.0;
            var_Set( p_input, "position", val );
        }
        else
        {
            val.i_time = ((int64_t)atoi( newval.psz_string )) * 1000000;
            var_Set( p_input, "time", val );
        }
        vlc_object_release( p_input );
        return VLC_SUCCESS;
    }
   
    else if( !strcmp( psz_cmd, "chapter" ) ||
             !strcmp( psz_cmd, "chapter_n" ) ||
             !strcmp( psz_cmd, "chapter_p" ) )
    {
        if( !strcmp( psz_cmd, "chapter" ) )
        {
            if ( *newval.psz_string )
            {
                /* Set. */
                val.i_int = atoi( newval.psz_string );
                var_Set( p_input, "chapter", val );
            }
            else
            {
                vlc_value_t val_list;

                /* Get. */
                var_Get( p_input, "chapter", &val );
                var_Change( p_input, "chapter", VLC_VAR_GETCHOICES,
                            &val_list, NULL );
                msg_rc( "Currently playing chapter %d/%d\n",
                        val.i_int, val_list.p_list->i_count );
                var_Change( p_this, "chapter", VLC_VAR_FREELIST,
                            &val_list, NULL );
            }
        }
        else if( !strcmp( psz_cmd, "chapter_n" ) )
        {
            val.b_bool = VLC_TRUE;
            var_Set( p_input, "next-chapter", val );
        }
        else if( !strcmp( psz_cmd, "chapter_p" ) )
        {
            val.b_bool = VLC_TRUE;
            var_Set( p_input, "prev-chapter", val );
        }

        vlc_object_release( p_input );
        return VLC_SUCCESS;
    }
    else if( !strcmp( psz_cmd, "title" ) ||
             !strcmp( psz_cmd, "title_n" ) ||
             !strcmp( psz_cmd, "title_p" ) )
    {
        if( !strcmp( psz_cmd, "title" ) )
        {
            if ( *newval.psz_string )
            {
                /* Set. */
                val.i_int = atoi( newval.psz_string );
                var_Set( p_input, "title", val );
            }
            else
            {
                vlc_value_t val_list;

                /* Get. */
                var_Get( p_input, "title", &val );
                var_Change( p_input, "title", VLC_VAR_GETCHOICES,
                            &val_list, NULL );
                msg_rc( "Currently playing title %d/%d\n",
                        val.i_int, val_list.p_list->i_count );
                var_Change( p_this, "title", VLC_VAR_FREELIST,
                            &val_list, NULL );
            }
        }
        else if( !strcmp( psz_cmd, "title_n" ) )
        {
            val.b_bool = VLC_TRUE;
            var_Set( p_input, "next-title", val );
        }
        else if( !strcmp( psz_cmd, "title_p" ) )
        {
            val.b_bool = VLC_TRUE;
            var_Set( p_input, "prev-title", val );
        }

        vlc_object_release( p_input );
        return VLC_SUCCESS;
    }

    /* Never reached. */
    return VLC_EGENERIC;
}

static int Playlist( vlc_object_t *p_this, char const *psz_cmd,
                     vlc_value_t oldval, vlc_value_t newval, void *p_data )
{
    intf_thread_t *p_intf = (intf_thread_t*)p_this;
    playlist_t *p_playlist;

    p_playlist = vlc_object_find( p_this, VLC_OBJECT_PLAYLIST,
                                           FIND_ANYWHERE );
    if( !p_playlist )
    {
        return VLC_ENOOBJ;
    }

    /* Parse commands that require a playlist */
    if( !strcmp( psz_cmd, "prev" ) )
    {
        playlist_Prev( p_playlist );
    }
    else if( !strcmp( psz_cmd, "next" ) )
    {
        playlist_Next( p_playlist );
    }
    else if( !strcmp( psz_cmd, "play" ) )
    {
        playlist_Play( p_playlist );
    }
    else if( !strcmp( psz_cmd, "stop" ) )
    {
        playlist_Stop( p_playlist );
    }
    else if( !strcmp( psz_cmd, "add" ) &&
             newval.psz_string && *newval.psz_string )
    {
        playlist_item_t *p_item = parse_MRL( p_intf, newval.psz_string );

        if( p_item )
        {
            msg_rc( "trying to add %s to playlist\n", newval.psz_string );
            playlist_AddItem( p_playlist, p_item,
                              PLAYLIST_GO|PLAYLIST_APPEND, PLAYLIST_END );
        }
    }
    else if( !strcmp( psz_cmd, "playlist" ) )
    {
        int i;
        for ( i = 0; i < p_playlist->i_size; i++ )
        {
            msg_rc( "|%s%s   %s|\n", i == p_playlist->i_index?"*":" ",
                    p_playlist->pp_items[i]->input.psz_name,
                    p_playlist->pp_items[i]->input.psz_uri );
        }
        if ( i == 0 )
        {
            msg_rc( "| no entries\n" );
        }
    }
 
    /*
     * sanity check
     */
    else
    {
        msg_rc( "unknown command!\n" );
    }

    vlc_object_release( p_playlist );
    return VLC_SUCCESS;
}

static int Other( vlc_object_t *p_this, char const *psz_cmd,
                     vlc_value_t oldval, vlc_value_t newval, void *p_data )
{
    intf_thread_t *p_intf = (intf_thread_t*)p_this;
    vlc_object_t *p_pl;
    vlc_value_t     val;

    p_pl = vlc_object_find( p_this, VLC_OBJECT_PLAYLIST,
                                           FIND_ANYWHERE );
    if( !p_pl )
    {
        return VLC_ENOOBJ;
    }

    /* Parse miscellaneous commands */
    if( !strcmp( psz_cmd, "marq-marquee" ) )
    {
        if( strlen( newval.psz_string ) > 0 )
        {
            val.psz_string = newval.psz_string;
            var_Set( p_pl, "marq-marquee", val );
        }
        else 
        {
                val.psz_string = "";
                var_Set( p_pl, "marq-marquee", val);
        }
    }
    else if( !strcmp( psz_cmd, "marq-x" ) )
    {
        if( strlen( newval.psz_string ) > 0) 
        {
            val.i_int = atoi( newval.psz_string );
            var_Set( p_pl, "marq-x", val );
        }
    }
    else if( !strcmp( psz_cmd, "marq-y" ) )
    {
        if( strlen( newval.psz_string ) > 0) 
        {
            val.i_int = atoi( newval.psz_string );
            var_Set( p_pl, "marq-y", val );
        }
    }
    else if( !strcmp( psz_cmd, "marq-timeout" ) )
    {
        if( strlen( newval.psz_string ) > 0) 
        {
            val.i_int = atoi( newval.psz_string );
            var_Set( p_pl, "marq-timeout", val );
        }
    }
 
    /*
     * sanity check
     */
    else
    {
        msg_rc( "unknown command!\n" );
    }

    vlc_object_release( p_pl );
    return VLC_SUCCESS;
}

static int Quit( vlc_object_t *p_this, char const *psz_cmd,
                 vlc_value_t oldval, vlc_value_t newval, void *p_data )
{
    p_this->p_vlc->b_die = VLC_TRUE;
    return VLC_SUCCESS;
}

static int Intf( vlc_object_t *p_this, char const *psz_cmd,
                 vlc_value_t oldval, vlc_value_t newval, void *p_data )
{
    intf_thread_t *p_newintf;

    p_newintf = intf_Create( p_this->p_vlc, newval.psz_string );

    if( p_newintf )
    {
        p_newintf->b_block = VLC_FALSE;
        if( intf_RunThread( p_newintf ) )
        {
            vlc_object_detach( p_newintf );
            intf_Destroy( p_newintf );
        }
    }

    return VLC_SUCCESS;
}

static int Volume( vlc_object_t *p_this, char const *psz_cmd,
                   vlc_value_t oldval, vlc_value_t newval, void *p_data )
{
    intf_thread_t *p_intf = (intf_thread_t*)p_this;
    int i_error;

    if ( *newval.psz_string )
    {
        /* Set. */
        audio_volume_t i_volume = atoi( newval.psz_string );
        if ( i_volume > AOUT_VOLUME_MAX )
        {
            msg_rc( "Volume must be in the range %d-%d\n", AOUT_VOLUME_MIN,
                    AOUT_VOLUME_MAX );
            i_error = VLC_EBADVAR;
        }
        else i_error = aout_VolumeSet( p_this, i_volume );
    }
    else
    {
        /* Get. */
        audio_volume_t i_volume;
        if ( aout_VolumeGet( p_this, &i_volume ) < 0 )
        {
            i_error = VLC_EGENERIC;
        }
        else
        {
            msg_rc( "Volume is %d\n", i_volume );
            i_error = VLC_SUCCESS;
        }
    }

    return i_error;
}

static int VolumeMove( vlc_object_t *p_this, char const *psz_cmd,
                       vlc_value_t oldval, vlc_value_t newval, void *p_data )
{
    intf_thread_t *p_intf = (intf_thread_t*)p_this;
    audio_volume_t i_volume;
    int i_nb_steps = atoi(newval.psz_string);
    int i_error = VLC_SUCCESS;

    if ( i_nb_steps <= 0 || i_nb_steps > (AOUT_VOLUME_MAX/AOUT_VOLUME_STEP) )
    {
        i_nb_steps = 1;
    }

    if ( !strcmp(psz_cmd, "volup") )
    {
        if ( aout_VolumeUp( p_this, i_nb_steps, &i_volume ) < 0 )
            i_error = VLC_EGENERIC;
    }
    else
    {
        if ( aout_VolumeDown( p_this, i_nb_steps, &i_volume ) < 0 )
            i_error = VLC_EGENERIC;
    }

    if ( !i_error ) msg_rc( "Volume is %d\n", i_volume );
    return i_error;
}

static int AudioConfig( vlc_object_t *p_this, char const *psz_cmd,
                        vlc_value_t oldval, vlc_value_t newval, void *p_data )
{
    intf_thread_t *p_intf = (intf_thread_t*)p_this;
    aout_instance_t * p_aout;
    const char * psz_variable;
    vlc_value_t val_name;
    int i_error;

    p_aout = vlc_object_find( p_this, VLC_OBJECT_AOUT, FIND_ANYWHERE );
    if ( p_aout == NULL ) return VLC_ENOOBJ;

    if ( !strcmp( psz_cmd, "adev" ) )
    {
        psz_variable = "audio-device";
    }
    else
    {
        psz_variable = "audio-channels";
    }

    /* Get the descriptive name of the variable */
    var_Change( (vlc_object_t *)p_aout, psz_variable, VLC_VAR_GETTEXT,
                 &val_name, NULL );
    if( !val_name.psz_string ) val_name.psz_string = strdup(psz_variable);

    if ( !*newval.psz_string )
    {
        /* Retrieve all registered ***. */
        vlc_value_t val, text;
        int i, i_value;

        if ( var_Get( (vlc_object_t *)p_aout, psz_variable, &val ) < 0 )
        {
            vlc_object_release( (vlc_object_t *)p_aout );
            return VLC_EGENERIC;
        }
        i_value = val.i_int;

        if ( var_Change( (vlc_object_t *)p_aout, psz_variable,
                         VLC_VAR_GETLIST, &val, &text ) < 0 )
        {
            vlc_object_release( (vlc_object_t *)p_aout );
            return VLC_EGENERIC;
        }

        msg_rc( "+----[ %s ]\n", val_name.psz_string );
        for ( i = 0; i < val.p_list->i_count; i++ )
        {
            if ( i_value == val.p_list->p_values[i].i_int )
                msg_rc( "| %i - %s *\n", val.p_list->p_values[i].i_int,
                        text.p_list->p_values[i].psz_string );
            else
                msg_rc( "| %i - %s\n", val.p_list->p_values[i].i_int,
                        text.p_list->p_values[i].psz_string );
        }
        var_Change( (vlc_object_t *)p_aout, psz_variable, VLC_VAR_FREELIST,
                    &val, &text );
        msg_rc( "+----[ end of %s ]\n", val_name.psz_string );

        if( val_name.psz_string ) free( val_name.psz_string );
        i_error = VLC_SUCCESS;
    }
    else
    {
        vlc_value_t val;
        val.i_int = atoi( newval.psz_string );

        i_error = var_Set( (vlc_object_t *)p_aout, psz_variable, val );
    }
    vlc_object_release( (vlc_object_t *)p_aout );

    return i_error;
}

#ifdef WIN32
vlc_bool_t ReadWin32( intf_thread_t *p_intf, char *p_buffer, int *pi_size )
{
    INPUT_RECORD input_record;
    DWORD i_dw;

    /* On Win32, select() only works on socket descriptors */
    while( WaitForSingleObject( p_intf->p_sys->hConsoleIn,
                                INTF_IDLE_SLEEP/1000 ) == WAIT_OBJECT_0 )
    {
        while( !p_intf->b_die && *pi_size < MAX_LINE_LENGTH &&
               ReadConsoleInput( p_intf->p_sys->hConsoleIn, &input_record,
                                 1, &i_dw ) )
        {
            if( input_record.EventType != KEY_EVENT ||
                !input_record.Event.KeyEvent.bKeyDown ||
                input_record.Event.KeyEvent.wVirtualKeyCode == VK_SHIFT ||
                input_record.Event.KeyEvent.wVirtualKeyCode == VK_CONTROL||
                input_record.Event.KeyEvent.wVirtualKeyCode == VK_MENU ||
                input_record.Event.KeyEvent.wVirtualKeyCode == VK_CAPITAL )
            {
                /* nothing interesting */
                continue;
            }

            p_buffer[ *pi_size ] = input_record.Event.KeyEvent.uChar.AsciiChar;

            /* Echo out the command */
            putc( p_buffer[ *pi_size ], stdout );

            /* Handle special keys */
            if( p_buffer[ *pi_size ] == '\r' || p_buffer[ *pi_size ] == '\n' )
            {
                putc( '\n', stdout );
                break;
            }
            switch( p_buffer[ *pi_size ] )
            {
            case '\b':
                if( *pi_size )
                {
                    *pi_size -= 2;
                    putc( ' ', stdout );
                    putc( '\b', stdout );
                }
                break;
            case '\r':
                (*pi_size) --;
                break;
            }

            (*pi_size)++;
        }

        if( *pi_size == MAX_LINE_LENGTH ||
            p_buffer[ *pi_size ] == '\r' || p_buffer[ *pi_size ] == '\n' )
        {
            p_buffer[ *pi_size ] = 0;
            return VLC_TRUE;
        }
    }

    return VLC_FALSE;
}
#endif

vlc_bool_t ReadCommand( intf_thread_t *p_intf, char *p_buffer, int *pi_size )
{
    int i_read = 0;

#ifdef WIN32
    if( p_intf->p_sys->i_socket == -1 && !p_intf->p_sys->b_quiet )
        return ReadWin32( p_intf, p_buffer, pi_size );
    else if( p_intf->p_sys->i_socket == -1 )
    {
        msleep( INTF_IDLE_SLEEP );
        return VLC_FALSE;
    }
#endif

    while( !p_intf->b_die && *pi_size < MAX_LINE_LENGTH &&
           (i_read = net_ReadNonBlock( p_intf, p_intf->p_sys->i_socket == -1 ?
                       0 /*STDIN_FILENO*/ : p_intf->p_sys->i_socket,
                       p_buffer + *pi_size, 1, INTF_IDLE_SLEEP ) ) > 0 )
    {
        if( p_buffer[ *pi_size ] == '\r' || p_buffer[ *pi_size ] == '\n' )
            break;

        (*pi_size)++;
    }

    /* Connection closed */
    if( i_read == -1 )
    {
        p_intf->p_sys->i_socket = -1;
        p_buffer[ *pi_size ] = 0;
        return VLC_TRUE;
    }

    if( *pi_size == MAX_LINE_LENGTH ||
        p_buffer[ *pi_size ] == '\r' || p_buffer[ *pi_size ] == '\n' )
    {
        p_buffer[ *pi_size ] = 0;
        return VLC_TRUE;
    }

    return VLC_FALSE;
}

/*****************************************************************************
 * parse_MRL: build a playlist item from a full mrl
 *****************************************************************************
 * MRL format: "simplified-mrl [:option-name[=option-value]]"
 * We don't check for '"' or '\'', we just assume that a ':' that follows a
 * space is a new option. Should be good enough for our purpose.
 *****************************************************************************/
static playlist_item_t *parse_MRL( intf_thread_t *p_intf, char *psz_mrl )
{
#define SKIPSPACE( p ) { while( *p && ( *p == ' ' || *p == '\t' ) ) p++; }
#define SKIPTRAILINGSPACE( p, d ) \
    { char *e=d; while( e > p && (*(e-1)==' ' || *(e-1)=='\t') ){e--;*e=0;} }

    playlist_item_t *p_item = NULL;
    char *psz_item = NULL, *psz_item_mrl = NULL, *psz_orig;
    char **ppsz_options = NULL;
    int i, i_options = 0;

    if( !psz_mrl ) return 0;

    psz_mrl = psz_orig = strdup( psz_mrl );
    while( *psz_mrl )
    {
        SKIPSPACE( psz_mrl );
        psz_item = psz_mrl;

        for( ; *psz_mrl; psz_mrl++ )
        {
            if( (*psz_mrl == ' ' || *psz_mrl == '\t') && psz_mrl[1] == ':' )
            {
                /* We have a complete item */
                break;
            }
            if( (*psz_mrl == ' ' || *psz_mrl == '\t') &&
                (psz_mrl[1] == '"' || psz_mrl[1] == '\'') && psz_mrl[2] == ':')
            {
                /* We have a complete item */
                break;
            }
        }

        if( *psz_mrl ) { *psz_mrl = 0; psz_mrl++; }
        SKIPTRAILINGSPACE( psz_item, psz_item + strlen( psz_item ) );

        /* Remove '"' and '\'' if necessary */
        if( *psz_item == '"' && psz_item[strlen(psz_item)-1] == '"' )
        { psz_item++; psz_item[strlen(psz_item)-1] = 0; }
        if( *psz_item == '\'' && psz_item[strlen(psz_item)-1] == '\'' )
        { psz_item++; psz_item[strlen(psz_item)-1] = 0; }

        if( !psz_item_mrl ) psz_item_mrl = psz_item;
        else if( *psz_item )
        {
            i_options++;
            ppsz_options = realloc( ppsz_options, i_options * sizeof(char *) );
            ppsz_options[i_options - 1] = &psz_item[1];
        }

        if( *psz_mrl ) SKIPSPACE( psz_mrl );
    }

    /* Now create a playlist item */
    if( psz_item_mrl )
    {
        p_item = playlist_ItemNew( p_intf, psz_item_mrl, psz_item_mrl );
        for( i = 0; i < i_options; i++ )
        {
            playlist_ItemAddOption( p_item, ppsz_options[i] );
        }
    }

    if( i_options ) free( ppsz_options );
    free( psz_orig );

    return p_item;
}
