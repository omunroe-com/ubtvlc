/*****************************************************************************
 * pixel.h: h264 encoder library
 *****************************************************************************
 * Copyright (C) 2003 Laurent Aimar
 * $Id: pixel.h,v 1.1 2004/06/03 19:27:07 fenrir Exp $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
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

#ifndef _PIXEL_H
#define _PIXEL_H 1

typedef int  (*x264_pixel_sad_t) ( uint8_t *, int, uint8_t *, int );
typedef int  (*x264_pixel_satd_t)( uint8_t *, int, uint8_t *, int );
typedef void (*x264_pixel_avg_t) ( uint8_t *, int, uint8_t *, int );

enum
{
    PIXEL_16x16 = 0,
    PIXEL_16x8  = 1,
    PIXEL_8x16  = 2,
    PIXEL_8x8   = 3,
    PIXEL_8x4   = 4,
    PIXEL_4x8   = 5,
    PIXEL_4x4   = 6,
};

static const struct {
    int w;
    int h;
} x264_pixel_size[7] = {
    { 16, 16 },
    { 16,  8 }, {  8, 16 },
    {  8,  8 },
    {  8,  4 }, {  4,  8 },
    {  4,  4 }
};

typedef struct
{
    x264_pixel_sad_t  sad[7];
    x264_pixel_satd_t satd[7];
    x264_pixel_avg_t  avg[7];
} x264_pixel_function_t;

void x264_pixel_init( int cpu, x264_pixel_function_t *pixf );

#endif
