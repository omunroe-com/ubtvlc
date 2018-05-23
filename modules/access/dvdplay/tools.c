/*****************************************************************************
 * tools.c: tools for dvd plugin.
 *****************************************************************************
 * Copyright (C) 2001 VideoLAN
 * $Id: tools.c 7675 2004-05-15 12:22:03Z gbazin $
 *
 * Author: St�phane Borel <stef@via.ecp.fr>
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
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

#include <vlc/vlc.h>

#include "stream_control.h"
#include "input_ext-intf.h"
#include "input_ext-dec.h"
#include "input_ext-plugins.h"

#include "dvd.h"

/*****************************************************************************
 * dvdplay_ParseCL: parse command line
 *****************************************************************************/
char * dvdplay_ParseCL( input_thread_t * p_input,
                       int * i_title, int * i_chapter, int * i_angle )
{
    dvd_data_t *            p_dvd;
    struct stat             stat_info;
    char *                  psz_parser;
    char *                  psz_source;
    char *                  psz_next;
    
    p_dvd = (dvd_data_t*)(p_input->p_access_data);

    psz_source = strdup( p_input->psz_name );
    if( psz_source == NULL )
    {
        return NULL;
    }

    *i_title = 0;
    *i_chapter = 1;
    *i_angle = 1;
    
    /* Start with the end, because you could have :
     * dvdplay:/Volumes/my@toto/VIDEO_TS@1,1
     * (yes, this is kludgy). */
    for ( psz_parser = psz_source + strlen(psz_source) - 1;
          psz_parser >= psz_source && *psz_parser != '@';
          psz_parser-- );

    if( psz_parser >= psz_source && *psz_parser == '@' )
    {
        /* Found options */
        *psz_parser = '\0';
        ++psz_parser;

        *i_title = (int)strtol( psz_parser, &psz_next, 10 );
        if( *psz_next )
        {
            psz_parser = psz_next + 1;
            *i_chapter = (int)strtol( psz_parser, &psz_next, 10 );
            if( *psz_next )
            {
                *i_angle = (int)strtol( psz_next + 1, NULL, 10 );
            }
        }
    }

    *i_title   = *i_title >= 0 ? *i_title : 0;
    *i_chapter = *i_chapter    ? *i_chapter : 1;
    *i_angle   = *i_angle      ? *i_angle : 1;

    if( !*psz_source )
    {
        free( psz_source );
        if( !p_input->psz_access )
        {
            return NULL;
        }
        psz_source = config_GetPsz( p_input, "dvd" );
        if( !psz_source ) return NULL;
    }

#ifdef WIN32
    if( psz_source[0] && psz_source[1] == ':' && psz_source[2] == '\0' )
    {
        /* Don't try to stat the file */
    }
    else if( psz_source[0] && psz_source[1] == ':' &&
             psz_source[2] == '\\' && psz_source[3] == '\0' )
    {
        /* Don't try to stat the file */
        psz_source[2] = '\0';
    }
    else
#endif
    {
        if( stat( psz_source, &stat_info ) == -1 )
        {
            msg_Warn( p_input, "cannot stat() source `%s' (%s)",
                               psz_source, strerror(errno) );
            free( psz_source );
            return NULL;
        }

        if( !S_ISBLK(stat_info.st_mode) &&
            !S_ISCHR(stat_info.st_mode) &&
            !S_ISDIR(stat_info.st_mode) )
        {
            msg_Dbg( p_input, "plugin discarded (not a valid source)" );
            free( psz_source );
            return NULL;
        }
    }
    
    msg_Dbg( p_input, "dvdroot=%s title=%d chapter=%d angle=%d",
                  psz_source,  *i_title, *i_chapter, *i_angle );
    
    return psz_source;
}
