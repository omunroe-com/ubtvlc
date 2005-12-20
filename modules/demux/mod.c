/*****************************************************************************
 * mod.c: MOD file demuxer (using libmodplug)
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 * $Id: mod.c 6963 2004-03-05 19:24:14Z murray $
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

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#include <stdlib.h>                                      /* malloc(), free() */

#include <vlc/vlc.h>
#include <vlc/input.h>

#include <libmodplug/modplug.h>

/* TODO:
 *  - extend demux control to query meta data (demuxer should NEVER touch
 *      playlist itself)
 *  - FIXME test endian of samples
 *  - ...
 */

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open    ( vlc_object_t * );
static void Close  ( vlc_object_t * );

vlc_module_begin();
    set_description( _("MOD demuxer (libmodplug)" ) );
    set_capability( "demux2", 10 );

    add_bool( "mod-noisereduction", VLC_TRUE, NULL, N_("Noise reduction"), N_("Noise reduction"), VLC_FALSE );

    add_bool( "mod-reverb", VLC_FALSE, NULL, N_("Reverb"), N_("Reverb"), VLC_FALSE );
    add_integer_with_range( "mod-reverb-level", 0, 0, 100, NULL, N_("Reverb level (0-100)"), N_("Reverb level (0-100 defaults to 0)"), VLC_FALSE );
    add_integer_with_range( "mod-reverb-delay", 40, 0, 1000, NULL, N_("Reverb delay (ms)"), N_("Reverb delay in ms (usually 40-200ms)"), VLC_FALSE );

    add_bool( "mod-megabass", VLC_FALSE, NULL, N_("Mega bass"), N_("Mega bass"), VLC_FALSE );
    add_integer_with_range( "mod-megabass-level", 0, 0, 100, NULL, N_("Mega bass level (0-100)"), N_("Mega bass level (0-100 defaults to 0)"), VLC_FALSE );
    add_integer_with_range( "mod-megabass-range", 10, 10, 100, NULL, N_("Mega bass cut off (Hz)"), N_("Mega bass cut off (10-100Hz)"), VLC_FALSE );

    add_bool( "mod-surround", VLC_FALSE, NULL, N_("Surround"), N_("Surround"), VLC_FALSE );
    add_integer_with_range( "mod-surround-level", 0, 0, 100, NULL, N_("Surround level (0-100)"), N_("Surround level (0-100 defaults to 0)"), VLC_FALSE );
    add_integer_with_range( "mod-surround-delay", 5, 0, 1000, NULL, N_("Surround delay (ms)"), N_("Surround delay in ms (usually 5-40ms)"), VLC_FALSE );

    set_callbacks( Open, Close );
    add_shortcut( "mod" );
vlc_module_end();

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/

struct demux_sys_t
{
    es_format_t  fmt;
    es_out_id_t *es;

    int64_t     i_time;
    int64_t     i_length;

    int         i_data;
    uint8_t     *p_data;
    ModPlugFile *f;
};

static int Demux  ( demux_t *p_demux );
static int Control( demux_t *p_demux, int i_query, va_list args );

static const char* mod_ext[] =
{
    "mod", "s3m", "xm",  "it",  "669", "amf", "ams", "dbm", "dmf", "dsm",
    "far", "mdl", "med", "mtm", "okt", "ptm", "stm", "ult", "umx", "mt2",
    "psm", NULL
};

/*****************************************************************************
 * Open
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    demux_t     *p_demux = (demux_t*)p_this;
    demux_sys_t *p_sys;
    char        *ext;
    int         i;
    ModPlug_Settings settings;
    vlc_value_t val;

    /* We accept file based on extention match */
    if( strcasecmp( p_demux->psz_demux, "mod" ) )
    {
        if( ( ext = strchr( p_demux->psz_path, '.' ) ) == NULL || stream_Size( p_demux->s ) == 0 )
        {
            msg_Warn( p_demux, "MOD module discarded (path=%s)", p_demux->psz_path );
            return VLC_EGENERIC;
        }
        ext++;  /* skip . */
        for( i = 0; mod_ext[i] != NULL; i++ )
        {
            if( !strcasecmp( ext, mod_ext[i] ) )
            {
                break;
            }
        }
        if( mod_ext[i] == NULL )
        {
            msg_Warn( p_demux, "MOD module discarded (extention '%s' unknown)", ext );
            return VLC_EGENERIC;
        }
        msg_Dbg( p_demux, "running MOD demuxer (ext=%s)", mod_ext[i] );
    }

    /* Fill p_demux field */
    p_demux->pf_demux = Demux;
    p_demux->pf_control = Control;
    p_demux->p_sys = p_sys = malloc( sizeof( demux_sys_t ) );

    msg_Dbg( p_demux, "loading complete file (could be long)" );
    p_sys->i_data = stream_Size( p_demux->s );
    p_sys->p_data = malloc( p_sys->i_data );
    p_sys->i_data = stream_Read( p_demux->s, p_sys->p_data, p_sys->i_data );
    if( p_sys->i_data <= 0 )
    {
        msg_Err( p_demux, "failed to read the complete file" );
        free( p_sys->p_data );
        free( p_sys );
        return VLC_EGENERIC;
    }
    /* Create our config variable */
    var_Create( p_demux, "mod-noisereduction", VLC_VAR_BOOL|VLC_VAR_DOINHERIT );
    var_Create( p_demux, "mod-reverb", VLC_VAR_BOOL|VLC_VAR_DOINHERIT );
    var_Create( p_demux, "mod-reverb-level", VLC_VAR_INTEGER|VLC_VAR_DOINHERIT );
    var_Create( p_demux, "mod-reverb-delay", VLC_VAR_INTEGER|VLC_VAR_DOINHERIT );
    var_Create( p_demux, "mod-megabass", VLC_VAR_BOOL|VLC_VAR_DOINHERIT );
    var_Create( p_demux, "mod-megabass-level", VLC_VAR_INTEGER|VLC_VAR_DOINHERIT );
    var_Create( p_demux, "mod-megabass-range", VLC_VAR_INTEGER|VLC_VAR_DOINHERIT );
    var_Create( p_demux, "mod-surround", VLC_VAR_BOOL|VLC_VAR_DOINHERIT );
    var_Create( p_demux, "mod-surround-level", VLC_VAR_INTEGER|VLC_VAR_DOINHERIT );
    var_Create( p_demux, "mod-surround-delay", VLC_VAR_INTEGER|VLC_VAR_DOINHERIT );

    /* Configure modplug before loading the file */
    ModPlug_GetSettings( &settings );
    settings.mFlags = MODPLUG_ENABLE_OVERSAMPLING;
    settings.mChannels = 2;
    settings.mBits = 16;
    settings.mFrequency = 44100;
    settings.mResamplingMode = MODPLUG_RESAMPLE_FIR;

    var_Get( p_demux, "mod-noisereduction", &val );
    if( val.b_bool) settings.mFlags |= MODPLUG_ENABLE_NOISE_REDUCTION;

    var_Get( p_demux, "mod-reverb", &val );
    if( val.b_bool) settings.mFlags |= MODPLUG_ENABLE_REVERB;
    var_Get( p_demux, "mod-reverb-level", &val );
    settings.mReverbDepth = val.i_int;
    var_Get( p_demux, "mod-reverb-delay", &val );
    settings.mReverbDelay = val.i_int;

    var_Get( p_demux, "mod-megabass", &val );
    if( val.b_bool) settings.mFlags |= MODPLUG_ENABLE_MEGABASS;
    var_Get( p_demux, "mod-megabass-level", &val );
    settings.mBassAmount = val.i_int;
    var_Get( p_demux, "mod-megabass-range", &val );
    settings.mBassRange = val.i_int;

    var_Get( p_demux, "mod-surround", &val );
    if( val.b_bool) settings.mFlags |= MODPLUG_ENABLE_SURROUND;
    var_Get( p_demux, "mod-surround-level", &val );
    settings.mSurroundDepth = val.i_int;
    var_Get( p_demux, "mod-surround-delay", &val );
    settings.mSurroundDelay = val.i_int;

    ModPlug_SetSettings( &settings );

    if( ( p_sys->f = ModPlug_Load( p_sys->p_data, p_sys->i_data ) ) == NULL )
    {
        msg_Err( p_demux, "failed to understand the file" );
        /* we try to seek to recover for other plugin */
        stream_Seek( p_demux->s, 0 );
        free( p_sys->p_data );
        free( p_sys );
        return VLC_EGENERIC;
    }

    /* init time */
    p_sys->i_time  = 1;
    p_sys->i_length = ModPlug_GetLength( p_sys->f ) * (int64_t)1000;

    msg_Dbg( p_demux, "MOD loaded name=%s lenght="I64Fd"ms",
             ModPlug_GetName( p_sys->f ),
             p_sys->i_length );

#ifdef WORDS_BIGENDIAN
    es_format_Init( &p_sys->fmt, AUDIO_ES, VLC_FOURCC( 't', 'w', 'o', 's' ) );
#else
    es_format_Init( &p_sys->fmt, AUDIO_ES, VLC_FOURCC( 'a', 'r', 'a', 'w' ) );
#endif
    p_sys->fmt.audio.i_rate = settings.mFrequency;
    p_sys->fmt.audio.i_channels = settings.mChannels;
    p_sys->fmt.audio.i_bitspersample = settings.mBits;
    p_sys->es = es_out_Add( p_demux->out, &p_sys->fmt );

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    demux_t     *p_demux = (demux_t*)p_this;
    demux_sys_t *p_sys = p_demux->p_sys;

    ModPlug_Unload( p_sys->f );
    free( p_sys->p_data );
    free( p_sys );
}


/*****************************************************************************
 * Demux:
 *****************************************************************************/
static int Demux( demux_t *p_demux )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    block_t     *p_frame;
    int         i_bk = ( p_sys->fmt.audio.i_bitspersample / 8 ) *
                       p_sys->fmt.audio.i_channels;

    p_frame = block_New( p_demux, p_sys->fmt.audio.i_rate / 10 * i_bk );

    p_frame->i_buffer = ModPlug_Read( p_sys->f, p_frame->p_buffer, p_frame->i_buffer );
    if( p_frame->i_buffer <= 0 )
    {
        /* EOF */
        block_Release( p_frame );
        return 0;
    }

    /* Set PCR */
    es_out_Control( p_demux->out, ES_OUT_SET_PCR, (int64_t)p_sys->i_time );

    /* We should use p_frame->i_buffer */
    p_sys->i_time += (int64_t)1000000 * p_frame->i_buffer / i_bk / p_sys->fmt.audio.i_rate;

    /* Send data */
    p_frame->i_dts = p_frame->i_pts = p_sys->i_time;
    es_out_Send( p_demux->out, p_sys->es, p_frame );

    return 1;
}

/*****************************************************************************
 * Control:
 *****************************************************************************/
static int Control( demux_t *p_demux, int i_query, va_list args )
{
    demux_sys_t *p_sys = p_demux->p_sys;
    double f, *pf;
    int64_t i64, *pi64;

    switch( i_query )
    {
        case DEMUX_GET_POSITION:
            pf = (double*) va_arg( args, double* );
            if( p_sys->i_length > 0 )
            {
                *pf = (double)p_sys->i_time / (double)p_sys->i_length;
                return VLC_SUCCESS;
            }
            return VLC_EGENERIC;

        case DEMUX_SET_POSITION:
            f = (double) va_arg( args, double );

            i64 = f * p_sys->i_length;
            if( i64 >= 0 && i64 <= p_sys->i_length )
            {
                ModPlug_Seek( p_sys->f, i64 / 1000 );
                p_sys->i_time = i64 + 1;
                es_out_Control( p_demux->out, ES_OUT_RESET_PCR );

                return VLC_SUCCESS;
            }
            return VLC_EGENERIC;

        case DEMUX_GET_TIME:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            *pi64 = p_sys->i_time;
            return VLC_SUCCESS;

        case DEMUX_GET_LENGTH:
            pi64 = (int64_t*)va_arg( args, int64_t * );
            *pi64 = p_sys->i_length;
            return VLC_SUCCESS;

        case DEMUX_SET_TIME:
            i64 = (int64_t)va_arg( args, int64_t );

            if( i64 >= 0 && i64 <= p_sys->i_length )
            {
                ModPlug_Seek( p_sys->f, i64 / 1000 );
                p_sys->i_time = i64 + 1;
                es_out_Control( p_demux->out, ES_OUT_RESET_PCR );

                return VLC_SUCCESS;
            }
            return VLC_EGENERIC;

        case DEMUX_GET_FPS: /* meaningless */
        default:
            return VLC_EGENERIC;
    }
}

