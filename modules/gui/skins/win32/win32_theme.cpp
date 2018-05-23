/*****************************************************************************
 * win32_theme.cpp: Win32 implementation of the Theme class
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: win32_theme.cpp 6961 2004-03-05 17:34:23Z sam $
 *
 * Authors: Olivier Teuli�re <ipkiss@via.ecp.fr>
 *          Emmanuel Puig    <karibu@via.ecp.fr>
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111,
 * USA.
 *****************************************************************************/

#ifdef WIN32

#ifdef _MSC_VER
#   define _WIN32_WINNT 0x0500
#endif

//--- WIN32 -----------------------------------------------------------------
#include <windows.h>

//--- VLC -------------------------------------------------------------------
#include <vlc/intf.h>

//--- SKIN ------------------------------------------------------------------
#include "../os_api.h"
#include "../src/banks.h"
#include "../src/window.h"
#include "../os_window.h"
#include "../src/event.h"
#include "../os_event.h"
#include "../src/theme.h"
#include "../os_theme.h"
#include "../src/vlcproc.h"
#include "../src/skin_common.h"


//---------------------------------------------------------------------------
int SkinManage( intf_thread_t *p_intf );

//---------------------------------------------------------------------------
// Refresh Timer Callback
//---------------------------------------------------------------------------
void CALLBACK RefreshTimer( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
    intf_thread_t *p_intf = (intf_thread_t *)GetWindowLongPtr( hwnd,
        GWLP_USERDATA );
    SkinManage( p_intf );
}

//---------------------------------------------------------------------------
// Win32 interface
//---------------------------------------------------------------------------
LRESULT CALLBACK Win32Proc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // Get pointer to thread info
    intf_thread_t *p_intf = (intf_thread_t *)GetWindowLongPtr( hwnd,
        GWLP_USERDATA );

    // If doesn't exist, treat windows message normally
    if( p_intf == NULL )
        return DefWindowProc( hwnd, uMsg, wParam, lParam );

    // Create event to dispatch in windows
    Event *evt = (Event *)new OSEvent( p_intf, hwnd, uMsg, wParam, lParam );


    // Find window matching with hwnd
    list<SkinWindow *>::const_iterator win;
    for( win = p_intf->p_sys->p_theme->WindowList.begin();
         win != p_intf->p_sys->p_theme->WindowList.end(); win++ )
    {
        // If it is the correct window
        if( hwnd == ( (Win32Window *)(*win) )->GetHandle() )
        {
            // Send event and check if processed
            if( (*win)->ProcessEvent( evt ) )
            {
                delete (OSEvent *)evt;
                return 0;
            }
            else
            {
                break;
            }
        }
    }
    delete (OSEvent *)evt;


    // If Window is parent window
    if( hwnd == ( (Win32Theme *)p_intf->p_sys->p_theme )->GetParentWindow() )
    {
        if( uMsg == WM_SYSCOMMAND )
        {
            if( (Event *)wParam != NULL )
                ( (Event *)wParam )->SendEvent();
            return 0;
        }
        else if( uMsg == WM_RBUTTONDOWN && wParam == 42 &&
                 lParam == WM_RBUTTONDOWN )
        {
            int x, y;
            OSAPI_GetMousePos( x, y );
            TrackPopupMenu(
                ( (Win32Theme *)p_intf->p_sys->p_theme )->GetSysMenu(),
                0, x, y, 0, hwnd, NULL );
        }
    }


    // If closing parent window
    if( uMsg == WM_CLOSE )
    {
        OSAPI_PostMessage( NULL, VLC_HIDE, VLC_QUIT, 0 );
        return 0;
    }

    // If hwnd does not match any window or message not processed
    return DefWindowProc( hwnd, uMsg, wParam, lParam );
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// THEME
//---------------------------------------------------------------------------
Win32Theme::Win32Theme( intf_thread_t *_p_intf ) : Theme( _p_intf )
{
    // Get instance handle
    hinst = GetModuleHandle( NULL );

    // Create window class
    WNDCLASS SkinWindow;

    SkinWindow.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
    SkinWindow.lpfnWndProc = (WNDPROC) Win32Proc;
    SkinWindow.lpszClassName = "SkinWindow";
    SkinWindow.lpszMenuName = NULL;
    SkinWindow.cbClsExtra = 0;
    SkinWindow.cbWndExtra = 0;
    SkinWindow.hbrBackground = HBRUSH (COLOR_WINDOW);
    SkinWindow.hCursor = LoadCursor( NULL , IDC_ARROW );
    SkinWindow.hIcon = LoadIcon( hinst, "VLC_ICON" );
    SkinWindow.hInstance = hinst;

    if( !RegisterClass( &SkinWindow ) )
    {
        WNDCLASS wndclass;

        // Check why it failed. If it's because the class already exists
        // then fine, otherwise return with an error.
        if( !GetClassInfo( hinst, "SkinWindow", &wndclass ) )
        {
            msg_Err( p_intf, "Cannot register window class" );
            return;
        }
    }

    // Initialize the OLE library
    OleInitialize( NULL );

    //Initialize value
    ParentWindow = NULL;

}
//---------------------------------------------------------------------------
Win32Theme::~Win32Theme()
{
    // Unregister the window class if needed
    WNDCLASS wndclass;
    if( GetClassInfo( hinst, "SkinWindow", &wndclass ) )
    {
        UnregisterClass( "SkinWindow", hinst );
    }
    if( GetClassInfo( hinst, "ParentWindow", &wndclass ) )
    {
        UnregisterClass( "ParentWindow", hinst );
    }

    // Delete tray icon if exists
    if( ShowInTray )
    {
        Shell_NotifyIcon( NIM_DELETE, &TrayIcon );
    }

    // Destroy parent window
    if( ParentWindow )
    {
        DestroyWindow( ParentWindow );
    }

    // Uninitialize the OLE library
    OleUninitialize();
}
//---------------------------------------------------------------------------
void Win32Theme::OnLoadTheme()
{
    // Create window class
    WNDCLASS ParentClass;
    ParentClass.style = CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS;
    ParentClass.lpfnWndProc = (WNDPROC) Win32Proc;
    ParentClass.lpszClassName = "ParentWindow";
    ParentClass.lpszMenuName = NULL;
    ParentClass.cbClsExtra = 0;
    ParentClass.cbWndExtra = 0;
    ParentClass.hbrBackground = HBRUSH (COLOR_WINDOW);
    ParentClass.hCursor = LoadCursor( NULL , IDC_ARROW );
    ParentClass.hIcon = LoadIcon( hinst, "VLC_ICON" );
    ParentClass.hInstance = hinst;

    // register class and check it
    if( !RegisterClass( &ParentClass ) )
    {
        WNDCLASS wndclass;

        // Check why it failed. If it's because the class already exists
        // then fine, otherwise return with an error.
        if( !GetClassInfo( hinst, "ParentWindow", &wndclass ) )
        {
            msg_Err( p_intf, "Cannot register window class" );
            return;
        }
    }

    // Create Window
    ParentWindow = CreateWindowEx( WS_EX_TOOLWINDOW, "ParentWindow",
        "VLC Media Player", WS_SYSMENU, 0, 0, 0, 0, 0, 0, hinst, NULL );

    // We do it this way otherwise CreateWindowEx will fail
    // if WS_EX_LAYERED is not supported
    SetWindowLongPtr( ParentWindow, GWL_EXSTYLE, GetWindowLong( ParentWindow,
                      GWL_EXSTYLE ) | WS_EX_LAYERED );

    // Store with it a pointer to the interface thread
    SetWindowLongPtr( ParentWindow, GWLP_USERDATA, (LONG_PTR)p_intf );
    ShowWindow( ParentWindow, SW_SHOW );

    // System tray icon
    TrayIcon.cbSize = sizeof( PNOTIFYICONDATA );
    TrayIcon.hWnd = ParentWindow;
    TrayIcon.uID = 42;
    TrayIcon.uFlags = NIF_ICON|NIF_TIP|NIF_MESSAGE;
    TrayIcon.uCallbackMessage = WM_RBUTTONDOWN;
    TrayIcon.hIcon = LoadIcon( hinst, "VLC_ICON" );
    strcpy( TrayIcon.szTip, "VLC Media Player" );

    // Remove default entries from system menu popup
    SysMenu = GetSystemMenu( ParentWindow, false );
    RemoveMenu( SysMenu, SC_RESTORE,  MF_BYCOMMAND );
    RemoveMenu( SysMenu, SC_MOVE,     MF_BYCOMMAND );
    RemoveMenu( SysMenu, SC_SIZE,     MF_BYCOMMAND );
    RemoveMenu( SysMenu, SC_MINIMIZE, MF_BYCOMMAND );
    RemoveMenu( SysMenu, SC_MAXIMIZE, MF_BYCOMMAND );
    RemoveMenu( SysMenu, SC_CLOSE,    MF_BYCOMMAND );
    RemoveMenu( SysMenu, 0,           MF_BYPOSITION );

    // The create menu
    CreateSystemMenu();

    // Create refresh timer
    SetTimer( ParentWindow, 42, 200, (TIMERPROC)RefreshTimer );
}
//---------------------------------------------------------------------------
void Win32Theme::AddSystemMenu( string name, Event *event )
{
    if( name == "SEPARATOR" )
    {
        AppendMenu( SysMenu, MF_SEPARATOR, 0, NULL );
    }
    else
    {
        AppendMenu( SysMenu, MF_STRING, (unsigned int)event,
                    (char *)name.c_str() );
    }
}
//---------------------------------------------------------------------------
void Win32Theme::ChangeClientWindowName( string name )
{
    SetWindowText( ParentWindow, name.c_str() );
}
//---------------------------------------------------------------------------
void Win32Theme::AddWindow( string name, int x, int y, bool visible,
    int fadetime, int alpha, int movealpha, bool dragdrop, bool playondrop )
{
    HWND hwnd;

    hwnd = CreateWindowEx( WS_EX_TOOLWINDOW,
        "SkinWindow", name.c_str(), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        0, 0, ParentWindow, 0, hinst, NULL );

    if( !hwnd )
    {
        msg_Err( p_intf, "CreateWindow failed" );
        return;
    }

    // We do it this way otherwise CreateWindowEx will fail
    // if WS_EX_LAYERED is not supported
    SetWindowLongPtr( hwnd, GWL_EXSTYLE,
                      GetWindowLong( hwnd, GWL_EXSTYLE ) | WS_EX_LAYERED );

    SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)p_intf );

    WindowList.push_back( (SkinWindow *)new OSWindow( p_intf, hwnd, x, y,
        visible, fadetime, alpha, movealpha, dragdrop, playondrop ) ) ;
}
//---------------------------------------------------------------------------
void Win32Theme::ChangeTray()
{
    if( ShowInTray )
    {
        Shell_NotifyIcon( NIM_DELETE, &TrayIcon );
        ShowInTray = false;
    }
    else
    {
        Shell_NotifyIcon( NIM_ADD, &TrayIcon );
        ShowInTray = true;
    }
}
//---------------------------------------------------------------------------
void Win32Theme::ChangeTaskbar()
{
    if( ShowInTaskbar )
    {
        ShowWindow( ParentWindow, SW_HIDE );
        SetWindowLongPtr( ParentWindow, GWL_EXSTYLE,
                          WS_EX_LAYERED|WS_EX_TOOLWINDOW );
        ShowWindow( ParentWindow, SW_SHOW );
        ShowInTaskbar = false;
    }
    else
    {
        ShowWindow( ParentWindow, SW_HIDE );
        SetWindowLongPtr( ParentWindow, GWL_EXSTYLE,
                          WS_EX_LAYERED|WS_EX_APPWINDOW );
        ShowWindow( ParentWindow, SW_SHOW );
        ShowInTaskbar = true;
    }
}
//---------------------------------------------------------------------------

#endif
