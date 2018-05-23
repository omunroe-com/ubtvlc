/*****************************************************************************
 * fileinfo.cpp : wxWindows plugin for vlc
 *****************************************************************************
 * Copyright (C) 2000-2004 VideoLAN
 * $Id: fileinfo.cpp 7209 2004-03-31 20:52:31Z gbazin $
 *
 * Authors: Sigmund Augdal <sigmunau@idi.ntnu.no>
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
#include <vlc/intf.h>

#include "wxwindows.h"

/*****************************************************************************
 * Event Table.
 *****************************************************************************/

/* IDs for the controls and the menu commands */
enum
{
    Close_Event
};

BEGIN_EVENT_TABLE(FileInfo, wxFrame)
    /* Button events */
    EVT_BUTTON(wxID_OK, FileInfo::OnClose)

    /* Hide the window when the user closes the window */
    EVT_CLOSE(FileInfo::OnClose)

END_EVENT_TABLE()

/*****************************************************************************
 * Constructor.
 *****************************************************************************/
FileInfo::FileInfo( intf_thread_t *_p_intf, wxWindow *p_parent ):
    wxFrame( p_parent, -1, wxU(_("Stream and media info")), wxDefaultPosition,
             wxDefaultSize, wxDEFAULT_FRAME_STYLE )
{
    /* Initializations */
    p_intf = _p_intf;
    SetIcon( *p_intf->p_sys->p_icon );
    SetAutoLayout( TRUE );

    /* Create a panel to put everything in */
    wxPanel *panel = new wxPanel( this, -1 );
    panel->SetAutoLayout( TRUE );

    fileinfo_tree =
        new wxTreeCtrl( panel, -1, wxDefaultPosition, wxSize( 350, 350 ),
                        wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT | wxSUNKEN_BORDER );

    fileinfo_root_label = wxT("");

    /* Place everything in sizers */
    wxBoxSizer *main_sizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *panel_sizer = new wxBoxSizer( wxVERTICAL );
    panel_sizer->Add( fileinfo_tree, 1, wxEXPAND | wxALL, 5 );
    panel_sizer->Layout();
    panel->SetSizerAndFit( panel_sizer );
    main_sizer->Add( panel, 1, wxEXPAND, 0 );
    main_sizer->Layout();
    SetSizerAndFit( main_sizer );

    UpdateFileInfo();
}

void FileInfo::UpdateFileInfo()
{
    input_thread_t *p_input = p_intf->p_sys->p_input;

    if( !p_input || p_input->b_dead || !p_input->psz_name )
    {
        if( fileinfo_root )
        {
            fileinfo_root_label = wxT("");
            fileinfo_tree->DeleteChildren( fileinfo_root );
        }
        return;
    }

    if( !fileinfo_root )
    {
        /* On linux, the first argument of wxTreeCtrl::AddRoot() can be
         * retrieved with the GetItemText() method, but it doesn't work on
         * Windows when the wxTR_HIDE_ROOT style is set. That's why we need to
         * use the fileinfo_root_label variable... */
        fileinfo_root = fileinfo_tree->AddRoot( wxL2U(p_input->psz_name) );
        fileinfo_root_label = wxL2U(p_input->psz_name);
    }
    else if( fileinfo_root_label == wxL2U(p_input->psz_name) )
    {
        return;
    }

    /* We rebuild the tree from scratch */
    fileinfo_tree->DeleteChildren( fileinfo_root );
    fileinfo_root_label = wxL2U(p_input->psz_name);

    vlc_mutex_lock( &p_input->p_item->lock );
    for( int i = 0; i < p_input->p_item->i_categories; i++ )
    {
        info_category_t *p_cat = p_input->p_item->pp_categories[i];

        wxTreeItemId cat = fileinfo_tree->AppendItem( fileinfo_root,
                                                      wxL2U(p_cat->psz_name) );
        for( int j = 0; j < p_cat->i_infos; j++ )
        {
            info_t *p_info = p_cat->pp_infos[j];

            fileinfo_tree->AppendItem( cat, (wxString)wxL2U(p_info->psz_name) +
                                       wxT(": ") + wxL2U(p_info->psz_value) );
        }
        fileinfo_tree->Expand( cat );
    }
    vlc_mutex_unlock( &p_input->p_item->lock );

    return;
}

FileInfo::~FileInfo()
{
}

void FileInfo::OnClose( wxCommandEvent& event )
{
    Hide();
}
