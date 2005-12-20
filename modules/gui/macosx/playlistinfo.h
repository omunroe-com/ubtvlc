/*****************************************************************************
 * playlistinfo.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2002-2004 VideoLAN
 * $Id: playlistinfo.h 7801 2004-05-28 16:01:26Z fkuehne $
 *
 * Authors: Benjamin Pracht <bigben at videolan dot org> 
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
 * VLCPlaylistInfo interface 
 *****************************************************************************/


@interface VLCInfo : NSObject
{
    IBOutlet id o_info_window;
    IBOutlet id o_uri_lbl;
    IBOutlet id o_title_lbl;
    IBOutlet id o_author_lbl;
    IBOutlet id o_uri_txt;
    IBOutlet id o_title_txt;
    IBOutlet id o_author_txt;
    IBOutlet id o_btn_ok;
    IBOutlet id o_btn_cancel;
    IBOutlet id o_btn_delete_group;
    IBOutlet id o_btn_add_group;
    IBOutlet id o_outline_view;
    IBOutlet id o_group_lbl;
    IBOutlet id o_group_cbx;
    IBOutlet id o_group_color;

    int i_item;
    NSMutableArray * o_selected;
}

- (IBAction)togglePlaylistInfoPanel:(id)sender;
- (IBAction)toggleInfoPanel:(id)sender;
- (void)initPanel:(id)sender;
- (IBAction)infoCancel:(id)sender;
- (IBAction)infoOk:(id)sender;
- (IBAction)handleGroup:(id)sender;
- (IBAction)deleteOutlineGroup:(id)sender;
- (IBAction)createOutlineGroup:(id)sender;
- (void)createComboBox;
- (int)getItem;

@end

@interface VLCInfoTreeItem : NSObject
{
    NSString *o_name;
    NSString *o_value;
    int i_object_id;
    int i_item;
    VLCInfoTreeItem *o_parent;
    NSMutableArray *o_children;
}

+ (VLCInfoTreeItem *)rootItem;
- (int)numberOfChildren;
- (VLCInfoTreeItem *)childAtIndex:(int)i_index;
- (NSString *)getName;
- (NSString *)getValue;
- (void)refresh;

@end

