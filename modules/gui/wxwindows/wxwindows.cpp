/*****************************************************************************
 * wxwindows.cpp : wxWindows plugin for vlc
 *****************************************************************************
 * Copyright (C) 2000-2004 VideoLAN
 * $Id: wxwindows.cpp 8869 2004-09-30 22:17:54Z gbazin $
 *
 * Authors: Gildas Bazin <gbazin@netcourrier.com>
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
#include <errno.h>                                                 /* ENOMEM */
#include <string.h>                                            /* strerror() */
#include <stdio.h>

#include <vlc/vlc.h>
#include <vlc/intf.h>

#ifdef HAVE_LOCALE_H
#   include <locale.h>
#endif

#include "wxwindows.h"

/* Temporary hack */
#if defined(WIN32) && defined(_WX_INIT_H_) 
#if (wxMAJOR_VERSION <= 2) && (wxMINOR_VERSION <= 5) && (wxRELEASE_NUMBER < 3)
/* Hack to detect wxWindows 2.5 which has a different wxEntry() prototype */
extern int wxEntry( HINSTANCE hInstance, HINSTANCE hPrevInstance = NULL,
                    char *pCmdLine = NULL, int nCmdShow = SW_NORMAL );
#endif
#endif

#ifdef SYS_DARWIN
int wxEntry( int argc, char *argv[] , bool enterLoop = TRUE );
#endif

/*****************************************************************************
 * Local prototypes.
 *****************************************************************************/
static int  Open         ( vlc_object_t * );
static void Close        ( vlc_object_t * );
static int  OpenDialogs  ( vlc_object_t * );

static void Run          ( intf_thread_t * );
static void Init         ( intf_thread_t * );

static void ShowDialog   ( intf_thread_t *, int, int, intf_dialog_args_t * );

/*****************************************************************************
 * Local classes declarations.
 *****************************************************************************/
class Instance: public wxApp
{
public:
    Instance();
    Instance( intf_thread_t *_p_intf );

    bool OnInit();
    int  OnExit();

private:
    intf_thread_t *p_intf;
    wxLocale locale;                                /* locale we'll be using */
};

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define EMBED_TEXT N_("Embed video in interface")
#define EMBED_LONGTEXT N_("Embed the video inside the interface instead " \
    "of having it in a separate window.")
#define BOOKMARKS_TEXT N_("Show bookmarks dialog")
#define BOOKMARKS_LONGTEXT N_("Show bookmarks dialog when the interface " \
    "starts.")

vlc_module_begin();
#ifdef WIN32
    int i_score = 150;
#else
    int i_score = getenv( "DISPLAY" ) == NULL ? 15 : 150;
#endif
    set_description( (char *) _("wxWindows interface module") );
    set_capability( "interface", i_score );
    set_callbacks( Open, Close );
    add_shortcut( "wxwindows" );
    add_shortcut( "wxwin" );
    add_shortcut( "wx" );
    set_program( "wxvlc" );

    add_bool( "wxwin-embed", 1, NULL,
              EMBED_TEXT, EMBED_LONGTEXT, VLC_FALSE );
    add_bool( "wxwin-bookmarks", 0, NULL,
              BOOKMARKS_TEXT, BOOKMARKS_LONGTEXT, VLC_FALSE );

    add_submodule();
    set_description( _("wxWindows dialogs provider") );
    set_capability( "dialogs provider", 50 );
    set_callbacks( OpenDialogs, Close );

#if !defined(WIN32)
    linked_with_a_crap_library_which_uses_atexit();
#endif
vlc_module_end();

/*****************************************************************************
 * Open: initialize and create window
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t *)p_this;

    /* Allocate instance and initialize some members */
    p_intf->p_sys = (intf_sys_t *)malloc( sizeof( intf_sys_t ) );
    if( p_intf->p_sys == NULL )
    {
        msg_Err( p_intf, "out of memory" );
        return VLC_ENOMEM;
    }
    memset( p_intf->p_sys, 0, sizeof( intf_sys_t ) );

    p_intf->pf_run = Run;

    p_intf->p_sys->p_sub = msg_Subscribe( p_intf );

    /* Initialize wxWindows thread */
    p_intf->p_sys->b_playing = 0;

    p_intf->p_sys->p_input = NULL;
    p_intf->p_sys->i_playing = -1;
    p_intf->p_sys->b_slider_free = 1;
    p_intf->p_sys->i_slider_pos = p_intf->p_sys->i_slider_oldpos = 0;

    p_intf->p_sys->p_popup_menu = NULL;
    p_intf->p_sys->p_video_window = NULL;

    p_intf->pf_show_dialog = NULL;

    /* We support play on start */
    p_intf->b_play = VLC_TRUE;

    return VLC_SUCCESS;
}

static int OpenDialogs( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t *)p_this;
    int i_ret = Open( p_this );

    p_intf->pf_show_dialog = ShowDialog;

    return i_ret;
}

/*****************************************************************************
 * Close: destroy interface window
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t *)p_this;

    if( p_intf->p_sys->p_input )
    {
        vlc_object_release( p_intf->p_sys->p_input );
    }

    vlc_mutex_lock( &p_intf->object_lock );
    p_intf->b_dead = VLC_TRUE;
    vlc_mutex_unlock( &p_intf->object_lock );

    if( p_intf->pf_show_dialog )
    {
        /* We must destroy the dialogs thread */
        wxCommandEvent event( wxEVT_DIALOG, INTF_DIALOG_EXIT );
        p_intf->p_sys->p_wxwindow->AddPendingEvent( event );
        vlc_thread_join( p_intf );
    }

    msg_Unsubscribe( p_intf, p_intf->p_sys->p_sub );

    /* Destroy structure */
    free( p_intf->p_sys );
}

/*****************************************************************************
 * Run: wxWindows thread
 *****************************************************************************/
#if !defined(__BUILTIN__) && defined( WIN32 )
HINSTANCE hInstance = 0;
extern "C" BOOL WINAPI
DllMain (HANDLE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    hInstance = (HINSTANCE)hModule;
    return TRUE;
}
#endif

static void Run( intf_thread_t *p_intf )
{
    if( p_intf->pf_show_dialog )
    {
        /* The module is used in dialog provider mode */

        /* Create a new thread for wxWindows */
        if( vlc_thread_create( p_intf, "Skins Dialogs Thread",
                               Init, 0, VLC_TRUE ) )
        {
            msg_Err( p_intf, "cannot create Skins Dialogs Thread" );
            p_intf->pf_show_dialog = NULL;
        }
    }
    else
    {
        /* The module is used in interface mode */
        Init( p_intf );
    }
}

static void Init( intf_thread_t *p_intf )
{
#if !defined( WIN32 )
    static char  *p_args[] = { "" };
    int i_args = 1;
#endif

    /* Hack to pass the p_intf pointer to the new wxWindow Instance object */
#ifdef wxTheApp
    wxApp::SetInstance( new Instance( p_intf ) );
#else
    wxTheApp = new Instance( p_intf );
#endif

#if defined( WIN32 )
#if !defined(__BUILTIN__)
    wxEntry( hInstance/*GetModuleHandle(NULL)*/, NULL, NULL, SW_SHOW );
#else
    wxEntry( GetModuleHandle(NULL), NULL, NULL, SW_SHOW );
#endif
#else
    wxEntry( i_args, p_args );
#endif
}

/* following functions are local */

/*****************************************************************************
 * Constructors.
 *****************************************************************************/
Instance::Instance( )
{
}

Instance::Instance( intf_thread_t *_p_intf )
{
    /* Initialization */
    p_intf = _p_intf;
}

IMPLEMENT_APP_NO_MAIN(Instance)

/*****************************************************************************
 * Instance::OnInit: the parent interface execution starts here
 *****************************************************************************
 * This is the "main program" equivalent, the program execution will
 * start here.
 *****************************************************************************/
bool Instance::OnInit()
{
    /* Initialization of i18n stuff.
     * Usefull for things we don't have any control over, like wxWindows
     * provided facilities (eg. open file dialog) */
    locale.Init( wxLANGUAGE_DEFAULT );

    /* FIXME: The stream output mrl parsing uses ',' already so we want to
     * keep the default '.' for floating point numbers. */
    setlocale( LC_NUMERIC, "C" );

    /* Make an instance of your derived frame. Passing NULL (the default value
     * of Frame's constructor is NULL) as the frame doesn't have a parent
     * since it is the first window */

    if( !p_intf->pf_show_dialog )
    {
        /* The module is used in interface mode */
        Interface *MainInterface = new Interface( p_intf );
        p_intf->p_sys->p_wxwindow = MainInterface;

        /* Show the interface */
        MainInterface->Show( TRUE );
        SetTopWindow( MainInterface );
        MainInterface->Raise();
    }

    /* Creates the dialogs provider */
    p_intf->p_sys->p_wxwindow =
        CreateDialogsProvider( p_intf, p_intf->pf_show_dialog ?
                               NULL : p_intf->p_sys->p_wxwindow );

    p_intf->p_sys->pf_show_dialog = ShowDialog;

    /* OK, initialization is over */
    vlc_thread_ready( p_intf );

    /* Check if we need to start playing */
    if( !p_intf->pf_show_dialog && p_intf->b_play )
    {
        playlist_t *p_playlist =
            (playlist_t *)vlc_object_find( p_intf, VLC_OBJECT_PLAYLIST,
                                           FIND_ANYWHERE );
        if( p_playlist )
        {
            playlist_Play( p_playlist );
            vlc_object_release( p_playlist );
        }
    }

    /* Return TRUE to tell program to continue (FALSE would terminate) */
    return TRUE;
}

/*****************************************************************************
 * Instance::OnExit: called when the interface execution stops
 *****************************************************************************/
int Instance::OnExit()
{
    if( p_intf->pf_show_dialog )
    {
         /* We need to manually clean up the dialogs class */
         if( p_intf->p_sys->p_wxwindow ) delete p_intf->p_sys->p_wxwindow;
    }
    return 0;
}

static void ShowDialog( intf_thread_t *p_intf, int i_dialog_event, int i_arg,
                        intf_dialog_args_t *p_arg )
{
    wxCommandEvent event( wxEVT_DIALOG, i_dialog_event );
    event.SetInt( i_arg );
    event.SetClientData( p_arg );

#ifdef WIN32
    SendMessage( (HWND)p_intf->p_sys->p_wxwindow->GetHandle(),
                 WM_CANCELMODE, 0, 0 );
#endif
    if( i_dialog_event == INTF_DIALOG_POPUPMENU && i_arg == 0 ) return;

    /* Hack to prevent popup events to be enqueued when
     * one is already active */
    if( i_dialog_event != INTF_DIALOG_POPUPMENU ||
        !p_intf->p_sys->p_popup_menu )
    {
        p_intf->p_sys->p_wxwindow->AddPendingEvent( event );
    }
}
