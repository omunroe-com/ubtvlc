/*****************************************************************************
 * mc.c: h264 encoder library (Motion Compensation)
 *****************************************************************************
 * Copyright (C) 2003 Laurent Aimar
 * $Id: mc.c,v 1.1 2004/06/03 19:27:07 fenrir Exp $
 *
 * Authors: Eric Petit <titer@m0k.org>
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#ifdef SYS_LINUX
#include <altivec.h>
#endif

#include "x264.h"
#include "../mc.h"
#include "../clip1.h"
#include "mc.h"
#include "ppccommon.h"

typedef void (*pf_mc_t)( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height );

static inline int x264_tapfilter( uint8_t *pix, int i_pix_next )
{
    return pix[-2*i_pix_next] - 5*pix[-1*i_pix_next] + 20*(pix[0] +
           pix[1*i_pix_next]) - 5*pix[ 2*i_pix_next] +
           pix[ 3*i_pix_next];
}
static inline int x264_tapfilter1( uint8_t *pix )
{
    return pix[-2] - 5*pix[-1] + 20*(pix[0] + pix[1]) - 5*pix[ 2] +
           pix[ 3];
}

/* pixel_avg */
static inline void pixel_avg_w4( uint8_t *dst,  int i_dst,
                                 uint8_t *src1, int i_src1,
                                 uint8_t *src2, int i_src2,
                                 int i_height )
{
    int x, y;
    for( y = 0; y < i_height; y++ )
    {
        for( x = 0; x < 4; x++ )
        {
            dst[x] = ( src1[x] + src2[x] + 1 ) >> 1;
        }
        dst  += i_dst;
        src1 += i_src1;
        src2 += i_src2;
    }
}
static inline void pixel_avg_w8( uint8_t *dst,  int i_dst,
                                 uint8_t *src1, int i_src1,
                                 uint8_t *src2, int i_src2,
                                 int i_height )
{
    /* TODO - optimize */
    pixel_avg_w4( &dst[0], i_dst, &src1[0], i_src1, &src2[0], i_src2,
                  i_height );
    pixel_avg_w4( &dst[4], i_dst, &src1[4], i_src1, &src2[4], i_src2,
                  i_height );
}
static inline void pixel_avg_w16( uint8_t *dst,  int i_dst,
                                  uint8_t *src1, int i_src1,
                                  uint8_t *src2, int i_src2,
                                  int i_height )
{
    int y;
    vector_u8_t src1v, src2v;
    for( y = 0; y < i_height; y++ )
    {
        LOAD_16( src1, src1v );
        LOAD_16( src2, src2v );
        src1v = vec_avg( src1v, src2v );
        STORE_16( src1v, dst );

        dst  += i_dst;
        src1 += i_src1;
        src2 += i_src2;
    }
}

/* mc_copy: plain c */
#define MC_COPY( name, a )                                \
static void name( uint8_t *src, int i_src,                \
                  uint8_t *dst, int i_dst, int i_height ) \
{                                                         \
    int y;                                                \
    for( y = 0; y < i_height; y++ )                       \
    {                                                     \
        memcpy( dst, src, a );                            \
        src += i_src;                                     \
        dst += i_dst;                                     \
    }                                                     \
}
MC_COPY( mc_copy_w4,  4  )
MC_COPY( mc_copy_w8,  8  )
MC_COPY( mc_copy_w16, 16 )

/* TAP_FILTER:
   a is source (vector_s16_t [6])
   b is a temporary vector_s16_t
   c is the result

   c   = src[0] + a[5] - 5 * ( a[1] + a[4] ) + 20 * ( a[2] + a[3] );
   c  += 16;
   c >>= 5;
   c  += 80; */
#define TAP_FILTER( a, b, c )                       \
    c = vec_add( a[0], a[5] );                      \
    b = vec_add( a[1], a[4] );                      \
    c = vec_sub( c, b );                            \
    b = vec_sl( b, vec_splat_u16( 2 ) );            \
    c = vec_sub( c, b );                            \
    b = vec_add( a[2], a[3] );                      \
    b = vec_sl( b, vec_splat_u16( 2 ) );            \
    c = vec_add( c, b );                            \
    b = vec_sl( b, vec_splat_u16( 2 ) );            \
    c = vec_add( c, b );                            \
    c = vec_add( c, vec_splat_s16( 8 ) );           \
    c = vec_add( c, vec_splat_s16( 8 ) );           \
    c = vec_sr( c, vec_splat_u16( 5 ) );            \
    c = vec_add( c, vec_sl( vec_splat_s16( 5 ),     \
                            vec_splat_u16( 4 ) ) );

/* mc_hh */
static inline void mc_hh_w4( uint8_t *src, int i_src,
                             uint8_t *dst, int i_dst, int i_height )
{
    int x, y;
    for( y = 0; y < i_height; y++ )
    {
        for( x = 0; x < 4; x++ )
        {
            dst[x] = x264_mc_clip1( ( x264_tapfilter1( &src[x] ) +
                                      16 ) >> 5 );
        }
        src += i_src;
        dst += i_dst;
    }
}
static inline void mc_hh_w8( uint8_t *src, int i_src,
                             uint8_t *dst, int i_dst, int i_height )
{
    long x, y;
    DECLARE_ALIGNED( int16_t, tmp[8], 16 );

    LOAD_ZERO;
    vector_u8_t    loadv;
    vector_s16_t   srcv[6];
    vector_u8_t  * _srcv = (vector_u8_t*) srcv;
    vector_s16_t   dstv;
    vector_s16_t   tmpv;

    for( y = 0; y < i_height; y++ )
    {
        LOAD_16( &src[-2], loadv );

        for( x = 0; x < 6; x++ )
        {
            _srcv[x] = vec_perm( loadv, zero_u8,
                                 vec_lvsl( 0, (int*) x ) );
            CONVERT_U8_TO_S16( srcv[x] );
        }

        TAP_FILTER( srcv, tmpv, dstv );
        vec_st( dstv, 0, tmp );

        for( x = 0; x < 8; x++ )
        {
            dst[x] = x264_mc_clip1_table[tmp[x]];
        }

        src += i_src;
        dst += i_dst;
    }
}
static inline void mc_hh_w16( uint8_t *src, int i_src,
                              uint8_t *dst, int i_dst, int i_height )
{
    mc_hh_w8( &src[0], i_src, &dst[0], i_dst, i_height );
    mc_hh_w8( &src[8], i_src, &dst[8], i_dst, i_height );
}

/* mc_hv */
static inline void mc_hv_w4( uint8_t *src, int i_src,
                             uint8_t *dst, int i_dst, int i_height )
{
    int x, y;
    for( y = 0; y < i_height; y++ )
    {
        for( x = 0; x < 4; x++ )
        {
            dst[x] = x264_mc_clip1( ( x264_tapfilter( &src[x], i_src ) +
                                      16 ) >> 5 );
        }
        src += i_src;
        dst += i_dst;
    }
}
static inline void mc_hv_w8( uint8_t *src, int i_src,
                             uint8_t *dst, int i_dst, int i_height )
{
    int x, y;
    DECLARE_ALIGNED( int16_t, tmp[8], 16 );

    LOAD_ZERO;
    vector_s16_t   srcv[6];
    vector_u8_t  * _srcv = (vector_u8_t*) srcv;
    vector_s16_t   dstv;
    vector_s16_t   tmpv;

    for( y = 0; y < i_height; y++ )
    {
        if( y )
        {
            for( x = 0; x < 5; x++ )
            {
                srcv[x] = srcv[x+1];
            }
            LOAD_8( &src[3*i_src], _srcv[5] );
            CONVERT_U8_TO_S16( srcv[5] );
        }
        else
        {
            for( x = 0; x < 6; x++ )
            {
                LOAD_8( &src[(x-2)*i_src], _srcv[x] );
                CONVERT_U8_TO_S16( srcv[x] );
            }
        }

        TAP_FILTER( srcv, tmpv, dstv );
        vec_st( dstv, 0, tmp );

        for( x = 0; x < 8; x++ )
        {
            dst[x] = x264_mc_clip1_table[tmp[x]];
        }
        src += i_src;
        dst += i_dst;
    }
}
static inline void mc_hv_w16( uint8_t *src, int i_src,
                              uint8_t *dst, int i_dst, int i_height )
{
    mc_hv_w8( &src[0], i_src, &dst[0], i_dst, i_height );
    mc_hv_w8( &src[8], i_src, &dst[8], i_dst, i_height );
}

/* mc_hc */
static inline void mc_hc_w4( uint8_t *src, int i_src,
                             uint8_t *dst, int i_dst, int i_height )
{
    uint8_t *out;
    uint8_t *pix;
    int x, y;

    for( x = 0; x < 4; x++ )
    {
        int tap[6];

        pix = &src[x];
        out = &dst[x];

        tap[0] = x264_tapfilter1( &pix[-2*i_src] );
        tap[1] = x264_tapfilter1( &pix[-1*i_src] );
        tap[2] = x264_tapfilter1( &pix[ 0*i_src] );
        tap[3] = x264_tapfilter1( &pix[ 1*i_src] );
        tap[4] = x264_tapfilter1( &pix[ 2*i_src] );

        for( y = 0; y < i_height; y++ )
        {
            tap[5] = x264_tapfilter1( &pix[ 3*i_src] );

            *out = x264_mc_clip1( ( tap[0] - 5*tap[1] + 20 * tap[2] +
                                    20 * tap[3] -5*tap[4] + tap[5] +
                                    512 ) >> 10 );

            /* Next line */
            pix += i_src;
            out += i_dst;
            tap[0] = tap[1];
            tap[1] = tap[2];
            tap[2] = tap[3];
            tap[3] = tap[4];
            tap[4] = tap[5];
        }
    }
}
static inline void mc_hc_w8( uint8_t *src, int i_src,
                             uint8_t *dst, int i_dst, int i_height )
{
    /* TODO: optimize */
    mc_hc_w4( &src[0], i_src, &dst[0], i_dst, i_height );
    mc_hc_w4( &src[4], i_src, &dst[4], i_dst, i_height );
}
static inline void mc_hc_w16( uint8_t *src, int i_src,
                              uint8_t *dst, int i_dst, int i_height )
{
    mc_hc_w8( &src[0], i_src, &dst[0], i_dst, i_height );
    mc_hc_w8( &src[8], i_src, &dst[8], i_dst, i_height );
}

/* mc I+H */
static void mc_xy10_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*4];
    mc_hh_w4( src, i_src, tmp, 4, i_height );
    pixel_avg_w4( dst, i_dst, src, i_src, tmp, 4, i_height );
}
static void mc_xy10_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*8];
    mc_hh_w8( src, i_src, tmp, 8, i_height );
    pixel_avg_w8( dst, i_dst, src, i_src, tmp, 8, i_height );
}
static void mc_xy10_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*16];
    mc_hh_w16( src, i_src, tmp, 16, i_height );
    pixel_avg_w16( dst, i_dst, src, i_src, tmp, 16, i_height );
}

static void mc_xy30_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*4];
    mc_hh_w4( src, i_src, tmp, 4, i_height );
    pixel_avg_w4( dst, i_dst, src + 1, i_src, tmp, 4, i_height );
}
static void mc_xy30_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*8];
    mc_hh_w8( src, i_src, tmp, 8, i_height );
    pixel_avg_w8( dst, i_dst, src + 1, i_src, tmp, 8, i_height );
}
static void mc_xy30_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*16];
    mc_hh_w16( src, i_src, tmp, 16, i_height );
    pixel_avg_w16( dst, i_dst, src + 1, i_src, tmp, 16, i_height );
}

/* mc I+V */
static void mc_xy01_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*4];
    mc_hv_w4( src, i_src, tmp, 4, i_height );
    pixel_avg_w4( dst, i_dst, src, i_src, tmp, 4, i_height );
}
static void mc_xy01_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*8];
    mc_hv_w8( src, i_src, tmp, 8, i_height );
    pixel_avg_w8( dst, i_dst, src, i_src, tmp, 8, i_height );
}
static void mc_xy01_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*16];
    mc_hv_w16( src, i_src, tmp, 16, i_height );
    pixel_avg_w16( dst, i_dst, src, i_src, tmp, 16, i_height );
}

static void mc_xy03_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*4];
    mc_hv_w4( src, i_src, tmp, 4, i_height );
    pixel_avg_w4( dst, i_dst, src + i_src, i_src, tmp, 4, i_height );
}
static void mc_xy03_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*8];
    mc_hv_w8( src, i_src, tmp, 8, i_height );
    pixel_avg_w8( dst, i_dst, src + i_src, i_src, tmp, 8, i_height );
}
static void mc_xy03_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp[16*16];
    mc_hv_w16( src, i_src, tmp, 16, i_height );
    pixel_avg_w16( dst, i_dst, src + i_src, i_src, tmp, 16, i_height );
}

/* H+V */
static void mc_xy11_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*4];
    uint8_t tmp2[16*4];
    mc_hv_w4( src, i_src, tmp1, 4, i_height );
    mc_hh_w4( src, i_src, tmp2, 4, i_height );
    pixel_avg_w4( dst, i_dst, tmp1, 4, tmp2, 4, i_height );
}
static void mc_xy11_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*8];
    uint8_t tmp2[16*8];
    mc_hv_w8( src, i_src, tmp1, 8, i_height );
    mc_hh_w8( src, i_src, tmp2, 8, i_height );
    pixel_avg_w8( dst, i_dst, tmp1, 8, tmp2, 8, i_height );
}
static void mc_xy11_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*16];
    uint8_t tmp2[16*16];
    mc_hv_w16( src, i_src, tmp1, 16, i_height );
    mc_hh_w16( src, i_src, tmp2, 16, i_height );
    pixel_avg_w16( dst, i_dst, tmp1, 16, tmp2, 16, i_height );
}

static void mc_xy31_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*4];
    uint8_t tmp2[16*4];
    mc_hv_w4( src+1, i_src, tmp1, 4, i_height );
    mc_hh_w4( src,   i_src, tmp2, 4, i_height );
    pixel_avg_w4( dst, i_dst, tmp1, 4, tmp2, 4, i_height );
}
static void mc_xy31_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*8];
    uint8_t tmp2[16*8];
    mc_hv_w8( src+1, i_src, tmp1, 8, i_height );
    mc_hh_w8( src,   i_src, tmp2, 8, i_height );
    pixel_avg_w8( dst, i_dst, tmp1, 8, tmp2, 8, i_height );
}
static void mc_xy31_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*16];
    uint8_t tmp2[16*16];
    mc_hv_w16( src+1, i_src, tmp1, 16, i_height );
    mc_hh_w16( src,   i_src, tmp2, 16, i_height );
    pixel_avg_w16( dst, i_dst, tmp1, 16, tmp2, 16, i_height );
}

static void mc_xy13_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*4];
    uint8_t tmp2[16*4];
    mc_hv_w4( src,       i_src, tmp1, 4, i_height );
    mc_hh_w4( src+i_src, i_src, tmp2, 4, i_height );
    pixel_avg_w4( dst, i_dst, tmp1, 4, tmp2, 4, i_height );
}
static void mc_xy13_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*8];
    uint8_t tmp2[16*8];
    mc_hv_w8( src,       i_src, tmp1, 8, i_height );
    mc_hh_w8( src+i_src, i_src, tmp2, 8, i_height );
    pixel_avg_w8( dst, i_dst, tmp1, 8, tmp2, 8, i_height );
}
static void mc_xy13_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*16];
    uint8_t tmp2[16*16];
    mc_hv_w16( src,       i_src, tmp1, 16, i_height );
    mc_hh_w16( src+i_src, i_src, tmp2, 16, i_height );
    pixel_avg_w16( dst, i_dst, tmp1, 16, tmp2, 16, i_height );
}

static void mc_xy33_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*4];
    uint8_t tmp2[16*4];
    mc_hv_w4( src+1,     i_src, tmp1, 4, i_height );
    mc_hh_w4( src+i_src, i_src, tmp2, 4, i_height );
    pixel_avg_w4( dst, i_dst, tmp1, 4, tmp2, 4, i_height );
}
static void mc_xy33_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*8];
    uint8_t tmp2[16*8];
    mc_hv_w8( src+1,     i_src, tmp1, 8, i_height );
    mc_hh_w8( src+i_src, i_src, tmp2, 8, i_height );
    pixel_avg_w8( dst, i_dst, tmp1, 8, tmp2, 8, i_height );
}
static void mc_xy33_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*16];
    uint8_t tmp2[16*16];
    mc_hv_w16( src+1,     i_src, tmp1, 16, i_height );
    mc_hh_w16( src+i_src, i_src, tmp2, 16, i_height );
    pixel_avg_w16( dst, i_dst, tmp1, 16, tmp2, 16, i_height );
}

static void mc_xy21_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*4];
    uint8_t tmp2[16*4];
    mc_hc_w4( src, i_src, tmp1, 4, i_height );
    mc_hh_w4( src, i_src, tmp2, 4, i_height );
    pixel_avg_w4( dst, i_dst, tmp1, 4, tmp2, 4, i_height );
}
static void mc_xy21_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*8];
    uint8_t tmp2[16*8];
    mc_hc_w8( src, i_src, tmp1, 8, i_height );
    mc_hh_w8( src, i_src, tmp2, 8, i_height );
    pixel_avg_w8( dst, i_dst, tmp1, 8, tmp2, 8, i_height );
}
static void mc_xy21_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*16];
    uint8_t tmp2[16*16];
    mc_hc_w16( src, i_src, tmp1, 16, i_height );
    mc_hh_w16( src, i_src, tmp2, 16, i_height );
    pixel_avg_w16( dst, i_dst, tmp1, 16, tmp2, 16, i_height );
}

static void mc_xy12_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*4];
    uint8_t tmp2[16*4];
    mc_hc_w4( src, i_src, tmp1, 4, i_height );
    mc_hv_w4( src, i_src, tmp2, 4, i_height );
    pixel_avg_w4( dst, i_dst, tmp1, 4, tmp2, 4, i_height );
}
static void mc_xy12_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*8];
    uint8_t tmp2[16*8];
    mc_hc_w8( src, i_src, tmp1, 8, i_height );
    mc_hv_w8( src, i_src, tmp2, 8, i_height );
    pixel_avg_w8( dst, i_dst, tmp1, 8, tmp2, 8, i_height );
}
static void mc_xy12_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*16];
    uint8_t tmp2[16*16];
    mc_hc_w16( src, i_src, tmp1, 16, i_height );
    mc_hv_w16( src, i_src, tmp2, 16, i_height );
    pixel_avg_w16( dst, i_dst, tmp1, 16, tmp2, 16, i_height );
}

static void mc_xy32_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*4];
    uint8_t tmp2[16*4];
    mc_hc_w4( src,   i_src, tmp1, 4, i_height );
    mc_hv_w4( src+1, i_src, tmp2, 4, i_height );
    pixel_avg_w4( dst, i_dst, tmp1, 4, tmp2, 4, i_height );
}
static void mc_xy32_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*8];
    uint8_t tmp2[16*8];
    mc_hc_w8( src,   i_src, tmp1, 8, i_height );
    mc_hv_w8( src+1, i_src, tmp2, 8, i_height );
    pixel_avg_w8( dst, i_dst, tmp1, 8, tmp2, 8, i_height );
}
static void mc_xy32_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*16];
    uint8_t tmp2[16*16];
    mc_hc_w16( src,   i_src, tmp1, 16, i_height );
    mc_hv_w16( src+1, i_src, tmp2, 16, i_height );
    pixel_avg_w16( dst, i_dst, tmp1, 16, tmp2, 16, i_height );
}

static void mc_xy23_w4( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*4];
    uint8_t tmp2[16*4];
    mc_hc_w4( src,       i_src, tmp1, 4, i_height );
    mc_hh_w4( src+i_src, i_src, tmp2, 4, i_height );
    pixel_avg_w4( dst, i_dst, tmp1, 4, tmp2, 4, i_height );
}
static void mc_xy23_w8( uint8_t *src, int i_src,
                        uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*8];
    uint8_t tmp2[16*8];
    mc_hc_w8( src,       i_src, tmp1, 8, i_height );
    mc_hh_w8( src+i_src, i_src, tmp2, 8, i_height );
    pixel_avg_w8( dst, i_dst, tmp1, 8, tmp2, 8, i_height );
}
static void mc_xy23_w16( uint8_t *src, int i_src,
                         uint8_t *dst, int i_dst, int i_height )
{
    uint8_t tmp1[16*16];
    uint8_t tmp2[16*16];
    mc_hc_w16( src,       i_src, tmp1, 16, i_height );
    mc_hh_w16( src+i_src, i_src, tmp2, 16, i_height );
    pixel_avg_w16( dst, i_dst, tmp1, 16, tmp2, 16, i_height );
}

static void motion_compensation_luma( uint8_t *src, int i_src,
                                      uint8_t *dst, int i_dst,
                                      int mvx,int mvy,
                                      int i_width, int i_height )
{
    static const pf_mc_t pf_mc[3][4][4] =    /*XXX [dqy][dqx] */
    {
        {
            { mc_copy_w4,  mc_xy10_w4,    mc_hh_w4,      mc_xy30_w4 },
            { mc_xy01_w4,  mc_xy11_w4,    mc_xy21_w4,    mc_xy31_w4 },
            { mc_hv_w4,    mc_xy12_w4,    mc_hc_w4,      mc_xy32_w4 },
            { mc_xy03_w4,  mc_xy13_w4,    mc_xy23_w4,    mc_xy33_w4 },
        },
        {
            { mc_copy_w8,  mc_xy10_w8,    mc_hh_w8,      mc_xy30_w8 },
            { mc_xy01_w8,  mc_xy11_w8,    mc_xy21_w8,    mc_xy31_w8 },
            { mc_hv_w8,    mc_xy12_w8,    mc_hc_w8,      mc_xy32_w8 },
            { mc_xy03_w8,  mc_xy13_w8,    mc_xy23_w8,    mc_xy33_w8 },
        },
        {
            { mc_copy_w16,  mc_xy10_w16,    mc_hh_w16,      mc_xy30_w16 },
            { mc_xy01_w16,  mc_xy11_w16,    mc_xy21_w16,    mc_xy31_w16 },
            { mc_hv_w16,    mc_xy12_w16,    mc_hc_w16,      mc_xy32_w16 },
            { mc_xy03_w16,  mc_xy13_w16,    mc_xy23_w16,    mc_xy33_w16 },
        }
    };

    src += (mvy >> 2) * i_src + (mvx >> 2);
    if( i_width == 4 )
    {
        pf_mc[0][mvy&0x03][mvx&0x03]( src, i_src, dst, i_dst, i_height );
    }
    else if( i_width == 8 )
    {
        pf_mc[1][mvy&0x03][mvx&0x03]( src, i_src, dst, i_dst, i_height );
    }
    else if( i_width == 16 )
    {
        pf_mc[2][mvy&0x03][mvx&0x03]( src, i_src, dst, i_dst, i_height );
    }
}

void x264_mc_altivec_init( x264_mc_function_t pf[2] )
{
    pf[MC_LUMA] = motion_compensation_luma;
}
