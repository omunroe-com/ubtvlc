/*****************************************************************************
 * macroblock.h: h264 encoder library
 *****************************************************************************
 * Copyright (C) 2003 Laurent Aimar
 * $Id: macroblock.h,v 1.1 2004/06/03 19:27:08 fenrir Exp $
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

#ifndef _ENCODER_MACROBLOCK_H
#define _ENCODER_MACROBLOCK_H 1

#include "../core/macroblock.h"

int x264_macroblock_probe_pskip( x264_t *h );

void x264_macroblock_encode      ( x264_t *h );
void x264_macroblock_write_cabac ( x264_t *h, bs_t *s );
void x264_macroblock_write_cavlc ( x264_t *h, bs_t *s );

void x264_cabac_mb_skip( x264_t *h, int b_skip );

#endif

