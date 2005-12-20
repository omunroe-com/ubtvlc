/*****************************************************************************
 * VlcWrapper.h: BeOS plugin for vlc (derived from MacOS X port)
 *****************************************************************************
 * Copyright (C) 2001 VideoLAN
 * $Id: VlcWrapper.h 6961 2004-03-05 17:34:23Z sam $
 *
 * Authors: Florian G. Pflug <fgp@phlo.org>
 *          Jon Lech Johansen <jon-vl@nanocrew.net>
 *          Tony Castley <tony@castley.net>
 *          Stephan Aßmus <stippi@yellowbites.com>
 *          Eric Petit <titer@videolan.org>
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

#include <SupportKit.h>

#define SEEKSLIDER_RANGE 2048

class InterfaceWindow;
class VlcWrapper;

/*****************************************************************************
 * intf_sys_t: internal variables of the BeOS interface
 *****************************************************************************/
struct intf_sys_t
{
    msg_subscription_t * p_sub;

    InterfaceWindow *    p_window;
    
    vlc_bool_t           b_loop;
    vlc_bool_t           b_mute;
    int                  i_part;
    int                  i_channel;
    
    VlcWrapper *         p_wrapper;
};

/* Necessary because of i18n */
const char * _AddEllipsis( char * string );

/*****************************************************************************
 * VlcWrapper
 *****************************************************************************
 * This class makes the link between the BeOS interface and the vlc core.
 * There is only one VlcWrapper instance at any time, which is stored
 * in p_intf->p_sys->p_wrapper
 *****************************************************************************/
class VlcWrapper
{
public:
    VlcWrapper( intf_thread_t *p_intf );
    ~VlcWrapper();
    
    /* Input */
    void         UpdateInput();
    bool         HasInput();
    int          InputStatus();
    int          InputRate();
    void         InputSetRate( int rate );
    BList *      GetChannels( int i_cat );
    void         ToggleLanguage( int i_language );
    void         ToggleSubtitle( int i_subtitle );
    const char * GetTimeAsString();
    float        GetTimeAsFloat();
    void         SetTimeAsFloat( float i_offset );
    bool         IsPlaying();
        
    /* Playlist */
    void    OpenFiles( BList *o_files, bool replace = true,
    				   int32 index = -1 );
    void    OpenDisc( BString o_type, BString o_device,
                     int i_title, int i_chapter );
    int     PlaylistSize();
    char *  PlaylistItemName( int );
    int     PlaylistCurrent();
    bool    PlaylistPlay();
    void    PlaylistPause();
    void    PlaylistStop();
    void    PlaylistNext();
    void    PlaylistPrev();
    bool    PlaylistPlaying();
    void    GetPlaylistInfo( int32& currentIndex,
                             int32& maxIndex );
    void    PlaylistJumpTo( int );
    void    GetNavCapabilities( bool * canSkipPrev,
                                bool * canSkipNext );
    void    NavigatePrev();
    void    NavigateNext();

	/* Playlist manipulation */
	bool	PlaylistLock() const;
	void	PlaylistUnlock() const;
	// playlist must be locked prior to calling all of these!
	void*	PlaylistItemAt( int index ) const;
			// both functions return a copy of the removed item
			// so that it can be added at another index
	void*	PlaylistRemoveItem( int index ) const;
	void*	PlaylistRemoveItem( void* item ) const;
			// uses playlist_AddItem()
	bool	PlaylistAddItem( void* item, int index ) const;
	void*	PlaylistCloneItem( void* item ) const;
			// only modifies playlist, doesn't effect playback
	void	PlaylistSetPlaying( int index ) const;

    /* Audio */
    unsigned short GetVolume();
    void           SetVolume( int value );
    void           VolumeMute();
    void           VolumeRestore();
    bool           IsMuted();

    /* DVD */
    bool    IsUsingMenus();
    bool    HasTitles();
    BList * GetTitles();
    void    PrevTitle();
    void    NextTitle();
    void    ToggleTitle( int i_title );
    void    TitleInfo( int32& currentIndex, int32& maxIndex );

    bool    HasChapters();
    BList * GetChapters();
    void    PrevChapter();
    void    NextChapter();
    void    ToggleChapter( int i_chapter );
    void    ChapterInfo( int32& currentIndex, int32& maxIndex );
    
    /* Miscellanous */
    void LoadSubFile( const char * psz_file );
    void FilterChange();
    
private:
    intf_thread_t *   p_intf;
    input_thread_t *  p_input;
    playlist_t *      p_playlist;
};
