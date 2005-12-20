/*****************************************************************************
 * vout.h: Windows DirectX video output header file
 *****************************************************************************
 * Copyright (C) 2001-2004 VideoLAN
 * $Id: vout.h 9269 2004-11-10 13:04:45Z gbazin $
 *
 * Authors: Gildas Bazin <gbazin@videolan.org>
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
 * event_thread_t: DirectX event thread
 *****************************************************************************/
typedef struct event_thread_t
{
    VLC_COMMON_MEMBERS

    vout_thread_t * p_vout;

} event_thread_t;

/*****************************************************************************
 * vout_sys_t: video output DirectX method descriptor
 *****************************************************************************
 * This structure is part of the video output thread descriptor.
 * It describes the DirectX specific properties of an output thread.
 *****************************************************************************/
struct vout_sys_t
{
    LPDIRECTDRAW2        p_ddobject;                    /* DirectDraw object */
    LPDIRECTDRAWSURFACE2 p_display;                        /* Display device */
    LPDIRECTDRAWSURFACE2 p_current_surface;   /* surface currently displayed */
    LPDIRECTDRAWCLIPPER  p_clipper;             /* clipper used for blitting */
    HINSTANCE            hddraw_dll;       /* handle of the opened ddraw dll */

    HWND                 hwnd;                  /* Handle of the main window */
    HWND                 hvideownd;        /* Handle of the video sub-window */
    HWND                 hparent;             /* Handle of the parent window */
    HWND                 hfswnd;          /* Handle of the fullscreen window */
    WNDPROC              pf_wndproc;             /* Window handling callback */

    /* Multi-monitor support */
    HMONITOR             hmonitor;          /* handle of the current monitor */
    GUID                 *p_display_driver;
    HMONITOR             (WINAPI* MonitorFromWindow)( HWND, DWORD );
    BOOL                 (WINAPI* GetMonitorInfo)( HMONITOR, LPMONITORINFO );

    vlc_bool_t   b_using_overlay;         /* Are we using an overlay surface */
    vlc_bool_t   b_use_sysmem;   /* Should we use system memory for surfaces */
    vlc_bool_t   b_hw_yuv;    /* Should we use hardware YUV->RGB conversions */
    vlc_bool_t   b_3buf_overlay;   /* Should we use triple buffered overlays */

    /* size of the display */
    RECT         rect_display;
    int          i_display_depth;

    /* Window position and size */
    int          i_window_x;
    int          i_window_y;
    int          i_window_width;
    int          i_window_height;

    /* Coordinates of src and dest images (used when blitting to display) */
    RECT         rect_src;
    RECT         rect_src_clipped;
    RECT         rect_dest;
    RECT         rect_dest_clipped;
    RECT         rect_parent;

    /* Overlay alignment restrictions */
    int          i_align_src_boundary;
    int          i_align_src_size;
    int          i_align_dest_boundary;
    int          i_align_dest_size;

    /* DDraw capabilities */
    int          b_caps_overlay_clipping;

    int          i_rgb_colorkey;      /* colorkey in RGB used by the overlay */
    int          i_colorkey;                 /* colorkey used by the overlay */

    volatile uint16_t i_changes;        /* changes made to the video display */

    /* Mouse */
    volatile vlc_bool_t b_cursor_hidden;
    volatile mtime_t    i_lastmoved;

    /* Misc */
    vlc_bool_t      b_on_top_change;

    vlc_bool_t      b_wallpaper;
    COLORREF        color_bkg;
    COLORREF        color_bkgtxt;

#ifdef MODULE_NAME_IS_glwin32
    HDC hGLDC;
    HGLRC hGLRC;
#endif

    event_thread_t *p_event;
    vlc_mutex_t    lock;
};

/*****************************************************************************
 * picture_sys_t: direct buffer method descriptor
 *****************************************************************************
 * This structure is part of the picture descriptor, it describes the
 * DirectX specific properties of a direct buffer.
 *****************************************************************************/
struct picture_sys_t
{
    LPDIRECTDRAWSURFACE2 p_surface;
    DDSURFACEDESC        ddsd;
    LPDIRECTDRAWSURFACE2 p_front_surface;
};

/*****************************************************************************
 * Prototypes from vout.c
 *****************************************************************************/
int DirectXUpdateOverlay( vout_thread_t *p_vout );

/*****************************************************************************
 * Prototypes from events.c
 *****************************************************************************/
void DirectXEventThread ( event_thread_t *p_event );
void DirectXUpdateRects ( vout_thread_t *p_vout, vlc_bool_t b_force );

/*****************************************************************************
 * Constants
 *****************************************************************************/
#define WM_VLC_HIDE_MOUSE WM_APP
#define WM_VLC_SHOW_MOUSE WM_APP + 1
#define WM_VLC_CREATE_VIDEO_WIN WM_APP + 2
#define WM_VLC_CHANGE_TEXT WM_APP + 3
#define IDM_TOGGLE_ON_TOP WM_USER + 1
#define DX_POSITION_CHANGE 0x1000
#define DX_WALLPAPER_CHANGE 0x2000
