/*****************************************************************************
 * renderer.c : dummy text rendering functions
 *****************************************************************************
 * Copyright (C) 2000, 2001 VideoLAN
 * $Id: renderer.c 8354 2004-08-03 00:11:32Z gbazin $
 *
 * Authors: Sigmund Augdal <sigmunau@idi.ntnu.no>
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

#include <vlc/vlc.h>
#include <vlc/vout.h>
#include "vlc_block.h"
#include "vlc_filter.h"

static subpicture_t *RenderText( filter_t *, block_t * );

int E_(OpenRenderer)( vlc_object_t *p_this )
{
    filter_t *p_filter = (filter_t *)p_this;
    p_filter->pf_render_string = RenderText;
    return VLC_SUCCESS;
}

static subpicture_t *RenderText( filter_t *p_filter, block_t *p_block )
{
    return NULL;
}
