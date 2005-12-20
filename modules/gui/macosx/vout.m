/*****************************************************************************
 * vout.m: MacOS X video output module
 *****************************************************************************
 * Copyright (C) 2001-2003 VideoLAN
 * $Id: vout.m 7710 2004-05-18 09:08:44Z titer $
 *
 * Authors: Colin Delacroix <colin@zoy.org>
 *          Florian G. Pflug <fgp@phlo.org>
 *          Jon Lech Johansen <jon-vl@nanocrew.net>
 *          Derk-Jan Hartman <thedj@users.sourceforge.net>
 *          Eric Petit <titer@m0k.org>
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
#include <errno.h>                                                 /* ENOMEM */
#include <stdlib.h>                                                /* free() */
#include <string.h>                                            /* strerror() */

#include <QuickTime/QuickTime.h>

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>

#include <vlc_keys.h>

#include "intf.h"
#include "vout.h"

#define QT_MAX_DIRECTBUFFERS 10
#define VL_MAX_DISPLAYS 16

#define OPENGL_EFFECT_NONE             1
#define OPENGL_EFFECT_CUBE             2
#define OPENGL_EFFECT_TRANSPARENT_CUBE 4

struct picture_sys_t
{
    void *p_info;
    unsigned int i_size;

    /* When using I420 output */
    PlanarPixmapInfoYUV420 pixmap_i420;
};

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/

static int  vout_Init      ( vout_thread_t * );
static void vout_End       ( vout_thread_t * );
static int  vout_Manage    ( vout_thread_t * );
static void vout_Display   ( vout_thread_t *, picture_t * );

static int  CoSendRequest      ( vout_thread_t *, SEL );
static int  CoCreateWindow     ( vout_thread_t * );
static int  CoDestroyWindow    ( vout_thread_t * );
static int  CoToggleFullscreen ( vout_thread_t * );

static void VLCHideMouse       ( vout_thread_t *, BOOL );

static void QTScaleMatrix      ( vout_thread_t * );
static int  QTCreateSequence   ( vout_thread_t * );
static void QTDestroySequence  ( vout_thread_t * );
static int  QTNewPicture       ( vout_thread_t *, picture_t * );
static void QTFreePicture      ( vout_thread_t *, picture_t * );

/*****************************************************************************
 * OpenVideo: allocates MacOS X video thread output method
 *****************************************************************************
 * This function allocates and initializes a MacOS X vout method.
 *****************************************************************************/
int E_(OpenVideo) ( vlc_object_t *p_this )
{   
    vout_thread_t * p_vout = (vout_thread_t *)p_this;
    OSErr err;
    int i_timeout;
    char *psz_vout_type;

    p_vout->p_sys = malloc( sizeof( vout_sys_t ) );
    if( p_vout->p_sys == NULL )
    {
        msg_Err( p_vout, "out of memory" );
        return( 1 );
    }

    memset( p_vout->p_sys, 0, sizeof( vout_sys_t ) );

    /* Wait for a MacOS X interface to appear. Timeout is 2 seconds. */
    for( i_timeout = 20 ; i_timeout-- ; )
    {
        if( NSApp == NULL )
        {
            msleep( INTF_IDLE_SLEEP );
        }
    }

    if( NSApp == NULL )
    {
        /* no MacOS X intf, unable to communicate with MT */
        msg_Err( p_vout, "no MacOS X interface present" );
        free( p_vout->p_sys );
        return( 1 );
    }

    if( [NSApp respondsToSelector: @selector(getIntf)] )
    {
        intf_thread_t * p_intf;

        for( i_timeout = 10 ; i_timeout-- ; )
        {
            if( ( p_intf = [NSApp getIntf] ) == NULL )
            {
                msleep( INTF_IDLE_SLEEP );
            }
        }

        if( p_intf == NULL )
        {
            msg_Err( p_vout, "MacOS X intf has getIntf, but is NULL" );
            free( p_vout->p_sys );
            return( 1 );
        }
    }

    p_vout->p_sys->b_mouse_moved = VLC_TRUE;
    p_vout->p_sys->i_time_mouse_last_moved = mdate();

    /* set window size */
    p_vout->p_sys->s_rect.size.width = p_vout->i_window_width;
    p_vout->p_sys->s_rect.size.height = p_vout->i_window_height;

    /* Check if we should use QuickTime or OpenGL */
    psz_vout_type = config_GetPsz( p_vout, "macosx-vout" );

    if( !strncmp( psz_vout_type, "auto", 4 ) )
    {
        p_vout->p_sys->i_opengl = CGDisplayUsesOpenGLAcceleration( kCGDirectMainDisplay );
    }
    else if( !strncmp( psz_vout_type, "opengl", 6 ) )
    {
        p_vout->p_sys->i_opengl = VLC_TRUE;
    }
    else
    {
        p_vout->p_sys->i_opengl = VLC_FALSE;
    }
    free( psz_vout_type );
    
    if( !p_vout->p_sys->i_opengl )
    {
        /* Initialize QuickTime */
        p_vout->p_sys->h_img_descr = 
            (ImageDescriptionHandle)NewHandleClear( sizeof(ImageDescription) );
        p_vout->p_sys->p_matrix =
            (MatrixRecordPtr)malloc( sizeof(MatrixRecord) );
        p_vout->p_sys->p_fullscreen_state = NULL;

        if( ( err = EnterMovies() ) != noErr )
        {
            msg_Err( p_vout, "EnterMovies failed: %d", err );
            free( p_vout->p_sys->p_matrix );
            DisposeHandle( (Handle)p_vout->p_sys->h_img_descr );
            free( p_vout->p_sys );
            return( 1 );
        } 

        /* Damn QT isn't thread safe. so keep a lock in the p_vlc object */
        vlc_mutex_lock( &p_vout->p_vlc->quicktime_lock );

        err = FindCodec( kYUV420CodecType, bestSpeedCodec,
                            nil, &p_vout->p_sys->img_dc );
        
        vlc_mutex_unlock( &p_vout->p_vlc->quicktime_lock );
        if( err == noErr && p_vout->p_sys->img_dc != 0 )
        {
            p_vout->output.i_chroma = VLC_FOURCC('I','4','2','0');
            p_vout->p_sys->i_codec = kYUV420CodecType;
        }
        else
        {
            msg_Err( p_vout, "failed to find an appropriate codec" );
        }

        if( p_vout->p_sys->img_dc == 0 )
        {
            free( p_vout->p_sys->p_matrix );
            DisposeHandle( (Handle)p_vout->p_sys->h_img_descr );
            free( p_vout->p_sys );
            return VLC_EGENERIC;        
        }
        msg_Dbg( p_vout, "using Quartz mode" );
    }
    else
    {
        msg_Dbg( p_vout, "using OpenGL mode" );
    }

    NSAutoreleasePool * o_pool = [[NSAutoreleasePool alloc] init];
    NSArray * o_screens = [NSScreen screens];
    if( [o_screens count] > 0 && var_Type( p_vout, "video-device" ) == 0 )
    {
        int i = 1;
        vlc_value_t val, text;
        NSScreen * o_screen;

        int i_option = config_GetInt( p_vout, "macosx-vdev" );

        var_Create( p_vout, "video-device", VLC_VAR_INTEGER |
                                            VLC_VAR_HASCHOICE ); 
        text.psz_string = _("Video device");
        var_Change( p_vout, "video-device", VLC_VAR_SETTEXT, &text, NULL );
        
        NSEnumerator * o_enumerator = [o_screens objectEnumerator];

        while( (o_screen = [o_enumerator nextObject]) != NULL )
        {
            char psz_temp[255];
            NSRect s_rect = [o_screen frame];

            snprintf( psz_temp, sizeof(psz_temp)/sizeof(psz_temp[0])-1, 
                      "%s %d (%dx%d)", _("Screen"), i,
                      (int)s_rect.size.width, (int)s_rect.size.height ); 

            text.psz_string = psz_temp;
            val.i_int = i;
            var_Change( p_vout, "video-device",
                        VLC_VAR_ADDCHOICE, &val, &text );

            if( ( i - 1 ) == i_option )
            {
                var_Set( p_vout, "video-device", val );
            }
            i++;
        }

        var_AddCallback( p_vout, "video-device", vout_VarCallback,
                         NULL );

        val.b_bool = VLC_TRUE;
        var_Set( p_vout, "intf-change", val );
    }
    [o_pool release];

    if( CoCreateWindow( p_vout ) )
    {
        msg_Err( p_vout, "unable to create window" );
        if( !p_vout->p_sys->i_opengl )
        {
            free( p_vout->p_sys->p_matrix );
            DisposeHandle( (Handle)p_vout->p_sys->h_img_descr );
        }
        free( p_vout->p_sys ); 
        return( 1 );
    }

    p_vout->pf_init = vout_Init;
    p_vout->pf_end = vout_End;
    p_vout->pf_manage = vout_Manage;
    p_vout->pf_render = NULL;
    p_vout->pf_display = vout_Display;

    return( 0 );
}

/*****************************************************************************
 * vout_Init: initialize video thread output method
 *****************************************************************************/
static int vout_InitOpenGL( vout_thread_t *p_vout );
static int vout_InitQuickTime( vout_thread_t *p_vout );
static int vout_Init( vout_thread_t *p_vout )
{
    I_OUTPUTPICTURES = 0;

    /* Initialize the output structure; we already found a codec,
     * and the corresponding chroma we will be using. Since we can
     * arbitrary scale, stick to the coordinates and aspect. */
    p_vout->output.i_width  = p_vout->render.i_width;
    p_vout->output.i_height = p_vout->render.i_height;
    p_vout->output.i_aspect = p_vout->render.i_aspect;

    if( p_vout->p_sys->i_opengl )
    {
        return vout_InitOpenGL( p_vout );
    }
    else
    {
        return vout_InitQuickTime( p_vout );    
    }
}

static int vout_InitOpenGL( vout_thread_t *p_vout )
{
    picture_t *p_pic;
    int i_bytes;
    int i_index;
    
    /* Apple OpenGL extensions only accept YUV as YUY2 */
    p_vout->output.i_chroma = VLC_FOURCC('Y','U','Y','2');
    p_vout->output.i_rmask  = 0xFF0000;
    p_vout->output.i_gmask  = 0x00FF00;
    p_vout->output.i_bmask  = 0x0000FF;

    /* Allocate our 2 picture buffers */
    i_bytes = 2 * p_vout->output.i_width * p_vout->output.i_height;
    p_vout->p_sys->p_data[0] = vlc_memalign(
            &p_vout->p_sys->p_data_orig[0], 16, i_bytes );
    p_vout->p_sys->p_data[1] = vlc_memalign(
            &p_vout->p_sys->p_data_orig[1], 16, i_bytes );
    p_vout->p_sys->i_cur_pic = 1;

    /* We declare only one picture and will switch buffers
       manually */
    p_pic = NULL;
    while( I_OUTPUTPICTURES < 1 )
    {
        /* Find an empty picture slot */
        for( i_index = 0; i_index < VOUT_MAX_PICTURES; i_index++ )
        {
            if( p_vout->p_picture[ i_index ].i_status == FREE_PICTURE )
            {
                p_pic = p_vout->p_picture + i_index;
                break;
            }
        }
        if( p_pic == NULL )
        {
            break;
        }

        vout_InitPicture( VLC_OBJECT( p_vout ), p_pic,
                p_vout->output.i_chroma, p_vout->output.i_width,
                p_vout->output.i_height, p_vout->output.i_aspect );
        p_pic->p_data = p_vout->p_sys->p_data[0];
        p_pic->p[0].p_pixels = p_pic->p_data;
        for( i_index = 1; i_index < p_pic->i_planes; i_index++ )
        {
            p_pic->p[i_index].p_pixels =
                p_pic->p[i_index-1].p_pixels +
                p_pic->p[i_index-1].i_lines *
                p_pic->p[i_index-1].i_pitch;
        }

        p_pic->i_status = DESTROYED_PICTURE;
        p_pic->i_type   = DIRECT_PICTURE;

        PP_OUTPUTPICTURE[ I_OUTPUTPICTURES ] = p_pic;
        I_OUTPUTPICTURES++;
    }

    [p_vout->p_sys->o_glview lockFocus];
    [p_vout->p_sys->o_glview initTextures];
    [p_vout->p_sys->o_glview reshape];
    [p_vout->p_sys->o_glview unlockFocus];

    return 0;
}

static int vout_InitQuickTime( vout_thread_t *p_vout )
{
    picture_t *p_pic;
    int i_index;

    SetPort( p_vout->p_sys->p_qdport );
    QTScaleMatrix( p_vout );

    if( QTCreateSequence( p_vout ) )
    {
        msg_Err( p_vout, "unable to create sequence" );
        return( 1 );
    }

    /* Try to initialize up to QT_MAX_DIRECTBUFFERS direct buffers */
    while( I_OUTPUTPICTURES < QT_MAX_DIRECTBUFFERS )
    {
        p_pic = NULL;

        /* Find an empty picture slot */
        for( i_index = 0; i_index < VOUT_MAX_PICTURES; i_index++ )
        {
            if( p_vout->p_picture[ i_index ].i_status == FREE_PICTURE )
            {
                p_pic = p_vout->p_picture + i_index;
                break;
            }
        }
        if( p_pic == NULL )
        {
            break;
        }

        /* Allocate the picture */
        if( QTNewPicture( p_vout, p_pic ) )
        {
            break;
        }

        p_pic->i_status = DESTROYED_PICTURE;
        p_pic->i_type   = DIRECT_PICTURE;

        PP_OUTPUTPICTURE[ I_OUTPUTPICTURES ] = p_pic;
        I_OUTPUTPICTURES++;
    }
    return 0;
}

/*****************************************************************************
 * vout_End: terminate video thread output method
 *****************************************************************************/
static void vout_End( vout_thread_t *p_vout )
{
    int i_index;

    if( !p_vout->p_sys->i_opengl )
    {
        QTDestroySequence( p_vout );
    }

    /* Free the direct buffers we allocated */
    for( i_index = I_OUTPUTPICTURES; i_index; )
    {
        i_index--;
        if( !p_vout->p_sys->i_opengl )
        {
            QTFreePicture( p_vout, PP_OUTPUTPICTURE[ i_index ] );
        }
        else
        {
            free( p_vout->p_sys->p_data_orig[0] );
            free( p_vout->p_sys->p_data_orig[1] );
        }
    }
}

/*****************************************************************************
 * CloseVideo: destroy video thread output method
 *****************************************************************************/
void E_(CloseVideo) ( vlc_object_t *p_this )
{       
    vout_thread_t * p_vout = (vout_thread_t *)p_this;     

    if( p_vout->p_sys->i_opengl )
    {
        [p_vout->p_sys->o_glview cleanUp];
    }

    if( CoDestroyWindow( p_vout ) )
    {
        msg_Err( p_vout, "unable to destroy window" );
    }

    if ( p_vout->p_sys->p_fullscreen_state != NULL )
    {
        EndFullScreen ( p_vout->p_sys->p_fullscreen_state, NULL );
    }

    if( !p_vout->p_sys->i_opengl )
    {
        ExitMovies();
        free( p_vout->p_sys->p_matrix );
        DisposeHandle( (Handle)p_vout->p_sys->h_img_descr );
    }

    free( p_vout->p_sys );
}

/*****************************************************************************
 * vout_Manage: handle events
 *****************************************************************************
 * This function should be called regularly by video output thread. It manages
 * console events. It returns a non null value on error.
 *****************************************************************************/
static int vout_Manage( vout_thread_t *p_vout )
{
    if( p_vout->i_changes & VOUT_FULLSCREEN_CHANGE )
    {
        if( CoToggleFullscreen( p_vout ) )  
        {
            return( 1 );
        }

        p_vout->i_changes &= ~VOUT_FULLSCREEN_CHANGE;
    }

    if( p_vout->i_changes & VOUT_SIZE_CHANGE ) 
    {
        if( !p_vout->p_sys->i_opengl )
        {
            QTScaleMatrix( p_vout );
            SetDSequenceMatrix( p_vout->p_sys->i_seq, 
                                p_vout->p_sys->p_matrix );
        }
 
        p_vout->i_changes &= ~VOUT_SIZE_CHANGE;
    }

    /* hide/show mouse cursor 
     * this code looks unnecessarily complicated, but is necessary like this.
     * it has to deal with multiple monitors and therefore checks a lot */
    if( !p_vout->p_sys->b_mouse_moved && p_vout->b_fullscreen )
    {
        if( mdate() - p_vout->p_sys->i_time_mouse_last_moved > 3000000 )
        {
            VLCHideMouse( p_vout, YES );
        }
    }
    else if ( p_vout->p_sys->b_mouse_moved && p_vout->b_fullscreen )
    {
        VLCHideMouse( p_vout, NO );
    }
    
    /* disable screen saver */
    UpdateSystemActivity( UsrActivity );
    
    return( 0 );
}

/*****************************************************************************
 * vout_Display: displays previously rendered output
 *****************************************************************************
 * This function sends the currently rendered image to the display.
 *****************************************************************************/
static void vout_Display( vout_thread_t *p_vout, picture_t *p_pic )
{
    if( !p_vout->p_sys->i_opengl )
    {
        OSErr err;
        CodecFlags flags;

        if( ( err = DecompressSequenceFrameS( 
                        p_vout->p_sys->i_seq,
                        p_pic->p_sys->p_info,
                        p_pic->p_sys->i_size,                    
                        codecFlagUseImageBuffer, &flags, nil ) != noErr ) )
        {
            msg_Warn( p_vout, "DecompressSequenceFrameS failed: %d", err );
        }
        else
        {
            QDFlushPortBuffer( p_vout->p_sys->p_qdport, nil );
        }
    }
    else
    {
        if( [p_vout->p_sys->o_glview lockFocusIfCanDraw] )
        {
            int i_index;
            int i_old = p_vout->p_sys->i_cur_pic;
            int i_new = ( i_old + 1 ) % 2;
 
            /* Draw the new picture */
            p_vout->p_sys->i_cur_pic = i_new;
            [p_vout->p_sys->o_glview drawRect:
                [p_vout->p_sys->o_glview bounds]];
           
            /* Reload the other texture. Textures have to be reloaded
               before the buffer is filled (thanks to gcc from
               arstechnica forums) */
            [p_vout->p_sys->o_glview reloadTexture: i_old];

            /* Switch buffers */
            p_pic->p_data = p_vout->p_sys->p_data[i_old];
            p_pic->p[0].p_pixels = p_pic->p_data;
            for( i_index = 1; i_index < p_pic->i_planes; i_index++ )
            {
                p_pic->p[i_index].p_pixels =
                    p_pic->p[i_index-1].p_pixels +
                    p_pic->p[i_index-1].i_lines *
                    p_pic->p[i_index-1].i_pitch;
            }
            [p_vout->p_sys->o_glview unlockFocus];
        }
    }
}

/*****************************************************************************
 * CoSendRequest: send request to interface thread
 *****************************************************************************
 * Returns 0 on success, 1 otherwise
 *****************************************************************************/
static int CoSendRequest( vout_thread_t *p_vout, SEL sel )
{
    int i_ret = 0;
    vlc_value_t val;
    intf_thread_t * p_intf;

    VLCVout * o_vlv = [[VLCVout alloc] init];

    if( ( i_ret = ExecuteOnMainThread( o_vlv, sel, (void *)p_vout ) ) )
    {
        msg_Err( p_vout, "SendRequest: no way to communicate with mt" );
    }

    [o_vlv release];

    /*This makes this function dependant of the presence of a macosx 
    interface. We do not check if this interface exists, since it has 
    already been done before.*/

    p_intf = [NSApp getIntf];

    val.b_bool = VLC_TRUE;
    var_Create(p_intf,"intf-change",VLC_VAR_BOOL);
    var_Set(p_intf, "intf-change",val);

    return( i_ret );
}

/*****************************************************************************
 * CoCreateWindow: create new window 
 *****************************************************************************
 * Returns 0 on success, 1 otherwise
 *****************************************************************************/
static int CoCreateWindow( vout_thread_t *p_vout )
{
    if( CoSendRequest( p_vout, @selector(createWindow:) ) )
    {
        msg_Err( p_vout, "CoSendRequest (createWindow) failed" );
        return( 1 );
    }
    
    return( 0 );
}

/*****************************************************************************
 * CoDestroyWindow: destroy window 
 *****************************************************************************
 * Returns 0 on success, 1 otherwise
 *****************************************************************************/
static int CoDestroyWindow( vout_thread_t *p_vout )
{

    VLCHideMouse( p_vout, NO );

    if( CoSendRequest( p_vout, @selector(destroyWindow:) ) )
    {
        msg_Err( p_vout, "CoSendRequest (destroyWindow) failed" );
        return( 1 );
    }

    return( 0 );
}

/*****************************************************************************
 * CoToggleFullscreen: toggle fullscreen 
 *****************************************************************************
 * Returns 0 on success, 1 otherwise
 *****************************************************************************/
static int CoToggleFullscreen( vout_thread_t *p_vout )
{
    if( !p_vout->p_sys->i_opengl )
    {
        QTDestroySequence( p_vout );
    }

    if( CoDestroyWindow( p_vout ) )
    {
        msg_Err( p_vout, "unable to destroy window" );
        return( 1 );
    }
    
    p_vout->b_fullscreen = !p_vout->b_fullscreen;

    if( CoCreateWindow( p_vout ) )
    {
        msg_Err( p_vout, "unable to create window" );
        return( 1 );
    }

    if( p_vout->p_sys->i_opengl )
    {
        [p_vout->p_sys->o_glview lockFocus];
        [p_vout->p_sys->o_glview initTextures];
        [p_vout->p_sys->o_glview reshape];
        [p_vout->p_sys->o_glview drawRect:
            [p_vout->p_sys->o_glview bounds]];
        [p_vout->p_sys->o_glview unlockFocus];
    }
    else
    {
        SetPort( p_vout->p_sys->p_qdport );
        QTScaleMatrix( p_vout );

        if( QTCreateSequence( p_vout ) )
        {
            msg_Err( p_vout, "unable to create sequence" );
            return( 1 ); 
        } 
    }

    return( 0 );
}

/*****************************************************************************
 * VLCHideMouse: if b_hide then hide the cursor
 *****************************************************************************/
static void VLCHideMouse ( vout_thread_t *p_vout, BOOL b_hide )
{
    BOOL b_inside;
    NSRect s_rect;
    NSPoint ml;
    NSWindow *o_window = p_vout->p_sys->o_window;
    NSView *o_contents = [o_window contentView];
    
    s_rect = [o_contents bounds];
    ml = [o_window convertScreenToBase:[NSEvent mouseLocation]];
    ml = [o_contents convertPoint:ml fromView:nil];
    b_inside = [o_contents mouse: ml inRect: s_rect];
    
    if ( b_hide && b_inside )
    {
        /* only hide if mouse over VLCQTView */
        [NSCursor setHiddenUntilMouseMoves: YES];
    }
    else if ( !b_hide )
    {
        [NSCursor setHiddenUntilMouseMoves: NO];
    }
    p_vout->p_sys->b_mouse_moved = NO;
    p_vout->p_sys->i_time_mouse_last_moved = mdate();
    return;
}

/*****************************************************************************
 * QTScaleMatrix: scale matrix 
 *****************************************************************************/
static void QTScaleMatrix( vout_thread_t *p_vout )
{
    Rect s_rect;
    unsigned int i_width, i_height;
    Fixed factor_x, factor_y;
    unsigned int i_offset_x = 0;
    unsigned int i_offset_y = 0;

    GetPortBounds( p_vout->p_sys->p_qdport, &s_rect );

    i_width = s_rect.right - s_rect.left;
    i_height = s_rect.bottom - s_rect.top;

    if( config_GetInt( p_vout, "macosx-stretch" ) )
    {
        factor_x = FixDiv( Long2Fix( i_width ),
                           Long2Fix( p_vout->output.i_width ) );
        factor_y = FixDiv( Long2Fix( i_height ),
                           Long2Fix( p_vout->output.i_height ) );
                           
    }
    else if( i_height * p_vout->output.i_aspect < i_width * VOUT_ASPECT_FACTOR )
    {
        int i_adj_width = i_height * p_vout->output.i_aspect /
                          VOUT_ASPECT_FACTOR;

        factor_x = FixDiv( Long2Fix( i_adj_width ),
                           Long2Fix( p_vout->output.i_width ) );
        factor_y = FixDiv( Long2Fix( i_height ),
                           Long2Fix( p_vout->output.i_height ) );

        i_offset_x = (i_width - i_adj_width) / 2;
    }
    else
    {
        int i_adj_height = i_width * VOUT_ASPECT_FACTOR /
                           p_vout->output.i_aspect;

        factor_x = FixDiv( Long2Fix( i_width ),
                           Long2Fix( p_vout->output.i_width ) );
        factor_y = FixDiv( Long2Fix( i_adj_height ),
                           Long2Fix( p_vout->output.i_height ) );

        i_offset_y = (i_height - i_adj_height) / 2;
    }
    
    SetIdentityMatrix( p_vout->p_sys->p_matrix );

    ScaleMatrix( p_vout->p_sys->p_matrix,
                 factor_x, factor_y,
                 Long2Fix(0), Long2Fix(0) );
                 
    TranslateMatrix( p_vout->p_sys->p_matrix,
                 Long2Fix(i_offset_x), Long2Fix(i_offset_y) );
}

/*****************************************************************************
 * QTCreateSequence: create a new sequence 
 *****************************************************************************
 * Returns 0 on success, 1 otherwise
 *****************************************************************************/
static int QTCreateSequence( vout_thread_t *p_vout )
{
    OSErr err;
    ImageDescriptionPtr p_descr;

    HLock( (Handle)p_vout->p_sys->h_img_descr );
    p_descr = *p_vout->p_sys->h_img_descr;

    p_descr->idSize = sizeof(ImageDescription);
    p_descr->cType = p_vout->p_sys->i_codec;
    p_descr->version = 1;
    p_descr->revisionLevel = 0;
    p_descr->vendor = 'appl';
    p_descr->width = p_vout->output.i_width;
    p_descr->height = p_vout->output.i_height;
    p_descr->hRes = Long2Fix(72);
    p_descr->vRes = Long2Fix(72);
    p_descr->spatialQuality = codecLosslessQuality;
    p_descr->frameCount = 1;
    p_descr->clutID = -1;
    p_descr->dataSize = 0;
    p_descr->depth = 24;

    HUnlock( (Handle)p_vout->p_sys->h_img_descr );

    if( ( err = DecompressSequenceBeginS( 
                              &p_vout->p_sys->i_seq,
                              p_vout->p_sys->h_img_descr,
                              NULL, 0,
                              p_vout->p_sys->p_qdport,
                              NULL, NULL,
                              p_vout->p_sys->p_matrix,
                              0, NULL,
                              codecFlagUseImageBuffer,
                              codecLosslessQuality,
                              p_vout->p_sys->img_dc ) ) )
    {
        msg_Err( p_vout, "DecompressSequenceBeginS failed: %d", err );
        return( 1 );
    }

    return( 0 );
}

/*****************************************************************************
 * QTDestroySequence: destroy sequence 
 *****************************************************************************/
static void QTDestroySequence( vout_thread_t *p_vout )
{
    CDSequenceEnd( p_vout->p_sys->i_seq );
}

/*****************************************************************************
 * QTNewPicture: allocate a picture
 *****************************************************************************
 * Returns 0 on success, 1 otherwise
 *****************************************************************************/
static int QTNewPicture( vout_thread_t *p_vout, picture_t *p_pic )
{
    int i_width  = p_vout->output.i_width;
    int i_height = p_vout->output.i_height;

    /* We know the chroma, allocate a buffer which will be used
     * directly by the decoder */
    p_pic->p_sys = malloc( sizeof( picture_sys_t ) );

    if( p_pic->p_sys == NULL )
    {
        return( -1 );
    }

    switch( p_vout->output.i_chroma )
    {
        case VLC_FOURCC('I','4','2','0'):

            p_pic->p_sys->p_info = (void *)&p_pic->p_sys->pixmap_i420;
            p_pic->p_sys->i_size = sizeof(PlanarPixmapInfoYUV420);

            /* Allocate the memory buffer */
            p_pic->p_data = vlc_memalign( &p_pic->p_data_orig,
                                          16, i_width * i_height * 3 / 2 );

            /* Y buffer */
            p_pic->Y_PIXELS = p_pic->p_data; 
            p_pic->p[Y_PLANE].i_lines = i_height;
            p_pic->p[Y_PLANE].i_pitch = i_width;
            p_pic->p[Y_PLANE].i_pixel_pitch = 1;
            p_pic->p[Y_PLANE].i_visible_pitch = i_width;

            /* U buffer */
            p_pic->U_PIXELS = p_pic->Y_PIXELS + i_height * i_width;
            p_pic->p[U_PLANE].i_lines = i_height / 2;
            p_pic->p[U_PLANE].i_pitch = i_width / 2;
            p_pic->p[U_PLANE].i_pixel_pitch = 1;
            p_pic->p[U_PLANE].i_visible_pitch = i_width / 2;

            /* V buffer */
            p_pic->V_PIXELS = p_pic->U_PIXELS + i_height * i_width / 4;
            p_pic->p[V_PLANE].i_lines = i_height / 2;
            p_pic->p[V_PLANE].i_pitch = i_width / 2;
            p_pic->p[V_PLANE].i_pixel_pitch = 1;
            p_pic->p[V_PLANE].i_visible_pitch = i_width / 2;

            /* We allocated 3 planes */
            p_pic->i_planes = 3;

#define P p_pic->p_sys->pixmap_i420
            P.componentInfoY.offset = (void *)p_pic->Y_PIXELS
                                       - p_pic->p_sys->p_info;
            P.componentInfoCb.offset = (void *)p_pic->U_PIXELS
                                        - p_pic->p_sys->p_info;
            P.componentInfoCr.offset = (void *)p_pic->V_PIXELS
                                        - p_pic->p_sys->p_info;

            P.componentInfoY.rowBytes = i_width;
            P.componentInfoCb.rowBytes = i_width / 2;
            P.componentInfoCr.rowBytes = i_width / 2;
#undef P

            break;

    default:
        /* Unknown chroma, tell the guy to get lost */
        free( p_pic->p_sys );
        msg_Err( p_vout, "never heard of chroma 0x%.8x (%4.4s)",
                 p_vout->output.i_chroma, (char*)&p_vout->output.i_chroma );
        p_pic->i_planes = 0;
        return( -1 );
    }

    return( 0 );
}

/*****************************************************************************
 * QTFreePicture: destroy a picture allocated with QTNewPicture
 *****************************************************************************/
static void QTFreePicture( vout_thread_t *p_vout, picture_t *p_pic )
{
    switch( p_vout->output.i_chroma )
    {
        case VLC_FOURCC('I','4','2','0'):
            free( p_pic->p_data_orig );
            break;
    }

    free( p_pic->p_sys );
}

/*****************************************************************************
 * VLCWindow implementation
 *****************************************************************************/
@implementation VLCWindow

- (void)setVout:(vout_thread_t *)_p_vout
{
    p_vout = _p_vout;
}

- (vout_thread_t *)getVout
{
    return( p_vout );
}

- (void)scaleWindowWithFactor: (float)factor
{
    NSSize newsize;
    int i_corrected_height, i_corrected_width;
    NSPoint topleftbase;
    NSPoint topleftscreen;
    
    if ( !p_vout->b_fullscreen )
    {
        topleftbase.x = 0;
        topleftbase.y = [self frame].size.height;
        topleftscreen = [self convertBaseToScreen: topleftbase];
        
        if( p_vout->output.i_height * p_vout->output.i_aspect > 
                        p_vout->output.i_width * VOUT_ASPECT_FACTOR )
        {
            i_corrected_width = p_vout->output.i_height * p_vout->output.i_aspect /
                                            VOUT_ASPECT_FACTOR;
            newsize.width = (int) ( i_corrected_width * factor );
            newsize.height = (int) ( p_vout->render.i_height * factor );
        }
        else
        {
            i_corrected_height = p_vout->output.i_width * VOUT_ASPECT_FACTOR /
                                            p_vout->output.i_aspect;
            newsize.width = (int) ( p_vout->render.i_width * factor );
            newsize.height = (int) ( i_corrected_height * factor );
        }
    
        [self setContentSize: newsize];
        
        [self setFrameTopLeftPoint: topleftscreen];
        p_vout->i_changes |= VOUT_SIZE_CHANGE;
    }
}

- (void)toggleFloatOnTop
{
    vlc_value_t val;
    if( var_Get( p_vout, "video-on-top", &val )>=0 && val.b_bool)
    {
        val.b_bool = VLC_FALSE;
        var_Set( p_vout, "video-on-top", val );
        [p_vout->p_sys->o_window setLevel: NSNormalWindowLevel];
    }
    else
    {
        val.b_bool = VLC_TRUE;
        var_Set( p_vout, "video-on-top", val );
        [p_vout->p_sys->o_window setLevel: NSStatusWindowLevel];
    }
}

- (void)toggleFullscreen
{
    vlc_value_t val;
    val.b_bool = !p_vout->b_fullscreen;
    var_Set( p_vout, "fullscreen", val );
}

- (BOOL)isFullscreen
{
    return( p_vout->b_fullscreen );
}

- (BOOL)canBecomeKeyWindow
{
    return( YES );
}

- (BOOL)performKeyEquivalent:(NSEvent *)o_event
{
    return [(VLCApplication *) [VLCApplication sharedApplication]
            hasDefinedShortcutKey:o_event];
}

- (void)keyDown:(NSEvent *)o_event
{
    unichar key = 0;
    vlc_value_t val;
    unsigned int i_pressed_modifiers = 0;
    val.i_int = 0;
    
    i_pressed_modifiers = [o_event modifierFlags];

    if( i_pressed_modifiers & NSShiftKeyMask )
        val.i_int |= KEY_MODIFIER_SHIFT;
    if( i_pressed_modifiers & NSControlKeyMask )
        val.i_int |= KEY_MODIFIER_CTRL;
    if( i_pressed_modifiers & NSAlternateKeyMask )
        val.i_int |= KEY_MODIFIER_ALT;
    if( i_pressed_modifiers & NSCommandKeyMask )
        val.i_int |= KEY_MODIFIER_COMMAND;

    key = [[o_event charactersIgnoringModifiers] characterAtIndex: 0];

    if( key )
    {
        /* Escape should always get you out of fullscreen */
        if( key == (unichar) 0x1b )
        {
             if( [self isFullscreen] )
             {
                 [self toggleFullscreen];
             }
        }
        else if ( key == ' ' )
        {
             playlist_t *p_playlist = vlc_object_find( p_vout, VLC_OBJECT_PLAYLIST,
                                                     FIND_ANYWHERE );
             if ( p_playlist != NULL )
             {
                 playlist_Pause( p_playlist );
                 vlc_object_release( p_playlist);
             }
        }
        else
        {
            val.i_int |= CocoaKeyToVLC( key );
            var_Set( p_vout->p_vlc, "key-pressed", val );
        }
    }
    else
    {
        [super keyDown: o_event];
    }
}

- (void)updateTitle
{
    NSMutableString * o_title;
    playlist_t * p_playlist = vlc_object_find( p_vout, VLC_OBJECT_PLAYLIST,
                                                       FIND_ANYWHERE );
    
    if( p_playlist == NULL )
    {
        return;
    }

    vlc_mutex_lock( &p_playlist->object_lock );
    o_title = [NSMutableString stringWithUTF8String: 
        p_playlist->pp_items[p_playlist->i_index]->input.psz_uri]; 
    vlc_mutex_unlock( &p_playlist->object_lock );

    vlc_object_release( p_playlist );

    if( o_title != nil )
    {
        NSRange prefix_range = [o_title rangeOfString: @"file:"];
        if( prefix_range.location != NSNotFound )
        {
            [o_title deleteCharactersInRange: prefix_range];
        }

        [self setTitleWithRepresentedFilename: o_title];
    }
    else
    {
        [self setTitle:
            [NSString stringWithCString: VOUT_TITLE " (QuickTime)"]];
    }
}

/* This is actually the same as VLCControls::stop. */
- (BOOL)windowShouldClose:(id)sender
{
    playlist_t * p_playlist = vlc_object_find( p_vout, VLC_OBJECT_PLAYLIST,
                                                       FIND_ANYWHERE );
    if( p_playlist == NULL )      
    {
        return NO;
    }

    playlist_Stop( p_playlist );
    vlc_object_release( p_playlist );

    /* The window will be closed by the intf later. */
    return NO;
}

@end

/* Common QT and OpenGL code to catch mouse events */
#define CATCH_MOUSE_EVENTS \
- (BOOL)acceptsFirstResponder \
{ \
    return( YES ); \
} \
 \
- (BOOL)becomeFirstResponder \
{ \
    id o_window = [self window]; \
     \
    [o_window setAcceptsMouseMovedEvents: YES]; \
    return( YES ); \
} \
 \
- (BOOL)resignFirstResponder \
{ \
    vout_thread_t * vout; \
    id o_window = [self window]; \
    vout = (vout_thread_t *)[o_window getVout]; \
     \
    [o_window setAcceptsMouseMovedEvents: NO]; \
    VLCHideMouse( vout, NO ); \
    return( YES ); \
} \
 \
- (void)mouseDown:(NSEvent *)o_event \
{ \
    vout_thread_t * vout; \
    id o_window = [self window]; \
    vout = (vout_thread_t *)[o_window getVout]; \
    vlc_value_t val; \
 \
    switch( [o_event type] ) \
    {         \
        case NSLeftMouseDown: \
        { \
            var_Get( vout, "mouse-button-down", &val ); \
            val.i_int |= 1; \
            var_Set( vout, "mouse-button-down", val ); \
        } \
        break; \
         \
        default: \
            [super mouseDown: o_event]; \
        break; \
    } \
} \
 \
- (void)otherMouseDown:(NSEvent *)o_event \
{ \
    vout_thread_t * vout; \
    id o_window = [self window]; \
    vout = (vout_thread_t *)[o_window getVout]; \
    vlc_value_t val; \
 \
    switch( [o_event type] ) \
    { \
        case NSOtherMouseDown: \
        { \
            var_Get( vout, "mouse-button-down", &val ); \
            val.i_int |= 2; \
            var_Set( vout, "mouse-button-down", val ); \
        } \
        break; \
         \
        default: \
            [super mouseDown: o_event]; \
        break; \
    } \
} \
 \
- (void)rightMouseDown:(NSEvent *)o_event \
{ \
    vout_thread_t * vout; \
    id o_window = [self window]; \
    vout = (vout_thread_t *)[o_window getVout]; \
    vlc_value_t val; \
 \
    switch( [o_event type] ) \
    { \
        case NSRightMouseDown: \
        { \
            var_Get( vout, "mouse-button-down", &val ); \
            val.i_int |= 4; \
            var_Set( vout, "mouse-button-down", val ); \
        } \
        break; \
         \
        default: \
            [super mouseDown: o_event]; \
        break; \
    } \
} \
 \
- (void)mouseUp:(NSEvent *)o_event \
{ \
    vout_thread_t * vout; \
    id o_window = [self window]; \
    vout = (vout_thread_t *)[o_window getVout]; \
    vlc_value_t val; \
 \
    switch( [o_event type] ) \
    { \
        case NSLeftMouseUp: \
        { \
            vlc_value_t b_val; \
            b_val.b_bool = VLC_TRUE; \
            var_Set( vout, "mouse-clicked", b_val ); \
             \
            var_Get( vout, "mouse-button-down", &val ); \
            val.i_int &= ~1; \
            var_Set( vout, "mouse-button-down", val ); \
        } \
        break; \
                 \
        default: \
            [super mouseUp: o_event]; \
        break; \
    } \
} \
 \
- (void)otherMouseUp:(NSEvent *)o_event \
{ \
    vout_thread_t * vout; \
    id o_window = [self window]; \
    vout = (vout_thread_t *)[o_window getVout]; \
    vlc_value_t val; \
 \
    switch( [o_event type] ) \
    { \
        case NSOtherMouseUp: \
        { \
            var_Get( vout, "mouse-button-down", &val ); \
            val.i_int &= ~2; \
            var_Set( vout, "mouse-button-down", val ); \
        } \
        break; \
                 \
        default: \
            [super mouseUp: o_event]; \
        break; \
    } \
} \
 \
- (void)rightMouseUp:(NSEvent *)o_event \
{ \
    vout_thread_t * vout; \
    id o_window = [self window]; \
    vout = (vout_thread_t *)[o_window getVout]; \
    vlc_value_t val; \
 \
    switch( [o_event type] ) \
    { \
        case NSRightMouseUp: \
        { \
            var_Get( vout, "mouse-button-down", &val ); \
            val.i_int &= ~4; \
            var_Set( vout, "mouse-button-down", val ); \
        } \
        break; \
         \
        default: \
            [super mouseUp: o_event]; \
        break; \
    } \
} \
 \
- (void)mouseDragged:(NSEvent *)o_event \
{ \
    [self mouseMoved:o_event]; \
} \
 \
- (void)otherMouseDragged:(NSEvent *)o_event \
{ \
    [self mouseMoved:o_event]; \
} \
 \
- (void)rightMouseDragged:(NSEvent *)o_event \
{ \
    [self mouseMoved:o_event]; \
}

/*****************************************************************************
 * VLCQTView implementation
 *****************************************************************************/
@implementation VLCQTView

- (void)drawRect:(NSRect)rect
{
    vout_thread_t * p_vout;
    id o_window = [self window];
    p_vout = (vout_thread_t *)[o_window getVout];
    
    [[NSColor blackColor] set];
    NSRectFill( rect );
    [super drawRect: rect];

    p_vout->i_changes |= VOUT_SIZE_CHANGE;
}

CATCH_MOUSE_EVENTS

- (void)mouseMoved:(NSEvent *)o_event
{
    NSPoint ml;
    NSRect s_rect;
    BOOL b_inside;

    vout_thread_t * p_vout;
    id o_window = [self window];
    p_vout = (vout_thread_t *)[o_window getVout];

    s_rect = [self bounds];
    ml = [self convertPoint: [o_event locationInWindow] fromView: nil];
    b_inside = [self mouse: ml inRect: s_rect];

    if( b_inside )
    {
        vlc_value_t val;
        int i_width, i_height, i_x, i_y;
        
        vout_PlacePicture( p_vout, (unsigned int)s_rect.size.width,
                                   (unsigned int)s_rect.size.height,
                                   &i_x, &i_y, &i_width, &i_height );

        val.i_int = ( ((int)ml.x) - i_x ) * 
                    p_vout->render.i_width / i_width;
        var_Set( p_vout, "mouse-x", val );

        val.i_int = ( ((int)ml.y) - i_y ) * 
                    p_vout->render.i_height / i_height;
        var_Set( p_vout, "mouse-y", val );
            
        val.b_bool = VLC_TRUE;
        var_Set( p_vout, "mouse-moved", val );
        p_vout->p_sys->i_time_mouse_last_moved = mdate();
        p_vout->p_sys->b_mouse_moved = YES;
    }

    [super mouseMoved: o_event];
}

@end

/*****************************************************************************
 * VLCGLView implementation
 *****************************************************************************/
@implementation VLCGLView


- (id) initWithFrame: (NSRect) frame vout: (vout_thread_t*) _p_vout
{
    char * psz_effect;
    p_vout = _p_vout;
    
    NSOpenGLPixelFormatAttribute attribs[] =
    {
        NSOpenGLPFAAccelerated,
        NSOpenGLPFANoRecovery,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAWindow,
        0
    };

    NSOpenGLPixelFormat * fmt = [[NSOpenGLPixelFormat alloc]
        initWithAttributes: attribs];

    if( !fmt )
    {
        msg_Warn( p_vout, "Cannot create NSOpenGLPixelFormat" );
        return nil;
    }

    self = [super initWithFrame:frame pixelFormat: fmt];
    [fmt release];

    [[self openGLContext] makeCurrentContext];
    [[self openGLContext] update];

    /* Black background */
    glClearColor( 0.0, 0.0, 0.0, 0.0 );

    /* Check if the user asked for useless visual effects */
    psz_effect = config_GetPsz( p_vout, "macosx-opengl-effect" );
    if( !psz_effect || !strcmp( psz_effect, "none" ))
    {
        i_effect = OPENGL_EFFECT_NONE;
    }
    else if( !strcmp( psz_effect, "cube" ) )
    {
        i_effect = OPENGL_EFFECT_CUBE;

        glEnable( GL_DEPTH_TEST );
    }
    else if( !strcmp( psz_effect, "transparent-cube" ) )
    {
        i_effect = OPENGL_EFFECT_TRANSPARENT_CUBE;

        glDisable( GL_DEPTH_TEST );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    }
    else
    {
        msg_Warn( p_vout, "no valid opengl effect provided, using "
                  "\"none\"" );
        i_effect = OPENGL_EFFECT_NONE;
    }

    if( i_effect & ( OPENGL_EFFECT_CUBE |
                OPENGL_EFFECT_TRANSPARENT_CUBE ) )
    {
        /* Set the perpective */
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glFrustum( -1.0, 1.0, -1.0, 1.0, 3.0, 20.0 );
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glTranslatef( 0.0, 0.0, - 5.0 );
    }

    pi_textures[0] = 0;
    pi_textures[1] = 0;
    initDone       = 0;

    return self;
}

- (void) reshape
{
    if( !initDone )
    {
        return;
    }
    
    [[self openGLContext] makeCurrentContext];

    NSRect bounds = [self bounds];
    glViewport( 0, 0, (GLint) bounds.size.width,
                (GLint) bounds.size.height );

    if( config_GetInt( p_vout, "macosx-stretch" ) )
    {
        f_x = 1.0;
        f_y = 1.0;
    }
    else
    {
        /* Quad size is set in order to preserve the aspect ratio */
        int fill = ( config_GetInt( p_vout, "macosx-fill" ) &&
                     p_vout->b_fullscreen );
        int large = ( bounds.size.height * p_vout->output.i_aspect <
                      bounds.size.width * VOUT_ASPECT_FACTOR );
        if( ( large && !fill ) || ( !large && fill ) )
        {
            f_x = bounds.size.height * p_vout->output.i_aspect /
                VOUT_ASPECT_FACTOR / bounds.size.width;
            f_y = 1.0;
        }
        else
        {
            f_x = 1.0;
            f_y = bounds.size.width * VOUT_ASPECT_FACTOR /
                p_vout->output.i_aspect / bounds.size.height;
        }
    }
}

- (void) initTextures
{
    int i;
    [[self openGLContext] makeCurrentContext];

    /* Free previous texture if any */
    if( initDone )
    {
        glDeleteTextures( 2, pi_textures );
    }

    glEnable( GL_TEXTURE_RECTANGLE_EXT );
    glEnable( GL_UNPACK_CLIENT_STORAGE_APPLE );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, p_vout->output.i_width );

    /* Tell the driver not to make a copy of the texture but to use
       our buffer */
    glPixelStorei( GL_UNPACK_CLIENT_STORAGE_APPLE, GL_TRUE );

    /* Create textures */
    glGenTextures( 2, pi_textures );

    for( i = 0; i < 2; i++ )
    {
        glBindTexture( GL_TEXTURE_RECTANGLE_EXT, pi_textures[i] );
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

        /* Linear interpolation */
        glTexParameteri( GL_TEXTURE_RECTANGLE_EXT,
                GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_RECTANGLE_EXT,
                GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        /* Use VRAM texturing */
        glTexParameteri( GL_TEXTURE_RECTANGLE_EXT,
                GL_TEXTURE_STORAGE_HINT_APPLE, GL_STORAGE_CACHED_APPLE );

        glTexImage2D( GL_TEXTURE_RECTANGLE_EXT, 0, GL_RGB8,
                p_vout->output.i_width, p_vout->output.i_height, 0,
                GL_YCBCR_422_APPLE, GL_UNSIGNED_SHORT_8_8_APPLE,
                p_vout->p_sys->p_data[i] );
    }

    /* Swap buffers only during the vertical retrace of the monitor.
       http://developer.apple.com/documentation/GraphicsImaging/
       Conceptual/OpenGL/chap5/chapter_5_section_44.html */
    long params[] = { 1 };
    CGLSetParameter( CGLGetCurrentContext(), kCGLCPSwapInterval,
                     params );

    initDone = 1;
}

- (void)reloadTexture: (int) index
{
    if( !initDone )
    {
        return;
    }
    
    [[self openGLContext] makeCurrentContext];

    glBindTexture( GL_TEXTURE_RECTANGLE_EXT, pi_textures[index] );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, p_vout->output.i_width );

    /* glTexSubImage2D is faster than glTexImage2D
       http://developer.apple.com/samplecode/Sample_Code/Graphics_3D/
       TextureRange/MainOpenGLView.m.htm */
    glTexSubImage2D( GL_TEXTURE_RECTANGLE_EXT, 0, 0, 0,
            p_vout->output.i_width, p_vout->output.i_height,
            GL_YCBCR_422_APPLE, GL_UNSIGNED_SHORT_8_8_APPLE,
            p_vout->p_sys->p_data[index] );
}

- (void) cleanUp
{
    initDone = 0;
}

- (void) drawQuad
{
    glBegin( GL_QUADS );
        /* Top left */
        glTexCoord2f( 0.0, 0.0 );
        glVertex2f( - f_x, f_y );
        /* Bottom left */
        glTexCoord2f( 0.0, (float) p_vout->output.i_height );
        glVertex2f( - f_x, - f_y );
        /* Bottom right */
        glTexCoord2f( (float) p_vout->output.i_width,
                      (float) p_vout->output.i_height );
        glVertex2f( f_x, - f_y );
        /* Top right */
        glTexCoord2f( (float) p_vout->output.i_width, 0.0 );
        glVertex2f( f_x, f_y );
    glEnd();
}

- (void) drawCube
{
    glBegin( GL_QUADS );
        /* Front */
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f( - 1.0, 1.0, 1.0 );
        glTexCoord2f( 0.0, (float) p_vout->output.i_height );
        glVertex3f( - 1.0, - 1.0, 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width,
                      (float) p_vout->output.i_height );
        glVertex3f( 1.0, - 1.0, 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width, 0.0 );
        glVertex3f( 1.0, 1.0, 1.0 );

        /* Left */
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f( - 1.0, 1.0, - 1.0 );
        glTexCoord2f( 0.0, (float) p_vout->output.i_height );
        glVertex3f( - 1.0, - 1.0, - 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width,
                      (float) p_vout->output.i_height );
        glVertex3f( - 1.0, - 1.0, 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width, 0.0 );
        glVertex3f( - 1.0, 1.0, 1.0 );

        /* Back */
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f( 1.0, 1.0, - 1.0 );
        glTexCoord2f( 0.0, (float) p_vout->output.i_height );
        glVertex3f( 1.0, - 1.0, - 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width,
                      (float) p_vout->output.i_height );
        glVertex3f( - 1.0, - 1.0, - 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width, 0.0 );
        glVertex3f( - 1.0, 1.0, - 1.0 );

        /* Right */
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f( 1.0, 1.0, 1.0 );
        glTexCoord2f( 0.0, (float) p_vout->output.i_height );
        glVertex3f( 1.0, - 1.0, 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width,
                      (float) p_vout->output.i_height );
        glVertex3f( 1.0, - 1.0, - 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width, 0.0 );
        glVertex3f( 1.0, 1.0, - 1.0 );

        /* Top */
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f( - 1.0, 1.0, - 1.0 );
        glTexCoord2f( 0.0, (float) p_vout->output.i_height );
        glVertex3f( - 1.0, 1.0, 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width,
                      (float) p_vout->output.i_height );
        glVertex3f( 1.0, 1.0, 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width, 0.0 );
        glVertex3f( 1.0, 1.0, - 1.0 );

        /* Bottom */
        glTexCoord2f( 0.0, 0.0 );
        glVertex3f( - 1.0, - 1.0, 1.0 );
        glTexCoord2f( 0.0, (float) p_vout->output.i_height );
        glVertex3f( - 1.0, - 1.0, - 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width,
                      (float) p_vout->output.i_height );
        glVertex3f( 1.0, - 1.0, - 1.0 );
        glTexCoord2f( (float) p_vout->output.i_width, 0.0 );
        glVertex3f( 1.0, - 1.0, 1.0 );
    glEnd();
}

- (void) drawRect: (NSRect) rect
{
    [[self openGLContext] makeCurrentContext];

    /* Black background */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if( !initDone )
    {
        glFlush();
        return;
    }

    /* Draw */
    glBindTexture( GL_TEXTURE_RECTANGLE_EXT,
                   pi_textures[p_vout->p_sys->i_cur_pic] );
    if( i_effect & ( OPENGL_EFFECT_CUBE |
                OPENGL_EFFECT_TRANSPARENT_CUBE ) )
    {
        glRotatef( 1.0, 0.3, 0.5, 0.7 );
        [self drawCube];
    }
    else
    {
        [self drawQuad];
    }

    /* Draw */
    glFlush();
}

CATCH_MOUSE_EVENTS

- (void)mouseMoved:(NSEvent *)o_event
{
    NSPoint ml;
    NSRect s_rect;
    BOOL b_inside;

    s_rect = [self bounds];
    ml = [self convertPoint: [o_event locationInWindow] fromView: nil];
    b_inside = [self mouse: ml inRect: s_rect];

    if( b_inside )
    {
        vlc_value_t val;
        int i_width, i_height, i_x, i_y;
        
        vout_PlacePicture( p_vout, (unsigned int)s_rect.size.width,
                                   (unsigned int)s_rect.size.height,
                                   &i_x, &i_y, &i_width, &i_height );

        val.i_int = ( (int)ml.x - i_x ) * 
                    p_vout->render.i_width / i_width;
        var_Set( p_vout, "mouse-x", val );

        /* Y coordinate is inverted in OpenGL */
        val.i_int = ( ((int)(s_rect.size.height - ml.y)) - i_y ) *
                    p_vout->render.i_height / i_height;
        var_Set( p_vout, "mouse-y", val );
            
        val.b_bool = VLC_TRUE;
        var_Set( p_vout, "mouse-moved", val );
        p_vout->p_sys->i_time_mouse_last_moved = mdate();
        p_vout->p_sys->b_mouse_moved = YES;
    }

    [super mouseMoved: o_event];
}

@end

/*****************************************************************************
 * VLCVout implementation
 *****************************************************************************/
@implementation VLCVout

- (void)createWindow:(NSValue *)o_value
{
    vlc_value_t val;
    VLCQTView * o_view;
    NSScreen * o_screen;
    vout_thread_t * p_vout;
    vlc_bool_t b_main_screen;
    
    p_vout = (vout_thread_t *)[o_value pointerValue];

    p_vout->p_sys->o_window = [VLCWindow alloc];
    [p_vout->p_sys->o_window setVout: p_vout];
    [p_vout->p_sys->o_window setReleasedWhenClosed: YES];

    if( var_Get( p_vout, "video-device", &val ) < 0 )
    {
        o_screen = [NSScreen mainScreen];
        b_main_screen = 1;
    }
    else
    {
        NSArray *o_screens = [NSScreen screens];
        unsigned int i_index = val.i_int;
        
        if( [o_screens count] < i_index )
        {
            o_screen = [NSScreen mainScreen];
            b_main_screen = 1;
        }
        else
        {
            i_index--;
            o_screen = [o_screens objectAtIndex: i_index];
            config_PutInt( p_vout, "macosx-vdev", i_index );
            b_main_screen = (i_index == 0);
        }
    } 

    if( p_vout->b_fullscreen )
    {
        NSRect screen_rect = [o_screen frame];
        screen_rect.origin.x = screen_rect.origin.y = 0;

        if ( b_main_screen && p_vout->p_sys->p_fullscreen_state == NULL )
            BeginFullScreen( &p_vout->p_sys->p_fullscreen_state, NULL, 0, 0,
                             NULL, NULL, fullScreenAllowEvents );

        [p_vout->p_sys->o_window 
            initWithContentRect: screen_rect
            styleMask: NSBorderlessWindowMask
            backing: NSBackingStoreBuffered
            defer: NO screen: o_screen];

        //[p_vout->p_sys->o_window setLevel: NSPopUpMenuWindowLevel - 1];
        p_vout->p_sys->b_mouse_moved = YES;
        p_vout->p_sys->i_time_mouse_last_moved = mdate();
    }
    else
    {
        unsigned int i_stylemask = NSTitledWindowMask |
                                   NSMiniaturizableWindowMask |
                                   NSClosableWindowMask |
                                   NSResizableWindowMask;
        
        if ( p_vout->p_sys->p_fullscreen_state != NULL )
            EndFullScreen ( p_vout->p_sys->p_fullscreen_state, NULL );
        p_vout->p_sys->p_fullscreen_state = NULL;

        [p_vout->p_sys->o_window 
            initWithContentRect: p_vout->p_sys->s_rect
            styleMask: i_stylemask
            backing: NSBackingStoreBuffered
            defer: NO screen: o_screen];

        [p_vout->p_sys->o_window setAlphaValue: config_GetFloat( p_vout, "macosx-opaqueness" )];
        
        if( config_GetInt( p_vout, "video-on-top" ) )
        {
            [p_vout->p_sys->o_window setLevel: NSStatusWindowLevel];
        }
        
        if( !p_vout->p_sys->b_pos_saved )   
        {
            [p_vout->p_sys->o_window center];
        }
    }

    if( !p_vout->p_sys->i_opengl )
    {
        o_view = [[VLCQTView alloc] init];
        /* FIXME: [o_view setMenu:] */
        [p_vout->p_sys->o_window setContentView: o_view];
        [o_view autorelease];

        [o_view lockFocus];
        p_vout->p_sys->p_qdport = [o_view qdPort];
        [o_view unlockFocus];
    }
    else
    {
#define o_glview p_vout->p_sys->o_glview
        o_glview = [[VLCGLView alloc] initWithFrame: p_vout->p_sys->s_rect vout: p_vout];
        [p_vout->p_sys->o_window setContentView: o_glview];
        [o_glview autorelease];
#undef o_glview
    }
    
    [p_vout->p_sys->o_window updateTitle];
    [p_vout->p_sys->o_window makeKeyAndOrderFront: nil];

}

- (void)destroyWindow:(NSValue *)o_value
{
    vout_thread_t * p_vout;

    p_vout = (vout_thread_t *)[o_value pointerValue];

    if( !p_vout->b_fullscreen )
    {
        NSRect s_rect;

        s_rect = [[p_vout->p_sys->o_window contentView] frame];
        p_vout->p_sys->s_rect.size = s_rect.size;

        s_rect = [p_vout->p_sys->o_window frame];
        p_vout->p_sys->s_rect.origin = s_rect.origin;

        p_vout->p_sys->b_pos_saved = YES;
    }
    
    p_vout->p_sys->p_qdport = nil;
    [p_vout->p_sys->o_window close];
    p_vout->p_sys->o_window = nil;
}

@end
