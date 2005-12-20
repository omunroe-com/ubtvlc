/*****************************************************************************
 * en50221.c : implementation of the transport, session and applications
 * layers of EN 50 221
 *****************************************************************************
 * Copyright (C) 2004 VideoLAN
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 * Based on code from libdvbci Copyright (C) 2000 Klaus Schmidinger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA    02111, USA.
 *****************************************************************************/

#include <vlc/vlc.h>
#include <vlc/input.h>

#include <sys/ioctl.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/poll.h>

/* DVB Card Drivers */
#include <linux/dvb/version.h>
#include <linux/dvb/dmx.h>
#include <linux/dvb/frontend.h>
#include <linux/dvb/ca.h>

#include "dvb.h"

#undef DEBUG_TPDU

static void ResourceManagerOpen( access_t * p_access, int i_session_id );
static void ApplicationInformationOpen( access_t * p_access, int i_session_id );
static void ConditionalAccessOpen( access_t * p_access, int i_session_id );
static void DateTimeOpen( access_t * p_access, int i_session_id );
static void MMIOpen( access_t * p_access, int i_session_id );

/*****************************************************************************
 * Utility functions
 *****************************************************************************/
#define SIZE_INDICATOR 0x80

static uint8_t *GetLength( uint8_t *p_data, int *pi_length )
{
    *pi_length = *p_data++;

    if ( (*pi_length & SIZE_INDICATOR) != 0 )
    {
        int l = *pi_length & ~SIZE_INDICATOR;
        int i;

        *pi_length = 0;
        for ( i = 0; i < l; i++ )
            *pi_length = (*pi_length << 8) | *p_data++;
    }

    return p_data;
}

static uint8_t *SetLength( uint8_t *p_data, int i_length )
{
    uint8_t *p = p_data;

    if ( i_length < 128 )
    {
        *p++ = i_length;
    }
    else if ( i_length < 256 )
    {
        *p++ = SIZE_INDICATOR | 0x1;
        *p++ = i_length;
    }
    else if ( i_length < 65536 )
    {
        *p++ = SIZE_INDICATOR | 0x2;
        *p++ = i_length >> 8;
        *p++ = i_length & 0xff;
    }
    else if ( i_length < 16777216 )
    {
        *p++ = SIZE_INDICATOR | 0x3;
        *p++ = i_length >> 16;
        *p++ = (i_length >> 8) & 0xff;
        *p++ = i_length & 0xff;
    }
    else
    {
        *p++ = SIZE_INDICATOR | 0x4;
        *p++ = i_length >> 24;
        *p++ = (i_length >> 16) & 0xff;
        *p++ = (i_length >> 8) & 0xff;
        *p++ = i_length & 0xff;
    }

    return p;
}


/*
 * Transport layer
 */

#define MAX_TPDU_SIZE  2048
#define MAX_TPDU_DATA  (MAX_TPDU_SIZE - 4)

#define DATA_INDICATOR 0x80

#define T_SB           0x80
#define T_RCV          0x81
#define T_CREATE_TC    0x82
#define T_CTC_REPLY    0x83
#define T_DELETE_TC    0x84
#define T_DTC_REPLY    0x85
#define T_REQUEST_TC   0x86
#define T_NEW_TC       0x87
#define T_TC_ERROR     0x88
#define T_DATA_LAST    0xA0
#define T_DATA_MORE    0xA1

static void Dump( vlc_bool_t b_outgoing, uint8_t *p_data, int i_size )
{
#ifdef DEBUG_TPDU
    int i;
#define MAX_DUMP 256
    fprintf(stderr, "%s ", b_outgoing ? "-->" : "<--");
    for ( i = 0; i < i_size && i < MAX_DUMP; i++)
        fprintf(stderr, "%02X ", p_data[i]);
    fprintf(stderr, "%s\n", i_size >= MAX_DUMP ? "..." : "");
#endif
}

/*****************************************************************************
 * TPDUSend
 *****************************************************************************/
static int TPDUSend( access_t * p_access, uint8_t i_slot, uint8_t i_tag,
                     const uint8_t *p_content, int i_length )
{
    access_sys_t *p_sys = p_access->p_sys;
    uint8_t i_tcid = i_slot + 1;
    uint8_t p_data[MAX_TPDU_SIZE];
    int i_size;

    i_size = 0;
    p_data[0] = i_slot;
    p_data[1] = i_tcid;
    p_data[2] = i_tag;

    switch ( i_tag )
    {
    case T_RCV:
    case T_CREATE_TC:
    case T_CTC_REPLY:
    case T_DELETE_TC:
    case T_DTC_REPLY:
    case T_REQUEST_TC:
        p_data[3] = 1; /* length */
        p_data[4] = i_tcid;
        i_size = 5;
        break;

    case T_NEW_TC:
    case T_TC_ERROR:
        p_data[3] = 2; /* length */
        p_data[4] = i_tcid;
        p_data[5] = p_content[0];
        i_size = 6;
        break;

    case T_DATA_LAST:
    case T_DATA_MORE:
    {
        /* i_length <= MAX_TPDU_DATA */
        uint8_t *p = p_data + 3;
        p = SetLength( p, i_length + 1 );
        *p++ = i_tcid;

        if ( i_length )
            memcpy( p, p_content, i_length );
            i_size = i_length + (p - p_data);
        }
        break;

    default:
        break;
    }
    Dump( VLC_TRUE, p_data, i_size );

    if ( write( p_sys->i_ca_handle, p_data, i_size ) != i_size )
    {
        msg_Err( p_access, "cannot write to CAM device (%s)",
                 strerror(errno) );
        return VLC_EGENERIC;
    }

    return VLC_SUCCESS;
}


/*****************************************************************************
 * TPDURecv
 *****************************************************************************/
#define CAM_READ_TIMEOUT  3500 // ms

static int TPDURecv( access_t * p_access, uint8_t i_slot, uint8_t *pi_tag,
                     uint8_t *p_data, int *pi_size )
{
    access_sys_t *p_sys = p_access->p_sys;
    uint8_t i_tcid = i_slot + 1;
    int i_size;
    struct pollfd pfd[1];

    pfd[0].fd = p_sys->i_ca_handle;
    pfd[0].events = POLLIN;
    if ( !(poll(pfd, 1, CAM_READ_TIMEOUT) > 0 && (pfd[0].revents & POLLIN)) )
    {
        msg_Err( p_access, "cannot poll from CAM device" );
        return VLC_EGENERIC;
    }

    if ( pi_size == NULL )
    {
        p_data = malloc( MAX_TPDU_SIZE );
    }

    for ( ; ; )
    {
        i_size = read( p_sys->i_ca_handle, p_data, MAX_TPDU_SIZE );

        if ( i_size >= 0 || errno != EINTR )
            break;
    }

    if ( i_size < 5 )
    {
        msg_Err( p_access, "cannot read from CAM device (%d:%s)", i_size,
                 strerror(errno) );
        return VLC_EGENERIC;
    }

    if ( p_data[1] != i_tcid )
    {
        msg_Err( p_access, "invalid read from CAM device (%d instead of %d)",
                 p_data[1], i_tcid );
        return VLC_EGENERIC;
    }

    *pi_tag = p_data[2];
    p_sys->pb_tc_has_data[i_slot] = (i_size >= 4
                                      && p_data[i_size - 4] == T_SB
                                      && p_data[i_size - 3] == 2
                                      && (p_data[i_size - 1] & DATA_INDICATOR))
                                        ?  VLC_TRUE : VLC_FALSE;

    Dump( VLC_FALSE, p_data, i_size );

    if ( pi_size == NULL )
        free( p_data );
    else
        *pi_size = i_size;

    return VLC_SUCCESS;
}


/*
 * Session layer
 */

#define ST_SESSION_NUMBER           0x90
#define ST_OPEN_SESSION_REQUEST     0x91
#define ST_OPEN_SESSION_RESPONSE    0x92
#define ST_CREATE_SESSION           0x93
#define ST_CREATE_SESSION_RESPONSE  0x94
#define ST_CLOSE_SESSION_REQUEST    0x95
#define ST_CLOSE_SESSION_RESPONSE   0x96

#define SS_OK             0x00
#define SS_NOT_ALLOCATED  0xF0

#define RI_RESOURCE_MANAGER            0x00010041
#define RI_APPLICATION_INFORMATION     0x00020041
#define RI_CONDITIONAL_ACCESS_SUPPORT  0x00030041
#define RI_HOST_CONTROL                0x00200041
#define RI_DATE_TIME                   0x00240041
#define RI_MMI                         0x00400041

static int ResourceIdToInt( uint8_t *p_data )
{
    return ((int)p_data[0] << 24) | ((int)p_data[1] << 16)
            | ((int)p_data[2] << 8) | p_data[3];
}

/*****************************************************************************
 * SPDUSend
 *****************************************************************************/
static int SPDUSend( access_t * p_access, int i_session_id,
                     uint8_t *p_data, int i_size )
{
    access_sys_t *p_sys = p_access->p_sys;
    uint8_t *p_spdu = malloc( i_size + 4 );
    uint8_t *p = p_spdu;
    uint8_t i_tag;
    uint8_t i_slot = p_sys->p_sessions[i_session_id - 1].i_slot;

    *p++ = ST_SESSION_NUMBER;
    *p++ = 0x02;
    *p++ = (i_session_id >> 8);
    *p++ = i_session_id & 0xff;

    memcpy( p, p_data, i_size );

    i_size += 4;
    p = p_spdu;

    while ( i_size > 0 )
    {
        if ( i_size > MAX_TPDU_DATA )
        {
            if ( TPDUSend( p_access, i_slot, T_DATA_MORE, p,
                           MAX_TPDU_DATA ) != VLC_SUCCESS )
            {
                msg_Err( p_access, "couldn't send TPDU on session %d",
                         i_session_id );
                free( p_spdu );
                return VLC_EGENERIC;
            }
            p += MAX_TPDU_DATA;
            i_size -= MAX_TPDU_DATA;
        }
        else
        {
            if ( TPDUSend( p_access, i_slot, T_DATA_LAST, p, i_size )
                    != VLC_SUCCESS )
            {
                msg_Err( p_access, "couldn't send TPDU on session %d",
                         i_session_id );
                free( p_spdu );
                return VLC_EGENERIC;
            }
            i_size = 0;
        }

        if ( TPDURecv( p_access, i_slot, &i_tag, NULL, NULL ) != VLC_SUCCESS
               || i_tag != T_SB )
        {
            msg_Err( p_access, "couldn't recv TPDU on session %d",
                     i_session_id );
            free( p_spdu );
            return VLC_EGENERIC;
        }
    }

    free( p_spdu );
    return VLC_SUCCESS;
}

/*****************************************************************************
 * SessionOpen
 *****************************************************************************/
static void SessionOpen( access_t * p_access, uint8_t i_slot,
                         uint8_t *p_spdu, int i_size )
{
    access_sys_t *p_sys = p_access->p_sys;
    int i_session_id;
    int i_resource_id = ResourceIdToInt( &p_spdu[2] );
    uint8_t p_response[16];
    int i_status = SS_NOT_ALLOCATED;
    uint8_t i_tag;

    for ( i_session_id = 1; i_session_id <= MAX_SESSIONS; i_session_id++ )
    {
        if ( !p_sys->p_sessions[i_session_id - 1].i_resource_id )
            break;
    }
    if ( i_session_id == MAX_SESSIONS )
    {
        msg_Err( p_access, "too many sessions !" );
        return;
    }
    p_sys->p_sessions[i_session_id - 1].i_slot = i_slot;
    p_sys->p_sessions[i_session_id - 1].i_resource_id = i_resource_id;
    p_sys->p_sessions[i_session_id - 1].pf_close = NULL;
    p_sys->p_sessions[i_session_id - 1].pf_manage = NULL;

    if ( i_resource_id == RI_RESOURCE_MANAGER
          || i_resource_id == RI_APPLICATION_INFORMATION
          || i_resource_id == RI_CONDITIONAL_ACCESS_SUPPORT
          || i_resource_id == RI_DATE_TIME
          || i_resource_id == RI_MMI )
    {
        i_status = SS_OK;
    }

    p_response[0] = ST_OPEN_SESSION_RESPONSE;
    p_response[1] = 0x7;
    p_response[2] = i_status;
    p_response[3] = p_spdu[2];
    p_response[4] = p_spdu[3];
    p_response[5] = p_spdu[4];
    p_response[6] = p_spdu[5];
    p_response[7] = i_session_id >> 8;
    p_response[8] = i_session_id & 0xff;

    if ( TPDUSend( p_access, i_slot, T_DATA_LAST, p_response, 9 ) !=
            VLC_SUCCESS )
    {
        msg_Err( p_access,
                 "SessionOpen: couldn't send TPDU on slot %d", i_slot );
        return;
    }
    if ( TPDURecv( p_access, i_slot, &i_tag, NULL, NULL ) != VLC_SUCCESS )
    {
        msg_Err( p_access,
                 "SessionOpen: couldn't recv TPDU on slot %d", i_slot );
        return;
    }

    switch ( i_resource_id )
    {
    case RI_RESOURCE_MANAGER:
        ResourceManagerOpen( p_access, i_session_id ); break; 
    case RI_APPLICATION_INFORMATION:
        ApplicationInformationOpen( p_access, i_session_id ); break; 
    case RI_CONDITIONAL_ACCESS_SUPPORT:
        ConditionalAccessOpen( p_access, i_session_id ); break; 
    case RI_DATE_TIME:
        DateTimeOpen( p_access, i_session_id ); break; 
    case RI_MMI:
        MMIOpen( p_access, i_session_id ); break; 

    case RI_HOST_CONTROL:
    default:
        msg_Err( p_access, "unknown resource id (0x%x)", i_resource_id );
        p_sys->p_sessions[i_session_id - 1].i_resource_id = 0;
    }
}

/*****************************************************************************
 * SessionClose
 *****************************************************************************/
static void SessionClose( access_t * p_access, int i_session_id )
{
    access_sys_t *p_sys = p_access->p_sys;
    uint8_t p_response[16];
    uint8_t i_tag;
    uint8_t i_slot = p_sys->p_sessions[i_session_id - 1].i_slot;

    if ( p_sys->p_sessions[i_session_id - 1].pf_close != NULL )
        p_sys->p_sessions[i_session_id - 1].pf_close( p_access, i_session_id );
    p_sys->p_sessions[i_session_id - 1].i_resource_id = 0;

    p_response[0] = ST_CLOSE_SESSION_RESPONSE;
    p_response[1] = 0x3;
    p_response[2] = SS_OK;
    p_response[3] = i_session_id >> 8;
    p_response[4] = i_session_id & 0xff;

    if ( TPDUSend( p_access, i_slot, T_DATA_LAST, p_response, 5 ) !=
            VLC_SUCCESS )
    {
        msg_Err( p_access,
                 "SessionOpen: couldn't send TPDU on slot %d", i_slot );
        return;
    }
    if ( TPDURecv( p_access, i_slot, &i_tag, NULL, NULL ) != VLC_SUCCESS )
    {
        msg_Err( p_access,
                 "SessionOpen: couldn't recv TPDU on slot %d", i_slot );
        return;
    }
}

/*****************************************************************************
 * SPDUHandle
 *****************************************************************************/
static void SPDUHandle( access_t * p_access, uint8_t i_slot,
                        uint8_t *p_spdu, int i_size )
{
    access_sys_t *p_sys = p_access->p_sys;
    int i_session_id;

    switch ( p_spdu[0] )
    {
    case ST_SESSION_NUMBER:
        if ( i_size <= 4 )
            return;
        i_session_id = ((int)p_spdu[2] << 8) | p_spdu[3];
        p_sys->p_sessions[i_session_id - 1].pf_handle( p_access, i_session_id,
                                                       p_spdu + 4, i_size - 4 );
        break;

    case ST_OPEN_SESSION_REQUEST:
        if ( i_size != 6 || p_spdu[1] != 0x4 )
            return;
        SessionOpen( p_access, i_slot, p_spdu, i_size );
        break;

    case ST_CLOSE_SESSION_REQUEST:
        i_session_id = ((int)p_spdu[2] << 8) | p_spdu[3];
        SessionClose( p_access, i_session_id );
        break;

    default:
        break;
    }
}


/*
 * Application layer
 */

#define AOT_NONE                    0x000000
#define AOT_PROFILE_ENQ             0x9F8010
#define AOT_PROFILE                 0x9F8011
#define AOT_PROFILE_CHANGE          0x9F8012
#define AOT_APPLICATION_INFO_ENQ    0x9F8020
#define AOT_APPLICATION_INFO        0x9F8021
#define AOT_ENTER_MENU              0x9F8022
#define AOT_CA_INFO_ENQ             0x9F8030
#define AOT_CA_INFO                 0x9F8031
#define AOT_CA_PMT                  0x9F8032
#define AOT_CA_PMT_REPLY            0x9F8033
#define AOT_TUNE                    0x9F8400
#define AOT_REPLACE                 0x9F8401
#define AOT_CLEAR_REPLACE           0x9F8402
#define AOT_ASK_RELEASE             0x9F8403
#define AOT_DATE_TIME_ENQ           0x9F8440
#define AOT_DATE_TIME               0x9F8441
#define AOT_CLOSE_MMI               0x9F8800
#define AOT_DISPLAY_CONTROL         0x9F8801
#define AOT_DISPLAY_REPLY           0x9F8802
#define AOT_TEXT_LAST               0x9F8803
#define AOT_TEXT_MORE               0x9F8804
#define AOT_KEYPAD_CONTROL          0x9F8805
#define AOT_KEYPRESS                0x9F8806
#define AOT_ENQ                     0x9F8807
#define AOT_ANSW                    0x9F8808
#define AOT_MENU_LAST               0x9F8809
#define AOT_MENU_MORE               0x9F880A
#define AOT_MENU_ANSW               0x9F880B
#define AOT_LIST_LAST               0x9F880C
#define AOT_LIST_MORE               0x9F880D
#define AOT_SUBTITLE_SEGMENT_LAST   0x9F880E
#define AOT_SUBTITLE_SEGMENT_MORE   0x9F880F
#define AOT_DISPLAY_MESSAGE         0x9F8810
#define AOT_SCENE_END_MARK          0x9F8811
#define AOT_SCENE_DONE              0x9F8812
#define AOT_SCENE_CONTROL           0x9F8813
#define AOT_SUBTITLE_DOWNLOAD_LAST  0x9F8814
#define AOT_SUBTITLE_DOWNLOAD_MORE  0x9F8815
#define AOT_FLUSH_DOWNLOAD          0x9F8816
#define AOT_DOWNLOAD_REPLY          0x9F8817
#define AOT_COMMS_CMD               0x9F8C00
#define AOT_CONNECTION_DESCRIPTOR   0x9F8C01
#define AOT_COMMS_REPLY             0x9F8C02
#define AOT_COMMS_SEND_LAST         0x9F8C03
#define AOT_COMMS_SEND_MORE         0x9F8C04
#define AOT_COMMS_RCV_LAST          0x9F8C05
#define AOT_COMMS_RCV_MORE          0x9F8C06

/*****************************************************************************
 * APDUGetTag
 *****************************************************************************/
static int APDUGetTag( const uint8_t *p_apdu, int i_size )
{
    if ( i_size >= 3 )
    {
        int i, t = 0;
        for ( i = 0; i < 3; i++ )
            t = (t << 8) | *p_apdu++;
        return t;
    }

    return AOT_NONE;
}

/*****************************************************************************
 * APDUGetLength
 *****************************************************************************/
static uint8_t *APDUGetLength( uint8_t *p_apdu, int *pi_size )
{
    return GetLength( &p_apdu[3], pi_size );
}

/*****************************************************************************
 * APDUSend
 *****************************************************************************/
static int APDUSend( access_t * p_access, int i_session_id, int i_tag,
                     uint8_t *p_data, int i_size )
{
    uint8_t *p_apdu = malloc( i_size + 12 );
    uint8_t *p = p_apdu;
    int i_ret;

    *p++ = (i_tag >> 16);
    *p++ = (i_tag >> 8) & 0xff;
    *p++ = i_tag & 0xff;
    p = SetLength( p, i_size );
    if ( i_size )
        memcpy( p, p_data, i_size );

    i_ret = SPDUSend( p_access, i_session_id, p_apdu, i_size + p - p_apdu );
    free( p_apdu );
    return i_ret;
}

/*****************************************************************************
 * ResourceManagerHandle
 *****************************************************************************/
static void ResourceManagerHandle( access_t * p_access, int i_session_id,
                                   uint8_t *p_apdu, int i_size )
{
    int i_tag = APDUGetTag( p_apdu, i_size );

    switch ( i_tag )
    {
    case AOT_PROFILE_ENQ:
    {
        int resources[] = { htonl(RI_RESOURCE_MANAGER),
                            htonl(RI_APPLICATION_INFORMATION),
                            htonl(RI_CONDITIONAL_ACCESS_SUPPORT),
                            htonl(RI_DATE_TIME),
                            htonl(RI_MMI)
                          };
        APDUSend( p_access, i_session_id, AOT_PROFILE, (uint8_t*)resources,
                  sizeof(resources) );
        break;
    }
    case AOT_PROFILE:
        APDUSend( p_access, i_session_id, AOT_PROFILE_CHANGE, NULL, 0 );
        break;

    default:
        msg_Err( p_access, "unexpected tag in ResourceManagerHandle (0x%x)",
                 i_tag );
    }
}

/*****************************************************************************
 * ResourceManagerOpen
 *****************************************************************************/
static void ResourceManagerOpen( access_t * p_access, int i_session_id )
{
    access_sys_t *p_sys = p_access->p_sys;

    msg_Dbg( p_access, "opening ResourceManager session (%d)", i_session_id );

    p_sys->p_sessions[i_session_id - 1].pf_handle = ResourceManagerHandle;

    APDUSend( p_access, i_session_id, AOT_PROFILE_ENQ, NULL, 0 );
}

/*****************************************************************************
 * ApplicationInformationHandle
 *****************************************************************************/
static void ApplicationInformationHandle( access_t * p_access, int i_session_id,
                                          uint8_t *p_apdu, int i_size )
{
    int i_tag = APDUGetTag( p_apdu, i_size );

    switch ( i_tag )
    {
    case AOT_APPLICATION_INFO:
    {
        int i_type, i_manufacturer, i_code;
        int l = 0;
        uint8_t *d = APDUGetLength( p_apdu, &l );

        if ( l < 4 ) break;
        p_apdu[l + 3] = '\0';

        i_type = *d++;
        i_manufacturer = ((int)d[0] << 8) | d[1];
        d += 2;
        i_code = ((int)d[0] << 8) | d[1];
        d += 2;
        d = GetLength( d, &l );
        d[l] = '\0';
        msg_Info( p_access, "CAM: %s, %02X, %04X, %04X",
                  d, i_type, i_manufacturer, i_code );
        break;
    }
    default:
        msg_Err( p_access,
                 "unexpected tag in ApplicationInformationHandle (0x%x)",
                 i_tag );
    }
}

/*****************************************************************************
 * ApplicationInformationOpen
 *****************************************************************************/
static void ApplicationInformationOpen( access_t * p_access, int i_session_id )
{
    access_sys_t *p_sys = p_access->p_sys;

    msg_Dbg( p_access, "opening ApplicationInformation session (%d)", i_session_id );

    p_sys->p_sessions[i_session_id - 1].pf_handle = ApplicationInformationHandle;

    APDUSend( p_access, i_session_id, AOT_APPLICATION_INFO_ENQ, NULL, 0 );
}

/*****************************************************************************
 * ConditionalAccessHandle
 *****************************************************************************/
static void ConditionalAccessHandle( access_t * p_access, int i_session_id,
                                     uint8_t *p_apdu, int i_size )
{
    access_sys_t *p_sys = p_access->p_sys;
    int i_tag = APDUGetTag( p_apdu, i_size );

    switch ( i_tag )
    {
    case AOT_CA_INFO:
    {
        if ( p_sys->i_nb_capmts )
        {
            int i;
            msg_Dbg( p_access, "sending CAPMT on session %d", i_session_id );
            for ( i = 0; i < p_sys->i_nb_capmts; i++ )
            {
                int i_size;
                uint8_t *p;
                p = GetLength( &p_sys->pp_capmts[i][3], &i_size );
                SPDUSend( p_access, i_session_id, p_sys->pp_capmts[i],
                          i_size + (p - p_sys->pp_capmts[i]) );
            }

            p_sys->i_ca_timeout = 100000;
        }
        break;
    }
    default:
        msg_Err( p_access,
                 "unexpected tag in ConditionalAccessHandle (0x%x)",
                 i_tag );
    }
}

/*****************************************************************************
 * ConditionalAccessOpen
 *****************************************************************************/
static void ConditionalAccessOpen( access_t * p_access, int i_session_id )
{
    access_sys_t *p_sys = p_access->p_sys;

    msg_Dbg( p_access, "opening ConditionalAccess session (%d)", i_session_id );

    p_sys->p_sessions[i_session_id - 1].pf_handle = ConditionalAccessHandle;

    APDUSend( p_access, i_session_id, AOT_CA_INFO_ENQ, NULL, 0 );
}

typedef struct
{
    int i_interval;
    mtime_t i_last;
} date_time_t;

/*****************************************************************************
 * DateTimeSend
 *****************************************************************************/
static void DateTimeSend( access_t * p_access, int i_session_id )
{
    access_sys_t *p_sys = p_access->p_sys;
    date_time_t *p_date =
        (date_time_t *)p_sys->p_sessions[i_session_id - 1].p_sys;

    time_t t = time(NULL);
    struct tm tm_gmt;
    struct tm tm_loc;

    if ( gmtime_r(&t, &tm_gmt) && localtime_r(&t, &tm_loc) )
    {
        int Y = tm_gmt.tm_year;
        int M = tm_gmt.tm_mon + 1;
        int D = tm_gmt.tm_mday;
        int L = (M == 1 || M == 2) ? 1 : 0;
        int MJD = 14956 + D + (int)((Y - L) * 365.25)
                    + (int)((M + 1 + L * 12) * 30.6001);
        uint8_t p_response[7];

#define DEC2BCD(d) (((d / 10) << 4) + (d % 10))

        p_response[0] = htons(MJD) >> 8;
        p_response[1] = htons(MJD) & 0xff;
        p_response[2] = DEC2BCD(tm_gmt.tm_hour);
        p_response[3] = DEC2BCD(tm_gmt.tm_min);
        p_response[4] = DEC2BCD(tm_gmt.tm_sec);
        p_response[5] = htons(tm_loc.tm_gmtoff / 60) >> 8;
        p_response[6] = htons(tm_loc.tm_gmtoff / 60) & 0xff;

        APDUSend( p_access, i_session_id, AOT_DATE_TIME, p_response, 7 );

        p_date->i_last = mdate();
    }
}

/*****************************************************************************
 * DateTimeHandle
 *****************************************************************************/
static void DateTimeHandle( access_t * p_access, int i_session_id,
                            uint8_t *p_apdu, int i_size )
{
    access_sys_t *p_sys = p_access->p_sys;
    date_time_t *p_date =
        (date_time_t *)p_sys->p_sessions[i_session_id - 1].p_sys;

    int i_tag = APDUGetTag( p_apdu, i_size );

    switch ( i_tag )
    {
    case AOT_DATE_TIME_ENQ:
    {
        int l;
        const uint8_t *d = APDUGetLength( p_apdu, &l );

        if ( l > 0 )
        {
            p_date->i_interval = *d;
            msg_Dbg( p_access, "DateTimeHandle : interval set to %d",
                     p_date->i_interval );
        }
        else
            p_date->i_interval = 0;

        DateTimeSend( p_access, i_session_id );
        break;
    }
    default:
        msg_Err( p_access, "unexpected tag in DateTimeHandle (0x%x)", i_tag );
    }
}

/*****************************************************************************
 * DateTimeManage
 *****************************************************************************/
static void DateTimeManage( access_t * p_access, int i_session_id )
{
    access_sys_t *p_sys = p_access->p_sys;
    date_time_t *p_date =
        (date_time_t *)p_sys->p_sessions[i_session_id - 1].p_sys;

    if ( p_date->i_interval
          && mdate() > p_date->i_last + (mtime_t)p_date->i_interval * 1000000 )
    {
        DateTimeSend( p_access, i_session_id );
    }
}

/*****************************************************************************
 * DateTimeOpen
 *****************************************************************************/
static void DateTimeOpen( access_t * p_access, int i_session_id )
{
    access_sys_t *p_sys = p_access->p_sys;

    msg_Dbg( p_access, "opening DateTime session (%d)", i_session_id );

    p_sys->p_sessions[i_session_id - 1].pf_handle = DateTimeHandle;
    p_sys->p_sessions[i_session_id - 1].pf_manage = DateTimeManage;
    p_sys->p_sessions[i_session_id - 1].p_sys = malloc(sizeof(date_time_t));
    memset( p_sys->p_sessions[i_session_id - 1].p_sys, 0, sizeof(date_time_t) );

    DateTimeSend( p_access, i_session_id );
}

/*****************************************************************************
 * MMIHandle
 *****************************************************************************/
static void MMIHandle( access_t * p_access, int i_session_id,
                            uint8_t *p_apdu, int i_size )
{
    int i_tag = APDUGetTag( p_apdu, i_size );

    switch ( i_tag )
    {
    default:
        msg_Err( p_access, "unexpected tag in MMIHandle (0x%x)", i_tag );
    }
}

/*****************************************************************************
 * MMIOpen
 *****************************************************************************/
static void MMIOpen( access_t * p_access, int i_session_id )
{
    access_sys_t *p_sys = p_access->p_sys;

    msg_Dbg( p_access, "opening MMI session (%d)", i_session_id );

    p_sys->p_sessions[i_session_id - 1].pf_handle = MMIHandle;
}


/*
 * Hardware handling
 */

/*****************************************************************************
 * InitSlot: Open the transport layer
 *****************************************************************************/
#define MAX_TC_RETRIES 20

static int InitSlot( access_t * p_access, int i_slot )
{
    access_sys_t *p_sys = p_access->p_sys;
    int i;

    if ( TPDUSend( p_access, i_slot, T_CREATE_TC, NULL, 0 )
            != VLC_SUCCESS )
    {
        msg_Err( p_access, "en50221_Init: couldn't send TPDU on slot %d",
                 i_slot );
        return VLC_EGENERIC;
    }

    /* This is out of the spec */
    for ( i = 0; i < MAX_TC_RETRIES; i++ )
    {
        uint8_t i_tag;
        if ( TPDURecv( p_access, i_slot, &i_tag, NULL, NULL ) == VLC_SUCCESS
              && i_tag == T_CTC_REPLY )
        {
            p_sys->pb_active_slot[i_slot] = VLC_TRUE;
            break;
        }

        if ( TPDUSend( p_access, i_slot, T_CREATE_TC, NULL, 0 )
                != VLC_SUCCESS )
        {
            msg_Err( p_access,
                     "en50221_Init: couldn't send TPDU on slot %d",
                     i_slot );
            continue;
        }
    }
    if ( p_sys->pb_active_slot[i_slot] )
    {
        p_sys->i_ca_timeout = 1000;
        return VLC_SUCCESS;
    }

    return VLC_EGENERIC;
}


/*
 * External entry points
 */

/*****************************************************************************
 * en50221_Poll : Poll the CAM for TPDUs
 *****************************************************************************/
int E_(en50221_Poll)( access_t * p_access )
{
    access_sys_t *p_sys = p_access->p_sys;
    int i_slot;
    int i_session_id;

    for ( i_slot = 0; i_slot < p_sys->i_nb_slots; i_slot++ )
    {
        uint8_t i_tag;

        if ( !p_sys->pb_active_slot[i_slot] )
        {
            ca_slot_info_t sinfo;
            sinfo.num = i_slot;
            if ( ioctl( p_sys->i_ca_handle, CA_GET_SLOT_INFO, &sinfo ) != 0 )
            {
                msg_Err( p_access, "en50221_Poll: couldn't get info on slot %d",
                         i_slot );
                continue;
            }

            if ( sinfo.flags & CA_CI_MODULE_READY )
            {
                msg_Dbg( p_access, "en50221_Poll: slot %d is active",
                         i_slot );
                p_sys->pb_active_slot[i_slot] = VLC_TRUE;
            }
            else
                continue;

            InitSlot( p_access, i_slot );
        }

        if ( !p_sys->pb_tc_has_data[i_slot] )
        {
            if ( TPDUSend( p_access, i_slot, T_DATA_LAST, NULL, 0 ) !=
                    VLC_SUCCESS )
            {
                msg_Err( p_access,
                         "en50221_Poll: couldn't send TPDU on slot %d",
                         i_slot );
                continue;
            }
            if ( TPDURecv( p_access, i_slot, &i_tag, NULL, NULL ) !=
                    VLC_SUCCESS )
            {
                msg_Err( p_access,
                         "en50221_Poll: couldn't recv TPDU on slot %d",
                         i_slot );
                continue;
            }
        }

        while ( p_sys->pb_tc_has_data[i_slot] )
        {
            uint8_t p_tpdu[MAX_TPDU_SIZE];
            int i_size, i_session_size;
            uint8_t *p_session;

            if ( TPDUSend( p_access, i_slot, T_RCV, NULL, 0 ) != VLC_SUCCESS )
            {
                msg_Err( p_access,
                         "en50221_Poll: couldn't send TPDU on slot %d",
                         i_slot );
                continue;
            }
            if ( TPDURecv( p_access, i_slot, &i_tag, p_tpdu, &i_size ) !=
                    VLC_SUCCESS )
            {
                msg_Err( p_access,
                         "en50221_Poll: couldn't recv TPDU on slot %d",
                         i_slot );
                continue;
            }

            p_session = GetLength( &p_tpdu[3], &i_session_size );
            if ( i_session_size <= 1 )
                continue;

            p_session++;
            i_session_size--;

            if ( i_tag != T_DATA_LAST )
            {
                msg_Err( p_access,
                         "en50221_Poll: fragmented TPDU not supported" );
                break;
            }

            SPDUHandle( p_access, i_slot, p_session, i_session_size );
        }
    }

    for ( i_session_id = 1; i_session_id <= MAX_SESSIONS; i_session_id++ )
    {
        if ( p_sys->p_sessions[i_session_id - 1].i_resource_id
              && p_sys->p_sessions[i_session_id - 1].pf_manage )
        {
            p_sys->p_sessions[i_session_id - 1].pf_manage( p_access,
                                                           i_session_id );
        }
    }

    return VLC_SUCCESS;
}


/*****************************************************************************
 * en50221_SetCAPMT :
 *****************************************************************************/
int E_(en50221_SetCAPMT)( access_t * p_access, uint8_t **pp_capmts,
                          int i_nb_capmts )
{
    access_sys_t *p_sys = p_access->p_sys;
    int i_session_id;

    for ( i_session_id = 1; i_session_id <= MAX_SESSIONS; i_session_id++ )
    {
        int i;

        if ( p_sys->p_sessions[i_session_id - 1].i_resource_id
              != RI_CONDITIONAL_ACCESS_SUPPORT )
            continue;

        msg_Dbg( p_access, "sending CAPMT on session %d", i_session_id );
        for ( i = 0; i < i_nb_capmts; i++ )
        {
            int i_size;
            uint8_t *p;
            p = GetLength( &pp_capmts[i][3], &i_size );
            SPDUSend( p_access, i_session_id, pp_capmts[i],
                      i_size + (p - pp_capmts[i]) );
        }

        p_sys->i_ca_timeout = 100000;
    }

    if ( p_sys->i_nb_capmts )
    {
        int i;
        for ( i = 0; i < p_sys->i_nb_capmts; i++ )
        {
            free( p_sys->pp_capmts[i] );
        }
        free( p_sys->pp_capmts );
    }
    p_sys->pp_capmts = pp_capmts;
    p_sys->i_nb_capmts = i_nb_capmts;

    return VLC_SUCCESS;
}

/*****************************************************************************
 * en50221_End :
 *****************************************************************************/
void E_(en50221_End)( access_t * p_access )
{
    access_sys_t *p_sys = p_access->p_sys;

    if ( p_sys->i_nb_capmts )
    {
        int i;
        for ( i = 0; i < p_sys->i_nb_capmts; i++ )
        {
            free( p_sys->pp_capmts[i] );
        }
        free( p_sys->pp_capmts );
    }

    /* TODO */
}
