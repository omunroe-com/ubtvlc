/*****************************************************************************
 * vlc_video.h: common video definitions
 * This header is required by all modules which have to handle pictures. It
 * includes all common video types and constants.
 *****************************************************************************
 * Copyright (C) 1999, 2000 VideoLAN
 * $Id: vlc_video.h 6961 2004-03-05 17:34:23Z sam $
 *
 * Authors: Vincent Seguin <seguin@via.ecp.fr>
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
#ifndef _VLC_VIDEO_H
#define _VLC_VIDEO_H 1

#include "vlc_es.h"

/**
 * Description of a planar graphic field
 */
typedef struct plane_t
{
    uint8_t *p_pixels;                        /**< Start of the plane's data */

    /* Variables used for fast memcpy operations */
    int i_lines;                                        /**< Number of lines */
    int i_pitch;           /**< Number of bytes in a line, including margins */

    /** Size of a macropixel, defaults to 1 */
    int i_pixel_pitch;

    /* Variables used for pictures with margins */
    int i_visible_pitch;            /**< How many visible pixels are there ? */

} plane_t;

/**
 * Video picture
 *
 * Any picture destined to be displayed by a video output thread should be
 * stored in this structure from it's creation to it's effective display.
 * Picture type and flags should only be modified by the output thread. Note
 * that an empty picture MUST have its flags set to 0.
 */
struct picture_t
{
    /** \name Picture format
     * Describes the properties of the picture
     * @{*/
    video_frame_format_t format;
    /**@}*/

    /** Picture data - data can always be freely modified, but p_data may
     * NEVER be modified. A direct buffer can be handled as the plugin
     * wishes, it can even swap p_pixels buffers. */
    uint8_t        *p_data;
    void           *p_data_orig;                /**< pointer before memalign */
    plane_t         p[ VOUT_MAX_PLANES ];     /**< description of the planes */
    int             i_planes;                /**< number of allocated planes */

    /** \name Type and flags
     * Should NOT be modified except by the vout thread
     * @{*/
    int             i_status;                             /**< picture flags */
    int             i_type;                /**< is picture a direct buffer ? */
    int             i_matrix_coefficients;   /**< in YUV type, encoding type */
    /**@}*/

    /** \name Picture management properties
     * These properties can be modified using the video output thread API,
     * but should never be written directly */
    /**@{*/
    int             i_refcount;                  /**< link reference counter */
    mtime_t         date;                                  /**< display date */
    vlc_bool_t      b_force;
    /**@}*/

    /** \name Picture dynamic properties
     * Those properties can be changed by the decoder
     * @{
     */
    vlc_bool_t      b_progressive;          /**< is it a progressive frame ? */
    unsigned int    i_nb_fields;                  /**< # of displayed fields */
    vlc_bool_t      b_top_field_first;             /**< which field is first */
    /**@}*/

    /** The picture heap we are attached to */
    picture_heap_t* p_heap;

    /* Some vouts require the picture to be locked before it can be modified */
    int (* pf_lock) ( vout_thread_t *, picture_t * );
    int (* pf_unlock) ( vout_thread_t *, picture_t * );

    /** Private data - the video output plugin might want to put stuff here to
     * keep track of the picture */
    picture_sys_t * p_sys;
};

/**
 * Video picture heap, either render (to store pictures used
 * by the decoder) or output (to store pictures displayed by the vout plugin)
 */
struct picture_heap_t
{
    int i_pictures;                                   /**< current heap size */

    /* \name Picture static properties
     * Those properties are fixed at initialization and should NOT be modified
     * @{
     */
    unsigned int i_width;                                 /**< picture width */
    unsigned int i_height;                               /**< picture height */
    vlc_fourcc_t i_chroma;                               /**< picture chroma */
    unsigned int i_aspect;                                 /**< aspect ratio */
    /**@}*/

    /* Real pictures */
    picture_t*      pp_picture[VOUT_MAX_PICTURES];             /**< pictures */
    int             i_last_used_pic;              /**< last used pic in heap */
    vlc_bool_t      b_allow_modify_pics;

    /* Stuff used for truecolor RGB planes */
    int i_rmask, i_rrshift, i_lrshift;
    int i_gmask, i_rgshift, i_lgshift;
    int i_bmask, i_rbshift, i_lbshift;

    /** Stuff used for palettized RGB planes */
    void (* pf_setpalette) ( vout_thread_t *, uint16_t *, uint16_t *, uint16_t * );
};

/*****************************************************************************
 * Flags used to describe the status of a picture
 *****************************************************************************/

/* Picture type */
#define EMPTY_PICTURE           0                            /* empty buffer */
#define MEMORY_PICTURE          100                 /* heap-allocated buffer */
#define DIRECT_PICTURE          200                         /* direct buffer */

/* Picture status */
#define FREE_PICTURE            0                  /* free and not allocated */
#define RESERVED_PICTURE        1                  /* allocated and reserved */
#define RESERVED_DATED_PICTURE  2              /* waiting for DisplayPicture */
#define RESERVED_DISP_PICTURE   3               /* waiting for a DatePicture */
#define READY_PICTURE           4                       /* ready for display */
#define DISPLAYED_PICTURE       5            /* been displayed but is linked */
#define DESTROYED_PICTURE       6              /* allocated but no more used */

/*****************************************************************************
 * Shortcuts to access image components
 *****************************************************************************/

/* Plane indices */
#define Y_PLANE      0
#define U_PLANE      1
#define V_PLANE      2

/* Shortcuts */
#define Y_PIXELS     p[Y_PLANE].p_pixels
#define Y_PITCH      p[Y_PLANE].i_pitch
#define U_PIXELS     p[U_PLANE].p_pixels
#define U_PITCH      p[U_PLANE].i_pitch
#define V_PIXELS     p[V_PLANE].p_pixels
#define V_PITCH      p[V_PLANE].i_pitch

/**
 * \defgroup subpicture Video Subpictures
 * Subpictures are pictures that should be displayed on top of the video, like
 * subtitles and OSD
 * \ingroup video_output
 * @{
 */

/**
 * Video subtitle
 *
 * Any subtitle destined to be displayed by a video output thread should
 * be stored in this structure from it's creation to it's effective display.
 * Subtitle type and flags should only be modified by the output thread. Note
 * that an empty subtitle MUST have its flags set to 0.
 */
struct subpicture_t
{
    /** \name Type and flags
       Should NOT be modified except by the vout thread */
    /**@{*/
    int             i_type;                                        /**< type */
    int             i_status;                                     /**< flags */
    subpicture_t *  p_next;               /**< next subtitle to be displayed */
    /**@}*/

    /** \name Date properties */
    /**@{*/
    mtime_t         i_start;                  /**< beginning of display date */
    mtime_t         i_stop;                         /**< end of display date */
    vlc_bool_t      b_ephemer;     /**< If this flag is set to true
                                      the subtitle will be displayed
                                      untill the next one appear */
    /**@}*/

    /** \name Display properties
     * These properties are only indicative and may be
     * changed by the video output thread, or simply ignored depending of the
     * subtitle type. */
    /**@{*/
    int             i_x;                 /**< offset from alignment position */
    int             i_y;                 /**< offset from alignment position */
    int             i_width;                              /**< picture width */
    int             i_height;                            /**< picture height */
    /**@}*/

    /** Pointer to function that renders this subtitle in a picture */
    void ( *pf_render )  ( vout_thread_t *, picture_t *, const subpicture_t * );
    /** Pointer to function that cleans up the private data of this subtitle */
    void ( *pf_destroy ) ( subpicture_t * );

    /** Private data - the subtitle plugin might want to put stuff here to
     * keep track of the subpicture */
    subpicture_sys_t *p_sys;                              /* subpicture data */
};

/* Subpicture type */
#define EMPTY_SUBPICTURE       0     /* subtitle slot is empty and available */
#define MEMORY_SUBPICTURE      100            /* subpicture stored in memory */

/* Subpicture status */
#define FREE_SUBPICTURE        0                   /* free and not allocated */
#define RESERVED_SUBPICTURE    1                   /* allocated and reserved */
#define READY_SUBPICTURE       2                        /* ready for display */

/*****************************************************************************
 * Prototypes
 *****************************************************************************/
/**
 * vout_AspectRatio
 *
 * Set the i_aspect_x and i_aspect_y from the encoded aspect ratio i_aspect.
 * \param i_aspect the encoded aspect ratio
 * \param i_aspect_x the decoded x-axis portion of i_aspect. This is set.
 * \param i_aspect_y the decoded y-axis portion of i_aspect  This is set.
 */
VLC_EXPORT( void,  vout_AspectRatio, ( unsigned int i_aspect, unsigned int *i_aspect_x, unsigned int *i_aspect_y ) );

/**@}*/

#endif /* _VLC_VIDEO_H */
