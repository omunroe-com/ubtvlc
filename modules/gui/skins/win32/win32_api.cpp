/*****************************************************************************
 * win32_api.cpp: Various win32-specific functions
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: win32_api.cpp 6961 2004-03-05 17:34:23Z sam $
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

//--- WIN32 -----------------------------------------------------------------
#include <windows.h>

//--- SKIN ------------------------------------------------------------------
#include <vlc/intf.h>
#include "../src/skin_common.h"
#include "../src/window.h"
#include "../os_window.h"
#include "../os_api.h"
#include "../src/event.h"         // for MAX_PARAM_SIZE

extern intf_thread_t *g_pIntf;  // ugly, but it's not my fault ;)

//---------------------------------------------------------------------------
// Event API
//---------------------------------------------------------------------------
void OSAPI_SendMessage( SkinWindow *win, unsigned int message, unsigned int param1,
                        long param2 )
{
    if( win == NULL )
        SendMessage( NULL, message, param1, param2 );
    else
        SendMessage( ( (Win32Window *)win )->GetHandle(), message, param1,
                     param2 );
}
//---------------------------------------------------------------------------
void OSAPI_PostMessage( SkinWindow *win, unsigned int message, unsigned int param1,
                        long param2 )
{
    if( win == NULL )
        PostThreadMessage( g_pIntf->p_sys->dwThreadId, message, param1,
                           param2 );
    else
        PostMessage( ( (Win32Window *)win )->GetHandle(), message, param1,
                     param2 );
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// Graphic API
//---------------------------------------------------------------------------
int OSAPI_GetNonTransparentColor( int c )
{
    // Get desktop device context
    HDC DeskDC = GetWindowDC( GetDesktopWindow() );

    // If color is black or color is same as black wether pixel color depth
    if( c == 0 || SetPixel( DeskDC, 0, 0, c ) == 0 )
    {
        if( GetDeviceCaps( DeskDC, BITSPIXEL ) < 24 )
            c = RGB(8, 0, 0);
        else
            c = RGB(1, 0, 0);
    }
    ReleaseDC( GetDesktopWindow(), DeskDC );
    return c;
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// General
//---------------------------------------------------------------------------
int OSAPI_GetTime()
{
    return GetTickCount();
}
//---------------------------------------------------------------------------
void OSAPI_GetScreenSize( int &w, int &h )
{
    w = GetSystemMetrics(SM_CXSCREEN);
    h = GetSystemMetrics(SM_CYSCREEN);
}
//---------------------------------------------------------------------------
void OSAPI_GetMousePos( int &x, int &y )
{
    LPPOINT MousePos = new POINT;
    GetCursorPos( MousePos );
    x = MousePos->x;
    y = MousePos->y;
    delete MousePos;
}
//---------------------------------------------------------------------------
string OSAPI_GetWindowTitle( SkinWindow *win )
{
    char *buffer = new char[MAX_PARAM_SIZE];
    GetWindowText( ((Win32Window *)win)->GetHandle(), buffer, MAX_PARAM_SIZE );
    string Title = buffer;
    delete buffer;

    return Title;
}
//---------------------------------------------------------------------------
bool OSAPI_RmDir( string path )
{
    WIN32_FIND_DATA find;
    string File;
    string FindFiles = path + "\\*.*";
    HANDLE handle    = FindFirstFile( (char *)FindFiles.c_str(), &find );

    while( handle != INVALID_HANDLE_VALUE )
    {
        // If file is neither "." nor ".."
        if( strcmp( find.cFileName, "." ) && strcmp( find.cFileName, ".." ) )
        {
            // Set file name
            File = path + "\\" + (string)find.cFileName;

            // If file is a directory, delete it recursively
            if( find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
                OSAPI_RmDir( File );
            }
            // Else, it is a file so simply delete it
            else
            {
                DeleteFile( (char *)File.c_str() );
            }
        }

        // If no more file in directory, exit while
        if( !FindNextFile( handle, &find ) )
            break;
    }

    // Now directory is empty so can be removed
    FindClose( handle );
    RemoveDirectory( (char *)path.c_str() );

    return true;
}
//---------------------------------------------------------------------------

#endif
