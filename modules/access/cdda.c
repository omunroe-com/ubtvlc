/*****************************************************************************
 * cdda.c : CD digital audio input module for vlc
 *****************************************************************************
 * Copyright (C) 2000, 2003 VideoLAN
 * $Id: cdda.c 8606 2004-08-31 18:32:54Z hartman $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Gildas Bazin <gbazin@netcourrier.com>
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
#include <stdlib.h>

#include <vlc/vlc.h>
#include <vlc/input.h>

#include "codecs.h"
#include "vcd/cdrom.h"

/*****************************************************************************
 * Module descriptior
 *****************************************************************************/
static int  Open ( vlc_object_t * );
static void Close( vlc_object_t * );

#define CACHING_TEXT N_("Caching value in ms")
#define CACHING_LONGTEXT N_( \
    "Allows you to modify the default caching value for cdda streams. This " \
    "value should be set in milliseconds units." )

vlc_module_begin();
    set_description( _("Audio CD input") );
    set_capability( "access2", 10 );
    set_callbacks( Open, Close );

    add_usage_hint( N_("[cdda:][device][@[track]]") );
    add_integer( "cdda-caching", DEFAULT_PTS_DELAY / 1000, NULL, CACHING_TEXT,
                 CACHING_LONGTEXT, VLC_TRUE );
    add_shortcut( "cdda" );
    add_shortcut( "cddasimple" );
vlc_module_end();


/* how many blocks VCDRead will read in each loop */
#define CDDA_BLOCKS_ONCE 20
#define CDDA_DATA_ONCE   (CDDA_BLOCKS_ONCE * CDDA_DATA_SIZE)

/*****************************************************************************
 * Access: local prototypes
 *****************************************************************************/
struct access_sys_t
{
    vcddev_t    *vcddev;                            /* vcd device descriptor */

    /* Title infos */
    int           i_titles;
    input_title_t *title[99];         /* No more that 99 track in a cd-audio */

    /* Current position */
    int         i_sector;                                  /* Current Sector */
    int *       p_sectors;                                  /* Track sectors */

    /* Wave header for the output data */
    WAVEHEADER  waveheader;
    vlc_bool_t  b_header;
};

static block_t *Block( access_t * );
static int      Seek( access_t *, int64_t );
static int      Control( access_t *, int, va_list );

/*****************************************************************************
 * Open: open cdda
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    access_t     *p_access = (access_t*)p_this;
    access_sys_t *p_sys;

    vcddev_t *vcddev;
    char *psz_name;
    int  i;

    if( !p_access->psz_path || !*p_access->psz_path )
    {
        /* Only when selected */
        if( !p_this->b_force ) return VLC_EGENERIC;

        psz_name = var_CreateGetString( p_this, "cd-audio" );
        if( !psz_name || !*psz_name )
        {
            if( psz_name ) free( psz_name );
            return VLC_EGENERIC;
        }
    }
    else psz_name = strdup( p_access->psz_path );

    /* Open CDDA */
    if( (vcddev = ioctl_Open( VLC_OBJECT(p_access), psz_name )) == NULL )
    {
        msg_Warn( p_access, "could not open %s", psz_name );
        free( psz_name );
        return VLC_EGENERIC;
    }
    free( psz_name );

    /* Set up p_access */
    p_access->pf_read = NULL;
    p_access->pf_block = Block;
    p_access->pf_control = Control;
    p_access->pf_seek = Seek;
    p_access->info.i_update = 0;
    p_access->info.i_size = 0;
    p_access->info.i_pos = 0;
    p_access->info.b_eof = VLC_FALSE;
    p_access->info.i_title = 0;
    p_access->info.i_seekpoint = 0;
    p_access->p_sys = p_sys = malloc( sizeof( access_sys_t ) );
    memset( p_sys, 0, sizeof( access_sys_t ) );
    p_sys->vcddev = vcddev;
    p_sys->b_header = VLC_FALSE;

    /* We read the Table Of Content information */
    p_sys->i_titles = ioctl_GetTracksMap( VLC_OBJECT(p_access),
                                          p_sys->vcddev, &p_sys->p_sectors );
    if( p_sys->i_titles < 0 )
    {
        msg_Err( p_access, "unable to count tracks" );
        goto error;
    }
    else if( p_sys->i_titles <= 0 )
    {
        msg_Err( p_access, "no audio tracks found" );
        goto error;
    }

    /* Build title table */
    for( i = 0; i < p_sys->i_titles; i++ )
    {
        input_title_t *t = p_sys->title[i] = vlc_input_title_New();

        msg_Dbg( p_access, "title[%d] start=%d", i, p_sys->p_sectors[i] );
        msg_Dbg( p_access, "title[%d] end=%d", i, p_sys->p_sectors[i+1] );

        asprintf( &t->psz_name, _("Track %i"), i + 1 );
        t->i_size = ( p_sys->p_sectors[i+1] - p_sys->p_sectors[i] ) *
                    (int64_t)CDDA_DATA_SIZE;

        t->i_length = I64C(1000000) * t->i_size / 44100 / 4;
    }

    p_sys->i_sector = p_sys->p_sectors[0];
    p_access->info.i_size = p_sys->title[0]->i_size;

    /* Build a WAV header for the output data */
    memset( &p_sys->waveheader, 0, sizeof(WAVEHEADER) );
    SetWLE( &p_sys->waveheader.Format, 1 ); /*WAVE_FORMAT_PCM*/
    SetWLE( &p_sys->waveheader.BitsPerSample, 16);
    p_sys->waveheader.MainChunkID = VLC_FOURCC('R', 'I', 'F', 'F');
    p_sys->waveheader.Length = 0;                     /* we just don't know */
    p_sys->waveheader.ChunkTypeID = VLC_FOURCC('W', 'A', 'V', 'E');
    p_sys->waveheader.SubChunkID = VLC_FOURCC('f', 'm', 't', ' ');
    SetDWLE( &p_sys->waveheader.SubChunkLength, 16);
    SetWLE( &p_sys->waveheader.Modus, 2);
    SetDWLE( &p_sys->waveheader.SampleFreq, 44100);
    SetWLE( &p_sys->waveheader.BytesPerSample,
            2 /*Modus*/ * 16 /*BitsPerSample*/ / 8 );
    SetDWLE( &p_sys->waveheader.BytesPerSec,
             2*16/8 /*BytesPerSample*/ * 44100 /*SampleFreq*/ );
    p_sys->waveheader.DataChunkID = VLC_FOURCC('d', 'a', 't', 'a');
    p_sys->waveheader.DataLength = 0;                 /* we just don't know */

    /* PTS delay */
    var_Create( p_access, "cdda-caching", VLC_VAR_INTEGER|VLC_VAR_DOINHERIT );
    return VLC_SUCCESS;

error:
    ioctl_Close( VLC_OBJECT(p_access), p_sys->vcddev );
    free( p_sys );
    return VLC_EGENERIC;
}

/*****************************************************************************
 * Close: closes cdda
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    access_t     *p_access = (access_t *)p_this;
    access_sys_t *p_sys = p_access->p_sys;
    int          i;

    for( i = 0; i < p_sys->i_titles; i++ )
    {
        vlc_input_title_Delete( p_sys->title[i] );
    }

    ioctl_Close( p_this, p_sys->vcddev );
    free( p_sys );
}

/*****************************************************************************
 * Block: read data (CDDA_DATA_ONCE)
 *****************************************************************************/
static block_t *Block( access_t *p_access )
{
    access_sys_t *p_sys = p_access->p_sys;
    int i_blocks = CDDA_BLOCKS_ONCE;
    block_t *p_block;

    /* Check end of file */
    if( p_access->info.b_eof ) return NULL;

    if( !p_sys->b_header )
    {
        /* Return only the header */
        p_block = block_New( p_access, sizeof( WAVEHEADER ) );
        memcpy( p_block->p_buffer, &p_sys->waveheader, sizeof(WAVEHEADER) );
        p_sys->b_header = VLC_TRUE;
        return p_block;
    }

    /* Check end of title */
    while( p_sys->i_sector >= p_sys->p_sectors[p_access->info.i_title + 1] )
    {
        if( p_access->info.i_title + 1 >= p_sys->i_titles )
        {
            p_access->info.b_eof = VLC_TRUE;
            return NULL;
        }

        p_access->info.i_update |= INPUT_UPDATE_TITLE | INPUT_UPDATE_SIZE;
        p_access->info.i_title++;
        p_access->info.i_size = p_sys->title[p_access->info.i_title]->i_size;
        p_access->info.i_pos = 0;
    }

    /* Don't read after the end of a title */
    if( p_sys->i_sector + i_blocks >=
        p_sys->p_sectors[p_access->info.i_title + 1] )
    {
        i_blocks = p_sys->p_sectors[p_access->info.i_title + 1 ] -
                   p_sys->i_sector;
    }

    /* Do the actual reading */
    if( !( p_block = block_New( p_access, i_blocks * CDDA_DATA_SIZE ) ) )
    {
        msg_Err( p_access, "cannot get a new block of size: %i",
                 i_blocks * CDDA_DATA_SIZE );
        return NULL;
    }

    if( ioctl_ReadSectors( VLC_OBJECT(p_access), p_sys->vcddev,
            p_sys->i_sector, p_block->p_buffer, i_blocks, CDDA_TYPE ) < 0 )
    {
        msg_Err( p_access, "cannot read sector %i", p_sys->i_sector );
        block_Release( p_block );

        /* Try to skip one sector (in case of bad sectors) */
        p_sys->i_sector++;
        p_access->info.i_pos += CDDA_DATA_SIZE;
        return NULL;
    }

    /* Update a few values */
    p_sys->i_sector += i_blocks;
    p_access->info.i_pos += p_block->i_buffer;

    return p_block;
}

/****************************************************************************
 * Seek
 ****************************************************************************/
static int Seek( access_t *p_access, int64_t i_pos )
{
    access_sys_t *p_sys = p_access->p_sys;

    /* Next sector to read */
    p_sys->i_sector = p_sys->p_sectors[p_access->info.i_title] +
        i_pos / CDDA_DATA_SIZE;
    p_access->info.i_pos = i_pos;

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Control:
 *****************************************************************************/
static int Control( access_t *p_access, int i_query, va_list args )
{
    access_sys_t *p_sys = p_access->p_sys;
    vlc_bool_t   *pb_bool;
    int          *pi_int;
    int64_t      *pi_64;
    input_title_t ***ppp_title;
    int i;

    switch( i_query )
    {
        /* */
        case ACCESS_CAN_SEEK:
        case ACCESS_CAN_FASTSEEK:
        case ACCESS_CAN_PAUSE:
        case ACCESS_CAN_CONTROL_PACE:
            pb_bool = (vlc_bool_t*)va_arg( args, vlc_bool_t* );
            *pb_bool = VLC_TRUE;
            break;

        /* */
        case ACCESS_GET_MTU:
            pi_int = (int*)va_arg( args, int * );
            *pi_int = CDDA_DATA_ONCE;
            break;

        case ACCESS_GET_PTS_DELAY:
            pi_64 = (int64_t*)va_arg( args, int64_t * );
            *pi_64 = var_GetInteger( p_access, "cdda-caching" ) * 1000;
            break;

        /* */
        case ACCESS_SET_PAUSE_STATE:
            break;

        case ACCESS_GET_TITLE_INFO:
            ppp_title = (input_title_t***)va_arg( args, input_title_t*** );
            pi_int    = (int*)va_arg( args, int* );
	    *((int*)va_arg( args, int* )) = 1; /* Title offset */

            /* Duplicate title infos */
            *pi_int = p_sys->i_titles;
            *ppp_title = malloc(sizeof( input_title_t **) * p_sys->i_titles );
            for( i = 0; i < p_sys->i_titles; i++ )
            {
                (*ppp_title)[i] = vlc_input_title_Duplicate( p_sys->title[i] );
            }
            break;

        case ACCESS_SET_TITLE:
            i = (int)va_arg( args, int );
            if( i != p_access->info.i_title )
            {
                /* Update info */
                p_access->info.i_update |=
                    INPUT_UPDATE_TITLE|INPUT_UPDATE_SIZE;
                p_access->info.i_title = i;
                p_access->info.i_size = p_sys->title[i]->i_size;
                p_access->info.i_pos = 0;

                /* Next sector to read */
                p_sys->i_sector = p_sys->p_sectors[i];
            }
            break;

        case ACCESS_SET_SEEKPOINT:
        case ACCESS_SET_PRIVATE_ID_STATE:
            return VLC_EGENERIC;

        default:
            msg_Warn( p_access, "unimplemented query in control" );
            return VLC_EGENERIC;

    }
    return VLC_SUCCESS;
}
