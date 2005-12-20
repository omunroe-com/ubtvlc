/*****************************************************************************
 * macroblock.c: h264 encoder library
 *****************************************************************************
 * Copyright (C) 2003 Laurent Aimar
 * $Id: macroblock.c,v 1.1 2004/06/03 19:27:06 fenrir Exp $
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "macroblock.h"

static const uint8_t block_idx_x[16] =
{
    0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3
};
static const uint8_t block_idx_y[16] =
{
    0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 3, 3
};
static const uint8_t block_idx_xy[4][4] =
{
    { 0, 2, 8,  10},
    { 1, 3, 9,  11},
    { 4, 6, 12, 14},
    { 5, 7, 13, 15}
};

static const int dequant_mf[6][4][4] =
{
    { {10, 13, 10, 13}, {13, 16, 13, 16}, {10, 13, 10, 13}, {13, 16, 13, 16} },
    { {11, 14, 11, 14}, {14, 18, 14, 18}, {11, 14, 11, 14}, {14, 18, 14, 18} },
    { {13, 16, 13, 16}, {16, 20, 16, 20}, {13, 16, 13, 16}, {16, 20, 16, 20} },
    { {14, 18, 14, 18}, {18, 23, 18, 23}, {14, 18, 14, 18}, {18, 23, 18, 23} },
    { {16, 20, 16, 20}, {20, 25, 20, 25}, {16, 20, 16, 20}, {20, 25, 20, 25} },
    { {18, 23, 18, 23}, {23, 29, 23, 29}, {18, 23, 18, 23}, {23, 29, 23, 29} }
};

#if 0
static const int i_chroma_qp_table[52] =
{
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    29, 30, 31, 32, 32, 33, 34, 34, 35, 35,
    36, 36, 37, 37, 37, 38, 38, 38, 39, 39,
    39, 39
};
#endif

int x264_mb_predict_intra4x4_mode( x264_t *h, int idx )
{
    const int ma = h->mb.cache.intra4x4_pred_mode[x264_scan8[idx] - 1];
    const int mb = h->mb.cache.intra4x4_pred_mode[x264_scan8[idx] - 8];
    const int m  = X264_MIN( ma, mb );

    if( m < 0 )
        return I_PRED_4x4_DC;

    return m;
}

int x264_mb_predict_non_zero_code( x264_t *h, int idx )
{
    const int za = h->mb.cache.non_zero_count[x264_scan8[idx] - 1];
    const int zb = h->mb.cache.non_zero_count[x264_scan8[idx] - 8];

    int i_ret = za + zb;

    if( i_ret < 0x80 )
    {
        i_ret = ( i_ret + 1 ) >> 1;
    }
    return i_ret & 0x7f;
}

/****************************************************************************
 * Scan and Quant functions
 ****************************************************************************/
void x264_mb_dequant_2x2_dc( int16_t dct[2][2], int i_qscale )
{
    const int i_qbits = i_qscale/6 - 1;

    if( i_qbits >= 0 )
    {
        const int i_dmf = dequant_mf[i_qscale%6][0][0] << i_qbits;

        dct[0][0] = dct[0][0] * i_dmf;
        dct[0][1] = dct[0][1] * i_dmf;
        dct[1][0] = dct[1][0] * i_dmf;
        dct[1][1] = dct[1][1] * i_dmf;
    }
    else
    {
        const int i_dmf = dequant_mf[i_qscale%6][0][0];

        dct[0][0] = ( dct[0][0] * i_dmf ) >> 1;
        dct[0][1] = ( dct[0][1] * i_dmf ) >> 1;
        dct[1][0] = ( dct[1][0] * i_dmf ) >> 1;
        dct[1][1] = ( dct[1][1] * i_dmf ) >> 1;
    }
}

void x264_mb_dequant_4x4_dc( int16_t dct[4][4], int i_qscale )
{
    const int i_qbits = i_qscale/6 - 2;
    int x,y;

    if( i_qbits >= 0 )
    {
        const int i_dmf = dequant_mf[i_qscale%6][0][0] << i_qbits;

        for( y = 0; y < 4; y++ )
        {
            for( x = 0; x < 4; x++ )
            {
                dct[y][x] = dct[y][x] * i_dmf;
            }
        }
    }
    else
    {
        const int i_dmf = dequant_mf[i_qscale%6][0][0];
        const int f = 1 << ( 1 + i_qbits );

        for( y = 0; y < 4; y++ )
        {
            for( x = 0; x < 4; x++ )
            {
                dct[y][x] = ( dct[y][x] * i_dmf + f ) >> (-i_qbits);
            }
        }
    }
}

void x264_mb_dequant_4x4( int16_t dct[4][4], int i_qscale )
{
    const int i_mf = i_qscale%6;
    const int i_qbits = i_qscale/6;
    int y;

    for( y = 0; y < 4; y++ )
    {
        dct[y][0] = ( dct[y][0] * dequant_mf[i_mf][y][0] ) << i_qbits;
        dct[y][1] = ( dct[y][1] * dequant_mf[i_mf][y][1] ) << i_qbits;
        dct[y][2] = ( dct[y][2] * dequant_mf[i_mf][y][2] ) << i_qbits;
        dct[y][3] = ( dct[y][3] * dequant_mf[i_mf][y][3] ) << i_qbits;
    }
}

static inline int x264_median( int a, int b, int c )
{
    int min = a, max =a;
    if( b < min )
        min = b;
    else
        max = b;    /* no need to do 'b > max' (more consuming than always doing affectation) */

    if( c < min )
        min = c;
    else if( c > max )
        max = c;

    return a + b + c - min - max;
}

void x264_mb_predict_mv( x264_t *h, int i_list, int idx, int i_width, int mvp[2] )
{
    const int i8 = x264_scan8[idx];
    const int i_ref= h->mb.cache.ref[i_list][i8];
    int     i_refa = h->mb.cache.ref[i_list][i8 - 1];
    int16_t *mv_a  = h->mb.cache.mv[i_list][i8 - 1];
    int     i_refb = h->mb.cache.ref[i_list][i8 - 8];
    int16_t *mv_b  = h->mb.cache.mv[i_list][i8 - 8];
    int     i_refc = h->mb.cache.ref[i_list][i8 - 8 + i_width ];
    int16_t *mv_c  = h->mb.cache.mv[i_list][i8 - 8 + i_width];

    int i_count;

    if( (idx&0x03) == 3 || ( i_width == 2 && (idx&0x3) == 2 )|| i_refc == -2 )
    {
        i_refc = h->mb.cache.ref[i_list][i8 - 8 - 1];
        mv_c   = h->mb.cache.mv[i_list][i8 - 8 - 1];
    }

    if( h->mb.i_partition == D_16x8 )
    {
        if( idx == 0 && i_refb == i_ref )
        {
            mvp[0] = mv_b[0];
            mvp[1] = mv_b[1];
            return;
        }
        else if( idx != 0 && i_refa == i_ref )
        {
            mvp[0] = mv_a[0];
            mvp[1] = mv_a[1];
            return;
        }
    }
    else if( h->mb.i_partition == D_8x16 )
    {
        if( idx == 0 && i_refa == i_ref )
        {
            mvp[0] = mv_a[0];
            mvp[1] = mv_a[1];
            return;
        }
        else if( idx != 0 && i_refc == i_ref )
        {
            mvp[0] = mv_c[0];
            mvp[1] = mv_c[1];
            return;
        }
    }

    i_count = 0;
    if( i_refa == i_ref ) i_count++;
    if( i_refb == i_ref ) i_count++;
    if( i_refc == i_ref ) i_count++;

    if( i_count > 1 )
    {
        mvp[0] = x264_median( mv_a[0], mv_b[0], mv_c[0] );
        mvp[1] = x264_median( mv_a[1], mv_b[1], mv_c[1] );
    }
    else if( i_count == 1 )
    {
        if( i_refa == i_ref )
        {
            mvp[0] = mv_a[0];
            mvp[1] = mv_a[1];
        }
        else if( i_refb == i_ref )
        {
            mvp[0] = mv_b[0];
            mvp[1] = mv_b[1];
        }
        else
        {
            mvp[0] = mv_c[0];
            mvp[1] = mv_c[1];
        }
    }
    else if( i_refb == -2 && i_refc == -2 && i_refa != -2 )
    {
        mvp[0] = mv_a[0];
        mvp[1] = mv_a[1];
    }
    else
    {
        mvp[0] = x264_median( mv_a[0], mv_b[0], mv_c[0] );
        mvp[1] = x264_median( mv_a[1], mv_b[1], mv_c[1] );
    }
}

void x264_mb_predict_mv_16x16( x264_t *h, int i_list, int i_ref, int mvp[2] )
{
    int     i_refa = h->mb.cache.ref[i_list][X264_SCAN8_0 - 1];
    int16_t *mv_a  = h->mb.cache.mv[i_list][X264_SCAN8_0 - 1];
    int     i_refb = h->mb.cache.ref[i_list][X264_SCAN8_0 - 8];
    int16_t *mv_b  = h->mb.cache.mv[i_list][X264_SCAN8_0 - 8];
    int     i_refc = h->mb.cache.ref[i_list][X264_SCAN8_0 - 8 + 4];
    int16_t *mv_c  = h->mb.cache.mv[i_list][X264_SCAN8_0 - 8 + 4];

    int i_count;

    if( i_refc == -2 )
    {
        i_refc = h->mb.cache.ref[i_list][X264_SCAN8_0 - 8 - 1];
        mv_c   = h->mb.cache.mv[i_list][X264_SCAN8_0 - 8 - 1];
    }

    i_count = 0;
    if( i_refa == i_ref ) i_count++;
    if( i_refb == i_ref ) i_count++;
    if( i_refc == i_ref ) i_count++;

    if( i_count > 1 )
    {
        mvp[0] = x264_median( mv_a[0], mv_b[0], mv_c[0] );
        mvp[1] = x264_median( mv_a[1], mv_b[1], mv_c[1] );
    }
    else if( i_count == 1 )
    {
        if( i_refa == i_ref )
        {
            mvp[0] = mv_a[0];
            mvp[1] = mv_a[1];
        }
        else if( i_refb == i_ref )
        {
            mvp[0] = mv_b[0];
            mvp[1] = mv_b[1];
        }
        else
        {
            mvp[0] = mv_c[0];
            mvp[1] = mv_c[1];
        }
    }
    else if( i_refb == -2 && i_refc == -2 && i_refa != -2 )
    {
        mvp[0] = mv_a[0];
        mvp[1] = mv_a[1];
    }
    else
    {
        mvp[0] = x264_median( mv_a[0], mv_b[0], mv_c[0] );
        mvp[1] = x264_median( mv_a[1], mv_b[1], mv_c[1] );
    }
}


void x264_mb_predict_mv_pskip( x264_t *h, int mv[2] )
{
    int     i_refa = h->mb.cache.ref[0][X264_SCAN8_0 - 1];
    int     i_refb = h->mb.cache.ref[0][X264_SCAN8_0 - 8];
    int16_t *mv_a  = h->mb.cache.mv[0][X264_SCAN8_0 - 1];
    int16_t *mv_b  = h->mb.cache.mv[0][X264_SCAN8_0 - 8];

    if( i_refa == -2 || i_refb == -2 ||
        ( i_refa == 0 && mv_a[0] == 0 && mv_a[1] == 0 ) ||
        ( i_refb == 0 && mv_b[0] == 0 && mv_b[1] == 0 ) )
    {
        mv[0] = mv[1] = 0;
    }
    else
    {
        x264_mb_predict_mv_16x16( h, 0, 0, mv );
    }
}

static inline void x264_mb_mc_0xywh( x264_t *h, int x, int y, int width, int height )
{
    const int i8 = x264_scan8[0]+x+8*y;
    const int i_ref = h->mb.cache.ref[0][i8];
    const int mvx   = h->mb.cache.mv[0][i8][0];
    const int mvy   = h->mb.cache.mv[0][i8][1];

    h->mc[MC_LUMA]( &h->mb.pic.p_fref[0][i_ref][0][4*y * h->mb.pic.i_stride[0]+4*x], h->mb.pic.i_stride[0],
                    &h->mb.pic.p_fdec[0][4*y * h->mb.pic.i_stride[0]+4*x],           h->mb.pic.i_stride[0],
                    mvx, mvy, 4*width, 4*height );

    h->mc[MC_CHROMA]( &h->mb.pic.p_fref[0][i_ref][1][2*y*h->mb.pic.i_stride[1]+2*x], h->mb.pic.i_stride[1],
                      &h->mb.pic.p_fdec[1][2*y*h->mb.pic.i_stride[1]+2*x],           h->mb.pic.i_stride[1],
                      mvx, mvy, 2*width, 2*height );

    h->mc[MC_CHROMA]( &h->mb.pic.p_fref[0][i_ref][2][2*y*h->mb.pic.i_stride[2]+2*x], h->mb.pic.i_stride[2],
                      &h->mb.pic.p_fdec[2][2*y*h->mb.pic.i_stride[2]+2*x],           h->mb.pic.i_stride[2],
                      mvx, mvy, 2*width, 2*height );
}
static inline void x264_mb_mc_1xywh( x264_t *h, int x, int y, int width, int height )
{
    const int i8 = x264_scan8[0]+x+8*y;
    const int i_ref = h->mb.cache.ref[1][i8];
    const int mvx   = h->mb.cache.mv[1][i8][0];
    const int mvy   = h->mb.cache.mv[1][i8][1];

    h->mc[MC_LUMA]( &h->mb.pic.p_fref[1][i_ref][0][4*y * h->mb.pic.i_stride[0]+4*x], h->mb.pic.i_stride[0],
                    &h->mb.pic.p_fdec[0][4*y *h->mb.pic.i_stride[0]+4*x],            h->mb.pic.i_stride[0],
                    mvx, mvy, 4*width, 4*height );

    h->mc[MC_CHROMA]( &h->mb.pic.p_fref[1][i_ref][1][2*y*h->mb.pic.i_stride[1]+2*x], h->mb.pic.i_stride[1],
                      &h->mb.pic.p_fdec[1][2*y*h->mb.pic.i_stride[1]+2*x],           h->mb.pic.i_stride[1],
                      mvx, mvy, 2*width, 2*height );

    h->mc[MC_CHROMA]( &h->mb.pic.p_fref[1][i_ref][2][2*y*h->mb.pic.i_stride[2]+2*x], h->mb.pic.i_stride[2],
                      &h->mb.pic.p_fdec[2][2*y*h->mb.pic.i_stride[2]+2*x],           h->mb.pic.i_stride[2],
                      mvx, mvy, 2*width, 2*height );
}

static inline void x264_mb_mc_01xywh( x264_t *h, int x, int y, int width, int height )
{
    const int i8 = x264_scan8[0]+x+8*y;

    const int i_ref0 = h->mb.cache.ref[0][i8];
    const int mvx0   = h->mb.cache.mv[0][i8][0];
    const int mvy0   = h->mb.cache.mv[0][i8][1];

    const int i_ref1 = h->mb.cache.ref[1][i8];
    const int mvx1   = h->mb.cache.mv[1][i8][0];
    const int mvy1   = h->mb.cache.mv[1][i8][1];
    DECLARE_ALIGNED( uint8_t, tmp[16*16], 16 );
    int     i_mode = 0;

    if( width == 4 && height == 4 ) i_mode = PIXEL_16x16;
    else if( width == 4 && height == 2 ) i_mode = PIXEL_16x8;
    else if( width == 2 && height == 4 ) i_mode = PIXEL_8x16;
    else if( width == 2 && height == 2 ) i_mode = PIXEL_8x8;
    else if( width == 2 && height == 1 ) i_mode = PIXEL_8x4;
    else if( width == 1 && height == 2 ) i_mode = PIXEL_4x8;
    else if( width == 1 && height == 1 ) i_mode = PIXEL_4x4;

    h->mc[MC_LUMA]( &h->mb.pic.p_fref[0][i_ref0][0][4*y * h->mb.pic.i_stride[0]+4*x], h->mb.pic.i_stride[0],
                    &h->mb.pic.p_fdec[0][4*y *h->mb.pic.i_stride[0]+4*x],             h->mb.pic.i_stride[0],
                    mvx0, mvy0, 4*width, 4*height );
    h->mc[MC_CHROMA]( &h->mb.pic.p_fref[0][i_ref0][1][2*y*h->mb.pic.i_stride[1]+2*x], h->mb.pic.i_stride[1],
                      &h->mb.pic.p_fdec[1][2*y*h->mb.pic.i_stride[1]+2*x],            h->mb.pic.i_stride[1],
                      mvx0, mvy0, 2*width, 2*height );
    h->mc[MC_CHROMA]( &h->mb.pic.p_fref[0][i_ref0][2][2*y*h->mb.pic.i_stride[2]+2*x], h->mb.pic.i_stride[2],
                      &h->mb.pic.p_fdec[2][2*y*h->mb.pic.i_stride[2]+2*x],            h->mb.pic.i_stride[2],
                      mvx0, mvy0, 2*width, 2*height );


    h->mc[MC_LUMA]( &h->mb.pic.p_fref[1][i_ref1][0][4*y * h->mb.pic.i_stride[0]+4*x], h->mb.pic.i_stride[0],
                    tmp, 16, mvx1, mvy1, 4*width, 4*height );
    h->pixf.avg[i_mode]( &h->mb.pic.p_fdec[0][4*y *h->mb.pic.i_stride[0]+4*x], h->mb.pic.i_stride[0], tmp, 16 );

    h->mc[MC_CHROMA]( &h->mb.pic.p_fref[1][i_ref1][1][2*y*h->mb.pic.i_stride[1]+2*x], h->mb.pic.i_stride[1],
                      tmp, 16, mvx1, mvy1, 2*width, 2*height );
    h->pixf.avg[i_mode]( &h->mb.pic.p_fdec[1][2*y*h->mb.pic.i_stride[1]+2*x], h->mb.pic.i_stride[1], tmp, 16 );

    h->mc[MC_CHROMA]( &h->mb.pic.p_fref[1][i_ref1][2][2*y*h->mb.pic.i_stride[2]+2*x], h->mb.pic.i_stride[2],
                      tmp, 16, mvx1, mvy1, 2*width, 2*height );
    h->pixf.avg[i_mode]( &h->mb.pic.p_fdec[2][2*y*h->mb.pic.i_stride[2]+2*x], h->mb.pic.i_stride[2], tmp, 16 );
}


void x264_mb_mc( x264_t *h )
{
    if( h->mb.i_type == P_L0 )
    {
        if( h->mb.i_partition == D_16x16 )
        {
            x264_mb_mc_0xywh( h, 0, 0, 4, 4 );
        }
        else if( h->mb.i_partition == D_16x8 )
        {
            x264_mb_mc_0xywh( h, 0, 0, 4, 2 );
            x264_mb_mc_0xywh( h, 0, 2, 4, 2 );
        }
        else if( h->mb.i_partition == D_8x16 )
        {
            x264_mb_mc_0xywh( h, 0, 0, 2, 4 );
            x264_mb_mc_0xywh( h, 2, 0, 2, 4 );
        }
    }
    else if( h->mb.i_type == P_8x8 )
    {
        int i;
        for( i = 0; i < 4; i++ )
        {
            const int x = 2*(i%2);
            const int y = 2*(i/2);
            switch( h->mb.i_sub_partition[i] )
            {
                case D_L0_8x8:
                    x264_mb_mc_0xywh( h, x, y, 2, 2 );
                    break;
                case D_L0_8x4:
                    x264_mb_mc_0xywh( h, x, y+0, 2, 1 );
                    x264_mb_mc_0xywh( h, x, y+1, 2, 1 );
                    break;
                case D_L0_4x8:
                    x264_mb_mc_0xywh( h, x+0, y, 1, 2 );
                    x264_mb_mc_0xywh( h, x+1, y, 1, 2 );
                    break;
                case D_L0_4x4:
                    x264_mb_mc_0xywh( h, x+0, y+0, 1, 1 );
                    x264_mb_mc_0xywh( h, x+1, y+0, 1, 1 );
                    x264_mb_mc_0xywh( h, x+0, y+1, 1, 1 );
                    x264_mb_mc_0xywh( h, x+1, y+1, 1, 1 );
                    break;
            }
        }
    }
    else if( h->mb.i_type == B_8x8 || h->mb.i_type == B_DIRECT )
    {
        x264_log( h, X264_LOG_ERROR, "mc_luma with unsupported mb\n" );
        return;
    }
    else    /* B_*x* */
    {
        int b_list0[2];
        int b_list1[2];

        int i;

        /* init ref list utilisations */
        for( i = 0; i < 2; i++ )
        {
            b_list0[i] = x264_mb_type_list0_table[h->mb.i_type][i];
            b_list1[i] = x264_mb_type_list1_table[h->mb.i_type][i];
        }
        if( h->mb.i_partition == D_16x16 )
        {
            if( b_list0[0] && b_list1[0] ) x264_mb_mc_01xywh( h, 0, 0, 4, 4 );
            else if( b_list0[0] )          x264_mb_mc_0xywh ( h, 0, 0, 4, 4 );
            else if( b_list1[0] )          x264_mb_mc_1xywh ( h, 0, 0, 4, 4 );
        }
        else if( h->mb.i_partition == D_16x8 )
        {
            if( b_list0[0] && b_list1[0] ) x264_mb_mc_01xywh( h, 0, 0, 4, 2 );
            else if( b_list0[0] )          x264_mb_mc_0xywh ( h, 0, 0, 4, 2 );
            else if( b_list1[0] )          x264_mb_mc_1xywh ( h, 0, 0, 4, 2 );

            if( b_list0[1] && b_list1[1] ) x264_mb_mc_01xywh( h, 0, 2, 4, 2 );
            else if( b_list0[1] )          x264_mb_mc_0xywh ( h, 0, 2, 4, 2 );
            else if( b_list1[1] )          x264_mb_mc_1xywh ( h, 0, 2, 4, 2 );
        }
        else if( h->mb.i_partition == D_8x16 )
        {
            if( b_list0[0] && b_list1[0] ) x264_mb_mc_01xywh( h, 0, 0, 2, 4 );
            else if( b_list0[0] )          x264_mb_mc_0xywh ( h, 0, 0, 2, 4 );
            else if( b_list1[0] )          x264_mb_mc_1xywh ( h, 0, 0, 2, 4 );

            if( b_list0[1] && b_list1[1] ) x264_mb_mc_01xywh( h, 2, 0, 2, 4 );
            else if( b_list0[1] )          x264_mb_mc_0xywh ( h, 2, 0, 2, 4 );
            else if( b_list1[1] )          x264_mb_mc_1xywh ( h, 2, 0, 2, 4 );
        }
    }
}

void x264_macroblock_cache_init( x264_t *h )
{
    int i_mb_count  = h->sps->i_mb_width * h->sps->i_mb_height;

    h->mb.i_mb_stride = h->sps->i_mb_width;

    h->mb.type= x264_malloc( i_mb_count * sizeof( int8_t) );
    h->mb.qp  = x264_malloc( i_mb_count * sizeof( int8_t) );
    h->mb.cbp = x264_malloc( i_mb_count * sizeof( int16_t) );

    /* 0 -> 3 top(4), 4 -> 6 : left(3) */
    h->mb.intra4x4_pred_mode = x264_malloc( i_mb_count * 7 * sizeof( int8_t ) );

    /* all coeffs */
    h->mb.non_zero_count = x264_malloc( i_mb_count * 24 * sizeof( uint8_t ) );

    h->mb.mv[0]  = x264_malloc( 2*16 * i_mb_count * sizeof( int16_t ) );
    h->mb.mv[1]  = x264_malloc( 2*16 * i_mb_count * sizeof( int16_t ) );
    h->mb.ref[0] = x264_malloc( 4 * i_mb_count * sizeof( int16_t ) );
    h->mb.ref[1] = x264_malloc( 4 * i_mb_count * sizeof( int16_t ) );

    if( h->param.b_cabac )
    {
        h->mb.chroma_pred_mode = x264_malloc( i_mb_count * sizeof( int8_t) );
        h->mb.mvd[0] = x264_malloc( 2*16 * i_mb_count * sizeof( int16_t ) );
        h->mb.mvd[1] = x264_malloc( 2*16 * i_mb_count * sizeof( int16_t ) );
    }

    /* init with not avaiable (for top right idx=7,15) */
    memset( h->mb.cache.ref[0], -2, X264_SCAN8_SIZE * sizeof( int8_t ) );
    memset( h->mb.cache.ref[1], -2, X264_SCAN8_SIZE * sizeof( int8_t ) );
}
void x264_macroblock_cache_end( x264_t *h )
{
    if( h->param.b_cabac )
    {
        x264_free( h->mb.chroma_pred_mode );
        x264_free( h->mb.mvd[0] );
        x264_free( h->mb.mvd[1] );
    }
    x264_free( h->mb.mv[0] );
    x264_free( h->mb.mv[1] );
    x264_free( h->mb.ref[0] );
    x264_free( h->mb.ref[1] );
    x264_free( h->mb.intra4x4_pred_mode );
    x264_free( h->mb.non_zero_count );
    x264_free( h->mb.cbp );
    x264_free( h->mb.qp );
    x264_free( h->mb.type );
}


void x264_macroblock_cache_load( x264_t *h, int i_mb_x, int i_mb_y )
{
    const int i_mb_4x4 = 16 * h->mb.i_mb_stride *i_mb_y + 4 * i_mb_x;
    const int i_mb_8x8 =  4 * h->mb.i_mb_stride *i_mb_y + 2 * i_mb_x;

    int i_top_xy = -1;
    int i_left_xy = -1;
    int i_top_type = -1;    /* gcc warn */
    int i_left_type= -1;

    int i;

    /* init index */
    h->mb.i_mb_x = i_mb_x;
    h->mb.i_mb_y = i_mb_y;
    h->mb.i_mb_xy = i_mb_y * h->mb.i_mb_stride + i_mb_x;
    h->mb.i_neighbour = 0;

    /* load picture pointers */
    for( i = 0; i < 3; i++ )
    {
        const int w = (i == 0 ? 16 : 8);
        const int i_stride = h->fdec->i_stride[i];
        int   j;

        h->mb.pic.i_stride[i] = i_stride;

        h->mb.pic.p_fenc[i] = &h->fenc->plane[i][ w * ( i_mb_x + i_mb_y * i_stride )];

        h->mb.pic.p_fdec[i] = &h->fdec->plane[i][ w * ( i_mb_x + i_mb_y * i_stride )];

        for( j = 0; j < h->i_ref0; j++ )
        {
            h->mb.pic.p_fref[0][j][i] = &h->fref0[j]->plane[i][ w * ( i_mb_x + i_mb_y * i_stride )];
        }
        for( j = 0; j < h->i_ref1; j++ )
        {
            h->mb.pic.p_fref[1][j][i] = &h->fref1[j]->plane[i][ w * ( i_mb_x + i_mb_y * i_stride )];
        }
    }

    /* load cache */
    if( i_mb_y > 0 )
    {
        i_top_xy  = h->mb.i_mb_xy - h->mb.i_mb_stride;
        i_top_type= h->mb.type[i_top_xy];

        h->mb.i_neighbour |= MB_TOP;

        /* load intra4x4 */
        h->mb.cache.intra4x4_pred_mode[x264_scan8[0] - 8] = h->mb.intra4x4_pred_mode[i_top_xy][0];
        h->mb.cache.intra4x4_pred_mode[x264_scan8[1] - 8] = h->mb.intra4x4_pred_mode[i_top_xy][1];
        h->mb.cache.intra4x4_pred_mode[x264_scan8[4] - 8] = h->mb.intra4x4_pred_mode[i_top_xy][2];
        h->mb.cache.intra4x4_pred_mode[x264_scan8[5] - 8] = h->mb.intra4x4_pred_mode[i_top_xy][3];

        /* load non_zero_count */
        h->mb.cache.non_zero_count[x264_scan8[0] - 8] = h->mb.non_zero_count[i_top_xy][10];
        h->mb.cache.non_zero_count[x264_scan8[1] - 8] = h->mb.non_zero_count[i_top_xy][11];
        h->mb.cache.non_zero_count[x264_scan8[4] - 8] = h->mb.non_zero_count[i_top_xy][14];
        h->mb.cache.non_zero_count[x264_scan8[5] - 8] = h->mb.non_zero_count[i_top_xy][15];

        h->mb.cache.non_zero_count[x264_scan8[16+0] - 8] = h->mb.non_zero_count[i_top_xy][16+2];
        h->mb.cache.non_zero_count[x264_scan8[16+1] - 8] = h->mb.non_zero_count[i_top_xy][16+3];

        h->mb.cache.non_zero_count[x264_scan8[16+4+0] - 8] = h->mb.non_zero_count[i_top_xy][16+4+2];
        h->mb.cache.non_zero_count[x264_scan8[16+4+1] - 8] = h->mb.non_zero_count[i_top_xy][16+4+3];
    }
    else
    {
        /* load intra4x4 */
        h->mb.cache.intra4x4_pred_mode[x264_scan8[0] - 8] =
        h->mb.cache.intra4x4_pred_mode[x264_scan8[1] - 8] =
        h->mb.cache.intra4x4_pred_mode[x264_scan8[4] - 8] =
        h->mb.cache.intra4x4_pred_mode[x264_scan8[5] - 8] = -1;

        /* load non_zero_count */
        h->mb.cache.non_zero_count[x264_scan8[0] - 8] =
        h->mb.cache.non_zero_count[x264_scan8[1] - 8] =
        h->mb.cache.non_zero_count[x264_scan8[4] - 8] =
        h->mb.cache.non_zero_count[x264_scan8[5] - 8] =
        h->mb.cache.non_zero_count[x264_scan8[16+0] - 8] =
        h->mb.cache.non_zero_count[x264_scan8[16+1] - 8] =
        h->mb.cache.non_zero_count[x264_scan8[16+4+0] - 8] =
        h->mb.cache.non_zero_count[x264_scan8[16+4+1] - 8] = 0x80;

    }

    if( i_mb_x > 0 )
    {
        i_left_xy  = h->mb.i_mb_xy - 1;
        i_left_type= h->mb.type[i_left_xy];

        h->mb.i_neighbour |= MB_LEFT;

        /* load intra4x4 */
        h->mb.cache.intra4x4_pred_mode[x264_scan8[0 ] - 1] = h->mb.intra4x4_pred_mode[i_left_xy][4];
        h->mb.cache.intra4x4_pred_mode[x264_scan8[2 ] - 1] = h->mb.intra4x4_pred_mode[i_left_xy][5];
        h->mb.cache.intra4x4_pred_mode[x264_scan8[8 ] - 1] = h->mb.intra4x4_pred_mode[i_left_xy][6];
        h->mb.cache.intra4x4_pred_mode[x264_scan8[10] - 1] = h->mb.intra4x4_pred_mode[i_left_xy][3];

        /* load non_zero_count */
        h->mb.cache.non_zero_count[x264_scan8[0 ] - 1] = h->mb.non_zero_count[i_left_xy][5];
        h->mb.cache.non_zero_count[x264_scan8[2 ] - 1] = h->mb.non_zero_count[i_left_xy][7];
        h->mb.cache.non_zero_count[x264_scan8[8 ] - 1] = h->mb.non_zero_count[i_left_xy][13];
        h->mb.cache.non_zero_count[x264_scan8[10] - 1] = h->mb.non_zero_count[i_left_xy][15];

        h->mb.cache.non_zero_count[x264_scan8[16+0] - 1] = h->mb.non_zero_count[i_left_xy][16+1];
        h->mb.cache.non_zero_count[x264_scan8[16+2] - 1] = h->mb.non_zero_count[i_left_xy][16+3];

        h->mb.cache.non_zero_count[x264_scan8[16+4+0] - 1] = h->mb.non_zero_count[i_left_xy][16+4+1];
        h->mb.cache.non_zero_count[x264_scan8[16+4+2] - 1] = h->mb.non_zero_count[i_left_xy][16+4+3];
    }
    else
    {
        h->mb.cache.intra4x4_pred_mode[x264_scan8[0 ] - 1] =
        h->mb.cache.intra4x4_pred_mode[x264_scan8[2 ] - 1] =
        h->mb.cache.intra4x4_pred_mode[x264_scan8[8 ] - 1] =
        h->mb.cache.intra4x4_pred_mode[x264_scan8[10] - 1] = -1;

        /* load non_zero_count */
        h->mb.cache.non_zero_count[x264_scan8[0 ] - 1] =
        h->mb.cache.non_zero_count[x264_scan8[2 ] - 1] =
        h->mb.cache.non_zero_count[x264_scan8[8 ] - 1] =
        h->mb.cache.non_zero_count[x264_scan8[10] - 1] =
        h->mb.cache.non_zero_count[x264_scan8[16+0] - 1] =
        h->mb.cache.non_zero_count[x264_scan8[16+2] - 1] =
        h->mb.cache.non_zero_count[x264_scan8[16+4+0] - 1] =
        h->mb.cache.non_zero_count[x264_scan8[16+4+2] - 1] = 0x80;
    }

    if( i_mb_y > 0 && i_mb_x < h->sps->i_mb_width - 1 )
    {
        h->mb.i_neighbour |= MB_TOPRIGHT;
    }

    /* load ref/mv/mvd */
    if( h->sh.i_type != SLICE_TYPE_I )
    {
        int s8x8 = 2 * h->mb.i_mb_stride;
        int s4x4 = 4 * h->mb.i_mb_stride;

        int i_top_left_xy   = -1;
        int i_top_right_xy  = -1;

        int i_list;

        if( h->mb.i_mb_y > 0 && h->mb.i_mb_x > 0 )
        {
            i_top_left_xy   = i_top_xy - 1;
        }
        if( h->mb.i_mb_y > 0 && h->mb.i_mb_x < h->sps->i_mb_width - 1 )
        {
            i_top_right_xy = i_top_xy + 1;
        }

        for( i_list = 0; i_list < (h->sh.i_type == SLICE_TYPE_P ? 1  : 2 ); i_list++ )
        {
            /*
            h->mb.cache.ref[i_list][x264_scan8[5 ]+1] =
            h->mb.cache.ref[i_list][x264_scan8[7 ]+1] =
            h->mb.cache.ref[i_list][x264_scan8[13]+1] = -2;
            */

            if( i_top_left_xy >= 0 )
            {
                const int i8 = x264_scan8[0] - 1 - 1*8;
                const int ir = i_mb_8x8 - s8x8 - 1;
                const int iv = i_mb_4x4 - s4x4 - 1;
                h->mb.cache.ref[i_list][i8]  = h->mb.ref[i_list][ir];
                h->mb.cache.mv[i_list][i8][0] = h->mb.mv[i_list][iv][0];
                h->mb.cache.mv[i_list][i8][1] = h->mb.mv[i_list][iv][1];
            }
            else
            {
                const int i8 = x264_scan8[0] - 1 - 1*8;
                h->mb.cache.ref[i_list][i8] = -2;
                h->mb.cache.mv[i_list][i8][0] = 0;
                h->mb.cache.mv[i_list][i8][1] = 0;
            }

            if( i_top_xy >= 0 )
            {
                const int i8 = x264_scan8[0] - 8;
                const int ir = i_mb_8x8 - s8x8;
                const int iv = i_mb_4x4 - s4x4;

                h->mb.cache.ref[i_list][i8+0] =
                h->mb.cache.ref[i_list][i8+1] = h->mb.ref[i_list][ir + 0];
                h->mb.cache.ref[i_list][i8+2] =
                h->mb.cache.ref[i_list][i8+3] = h->mb.ref[i_list][ir + 1];

                for( i = 0; i < 4; i++ )
                {
                    h->mb.cache.mv[i_list][i8+i][0] = h->mb.mv[i_list][iv + i][0];
                    h->mb.cache.mv[i_list][i8+i][1] = h->mb.mv[i_list][iv + i][1];
                }
            }
            else
            {
                const int i8 = x264_scan8[0] - 8;
                for( i = 0; i < 4; i++ )
                {
                    h->mb.cache.ref[i_list][i8+i] = -2;
                    h->mb.cache.mv[i_list][i8+i][0] =
                    h->mb.cache.mv[i_list][i8+i][1] = 0;
                }
            }

            if( i_top_right_xy >= 0 )
            {
                const int i8 = x264_scan8[0] + 4 - 1*8;
                const int ir = i_mb_8x8 - s8x8 + 2;
                const int iv = i_mb_4x4 - s4x4 + 4;

                h->mb.cache.ref[i_list][i8]  = h->mb.ref[i_list][ir];
                h->mb.cache.mv[i_list][i8][0] = h->mb.mv[i_list][iv][0];
                h->mb.cache.mv[i_list][i8][1] = h->mb.mv[i_list][iv][1];
            }
            else
            {
                const int i8 = x264_scan8[0] + 4 - 1*8;
                h->mb.cache.ref[i_list][i8] = -2;
                h->mb.cache.mv[i_list][i8][0] = 0;
                h->mb.cache.mv[i_list][i8][1] = 0;
            }

            if( i_left_xy >= 0 )
            {
                const int i8 = x264_scan8[0] - 1;
                const int ir = i_mb_8x8 - 1;
                const int iv = i_mb_4x4 - 1;

                h->mb.cache.ref[i_list][i8+0*8] =
                h->mb.cache.ref[i_list][i8+1*8] = h->mb.ref[i_list][ir + 0*s8x8];
                h->mb.cache.ref[i_list][i8+2*8] =
                h->mb.cache.ref[i_list][i8+3*8] = h->mb.ref[i_list][ir + 1*s8x8];

                for( i = 0; i < 4; i++ )
                {
                    h->mb.cache.mv[i_list][i8+i*8][0] = h->mb.mv[i_list][iv + i*s4x4][0];
                    h->mb.cache.mv[i_list][i8+i*8][1] = h->mb.mv[i_list][iv + i*s4x4][1];
                }
            }
            else
            {
                const int i8 = x264_scan8[0] - 1;
                for( i = 0; i < 4; i++ )
                {
                    h->mb.cache.ref[i_list][i8+i*8] = -2;
                    h->mb.cache.mv[i_list][i8+i*8][0] =
                    h->mb.cache.mv[i_list][i8+i*8][1] = 0;
                }
            }

            if( h->param.b_cabac )
            {
                if( i_top_xy >= 0 )
                {
                    const int i8 = x264_scan8[0] - 8;
                    const int iv = i_mb_4x4 - s4x4;
                    for( i = 0; i < 4; i++ )
                    {
                        h->mb.cache.mvd[i_list][i8+i][0] = h->mb.mvd[i_list][iv + i][0];
                        h->mb.cache.mvd[i_list][i8+i][1] = h->mb.mvd[i_list][iv + i][1];
                    }
                }
                else
                {
                    const int i8 = x264_scan8[0] - 8;
                    for( i = 0; i < 4; i++ )
                    {
                        h->mb.cache.mvd[i_list][i8+i][0] =
                        h->mb.cache.mvd[i_list][i8+i][1] = 0;
                    }
                }

                if( i_left_xy >= 0 )
                {
                    const int i8 = x264_scan8[0] - 1;
                    const int iv = i_mb_4x4 - 1;
                    for( i = 0; i < 4; i++ )
                    {
                        h->mb.cache.mvd[i_list][i8+i*8][0] = h->mb.mvd[i_list][iv + i*s4x4][0];
                        h->mb.cache.mvd[i_list][i8+i*8][1] = h->mb.mvd[i_list][iv + i*s4x4][1];
                    }
                }
                else
                {
                    const int i8 = x264_scan8[0] - 1;
                    for( i = 0; i < 4; i++ )
                    {
                        h->mb.cache.mvd[i_list][i8+i*8][0] =
                        h->mb.cache.mvd[i_list][i8+i*8][1] = 0;
                    }
                }
            }
        }
    }
}

void x264_macroblock_cache_save( x264_t *h )
{
    const int i_mb_xy = h->mb.i_mb_xy;
    const int i_mb_type = h->mb.i_type;
    const int i_mb_4x4 = 16 * h->mb.i_mb_stride * h->mb.i_mb_y + 4 * h->mb.i_mb_x;
    const int i_mb_8x8 =  4 * h->mb.i_mb_stride * h->mb.i_mb_y + 2 * h->mb.i_mb_x;

    int i;

    if( IS_SKIP( h->mb.i_type ) )
        h->mb.qp[i_mb_xy] = h->mb.i_last_qp;

    h->mb.i_last_dqp = h->mb.qp[i_mb_xy] - h->mb.i_last_qp;
    h->mb.i_last_qp = h->mb.qp[i_mb_xy];

    /* save intra4x4 */
    if( i_mb_type == I_4x4 )
    {
        h->mb.intra4x4_pred_mode[i_mb_xy][0] = h->mb.cache.intra4x4_pred_mode[x264_scan8[10] ];
        h->mb.intra4x4_pred_mode[i_mb_xy][1] = h->mb.cache.intra4x4_pred_mode[x264_scan8[11] ];
        h->mb.intra4x4_pred_mode[i_mb_xy][2] = h->mb.cache.intra4x4_pred_mode[x264_scan8[14] ];
        h->mb.intra4x4_pred_mode[i_mb_xy][3] = h->mb.cache.intra4x4_pred_mode[x264_scan8[15] ];
        h->mb.intra4x4_pred_mode[i_mb_xy][4] = h->mb.cache.intra4x4_pred_mode[x264_scan8[5] ];
        h->mb.intra4x4_pred_mode[i_mb_xy][5] = h->mb.cache.intra4x4_pred_mode[x264_scan8[7] ];
        h->mb.intra4x4_pred_mode[i_mb_xy][6] = h->mb.cache.intra4x4_pred_mode[x264_scan8[13] ];
    }
    else
    {
        h->mb.intra4x4_pred_mode[i_mb_xy][0] =
        h->mb.intra4x4_pred_mode[i_mb_xy][1] =
        h->mb.intra4x4_pred_mode[i_mb_xy][2] =
        h->mb.intra4x4_pred_mode[i_mb_xy][3] =
        h->mb.intra4x4_pred_mode[i_mb_xy][4] =
        h->mb.intra4x4_pred_mode[i_mb_xy][5] =
        h->mb.intra4x4_pred_mode[i_mb_xy][6] = I_PRED_4x4_DC;
    }

    if( i_mb_type == I_PCM )
    {
        h->mb.cbp[i_mb_xy] = 0x72f;   /* all set */
        for( i = 0; i < 16 + 2*4; i++ )
        {
            h->mb.non_zero_count[i_mb_xy][i] = 16;
        }
    }
    else
    {
        /* save non zero count */
        for( i = 0; i < 16 + 2*4; i++ )
        {
            h->mb.non_zero_count[i_mb_xy][i] = h->mb.cache.non_zero_count[x264_scan8[i]];
        }
    }

    if( !IS_INTRA( i_mb_type ) )
    {
        int i_list;
        for( i_list = 0; i_list < (h->sh.i_type == SLICE_TYPE_P ? 1  : 2 ); i_list++ )
        {
            const int s8x8 = 2 * h->mb.i_mb_stride;
            const int s4x4 = 4 * h->mb.i_mb_stride;
            int y,x;

            h->mb.ref[i_list][i_mb_8x8+0+0*s8x8] = h->mb.cache.ref[i_list][x264_scan8[0]];
            h->mb.ref[i_list][i_mb_8x8+1+0*s8x8] = h->mb.cache.ref[i_list][x264_scan8[4]];
            h->mb.ref[i_list][i_mb_8x8+0+1*s8x8] = h->mb.cache.ref[i_list][x264_scan8[8]];
            h->mb.ref[i_list][i_mb_8x8+1+1*s8x8] = h->mb.cache.ref[i_list][x264_scan8[12]];

            for( y = 0; y < 4; y++ )
            {
                for( x = 0; x < 4; x++ )
                {
                    h->mb.mv[i_list][i_mb_4x4+x+y*s4x4][0] = h->mb.cache.mv[i_list][x264_scan8[0]+x+8*y][0];
                    h->mb.mv[i_list][i_mb_4x4+x+y*s4x4][1] = h->mb.cache.mv[i_list][x264_scan8[0]+x+8*y][1];
                }
            }
        }
    }
    else
    {
        int i_list;
        for( i_list = 0; i_list < (h->sh.i_type == SLICE_TYPE_P ? 1  : 2 ); i_list++ )
        {
            const int s8x8 = 2 * h->mb.i_mb_stride;
            const int s4x4 = 4 * h->mb.i_mb_stride;
            int y,x;

            h->mb.ref[i_list][i_mb_8x8+0+0*s8x8] =
            h->mb.ref[i_list][i_mb_8x8+1+0*s8x8] =
            h->mb.ref[i_list][i_mb_8x8+0+1*s8x8] =
            h->mb.ref[i_list][i_mb_8x8+1+1*s8x8] = -1;

            for( y = 0; y < 4; y++ )
            {
                for( x = 0; x < 4; x++ )
                {
                    h->mb.mv[i_list][i_mb_4x4+x+y*s4x4][0] = 0;
                    h->mb.mv[i_list][i_mb_4x4+x+y*s4x4][1] = 0;
                }
            }
        }
    }

    if( h->param.b_cabac )
    {
        if( i_mb_type == I_4x4 || i_mb_type == I_16x16 )
            h->mb.chroma_pred_mode[i_mb_xy] = h->mb.i_chroma_pred_mode;
        else
            h->mb.chroma_pred_mode[i_mb_xy] = I_PRED_CHROMA_DC;

        if( !IS_INTRA( i_mb_type ) && !IS_SKIP( i_mb_type ) )
        {
            int i_list;
            for( i_list  = 0; i_list < 2; i_list++ )
            {
                const int s4x4 = 4 * h->mb.i_mb_stride;
                int y,x;
                for( y = 0; y < 4; y++ )
                {
                    for( x = 0; x < 4; x++ )
                    {
                        h->mb.mvd[i_list][i_mb_4x4+x+y*s4x4][0] = h->mb.cache.mvd[i_list][x264_scan8[0]+x+8*y][0];
                        h->mb.mvd[i_list][i_mb_4x4+x+y*s4x4][1] = h->mb.cache.mvd[i_list][x264_scan8[0]+x+8*y][1];
                    }
                }
            }
        }
        else
        {
            int i_list;
            for( i_list  = 0; i_list < 2; i_list++ )
            {
                const int s4x4 = 4 * h->mb.i_mb_stride;
                int y,x;
                for( y = 0; y < 4; y++ )
                {
                    for( x = 0; x < 4; x++ )
                    {
                        h->mb.mvd[i_list][i_mb_4x4+x+y*s4x4][0] = 0;
                        h->mb.mvd[i_list][i_mb_4x4+x+y*s4x4][1] = 0;
                    }
                }
            }
        }
    }
}

