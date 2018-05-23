/*****************************************************************************
 * output.m: MacOS X Output Dialog
 *****************************************************************************
 * Copyright (C) 2002-2003 VideoLAN
 * $Id: output.m 7693 2004-05-16 20:27:47Z hartman $
 *
 * Authors: Jon Lech Johansen <jon-vl@nanocrew.net> 
 *          Christophe Massiot <massiot@via.ecp.fr>
 *          Derk-Jan Hartman <thedj@users.sourceforge.net>
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
#include <string.h>

#include "intf.h"
#include "output.h"

/*****************************************************************************
 * VLCOutput implementation 
 *****************************************************************************/
@implementation VLCOutput

- (id)init
{
    self = [super init];
    o_mrl = [[NSString alloc] init];
    o_transcode = [[NSString alloc] init];
    return self;
}

- (void)dealloc
{
    [o_mrl release];
    [o_transcode release];
    [super dealloc];
}

- (void)setMRL:(NSString *)o_mrl_string
{
    [o_mrl autorelease];
    o_mrl = [o_mrl_string copy];
}

- (NSString *)getMRL
{
    return [o_mrl copy];
}

- (void)setTranscode:(NSString *)o_transcode_string
{
    [o_transcode autorelease];
    o_transcode = [o_transcode_string copy];
}

- (void)awakeFromNib
{
    [self initStrings];
    
    [[NSNotificationCenter defaultCenter] addObserver: self
        selector: @selector(outputInfoChanged:)
        name: NSControlTextDidChangeNotification
        object: o_file_field];
    [[NSNotificationCenter defaultCenter] addObserver: self
        selector: @selector(outputInfoChanged:)
        name: NSControlTextDidChangeNotification
        object: o_stream_address];
    [[NSNotificationCenter defaultCenter] addObserver: self
        selector: @selector(outputInfoChanged:)
        name: NSControlTextDidChangeNotification
        object: o_stream_port];
    [[NSNotificationCenter defaultCenter] addObserver: self
        selector: @selector(TTLChanged:)
        name: NSControlTextDidChangeNotification
        object: o_stream_ttl];
    [[NSNotificationCenter defaultCenter] addObserver: self
        selector: @selector(transcodeInfoChanged:)
        name: NSControlTextDidChangeNotification
        object: o_transcode_video_bitrate];
    [[NSNotificationCenter defaultCenter] addObserver: self
        selector: @selector(transcodeInfoChanged:)
        name: NSControlTextDidChangeNotification
        object: o_transcode_audio_bitrate];
    [[NSNotificationCenter defaultCenter] addObserver: self
        selector: @selector(transcodeInfoChanged:)
        name: NSControlTextDidChangeNotification
        object: o_transcode_audio_channels];
    [[NSNotificationCenter defaultCenter] addObserver: self
        selector: @selector(transcodeInfoChanged:)
        name: NSControlTextDidChangeNotification
        object: o_channel_name];

    [o_mux_selector setAutoenablesItems: NO];
    [self transcodeChanged:nil];
}

- (void)initStrings
{
    NSArray *o_muxers = [NSArray arrayWithObjects: @"MPEG TS", @"MPEG PS", @"MPEG 1",
        @"Ogg", @"AVI", @"ASF", @"MPEG 4", @"Quicktime", @"Raw", nil];
    NSArray *o_a_channels = [NSArray arrayWithObjects: @"1", @"2", @"4", @"6", nil];
    NSArray *o_a_bitrates = [NSArray arrayWithObjects: @"16", @"32", @"64", @"96",
        @"128", @"192", @"256", @"512", nil];
    NSArray *o_v_bitrates = [NSArray arrayWithObjects: @"16", @"32", @"64", @"96",
        @"128", @"192", @"256", @"384", @"512", @"768", @"1024", @"2048", @"3072", nil];
    NSArray *o_a_codecs = [NSArray arrayWithObjects: @"mpga", @"mp3 ", @"mp4a", @"a52 ", @"vorb", @"flac", @"spx ", nil];
    NSArray *o_v_codecs = [NSArray arrayWithObjects: @"mp1v", @"mp2v", @"mp4v", @"DIV1",
        @"DIV2", @"DIV3", @"H263", @"I263", @"WMV1", @"WMV2", @"MJPG", @"theo", nil];
    
    [o_output_ckbox setTitle: _NS("Advanced output:")];
    [o_output_settings setTitle: _NS("Settings...")];
    [o_btn_ok setTitle: _NS("OK")];
    
    [o_options_lbl setTitle: _NS("Output Options")];
    [o_display setTitle: _NS("Play locally")];
    [[o_method cellAtRow:0 column:0] setTitle: _NS("File")];
    [[o_method cellAtRow:1 column:0] setTitle: _NS("Stream")];
    [o_dump_chkbox setTitle: _NS("Dump raw input")];
    [o_btn_browse setTitle: _NS("Browse...")]; 
    [o_stream_address_lbl setStringValue: _NS("Address")];
    [o_stream_port_lbl setStringValue: _NS("Port")];
    [o_stream_ttl_lbl setStringValue: @"TTL"];
    [[o_stream_type itemAtIndex: 0] setTitle: @"HTTP"];
    [[o_stream_type itemAtIndex: 1] setTitle: @"MMSH"];
    [[o_stream_type itemAtIndex: 2] setTitle: @"UDP"];
    [[o_stream_type itemAtIndex: 3] setTitle: @"RTP"];
    [o_stream_type_lbl setStringValue: _NS("Type")];
    
    [o_mux_lbl setStringValue: _NS("Encapsulation Method")];
    [o_mux_selector removeAllItems];
    [o_mux_selector addItemsWithTitles: o_muxers];
    
    [o_transcode_lbl setTitle: _NS("Transcode options")];
    [o_transcode_video_chkbox setTitle: _NS("Video")];
    [o_transcode_video_selector removeAllItems];
    [o_transcode_video_selector addItemsWithTitles: o_v_codecs];
    [o_transcode_video_bitrate_lbl setStringValue: _NS("Bitrate (kb/s)")];
    [o_transcode_video_bitrate removeAllItems];
    [o_transcode_video_bitrate addItemsWithObjectValues: o_v_bitrates];
    [o_transcode_audio_chkbox setTitle: _NS("Audio")];
    [o_transcode_audio_selector removeAllItems];
    [o_transcode_audio_selector addItemsWithTitles: o_a_codecs];
    [o_transcode_audio_bitrate_lbl setStringValue: _NS("Bitrate (kb/s)")];
    [o_transcode_audio_bitrate removeAllItems];
    [o_transcode_audio_bitrate addItemsWithObjectValues: o_a_bitrates];
    [o_transcode_audio_channels_lbl setStringValue: _NS("Channels")];
    [o_transcode_audio_channels removeAllItems];
    [o_transcode_audio_channels addItemsWithObjectValues: o_a_channels];
    
    [o_misc_lbl setTitle: _NS("Stream Announcing")];
    [o_sap_chkbox setTitle: _NS("SAP announce")];
    [o_slp_chkbox setTitle: _NS("SLP announce")];
    [o_channel_name_lbl setStringValue: _NS("Channel Name")];
}

- (IBAction)outputChanged:(id)sender;
{
    if ([o_output_ckbox state] == NSOnState)
    {
        [o_output_settings setEnabled:YES];
    }
    else
    {
        [o_output_settings setEnabled:NO];
    }
}

- (IBAction)outputSettings:(id)sender
{
    [NSApp beginSheet: o_output_sheet
        modalForWindow: o_open_panel
        modalDelegate: self
        didEndSelector: NULL
        contextInfo: nil];
}

- (IBAction)outputCloseSheet:(id)sender
{
    [o_output_sheet orderOut:sender];
    [NSApp endSheet: o_output_sheet];
}

- (void)outputMethodChanged:(NSNotification *)o_notification
{
    NSString *o_mode;
    o_mode = [[o_method selectedCell] title];
    
    [o_sap_chkbox setEnabled: NO];
    [o_slp_chkbox setEnabled: NO];
    [o_channel_name setEnabled: NO];
    [[o_mux_selector itemAtIndex: 0] setEnabled: YES];

    if( [o_mode isEqualToString: _NS("File")] )
    {
        [o_file_field setEnabled: YES];
        [o_btn_browse setEnabled: YES];
        [o_dump_chkbox setEnabled: YES];
        [o_stream_address setEnabled: NO];
        [o_stream_port setEnabled: NO];
        [o_stream_ttl setEnabled: NO];
        [o_stream_port_stp setEnabled: NO];
        [o_stream_ttl_stp setEnabled: NO];
        [o_stream_type setEnabled: NO];
        [o_mux_selector setEnabled: YES];
        [[o_mux_selector itemAtIndex: 1] setEnabled: YES]; // MPEG PS
        [[o_mux_selector itemAtIndex: 2] setEnabled: YES]; // MPEG 1
        [[o_mux_selector itemAtIndex: 3] setEnabled: YES]; // Ogg
        [[o_mux_selector itemAtIndex: 4] setEnabled: YES]; // AVI
        [[o_mux_selector itemAtIndex: 5] setEnabled: YES]; // ASF
        [[o_mux_selector itemAtIndex: 6] setEnabled: YES]; // MPEG 4
        [[o_mux_selector itemAtIndex: 7] setEnabled: YES]; // QuickTime
        [[o_mux_selector itemAtIndex: 8] setEnabled: YES]; // Raw
    }
    else if( [o_mode isEqualToString: _NS("Stream")] )
    {
        [o_file_field setEnabled: NO];
        [o_dump_chkbox setEnabled: NO];
        [o_btn_browse setEnabled: NO];
        [o_stream_port setEnabled: YES];
        [o_stream_port_stp setEnabled: YES];
        [o_stream_type setEnabled: YES];
        [o_mux_selector setEnabled: YES];
        
        o_mode = [o_stream_type titleOfSelectedItem];
        
        if( [o_mode isEqualToString: @"HTTP"] )
        {
            [o_stream_address setEnabled: YES];
            [o_stream_ttl setEnabled: NO];
            [o_stream_ttl_stp setEnabled: NO];
            [[o_mux_selector itemAtIndex: 1] setEnabled: YES];
            [[o_mux_selector itemAtIndex: 2] setEnabled: YES];
            [[o_mux_selector itemAtIndex: 3] setEnabled: YES];
            [[o_mux_selector itemAtIndex: 4] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 5] setEnabled: YES];
            [[o_mux_selector itemAtIndex: 6] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 7] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 8] setEnabled: YES];
        }
        else if( [o_mode isEqualToString: @"MMSH"] )
        {
            [o_stream_address setEnabled: YES];
            [o_stream_ttl setEnabled: NO];
            [o_stream_ttl_stp setEnabled: NO];
            [[o_mux_selector itemAtIndex: 0] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 1] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 2] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 3] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 4] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 5] setEnabled: YES];
            [[o_mux_selector itemAtIndex: 6] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 7] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 8] setEnabled: NO];
        }
        else if( [o_mode isEqualToString: @"UDP"] )
        {
            [o_stream_address setEnabled: YES];
            [o_stream_ttl setEnabled: YES];
            [o_stream_ttl_stp setEnabled: YES];
            [[o_mux_selector itemAtIndex: 1] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 2] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 3] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 4] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 5] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 6] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 7] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 8] setEnabled: YES];
            [o_sap_chkbox setEnabled: YES];
            [o_slp_chkbox setEnabled: YES];
            [o_channel_name setEnabled: YES];
        }
        else if( [o_mode isEqualToString: @"RTP"] )
        {
            [o_stream_address setEnabled: YES];
            [o_stream_ttl setEnabled: YES];
            [o_stream_ttl_stp setEnabled: YES];
            [[o_mux_selector itemAtIndex: 1] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 2] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 3] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 4] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 5] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 6] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 7] setEnabled: NO];
            [[o_mux_selector itemAtIndex: 8] setEnabled: YES];
        }
    }
    if( ![[o_mux_selector selectedItem] isEnabled] )
    {
        [o_mux_selector selectItemAtIndex: 0];
    }
    [self outputInfoChanged: nil];
}

- (void)outputInfoChanged:(NSNotification *)o_notification
{
    NSString *o_mode, *o_mux, *o_mux_string, *o_announce;
    NSMutableString *o_mrl_string = [NSMutableString stringWithString:@":sout=#"];

    [o_mrl_string appendString: o_transcode];
    if( [o_display state] == NSOnState )
    {
        [o_mrl_string appendString: @"duplicate{dst=display,dst="];
    }

    o_mode = [[o_method selectedCell] title];
    o_mux = [o_mux_selector titleOfSelectedItem];

    if ( [o_mux isEqualToString: @"AVI"] ) o_mux_string = @"avi";
    else if ( [o_mux isEqualToString: @"Ogg"] ) o_mux_string = @"ogg";
    else if ( [o_mux isEqualToString: @"MPEG PS"] ) o_mux_string = @"ps";
    else if ( [o_mux isEqualToString: @"MPEG 4"] ) o_mux_string = @"mp4";
    else if ( [o_mux isEqualToString: @"MPEG 1"] ) o_mux_string = @"mpeg1";
    else if ( [o_mux isEqualToString: @"Quicktime"] ) o_mux_string = @"mov";
    else if ( [o_mux isEqualToString: @"ASF"] ) o_mux_string = @"asf";
    else if ( [o_mux isEqualToString: @"Raw"] ) o_mux_string = @"raw";
    else o_mux_string = @"ts";

    if( [o_mode isEqualToString: _NS("File")] )
    {
        if( [o_dump_chkbox state] == NSOnState )
        {
            o_mrl_string = [NSMutableString stringWithFormat:
                            @":demux=demuxdump :demuxdump-file=\"%@\"",
                            [o_file_field stringValue]];
            [self setMRL:o_mrl_string];
            return;
        }
        else
        {
                [o_mrl_string appendFormat:
                        @"std{access=file,mux=%@,url=\"%@\"}",
                        o_mux_string, [o_file_field stringValue]];
        }
    }
    else if( [o_mode isEqualToString: _NS("Stream")] )
    {
        o_mode = [o_stream_type titleOfSelectedItem];
        o_announce = @"";
        
        if ( [o_mode isEqualToString: @"HTTP"] )
            o_mode = @"http";
        else if ( [o_mode isEqualToString: @"MMSH"] )
        {
            if ( [o_mux isEqualToString: @"ASF"] ) o_mux_string = @"asfh";
            o_mode = @"mmsh";
        }
        else if ( [o_mode isEqualToString: @"UDP"] )
        {
            o_mode = @"udp";
            if( [o_sap_chkbox state] == NSOnState )
            {
                if ( ![[o_channel_name stringValue] isEqualToString: @""] )
                    o_announce = [NSString stringWithFormat:@",sap,name=%@", 
                        [o_channel_name stringValue]];
                else
                    o_announce = @",sap";
            }
            if( [o_slp_chkbox state] == NSOnState )
            {
               if ( ![[o_channel_name stringValue] isEqualToString: @""] )
                    o_announce = [o_announce stringByAppendingFormat:@
                            "slp,name=%@",[o_channel_name stringValue]];
                else
                    o_announce = [o_announce stringByAppendingString: @",slp"];
            }
        }
        else if ( [o_mode isEqualToString: @"RTP"] )
            o_mode = @"rtp";
            
        [o_mrl_string appendFormat:
                        @"std{access=%@,mux=%@,url=\"%@:%@\"%@}",
                        o_mode, o_mux_string, [o_stream_address stringValue],
                        [o_stream_port stringValue], o_announce];
    }
    if( [o_display state] == NSOnState )
    {
        [o_mrl_string appendString: @"}"];
    }
    [self setMRL:o_mrl_string];
}

- (void)TTLChanged:(NSNotification *)o_notification
{
    intf_thread_t * p_intf = [NSApp getIntf];
    config_PutInt( p_intf, "ttl", [o_stream_ttl intValue] );
}

- (IBAction)outputFileBrowse:(id)sender
{
    NSSavePanel *o_save_panel = [NSSavePanel savePanel];
    NSString *o_mux_string;
    if ( [[o_mux_selector titleOfSelectedItem] isEqualToString: @"MPEG PS"] )
        o_mux_string = @"vob";
    else if ( [[o_mux_selector titleOfSelectedItem] isEqualToString: @"MPEG 1"] )
        o_mux_string = @"mpg";
    else if ( [[o_mux_selector titleOfSelectedItem] isEqualToString: @"AVI"] )
        o_mux_string = @"avi";
    else if ( [[o_mux_selector titleOfSelectedItem] isEqualToString: @"ASF"] )
        o_mux_string = @"asf";
    else if ( [[o_mux_selector titleOfSelectedItem] isEqualToString: @"Ogg"] )
        o_mux_string = @"ogm";
    else if ( [[o_mux_selector titleOfSelectedItem] isEqualToString: @"MPEG 4"] )
        o_mux_string = @"mp4";
    else if ( [[o_mux_selector titleOfSelectedItem] isEqualToString: @"Quicktime"] )
        o_mux_string = @"mov";
    else if ( [[o_mux_selector titleOfSelectedItem] isEqualToString: @"Raw"] )
        o_mux_string = @"raw";
    else
        o_mux_string = @"ts";

    NSString * o_name = [NSString stringWithFormat: @"vlc-output.%@",
                         o_mux_string];

    [o_save_panel setTitle: _NS("Save File")];
    [o_save_panel setPrompt: _NS("Save")];

    if( [o_save_panel runModalForDirectory: nil
            file: o_name] == NSOKButton )
    {
        NSString *o_filename = [o_save_panel filename];
        [o_file_field setStringValue: o_filename];
        [self outputInfoChanged: nil];
    }
}

- (IBAction)streamPortStepperChanged:(id)sender
{
    [o_stream_port setIntValue: [o_stream_port_stp intValue]];
    [self outputInfoChanged: nil];
}

- (IBAction)streamTTLStepperChanged:(id)sender
{
    [o_stream_ttl setIntValue: [o_stream_ttl_stp intValue]];
    [self TTLChanged:nil];
}

- (void)transcodeChanged:(NSNotification *)o_notification
{
    if( [o_transcode_video_chkbox state] == NSOnState )
    {
        [o_transcode_video_selector setEnabled: YES];
        [o_transcode_video_bitrate setEnabled: YES];
    }
    else
    {
        [o_transcode_video_selector setEnabled: NO];
        [o_transcode_video_bitrate setEnabled: NO];
    }
    if( [o_transcode_audio_chkbox state] == NSOnState )
    {
        [o_transcode_audio_selector setEnabled: YES];
        [o_transcode_audio_bitrate setEnabled: YES];
        [o_transcode_audio_channels setEnabled: YES];
    }
    else
    {
        [o_transcode_audio_selector setEnabled: NO];
        [o_transcode_audio_bitrate setEnabled: NO];
        [o_transcode_audio_channels setEnabled: NO];
    }

    [self transcodeInfoChanged:nil];
}

- (void)transcodeInfoChanged:(NSNotification *)o_notification
{
    NSMutableString *o_transcode_string;
    
    if( [o_transcode_video_chkbox state] == NSOnState ||
        [o_transcode_audio_chkbox state] == NSOnState )
    {
        o_transcode_string = [NSMutableString stringWithString:@"transcode{"];
        if ( [o_transcode_video_chkbox state] == NSOnState )
        {
            [o_transcode_string appendFormat: @"vcodec=\"%@\",vb=\"%@\"",
                [o_transcode_video_selector titleOfSelectedItem],
                [o_transcode_video_bitrate stringValue]];
            if ( [o_transcode_audio_chkbox state] == NSOnState )
            {
                [o_transcode_string appendString: @","];
            }
        }
        if ( [o_transcode_audio_chkbox state] == NSOnState )
        {
            [o_transcode_string appendFormat: @"acodec=\"%@\",ab=\"%@\"",
                [o_transcode_audio_selector titleOfSelectedItem],
                [o_transcode_audio_bitrate stringValue]];
        }
        [o_transcode_string appendString:@"}:"];
    }
    else
    {
        o_transcode_string = [NSString stringWithString:@""];
    }
    [self setTranscode: o_transcode_string];
    [self outputInfoChanged:nil];
}

- (IBAction)announceChanged:(id)sender
{
    [o_channel_name setEnabled: [o_sap_chkbox state] || [o_slp_chkbox state]];
    [self outputInfoChanged: nil];
}

@end
