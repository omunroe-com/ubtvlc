/*****************************************************************************
 * win32_window.cpp: Win32 implementation of the Window class
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: win32_window.cpp 6961 2004-03-05 17:34:23Z sam $
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

/* For TrackMouseEvent, WM_MOUSEWHEEL and GET_WHEEL_DELTA_WPARAM */
#undef WINVER
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#define WINVER 0x0500

//--- GENERAL ---------------------------------------------------------------
//#include <math.h>

//--- VLC -------------------------------------------------------------------
#include <vlc/intf.h>

//--- WIN32 -----------------------------------------------------------------
#include <windows.h>

//--- SKIN ------------------------------------------------------------------
#include "../os_api.h"
#include "../src/anchor.h"
#include "../controls/generic.h"
#include "../src/window.h"
#include "../os_window.h"
#include "../src/event.h"
#include "../os_event.h"
#include "../src/graphics.h"
#include "../os_graphics.h"
#include "../src/skin_common.h"
#include "../src/theme.h"
#include "../os_theme.h"
#include "../src/banks.h"

//---------------------------------------------------------------------------
// Fading API
//---------------------------------------------------------------------------
#define LWA_COLORKEY  0x00000001
#define LWA_ALPHA     0x00000002

//---------------------------------------------------------------------------
// Skinable Window
//---------------------------------------------------------------------------
Win32Window::Win32Window( intf_thread_t *p_intf, HWND hwnd, int x, int y,
    bool visible, int transition, int normalalpha, int movealpha,
    bool dragdrop, bool playondrop )
    : SkinWindow( p_intf, x, y, visible, transition, normalalpha, movealpha,
              dragdrop )
{
    // Set handles
    hWnd      = hwnd;

    // Set position parameters
    CursorPos = new POINT;
    WindowPos = new POINT;

    // Create Tool Tip Window
    ToolTipWindow = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        hWnd, 0, GetModuleHandle( NULL ), 0);

    // Create Tool Tip infos
    ToolTipInfo.cbSize = sizeof(TOOLINFO);
    ToolTipInfo.uFlags = TTF_SUBCLASS|TTF_IDISHWND;
    ToolTipInfo.hwnd = hWnd;
    ToolTipInfo.hinst = GetModuleHandle( NULL );
    ToolTipInfo.uId = (unsigned int)hWnd;
    ToolTipInfo.lpszText = NULL;
    ToolTipInfo.rect.left = ToolTipInfo.rect.top = 0;
    ToolTipInfo.rect.right = ToolTipInfo.rect.bottom = 0;

    SendMessage( ToolTipWindow, TTM_ADDTOOL, 0,
                    (LPARAM)(LPTOOLINFO) &ToolTipInfo );

    // Drag & drop
    if( DragDrop )
    {
        DropTarget = (LPDROPTARGET) new Win32DropObject( playondrop );
        // register the listview as a drop target
        RegisterDragDrop( hWnd, DropTarget );
    }
}
//---------------------------------------------------------------------------
Win32Window::~Win32Window()
{
    delete CursorPos;
    delete WindowPos;

    if( ToolTipWindow != NULL )
    {
        DestroyWindow( ToolTipWindow );
    }

    if( hWnd != NULL )
    {
        if( DragDrop )
        {
            // Remove the listview from the list of drop targets
            RevokeDragDrop( hWnd );
            DropTarget->Release();
        }

        DestroyWindow( hWnd );
    }
}
//---------------------------------------------------------------------------
bool Win32Window::ProcessOSEvent( Event *evt )
{
    unsigned int msg = evt->GetMessage();
    unsigned int p1  = evt->GetParam1();
    int          p2  = evt->GetParam2();

    switch( msg )
    {
        case WM_PAINT:
            HDC DC;
            PAINTSTRUCT Infos;
            DC = BeginPaint( hWnd , &Infos );
            EndPaint( hWnd , &Infos );
            RefreshFromImage( 0, 0, Width, Height );
            return true;

        case WM_MOUSEMOVE:
            TRACKMOUSEEVENT TrackEvent;
            TrackEvent.cbSize      = sizeof( TRACKMOUSEEVENT );
            TrackEvent.dwFlags     = TME_LEAVE;
            TrackEvent.hwndTrack   = hWnd;
            TrackEvent.dwHoverTime = 1;
            TrackMouseEvent( &TrackEvent );
            if( p1 == MK_LBUTTON )
                MouseMove( LOWORD( p2 ), HIWORD( p2 ), 1 );
            else if( p1 == MK_RBUTTON )
                MouseMove( LOWORD( p2 ), HIWORD( p2 ), 2 );
            else
                MouseMove( LOWORD( p2 ), HIWORD( p2 ), 0 );

            return true;

        case WM_LBUTTONDOWN:
            SetCapture( hWnd );
            MouseDown( LOWORD( p2 ), HIWORD( p2 ), 1 );
            return true;

        case WM_LBUTTONUP:
            ReleaseCapture();
            MouseUp( LOWORD( p2 ), HIWORD( p2 ), 1 );
            return true;

        case WM_RBUTTONDOWN:
            MouseDown( LOWORD( p2 ), HIWORD( p2 ), 2 );
            return true;

        case WM_RBUTTONUP:
            MouseUp( LOWORD( p2 ), HIWORD( p2 ), 2 );
            return true;

        case WM_LBUTTONDBLCLK:
            MouseDblClick( LOWORD( p2 ), HIWORD( p2 ), 1 );
            return true;

        case WM_MOUSELEAVE:
            OSAPI_PostMessage( this, WINDOW_LEAVE, 0, 0 );
            return true;

        case WM_MOUSEWHEEL:
            if( GET_WHEEL_DELTA_WPARAM( p1 ) > 0 )
                MouseScroll( LOWORD( p2 ) - Left, HIWORD( p2 ) - Top,
                    MOUSE_SCROLL_UP );
            else if( GET_WHEEL_DELTA_WPARAM( p1 ) < 0 )
                MouseScroll( LOWORD( p2 ) - Left, HIWORD( p2 ) - Top,
                    MOUSE_SCROLL_DOWN );
            return true;

        default:
            return false;
    }
}
//---------------------------------------------------------------------------
void Win32Window::ToggleOnTop()
{
    Win32Theme *winTheme = (Win32Theme *)p_intf->p_sys->p_theme;
    HMENU hMenu = GetSystemMenu( winTheme->GetParentWindow(), false );
    Event *event = p_intf->p_sys->p_theme->EvtBank->Get( "on_top" );

    if( !p_intf->p_sys->b_on_top )
    {
        // Set the window on top
        SetWindowPos( hWnd, HWND_TOPMOST, 0, 0, 0, 0,
                      SWP_NOSIZE | SWP_NOMOVE );
        // Check the menu entry (FIXME: we shouldn't do that here...)
        CheckMenuItem( hMenu, (unsigned int)event,
                       MF_BYCOMMAND | MFS_CHECKED );
    }
    else
    {
        // Set the window not on top
        SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
                      SWP_NOSIZE | SWP_NOMOVE );
        // Uncheck the menu entry (FIXME: we shouldn't do that here...)
        CheckMenuItem( hMenu, (unsigned int)event,
                       MF_BYCOMMAND | MFS_UNCHECKED );
    }
}
//---------------------------------------------------------------------------
void Win32Window::OSShow( bool show )
{
    if( show )
    {
        ShowWindow( hWnd, SW_SHOW );
    }
    else
    {
        ShowWindow( hWnd, SW_HIDE );
    }
}
//---------------------------------------------------------------------------
void Win32Window::SetTransparency( int Value )
{
    if( Value > -1 )
        Alpha = Value;

    if( p_intf->p_sys->SetLayeredWindowAttributes )
        p_intf->p_sys->SetLayeredWindowAttributes( hWnd, 0, Alpha,
                                                   LWA_ALPHA | LWA_COLORKEY );

    UpdateWindow( hWnd );
}
//---------------------------------------------------------------------------
void Win32Window::RefreshFromImage( int x, int y, int w, int h )
{
    // Initialize painting
    HDC DC = GetWindowDC( hWnd );

    // Draw image on window
    BitBlt( DC, x, y, w, h, ( (Win32Graphics *)Image )->GetImageHandle(),
            x, y, SRCCOPY );

    // Release window device context
    ReleaseDC( hWnd, DC );
}
//---------------------------------------------------------------------------
void Win32Window::WindowManualMove()
{
    // Get mouse cursor position
    LPPOINT NewPos = new POINT;
    GetCursorPos( NewPos );

    // Move window and chek for magnetism
    p_intf->p_sys->p_theme->MoveSkinMagnet( this,
        WindowPos->x + NewPos->x - CursorPos->x,
        WindowPos->y + NewPos->y - CursorPos->y );

    // Free memory
    delete[] NewPos;
}
//---------------------------------------------------------------------------
void Win32Window::WindowManualMoveInit()
{
    GetCursorPos( CursorPos );
    WindowPos->x = Left;
    WindowPos->y = Top;
}
//---------------------------------------------------------------------------
void Win32Window::Move( int left, int top )
{
    Left = left;
    Top  = top;
    //SetWindowPos( hWnd, HWND_TOP, Left, Top, Width, Height,
    //              SWP_NOSIZE|SWP_NOREDRAW|SWP_NOZORDER );
    MoveWindow( hWnd, Left, Top, Width, Height, false );
}
//---------------------------------------------------------------------------
void Win32Window::Size( int width, int height )
{
    Width  = width;
    Height = height;
    SetWindowPos( hWnd, HWND_TOP, Left, Top, Width, Height,
                  SWP_NOMOVE|SWP_NOREDRAW|SWP_NOZORDER );
}
//---------------------------------------------------------------------------
void Win32Window::ChangeToolTipText( string text )
{
    if( text == "none" )
    {
        if( ToolTipText != "none" )
        {
            ToolTipText = "none";
            ToolTipInfo.lpszText = NULL;
            SendMessage( ToolTipWindow, TTM_ACTIVATE, 0 , 0 );
        }
    }
    else
    {
        if( text != ToolTipText )
        {
            ToolTipText = text;
            ToolTipInfo.lpszText = (char *)ToolTipText.c_str();
            SendMessage( ToolTipWindow, TTM_ACTIVATE, 1 , 0 );
            SendMessage( ToolTipWindow, TTM_UPDATETIPTEXT, 0,
                             (LPARAM)(LPTOOLINFO)&ToolTipInfo );
        }
    }
}
//---------------------------------------------------------------------------

#endif
