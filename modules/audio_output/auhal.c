/*****************************************************************************
 * auhal.c: AUHAL output plugin
 *****************************************************************************
 * Copyright (C) 2005 VideoLAN
 * $Id$
 *
 * Authors: Derk-Jan Hartman <hartman at videolan dot org>
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
#include <string.h>
#include <stdlib.h>

#include <vlc/vlc.h>
#include <vlc/aout.h>

#include "aout_internal.h"

#include <CoreAudio/CoreAudio.h>
#include <CoreAudio/CoreAudioTypes.h>
#include <AudioUnit/AudioUnitProperties.h>
#include <AudioUnit/AudioUnitParameters.h>
#include <AudioUnit/AudioOutputUnit.h>
#include <AudioToolbox/AudioFormat.h>

#define STREAM_FORMAT_MSG( pre, sfm ) \
    pre ":\nsamplerate: [%ld]\nFormatID: [%4.4s]\nFormatFlags: [%ld]\nBypesPerPacket: [%ld]\nFramesPerPacket: [%ld]\nBytesPerFrame: [%ld]\nChannelsPerFrame: [%ld]\nBitsPerChannel[%ld]", \
    (UInt32)sfm.mSampleRate, (char *)&sfm.mFormatID, \
    sfm.mFormatFlags, sfm.mBytesPerPacket, \
    sfm.mFramesPerPacket, sfm.mBytesPerFrame, \
    sfm.mChannelsPerFrame, sfm.mBitsPerChannel

#define BUFSIZE 0xffffff

/*****************************************************************************
 * aout_sys_t: private audio output method descriptor
 *****************************************************************************
 * This structure is part of the audio output thread descriptor.
 * It describes the CoreAudio specific properties of an output thread.
 *****************************************************************************/
struct aout_sys_t
{
    AudioDeviceID               i_default_dev;  /* Keeps DeviceID of defaultOutputDevice */
    AudioDeviceID               i_selected_dev; /* Keeps DeviceID of the selected device */
    UInt32                      i_devices;      /* Number of CoreAudio Devices */
    vlc_bool_t                  b_supports_digital;/* Does the currently selected device support digital mode? */
    vlc_bool_t                  b_digital;      /* Are we running in digital mode? */
    Component                   au_component;   /* The Audiocomponent we use */
    AudioUnit                   au_unit;        /* The AudioUnit we use */
    mtime_t                     clock_diff;
    uint8_t                      p_remainder_buffer[BUFSIZE];
    uint32_t                    i_read_bytes;
    uint32_t                    i_total_bytes;
    audio_date_t                end_date_t;
    
    
};

/*****************************************************************************
 * Local prototypes.
 *****************************************************************************/
static int      Open                    ( vlc_object_t * );
static void     Close                   ( vlc_object_t * );

static void     Play                    ( aout_instance_t *);

static int      Probe                   ( aout_instance_t * );
static int      DeviceDigitalMode       ( aout_instance_t *, AudioDeviceID );
int             AudioDeviceHasOutput    ( AudioDeviceID );
static int      DigitalInit             ( aout_instance_t * );

static OSStatus RenderCallbackAnalog    ( vlc_object_t *, AudioUnitRenderActionFlags *, const AudioTimeStamp *,
                                          unsigned int, unsigned int, AudioBufferList *);
static OSStatus HardwareListener        ( AudioHardwarePropertyID, void *);

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define ADEV_TEXT N_("Audio Device")
#define ADEV_LONGTEXT N_("Choose a number corresponding to the number of an " \
    "audio device, as listed in your 'Audio Device' menu. This device will " \
    "then be used by default for audio playback.")

vlc_module_begin();
    set_shortname( "auhal" );
    set_description( _("HAL AudioUnit output") );
    set_capability( "audio output", 101 );
    set_category( CAT_AUDIO );
    set_subcategory( SUBCAT_AUDIO_AOUT );
    set_callbacks( Open, Close );
    //add_integer( "coreaudio-dev", -1, NULL, ADEV_TEXT, ADEV_LONGTEXT, VLC_FALSE ); 
vlc_module_end();

/*****************************************************************************
 * Open: open a HAL AudioUnit
 *****************************************************************************/
static int Open( vlc_object_t * p_this )
{
    OSStatus                err = noErr;
    ComponentDescription    desc;
    UInt32                  i_param_size,i;
    struct aout_sys_t       *p_sys;
    vlc_value_t             val;
    aout_instance_t         *p_aout = (aout_instance_t *)p_this;

    /* Allocate structure */
    p_sys = (struct aout_sys_t *)malloc( sizeof( struct aout_sys_t ) );
    if( p_sys == NULL )
    {
        msg_Err( p_aout, "out of memory" );
        return( VLC_ENOMEM );
    }

    memset( p_sys, 0, sizeof( struct aout_sys_t ) );

    p_sys->b_digital = VLC_FALSE; /* We assume we are not digital */

    p_aout->output.p_sys = p_sys;
    p_aout->output.pf_play = Play;
    
    aout_FormatPrint( p_aout, "VLC is looking for:\n", (audio_sample_format_t *)&p_aout->output.output );
    
    /* Build a list of devices */
    if( var_Type( p_aout, "audio-device" ) == 0 )
    {
        Probe( p_aout );
        /*if( Probe( p_aout ) != VLC_SUCCESS );
        {
            msg_Err( p_aout, "Probe failed" );
            free( p_sys );
            return VLC_EGENERIC;
        }*/
    }
    
    /* What device do we want? */
    if( var_Get( p_aout, "audio-device", &val ) < 0 )
    {
        msg_Err( p_aout, "audio-device var does not exist" );
        free( p_sys );
        return( VLC_ENOVAR );
    }
    p_sys->i_selected_dev = val.i_int;

    /* what is vlc format? if digital, take digital route else AUHAL route */
    DeviceDigitalMode( p_aout, p_sys->i_selected_dev );
    /*if( DeviceDigitalMode( p_aout, p_sys->i_selected_dev ) != VLC_SUCCESS );
    {
        msg_Err( p_aout, "DeviceDigitalMode failed" );
        free( p_sys );
        return VLC_EGENERIC;
    }
    */
    
    if( AOUT_FMT_NON_LINEAR( &p_aout->output.output ) && p_sys->b_supports_digital )
    {
        p_sys->b_digital = VLC_TRUE;
        p_aout->output.output.i_format = VLC_FOURCC('s','p','d','i');
        msg_Dbg( p_aout, "we found a digital stream, and we WANT a digital stream" );
    }
    else if( AOUT_FMT_NON_LINEAR( &p_aout->output.output ) && !p_sys->b_supports_digital )
    {
        msg_Dbg( p_aout, "we had requested a digital stream, but it's not possible for this device" );
    }
 
    /* If analog only start setting up AUHAL */

    /* Lets go find our Component */
    desc.componentType = kAudioUnitType_Output;
    desc.componentSubType = kAudioUnitSubType_HALOutput;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;

    p_sys->au_component = FindNextComponent( NULL, &desc );
    if( p_sys->au_component == NULL )
    {
        msg_Err( p_aout, "we cannot find our HAL component" );
        free( p_sys );
        return VLC_EGENERIC;
    }

    err = OpenAComponent( p_sys->au_component, &p_sys->au_unit );
    if( err )
    {
        
        msg_Err( p_aout, "we cannot find our HAL component" );
        free( p_sys );
        return VLC_EGENERIC;
    }
    
    /* Enable IO for the component */
    
    /* Set the device */
    verify_noerr( AudioUnitSetProperty( p_sys->au_unit,
                         kAudioOutputUnitProperty_CurrentDevice,
                         kAudioUnitScope_Global,
                         0,
                         &p_sys->i_selected_dev,
                         sizeof(p_sys->i_selected_dev)));
                         
    /* Get the current format */
    AudioStreamBasicDescription DeviceFormat;
    
    i_param_size = sizeof(AudioStreamBasicDescription);

    verify_noerr( AudioUnitGetProperty( p_sys->au_unit,
                                   kAudioUnitProperty_StreamFormat,
                                   kAudioUnitScope_Input,
                                   0,
                                   &DeviceFormat,
                                   &i_param_size ));
                                   
    msg_Dbg( p_aout, STREAM_FORMAT_MSG( "current format is " , DeviceFormat ) );

    /* Get the channel layout */
    AudioChannelLayout *layout;
    verify_noerr( AudioUnitGetPropertyInfo( p_sys->au_unit,
                                   kAudioDevicePropertyPreferredChannelLayout,
                                   kAudioUnitScope_Output,
                                   0,
                                   &i_param_size,
                                   NULL ));

    layout = (AudioChannelLayout *)malloc( i_param_size);

    verify_noerr( AudioUnitGetProperty( p_sys->au_unit,
                                   kAudioDevicePropertyPreferredChannelLayout,
                                   kAudioUnitScope_Output,
                                   0,
                                   layout,
                                   &i_param_size ));
                                   
    /* Lets fill out the ChannelLayout */
    if( layout->mChannelLayoutTag == kAudioChannelLayoutTag_UseChannelBitmap)
    {
        msg_Dbg( p_aout, "bitmap defined channellayout" );
        verify_noerr( AudioFormatGetProperty( kAudioFormatProperty_ChannelLayoutForBitmap,
                                sizeof( UInt32), &layout->mChannelBitmap,
                                &i_param_size,
                                layout ));
    }
    else if( layout->mChannelLayoutTag != kAudioChannelLayoutTag_UseChannelDescriptions )
    {
        msg_Dbg( p_aout, "layouttags defined channellayout" );
        verify_noerr( AudioFormatGetProperty( kAudioFormatProperty_ChannelLayoutForTag,
                                sizeof( AudioChannelLayoutTag ), &layout->mChannelLayoutTag,
                                &i_param_size,
                                layout ));
    }

    msg_Dbg( p_aout, "Layout of AUHAL has %d channels" , (int)layout->mNumberChannelDescriptions );
    
    p_aout->output.output.i_physical_channels = 0;
    for( i = 0; i < layout->mNumberChannelDescriptions; i++ )
    {
        msg_Dbg( p_aout, "This is channel: %d", (int)layout->mChannelDescriptions[i].mChannelLabel );

        switch( layout->mChannelDescriptions[i].mChannelLabel )
        {
            case kAudioChannelLabel_Left:
                p_aout->output.output.i_physical_channels |= AOUT_CHAN_LEFT;
                continue;
            case kAudioChannelLabel_Right:
                p_aout->output.output.i_physical_channels |= AOUT_CHAN_RIGHT;
                continue;
            case kAudioChannelLabel_Center:
                p_aout->output.output.i_physical_channels |= AOUT_CHAN_CENTER;
                continue;
            case kAudioChannelLabel_LFEScreen:
                p_aout->output.output.i_physical_channels |= AOUT_CHAN_LFE;
                continue;
            case kAudioChannelLabel_LeftSurround:
                p_aout->output.output.i_physical_channels |= AOUT_CHAN_REARLEFT;
                continue;
            case kAudioChannelLabel_RightSurround:
                p_aout->output.output.i_physical_channels |= AOUT_CHAN_REARRIGHT;
                continue;
            case kAudioChannelLabel_RearSurroundLeft:
                p_aout->output.output.i_physical_channels |= AOUT_CHAN_MIDDLELEFT;
                continue;
            case kAudioChannelLabel_RearSurroundRight:
                p_aout->output.output.i_physical_channels |= AOUT_CHAN_MIDDLERIGHT;
                continue;
            case kAudioChannelLabel_CenterSurround:
                p_aout->output.output.i_physical_channels |= AOUT_CHAN_REARCENTER;
                continue;
            default:
                msg_Warn( p_aout, "Unrecognized channel form provided by driver: %d", (int)layout->mChannelDescriptions[i].mChannelLabel );
                if( i == 0 )
                {
                    msg_Warn( p_aout, "Probably no channellayout is set. force based on channelcount" );
                    switch( layout->mNumberChannelDescriptions )
                    {
                        /* We make assumptions based on number of channels here.
                         * Unfortunatly Apple has provided no 100% method to retrieve the speaker configuration */
                        case 1:
                            p_aout->output.output.i_physical_channels = AOUT_CHAN_CENTER;
                            break;
                        case 4:
                            p_aout->output.output.i_physical_channels = AOUT_CHAN_LEFT | AOUT_CHAN_RIGHT |
                                                                        AOUT_CHAN_REARLEFT | AOUT_CHAN_REARRIGHT;
                            break;
                        case 6:
                            p_aout->output.output.i_physical_channels = AOUT_CHAN_LEFT | AOUT_CHAN_RIGHT |
                                                                        AOUT_CHAN_CENTER | AOUT_CHAN_LFE |
                                                                        AOUT_CHAN_REARLEFT | AOUT_CHAN_REARRIGHT;
                            break;
                        case 7:
                            p_aout->output.output.i_physical_channels = AOUT_CHAN_LEFT | AOUT_CHAN_RIGHT |
                                                                        AOUT_CHAN_CENTER | AOUT_CHAN_LFE |
                                                                        AOUT_CHAN_REARLEFT | AOUT_CHAN_REARRIGHT | AOUT_CHAN_REARCENTER;
                            break;
                        case 8:
                            p_aout->output.output.i_physical_channels = AOUT_CHAN_LEFT | AOUT_CHAN_RIGHT |
                                                                        AOUT_CHAN_CENTER | AOUT_CHAN_LFE |
                                                                        AOUT_CHAN_REARLEFT | AOUT_CHAN_REARRIGHT |
                                                                        AOUT_CHAN_REARLEFT | AOUT_CHAN_REARRIGHT;
                            break;
                        case 2:
                        default:
                            p_aout->output.output.i_physical_channels = AOUT_CHAN_LEFT | AOUT_CHAN_RIGHT;
                    }
                }
                break;
        }
    }
    free( layout );

    msg_Dbg( p_aout, "defined %d physical channels for vlc core", aout_FormatNbChannels( &p_aout->output.output ) );
    msg_Dbg( p_aout, "%s", aout_FormatPrintChannels( &p_aout->output.output ));
    
    AudioChannelLayout new_layout;
    memset (&new_layout, 0, sizeof(new_layout));
    switch( aout_FormatNbChannels( &p_aout->output.output ) )
    {
        case 1:
            new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_Mono;
            break;
        case 2:
            new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_Stereo;
            break;
        case 3:
            if( p_aout->output.output.i_physical_channels & AOUT_CHAN_CENTER )
            {
                new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_DVD_7; // L R C
            }
            else if( p_aout->output.output.i_physical_channels & AOUT_CHAN_LFE )
            {
                new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_DVD_4; // L R LFE
            }
            break;
        case 4:
            if( p_aout->output.output.i_physical_channels & ( AOUT_CHAN_CENTER | AOUT_CHAN_LFE ) )
            {
                new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_DVD_10; // L R C LFE
            }
            else if( p_aout->output.output.i_physical_channels & ( AOUT_CHAN_REARLEFT | AOUT_CHAN_REARRIGHT ) )
            {
                new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_DVD_3; // L R Ls Rs
            }
            else if( p_aout->output.output.i_physical_channels & ( AOUT_CHAN_CENTER | AOUT_CHAN_REARCENTER ) )
            {
                new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_DVD_3; // L R C Cs
            }
            break;
        case 5:
            if( p_aout->output.output.i_physical_channels & ( AOUT_CHAN_CENTER ) )
            {
                new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_DVD_19; // L R Ls Rs C
            }
            else if( p_aout->output.output.i_physical_channels & ( AOUT_CHAN_LFE ) )
            {
                new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_DVD_18; // L R Ls Rs LFE
            }
            break;
        case 6:
            new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_DVD_20; // L R Ls Rs C LFE
            break;
        case 7:
            /* FIXME: This is incorrect. VLC uses the internal ordering: L R Lm Rm Lr Rr C LFE but this is wrong */
            new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_MPEG_6_1_A; // L R C LFE Ls Rs Cs
            break;
        case 8:
            /* FIXME: This is incorrect. VLC uses the internal ordering: L R Lm Rm Lr Rr C LFE but this is wrong */
            new_layout.mChannelLayoutTag = kAudioChannelLayoutTag_MPEG_7_1_A; // L R C LFE Ls Rs Lc Rc
            break;
    }

    /* Set up the format to be used */
    DeviceFormat.mSampleRate = p_aout->output.output.i_rate;
    DeviceFormat.mFormatID = kAudioFormatLinearPCM;

    /* We use float 32. It's the best supported format by both VLC and Coreaudio */
    p_aout->output.output.i_format = VLC_FOURCC( 'f','l','3','2');
    DeviceFormat.mFormatFlags = kAudioFormatFlagsNativeFloatPacked;
    DeviceFormat.mBitsPerChannel = 32;
    DeviceFormat.mChannelsPerFrame = aout_FormatNbChannels( &p_aout->output.output );
    
    /* Calculate framesizes and stuff */
    aout_FormatPrepare( &p_aout->output.output );
    DeviceFormat.mFramesPerPacket = 1;
    DeviceFormat.mBytesPerFrame = DeviceFormat.mBitsPerChannel * DeviceFormat.mChannelsPerFrame / 8;
    DeviceFormat.mBytesPerPacket = DeviceFormat.mBytesPerFrame * DeviceFormat.mFramesPerPacket;
 
    i_param_size = sizeof(AudioStreamBasicDescription);
    /* Set desired format (Use CAStreamBasicDescription )*/
    verify_noerr( AudioUnitSetProperty( p_sys->au_unit,
                                   kAudioUnitProperty_StreamFormat,
                                   kAudioUnitScope_Input,
                                   0,
                                   &DeviceFormat,
                                   i_param_size ));
                                   
    msg_Dbg( p_aout, STREAM_FORMAT_MSG( "we set the AU format: " , DeviceFormat ) );
    
    /* Retrieve actual format??? */
    verify_noerr( AudioUnitGetProperty( p_sys->au_unit,
                                   kAudioUnitProperty_StreamFormat,
                                   kAudioUnitScope_Input,
                                   0,
                                   &DeviceFormat,
                                   &i_param_size ));
                                   
    msg_Dbg( p_aout, STREAM_FORMAT_MSG( "the actual set AU format is " , DeviceFormat ) );

    p_aout->output.i_nb_samples = 2048;
    aout_VolumeSoftInit( p_aout );

    /* Let's pray for the following operation to be atomic... */
    p_sys->clock_diff = - (mtime_t)
        AudioConvertHostTimeToNanos( AudioGetCurrentHostTime() ) / 1000; 
    p_sys->clock_diff += mdate();
    
    p_sys->i_read_bytes = 0;
    p_sys->i_total_bytes = 0;

    /* set the IOproc callback */
    AURenderCallbackStruct input;
    input.inputProc = (AURenderCallback) RenderCallbackAnalog;
    input.inputProcRefCon = p_aout;
    
    verify_noerr( AudioUnitSetProperty( p_sys->au_unit,
                            kAudioUnitProperty_SetRenderCallback,
                            kAudioUnitScope_Input,
                            0, &input, sizeof( input ) ) );

    input.inputProc = (AURenderCallback) RenderCallbackAnalog;
    input.inputProcRefCon = p_aout;
    
    /* Set the new_layout as the layout VLC feeds to the AU unit */
    verify_noerr( AudioUnitSetProperty( p_sys->au_unit,
                            kAudioUnitProperty_AudioChannelLayout,
                            kAudioUnitScope_Input,
                            0, &new_layout, sizeof(new_layout) ) );
    
    /* AU initiliaze */
    verify_noerr( AudioUnitInitialize(p_sys->au_unit) );

    verify_noerr( AudioOutputUnitStart(p_sys->au_unit) );
    return( VLC_SUCCESS );
}

/*****************************************************************************
 * Close: Close HAL AudioUnit
 *****************************************************************************/
static void Close( vlc_object_t * p_this )
{
    aout_instance_t     *p_aout = (aout_instance_t *)p_this;
    struct aout_sys_t   *p_sys = p_aout->output.p_sys;
    
    if( p_sys->au_unit )
    {
        verify_noerr( AudioOutputUnitStop( p_sys->au_unit ) );
        verify_noerr( AudioUnitUninitialize( p_sys->au_unit ) );
        verify_noerr( CloseComponent( p_sys->au_unit ) );
    }
    free( p_sys );
}

/*****************************************************************************
 * Play: nothing to do
 *****************************************************************************/
static void Play( aout_instance_t * p_aout )
{
}


/*****************************************************************************
 * Probe
 *****************************************************************************/
static int Probe( aout_instance_t * p_aout )
{
    OSStatus            err = noErr;
    UInt32              i, i_param_size;
    AudioDeviceID       devid_def;
    AudioDeviceID       *p_devices = NULL;
    vlc_value_t         val, text;

    struct aout_sys_t   *p_sys = p_aout->output.p_sys;

    /* Get number of devices */
    err = AudioHardwareGetPropertyInfo( kAudioHardwarePropertyDevices,
                                        &i_param_size, NULL );
    if( err != noErr )
    {
        msg_Err( p_aout, "could not get number of devices: [%4.4s]", (char *)&err );
        goto error;
    }

    p_sys->i_devices = i_param_size / sizeof( AudioDeviceID );

    if( p_sys->i_devices < 1 )
    {
        msg_Err( p_aout, "no devices found" );
        goto error;
    }

    msg_Dbg( p_aout, "system has [%ld] device(s)", p_sys->i_devices );

    /* Allocate DeviceID array */
    p_devices = (AudioDeviceID *)malloc( i_param_size );
    if( p_devices == NULL )
    {
        msg_Err( p_aout, "out of memory" );
        goto error;
    }

    /* Populate DeviceID array */
    err = AudioHardwareGetProperty( kAudioHardwarePropertyDevices,
                                    &i_param_size, (void *)p_devices );
    if( err != noErr )
    {
        msg_Err( p_aout, "could not get the device ID's: [%4.4s]", (char *)&err );
        goto error;
    }

    /* Find the ID of the default Device */
    i_param_size = sizeof( AudioDeviceID );
    err = AudioHardwareGetProperty( kAudioHardwarePropertyDefaultOutputDevice,
                                    &i_param_size, (void *)&devid_def );
    if( err != noErr )
    {
        msg_Err( p_aout, "could not get default audio device: [%4.4s]", (char *)&err );
        goto error;
    }
    p_sys->i_default_dev = devid_def;
    
    var_Create( p_aout, "audio-device", VLC_VAR_INTEGER|VLC_VAR_HASCHOICE );
    text.psz_string = _("Audio Device");
    var_Change( p_aout, "audio-device", VLC_VAR_SETTEXT, &text, NULL );
    
    for( i = 0; i < p_sys->i_devices; i++ )
    {
        char psz_devuid[1024];
        char psz_name[1024];
        CFStringRef devUID;

        i_param_size = sizeof psz_name;
        err = AudioDeviceGetProperty(
                    p_devices[i], 0, VLC_FALSE,
                    kAudioDevicePropertyDeviceName,
                    &i_param_size, psz_name);
        if( err )
            goto error;

        i_param_size = sizeof(CFStringRef);    
        err = AudioDeviceGetProperty(
                    p_devices[i], 0, VLC_FALSE,
                    kAudioDevicePropertyDeviceUID,
                    &i_param_size, &devUID);
        if( err )
            goto error;

        CFStringGetCString( devUID, psz_devuid, sizeof psz_devuid, CFStringGetSystemEncoding() );
        msg_Dbg( p_aout, "DevID: %lu  DevName: %s  DevUID: %s", p_devices[i], psz_name, psz_devuid );
        CFRelease( devUID );

        if( !AudioDeviceHasOutput( p_devices[i]) )
        {
            msg_Dbg( p_aout, "this device is INPUT only. skipping..." );
            continue;
        }

        val.i_int = (int) p_devices[i];
        text.psz_string = psz_name;
        var_Change( p_aout, "audio-device", VLC_VAR_ADDCHOICE, &val, &text );
        if( devid_def == p_devices[i] )
        {
            var_Change( p_aout, "audio-device", VLC_VAR_SETDEFAULT, &val, NULL );
            var_Set( p_aout, "audio-device", val );
        }
    }
    var_AddCallback( p_aout, "audio-device", aout_ChannelsRestart, NULL );
    
    /* attach a Listener so that we are notified of a change in the Device setup */
    /* err = AudioHardwareAddPropertyListener( kAudioHardwarePropertyDevices,
                                            HardwareListener, 
                                            (void *)p_aout );
    if( err )
        goto error;*/
    
    msg_Dbg( p_aout, "succesful finish of deviceslist" );
    if( p_devices ) free( p_devices );
    return (VLC_SUCCESS);

error:
    var_Destroy( p_aout, "audio-device" );
    if( p_devices ) free( p_devices );
    return VLC_EGENERIC;
}

/*****************************************************************************
 * DeviceDigitalMode: Check i_dev_id for digital stream support.
 *****************************************************************************/
static int DeviceDigitalMode( aout_instance_t *p_aout, AudioDeviceID i_dev_id )
{
    OSStatus                    err = noErr;
    UInt32                      i_param_size;
    AudioStreamBasicDescription *p_format_list;
    int                         i, i_formats;
    struct aout_sys_t           *p_sys = p_aout->output.p_sys;
    
    p_sys->b_supports_digital = VLC_FALSE;
    
    err = AudioDeviceGetPropertyInfo( i_dev_id, 0, FALSE,
                                      kAudioDevicePropertyStreamFormats,
                                      &i_param_size, NULL );
    if( err != noErr )
    {
        msg_Err( p_aout, "could not get number of streamsformats: [%4.4s]", (char *)&err );
        return( VLC_EGENERIC );
    }
    
    i_formats = i_param_size / sizeof( AudioStreamBasicDescription );
    p_format_list = (AudioStreamBasicDescription *)malloc( i_param_size );
    if( p_format_list == NULL )
    {
        return( VLC_ENOMEM );
    }
    
    err = AudioDeviceGetProperty( i_dev_id, 0, FALSE,
                                      kAudioDevicePropertyStreamFormats,
                                      &i_param_size, (void *)p_format_list );
    if( err != noErr )
    {
        msg_Err( p_aout, "could not get the list of formats: [%4.4s]", (char *)&err );
        return( VLC_EGENERIC );
    }

    for( i = 0; i < i_formats; i++ )
    {
        msg_Dbg( p_aout, STREAM_FORMAT_MSG( "supported format", p_format_list[i] ) );
        
        if( p_format_list[i].mFormatID == 'IAC3' ||
                  p_format_list[i].mFormatID == kAudioFormat60958AC3 )
        {
            p_sys->b_supports_digital = VLC_TRUE;
            msg_Dbg( p_aout, "this device supports a digital stream" );
            break;
        }
    }
    
    free( (void *)p_format_list );
    return VLC_SUCCESS;
}

/*****************************************************************************
 * RenderCallbackAnalog: This function is called everytime the AudioUnit wants
 * us to provide some more audio data.
 * Don't print anything during normal playback, calling blocking function from
 * this callback is not allowed.
 *****************************************************************************/
static OSStatus RenderCallbackAnalog( vlc_object_t *_p_aout,
                                      AudioUnitRenderActionFlags *ioActionFlags,
                                      const AudioTimeStamp *inTimeStamp,
                                      unsigned int inBusNummer,
                                      unsigned int inNumberFrames,
                                      AudioBufferList *ioData )
{
    AudioTimeStamp  host_time;
    mtime_t         current_date = 0;
    uint32_t        i_mData_bytes = 0;    

    aout_instance_t * p_aout = (aout_instance_t *)_p_aout;
    struct aout_sys_t * p_sys = p_aout->output.p_sys;

    host_time.mFlags = kAudioTimeStampHostTimeValid;
    AudioDeviceTranslateTime( p_sys->i_selected_dev, inTimeStamp, &host_time );

    p_sys->clock_diff = - (mtime_t)
        AudioConvertHostTimeToNanos( AudioGetCurrentHostTime() ) / 1000; 
    p_sys->clock_diff += mdate();

    current_date = p_sys->clock_diff +
                   AudioConvertHostTimeToNanos( host_time.mHostTime ) / 1000;

    if( ioData == NULL && ioData->mNumberBuffers < 1 )
    {
        msg_Err( p_aout, "no iodata or buffers");
        return 0;
    }
    if( ioData->mNumberBuffers > 1 )
        msg_Err( p_aout, "well this is weird. seems like there is more than one buffer..." );


    if( p_sys->i_total_bytes > 0 )
    {
        i_mData_bytes = __MIN( p_sys->i_total_bytes - p_sys->i_read_bytes, ioData->mBuffers[0].mDataByteSize );
        p_aout->p_vlc->pf_memcpy( ioData->mBuffers[0].mData, &p_sys->p_remainder_buffer[p_sys->i_read_bytes], i_mData_bytes );
        p_sys->i_read_bytes += i_mData_bytes;
        current_date += (mtime_t) ( (mtime_t) 1000000 / p_aout->output.output.i_rate ) *
                        ( i_mData_bytes / 4 / aout_FormatNbChannels( &p_aout->output.output )  ); // 4 is fl32 specific
        
        if( p_sys->i_read_bytes >= p_sys->i_total_bytes )
            p_sys->i_read_bytes = p_sys->i_total_bytes = 0;
    }
    
    while( i_mData_bytes < ioData->mBuffers[0].mDataByteSize )
    {
        /* We don't have enough data yet */
        aout_buffer_t * p_buffer;
        p_buffer = aout_OutputNextBuffer( p_aout, current_date , VLC_FALSE );
        
        if( p_buffer != NULL )
        {
            uint32_t i_second_mData_bytes = __MIN( p_buffer->i_nb_bytes, ioData->mBuffers[0].mDataByteSize - i_mData_bytes );
            
            p_aout->p_vlc->pf_memcpy( (uint8_t *)ioData->mBuffers[0].mData + i_mData_bytes, p_buffer->p_buffer, i_second_mData_bytes );
            i_mData_bytes += i_second_mData_bytes;

            if( i_mData_bytes >= ioData->mBuffers[0].mDataByteSize )
            {
                p_sys->i_total_bytes = p_buffer->i_nb_bytes - i_second_mData_bytes;
                p_aout->p_vlc->pf_memcpy( p_sys->p_remainder_buffer, &p_buffer->p_buffer[i_second_mData_bytes], p_sys->i_total_bytes );
            }
            else
            {
                // update current_date
                current_date += (mtime_t) ( (mtime_t) 1000000 / p_aout->output.output.i_rate ) *
                                ( i_second_mData_bytes / 4 / aout_FormatNbChannels( &p_aout->output.output )  ); // 4 is fl32 specific
            }
            aout_BufferFree( p_buffer );
        }
        else
        {
             p_aout->p_vlc->pf_memset( (uint8_t *)ioData->mBuffers[0].mData +i_mData_bytes, 0, ioData->mBuffers[0].mDataByteSize - i_mData_bytes );
             i_mData_bytes += ioData->mBuffers[0].mDataByteSize - i_mData_bytes;
        }
    }
    return( noErr );     
}


/*****************************************************************************
 * Setup a digital stream
 *****************************************************************************/
static int DigitalInit( aout_instance_t * p_aout )
{
    OSStatus            err = noErr;
    UInt32              i, i_param_size;
    AudioDeviceID       devid_def;
    AudioDeviceID       *p_devices = NULL;
    vlc_value_t         val, text;

    struct aout_sys_t   *p_sys = p_aout->output.p_sys;

    
    
    return (VLC_SUCCESS);

error:
    return VLC_EGENERIC;
}

int AudioDeviceHasOutput( AudioDeviceID i_dev_id )
{
    UInt32			dataSize;
    Boolean			isWritable;
	
    verify_noerr( AudioDeviceGetPropertyInfo( i_dev_id, 0, FALSE, kAudioDevicePropertyStreams, &dataSize, &isWritable) );
    if (dataSize == 0) return FALSE;
    
    return TRUE;
}


/*****************************************************************************
 * HardwareListener: Warns us of changes in the list of registered devices
 *****************************************************************************/
static OSStatus HardwareListener( AudioHardwarePropertyID inPropertyID,
                                  void * inClientData )
{
    OSStatus err = noErr;

    aout_instance_t     *p_aout = (aout_instance_t *)inClientData;
    /* struct aout_sys_t   *p_sys = p_aout->output.p_sys; */

    switch( inPropertyID )
    {
        case kAudioHardwarePropertyDevices:
        {
            /* something changed in the list of devices */
            /* We trigger the audio-device's aout_ChannelsRestart callback */
            var_Change( p_aout, "audio-device", VLC_VAR_TRIGGER_CALLBACKS, NULL, NULL );
        }
        break;
    }

    return( err );
}
