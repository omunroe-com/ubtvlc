;*****************************************************************************
;* pixel.asm: h264 encoder library
;*****************************************************************************
;* Copyright (C) 2003 x264 project
;* $Id: pixel.asm,v 1.1 2004/06/03 19:27:07 fenrir Exp $
;*
;* Authors: Laurent Aimar <fenrir@via.ecp.fr>
;*
;* This program is free software; you can redistribute it and/or modify
;* it under the terms of the GNU General Public License as published by
;* the Free Software Foundation; either version 2 of the License, or
;* (at your option) any later version.
;*
;* This program is distributed in the hope that it will be useful,
;* but WITHOUT ANY WARRANTY; without even the implied warranty of
;* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;* GNU General Public License for more details.
;*
;* You should have received a copy of the GNU General Public License
;* along with this program; if not, write to the Free Software
;* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
;*****************************************************************************

BITS 64

;=============================================================================
; Macros and other preprocessor constants
;=============================================================================

%include "amd64inc.asm"

%macro SAD_INC_2x16P 0
    movq    mm1,    [parm1q]
    movq    mm2,    [parm1q+8]
    movq    mm3,    [parm1q+parm2q]
    movq    mm4,    [parm1q+parm2q+8]
    psadbw  mm1,    [parm3q]
    psadbw  mm2,    [parm3q+8]
    psadbw  mm3,    [parm3q+parm4q]
    psadbw  mm4,    [parm3q+parm4q+8]
    lea     parm1q, [parm1q+2*parm2q]
    paddw   mm1,    mm2
    paddw   mm3,    mm4
    lea     parm3q, [parm3q+2*parm4q]
    paddw   mm0,    mm1
    paddw   mm0,    mm3
%endmacro

%macro SAD_INC_2x8P 0
    movq    mm1,    [parm1q]
    movq    mm2,    [parm1q+parm2q]
    psadbw  mm1,    [parm3q]
    psadbw  mm2,    [parm3q+parm4q]
    lea     parm1q,    [parm1q+2*parm2q]
    paddw   mm0,    mm1
    paddw   mm0,    mm2
    lea     parm3q, [parm3q+2*parm4q]
%endmacro

%macro SAD_INC_2x4P 0
    movd    mm1,    [parm1q]
    movd    mm2,    [parm3q]
    movd    mm3,    [parm1q+parm2q]
    movd    mm4,    [parm3q+parm4q]

    psadbw  mm1,    mm2
    psadbw  mm3,    mm4
    paddw   mm0,    mm1
    paddw   mm0,    mm3

    lea     parm1q, [parm1q+2*parm2q]
    lea     parm3q, [parm3q+2*parm4q]
%endmacro

%macro SSD_INC_1x16P 0
    movq    mm1,    [rax]
    movq    mm2,    [rcx]
    movq    mm3,    [rax+8]
    movq    mm4,    [rcx+8]

    movq    mm5,    mm2
    movq    mm6,    mm4
    psubusb mm2,    mm1
    psubusb mm4,    mm3
    psubusb mm1,    mm5
    psubusb mm3,    mm6
    por     mm1,    mm2
    por     mm3,    mm4

    movq    mm2,    mm1
    movq    mm4,    mm3
    punpcklbw mm1,  mm7
    punpcklbw mm3,  mm7
    punpckhbw mm2,  mm7
    punpckhbw mm4,  mm7
    pmaddwd mm1,    mm1
    pmaddwd mm2,    mm2
    pmaddwd mm3,    mm3
    pmaddwd mm4,    mm4

    add     rax,    rbx
    add     rcx,    rdx
    paddd   mm0,    mm1
    paddd   mm0,    mm2
    paddd   mm0,    mm3
    paddd   mm0,    mm4
%endmacro

%macro SSD_INC_1x8P 0
    movq    mm1,    [rax]
    movq    mm2,    [rcx]

    movq    mm5,    mm2
    psubusb mm2,    mm1
    psubusb mm1,    mm5
    por     mm1,    mm2         ; mm1 = 8bit abs diff

    movq    mm2,    mm1
    punpcklbw mm1,  mm7
    punpckhbw mm2,  mm7         ; (mm1,mm2) = 16bit abs diff
    pmaddwd mm1,    mm1
    pmaddwd mm2,    mm2

    add     rax,    rbx
    add     rcx,    rdx
    paddd   mm0,    mm1
    paddd   mm0,    mm2
%endmacro

%macro SSD_INC_1x4P 0
    movd    mm1,    [rax]
    movd    mm2,    [rcx]

    movq    mm5,    mm2
    psubusb mm2,    mm1
    psubusb mm1,    mm5
    por     mm1,    mm2
    punpcklbw mm1,  mm7
    pmaddwd mm1,    mm1

    add     rax,    rbx
    add     rcx,    rdx
    paddd   mm0,    mm1
%endmacro

%macro SSD_INC_8x16P 0
    SSD_INC_1x16P
    SSD_INC_1x16P
    SSD_INC_1x16P
    SSD_INC_1x16P
    SSD_INC_1x16P
    SSD_INC_1x16P
    SSD_INC_1x16P
    SSD_INC_1x16P
%endmacro

%macro SSD_INC_4x8P 0
    SSD_INC_1x8P
    SSD_INC_1x8P
    SSD_INC_1x8P
    SSD_INC_1x8P
%endmacro

%macro SSD_INC_4x4P 0
    SSD_INC_1x4P
    SSD_INC_1x4P
    SSD_INC_1x4P
    SSD_INC_1x4P
%endmacro

%macro LOAD_DIFF_4P 4  ; MMP, MMT, [pix1], [pix2]
    movd        %1, %3
    movd        %2, %4
    punpcklbw   %1, %2
    punpcklbw   %2, %2
    psubw       %1, %2
%endmacro

%macro LOAD_DIFF_INC_4x4 10 ; p1,p2,p3,p4, t, pix1, i_pix1, pix2, i_pix2, offset
    LOAD_DIFF_4P %1, %5, [%6+%10],    [%8+%10]
    LOAD_DIFF_4P %2, %5, [%6+%7+%10], [%8+%9+%10]
    lea %6, [%6+2*%7]
    lea %8, [%8+2*%9]
    LOAD_DIFF_4P %3, %5, [%6+%10],    [%8+%10]
    LOAD_DIFF_4P %4, %5, [%6+%7+%10], [%8+%9+%10]
    lea %6, [%6+2*%7]
    lea %8, [%8+2*%9]
%endmacro

%macro LOAD_DIFF_4x4 10 ; p1,p2,p3,p4, t, pix1, i_pix1, pix2, i_pix2, offset
    LOAD_DIFF_4P %1, %5, [%6+%10],      [%8+%10]
    LOAD_DIFF_4P %3, %5, [%6+2*%7+%10], [%8+2*%9+%10]
    add     %6, %7
    add     %8, %9
    LOAD_DIFF_4P %2, %5, [%6+%10],      [%8+%10]
    LOAD_DIFF_4P %4, %5, [%6+2*%7+%10], [%8+2*%9+%10]
%endmacro

%macro HADAMARD4_SUB_BADC 4
    paddw %1,   %2
    paddw %3,   %4
    paddw %2,   %2
    paddw %4,   %4
    psubw %2,   %1
    psubw %4,   %3
%endmacro

%macro HADAMARD4x4 4
    HADAMARD4_SUB_BADC %1, %2, %3, %4
    HADAMARD4_SUB_BADC %1, %3, %2, %4
%endmacro

%macro SBUTTERFLYwd 3
    movq        %3, %1
    punpcklwd   %1, %2
    punpckhwd   %3, %2
%endmacro

%macro SBUTTERFLYdq 3
    movq        %3, %1
    punpckldq   %1, %2
    punpckhdq   %3, %2
%endmacro

%macro TRANSPOSE4x4 5   ; abcd-t -> adtc
    SBUTTERFLYwd %1, %2, %5
    SBUTTERFLYwd %3, %4, %2
    SBUTTERFLYdq %1, %3, %4
    SBUTTERFLYdq %5, %2, %3
%endmacro

%macro MMX_ABS 2        ; mma, mmt
    pxor    %2, %2
    psubw   %2, %1
    pmaxsw  %1, %2
%endmacro

%macro HADAMARD4x4_SUM 1    ; %1 = dest (row sum of one block)
    HADAMARD4x4 mm4, mm5, mm6, mm7
    TRANSPOSE4x4 mm4, mm5, mm6, mm7, %1
    HADAMARD4x4 mm4, mm7, %1, mm6
    MMX_ABS     mm4, mm5
    MMX_ABS     mm7, mm5
    MMX_ABS     %1,  mm5
    MMX_ABS     mm6, mm5
    paddw       %1,  mm4
    paddw       mm6, mm7
    pavgw       %1,  mm6
%endmacro

;=============================================================================
; Code
;=============================================================================

SECTION .text

cglobal x264_pixel_sad_16x16_mmxext
cglobal x264_pixel_sad_16x8_mmxext
cglobal x264_pixel_sad_8x16_mmxext
cglobal x264_pixel_sad_8x8_mmxext
cglobal x264_pixel_sad_8x4_mmxext
cglobal x264_pixel_sad_4x8_mmxext
cglobal x264_pixel_sad_4x4_mmxext

cglobal x264_pixel_sad_pde_16x16_mmxext
cglobal x264_pixel_sad_pde_16x8_mmxext
cglobal x264_pixel_sad_pde_8x16_mmxext

cglobal x264_pixel_ssd_16x16_mmxext
cglobal x264_pixel_ssd_16x8_mmxext
cglobal x264_pixel_ssd_8x16_mmxext
cglobal x264_pixel_ssd_8x8_mmxext
cglobal x264_pixel_ssd_8x4_mmxext
cglobal x264_pixel_ssd_4x8_mmxext
cglobal x264_pixel_ssd_4x4_mmxext

cglobal x264_pixel_satd_4x4_mmxext
cglobal x264_pixel_satd_4x8_mmxext
cglobal x264_pixel_satd_8x4_mmxext
cglobal x264_pixel_satd_8x8_mmxext
cglobal x264_pixel_satd_16x8_mmxext
cglobal x264_pixel_satd_8x16_mmxext
cglobal x264_pixel_satd_16x16_mmxext

%macro SAD_START 0
    pxor    mm0,    mm0
%endmacro

%macro SAD_END 0
    movd    eax, mm0
    ret
%endmacro

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_sad_16x16_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_16x16_mmxext:
    SAD_START
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_sad_16x8_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_16x8_mmxext:
    SAD_START
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_sad_8x16_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_8x16_mmxext:
    SAD_START
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_sad_8x8_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_8x8_mmxext:
    SAD_START
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_sad_8x4_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_8x4_mmxext:
    SAD_START
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_sad_4x8_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_4x8_mmxext:
    SAD_START
    SAD_INC_2x4P
    SAD_INC_2x4P
    SAD_INC_2x4P
    SAD_INC_2x4P
    SAD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_sad_4x4_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_4x4_mmxext:
    SAD_START
    SAD_INC_2x4P
    SAD_INC_2x4P
    SAD_END



%macro PDE_CHECK 0
    movd eax, mm0
    cmp  eax, parm5d ; prev_score
    jl   .continue
    ret
ALIGN 4
.continue:
%endmacro

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_sad_pde_16x16_mmxext (uint8_t *, int, uint8_t *, int, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_pde_16x16_mmxext:
    SAD_START
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    PDE_CHECK
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_sad_pde_16x8_mmxext (uint8_t *, int, uint8_t *, int, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_pde_16x8_mmxext:
    SAD_START
    SAD_INC_2x16P
    SAD_INC_2x16P
    PDE_CHECK
    SAD_INC_2x16P
    SAD_INC_2x16P
    SAD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_sad_pde_8x16_mmxext (uint8_t *, int, uint8_t *, int, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_pde_8x16_mmxext:
    SAD_START
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    PDE_CHECK
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_INC_2x8P
    SAD_END



%macro SSD_START 0
    firstpush rbx
    pushreg   rbx
    endprolog

    mov     rax, parm1q         ; pix1 =parm1
    movsxd  rbx, parm2d         ; stride1 =parm2
%ifdef WIN64
    mov     rcx, parm3q
    movsxd  rdx, parm4d
%else
   xchg     rcx, rdx
%endif

    pxor    mm7,    mm7         ; zero
    pxor    mm0,    mm0         ; mm0 holds the sum
%endmacro

%macro SSD_END 0
    movq    mm1,    mm0
    psrlq   mm1,    32
    paddd   mm0,    mm1
    movd    eax,    mm0

    pop rbx
    ret
    endfunc
%endmacro

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_ssd_16x16_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_ssd_16x16_mmxext:
    SSD_START
    SSD_INC_8x16P
    SSD_INC_8x16P
    SSD_END

ALIGN 16
x264_pixel_ssd_16x8_mmxext:
    SSD_START
    SSD_INC_8x16P
    SSD_END

ALIGN 16
x264_pixel_ssd_8x16_mmxext:
    SSD_START
    SSD_INC_4x8P
    SSD_INC_4x8P
    SSD_INC_4x8P
    SSD_INC_4x8P
    SSD_END

ALIGN 16
x264_pixel_ssd_8x8_mmxext:
    SSD_START
    SSD_INC_4x8P
    SSD_INC_4x8P
    SSD_END

ALIGN 16
x264_pixel_ssd_8x4_mmxext:
    SSD_START
    SSD_INC_4x8P
    SSD_END

ALIGN 16
x264_pixel_ssd_4x8_mmxext:
    SSD_START
    SSD_INC_4x4P
    SSD_INC_4x4P
    SSD_END

ALIGN 16
x264_pixel_ssd_4x4_mmxext:
    SSD_START
    SSD_INC_4x4P
    SSD_END


%macro SATD_START 0
;   mov     rdi, rdi            ; pix1
;   movsxd  rsi, esi            ; stride1
;   mov     rdx, rdx            ; pix2
;   movsxd  rcx, ecx            ; stride2
%endmacro

%macro SATD_END 0
    pshufw      mm1, mm0, 01001110b
    paddw       mm0, mm1
    pshufw      mm1, mm0, 10110001b
    paddw       mm0, mm1
    movd        eax, mm0
    and         eax, 0xffff
    ret
%endmacro

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_satd_4x4_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_4x4_mmxext:
    SATD_START
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm0, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm0
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_satd_4x8_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_4x8_mmxext:
    SATD_START
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm0, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm0
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm1, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm1
    paddw       mm0, mm1
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_satd_8x4_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_8x4_mmxext:
    SATD_START
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm0, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm0
    sub         parm1q, parm2q
    sub         parm3q, parm4q
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm1, parm1q, parm2q, parm3q, parm4q, 4
    HADAMARD4x4_SUM     mm1
    paddw       mm0, mm1
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_satd_8x8_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_8x8_mmxext:
    SATD_START
    mov         r10, parm1q        ; pix1
    mov         r11, parm3q        ; pix2

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm0, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm0
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm1, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm1

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, r10, parm2q, r11, parm4q, 4
    HADAMARD4x4_SUM     mm2
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm3, r10, parm2q, r11, parm4q, 4
    HADAMARD4x4_SUM     mm3
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_satd_16x8_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_16x8_mmxext:
    SATD_START
    mov         r10, parm1q
    mov         r11, parm3q

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm0, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm0
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm1, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm1

    mov         parm1q, r10
    mov         parm3q, r11
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, parm1q, parm2q, parm3q, parm4q, 4
    HADAMARD4x4_SUM     mm2
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm3, parm1q, parm2q, parm3q, parm4q, 4
    HADAMARD4x4_SUM     mm3
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    mov         parm1q, r10
    mov         parm3q, r11
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm1, parm1q, parm2q, parm3q, parm4q, 8
    HADAMARD4x4_SUM     mm1
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm2, parm1q, parm2q, parm3q, parm4q, 8
    HADAMARD4x4_SUM     mm2
    paddw       mm1, mm2

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, r10, parm2q, r11, parm4q, 12
    HADAMARD4x4_SUM     mm2
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm3, r10, parm2q, r11, parm4q, 12
    HADAMARD4x4_SUM     mm3
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_satd_8x16_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_8x16_mmxext:
    SATD_START
    mov         r10, parm1q
    mov         r11, parm3q

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm0, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm0
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm1, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm1
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm2
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm3, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm3
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm1, r10, parm2q, r11, parm4q, 4
    HADAMARD4x4_SUM     mm1
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, r10, parm2q, r11, parm4q, 4
    HADAMARD4x4_SUM     mm2
    paddw       mm1, mm2

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, r10, parm2q, r11, parm4q, 4
    HADAMARD4x4_SUM     mm2
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm3, r10, parm2q, r11, parm4q, 4
    HADAMARD4x4_SUM     mm3
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2
    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int x264_pixel_satd_16x16_mmxext (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_16x16_mmxext:
    SATD_START
    mov         r10, parm1q
    mov         r11, parm3q

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm0, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm0
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm1, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm1
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm2
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm3, parm1q, parm2q, parm3q, parm4q, 0
    HADAMARD4x4_SUM     mm3
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    mov         parm1q, r10
    mov         parm3q, r11
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm1, parm1q, parm2q, parm3q, parm4q, 4
    HADAMARD4x4_SUM     mm1
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, parm1q, parm2q, parm3q, parm4q, 4
    HADAMARD4x4_SUM     mm2
    paddw       mm1, mm2

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, parm1q, parm2q, parm3q, parm4q, 4
    HADAMARD4x4_SUM     mm2
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm3, parm1q, parm2q, parm3q, parm4q, 4
    HADAMARD4x4_SUM     mm3
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    mov         parm1q, r10
    mov         parm3q, r11
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm1, parm1q, parm2q, parm3q, parm4q, 8
    HADAMARD4x4_SUM     mm1
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, parm1q, parm2q, parm3q, parm4q, 8
    HADAMARD4x4_SUM     mm2
    paddw       mm1, mm2

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, parm1q, parm2q, parm3q, parm4q, 8
    HADAMARD4x4_SUM     mm2
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm3, parm1q, parm2q, parm3q, parm4q, 8
    HADAMARD4x4_SUM     mm3
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm1, r10, parm2q, r11, parm4q, 12
    HADAMARD4x4_SUM     mm1
    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, r10, parm2q, r11, parm4q, 12
    HADAMARD4x4_SUM     mm2
    paddw       mm1, mm2

    LOAD_DIFF_INC_4x4   mm4, mm5, mm6, mm7, mm2, r10, parm2q, r11, parm4q, 12
    HADAMARD4x4_SUM     mm2
    LOAD_DIFF_4x4       mm4, mm5, mm6, mm7, mm3, r10, parm2q, r11, parm4q, 12
    HADAMARD4x4_SUM     mm3
    paddw       mm0, mm1
    paddw       mm2, mm3
    paddw       mm0, mm2

    pxor        mm3, mm3
    pshufw      mm1, mm0, 01001110b
    paddw       mm0, mm1
    punpcklwd   mm0, mm3
    pshufw      mm1, mm0, 01001110b
    paddd       mm0, mm1
    movd        eax, mm0
    ret

