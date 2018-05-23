/*****************************************************************************
 * access.c: DVB card input v4l2 only
 *****************************************************************************
 * Copyright (C) 1998-2004 VideoLAN
 *
 * Authors: Johan Bilien <jobi@via.ecp.fr>
 *          Jean-Paul Saman <jpsaman@wxs.nl>
 *          Christophe Massiot <massiot@via.ecp.fr>
 *          Laurent Aimar <fenrir@via.ecp.fr>
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
#include <vlc/vlc.h>
#include <vlc/input.h>

#ifdef HAVE_UNISTD_H
#   include <unistd.h>
#endif

#include <fcntl.h>
#include <sys/types.h>

#include <errno.h>

#include "dvb.h"

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int  Open( vlc_object_t *p_this );
static void Close( vlc_object_t *p_this );

#define CACHING_TEXT N_("Caching value in ms")
#define CACHING_LONGTEXT N_( \
    "Allows you to modify the default caching value for dvb streams. This " \
    "value should be set in millisecond units." )

#define ADAPTER_TEXT N_("Adapter card to tune")
#define ADAPTER_LONGTEXT N_("Adapter cards have a device file in directory named /dev/dvb/adapter[n] with n>=0.")

#define DEVICE_TEXT N_("Device number to use on adapter")
#define DEVICE_LONGTEXT ""

#define FREQ_TEXT N_("Transponder/multiplex frequency")
#define FREQ_LONGTEXT N_("In kHz for DVB-S or Hz for DVB-C/T")

#define INVERSION_TEXT N_("Inversion mode")
#define INVERSION_LONGTEXT N_("Inversion mode [0=off, 1=on, 2=auto]")

#define PROBE_TEXT N_("Probe DVB card for capabilities")
#define PROBE_LONGTEXT N_("Some DVB cards do not like to be probed for their capabilities.")

#define LNB_LOF1_TEXT N_("Antenna lnb_lof1 (kHz)")
#define LNB_LOF1_LONGTEXT ""

#define LNB_LOF2_TEXT N_("Antenna lnb_lof2 (kHz)")
#define LNB_LOF2_LONGTEXT ""

#define LNB_SLOF_TEXT N_("Antenna lnb_slof (kHz)")
#define LNB_SLOF_LONGTEXT ""

/* Satellite */
#define BUDGET_TEXT N_("Budget mode")
#define BUDGET_LONGTEXT N_("This allows you to stream an entire transponder with a budget card.")

#define SATNO_TEXT N_("Satellite number in the Diseqc system")
#define SATNO_LONGTEXT N_("[0=no diseqc, 1-4=normal diseqc, -1=A, -2=B simple diseqc]")

#define VOLTAGE_TEXT N_("LNB voltage")
#define VOLTAGE_LONGTEXT N_("In Volts [0, 13=vertical, 18=horizontal]")

#define TONE_TEXT N_("22 kHz tone")
#define TONE_LONGTEXT N_("[0=off, 1=on, -1=auto]")

#define FEC_TEXT N_("Transponder FEC")
#define FEC_LONGTEXT N_("FEC=Forward Error Correction mode [9=auto]")

#define SRATE_TEXT N_("Transponder symbol rate in kHz")
#define SRATE_LONGTEXT ""

/* Cable */
#define MODULATION_TEXT N_("Modulation type")
#define MODULATION_LONGTEXT N_("Modulation type for front-end device.")

/* Terrestrial */
#define CODE_RATE_HP_TEXT N_("Terrestrial high priority stream code rate (FEC)")
#define CODE_RATE_HP_LONGTEXT ""

#define CODE_RATE_LP_TEXT N_("Terrestrial low priority stream code rate (FEC)")
#define CODE_RATE_LP_LONGTEXT ""

#define BANDWIDTH_TEXT N_("Terrestrial bandwidth")
#define BANDWIDTH_LONGTEXT N_("Terrestrial bandwidth [0=auto,6,7,8 in MHz]")

#define GUARD_TEXT N_("Terrestrial guard interval")
#define GUARD_LONGTEXT ""

#define TRANSMISSION_TEXT N_("Terrestrial transmission mode")
#define TRANSMISSION_LONGTEXT ""

#define HIERARCHY_TEXT N_("Terrestrial hierarchy mode")
#define HIERARCHY_LONGTEXT ""

vlc_module_begin();
    set_shortname( _("DVB") );
    set_description( N_("DVB input with v4l2 support") );

    add_integer( "dvb-caching", DEFAULT_PTS_DELAY / 1000, NULL, CACHING_TEXT,
                 CACHING_LONGTEXT, VLC_TRUE );
    add_integer( "dvb-adapter", 0, NULL, ADAPTER_TEXT, ADAPTER_LONGTEXT,
                 VLC_FALSE );
    add_integer( "dvb-device", 0, NULL, DEVICE_TEXT, DEVICE_LONGTEXT,
                 VLC_TRUE );
    add_integer( "dvb-frequency", 11954000, NULL, FREQ_TEXT, FREQ_LONGTEXT,
                 VLC_FALSE );
    add_integer( "dvb-inversion", 2, NULL, INVERSION_TEXT, INVERSION_LONGTEXT,
                 VLC_TRUE );
    add_bool( "dvb-probe", 1, NULL, PROBE_TEXT, PROBE_LONGTEXT, VLC_TRUE );
    add_integer( "dvb-lnb-lof1", 9750000, NULL, LNB_LOF1_TEXT,
                 LNB_LOF1_LONGTEXT, VLC_TRUE );
    add_integer( "dvb-lnb-lof2", 10600000, NULL, LNB_LOF2_TEXT,
                 LNB_LOF2_LONGTEXT, VLC_TRUE );
    add_integer( "dvb-lnb-slof", 11700000, NULL, LNB_SLOF_TEXT,
                 LNB_SLOF_LONGTEXT, VLC_TRUE );
    /* DVB-S (satellite) */
    add_bool( "dvb-budget-mode", 0, NULL, BUDGET_TEXT, BUDGET_LONGTEXT,
              VLC_TRUE );
    add_integer( "dvb-satno", 0, NULL, SATNO_TEXT, SATNO_LONGTEXT,
                 VLC_TRUE );
    add_integer( "dvb-voltage", 13, NULL, VOLTAGE_TEXT, VOLTAGE_LONGTEXT,
                 VLC_TRUE );
    add_integer( "dvb-tone", -1, NULL, TONE_TEXT, TONE_LONGTEXT,
                 VLC_TRUE );
    add_integer( "dvb-fec", 9, NULL, FEC_TEXT, FEC_LONGTEXT, VLC_TRUE );
    add_integer( "dvb-srate", 27500000, NULL, SRATE_TEXT, SRATE_LONGTEXT,
                 VLC_FALSE );
    /* DVB-T (terrestrial) */
    add_integer( "dvb-modulation", 0, NULL, MODULATION_TEXT,
                 MODULATION_LONGTEXT, VLC_TRUE );
    /* DVB-T (terrestrial) */
    add_integer( "dvb-code-rate-hp", 9, NULL, CODE_RATE_HP_TEXT,
                 CODE_RATE_HP_LONGTEXT, VLC_TRUE );
    add_integer( "dvb-code-rate-lp", 9, NULL, CODE_RATE_LP_TEXT,
                 CODE_RATE_LP_LONGTEXT, VLC_TRUE );
    add_integer( "dvb-bandwidth", 0, NULL, BANDWIDTH_TEXT, BANDWIDTH_LONGTEXT,
                 VLC_TRUE );
    add_integer( "dvb-guard", 0, NULL, GUARD_TEXT, GUARD_LONGTEXT, VLC_TRUE );
    add_integer( "dvb-transmission", 0, NULL, TRANSMISSION_TEXT,
                 TRANSMISSION_LONGTEXT, VLC_TRUE );
    add_integer( "dvb-hierarchy", 0, NULL, HIERARCHY_TEXT, HIERARCHY_LONGTEXT,
                 VLC_TRUE );

    set_capability( "access2", 0 );
    add_shortcut( "dvb" );
    add_shortcut( "dvb-s" );
    add_shortcut( "qpsk" );
    add_shortcut( "dvb-c" );
    add_shortcut( "cable" );
    add_shortcut( "dvb-t" );
    add_shortcut( "terrestrial" );
    add_shortcut( "satellite" );    /* compatibility with the interface. */
    set_callbacks( Open, Close );
vlc_module_end();


/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static block_t *Block( access_t * );
static int Control( access_t *, int, va_list );

#define DVB_READ_ONCE 3
#define TS_PACKET_SIZE 188

static void FilterUnset( access_t *, int i_max );
static void FilterUnsetPID( access_t *, int i_pid );
static void FilterSet( access_t *, int i_pid, int i_type );

static void VarInit( access_t * );
static int  ParseMRL( access_t * );


/*****************************************************************************
 * Open: open the frontend device
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    access_t     *p_access = (access_t*)p_this;
    access_sys_t *p_sys;

    /* Only if selected */
    if( *p_access->psz_access == '\0' )
        return VLC_EGENERIC;

    /* Set up access */
    p_access->pf_read = NULL;
    p_access->pf_block = Block;
    p_access->pf_control = Control;
    p_access->pf_seek = NULL;
    p_access->info.i_update = 0;
    p_access->info.i_size = 0;
    p_access->info.i_pos = 0;
    p_access->info.b_eof = VLC_FALSE;
    p_access->info.i_title = 0;
    p_access->info.i_seekpoint = 0;

    p_access->p_sys = p_sys = malloc( sizeof( access_sys_t ) );
    memset( p_sys, 0, sizeof( access_sys_t ) );

    /* Create all variables */
    VarInit( p_access );

    /* Parse the command line */
    if( ParseMRL( p_access ) )
    {
        free( p_sys );
        return VLC_EGENERIC;
    }

    /* Getting frontend info */
    if( E_(FrontendOpen)( p_access) )
    {
        free( p_sys );
        return VLC_EGENERIC;
    }

    /* Setting frontend parameters for tuning the hardware */
    msg_Dbg( p_access, "trying to tune the frontend...");
    if( E_(FrontendSet)( p_access ) < 0 )
    {
        E_(FrontendClose)( p_access );
        free( p_sys );
        return VLC_EGENERIC;
    }

    /* Opening DVR device */
    if( E_(DVROpen)( p_access ) < 0 )
    {
        E_(FrontendClose)( p_access );
        free( p_sys );
        return VLC_EGENERIC;
    }

    p_sys->b_budget_mode = var_GetBool( p_access, "dvb-budget-mode" );
    if( p_sys->b_budget_mode )
    {
        msg_Dbg( p_access, "setting filter on all PIDs" );
        FilterSet( p_access, 0x2000, OTHER_TYPE );
    }
    else
    {
        msg_Dbg( p_access, "setting filter on PAT" );
        FilterSet( p_access, 0x0, OTHER_TYPE );
    }

    E_(CAMOpen)( p_access );

    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close : Close the device
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    access_t     *p_access = (access_t*)p_this;
    access_sys_t *p_sys = p_access->p_sys;

    FilterUnset( p_access, p_sys->b_budget_mode ? 1 : MAX_DEMUX );

    E_(DVRClose)( p_access );
    E_(FrontendClose)( p_access );
    E_(CAMClose)( p_access );

    free( p_sys );
}

/*****************************************************************************
 * Block:
 *****************************************************************************/
static block_t *Block( access_t *p_access )
{
    access_sys_t *p_sys = p_access->p_sys;
    block_t *p_block;

    for ( ; ; )
    {
        struct timeval timeout;
        fd_set fds;
        int i_ret;

        /* Initialize file descriptor set */
        FD_ZERO( &fds );
        FD_SET( p_sys->i_handle, &fds );

        /* We'll wait 0.5 second if nothing happens */
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;

        /* Find if some data is available */
        i_ret = select( p_sys->i_handle + 1, &fds, NULL, NULL, &timeout );

        if ( p_access->b_die )
            return NULL;

        if ( i_ret < 0 && errno == EINTR )
            continue;

        if ( i_ret < 0 )
        {
            msg_Err( p_access, "select error (%s)", strerror(errno) );
            return NULL;
        }

        if ( p_sys->i_ca_handle && mdate() > p_sys->i_ca_next_event )
        {
            E_(CAMPoll)( p_access );
            p_sys->i_ca_next_event = mdate() + p_sys->i_ca_timeout;
        }

        if ( FD_ISSET( p_sys->i_handle, &fds ) )
        {
            break;
        }
    }

    p_block = block_New( p_access, DVB_READ_ONCE * TS_PACKET_SIZE );
    if( ( p_block->i_buffer = read( p_sys->i_handle, p_block->p_buffer,
                                    DVB_READ_ONCE * TS_PACKET_SIZE ) ) <= 0 )
    {
        msg_Err( p_access, "read failed (%s)", strerror(errno) );
        block_Release( p_block );
        return NULL;
    }

    return p_block;
}

/*****************************************************************************
 * Control:
 *****************************************************************************/
static int Control( access_t *p_access, int i_query, va_list args )
{
    access_sys_t *p_sys = p_access->p_sys;
    vlc_bool_t   *pb_bool, b_bool;
    int          *pi_int, i_int;
    int64_t      *pi_64;

    switch( i_query )
    {
        /* */
        case ACCESS_CAN_SEEK:
        case ACCESS_CAN_FASTSEEK:
        case ACCESS_CAN_PAUSE:
        case ACCESS_CAN_CONTROL_PACE:
            pb_bool = (vlc_bool_t*)va_arg( args, vlc_bool_t* );
            *pb_bool = VLC_FALSE;
            break;
        /* */
        case ACCESS_GET_MTU:
            pi_int = (int*)va_arg( args, int * );
            *pi_int = DVB_READ_ONCE * TS_PACKET_SIZE;
            break;

        case ACCESS_GET_PTS_DELAY:
            pi_64 = (int64_t*)va_arg( args, int64_t * );
            *pi_64 = var_GetInteger( p_access, "dvb-caching" ) * 1000;
            break;

        /* */
        case ACCESS_SET_PAUSE_STATE:
        case ACCESS_GET_TITLE_INFO:
        case ACCESS_SET_TITLE:
        case ACCESS_SET_SEEKPOINT:
            return VLC_EGENERIC;

        case ACCESS_SET_PRIVATE_ID_STATE:
            i_int  = (int)va_arg( args, int );               /* Private data (pid for now)*/
            b_bool = (vlc_bool_t)va_arg( args, vlc_bool_t ); /* b_selected */
            if( !p_sys->b_budget_mode )
            {
                /* FIXME we may want to give the real type (me ?, I don't ;) */
                if( b_bool )
                    FilterSet( p_access, i_int, OTHER_TYPE );
                else
                    FilterUnsetPID( p_access, i_int );
            }
            break;

        case ACCESS_SET_PRIVATE_ID_CA:
        {
            uint8_t **pp_capmts;
            int i_nb_capmts;

            pp_capmts = (uint8_t **)va_arg( args, uint8_t ** );
            i_nb_capmts = (int)va_arg( args, int );

            E_(CAMSet)( p_access, pp_capmts, i_nb_capmts );
            break;
        }
        default:
            msg_Warn( p_access, "unimplemented query in control" );
            return VLC_EGENERIC;

    }
    return VLC_SUCCESS;
}

/*****************************************************************************
 * FilterSet/FilterUnset:
 *****************************************************************************/
static void FilterSet( access_t *p_access, int i_pid, int i_type )
{
    access_sys_t *p_sys = p_access->p_sys;
    int i;

    /* Find first free slot */
    for( i = 0; i < MAX_DEMUX; i++ )
    {
        if( !p_sys->p_demux_handles[i].i_type )
            break;

        if( p_sys->p_demux_handles[i].i_pid == i_pid )
            return; /* Already set */
    }

    if( i >= MAX_DEMUX )
    {
        msg_Err( p_access, "no free p_demux_handles !" );
        return;
    }

    if( E_(DMXSetFilter)( p_access, i_pid,
                           &p_sys->p_demux_handles[i].i_handle, i_type ) )
    {
        msg_Err( p_access, "DMXSetFilter failed" );
        return;
    }
    p_sys->p_demux_handles[i].i_type = i_type;
    p_sys->p_demux_handles[i].i_pid = i_pid;
}

static void FilterUnset( access_t *p_access, int i_max )
{
    access_sys_t *p_sys = p_access->p_sys;
    int i;

    for( i = 0; i < i_max; i++ )
    {
        if( p_sys->p_demux_handles[i].i_type )
        {
            E_(DMXUnsetFilter)( p_access, p_sys->p_demux_handles[i].i_handle );
            p_sys->p_demux_handles[i].i_type = 0;
        }
    }
}

static void FilterUnsetPID( access_t *p_access, int i_pid )
{
    access_sys_t *p_sys = p_access->p_sys;
    int i;

    for( i = 0; i < MAX_DEMUX; i++ )
    {
        if( p_sys->p_demux_handles[i].i_type &&
            p_sys->p_demux_handles[i].i_pid == i_pid )
        {
            E_(DMXUnsetFilter)( p_access, p_sys->p_demux_handles[i].i_handle );
            p_sys->p_demux_handles[i].i_type = 0;
        }
    }
}

/*****************************************************************************
 * VarInit/ParseMRL:
 *****************************************************************************/
static void VarInit( access_t *p_access )
{
    /* */
    var_Create( p_access, "dvb-caching", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );

    /* */
    var_Create( p_access, "dvb-adapter", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-device", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-frequency", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-inversion", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-probe", VLC_VAR_BOOL | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-lnb-lof1", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-lnb-lof2", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-lnb-slof", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );

    /* */
    var_Create( p_access, "dvb-budget-mode", VLC_VAR_BOOL | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-satno", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-voltage", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-tone", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-fec", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-srate", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );

    /* */
    var_Create( p_access, "dvb-modulation", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );

    /* */
    var_Create( p_access, "dvb-code-rate-hp", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-code-rate-lp", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-bandwidth", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-transmission", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-guard", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
    var_Create( p_access, "dvb-hierarchy", VLC_VAR_INTEGER | VLC_VAR_DOINHERIT );
}

/* */
static int ParseMRL( access_t *p_access )
{
    char *psz_dup = strdup( p_access->psz_path );
    char *psz_parser = psz_dup;
    char *psz_next;
    vlc_value_t         val;

#define GET_OPTION_INT( option )                                            \
    if ( !strncmp( psz_parser, option "=", strlen(option "=") ) )           \
    {                                                                       \
        val.i_int = strtol( psz_parser + strlen(option "="), &psz_parser,   \
                            0 );                                            \
        var_Set( p_access, "dvb-" option, val );                            \
    }

#define GET_OPTION_BOOL( option )                                           \
    if ( !strncmp( psz_parser, option "=", strlen(option "=") ) )           \
    {                                                                       \
        val.b_bool = strtol( psz_parser + strlen(option "="), &psz_parser,  \
                             0 );                                           \
        var_Set( p_access, "dvb-" option, val );                            \
    }

    /* Test for old syntax */
    strtol( psz_parser, &psz_next, 10 );
    if( psz_next != psz_parser )
    {
        msg_Err( p_access, "the DVB input old syntax is deprecated, use vlc "
                          "-p dvb to see an explanation of the new syntax" );
        free( psz_dup );
        return VLC_EGENERIC;
    }

    while( *psz_parser )
    {
        GET_OPTION_INT("adapter")
        else GET_OPTION_INT("device")
        else GET_OPTION_INT("frequency")
        else GET_OPTION_INT("inversion")
        else GET_OPTION_BOOL("probe")
        else GET_OPTION_INT("lnb-lof1")
        else GET_OPTION_INT("lnb-lof2")
        else GET_OPTION_INT("lnb-slof")

        else GET_OPTION_BOOL("budget-mode")
        else GET_OPTION_INT("voltage")
        else GET_OPTION_INT("tone")
        else GET_OPTION_INT("fec")
        else GET_OPTION_INT("srate")

        else GET_OPTION_INT("modulation")

        else GET_OPTION_INT("code-rate-hp")
        else GET_OPTION_INT("code-rate-lp")
        else GET_OPTION_INT("bandwidth")
        else GET_OPTION_INT("transmission")
        else GET_OPTION_INT("guard")
        else GET_OPTION_INT("hierarchy")

        else if( !strncmp( psz_parser, "satno=",
                           strlen( "satno=" ) ) )
        {
            psz_parser += strlen( "satno=" );
            if ( *psz_parser == 'A' || *psz_parser == 'a' )
                val.i_int = -1;
            else if ( *psz_parser == 'B' || *psz_parser == 'b' )
                val.i_int = -2;
            else
                val.i_int = strtol( psz_parser, &psz_parser, 0 );
            var_Set( p_access, "dvb-satno", val );
        }
        /* Redundant with voltage but much easier to use */
        else if( !strncmp( psz_parser, "polarization=",
                           strlen( "polarization=" ) ) )
        {
            psz_parser += strlen( "polarization=" );
            if ( *psz_parser == 'V' || *psz_parser == 'v' )
                val.i_int = 13;
            else if ( *psz_parser == 'H' || *psz_parser == 'h' )
                val.i_int = 18;
            else
            {
                msg_Err( p_access, "illegal polarization %c", *psz_parser );
                free( psz_dup );
                return VLC_EGENERIC;
            }
            var_Set( p_access, "dvb-voltage", val );
        }
        else
        {
            msg_Err( p_access, "unknown option (%s)", psz_parser );
            free( psz_dup );
            return VLC_EGENERIC;
        }

        if ( *psz_parser )
            psz_parser++;
    }
#undef GET_OPTION_INT
#undef GET_OPTION_BOOL

    free( psz_dup );
    return VLC_SUCCESS;
}

