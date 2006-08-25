;*****************************************************************************
;* pixel-sse2.asm: h264 encoder library
;*****************************************************************************
;* Copyright (C) 2005 x264 project
;*
;* Authors: Alex Izvorski <aizvorksi@gmail.com>
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

BITS 32

;=============================================================================
; Macros and other preprocessor constants
;=============================================================================

%include "i386inc.asm"

SECTION_RODATA

pw_1:    times 8 dw 1
ssim_c1: times 4 dd 416    ; .01*.01*255*255*64
ssim_c2: times 4 dd 235963 ; .03*.03*255*255*64*63
mask_ff: times 16 db 0xff
         times 16 db 0


SECTION .text


cglobal x264_pixel_sad_16x16_sse2
cglobal x264_pixel_sad_16x8_sse2
cglobal x264_pixel_sad_x3_16x16_sse2
cglobal x264_pixel_sad_x3_16x8_sse2
cglobal x264_pixel_sad_x4_16x16_sse2
cglobal x264_pixel_sad_x4_16x8_sse2
cglobal x264_pixel_ssd_16x16_sse2
cglobal x264_pixel_ssd_16x8_sse2
cglobal x264_pixel_satd_8x4_sse2
cglobal x264_pixel_satd_8x8_sse2
cglobal x264_pixel_satd_16x8_sse2
cglobal x264_pixel_satd_8x16_sse2
cglobal x264_pixel_satd_16x16_sse2
cglobal x264_pixel_ssim_4x4x2_core_sse2
cglobal x264_pixel_ssim_end4_sse2


%macro SBUTTERFLY 5
    mov%1       %5, %3
    punpckl%2   %3, %4
    punpckh%2   %5, %4
%endmacro

%macro TRANSPOSE4x4D 5   ; abcd-t -> adtc
    SBUTTERFLY dqa, dq,  %1, %2, %5
    SBUTTERFLY dqa, dq,  %3, %4, %2
    SBUTTERFLY dqa, qdq, %1, %3, %4
    SBUTTERFLY dqa, qdq, %5, %2, %3
%endmacro


%macro SAD_INC_4x16P_SSE2 0
    movdqu  xmm1,   [ecx]
    movdqu  xmm2,   [ecx+edx]
    lea     ecx,    [ecx+2*edx]
    movdqu  xmm3,   [ecx]
    movdqu  xmm4,   [ecx+edx]
    psadbw  xmm1,   [eax]
    psadbw  xmm2,   [eax+ebx]
    lea     eax,    [eax+2*ebx]
    psadbw  xmm3,   [eax]
    psadbw  xmm4,   [eax+ebx]
    lea     eax,    [eax+2*ebx]
    lea     ecx,    [ecx+2*edx]
    paddw   xmm1,   xmm2
    paddw   xmm3,   xmm4
    paddw   xmm0,   xmm1
    paddw   xmm0,   xmm3
%endmacro

%macro SAD_START_SSE2 0
    push    ebx

    mov     eax,    [esp+ 8]    ; pix1
    mov     ebx,    [esp+12]    ; stride1
    mov     ecx,    [esp+16]    ; pix2
    mov     edx,    [esp+20]    ; stride2
%endmacro

%macro SAD_END_SSE2 0
    movdqa  xmm1, xmm0
    psrldq  xmm0,  8
    paddw   xmm0, xmm1
    movd    eax,  xmm0

    pop ebx
    ret
%endmacro

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_sad_16x16_sse2 (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_16x16_sse2:
    SAD_START_SSE2
    movdqu xmm0, [ecx]
    movdqu xmm1, [ecx+edx]
    lea    ecx,  [ecx+2*edx]
    movdqu xmm2, [ecx]
    movdqu xmm3, [ecx+edx]
    lea    ecx,  [ecx+2*edx]
    psadbw xmm0, [eax]
    psadbw xmm1, [eax+ebx]
    lea    eax,  [eax+2*ebx]
    movdqu xmm4, [ecx]
    paddw  xmm0, xmm1
    psadbw xmm2, [eax]
    psadbw xmm3, [eax+ebx]
    lea    eax,  [eax+2*ebx]
    movdqu xmm5, [ecx+edx]
    lea    ecx,  [ecx+2*edx]
    paddw  xmm2, xmm3
    movdqu xmm6, [ecx]
    movdqu xmm7, [ecx+edx]
    lea    ecx,  [ecx+2*edx]
    paddw  xmm0, xmm2
    psadbw xmm4, [eax]
    psadbw xmm5, [eax+ebx]
    lea    eax,  [eax+2*ebx]
    movdqu xmm1, [ecx]
    paddw  xmm4, xmm5
    psadbw xmm6, [eax]
    psadbw xmm7, [eax+ebx]
    lea    eax,  [eax+2*ebx]
    movdqu xmm2, [ecx+edx]
    lea    ecx,  [ecx+2*edx]
    paddw  xmm6, xmm7
    movdqu xmm3, [ecx]
    paddw  xmm0, xmm4
    movdqu xmm4, [ecx+edx]
    lea    ecx,  [ecx+2*edx]
    paddw  xmm0, xmm6
    psadbw xmm1, [eax]
    psadbw xmm2, [eax+ebx]
    lea    eax,  [eax+2*ebx]
    movdqu xmm5, [ecx]
    paddw  xmm1, xmm2
    psadbw xmm3, [eax]
    psadbw xmm4, [eax+ebx]
    lea    eax,  [eax+2*ebx]
    movdqu xmm6, [ecx+edx]
    lea    ecx,  [ecx+2*edx]
    paddw  xmm3, xmm4
    movdqu xmm7, [ecx]
    paddw  xmm0, xmm1
    movdqu xmm1, [ecx+edx]
    paddw  xmm0, xmm3
    psadbw xmm5, [eax]
    psadbw xmm6, [eax+ebx]
    lea    eax,  [eax+2*ebx]
    paddw  xmm5, xmm6
    psadbw xmm7, [eax]
    psadbw xmm1, [eax+ebx]
    paddw  xmm7, xmm1
    paddw  xmm0, xmm5
    paddw  xmm0, xmm7
    SAD_END_SSE2

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_sad_16x8_sse2 (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_sad_16x8_sse2:
    SAD_START_SSE2
    pxor    xmm0,   xmm0
    SAD_INC_4x16P_SSE2
    SAD_INC_4x16P_SSE2
    SAD_END_SSE2


%macro SAD_X3_START_1x16P 0
    push    edi
    push    esi
    mov     edi,    [esp+12]
    mov     eax,    [esp+16]
    mov     ecx,    [esp+20]
    mov     edx,    [esp+24]
    mov     esi,    [esp+28]
    movdqa  xmm3,   [edi]
    movdqu  xmm0,   [eax]
    movdqu  xmm1,   [ecx]
    movdqu  xmm2,   [edx]
    psadbw  xmm0,   xmm3
    psadbw  xmm1,   xmm3
    psadbw  xmm2,   xmm3
%endmacro

%macro SAD_X3_1x16P 2
    movdqa  xmm3,   [edi+%1]
    movdqu  xmm4,   [eax+%2]
    movdqu  xmm5,   [ecx+%2]
    movdqu  xmm6,   [edx+%2]
    psadbw  xmm4,   xmm3
    psadbw  xmm5,   xmm3
    psadbw  xmm6,   xmm3
    paddw   xmm0,   xmm4
    paddw   xmm1,   xmm5
    paddw   xmm2,   xmm6
%endmacro

%macro SAD_X3_2x16P 1
%if %1
    SAD_X3_START_1x16P
%else
    SAD_X3_1x16P 0, 0
%endif
    SAD_X3_1x16P FENC_STRIDE, esi
    add     edi, 2*FENC_STRIDE
    lea     eax, [eax+2*esi]
    lea     ecx, [ecx+2*esi]
    lea     edx, [edx+2*esi]
%endmacro

%macro SAD_X4_START_1x16P 0
    push    edi
    push    esi
    push    ebx
    mov     edi,    [esp+16]
    mov     eax,    [esp+20]
    mov     ebx,    [esp+24]
    mov     ecx,    [esp+28]
    mov     edx,    [esp+32]
    mov     esi,    [esp+36]
    movdqa  xmm7,   [edi]
    movdqu  xmm0,   [eax]
    movdqu  xmm1,   [ebx]
    movdqu  xmm2,   [ecx]
    movdqu  xmm3,   [edx]
    psadbw  xmm0,   xmm7
    psadbw  xmm1,   xmm7
    psadbw  xmm2,   xmm7
    psadbw  xmm3,   xmm7
%endmacro

%macro SAD_X4_1x16P 2
    movdqa  xmm7,   [edi+%1]
    movdqu  xmm4,   [eax+%2]
    movdqu  xmm5,   [ebx+%2]
    movdqu  xmm6,   [ecx+%2]
    psadbw  xmm4,   xmm7
    psadbw  xmm5,   xmm7
    paddw   xmm0,   xmm4
    psadbw  xmm6,   xmm7
    movdqu  xmm4,   [edx+%2]
    paddw   xmm1,   xmm5
    psadbw  xmm4,   xmm7
    paddw   xmm2,   xmm6
    paddw   xmm3,   xmm4
%endmacro

%macro SAD_X4_2x16P 1
%if %1
    SAD_X4_START_1x16P
%else
    SAD_X4_1x16P 0, 0
%endif
    SAD_X4_1x16P FENC_STRIDE, esi
    add     edi, 2*FENC_STRIDE
    lea     eax, [eax+2*esi]
    lea     ebx, [ebx+2*esi]
    lea     ecx, [ecx+2*esi]
    lea     edx, [edx+2*esi]
%endmacro

%macro SAD_X3_END 0
    mov     eax,  [esp+32]
    pshufd  xmm4, xmm0, 2
    pshufd  xmm5, xmm1, 2
    pshufd  xmm6, xmm2, 2
    paddw   xmm0, xmm4
    paddw   xmm1, xmm5
    paddw   xmm2, xmm6
    movd    [eax+0], xmm0
    movd    [eax+4], xmm1
    movd    [eax+8], xmm2
    pop     esi
    pop     edi
    ret
%endmacro

%macro SAD_X4_END 0
    mov     eax,  [esp+40]
    pshufd  xmm4, xmm0, 2
    pshufd  xmm5, xmm1, 2
    pshufd  xmm6, xmm2, 2
    pshufd  xmm7, xmm3, 2
    paddw   xmm0, xmm4
    paddw   xmm1, xmm5
    paddw   xmm2, xmm6
    paddw   xmm3, xmm7
    movd    [eax+0], xmm0
    movd    [eax+4], xmm1
    movd    [eax+8], xmm2
    movd    [eax+12], xmm3
    pop     ebx
    pop     esi
    pop     edi
    ret
%endmacro

ALIGN 16
;-----------------------------------------------------------------------------
;  void x264_pixel_sad_x3_16x16_sse2( uint8_t *fenc, uint8_t *pix0, uint8_t *pix1,
;                                     uint8_t *pix2, int i_stride, int scores[3] )
;-----------------------------------------------------------------------------
%macro SAD_X 3
ALIGN 16
x264_pixel_sad_x%1_%2x%3_sse2:
    SAD_X%1_2x%2P 1
%rep %3/2-1
    SAD_X%1_2x%2P 0
%endrep
    SAD_X%1_END
%endmacro

SAD_X 3, 16, 16
SAD_X 3, 16,  8
SAD_X 4, 16, 16
SAD_X 4, 16,  8


%macro SSD_INC_2x16P_SSE2 0
    movdqu  xmm1,   [eax]
    movdqu  xmm2,   [ecx]
    movdqu  xmm3,   [eax+ebx]
    movdqu  xmm4,   [ecx+edx]

    movdqa  xmm5,   xmm1
    movdqa  xmm6,   xmm3
    psubusb xmm1,   xmm2
    psubusb xmm3,   xmm4
    psubusb xmm2,   xmm5
    psubusb xmm4,   xmm6
    por     xmm1,   xmm2
    por     xmm3,   xmm4

    movdqa  xmm2,   xmm1
    movdqa  xmm4,   xmm3
    punpcklbw xmm1, xmm7
    punpckhbw xmm2, xmm7
    punpcklbw xmm3, xmm7
    punpckhbw xmm4, xmm7
    pmaddwd xmm1,   xmm1
    pmaddwd xmm2,   xmm2
    pmaddwd xmm3,   xmm3
    pmaddwd xmm4,   xmm4

    lea     eax,    [eax+2*ebx]
    lea     ecx,    [ecx+2*edx]

    paddd   xmm1,   xmm2
    paddd   xmm3,   xmm4
    paddd   xmm0,   xmm1
    paddd   xmm0,   xmm3
%endmacro

%macro SSD_START_SSE2 0
    push    ebx

    mov     eax,    [esp+ 8]    ; pix1
    mov     ebx,    [esp+12]    ; stride1
    mov     ecx,    [esp+16]    ; pix2
    mov     edx,    [esp+20]    ; stride2

    pxor    xmm7,   xmm7         ; zero
    pxor    xmm0,   xmm0         ; mm0 holds the sum
%endmacro

%macro SSD_END_SSE2 0
    movdqa  xmm1,   xmm0
    psrldq  xmm1,    8
    paddd   xmm0,   xmm1

    movdqa  xmm1,   xmm0
    psrldq  xmm1,    4
    paddd   xmm0,   xmm1

    movd    eax,    xmm0

    pop ebx
    ret
%endmacro

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_ssd_16x16_sse2 (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_ssd_16x16_sse2:
    SSD_START_SSE2
%rep 8
    SSD_INC_2x16P_SSE2
%endrep
    SSD_END_SSE2

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_ssd_16x8_sse2 (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_ssd_16x8_sse2:
    SSD_START_SSE2
%rep 4
    SSD_INC_2x16P_SSE2
%endrep
    SSD_END_SSE2

; %1=(row2, row0) %2=(row3, row1) %3=junk
; output in %1=(row3, row0) and %3=(row2, row1)
%macro HADAMARD4x4_SSE2 3
    movdqa     %3, %1
    paddw      %1, %2
    psubw      %3, %2
    movdqa     %2, %1
    punpcklqdq %1, %3
    punpckhqdq %2, %3
    movdqa     %3, %1
    paddw      %1, %2
    psubw      %3, %2
%endmacro

;;; two HADAMARD4x4_SSE2 running side-by-side
%macro HADAMARD4x4_TWO_SSE2 6    ; a02 a13 junk1 b02 b13 junk2 (1=4 2=5 3=6)
    movdqa     %3, %1
    movdqa     %6, %4
    paddw      %1, %2
    paddw      %4, %5
    psubw      %3, %2
    psubw      %6, %5
    movdqa     %2, %1
    movdqa     %5, %4
    punpcklqdq %1, %3
    punpcklqdq %4, %6
    punpckhqdq %2, %3
    punpckhqdq %5, %6
    movdqa     %3, %1
    movdqa     %6, %4
    paddw      %1, %2
    paddw      %4, %5
    psubw      %3, %2
    psubw      %6, %5
%endmacro

%macro TRANSPOSE4x4_TWIST_SSE2 3    ; %1=(row3, row0) %2=(row2, row1) %3=junk, output in %1 and %2
    movdqa     %3, %1
    punpcklwd  %1, %2
    punpckhwd  %2, %3             ; backwards because the high quadwords are already swapped

    movdqa     %3, %1
    punpckldq  %1, %2
    punpckhdq  %3, %2

    movdqa     %2, %1
    punpcklqdq %1, %3
    punpckhqdq %2, %3
%endmacro

;;; two TRANSPOSE4x4_TWIST_SSE2 running side-by-side
%macro TRANSPOSE4x4_TWIST_TWO_SSE2 6    ; a02 a13 junk1 b02 b13 junk2 (1=4 2=5 3=6)
    movdqa     %3, %1
    movdqa     %6, %4
    punpcklwd  %1, %2
    punpcklwd  %4, %5
    punpckhwd  %2, %3
    punpckhwd  %5, %6
    movdqa     %3, %1
    movdqa     %6, %4
    punpckldq  %1, %2
    punpckldq  %4, %5
    punpckhdq  %3, %2
    punpckhdq  %6, %5
    movdqa     %2, %1
    movdqa     %5, %4
    punpcklqdq %1, %3
    punpcklqdq %4, %6
    punpckhqdq %2, %3
    punpckhqdq %5, %6
%endmacro

;;; loads the difference of two 4x4 blocks into xmm0,xmm1 and xmm4,xmm5 in interleaved-row order
;;; destroys xmm2, 3
;;; the value in xmm7 doesn't matter: it's only subtracted from itself
%macro LOAD4x8_DIFF_SSE2 0
    movq      xmm0, [eax]
    movq      xmm4, [ecx]
    punpcklbw xmm0, xmm7
    punpcklbw xmm4, xmm7
    psubw     xmm0, xmm4

    movq      xmm1, [eax+ebx]
    movq      xmm5, [ecx+edx]
    lea       eax,  [eax+2*ebx]
    lea       ecx,  [ecx+2*edx]
    punpcklbw xmm1, xmm7
    punpcklbw xmm5, xmm7
    psubw     xmm1, xmm5

    movq       xmm2, [eax]
    movq       xmm4, [ecx]
    punpcklbw  xmm2, xmm7
    punpcklbw  xmm4, xmm7
    psubw      xmm2, xmm4
    movdqa     xmm4, xmm0
    punpcklqdq xmm0, xmm2        ; rows 0 and 2
    punpckhqdq xmm4, xmm2        ; next 4x4 rows 0 and 2

    movq       xmm3, [eax+ebx]
    movq       xmm5, [ecx+edx]
    lea        eax,  [eax+2*ebx]
    lea        ecx,  [ecx+2*edx]
    punpcklbw  xmm3, xmm7
    punpcklbw  xmm5, xmm7
    psubw      xmm3, xmm5
    movdqa     xmm5, xmm1
    punpcklqdq xmm1, xmm3        ; rows 1 and 3
    punpckhqdq xmm5, xmm3        ; next 4x4 rows 1 and 3
%endmacro

%macro SUM4x4_SSE2 4    ; 02 13 junk sum
    pxor    %3, %3
    psubw   %3, %1
    pmaxsw  %1, %3

    pxor    %3, %3
    psubw   %3, %2
    pmaxsw  %2, %3

    paddusw %4, %1
    paddusw %4, %2
%endmacro

;;; two SUM4x4_SSE2 running side-by-side
%macro SUM4x4_TWO_SSE2 7    ; a02 a13 junk1 b02 b13 junk2 (1=4 2=5 3=6) sum
    pxor    %3, %3
    pxor    %6, %6
    psubw   %3, %1
    psubw   %6, %4
    pmaxsw  %1, %3
    pmaxsw  %4, %6
    pxor    %3, %3
    pxor    %6, %6
    psubw   %3, %2
    psubw   %6, %5
    pmaxsw  %2, %3
    pmaxsw  %5, %6
    paddusw %1, %2
    paddusw %4, %5
    paddusw %7, %1
    paddusw %7, %4
%endmacro

%macro HADDW 2    ; sum junk
    ; ebx is no longer used at this point, so no push needed
    picgetgot ebx
    pmaddwd %1, [pw_1 GOT_ebx]
    movhlps %2, %1
    paddd   %1, %2
    pshuflw %2, %1, 0xE 
    paddd   %1, %2
%endmacro

%macro SATD_TWO_SSE2 0
    LOAD4x8_DIFF_SSE2
    HADAMARD4x4_TWO_SSE2        xmm0, xmm1, xmm2, xmm4, xmm5, xmm3
    TRANSPOSE4x4_TWIST_TWO_SSE2 xmm0, xmm2, xmm1, xmm4, xmm3, xmm5
    HADAMARD4x4_TWO_SSE2        xmm0, xmm2, xmm1, xmm4, xmm3, xmm5
    SUM4x4_TWO_SSE2             xmm0, xmm1, xmm2, xmm4, xmm5, xmm3, xmm6
%endmacro

%macro SATD_START 0
    push    ebx

    mov     eax,    [esp+ 8]    ; pix1
    mov     ebx,    [esp+12]    ; stride1
    mov     ecx,    [esp+16]    ; pix2
    mov     edx,    [esp+20]    ; stride2

    pxor    xmm6,    xmm6
%endmacro

%macro SATD_END 0
    ; each column sum of SATD is necessarily even, so we don't lose any precision by shifting first.
    psrlw   xmm6, 1
    HADDW   xmm6, xmm7
    movd    eax,  xmm6
    pop     ebx
    ret
%endmacro

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_satd_16x16_sse2 (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_16x16_sse2:
    SATD_START

    SATD_TWO_SSE2
    SATD_TWO_SSE2
    SATD_TWO_SSE2
    SATD_TWO_SSE2

    mov     eax,    [esp+ 8]
    mov     ecx,    [esp+16]
    lea     eax,    [eax+8]
    lea     ecx,    [ecx+8]

    SATD_TWO_SSE2
    SATD_TWO_SSE2
    SATD_TWO_SSE2
    SATD_TWO_SSE2

    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_satd_8x16_sse2 (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_8x16_sse2:
    SATD_START

    SATD_TWO_SSE2
    SATD_TWO_SSE2
    SATD_TWO_SSE2
    SATD_TWO_SSE2

    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_satd_16x8_sse2 (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_16x8_sse2:
    SATD_START

    SATD_TWO_SSE2
    SATD_TWO_SSE2

    mov     eax,    [esp+ 8]
    mov     ecx,    [esp+16]
    lea     eax,    [eax+8]
    lea     ecx,    [ecx+8]

    SATD_TWO_SSE2
    SATD_TWO_SSE2

    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_satd_8x8_sse2 (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_8x8_sse2:
    SATD_START

    SATD_TWO_SSE2
    SATD_TWO_SSE2

    SATD_END

ALIGN 16
;-----------------------------------------------------------------------------
;   int __cdecl x264_pixel_satd_8x4_sse2 (uint8_t *, int, uint8_t *, int )
;-----------------------------------------------------------------------------
x264_pixel_satd_8x4_sse2:
    SATD_START

    SATD_TWO_SSE2

    SATD_END



;-----------------------------------------------------------------------------
; void x264_pixel_ssim_4x4x2_core_sse2( const uint8_t *pix1, int stride1,
;                                       const uint8_t *pix2, int stride2, int sums[2][4] )
;-----------------------------------------------------------------------------
ALIGN 16
x264_pixel_ssim_4x4x2_core_sse2:
    push      ebx
    mov       eax,  [esp+ 8]
    mov       ebx,  [esp+12]
    mov       ecx,  [esp+16]
    mov       edx,  [esp+20]
    pxor      xmm0, xmm0
    pxor      xmm1, xmm1
    pxor      xmm2, xmm2
    pxor      xmm3, xmm3
    pxor      xmm4, xmm4
%rep 4
    movq      xmm5, [eax]
    movq      xmm6, [ecx]
    punpcklbw xmm5, xmm0
    punpcklbw xmm6, xmm0
    paddw     xmm1, xmm5
    paddw     xmm2, xmm6
    movdqa    xmm7, xmm5
    pmaddwd   xmm5, xmm5
    pmaddwd   xmm7, xmm6
    pmaddwd   xmm6, xmm6
    paddd     xmm3, xmm5
    paddd     xmm4, xmm7
    paddd     xmm3, xmm6
    add       eax,  ebx
    add       ecx,  edx
%endrep
    ; PHADDW xmm1, xmm2
    ; PHADDD xmm3, xmm4
    mov       eax,  [esp+24]
    picgetgot ebx
    movdqa    xmm7, [pw_1 GOT_ebx]
    pshufd    xmm5, xmm3, 0xB1
    pmaddwd   xmm1, xmm7
    pmaddwd   xmm2, xmm7
    pshufd    xmm6, xmm4, 0xB1
    packssdw  xmm1, xmm2
    paddd     xmm3, xmm5
    pmaddwd   xmm1, xmm7
    paddd     xmm4, xmm6
    pshufd    xmm1, xmm1, 0xD8
    movdqa    xmm5, xmm3
    punpckldq xmm3, xmm4
    punpckhdq xmm5, xmm4
    movq      [eax+ 0], xmm1
    movq      [eax+ 8], xmm3
    psrldq    xmm1, 8
    movq      [eax+16], xmm1
    movq      [eax+24], xmm5
    pop       ebx
    ret

;-----------------------------------------------------------------------------
; float x264_pixel_ssim_end_sse2( int sum0[5][4], int sum1[5][4], int width )
;-----------------------------------------------------------------------------
ALIGN 16
x264_pixel_ssim_end4_sse2:
    mov      eax,  [esp+ 4]
    mov      ecx,  [esp+ 8]
    mov      edx,  [esp+12]
    picpush  ebx
    picgetgot ebx
    movdqa   xmm0, [eax+ 0]
    movdqa   xmm1, [eax+16]
    movdqa   xmm2, [eax+32]
    movdqa   xmm3, [eax+48]
    movdqa   xmm4, [eax+64]
    paddd    xmm0, [ecx+ 0]
    paddd    xmm1, [ecx+16]
    paddd    xmm2, [ecx+32]
    paddd    xmm3, [ecx+48]
    paddd    xmm4, [ecx+64]
    paddd    xmm0, xmm1
    paddd    xmm1, xmm2
    paddd    xmm2, xmm3
    paddd    xmm3, xmm4
    movdqa   xmm5, [ssim_c1 GOT_ebx]
    movdqa   xmm6, [ssim_c2 GOT_ebx]
    TRANSPOSE4x4D  xmm0, xmm1, xmm2, xmm3, xmm4

;   s1=mm0, s2=mm3, ss=mm4, s12=mm2
    movdqa   xmm1, xmm3
    pslld    xmm3, 16
    pmaddwd  xmm1, xmm0  ; s1*s2
    por      xmm0, xmm3
    pmaddwd  xmm0, xmm0  ; s1*s1 + s2*s2
    pslld    xmm1, 1
    pslld    xmm2, 7
    pslld    xmm4, 6
    psubd    xmm2, xmm1  ; covar*2
    psubd    xmm4, xmm0  ; vars
    paddd    xmm0, xmm5
    paddd    xmm1, xmm5
    paddd    xmm2, xmm6
    paddd    xmm4, xmm6
    cvtdq2ps xmm0, xmm0  ; (float)(s1*s1 + s2*s2 + ssim_c1)
    cvtdq2ps xmm1, xmm1  ; (float)(s1*s2*2 + ssim_c1)
    cvtdq2ps xmm2, xmm2  ; (float)(covar*2 + ssim_c2)
    cvtdq2ps xmm4, xmm4  ; (float)(vars + ssim_c2)
    mulps    xmm1, xmm2
    mulps    xmm0, xmm4
    divps    xmm1, xmm0  ; ssim

    neg      edx
    movdqu   xmm3, [mask_ff + edx*4 + 16 GOT_ebx]
    pand     xmm1, xmm3
    movhlps  xmm0, xmm1
    addps    xmm0, xmm1
    pshuflw  xmm1, xmm0, 0xE
    addss    xmm0, xmm1

    movd     [picesp+4], xmm0
    fld      dword [picesp+4]
    picpop   ebx
    ret

