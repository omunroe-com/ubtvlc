/*****************************************************************************
 * mosaic.c : Mosaic video plugin for vlc
 *****************************************************************************
 * Copyright (C) 2004-2005 the VideoLAN team
 * $Id: mosaic.c 11664 2005-07-09 06:17:09Z courmisch $
 *
 * Authors: Antoine Cellerier <dionoea@via.ecp.fr>
 *          Christophe Massiot <massiot@via.ecp.fr>
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
#include <math.h>

#include <vlc/vlc.h>
#include <vlc/vout.h>

#ifdef HAVE_LIMITS_H
#   include <limits.h> /* INT_MAX */
#endif

#include "vlc_filter.h"
#include "vlc_image.h"

#include "mosaic.h"

#define BLANK_DELAY I64C(1000000)

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int  CreateFilter    ( vlc_object_t * );
static void DestroyFilter   ( vlc_object_t * );

static subpicture_t *Filter( filter_t *, mtime_t );

static int MosaicCallback( vlc_object_t *, char const *, vlc_value_t,
                           vlc_value_t, void * );

/*****************************************************************************
 * filter_sys_t : filter descriptor
 *****************************************************************************/
struct filter_sys_t
{
    vlc_mutex_t lock;
    vlc_mutex_t *p_lock;

    image_handler_t *p_image;
    picture_t *p_pic;

    int i_position; /* mosaic positioning method */
    vlc_bool_t b_ar; /* do we keep the aspect ratio ? */
    vlc_bool_t b_keep; /* do we keep the original picture format ? */
    int i_width, i_height; /* mosaic height and width */
    int i_cols, i_rows; /* mosaic rows and cols */
    int i_align; /* mosaic alignment in background video */
    int i_xoffset, i_yoffset; /* top left corner offset */
    int i_vborder, i_hborder; /* border width/height between miniatures */
    int i_alpha; /* subfilter alpha blending */

    char **ppsz_order; /* list of picture-id */
    int i_order_length;

    mtime_t i_delay;
};

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define ALPHA_TEXT N_("Alpha blending")
#define ALPHA_LONGTEXT N_("Alpha blending (0 -> 255). Default is 255")

#define HEIGHT_TEXT N_("Height in pixels")
#define WIDTH_TEXT N_("Width in pixels")
#define XOFFSET_TEXT N_("Top left corner x coordinate")
#define YOFFSET_TEXT N_("Top left corner y coordinate")
#define VBORDER_TEXT N_("Vertical border width in pixels")
#define HBORDER_TEXT N_("Horizontal border width in pixels")
#define ALIGN_TEXT N_("Mosaic alignment")

#define POS_TEXT N_("Positioning method")
#define POS_LONGTEXT N_("Positioning method. auto: automatically choose " \
        "the best number of rows and columns. fixed: use the user-defined " \
        "number of rows and columns.")
#define ROWS_TEXT N_("Number of rows")
#define COLS_TEXT N_("Number of columns")
#define AR_TEXT N_("Keep aspect ratio when resizing")
#define KEEP_TEXT N_("Keep original size")

#define ORDER_TEXT N_("Order as a comma separated list of picture-id(s)")

#define DELAY_TEXT N_("Delay")
#define DELAY_LONGTEXT N_("Pictures coming from the picture video outputs " \
        "will be delayed accordingly (in milliseconds). For high " \
        "values you will need to raise file-caching and others.")

static int pi_pos_values[] = { 0, 1 };
static char * ppsz_pos_descriptions[] =
{ N_("auto"), N_("fixed") };

static int pi_align_values[] = { 0, 1, 2, 4, 8, 5, 6, 9, 10 };
static char *ppsz_align_descriptions[] =
     { N_("Center"), N_("Left"), N_("Right"), N_("Top"), N_("Bottom"),
     N_("Top-Left"), N_("Top-Right"), N_("Bottom-Left"), N_("Bottom-Right") };


vlc_module_begin();
    set_description( N_("Mosaic video sub filter") );
    set_shortname( N_("Mosaic") );
    set_category( CAT_VIDEO );
    set_subcategory( SUBCAT_VIDEO_SUBPIC);
    set_capability( "sub filter", 0 );
    set_callbacks( CreateFilter, DestroyFilter );

    add_integer( "mosaic-alpha", 255, NULL, ALPHA_TEXT, ALPHA_LONGTEXT, VLC_FALSE );
    add_integer( "mosaic-height", 100, NULL, HEIGHT_TEXT, HEIGHT_TEXT, VLC_FALSE );
    add_integer( "mosaic-width", 100, NULL, WIDTH_TEXT, WIDTH_TEXT, VLC_FALSE );
    add_integer( "mosaic-align", 5, NULL, ALIGN_TEXT, ALIGN_TEXT, VLC_TRUE);
        change_integer_list( pi_align_values, ppsz_align_descriptions, 0 );
    add_integer( "mosaic-xoffset", 0, NULL, XOFFSET_TEXT, XOFFSET_TEXT, VLC_TRUE );
    add_integer( "mosaic-yoffset", 0, NULL, YOFFSET_TEXT, YOFFSET_TEXT, VLC_TRUE );
    add_integer( "mosaic-vborder", 0, NULL, VBORDER_TEXT, VBORDER_TEXT, VLC_TRUE );
    add_integer( "mosaic-hborder", 0, NULL, HBORDER_TEXT, HBORDER_TEXT, VLC_TRUE );

    add_integer( "mosaic-position", 0, NULL, POS_TEXT, POS_LONGTEXT, VLC_FALSE );
        change_integer_list( pi_pos_values, ppsz_pos_descriptions, 0 );
    add_integer( "mosaic-rows", 2, NULL, ROWS_TEXT, ROWS_TEXT, VLC_FALSE );
    add_integer( "mosaic-cols", 2, NULL, COLS_TEXT, COLS_TEXT, VLC_FALSE );
    add_bool( "mosaic-keep-aspect-ratio", 0, NULL, AR_TEXT, AR_TEXT, VLC_FALSE );
    add_bool( "mosaic-keep-picture", 0, NULL, KEEP_TEXT, KEEP_TEXT, VLC_FALSE );
    add_string( "mosaic-order", "", NULL, ORDER_TEXT, ORDER_TEXT, VLC_FALSE );

    add_integer( "mosaic-delay", 0, NULL, DELAY_TEXT, DELAY_LONGTEXT,
                 VLC_FALSE );

    var_Create( p_module->p_libvlc, "mosaic-lock", VLC_VAR_MUTEX );
vlc_module_end();


/*****************************************************************************
 * CreateFiler: allocate mosaic video filter
 *****************************************************************************/
static int CreateFilter( vlc_object_t *p_this )
{
    filter_t *p_filter = (filter_t *)p_this;
    filter_sys_t *p_sys;
    libvlc_t *p_libvlc = p_filter->p_libvlc;
    char *psz_order;
    int i_index;
    vlc_value_t val;

    /* The mosaic thread is more important than the decoder threads */
    vlc_thread_set_priority( p_this, VLC_THREAD_PRIORITY_OUTPUT );

    /* Allocate structure */
    p_sys = p_filter->p_sys = malloc( sizeof( filter_sys_t ) );
    if( p_sys == NULL )
    {
        msg_Err( p_filter, "out of memory" );
        return VLC_ENOMEM;
    }

    p_filter->pf_sub_filter = Filter;
    p_sys->p_pic = NULL;

    vlc_mutex_init( p_filter, &p_sys->lock );
    vlc_mutex_lock( &p_sys->lock );

    var_Get( p_libvlc, "mosaic-lock", &val );
    p_sys->p_lock = val.p_address;

#define GET_VAR( name, min, max )                                           \
    p_sys->i_##name = __MIN( max, __MAX( min,                               \
                var_CreateGetInteger( p_filter, "mosaic-" #name ) ) );      \
    var_Destroy( p_filter, "mosaic-" #name );                               \
    var_Create( p_libvlc, "mosaic-" #name, VLC_VAR_INTEGER );               \
    var_SetInteger( p_libvlc, "mosaic-" #name, p_sys->i_##name );           \
    var_AddCallback( p_libvlc, "mosaic-" #name, MosaicCallback, p_sys );

    GET_VAR( width, 0, INT_MAX );
    GET_VAR( height, 0, INT_MAX );
    GET_VAR( xoffset, 0, INT_MAX );
    GET_VAR( yoffset, 0, INT_MAX );

    p_sys->i_align = __MIN( 10, __MAX( 0,  var_CreateGetInteger( p_filter, "mosaic-align" ) ) );
    if( p_sys->i_align == 3 || p_sys->i_align == 7 )
        p_sys->i_align = 5;
    var_Destroy( p_filter, "mosaic-align" );
    var_Create( p_libvlc, "mosaic-align", VLC_VAR_INTEGER );
    var_SetInteger( p_libvlc, "mosaic-align", p_sys->i_align );
    var_AddCallback( p_libvlc, "mosaic-align", MosaicCallback, p_sys );

    GET_VAR( vborder, 0, INT_MAX );
    GET_VAR( hborder, 0, INT_MAX );
    GET_VAR( rows, 1, INT_MAX );
    GET_VAR( cols, 1, INT_MAX );
    GET_VAR( alpha, 0, 255 );
    GET_VAR( position, 0, 1 );
    GET_VAR( delay, 100, INT_MAX );
    p_sys->i_delay *= 1000;

    p_sys->b_ar = var_CreateGetBool( p_filter, "mosaic-keep-aspect-ratio" );
    var_Destroy( p_filter, "mosaic-keep-aspect-ratio" );
    var_Create( p_libvlc, "mosaic-keep-aspect-ratio", VLC_VAR_INTEGER );
    var_SetBool( p_libvlc, "mosaic-keep-aspect-ratio", p_sys->b_ar );
    var_AddCallback( p_libvlc, "mosaic-keep-aspect-ratio", MosaicCallback,
                     p_sys );

    p_sys->b_keep = var_CreateGetBool( p_filter, "mosaic-keep-picture" );
    if ( !p_sys->b_keep )
    {
        p_sys->p_image = image_HandlerCreate( p_filter );
    }

    p_sys->i_order_length = 0;
    p_sys->ppsz_order = NULL;
    psz_order = var_CreateGetString( p_filter, "mosaic-order" );

    if( psz_order[0] != 0 )
    {
        char *psz_end = NULL;
        i_index = 0;
        do
        { 
            psz_end = strchr( psz_order, ',' );
            i_index++;
            p_sys->ppsz_order = realloc( p_sys->ppsz_order,
                                         i_index * sizeof(char *) );
            p_sys->ppsz_order[i_index - 1] = strndup( psz_order,
                                           psz_end - psz_order );
            psz_order = psz_end+1;
        } while( NULL !=  psz_end );
        p_sys->i_order_length = i_index;
    }

    vlc_mutex_unlock( &p_sys->lock );

    return VLC_SUCCESS;
}

/*****************************************************************************
 * DestroyFilter: destroy mosaic video filter
 *****************************************************************************/
static void DestroyFilter( vlc_object_t *p_this )
{
    filter_t *p_filter = (filter_t*)p_this;
    filter_sys_t *p_sys = p_filter->p_sys;
    libvlc_t *p_libvlc = p_filter->p_libvlc;
    int i_index;

    vlc_mutex_lock( &p_sys->lock );

    if( !p_sys->b_keep )
    {
        image_HandlerDelete( p_sys->p_image );
    }

    if( p_sys->i_order_length )
    {
        for( i_index = 0; i_index < p_sys->i_order_length; i_index++ )
        {
            free( p_sys->ppsz_order[i_index] );
        }
        free( p_sys->ppsz_order );
    }

    var_Destroy( p_libvlc, "mosaic-alpha" );
    var_Destroy( p_libvlc, "mosaic-height" );
    var_Destroy( p_libvlc, "mosaic-align" );
    var_Destroy( p_libvlc, "mosaic-width" );
    var_Destroy( p_libvlc, "mosaic-xoffset" );
    var_Destroy( p_libvlc, "mosaic-yoffset" );
    var_Destroy( p_libvlc, "mosaic-vborder" );
    var_Destroy( p_libvlc, "mosaic-hborder" );
    var_Destroy( p_libvlc, "mosaic-position" );
    var_Destroy( p_libvlc, "mosaic-rows" );
    var_Destroy( p_libvlc, "mosaic-cols" );
    var_Destroy( p_libvlc, "mosaic-keep-aspect-ratio" );

    if( p_sys->p_pic ) p_sys->p_pic->pf_release( p_sys->p_pic );
    vlc_mutex_unlock( &p_sys->lock );
    vlc_mutex_destroy( &p_sys->lock );
    free( p_sys );
}

/*****************************************************************************
 * MosaicReleasePicture : Hack to avoid picture duplication
 *****************************************************************************/
static void MosaicReleasePicture( picture_t *p_picture )
{
    picture_t *p_original_pic = (picture_t *)p_picture->p_sys;

    p_original_pic->pf_release( p_original_pic );
}

/*****************************************************************************
 * Filter
 *****************************************************************************/
static subpicture_t *Filter( filter_t *p_filter, mtime_t date )
{
    filter_sys_t *p_sys = p_filter->p_sys;
    bridge_t *p_bridge;

    subpicture_t *p_spu;

    int i_index, i_real_index, i_row, i_col;
    int i_greatest_real_index_used = p_sys->i_order_length - 1;

    subpicture_region_t *p_region;
    subpicture_region_t *p_region_prev = NULL;

    /* Allocate the subpicture internal data. */
    p_spu = p_filter->pf_sub_buffer_new( p_filter );
    if( !p_spu )
    {
        return NULL;
    }

    /* Initialize subpicture */
    p_spu->i_channel = 0;
    p_spu->i_start  = date;
    p_spu->i_stop = 0;
    p_spu->b_ephemer = VLC_TRUE;
    p_spu->i_alpha = p_sys->i_alpha;
    p_spu->i_flags = p_sys->i_align;
    p_spu->b_absolute = VLC_FALSE;

    vlc_mutex_lock( &p_sys->lock );
    vlc_mutex_lock( p_sys->p_lock );

    p_bridge = GetBridge( p_filter );
    if ( p_bridge == NULL )
    {
        vlc_mutex_unlock( p_sys->p_lock );
        vlc_mutex_unlock( &p_sys->lock );
        return p_spu;
    }

    if ( p_sys->i_position == 0 ) /* use automatic positioning */
    {
        int i_numpics = p_sys->i_order_length; /* keep slots and all */
        for ( i_index = 0; i_index < p_bridge->i_es_num; i_index++ )
        {
            bridged_es_t *p_es = p_bridge->pp_es[i_index];
            if ( !p_es->b_empty )
            {
                i_numpics ++;
                if( p_sys->i_order_length && p_es->psz_id != 0 )
                {
                    /* We also want to leave slots for images given in
                     * mosaic-order that are not available in p_vout_picture */
                    int i;
                    for( i = 0; i < p_sys->i_order_length ; i++ )
                    {
                        if( !strcmp( p_sys->ppsz_order[i], p_es->psz_id ) )
                        {
                            i_numpics--;
                            break;
                        }
                    }

                }
            }
        }
        p_sys->i_rows = ((int)ceil(sqrt( (float)i_numpics )));
        p_sys->i_cols = ( i_numpics % p_sys->i_rows == 0 ?
                            i_numpics / p_sys->i_rows :
                            i_numpics / p_sys->i_rows + 1 );
    }

    i_real_index = 0;

    for ( i_index = 0; i_index < p_bridge->i_es_num; i_index++ )
    {
        bridged_es_t *p_es = p_bridge->pp_es[i_index];
        video_format_t fmt_in = {0}, fmt_out = {0};
        picture_t *p_converted;

        if ( p_es->b_empty )
            continue;

        while ( p_es->p_picture != NULL
                 && p_es->p_picture->date + p_sys->i_delay < date )
        {
            if ( p_es->p_picture->p_next != NULL )
            {
                picture_t *p_next = p_es->p_picture->p_next;
                p_es->p_picture->pf_release( p_es->p_picture );
                p_es->p_picture = p_next;
            }
            else if ( p_es->p_picture->date + p_sys->i_delay + BLANK_DELAY <
                        date )
            {
                /* Display blank */
                p_es->p_picture->pf_release( p_es->p_picture );
                p_es->p_picture = NULL;
                p_es->pp_last = &p_es->p_picture;
                break;
            }
            else
            {
                msg_Dbg( p_filter, "too late picture for %s (" I64Fd ")",
                         p_es->psz_id,
                         date - p_es->p_picture->date - p_sys->i_delay );
                break;
            }
        }

        if ( p_es->p_picture == NULL )
            continue;

        if ( p_sys->i_order_length == 0 )
        {
            i_real_index++;
        }
        else
        {
            int i;
            for ( i = 0; i <= p_sys->i_order_length; i++ )
            {
                if ( i == p_sys->i_order_length ) break;
                if ( strcmp( p_es->psz_id, p_sys->ppsz_order[i] ) == 0 )
                {
                    i_real_index = i;
                    break;
                }
            }
            if ( i == p_sys->i_order_length )
                i_real_index = ++i_greatest_real_index_used;
        }
        i_row = ( i_real_index / p_sys->i_cols ) % p_sys->i_rows;
        i_col = i_real_index % p_sys->i_cols ;

        if ( !p_sys->b_keep )
        {
            /* Convert the images */
            fmt_in.i_chroma = p_es->p_picture->format.i_chroma;
            fmt_in.i_height = p_es->p_picture->format.i_height;
            fmt_in.i_width = p_es->p_picture->format.i_width;

            fmt_out.i_chroma = VLC_FOURCC('Y','U','V','A');
            fmt_out.i_width = fmt_in.i_width *
                ( ( p_sys->i_width - ( p_sys->i_cols - 1 ) * p_sys->i_vborder )
                  / p_sys->i_cols ) / fmt_in.i_width;
            fmt_out.i_height = fmt_in.i_height *
                ( ( p_sys->i_height - ( p_sys->i_rows - 1 ) * p_sys->i_hborder )
                  / p_sys->i_rows ) / fmt_in.i_height;
            if( p_sys->b_ar ) /* keep aspect ratio */
            {
                if( (float)fmt_out.i_width / (float)fmt_out.i_height
                      > (float)fmt_in.i_width / (float)fmt_in.i_height )
                {
                    fmt_out.i_width = ( fmt_out.i_height * fmt_in.i_width )
                                         / fmt_in.i_height;
                }
                else
                {
                    fmt_out.i_height = ( fmt_out.i_width * fmt_in.i_height )
                                        / fmt_in.i_width;
                }
             }

            fmt_out.i_visible_width = fmt_out.i_width;
            fmt_out.i_visible_height = fmt_out.i_height;

            p_converted = image_Convert( p_sys->p_image, p_es->p_picture,
                                         &fmt_in, &fmt_out );
            if( !p_converted )
            {
                msg_Warn( p_filter,
                           "image resizing and chroma conversion failed" );
                continue;
            }
        }
        else
        {
            p_converted = p_es->p_picture;
            p_converted->i_refcount++;
            fmt_in.i_width = fmt_out.i_width = p_converted->format.i_width;
            fmt_in.i_height = fmt_out.i_height = p_converted->format.i_height;
            fmt_in.i_chroma = fmt_out.i_chroma = p_converted->format.i_chroma;
            fmt_out.i_visible_width = fmt_out.i_width;
            fmt_out.i_visible_height = fmt_out.i_height;
        }

        p_region = p_spu->pf_make_region( VLC_OBJECT(p_filter), &fmt_out,
                                          p_converted );
        if( !p_region )
        {
            msg_Err( p_filter, "cannot allocate SPU region" );
            p_filter->pf_sub_buffer_del( p_filter, p_spu );
            vlc_mutex_unlock( &p_sys->lock );
            vlc_mutex_unlock( p_sys->p_lock );
            return p_spu;
        }

        /* HACK ALERT : let's fix the pointers to avoid picture duplication.
         * This is necessary because p_region->picture is not a pointer
         * as it ought to be. */
        if( !p_sys->b_keep )
        {
            free( p_converted );
        }
        else
        {
            /* Keep a pointer to the original picture (and its refcount...). */
            p_region->picture.p_sys = (picture_sys_t *)p_converted;
            p_region->picture.pf_release = MosaicReleasePicture;
        }

        if( p_sys->b_ar || p_sys->b_keep ) /* keep aspect ratio */
        {
            /* center the video in the dedicated rectangle */
            p_region->i_x = p_sys->i_xoffset
                        + i_col * ( p_sys->i_width / p_sys->i_cols )
                        + ( i_col * p_sys->i_vborder ) / p_sys->i_cols
                        + ( ( ( p_sys->i_width
                                 - ( p_sys->i_cols - 1 ) * p_sys->i_vborder )
                            / p_sys->i_cols ) - fmt_out.i_width ) / 2;
            p_region->i_y = p_sys->i_yoffset
                        + i_row * ( p_sys->i_height / p_sys->i_rows )
                        + ( i_row * p_sys->i_hborder ) / p_sys->i_rows
                        + ( ( ( p_sys->i_height
                                 - ( p_sys->i_rows - 1 ) * p_sys->i_hborder )
                            / p_sys->i_rows ) - fmt_out.i_height ) / 2;
        }
        else
        {
            /* we don't have to center the video since it takes the
            whole rectangle area */
            p_region->i_x = p_sys->i_xoffset
                            + i_col * ( p_sys->i_width / p_sys->i_cols )
                            + ( i_col * p_sys->i_vborder ) / p_sys->i_cols;
            p_region->i_y = p_sys->i_yoffset
                        + i_row * ( p_sys->i_height / p_sys->i_rows )
                        + ( i_row * p_sys->i_hborder ) / p_sys->i_rows;
        }

        if( p_region_prev == NULL )
        {
            p_spu->p_region = p_region;
        }
        else
        {
            p_region_prev->p_next = p_region;
        }

        p_region_prev = p_region;
    }

    vlc_mutex_unlock( p_sys->p_lock );
    vlc_mutex_unlock( &p_sys->lock );

    return p_spu;
}

/*****************************************************************************
* Callback to update params on the fly
*****************************************************************************/
static int MosaicCallback( vlc_object_t *p_this, char const *psz_var,
                            vlc_value_t oldval, vlc_value_t newval,
                            void *p_data )
{
    filter_sys_t *p_sys = (filter_sys_t *) p_data;
    if( !strcmp( psz_var, "mosaic-alpha" ) )
    {
        vlc_mutex_lock( &p_sys->lock );
        msg_Dbg( p_this, "Changing alpha from %d/255 to %d/255",
                         p_sys->i_alpha, newval.i_int);
        p_sys->i_alpha = __MIN( __MAX( newval.i_int, 0 ), 255 );
        vlc_mutex_unlock( &p_sys->lock );
    }
    else if( !strcmp( psz_var, "mosaic-height" ) )
    {
        vlc_mutex_lock( &p_sys->lock );
        msg_Dbg( p_this, "Changing height from %dpx to %dpx",
                          p_sys->i_height, newval.i_int );
        p_sys->i_height = __MAX( newval.i_int, 0 );
        vlc_mutex_unlock( &p_sys->lock );
    }
    else if( !strcmp( psz_var, "mosaic-width" ) )
    {
        vlc_mutex_lock( &p_sys->lock );
        msg_Dbg( p_this, "Changing width from %dpx to %dpx",
                         p_sys->i_width, newval.i_int );
        p_sys->i_width = __MAX( newval.i_int, 0 );
        vlc_mutex_unlock( &p_sys->lock );
    }
    else if( !strcmp( psz_var, "mosaic-xoffset" ) )
    {
        vlc_mutex_lock( &p_sys->lock );
        msg_Dbg( p_this, "Changing x offset from %dpx to %dpx",
                         p_sys->i_xoffset, newval.i_int );
        p_sys->i_xoffset = __MAX( newval.i_int, 0 );
        vlc_mutex_unlock( &p_sys->lock );
    }
    else if( !strcmp( psz_var, "mosaic-yoffset" ) )
    {
        vlc_mutex_lock( &p_sys->lock );
        msg_Dbg( p_this, "Changing y offset from %dpx to %dpx",
                         p_sys->i_yoffset, newval.i_int );
        p_sys->i_yoffset = __MAX( newval.i_int, 0 );
        vlc_mutex_unlock( &p_sys->lock );
    }
    else if( !strcmp( psz_var, "mosaic-align" ) )
    {
        int i_old = 0, i_new = 0;
        vlc_mutex_lock( &p_sys->lock );
        newval.i_int = __MIN( __MAX( newval.i_int, 0 ), 10 );
        if( newval.i_int == 3 || newval.i_int == 7 )
            newval.i_int = 5;
        while( pi_align_values[i_old] != p_sys->i_align ) i_old++;
        while( pi_align_values[i_new] != newval.i_int ) i_new++;
        msg_Dbg( p_this, "Changing alignment from %d (%s) to %d (%s)",
                     p_sys->i_align, ppsz_align_descriptions[i_old],
                     newval.i_int, ppsz_align_descriptions[i_new] );
        p_sys->i_align = newval.i_int;
        vlc_mutex_unlock( &p_sys->lock );
    }
    else if( !strcmp( psz_var, "mosaic-vborder" ) )
    {
        vlc_mutex_lock( &p_sys->lock );
        msg_Dbg( p_this, "Changing vertical border from %dpx to %dpx",
                         p_sys->i_vborder, newval.i_int );
        p_sys->i_vborder = __MAX( newval.i_int, 0 );
        vlc_mutex_unlock( &p_sys->lock );
    }
    else if( !strcmp( psz_var, "mosaic-hborder" ) )
    {
        vlc_mutex_lock( &p_sys->lock );
        msg_Dbg( p_this, "Changing horizontal border from %dpx to %dpx",
                         p_sys->i_vborder, newval.i_int );
        p_sys->i_hborder = __MAX( newval.i_int, 0 );
        vlc_mutex_unlock( &p_sys->lock );
    }
    else if( !strcmp( psz_var, "mosaic-position" ) )
    {
        if( newval.i_int > 1 || newval.i_int < 0 )
        {
            msg_Err( p_this, "Position is either 0 (auto) or 1 (fixed)" );
        }
        else
        {
            vlc_mutex_lock( &p_sys->lock );
            msg_Dbg( p_this, "Changing position method from %d (%s) to %d (%s)",
                             p_sys->i_position, ppsz_pos_descriptions[p_sys->i_position],
                             newval.i_int, ppsz_pos_descriptions[newval.i_int]);
            p_sys->i_position = newval.i_int;
            vlc_mutex_unlock( &p_sys->lock );
        }
    }
    else if( !strcmp( psz_var, "mosaic-rows" ) )
    {
        vlc_mutex_lock( &p_sys->lock );
        msg_Dbg( p_this, "Changing number of rows from %d to %d",
                         p_sys->i_rows, newval.i_int );
        p_sys->i_rows = __MAX( newval.i_int, 1 );
        vlc_mutex_unlock( &p_sys->lock );
    }
    else if( !strcmp( psz_var, "mosaic-cols" ) )
    {
        vlc_mutex_lock( &p_sys->lock );
        msg_Dbg( p_this, "Changing number of columns from %d to %d",
                         p_sys->i_cols, newval.i_int );
        p_sys->i_cols = __MAX( newval.i_int, 1 );
        vlc_mutex_unlock( &p_sys->lock );
    }
    else if( !strcmp( psz_var, "mosaic-keep-aspect-ratio" ) )
    {
        vlc_mutex_lock( &p_sys->lock );
        if( newval.i_int )
        {
            msg_Dbg( p_this, "Keep aspect ratio" );
            p_sys->b_ar = 1;
        }
        else
        {
            msg_Dbg( p_this, "Don't keep aspect ratio" );
            p_sys->b_ar = 0;
        }
        vlc_mutex_unlock( &p_sys->lock );
    }
    return VLC_SUCCESS;
}
