/*****************************************************************************
 * dvb.c : DVB channel list import (szap/tzap/czap compatible channel lists)
 *****************************************************************************
 * Copyright (C) 2005 the VideoLAN team
 * $Id: dvb.c 16204 2006-08-03 16:58:10Z zorglub $
 *
 * Authors: Gildas Bazin <gbazin@videolan.org>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <vlc/vlc.h>
#include <vlc/input.h>
#include <vlc/intf.h>
#include "charset.h"

#include "playlist.h"

#ifndef LONG_MAX
#   define LONG_MAX 2147483647L
#   define LONG_MIN (-LONG_MAX-1)
#endif

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int Demux( demux_t *p_demux);
static int Control( demux_t *p_demux, int i_query, va_list args );

static int ParseLine( char *, char **, char ***, int *);

/*****************************************************************************
 * Import_DVB: main import function
 *****************************************************************************/
int E_(Import_DVB)( vlc_object_t *p_this )
{
    demux_t *p_demux = (demux_t *)p_this;
    uint8_t *p_peek;
    int     i_peek;
    vlc_bool_t b_valid = VLC_FALSE;

    if( !isExtension( p_demux, ".conf" ) && !p_demux->b_force )
        return VLC_EGENERIC;

    /* Check if this really is a channels file */
    if( (i_peek = stream_Peek( p_demux->s, &p_peek, 1024 )) > 0 )
    {
        char psz_line[1024+1];
        int i;

        for( i = 0; i < i_peek; i++ )
        {
            if( p_peek[i] == '\n' ) break;
            psz_line[i] = p_peek[i];
        }
        psz_line[i] = 0;

        if( ParseLine( psz_line, 0, 0, 0 ) ) b_valid = VLC_TRUE;
    }

    if( !b_valid ) return VLC_EGENERIC;

    msg_Dbg( p_demux, "found valid DVB conf playlist file");
    p_demux->pf_control = Control;
    p_demux->pf_demux = Demux;

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Deactivate: frees unused data
 *****************************************************************************/
void E_(Close_DVB)( vlc_object_t *p_this )
{
}

/*****************************************************************************
 * Demux: The important stuff
 *****************************************************************************/
static int Demux( demux_t *p_demux )
{
    char       *psz_line;
    INIT_PLAYLIST_STUFF;

    while( (psz_line = stream_ReadLine( p_demux->s )) )
    {
        char **ppsz_options = NULL;
        int  i, i_options = 0;
        char *psz_name = NULL;

        if( !ParseLine( psz_line, &psz_name, &ppsz_options, &i_options ) )
        {
            free( psz_line );
            continue;
        }

        EnsureUTF8( psz_name );

        p_input = input_ItemNewExt( p_playlist, "dvb:", psz_name, 0, NULL, -1 );
        for( i = 0; i< i_options; i++ )
        {
            EnsureUTF8( ppsz_options[i] );
            vlc_input_item_AddOption( p_input, ppsz_options[i] );
        }
        playlist_AddWhereverNeeded( p_playlist, p_input, p_current, 
                                    p_item_in_category,
                                    (i_parent_id > 0 ) ? VLC_TRUE: VLC_FALSE,
                                    PLAYLIST_APPEND );

        while( i_options-- ) free( ppsz_options[i_options] );
        if( ppsz_options ) free( ppsz_options );

        free( psz_line );
    }

    HANDLE_PLAY_AND_RELEASE;
    return VLC_SUCCESS;
}

static struct
{
    char *psz_name;
    char *psz_option;

} dvb_options[] =
{
    { "INVERSION_OFF", "dvb-inversion=0" },
    { "INVERSION_ON", "dvb-inversion=1" },
    { "INVERSION_AUTO", "dvb-inversion=2" },

    { "BANDWIDTH_AUTO", "dvb-bandwidth=0" },
    { "BANDWIDTH_6_MHZ", "dvb-bandwidth=6" },
    { "BANDWIDTH_7_MHZ", "dvb-bandwidth=7" },
    { "BANDWIDTH_8_MHZ", "dvb-bandwidth=8" },

    { "FEC_NONE", "dvb-fec=0" },
    { "FEC_1_2", "dvb-fec=1" },
    { "FEC_2_3", "dvb-fec=2" },
    { "FEC_3_4", "dvb-fec=3" },
    { "FEC_4_5", "dvb-fec=4" },
    { "FEC_5_6", "dvb-fec=5" },
    { "FEC_6_7", "dvb-fec=6" },
    { "FEC_7_8", "dvb-fec=7" },
    { "FEC_8_9", "dvb-fec=8" },
    { "FEC_AUTO", "dvb-fec=9" },

    { "GUARD_INTERVAL_AUTO", "dvb-guard=0" },
    { "GUARD_INTERVAL_1_4", "dvb-guard=4" },
    { "GUARD_INTERVAL_1_8", "dvb-guard=8" },
    { "GUARD_INTERVAL_1_16", "dvb-guard=16" },
    { "GUARD_INTERVAL_1_32", "dvb-guard=32" },

    { "HIERARCHY_NONE", "dvb-hierarchy=-1" },
    { "HIERARCHY_1", "dvb-hierarchy=1" },
    { "HIERARCHY_2", "dvb-hierarchy=2" },
    { "HIERARCHY_4", "dvb-hierarchy=4" },

    { "QPSK", "dvb-modulation=-1" },
    { "QAM_AUTO", "dvb-modulation=0" },
    { "QAM_16", "dvb-modulation=16" },
    { "QAM_32", "dvb-modulation=32" },
    { "QAM_64", "dvb-modulation=64" },
    { "QAM_128", "dvb-modulation=128" },
    { "QAM_256", "dvb-modulation=256" },

    { "TRANSMISSION_MODE_AUTO", "dvb-transmission=0" },
    { "TRANSMISSION_MODE_2K", "dvb-transmission=2" },
    { "TRANSMISSION_MODE_8K", "dvb-transmission=8" },
    { 0, 0 }

};

static int ParseLine( char *psz_line, char **ppsz_name,
                      char ***pppsz_options, int *pi_options )
{
    char *psz_name = 0, *psz_parse = psz_line;
    int i_count = 0, i_program = 0, i_frequency = 0;
    vlc_bool_t b_valid = VLC_FALSE;

    if( pppsz_options ) *pppsz_options = 0;
    if( pi_options ) *pi_options = 0;
    if( ppsz_name ) *ppsz_name = 0;

    /* Skip leading tabs and spaces */
    while( *psz_parse == ' ' || *psz_parse == '\t' ||
           *psz_parse == '\n' || *psz_parse == '\r' ) psz_parse++;

    /* Ignore comments */
    if( *psz_parse == '#' ) return VLC_FALSE;

    while( psz_parse )
    {
        char *psz_option = 0;
        char *psz_end = strchr( psz_parse, ':' );
        if( psz_end ) { *psz_end = 0; psz_end++; }

        if( i_count == 0 )
        {
            /* Channel name */
            psz_name = psz_parse;
        }
        else if( i_count == 1 )
        {
            /* Frequency */
            char *psz_end;
            long i_value;

            i_value = strtol( psz_parse, &psz_end, 10 );
            if( psz_end == psz_parse ||
                i_value == LONG_MAX || i_value == LONG_MIN ) break;

            i_frequency = i_value;
        }
        else
        {
            int i;

            /* Check option name with our list */
            for( i = 0; dvb_options[i].psz_name; i++ )
            {
                if( !strcmp( psz_parse, dvb_options[i].psz_name ) )
                {
                    psz_option = dvb_options[i].psz_option;

                    /* If we recognize one of the strings, then we are sure
                     * the data is really valid (ie. a channels file). */
                    b_valid = VLC_TRUE;
                    break;
                }
            }

            if( !psz_option )
            {
                /* Option not recognized, test if it is a number */
                char *psz_end;
                long i_value;

                i_value = strtol( psz_parse, &psz_end, 10 );
                if( psz_end != psz_parse &&
                    i_value != LONG_MAX && i_value != LONG_MIN )
                {
                    i_program = i_value;
                }
            }
        }

        if( psz_option && pppsz_options && pi_options )
        {
            psz_option = strdup( psz_option );
            INSERT_ELEM( *pppsz_options, (*pi_options), (*pi_options),
                         psz_option );
        }

        psz_parse = psz_end;
        i_count++;
    }

    if( !b_valid && pppsz_options && pi_options )
    {
        /* This isn't a valid channels file, cleanup everything */
        while( (*pi_options)-- ) free( (*pppsz_options)[*pi_options] );
        if( *pppsz_options ) free( *pppsz_options );
        *pppsz_options = 0; *pi_options = 0;
    }

    if( i_program && pppsz_options && pi_options )
    {
        char *psz_option;

        asprintf( &psz_option, "program=%i", i_program );
        INSERT_ELEM( *pppsz_options, (*pi_options), (*pi_options),
                     psz_option );
    }
    if( i_frequency && pppsz_options && pi_options )
    {
        char *psz_option;

        asprintf( &psz_option, "dvb-frequency=%i", i_frequency );
        INSERT_ELEM( *pppsz_options, (*pi_options), (*pi_options),
                     psz_option );
    }
    if( ppsz_name && psz_name ) *ppsz_name = strdup( psz_name );

    return b_valid;
}

static int Control( demux_t *p_demux, int i_query, va_list args )
{
    return VLC_EGENERIC;
}
