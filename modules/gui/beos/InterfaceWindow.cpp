/*****************************************************************************
 * InterfaceWindow.cpp: beos interface
 *****************************************************************************
 * Copyright (C) 1999, 2000, 2001 VideoLAN
 * $Id: InterfaceWindow.cpp 7350 2004-04-15 10:46:11Z stippi $
 *
 * Authors: Jean-Marc Dressler <polux@via.ecp.fr>
 *          Samuel Hocevar <sam@zoy.org>
 *          Tony Castley <tony@castley.net>
 *          Richard Shepherd <richard@rshepherd.demon.co.uk>
 *          Stephan Aßmus <superstippi@gmx.de>
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

/* System headers */
#include <kernel/OS.h>
#include <InterfaceKit.h>
#include <AppKit.h>
#include <StorageKit.h>
#include <SupportKit.h>
#include <malloc.h>
#include <scsi.h>
#include <scsiprobe_driver.h>
#include <fs_info.h>
#include <string.h>

/* VLC headers */
#include <vlc/vlc.h>
#include <vlc/aout.h>
#include <vlc/intf.h>

/* BeOS interface headers */
#include "VlcWrapper.h"
#include "MsgVals.h"
#include "MediaControlView.h"
#include "PlayListWindow.h"
#include "PreferencesWindow.h"
#include "MessagesWindow.h"
#include "InterfaceWindow.h"

#define INTERFACE_UPDATE_TIMEOUT 80000 // 2 frames if at 25 fps
#define INTERFACE_LOCKING_TIMEOUT 5000

// make_sure_frame_is_on_screen
bool
make_sure_frame_is_on_screen( BRect& frame )
{
	BScreen screen( B_MAIN_SCREEN_ID );
	if (frame.IsValid() && screen.IsValid()) {
		if (!screen.Frame().Contains(frame)) {
			// make sure frame fits in the screen
			if (frame.Width() > screen.Frame().Width())
				frame.right -= frame.Width() - screen.Frame().Width() + 10.0;
			if (frame.Height() > screen.Frame().Height())
				frame.bottom -= frame.Height() - screen.Frame().Height() + 30.0;
			// frame is now at the most the size of the screen
			if (frame.right > screen.Frame().right)
				frame.OffsetBy(-(frame.right - screen.Frame().right), 0.0);
			if (frame.bottom > screen.Frame().bottom)
				frame.OffsetBy(0.0, -(frame.bottom - screen.Frame().bottom));
			if (frame.left < screen.Frame().left)
				frame.OffsetBy((screen.Frame().left - frame.left), 0.0);
			if (frame.top < screen.Frame().top)
				frame.OffsetBy(0.0, (screen.Frame().top - frame.top));
		}
		return true;
	}
	return false;
}

// make_sure_frame_is_within_limits
void
make_sure_frame_is_within_limits( BRect& frame, float minWidth, float minHeight,
                                  float maxWidth, float maxHeight )
{
    if ( frame.Width() < minWidth )
        frame.right = frame.left + minWidth;
    if ( frame.Height() < minHeight )
        frame.bottom = frame.top + minHeight;
    if ( frame.Width() > maxWidth )
        frame.right = frame.left + maxWidth;
    if ( frame.Height() > maxHeight )
        frame.bottom = frame.top + maxHeight;
}

// get_volume_info
bool
get_volume_info( BVolume& volume, BString& volumeName, bool& isCDROM, BString& deviceName )
{
	bool success = false;
	isCDROM = false;
	deviceName = "";
	volumeName = "";
	char name[B_FILE_NAME_LENGTH];
	if ( volume.GetName( name ) >= B_OK )	// disk is currently mounted
	{
		volumeName = name;
		dev_t dev = volume.Device();
		fs_info info;
		if ( fs_stat_dev( dev, &info ) == B_OK )
		{
			success = true;
			deviceName = info.device_name;
			if ( volume.IsReadOnly() )
			{
				int i_dev = open( info.device_name, O_RDONLY );
				if ( i_dev >= 0 )
				{
					device_geometry g;
					if ( ioctl( i_dev, B_GET_GEOMETRY, &g, sizeof( g ) ) >= 0 )
						isCDROM = ( g.device_type == B_CD );
					close( i_dev );
				}
			}
		}
 	}
 	return success;
}

// collect_folder_contents
void
collect_folder_contents( BDirectory& dir, BList& list, bool& deep, bool& asked, BEntry& entry )
{
	while ( dir.GetNextEntry( &entry, true ) == B_OK )
	{
		if ( !entry.IsDirectory() )
		{
			BPath path;
			// since the directory will give us the entries in reverse order,
			// we put them each at the same index, effectively reversing the
			// items while adding them
			if ( entry.GetPath( &path ) == B_OK )
			{
				BString* string = new BString( path.Path() );
				if ( !list.AddItem( string, 0 ) )
					delete string;	// at least don't leak
			}
		}
		else
		{
			if ( !asked )
			{
				// ask user if we should parse sub-folders as well
				BAlert* alert = new BAlert( "sub-folders?",
											_("Open files from all sub-folders as well?"),
											_("Cancel"), _("Open"), NULL, B_WIDTH_AS_USUAL,
											B_IDEA_ALERT );
				int32 buttonIndex = alert->Go();
				deep = buttonIndex == 1;
				asked = true;
				// never delete BAlerts!!
			}
			if ( deep )
			{
				BDirectory subDir( &entry );
				if ( subDir.InitCheck() == B_OK )
					collect_folder_contents( subDir, list,
											 deep, asked, entry );
			}
		}
	}
}


/*****************************************************************************
 * InterfaceWindow
 *****************************************************************************/

InterfaceWindow::InterfaceWindow( BRect frame, const char* name,
                                  intf_thread_t* p_interface )
    : BWindow( frame, name, B_TITLED_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL,
               B_NOT_ZOOMABLE | B_WILL_ACCEPT_FIRST_CLICK | B_ASYNCHRONOUS_CONTROLS ),
      p_intf( p_interface ),
      fFilePanel( NULL ),
      fLastUpdateTime( system_time() ),
	  fSettings( new BMessage( 'sett' ) ),
	  p_wrapper( p_intf->p_sys->p_wrapper )
{
    fPlaylistIsEmpty = !( p_wrapper->PlaylistSize() > 0 );
    
    BScreen screen;
    BRect screen_rect = screen.Frame();
    BRect window_rect;
    window_rect.Set( ( screen_rect.right - PREFS_WINDOW_WIDTH ) / 2,
                     ( screen_rect.bottom - PREFS_WINDOW_HEIGHT ) / 2,
                     ( screen_rect.right + PREFS_WINDOW_WIDTH ) / 2,
                     ( screen_rect.bottom + PREFS_WINDOW_HEIGHT ) / 2 );
    fPreferencesWindow = new PreferencesWindow( p_intf, window_rect, _("Preferences") );
    window_rect.Set( screen_rect.right - 500,
                     screen_rect.top + 50,
                     screen_rect.right - 150,
                     screen_rect.top + 250 );
    fPlaylistWindow = new PlayListWindow( window_rect, _("Playlist"), this, p_intf );
    window_rect.Set( screen_rect.right - 550,
                     screen_rect.top + 300,
                     screen_rect.right - 150,
                     screen_rect.top + 500 );
    fMessagesWindow = new MessagesWindow( p_intf, window_rect, _("Messages") );

    // the media control view
    p_mediaControl = new MediaControlView( BRect( 0.0, 0.0, 250.0, 50.0 ),
                                           p_intf );
    p_mediaControl->SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );

    float width, height;
    p_mediaControl->GetPreferredSize( &width, &height );

    // set up the main menu
    fMenuBar = new BMenuBar( BRect(0.0, 0.0, width, 15.0), "main menu",
                             B_FOLLOW_NONE, B_ITEMS_IN_ROW, false );

    // make menu bar resize to correct height
    float menuWidth, menuHeight;
    fMenuBar->GetPreferredSize( &menuWidth, &menuHeight );
    fMenuBar->ResizeTo( width, menuHeight );    // don't change! it's a workarround!
    // take care of proper size for ourself
    height += fMenuBar->Bounds().Height();
    ResizeTo( width, height );

    p_mediaControl->MoveTo( fMenuBar->Bounds().LeftBottom() + BPoint(0.0, 1.0) );
    AddChild( fMenuBar );

    // Add the file Menu
    BMenu* fileMenu = new BMenu( _("File") );
    fMenuBar->AddItem( fileMenu );
    fileMenu->AddItem( new BMenuItem( _AddEllipsis(_("Open File")),
                                      new BMessage( OPEN_FILE ), 'O') );
    
    fileMenu->AddItem( new CDMenu( _("Open Disc") ) );

    fileMenu->AddItem( new BMenuItem( _AddEllipsis(_("Open Subtitles")),
                                      new BMessage( LOAD_SUBFILE ) ) );

    fileMenu->AddSeparatorItem();
    BMenuItem* item = new BMenuItem( _AddEllipsis(_("About")),
                                     new BMessage( B_ABOUT_REQUESTED ), 'A');
    item->SetTarget( be_app );
    fileMenu->AddItem( item );
    fileMenu->AddItem( new BMenuItem( _("Quit"), new BMessage( B_QUIT_REQUESTED ), 'Q') );

    fLanguageMenu = new LanguageMenu( _("Language"), AUDIO_ES, p_wrapper);
    fSubtitlesMenu = new LanguageMenu( _("Subtitles"), SPU_ES, p_wrapper);

    /* Add the Audio menu */
    fAudioMenu = new BMenu( _("Audio") );
    fMenuBar->AddItem ( fAudioMenu );
    fAudioMenu->AddItem( fLanguageMenu );
    fAudioMenu->AddItem( fSubtitlesMenu );

    fPrevTitleMI = new BMenuItem( _("Prev Title"), new BMessage( PREV_TITLE ) );
    fNextTitleMI = new BMenuItem( _("Next Title"), new BMessage( NEXT_TITLE ) );
    fPrevChapterMI = new BMenuItem( _("Previous chapter"), new BMessage( PREV_CHAPTER ) );
    fNextChapterMI = new BMenuItem( _("Next chapter"), new BMessage( NEXT_CHAPTER ) );
    fGotoMenuMI = new BMenuItem( _("Goto Menu"), new BMessage( NAVIGATE_MENU ) );

    /* Add the Navigation menu */
    fNavigationMenu = new BMenu( _("Navigation") );
    fMenuBar->AddItem( fNavigationMenu );
    fNavigationMenu->AddItem( fGotoMenuMI );
    fNavigationMenu->AddSeparatorItem();
    fNavigationMenu->AddItem( fPrevTitleMI );
    fNavigationMenu->AddItem( fNextTitleMI );
    fNavigationMenu->AddItem( fTitleMenu = new TitleMenu( _("Go to Title"), p_intf ) );
    fNavigationMenu->AddSeparatorItem();
    fNavigationMenu->AddItem( fPrevChapterMI );
    fNavigationMenu->AddItem( fNextChapterMI );
    fNavigationMenu->AddItem( fChapterMenu = new ChapterMenu( _("Go to Chapter"), p_intf ) );

    /* Add the Speed menu */
    fSpeedMenu = new BMenu( _("Speed") );
    fSpeedMenu->SetRadioMode( true );
    fSpeedMenu->AddItem(
        fHeighthMI = new BMenuItem( "1/8x", new BMessage( HEIGHTH_PLAY ) ) );
    fSpeedMenu->AddItem(
        fQuarterMI = new BMenuItem( "1/4x", new BMessage( QUARTER_PLAY ) ) );
    fSpeedMenu->AddItem(
        fHalfMI = new BMenuItem( "1/2x", new BMessage( HALF_PLAY ) ) );
    fSpeedMenu->AddItem(
        fNormalMI = new BMenuItem( "1x", new BMessage( NORMAL_PLAY ) ) );
    fSpeedMenu->AddItem(
        fTwiceMI = new BMenuItem( "2x", new BMessage( TWICE_PLAY ) ) );
    fSpeedMenu->AddItem(
        fFourMI = new BMenuItem( "4x", new BMessage( FOUR_PLAY ) ) );
    fSpeedMenu->AddItem(
        fHeightMI = new BMenuItem( "8x", new BMessage( HEIGHT_PLAY ) ) );
    fMenuBar->AddItem( fSpeedMenu );

    /* Add the Show menu */
    fShowMenu = new BMenu( _("Window") );
    fShowMenu->AddItem( new BMenuItem( _AddEllipsis(_("Playlist")),
                                       new BMessage( OPEN_PLAYLIST ), 'P') );
    fShowMenu->AddItem( new BMenuItem( _AddEllipsis(_("Messages")),
                                       new BMessage( OPEN_MESSAGES ), 'M' ) );
    fShowMenu->AddItem( new BMenuItem( _AddEllipsis(_("Preferences")),
                                       new BMessage( OPEN_PREFERENCES ), 'S' ) );
    fMenuBar->AddItem( fShowMenu );

	// add the media control view after the menubar is complete
	// because it will set the window size limits in AttachedToWindow()
	// and the menubar needs to report the correct PreferredSize()
    AddChild( p_mediaControl );

    /* Prepare fow showing */
    _SetMenusEnabled( false );
    p_mediaControl->SetEnabled( false );

    _RestoreSettings();

    Show();
}

InterfaceWindow::~InterfaceWindow()
{
    if( fPlaylistWindow )
        fPlaylistWindow->ReallyQuit();
    fPlaylistWindow = NULL;
    if( fMessagesWindow )
        fMessagesWindow->ReallyQuit();
    fMessagesWindow = NULL;
    if( fPreferencesWindow )
        fPreferencesWindow->ReallyQuit();
    fPreferencesWindow = NULL;
	delete fFilePanel;
	delete fSettings;
}

/*****************************************************************************
 * InterfaceWindow::FrameResized
 *****************************************************************************/
void
InterfaceWindow::FrameResized(float width, float height)
{
    BRect r(Bounds());
    fMenuBar->MoveTo(r.LeftTop());
    fMenuBar->ResizeTo(r.Width(), fMenuBar->Bounds().Height());
    r.top += fMenuBar->Bounds().Height() + 1.0;
    p_mediaControl->MoveTo(r.LeftTop());
    p_mediaControl->ResizeTo(r.Width(), r.Height());
}

/*****************************************************************************
 * InterfaceWindow::MessageReceived
 *****************************************************************************/
void InterfaceWindow::MessageReceived( BMessage * p_message )
{
    int playback_status;      // remember playback state
    playback_status = p_wrapper->InputStatus();

    switch( p_message->what )
    {
        case B_ABOUT_REQUESTED:
        {
            BAlert* alert = new BAlert( "VLC " PACKAGE_VERSION,
                                        "VLC " PACKAGE_VERSION " for BeOS"
                                        "\n\n<www.videolan.org>", _("OK"));
            alert->Go();
            break;
        }
        case TOGGLE_ON_TOP:
            break;

        case OPEN_FILE:
        	_ShowFilePanel( B_REFS_RECEIVED, _("VLC media player: Open Media Files") );
            break;

        case LOAD_SUBFILE:
        	_ShowFilePanel( SUBFILE_RECEIVED, _("VLC media player: Open Subtitle File") );
            break;

        case OPEN_PLAYLIST:
            if (fPlaylistWindow->Lock())
            {
                if (fPlaylistWindow->IsHidden())
                    fPlaylistWindow->Show();
                else
                    fPlaylistWindow->Activate();
                fPlaylistWindow->Unlock();
            }
            break;
        case OPEN_DVD:
            {
                const char *psz_device;
                BString type( "dvd" );
                if( p_message->FindString( "device", &psz_device ) == B_OK )
                {
                    BString device( psz_device );
                    p_wrapper->OpenDisc( type, device, 0, 0 );
                }
                _UpdatePlaylist();
            }
            break;

        case SUBFILE_RECEIVED:
        {
            entry_ref ref;
            if( p_message->FindRef( "refs", 0, &ref ) == B_OK )
            {
                BPath path( &ref );
                if ( path.InitCheck() == B_OK )
                    p_wrapper->LoadSubFile( path.Path() );
            }
            break;
        }

        case STOP_PLAYBACK:
            // this currently stops playback not nicely
            if (playback_status > UNDEF_S)
            {
                p_wrapper->PlaylistStop();
                p_mediaControl->SetStatus(UNDEF_S, DEFAULT_RATE);
            }
            break;
    
        case START_PLAYBACK:
            /*  starts playing in normal mode */
    
        case PAUSE_PLAYBACK:
            /* toggle between pause and play */
            if (playback_status > UNDEF_S)
            {
                /* pause if currently playing */
                if ( playback_status == PLAYING_S )
                {
                    p_wrapper->PlaylistPause();
                }
                else
                {
                    p_wrapper->PlaylistPlay();
                }
            }
            else
            {
                /* Play a new file */
                p_wrapper->PlaylistPlay();
            }    
            break;
    
        case HEIGHTH_PLAY:
            p_wrapper->InputSetRate( DEFAULT_RATE * 8 );
            break;

        case QUARTER_PLAY:
            p_wrapper->InputSetRate( DEFAULT_RATE * 4 );
            break;

        case HALF_PLAY:
            p_wrapper->InputSetRate( DEFAULT_RATE * 2 );
            break;

        case NORMAL_PLAY:
            p_wrapper->InputSetRate( DEFAULT_RATE );
            break;

        case TWICE_PLAY:
            p_wrapper->InputSetRate( DEFAULT_RATE / 2 );
            break;

        case FOUR_PLAY:
            p_wrapper->InputSetRate( DEFAULT_RATE / 4 );
            break;

        case HEIGHT_PLAY:
            p_wrapper->InputSetRate( DEFAULT_RATE / 8 );
            break;

        case SEEK_PLAYBACK:
            /* handled by semaphores */
            break;
        // volume related messages
        case VOLUME_CHG:
            /* adjust the volume */
            if (playback_status > UNDEF_S)
            {
                p_wrapper->SetVolume( p_mediaControl->GetVolume() );
                p_mediaControl->SetMuted( p_wrapper->IsMuted() );
            }
            break;
    
        case VOLUME_MUTE:
            // toggle muting
            if( p_wrapper->IsMuted() )
                p_wrapper->VolumeRestore();
            else
                p_wrapper->VolumeMute();
            p_mediaControl->SetMuted( p_wrapper->IsMuted() );
            break;
    
        case SELECT_CHANNEL:
            if ( playback_status > UNDEF_S )
            {
                int32 channel;
                if ( p_message->FindInt32( "channel", &channel ) == B_OK )
                {
                    p_wrapper->ToggleLanguage( channel );
                }
            }
            break;
    
        case SELECT_SUBTITLE:
            if ( playback_status > UNDEF_S )
            {
                int32 subtitle;
                if ( p_message->FindInt32( "subtitle", &subtitle ) == B_OK )
                     p_wrapper->ToggleSubtitle( subtitle );
            }
            break;
    
        // specific navigation messages
        case PREV_TITLE:
        {
            p_wrapper->PrevTitle();
            break;
        }
        case NEXT_TITLE:
        {
            p_wrapper->NextTitle();
            break;
        }
        case NAVIGATE_MENU:
        	p_wrapper->ToggleTitle( 0 );
        	break;
        case TOGGLE_TITLE:
            if ( playback_status > UNDEF_S )
            {
                int32 index;
                if( p_message->FindInt32( "index", &index ) == B_OK )
                    p_wrapper->ToggleTitle( index );
            }
            break;
        case PREV_CHAPTER:
        {
            p_wrapper->PrevChapter();
            break;
        }
        case NEXT_CHAPTER:
        {
            p_wrapper->NextChapter();
            break;
        }
        case TOGGLE_CHAPTER:
            if ( playback_status > UNDEF_S )
            {
                int32 index;
                if( p_message->FindInt32( "index", &index ) == B_OK )
                    p_wrapper->ToggleChapter( index );
            }
            break;
        case PREV_FILE:
            p_wrapper->PlaylistPrev();
            break;
        case NEXT_FILE:
            p_wrapper->PlaylistNext();
            break;
        // general next/prev functionality (skips to whatever makes most sense)
        case NAVIGATE_PREV:
            p_wrapper->NavigatePrev();
            break;
        case NAVIGATE_NEXT:
            p_wrapper->NavigateNext();
            break;
        // drag'n'drop and system messages
        case MSG_SOUNDPLAY:
        	// convert soundplay drag'n'drop message (containing paths)
        	// to normal message (containing refs)
        	{
	        	const char* path;
	        	for ( int32 i = 0; p_message->FindString( "path", i, &path ) == B_OK; i++ )
	        	{
	        		entry_ref ref;
	        		if ( get_ref_for_path( path, &ref ) == B_OK )
		        		p_message->AddRef( "refs", &ref );
	        	}
        	}
        	// fall through
        case B_REFS_RECEIVED:
        case B_SIMPLE_DATA:
            {
                /* file(s) opened by the File menu -> append to the playlist;
                 * file(s) opened by drag & drop -> replace playlist;
                 * file(s) opened by 'shift' + drag & drop -> append */
                bool replace = false;
                bool reverse = false;
                if ( p_message->WasDropped() )
                {
                    replace = !( modifiers() & B_SHIFT_KEY );
                    reverse = true;
                }
                    
                // build list of files to be played from message contents
                entry_ref ref;
                BList files;
                
                // if we should parse sub-folders as well
           		bool askedAlready = false;
           		bool parseSubFolders = askedAlready;
           		// traverse refs in reverse order
           		int32 count;
           		type_code dummy;
           		if ( p_message->GetInfo( "refs", &dummy, &count ) == B_OK && count > 0 )
           		{
           			int32 i = reverse ? count - 1 : 0;
           			int32 increment = reverse ? -1 : 1;
	                for ( ; p_message->FindRef( "refs", i, &ref ) == B_OK; i += increment )
	                {
	                    BPath path( &ref );
	                    if ( path.InitCheck() == B_OK )
	                    {
	                        bool add = true;
	                        // has the user dropped a folder?
	                        BDirectory dir( &ref );
	                        if ( dir.InitCheck() == B_OK)
	                        {
		                        // has the user dropped a dvd disk icon?
								if ( dir.IsRootDirectory() )
								{
									BVolumeRoster volRoster;
									BVolume vol;
									BDirectory volumeRoot;
									status_t status = volRoster.GetNextVolume( &vol );
									while ( status == B_NO_ERROR )
									{
										if ( vol.GetRootDirectory( &volumeRoot ) == B_OK
											 && dir == volumeRoot )
										{
											BString volumeName;
											BString deviceName;
											bool isCDROM;
											if ( get_volume_info( vol, volumeName, isCDROM, deviceName )
												 && isCDROM )
											{
												BMessage msg( OPEN_DVD );
												msg.AddString( "device", deviceName.String() );
												PostMessage( &msg );
												add = false;
											}
									 		break;
										}
										else
										{
									 		vol.Unset();
											status = volRoster.GetNextVolume( &vol );
										}
									}
								}
	                        	if ( add )
	                        	{
	                        		add = false;
	                        		dir.Rewind();	// defensive programming
	                        		BEntry entry;
									collect_folder_contents( dir, files,
															 parseSubFolders,
															 askedAlready,
															 entry );
	                        	}
	                        }
	                        if ( add )
	                        {
	                        	BString* string = new BString( path.Path() );
	                        	if ( !files.AddItem( string, 0 ) )
	                        		delete string;	// at least don't leak
	                        }
	                    }
	                }
	                // give the list to VLC
	                // BString objects allocated here will be deleted there
	                int32 index;
	                if ( p_message->FindInt32("drop index", &index) != B_OK )
	                	index = -1;
	                p_wrapper->OpenFiles( &files, replace, index );
	                _UpdatePlaylist();
           		}
            }
            break;

        case OPEN_PREFERENCES:
        {
            if( fPreferencesWindow->Lock() )
            {
                if (fPreferencesWindow->IsHidden())
                    fPreferencesWindow->Show();
                else
                    fPreferencesWindow->Activate();
                fPreferencesWindow->Unlock();
            }
            break;
        }

        case OPEN_MESSAGES:
        {
            if( fMessagesWindow->Lock() )
            {
                if (fMessagesWindow->IsHidden())
                    fMessagesWindow->Show();
                else
                    fMessagesWindow->Activate();
                fMessagesWindow->Unlock();
            }
            break;
        }
        case MSG_UPDATE:
        	UpdateInterface();
        	break;
        default:
            BWindow::MessageReceived( p_message );
            break;
    }

}

/*****************************************************************************
 * InterfaceWindow::QuitRequested
 *****************************************************************************/
bool InterfaceWindow::QuitRequested()
{
    p_wrapper->PlaylistStop();
    p_mediaControl->SetStatus(UNDEF_S, DEFAULT_RATE);

 	_StoreSettings();
   
    p_intf->b_die = 1;

    return( true );
}

/*****************************************************************************
 * InterfaceWindow::UpdateInterface
 *****************************************************************************/
void InterfaceWindow::UpdateInterface()
{
    if( p_wrapper->HasInput() )
    {
        if ( acquire_sem( p_mediaControl->fScrubSem ) == B_OK )
        {
            p_wrapper->SetTimeAsFloat( p_mediaControl->GetSeekTo() );
        }
        else if ( LockWithTimeout( INTERFACE_LOCKING_TIMEOUT ) == B_OK )
        {
            p_mediaControl->SetEnabled( true );
            bool hasTitles = p_wrapper->HasTitles();
            bool hasChapters = p_wrapper->HasChapters();
            p_mediaControl->SetStatus( p_wrapper->InputStatus(), 
                                       p_wrapper->InputRate() );
            p_mediaControl->SetProgress( p_wrapper->GetTimeAsFloat() );
            _SetMenusEnabled( true, hasChapters, hasTitles );

            _UpdateSpeedMenu( p_wrapper->InputRate() );

            // enable/disable skip buttons
            bool canSkipPrev;
            bool canSkipNext;
            p_wrapper->GetNavCapabilities( &canSkipPrev, &canSkipNext );
            p_mediaControl->SetSkippable( canSkipPrev, canSkipNext );

            if ( p_wrapper->HasInput() )
            {
                p_mediaControl->SetAudioEnabled( true );
                p_mediaControl->SetMuted( p_wrapper->IsMuted() );
            } else
                p_mediaControl->SetAudioEnabled( false );

            Unlock();
        }
        // update playlist as well
        if ( fPlaylistWindow->LockWithTimeout( INTERFACE_LOCKING_TIMEOUT ) == B_OK )
        {
            fPlaylistWindow->UpdatePlaylist();
            fPlaylistWindow->Unlock();
        }
    }
    else
    {
		if ( LockWithTimeout(INTERFACE_LOCKING_TIMEOUT) == B_OK )
		{
	        _SetMenusEnabled( false );
	        if( !( p_wrapper->PlaylistSize() > 0 ) )
	            p_mediaControl->SetEnabled( false );
	        else
	        {
	            p_mediaControl->SetProgress( 0 );
	            // enable/disable skip buttons
	            bool canSkipPrev;
	            bool canSkipNext;
	            p_wrapper->GetNavCapabilities( &canSkipPrev, &canSkipNext );
	            p_mediaControl->SetSkippable( canSkipPrev, canSkipNext );
			}
            Unlock();
        }
    }

    fLastUpdateTime = system_time();
}

/*****************************************************************************
 * InterfaceWindow::IsStopped
 *****************************************************************************/
bool
InterfaceWindow::IsStopped() const
{
    return (system_time() - fLastUpdateTime > INTERFACE_UPDATE_TIMEOUT);
}

/*****************************************************************************
 * InterfaceWindow::_UpdatePlaylist
 *****************************************************************************/
void
InterfaceWindow::_UpdatePlaylist()
{
    if ( fPlaylistWindow->Lock() )
    {
        fPlaylistWindow->UpdatePlaylist( true );
        fPlaylistWindow->Unlock();
        p_mediaControl->SetEnabled( p_wrapper->PlaylistSize() );
    }
}

/*****************************************************************************
 * InterfaceWindow::_SetMenusEnabled
 *****************************************************************************/
void
InterfaceWindow::_SetMenusEnabled(bool hasFile, bool hasChapters, bool hasTitles)
{
    if (!hasFile)
    {
        hasChapters = false;
        hasTitles = false;
    }
    if ( LockWithTimeout( INTERFACE_LOCKING_TIMEOUT ) == B_OK)
    {
        if ( fNextChapterMI->IsEnabled() != hasChapters )
             fNextChapterMI->SetEnabled( hasChapters );
        if ( fPrevChapterMI->IsEnabled() != hasChapters )
             fPrevChapterMI->SetEnabled( hasChapters );
        if ( fChapterMenu->IsEnabled() != hasChapters )
             fChapterMenu->SetEnabled( hasChapters );
        if ( fNextTitleMI->IsEnabled() != hasTitles )
             fNextTitleMI->SetEnabled( hasTitles );
        if ( fPrevTitleMI->IsEnabled() != hasTitles )
             fPrevTitleMI->SetEnabled( hasTitles );
        if ( fTitleMenu->IsEnabled() != hasTitles )
             fTitleMenu->SetEnabled( hasTitles );
        if ( fAudioMenu->IsEnabled() != hasFile )
             fAudioMenu->SetEnabled( hasFile );
        if ( fNavigationMenu->IsEnabled() != hasFile )
             fNavigationMenu->SetEnabled( hasFile );
        if ( fLanguageMenu->IsEnabled() != hasFile )
             fLanguageMenu->SetEnabled( hasFile );
        if ( fSubtitlesMenu->IsEnabled() != hasFile )
             fSubtitlesMenu->SetEnabled( hasFile );
        if ( fSpeedMenu->IsEnabled() != hasFile )
             fSpeedMenu->SetEnabled( hasFile );
        // "goto menu" menu item
        bool hasMenu = p_wrapper->IsUsingMenus();
        if ( fGotoMenuMI->IsEnabled() != hasMenu )
             fGotoMenuMI->SetEnabled( hasMenu );
        Unlock();
    }
}

/*****************************************************************************
 * InterfaceWindow::_UpdateSpeedMenu
 *****************************************************************************/
void
InterfaceWindow::_UpdateSpeedMenu( int rate )
{
    BMenuItem * toMark = NULL;
    
    switch( rate )
    {
        case ( DEFAULT_RATE * 8 ):
            toMark = fHeighthMI;
            break;
            
        case ( DEFAULT_RATE * 4 ):
            toMark = fQuarterMI;
            break;
            
        case ( DEFAULT_RATE * 2 ):
            toMark = fHalfMI;
            break;
            
        case ( DEFAULT_RATE ):
            toMark = fNormalMI;
            break;
            
        case ( DEFAULT_RATE / 2 ):
            toMark = fTwiceMI;
            break;
            
        case ( DEFAULT_RATE / 4 ):
            toMark = fFourMI;
            break;
            
        case ( DEFAULT_RATE / 8 ):
            toMark = fHeightMI;
            break;
    }

    if ( !toMark->IsMarked() )
        toMark->SetMarked( true );
}

/*****************************************************************************
 * InterfaceWindow::_ShowFilePanel
 *****************************************************************************/
void
InterfaceWindow::_ShowFilePanel( uint32 command, const char* windowTitle )
{
	if( !fFilePanel )
	{
		fFilePanel = new BFilePanel( B_OPEN_PANEL, NULL, NULL,
									 B_FILE_NODE | B_DIRECTORY_NODE );
		fFilePanel->SetTarget( this );
	}
	fFilePanel->Window()->SetTitle( windowTitle );
	BMessage message( command );
	fFilePanel->SetMessage( &message );
	if ( !fFilePanel->IsShowing() )
	{
		fFilePanel->Refresh();
		fFilePanel->Show();
	}
}

// set_window_pos
void
set_window_pos( BWindow* window, BRect frame )
{
	// sanity checks: make sure window is not too big/small
	// and that it's not off-screen
	float minWidth, maxWidth, minHeight, maxHeight;
	window->GetSizeLimits( &minWidth, &maxWidth, &minHeight, &maxHeight );

	make_sure_frame_is_within_limits( frame,
									  minWidth, minHeight, maxWidth, maxHeight );
	if ( make_sure_frame_is_on_screen( frame ) )
	{
		window->MoveTo( frame.LeftTop() );
		window->ResizeTo( frame.Width(), frame.Height() );
	}
}

// set_window_pos
void
launch_window( BWindow* window, bool showing )
{
	if ( window->Lock() )
	{
		if ( showing )
		{
			if ( window->IsHidden() )
				window->Show();
		}
		else
		{
			if ( !window->IsHidden() )
				window->Hide();
		}
		window->Unlock();
	}
}

/*****************************************************************************
 * InterfaceWindow::_RestoreSettings
 *****************************************************************************/
void
InterfaceWindow::_RestoreSettings()
{
	if ( load_settings( fSettings, "interface_settings", "VideoLAN Client" ) == B_OK )
	{
		BRect frame;
		if ( fSettings->FindRect( "main frame", &frame ) == B_OK )
			set_window_pos( this, frame );
		if (fSettings->FindRect( "playlist frame", &frame ) == B_OK )
			set_window_pos( fPlaylistWindow, frame );
		if (fSettings->FindRect( "messages frame", &frame ) == B_OK )
			set_window_pos( fMessagesWindow, frame );
		if (fSettings->FindRect( "settings frame", &frame ) == B_OK )
		{
		    /* FIXME: Preferences resizing doesn't work correctly yet */
		    frame.right = frame.left + fPreferencesWindow->Frame().Width();
		    frame.bottom = frame.top + fPreferencesWindow->Frame().Height();
			set_window_pos( fPreferencesWindow, frame );
		}
		
		bool showing;
		if ( fSettings->FindBool( "playlist showing", &showing ) == B_OK )
			launch_window( fPlaylistWindow, showing );
		if ( fSettings->FindBool( "messages showing", &showing ) == B_OK )
			launch_window( fMessagesWindow, showing );
		if ( fSettings->FindBool( "settings showing", &showing ) == B_OK )
			launch_window( fPreferencesWindow, showing );

		uint32 displayMode;
		if ( fSettings->FindInt32( "playlist display mode", (int32*)&displayMode ) == B_OK )
			fPlaylistWindow->SetDisplayMode( displayMode );
	}
}

/*****************************************************************************
 * InterfaceWindow::_StoreSettings
 *****************************************************************************/
void
InterfaceWindow::_StoreSettings()
{
    /* Save the volume */
    config_PutInt( p_intf, "volume", p_mediaControl->GetVolume() );
    config_SaveConfigFile( p_intf, "main" );

    /* Save the windows positions */
	if ( fSettings->ReplaceRect( "main frame", Frame() ) != B_OK )
		fSettings->AddRect( "main frame", Frame() );
	if ( fPlaylistWindow->Lock() )
	{
		if (fSettings->ReplaceRect( "playlist frame", fPlaylistWindow->Frame() ) != B_OK)
			fSettings->AddRect( "playlist frame", fPlaylistWindow->Frame() );
		if (fSettings->ReplaceBool( "playlist showing", !fPlaylistWindow->IsHidden() ) != B_OK)
			fSettings->AddBool( "playlist showing", !fPlaylistWindow->IsHidden() );
		fPlaylistWindow->Unlock();
	}
	if ( fMessagesWindow->Lock() )
	{
		if (fSettings->ReplaceRect( "messages frame", fMessagesWindow->Frame() ) != B_OK)
			fSettings->AddRect( "messages frame", fMessagesWindow->Frame() );
		if (fSettings->ReplaceBool( "messages showing", !fMessagesWindow->IsHidden() ) != B_OK)
			fSettings->AddBool( "messages showing", !fMessagesWindow->IsHidden() );
		fMessagesWindow->Unlock();
	}
	if ( fPreferencesWindow->Lock() )
	{
		if (fSettings->ReplaceRect( "settings frame", fPreferencesWindow->Frame() ) != B_OK)
			fSettings->AddRect( "settings frame", fPreferencesWindow->Frame() );
		if (fSettings->ReplaceBool( "settings showing", !fPreferencesWindow->IsHidden() ) != B_OK)
			fSettings->AddBool( "settings showing", !fPreferencesWindow->IsHidden() );
		fPreferencesWindow->Unlock();
	}
	uint32 displayMode = fPlaylistWindow->DisplayMode();
	if (fSettings->ReplaceInt32( "playlist display mode", displayMode ) != B_OK )
		fSettings->AddInt32( "playlist display mode", displayMode );

	save_settings( fSettings, "interface_settings", "VideoLAN Client" );
}


/*****************************************************************************
 * CDMenu::CDMenu
 *****************************************************************************/
CDMenu::CDMenu(const char *name)
      : BMenu(name)
{
}

/*****************************************************************************
 * CDMenu::~CDMenu
 *****************************************************************************/
CDMenu::~CDMenu()
{
}

/*****************************************************************************
 * CDMenu::AttachedToWindow
 *****************************************************************************/
void CDMenu::AttachedToWindow(void)
{
    // remove all items
    while ( BMenuItem* item = RemoveItem( 0L ) )
        delete item;
    GetCD( "/dev/disk" );
    BMenu::AttachedToWindow();
}

/*****************************************************************************
 * CDMenu::GetCD
 *****************************************************************************/
int CDMenu::GetCD( const char *directory )
{
	BVolumeRoster volRoster;
	BVolume vol;
	BDirectory dir;
	status_t status = volRoster.GetNextVolume( &vol );
	while ( status ==  B_NO_ERROR )
	{
		BString deviceName;
		BString volumeName;
		bool isCDROM;
		if ( get_volume_info( vol, volumeName, isCDROM, deviceName )
			 && isCDROM )
		{
			BMessage* msg = new BMessage( OPEN_DVD );
			msg->AddString( "device", deviceName.String() );
			BMenuItem* item = new BMenuItem( volumeName.String(), msg );
			AddItem( item );
		}
 		vol.Unset();
		status = volRoster.GetNextVolume( &vol );
	}
	return 0;
}

/*****************************************************************************
 * LanguageMenu::LanguageMenu
 *****************************************************************************/
LanguageMenu::LanguageMenu( const char *name, int menu_kind,
                            VlcWrapper *p_wrapper )
    :BMenu(name)
{
    kind = menu_kind;
    this->p_wrapper = p_wrapper;
}

/*****************************************************************************
 * LanguageMenu::~LanguageMenu
 *****************************************************************************/
LanguageMenu::~LanguageMenu()
{
}

/*****************************************************************************
 * LanguageMenu::AttachedToWindow
 *****************************************************************************/
void LanguageMenu::AttachedToWindow()
{
    // remove all items
    while ( BMenuItem* item = RemoveItem( 0L ) )
        delete item;

    SetRadioMode( true );
	if ( BList *list = p_wrapper->GetChannels( kind ) )
	{
	    for ( int32 i = 0; BMenuItem* item = (BMenuItem*)list->ItemAt( i ); i++ )
	        AddItem( item );
	    
	    if ( list->CountItems() > 1 )
	        AddItem( new BSeparatorItem(), 1 );
	}
    BMenu::AttachedToWindow();
}

/*****************************************************************************
 * TitleMenu::TitleMenu
 *****************************************************************************/
TitleMenu::TitleMenu( const char *name, intf_thread_t  *p_interface )
    : BMenu(name),
    p_intf( p_interface )
{
}

/*****************************************************************************
 * TitleMenu::~TitleMenu
 *****************************************************************************/
TitleMenu::~TitleMenu()
{
}

/*****************************************************************************
 * TitleMenu::AttachedToWindow
 *****************************************************************************/
void TitleMenu::AttachedToWindow()
{
    while( BMenuItem* item = RemoveItem( 0L ) )
        delete item;

    if ( BList *list = p_intf->p_sys->p_wrapper->GetTitles() )
	{    
		for( int i = 0; BMenuItem* item = (BMenuItem*)list->ItemAt( i ); i++ )
	        AddItem( item );
	}
    BMenu::AttachedToWindow();
}


/*****************************************************************************
 * ChapterMenu::ChapterMenu
 *****************************************************************************/
ChapterMenu::ChapterMenu( const char *name, intf_thread_t  *p_interface )
    : BMenu(name),
    p_intf( p_interface )
{
}

/*****************************************************************************
 * ChapterMenu::~ChapterMenu
 *****************************************************************************/
ChapterMenu::~ChapterMenu()
{
}

/*****************************************************************************
 * ChapterMenu::AttachedToWindow
 *****************************************************************************/
void ChapterMenu::AttachedToWindow()
{
    while( BMenuItem* item = RemoveItem( 0L ) )
        delete item;

    if ( BList* list = p_intf->p_sys->p_wrapper->GetChapters() )
	{    
	    for( int i = 0; BMenuItem* item = (BMenuItem*)list->ItemAt( i ); i++ )
	        AddItem( item );
	}
    
    BMenu::AttachedToWindow();
}









/*****************************************************************************
 * load_settings
 *****************************************************************************/
status_t
load_settings( BMessage* message, const char* fileName, const char* folder )
{
	status_t ret = B_BAD_VALUE;
	if ( message )
	{
		BPath path;
		if ( ( ret = find_directory( B_USER_SETTINGS_DIRECTORY, &path ) ) == B_OK )
		{
			// passing folder is optional
			if ( folder )
				ret = path.Append( folder );
			if ( ret == B_OK && ( ret = path.Append( fileName ) ) == B_OK )
			{
				BFile file( path.Path(), B_READ_ONLY );
				if ( ( ret = file.InitCheck() ) == B_OK )
				{
					ret = message->Unflatten( &file );
					file.Unset();
				}
			}
		}
	}
	return ret;
}

/*****************************************************************************
 * save_settings
 *****************************************************************************/
status_t
save_settings( BMessage* message, const char* fileName, const char* folder )
{
	status_t ret = B_BAD_VALUE;
	if ( message )
	{
		BPath path;
		if ( ( ret = find_directory( B_USER_SETTINGS_DIRECTORY, &path ) ) == B_OK )
		{
			// passing folder is optional
			if ( folder && ( ret = path.Append( folder ) ) == B_OK )
				ret = create_directory( path.Path(), 0777 );
			if ( ret == B_OK && ( ret = path.Append( fileName ) ) == B_OK )
			{
				BFile file( path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE );
				if ( ( ret = file.InitCheck() ) == B_OK )
				{
					ret = message->Flatten( &file );
					file.Unset();
				}
			}
		}
	}
	return ret;
}
