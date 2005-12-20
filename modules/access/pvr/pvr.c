/*****************************************************************************
 * pvr.c
 *****************************************************************************
 * Copyright (C) 2001, 2002 VideoLAN
 * $Id: pvr.c 7690 2004-05-16 19:17:56Z gbazin $
 *
 * Authors: Eric Petit <titer@videolan.org>
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

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include "videodev2.h"

/* ivtv specific ioctls */
#define IVTV_IOC_G_CODEC    0xFFEE7703
#define IVTV_IOC_S_CODEC    0xFFEE7704


/* for use with IVTV_IOC_G_CODEC and IVTV_IOC_S_CODEC */

struct ivtv_ioctl_codec {
        uint32_t aspect;
        uint32_t audio_bitmask;
        uint32_t bframes;
        uint32_t bitrate_mode;
        uint32_t bitrate;
        uint32_t bitrate_peak;
        uint32_t dnr_mode;
        uint32_t dnr_spatial;
        uint32_t dnr_temporal;
        uint32_t dnr_type;
        uint32_t framerate;
        uint32_t framespergop;
        uint32_t gop_closure;
        uint32_t pulldown;
        uint32_t stream_type;
};

/*****************************************************************************
 * Prototypes
 *****************************************************************************/
static int     Open   ( vlc_object_t * );
static void    Close  ( vlc_object_t * );

static ssize_t Read   ( input_thread_t *, byte_t *, size_t );

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
vlc_module_begin();
    set_shortname( _("PVR") );
    set_description( _("MPEG Encoding cards input (with ivtv drivers)") );
    set_capability( "access", 0 );
    add_shortcut( "pvr" );
    set_callbacks( Open, Close );
vlc_module_end();

/*****************************************************************************
 * Private access data
 *****************************************************************************/
struct access_sys_t
{
    /* file descriptor */
    int i_fd;
    
    /* options */
    int i_standard;
    int i_width;
    int i_height;
    int i_frequency;
    int i_framerate;
    int i_bitrate;
    int i_bitrate_peak;
    int i_bitrate_mode;
    int i_audio_bitmask;
    int i_input;
};

/*****************************************************************************
 * Open: open the device
 *****************************************************************************/
static int Open( vlc_object_t * p_this )
{
    input_thread_t * p_input = (input_thread_t*) p_this;
    access_sys_t * p_sys;
    char * psz_tofree, * psz_parser, * psz_device;
    char psz_tmp[5];

    struct v4l2_format vfmt;
    struct v4l2_frequency vf;
    struct ivtv_ioctl_codec codec;

    //psz_device = calloc( strlen( "/dev/videox" ) + 1, 1 );

    p_input->pf_read = Read;
    p_input->stream.b_pace_control = 0;
    p_input->stream.b_seekable = 0;
    p_input->i_pts_delay = 1000000;

    /* create private access data */
    p_sys = calloc( sizeof( access_sys_t ), 1 );
    p_input->p_access_data = p_sys;

    /* defaults values */

    psz_device = 0;
    p_sys->i_standard = V4L2_STD_UNKNOWN;
    p_sys->i_width = -1;
    p_sys->i_height = -1;
    p_sys->i_frequency = -1;
    p_sys->i_framerate = -1;
    p_sys->i_bitrate = -1;
    p_sys->i_bitrate_peak = -1;
    p_sys->i_bitrate_mode = -1;
    p_sys->i_audio_bitmask = -1;
    p_sys->i_input = -1;

    /* parse command line options */
    psz_tofree = strdup( p_input->psz_name );
    psz_parser = psz_tofree;

    if( *psz_parser )
    {
        for( ;; )
        {
            if ( !strncmp( psz_parser, "norm=", strlen( "norm=" ) ) )
            {
                char *psz_parser_init;
                psz_parser += strlen( "norm=" );
                psz_parser_init = psz_parser;
                while ( *psz_parser != ':' && *psz_parser != ','
                                                    && *psz_parser != '\0' )
                {
                    psz_parser++;
                }

                if ( !strncmp( psz_parser_init, "secam" ,
                               psz_parser - psz_parser_init ) )
                {
                    p_sys->i_standard = V4L2_STD_SECAM;
                }
                else if ( !strncmp( psz_parser_init, "pal" ,
                                    psz_parser - psz_parser_init ) )
                {
                    p_sys->i_standard = V4L2_STD_PAL;
                }
                else if ( !strncmp( psz_parser_init, "ntsc" ,
                                    psz_parser - psz_parser_init ) )
                {
                    p_sys->i_standard = V4L2_STD_NTSC;
                }
                else
                {
                    p_sys->i_standard = strtol( psz_parser_init ,
                                                &psz_parser, 0 );
                }
            }
            else if( !strncmp( psz_parser, "channel=",
                               strlen( "channel=" ) ) )
            {
                p_sys->i_input =
                  strtol( psz_parser + strlen( "channel=" ),
                            &psz_parser, 0 );
            }
            else if( !strncmp( psz_parser, "device=", strlen( "device=" ) ) )
            {
                psz_device = calloc( strlen( "/dev/videox" ) + 1, 1 );
                sprintf( psz_device, "/dev/video%ld",
                            strtol( psz_parser + strlen( "device=" ),
                            &psz_parser, 0 ) );
            }
            else if( !strncmp( psz_parser, "frequency=",
                               strlen( "frequency=" ) ) )
            {
                p_sys->i_frequency =
                  strtol( psz_parser + strlen( "frequency=" ),
                            &psz_parser, 0 );
            }
            else if( !strncmp( psz_parser, "framerate=",
                               strlen( "framerate=" ) ) )
            {
                p_sys->i_framerate =
                    strtol( psz_parser + strlen( "framerate=" ),
                            &psz_parser, 0 );
            }
            else if( !strncmp( psz_parser, "width=",
                               strlen( "width=" ) ) )
            {
                p_sys->i_width =
                    strtol( psz_parser + strlen( "width=" ),
                            &psz_parser, 0 );
            }
            else if( !strncmp( psz_parser, "height=",
                               strlen( "height=" ) ) )
            {
                p_sys->i_height =
                    strtol( psz_parser + strlen( "height=" ),
                            &psz_parser, 0 );
            }
            else if( !strncmp( psz_parser, "audio=",
                               strlen( "audio=" ) ) )
            {
                p_sys->i_audio_bitmask =
                    strtol( psz_parser + strlen( "audio=" ),
                            &psz_parser, 0 );
            }
            else if( !strncmp( psz_parser, "bitrate=",
                               strlen( "bitrate=" ) ) )
            {
                p_sys->i_bitrate =
                    strtol( psz_parser + strlen( "bitrate=" ),
                            &psz_parser, 0 );
            }
            else if( !strncmp( psz_parser, "maxbitrate=",
                               strlen( "maxbitrate=" ) ) )
            {
                p_sys->i_bitrate_peak =
                    strtol( psz_parser + strlen( "maxbitrate=" ),
                            &psz_parser, 0 );
            }
            else if( !strncmp( psz_parser, "bitratemode=",
                               strlen( "bitratemode=" ) ) )
            {
                char *psz_parser_init;
                psz_parser += strlen( "bitratemode=" );
                psz_parser_init = psz_parser;
                while ( *psz_parser != ':' && *psz_parser != ','
                         && *psz_parser != '\0' )
                {
                    psz_parser++;
                }

                if ( !strncmp( psz_parser_init, "vbr" ,
                               psz_parser - psz_parser_init ) )
                {
                     p_sys->i_bitrate_mode = 0;
                }
                else if ( !strncmp( psz_parser_init, "cbr" ,
                                    psz_parser - psz_parser_init ) )
                {
                    p_sys->i_bitrate_mode = 1;
                }
            }
            else if( !strncmp( psz_parser, "size=",
                               strlen( "size=" ) ) )
            {
                p_sys->i_width =
                    strtol( psz_parser + strlen( "size=" ),
                            &psz_parser, 0 );
                p_sys->i_height =
                    strtol( psz_parser + 1 ,
                            &psz_parser, 0 );
            }
            else
            {
                char *psz_parser_init;
                psz_parser_init = psz_parser;
                while ( *psz_parser != ':' && *psz_parser != ',' && *psz_parser != '\0' )
                {
                    psz_parser++;
                }
                psz_device = calloc( psz_parser - psz_parser_init + 1, 1 );
                strncpy( psz_device, psz_parser_init,
                         psz_parser - psz_parser_init );
            }
            if( *psz_parser )
                psz_parser++;
            else
                break;
        }
    }

    //give a default value to psz_device if none has bee specified

    if (!psz_device)
    {
        psz_device = calloc( strlen( "/dev/videox" ) + 1, 1 );
        strcpy( psz_device, "/dev/video0" );
    }

    free( psz_tofree );

    /* open the device */
    if( ( p_sys->i_fd = open( psz_device, O_RDWR ) ) < 0 )
    {
        msg_Err( p_input, "cannot open device (%s)", strerror( errno ) );
        return VLC_EGENERIC;
    }
    else
    {
        msg_Dbg( p_input, "using video device: %s",psz_device);
    }

    free( psz_device );

    /* set the input */
    if ( p_sys->i_input != -1 )
    {
        if ( ioctl( p_sys->i_fd, VIDIOC_S_INPUT, &p_sys->i_input ) < 0 )
        {
            msg_Warn( p_input, "VIDIOC_S_INPUT failed" );
        }
        else
        {
            msg_Dbg( p_input, "input set to:%d", p_sys->i_input);
        }
    }

    /* set the video standard */
    if ( p_sys->i_standard != V4L2_STD_UNKNOWN )
    {
        if ( ioctl( p_sys->i_fd, VIDIOC_S_STD, &p_sys->i_standard ) < 0 )
        {
            msg_Warn( p_input, "VIDIOC_S_STD failed" );
        }
        else
        {
            msg_Dbg( p_input, "video standard set to:%x", p_sys->i_standard);
        }
    }

    /* set the picture size */
    if ( p_sys->i_width != -1 || p_sys->i_height != -1 )
    {
        if ( ioctl( p_sys->i_fd, VIDIOC_G_FMT, &vfmt ) < 0 )
        {
            msg_Warn( p_input, "VIDIOC_G_FMT failed" );
        }
        else
        {
            if ( p_sys->i_width != -1 )
            {
                vfmt.fmt.pix.width = p_sys->i_width;
            }

            if ( p_sys->i_height != -1 )
            {
                vfmt.fmt.pix.height = p_sys->i_height;
            }

            if ( ioctl( p_sys->i_fd, VIDIOC_S_FMT, &vfmt ) < 0 )
            {
                msg_Warn( p_input, "VIDIOC_S_FMT failed" );
            }
            else
            {
                msg_Dbg( p_input, "picture size set to:%dx%d",
                         vfmt.fmt.pix.width, vfmt.fmt.pix.height );
            }
        }
    }

    /* set the frequency */
    if ( p_sys->i_frequency != -1 )
    {
        vf.tuner = 0; /* TODO: let the user choose the tuner */
        if ( ioctl( p_sys->i_fd, VIDIOC_G_FREQUENCY, &vf ) < 0 )
        {
            msg_Warn( p_input, "VIDIOC_G_FREQUENCY failed (%s)",
                      strerror( errno ) );
        }
        else
        {
            vf.frequency = p_sys->i_frequency * 16 / 1000;
            if( ioctl( p_sys->i_fd, VIDIOC_S_FREQUENCY, &vf ) < 0 )
            {
                msg_Warn( p_input, "VIDIOC_S_FREQUENCY failed (%s)",
                          strerror( errno ) );
            }
            else
            {
                msg_Dbg( p_input, "Tuner frequency set to:%d",
                         p_sys->i_frequency);
            }
        }
    }

    /* codec parameters */
    if ( p_sys->i_framerate != -1
            || p_sys->i_bitrate_mode != -1
            || p_sys->i_bitrate_peak != -1
            || p_sys->i_bitrate != -1
            || p_sys->i_audio_bitmask != -1 )
    {
        if ( ioctl( p_sys->i_fd, IVTV_IOC_G_CODEC, &codec ) < 0 )
        {
            msg_Warn( p_input, "IVTV_IOC_G_CODEC failed" );
        }
        else
        {
            if ( p_sys->i_framerate != -1 )
            {
                switch ( p_sys->i_framerate )
                {
                    case 30:
                        codec.framerate = 0;
                        break;

                    case 25:
                        codec.framerate = 1;
                        break;

                    default:
                        msg_Warn( p_input, "invalid framerate, reverting to 25" );
                        codec.framerate = 1;
                        break;
                }
            }

            if ( p_sys->i_bitrate != -1 )
            {
                codec.bitrate = p_sys->i_bitrate;
            }

            if ( p_sys->i_bitrate_peak != -1 )
            {
                codec.bitrate_peak = p_sys->i_bitrate_peak;
            }

            if ( p_sys->i_bitrate_mode != -1 )
            {
                codec.bitrate_mode = p_sys->i_bitrate_mode;
            }

            if ( p_sys->i_audio_bitmask != -1 )
            {
                codec.audio_bitmask = p_sys->i_audio_bitmask;
            }

            if( ioctl( p_sys->i_fd, IVTV_IOC_S_CODEC, &codec ) < 0 )
            {
                msg_Warn( p_input, "IVTV_IOC_S_CODEC failed" );
            }
            else
            {
                msg_Dbg( p_input, "Setting codec parameters to:  framerate: %d, bitrate: %d/%d/%d",
               codec.framerate, codec.bitrate, codec.bitrate_peak, codec.bitrate_mode );
            }
        }
    }

    /* do a quick read */
#if 0
    if ( p_sys->i_fd )
    {
        if ( read( p_sys->i_fd, psz_tmp, 1 ) )
        {
            msg_Dbg(p_input, "Could read byte from device");
        }
        else
        {
            msg_Warn(p_input, "Could not read byte from device");
        }
    }
#endif
    return VLC_SUCCESS;
}

/*****************************************************************************
 * Close: close the device
 *****************************************************************************/
static void Close( vlc_object_t * p_this )
{
    input_thread_t * p_input = (input_thread_t*) p_this;
    access_sys_t * p_sys = p_input->p_access_data;

    close( p_sys->i_fd );
    free( p_sys );
}

/*****************************************************************************
 * Read
 *****************************************************************************/
static ssize_t Read( input_thread_t * p_input, byte_t * p_buffer,
                     size_t i_len )
{
    access_sys_t * p_sys = p_input->p_access_data;

    int i_ret;
    
    struct timeval timeout;
    fd_set fds;

    FD_ZERO( &fds );
    FD_SET( p_sys->i_fd, &fds );
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000;

    while( !( i_ret = select( p_sys->i_fd + 1, &fds,
                              NULL, NULL, &timeout ) ) )
    {
        FD_ZERO( &fds );
        FD_SET( p_sys->i_fd, &fds );
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;

        if( p_input->b_die || p_input->b_error )
            return 0;
    }

    if( i_ret < 0 )
    {
        msg_Err( p_input, "select error (%s)", strerror( errno ) );
        return -1;
    }

    i_ret = read( p_sys->i_fd, p_buffer, i_len );
    return i_ret;
}

