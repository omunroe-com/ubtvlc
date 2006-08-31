/*****************************************************************************
 * menus.cpp : Qt menus
 *****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 * $Id: menus.cpp 16235 2006-08-06 17:12:01Z zorglub $
 *
 * Authors: Cl�ment Stenac <zorglub@videolan.org>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "menus.hpp"
#include "dialogs_provider.hpp"
#include "input_manager.hpp"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QSignalMapper>

enum
{
    ITEM_NORMAL,
    ITEM_CHECK,
    ITEM_RADIO
};

static QActionGroup *currentGroup;
static char ** pp_sds;

// Add static entries to menus
#define DP_SADD( text, help, icon, slot ) { if( strlen(icon) > 0 ) { QAction *action = menu->addAction( text, THEDP, SLOT( slot ) ); action->setIcon(QIcon(icon));} else { menu->addAction( text, THEDP, SLOT( slot ) ); } }
#define MIM_SADD( text, help, icon, slot ) { if( strlen(icon) > 0 ) { QAction *action = menu->addAction( text, THEMIM, SLOT( slot ) ); action->setIcon(QIcon(icon));} else { menu->addAction( text, THEMIM, SLOT( slot ) ); } }
#define PL_SADD

/*****************************************************************************
 * Definitions of variables for the dynamic menus
 *****************************************************************************/
#define PUSH_VAR( var ) varnames.push_back( var ); \
                        objects.push_back( p_object->i_object_id )

#define PUSH_SEPARATOR if( objects.size() != i_last_separator ) { \
                           objects.push_back( 0 ); varnames.push_back( "" ); \
                           i_last_separator = objects.size(); }

static int InputAutoMenuBuilder( vlc_object_t *p_object,
                                 vector<int> &objects,
                                 vector<const char *> &varnames )
{
    PUSH_VAR( "bookmark");
    PUSH_VAR( "title" );
    PUSH_VAR ("chapter" );
    PUSH_VAR( "program" );
    PUSH_VAR( "navigation" );
    PUSH_VAR( "dvd_menus" );
    return VLC_SUCCESS;
}

static int VideoAutoMenuBuilder( vlc_object_t *p_object,
                                 vector<int> &objects,
                                 vector<const char *> &varnames )
{
    PUSH_VAR( "fullscreen" );
    PUSH_VAR( "zoom" );
    PUSH_VAR( "deinterlace" );
    PUSH_VAR( "aspect-ratio" );
    PUSH_VAR( "crop" );
    PUSH_VAR( "video-on-top" );
    PUSH_VAR( "directx-wallpaper" );
    PUSH_VAR( "video-snapshot" );

    vlc_object_t *p_dec_obj = (vlc_object_t *)vlc_object_find( p_object,
                                                 VLC_OBJECT_DECODER,
                                                 FIND_PARENT );
    if( p_dec_obj != NULL )
    {
        PUSH_VAR( "ffmpeg-pp-q" );
        vlc_object_release( p_dec_obj );
    }
    return VLC_SUCCESS;
}

static int AudioAutoMenuBuilder( vlc_object_t *p_object,
                                 vector<int> &objects,
                                 vector<const char *> &varnames )
{
    PUSH_VAR( "audio-device" );
    PUSH_VAR( "audio-channels" );
    PUSH_VAR( "visual" );
    PUSH_VAR( "equalizer" );
    return VLC_SUCCESS;
}

/*****************************************************************************
 * All normal menus
 *****************************************************************************/

#define BAR_ADD( func, title ) { \
    QMenu *menu = func; menu->setTitle( title  ); bar->addMenu( menu ); }

#define BAR_DADD( func, title, id ) { \
    QMenu *menu = func; menu->setTitle( title  ); bar->addMenu( menu ); \
    MenuFunc *f = new MenuFunc( menu, id ); \
    connect( menu, SIGNAL( aboutToShow() ), \
            THEDP->menusUpdateMapper, SLOT(map()) ); \
    THEDP->menusUpdateMapper->setMapping( menu, f ); }

void QVLCMenu::createMenuBar( QMenuBar *bar, intf_thread_t *p_intf )
{
    BAR_ADD( FileMenu(), qtr("File") );
    BAR_ADD( ToolsMenu( p_intf ), qtr("Tools") );
    BAR_DADD( VideoMenu( p_intf, NULL ), qtr("Video"), 1 );
    BAR_DADD( AudioMenu( p_intf, NULL ), qtr("Audio"), 2 );
    BAR_DADD( NavigMenu( p_intf, NULL ), qtr("Navigation"), 3 );

    //    BAR_ADD( HelpMenu(), qtr("Help" ) );
}

void QVLCMenu::createPlMenuBar( QMenuBar *bar, intf_thread_t *p_intf )
{
    QMenu *manageMenu = new QMenu();
    manageMenu->addAction( "Quick &Add File...", THEDP,
                           SLOT( simpleAppendDialog() ) );
    manageMenu->addSeparator();
    manageMenu->addMenu( SDMenu( p_intf ) );

    bar->addMenu( manageMenu );
}

QMenu *QVLCMenu::SDMenu( intf_thread_t *p_intf )
{
    QMenu *menu = new QMenu();
    menu->setTitle( qtr( "Services Discovery" ) );
    playlist_t *p_playlist = (playlist_t *)vlc_object_find( p_intf,
                                            VLC_OBJECT_PLAYLIST,
                                            FIND_ANYWHERE );
    assert( p_playlist );
    vlc_list_t *p_list = vlc_list_find( p_intf, VLC_OBJECT_MODULE,
                                        FIND_ANYWHERE );
    int i_num = 0;
    for( int i_index = 0 ; i_index < p_list->i_count; i_index++ )
    {
        module_t * p_parser = (module_t *)p_list->p_values[i_index].p_object ;
        if( !strcmp( p_parser->psz_capability, "services_discovery" ) )
            i_num++;
    }
    if( i_num )  pp_sds = (char **)calloc( i_num, sizeof(void *) );
    for( int i_index = 0 ; i_index < p_list->i_count; i_index++ )
    {
        module_t * p_parser = (module_t *)p_list->p_values[i_index].p_object;
        if( !strcmp( p_parser->psz_capability, "services_discovery" ) )
        {
            QAction *a = menu->addAction(
                            qfu( p_parser->psz_longname ?
                                   p_parser->psz_longname :
                                   ( p_parser->psz_shortname ?
                                       p_parser->psz_shortname :
                                       p_parser->psz_object_name ) ) );
            a->setCheckable( true );
            /* hack to handle submodules properly */
            int i = -1;
            while( p_parser->pp_shortcuts[++i] != NULL );
            i--;
            if( playlist_IsServicesDiscoveryLoaded( p_playlist,
                 i>=0?p_parser->pp_shortcuts[i] : p_parser->psz_object_name ) )
            {
                a->setChecked( true );
            }
            pp_sds[i_num++] = i>=0? p_parser->pp_shortcuts[i] :
                                    p_parser->psz_object_name;
        }
    }
    vlc_list_release( p_list );
    vlc_object_release( p_playlist );
    return menu;
}

QMenu *QVLCMenu::FileMenu()
{
    QMenu *menu = new QMenu();
    DP_SADD( qtr("Quick &Open File...") , "", "", simpleOpenDialog() );
    DP_SADD( qtr("&Advanced Open..." ), "", "", openDialog() );
    menu->addSeparator();
    DP_SADD( qtr("Streaming..."), "", "", streamingDialog() );
    menu->addSeparator();
    DP_SADD( qtr("&Quit") , "", "", quit() );
    return menu;
}

QMenu *QVLCMenu::ToolsMenu( intf_thread_t *p_intf, bool with_intf )
{
    QMenu *menu = new QMenu();
    if( with_intf )
    {
        QMenu *intfmenu = InterfacesMenu( p_intf, NULL );
        intfmenu->setTitle( qtr("Interfaces" ) );
        menu->addMenu( intfmenu );
        /** \todo ADD EXT GUI HERE */
        menu->addSeparator();
    }
    DP_SADD( qtr("Messages" ), "", "", messagesDialog() );
    DP_SADD( qtr("Information") , "", "", streaminfoDialog() );
    DP_SADD( qtr("Bookmarks"), "", "", bookmarksDialog() );
    menu->addSeparator();
    DP_SADD( qtr("Preferences"), "", "", prefsDialog() );
    return menu;
}

QMenu *QVLCMenu::InterfacesMenu( intf_thread_t *p_intf, QMenu *current )
{
    vector<int> objects;
    vector<const char *> varnames;
    /** \todo add "switch to XXX" */
    varnames.push_back( "intf-add" );
    objects.push_back( p_intf->i_object_id );

    QMenu *menu = Populate( p_intf, current, varnames, objects );
    connect( menu, SIGNAL( aboutToShow() ),
             THEDP->menusUpdateMapper, SLOT(map()) );
    THEDP->menusUpdateMapper->setMapping( menu, 4 );

    return menu;
}

QMenu *QVLCMenu::AudioMenu( intf_thread_t *p_intf, QMenu * current )
{
    vector<int> objects;
    vector<const char *> varnames;

    vlc_object_t *p_object = (vlc_object_t *)vlc_object_find( p_intf,
                                        VLC_OBJECT_INPUT, FIND_ANYWHERE );
    if( p_object != NULL )
    {
        PUSH_VAR( "audio-es" );
        vlc_object_release( p_object );
    }

    p_object = (vlc_object_t *)vlc_object_find( p_intf, VLC_OBJECT_AOUT,
                                                FIND_ANYWHERE );
    if( p_object )
    {
        AudioAutoMenuBuilder( p_object, objects, varnames );
        vlc_object_release( p_object );
    }
    return Populate( p_intf, current, varnames, objects );
}


QMenu *QVLCMenu::VideoMenu( intf_thread_t *p_intf, QMenu *current )
{
    vlc_object_t *p_object;
    vector<int> objects;
    vector<const char *> varnames;

    p_object = (vlc_object_t *)vlc_object_find( p_intf, VLC_OBJECT_INPUT,
                                                FIND_ANYWHERE );
    if( p_object != NULL )
    {
        PUSH_VAR( "video-es" );
        PUSH_VAR( "spu-es" );
        vlc_object_release( p_object );
    }

    p_object = (vlc_object_t *)vlc_object_find( p_intf, VLC_OBJECT_VOUT,
                                                FIND_ANYWHERE );
    if( p_object != NULL )
    {
        VideoAutoMenuBuilder( p_object, objects, varnames );
        vlc_object_release( p_object );
    }
    return Populate( p_intf, current, varnames, objects );
}

QMenu *QVLCMenu::NavigMenu( intf_thread_t *p_intf, QMenu *current )
{
    vlc_object_t *p_object;
    vector<int> objects;
    vector<const char *> varnames;

    /* FIXME */
    p_object = (vlc_object_t *)vlc_object_find( p_intf, VLC_OBJECT_INPUT,
                                                FIND_ANYWHERE );
    if( p_object != NULL )
    {
        InputAutoMenuBuilder( p_object, objects, varnames );
        PUSH_VAR( "prev-title"); PUSH_VAR ( "next-title" );
        PUSH_VAR( "prev-chapter"); PUSH_VAR( "next-chapter" );
        vlc_object_release( p_object );
    }
    return Populate( p_intf, current, varnames, objects );
}


/*****************************************************************************
 * Popup menus
 *****************************************************************************/
#define POPUP_BOILERPLATE \
    unsigned int i_last_separator = 0; \
    vector<int> objects; \
    vector<const char *> varnames; \
    input_thread_t *p_input = THEMIM->getInput();

#define CREATE_POPUP \
    QMenu *menu = new QMenu(); \
    Populate( p_intf, menu, varnames, objects ); \
    p_intf->p_sys->p_popup_menu = menu; \
    menu->popup( QCursor::pos() ); \
    p_intf->p_sys->p_popup_menu = NULL; \
    i_last_separator = 0;

#define POPUP_STATIC_ENTRIES \
    vlc_value_t val; \
    MIM_SADD( qtr("Stop"), "", "", stop() ); \
    MIM_SADD( qtr("Previous"), "", "", prev() ); \
    MIM_SADD( qtr("Next"), "", "", next() ); \
    if( p_input ) \
    { \
        var_Get( p_input, "state", &val ); \
        if( val.i_int == PAUSE_S ) \
            MIM_SADD( qtr("Play"), "", "", togglePlayPause() ) \
        else \
            MIM_SADD( qtr("Pause"), "", "", togglePlayPause() ) \
    } \
    else if( THEPL->i_size && THEPL->i_enabled ) \
        MIM_SADD( qtr("Play"), "", "", togglePlayPause() ) \
    \
    QMenu *intfmenu = InterfacesMenu( p_intf, NULL ); \
    intfmenu->setTitle( qtr("Interfaces" ) ); \
    menu->addMenu( intfmenu ); \
    \
    QMenu *toolsmenu = ToolsMenu( p_intf, false ); \
    toolsmenu->setTitle( qtr("Tools" ) ); \
    menu->addMenu( toolsmenu ); \

void QVLCMenu::VideoPopupMenu( intf_thread_t *p_intf )
{
    POPUP_BOILERPLATE;
    if( p_input )
    {
        vlc_object_yield( p_input );
        varnames.push_back( "video-es" );
        objects.push_back( p_input->i_object_id );
        varnames.push_back( "spu-es" );
        objects.push_back( p_input->i_object_id );
        vlc_object_t *p_vout = (vlc_object_t *)vlc_object_find( p_input,
                                                VLC_OBJECT_VOUT, FIND_CHILD );
        if( p_vout )
        {
            VideoAutoMenuBuilder( p_vout, objects, varnames );
            vlc_object_release( p_vout );
        }
        vlc_object_release( p_input );
    }
    CREATE_POPUP;
}

void QVLCMenu::AudioPopupMenu( intf_thread_t *p_intf )
{
    POPUP_BOILERPLATE;
    if( p_input )
    {
        vlc_object_yield( p_input );
        varnames.push_back( "audio-es" );
        objects.push_back( p_input->i_object_id );
        vlc_object_t *p_aout = (vlc_object_t *)vlc_object_find( p_input,
                                             VLC_OBJECT_AOUT, FIND_ANYWHERE );
        if( p_aout )
        {
            AudioAutoMenuBuilder( p_aout, objects, varnames );
            vlc_object_release( p_aout );
        }
        vlc_object_release( p_input );
    }
    CREATE_POPUP;
}

/* Navigation stuff, and general */
void QVLCMenu::MiscPopupMenu( intf_thread_t *p_intf )
{
    POPUP_BOILERPLATE;
    if( p_input )
    {
        vlc_object_yield( p_input );
        varnames.push_back( "audio-es" );
        InputAutoMenuBuilder( VLC_OBJECT(p_input), objects, varnames );
        PUSH_SEPARATOR;
    }

    QMenu *menu = new QMenu();
    Populate( p_intf, menu, varnames, objects );
    menu->addSeparator();
    POPUP_STATIC_ENTRIES;

    p_intf->p_sys->p_popup_menu = menu;
    menu->popup( QCursor::pos() );
    p_intf->p_sys->p_popup_menu = NULL;
}

void QVLCMenu::PopupMenu( intf_thread_t *p_intf )
{
    POPUP_BOILERPLATE;
    if( p_input )
    {
        vlc_object_yield( p_input );
        InputAutoMenuBuilder( VLC_OBJECT(p_input), objects, varnames );

        /* Video menu */
        PUSH_SEPARATOR;
        varnames.push_back( "video-es" );
        objects.push_back( p_input->i_object_id );
        varnames.push_back( "spu-es" );
        objects.push_back( p_input->i_object_id );
        vlc_object_t *p_vout = (vlc_object_t *)vlc_object_find( p_input,
                                                VLC_OBJECT_VOUT, FIND_CHILD );
        if( p_vout )
        {
            VideoAutoMenuBuilder( p_vout, objects, varnames );
            vlc_object_release( p_vout );
        }
        /* Audio menu */
        PUSH_SEPARATOR
        varnames.push_back( "audio-es" );
        objects.push_back( p_input->i_object_id );
        vlc_object_t *p_aout = (vlc_object_t *)vlc_object_find( p_input,
                                             VLC_OBJECT_AOUT, FIND_ANYWHERE );
        if( p_aout )
        {
            AudioAutoMenuBuilder( p_aout, objects, varnames );
            vlc_object_release( p_aout );
        }
    }

    QMenu *menu = new QMenu();
    Populate( p_intf, menu, varnames, objects );
    menu->addSeparator();
    POPUP_STATIC_ENTRIES;

    p_intf->p_sys->p_popup_menu = menu;
    menu->popup( QCursor::pos() );
    p_intf->p_sys->p_popup_menu = NULL;
}

#undef PUSH_VAR
#undef PUSH_SEPARATOR

/*************************************************************************
 * Builders for automenus
 *************************************************************************/
QMenu * QVLCMenu::Populate( intf_thread_t *p_intf, QMenu *current,
                            vector< const char *> & varnames,
                            vector<int> & objects, bool append )
{
    QMenu *menu = current;
    if( !menu )
        menu = new QMenu();
    else if( !append )
        menu->clear();

    currentGroup = NULL;

    vlc_object_t *p_object;
    vlc_bool_t b_section_empty = VLC_FALSE;
    int i;

#define APPEND_EMPTY { QAction *action = menu->addAction( qtr("Empty" ) ); \
                       action->setEnabled( false ); }

    for( i = 0; i < (int)objects.size() ; i++ )
    {
        if( !varnames[i] || !*varnames[i] )
        {
            if( b_section_empty )
                APPEND_EMPTY;
            menu->addSeparator();
            b_section_empty = VLC_TRUE;
            continue;
        }

        if( objects[i] == 0  )
        {
            /// \bug What is this ?
            // Append( menu, varnames[i], NULL );
            b_section_empty = VLC_FALSE;
            continue;
        }

        p_object = (vlc_object_t *)vlc_object_get( p_intf,
                                                   objects[i] );
        if( p_object == NULL ) continue;

        b_section_empty = VLC_FALSE;
        /* Ugly specific stuff */
        if( strstr(varnames[i], "intf-add" ) )
            CreateItem( menu, varnames[i], p_object, false );
        else
            CreateItem( menu, varnames[i], p_object, true );
        vlc_object_release( p_object );
    }

    /* Special case for empty menus */
    if( menu->actions().size() == 0 || b_section_empty )
        APPEND_EMPTY

    return menu;
}

/*****************************************************************************
 * Private methods.
 *****************************************************************************/

static bool IsMenuEmpty( const char *psz_var, vlc_object_t *p_object,
                         bool b_root = TRUE )
{
    vlc_value_t val, val_list;
    int i_type, i_result, i;

    /* Check the type of the object variable */
    i_type = var_Type( p_object, psz_var );

    /* Check if we want to display the variable */
    if( !(i_type & VLC_VAR_HASCHOICE) ) return FALSE;

    var_Change( p_object, psz_var, VLC_VAR_CHOICESCOUNT, &val, NULL );
    if( val.i_int == 0 ) return TRUE;

    if( (i_type & VLC_VAR_TYPE) != VLC_VAR_VARIABLE )
    {
        /* Very evil hack ! intf-switch can have only one value */ 
        if( !strcmp( psz_var, "intf-switch" ) ) return FALSE;
        if( val.i_int == 1 && b_root ) return TRUE;
        else return FALSE;
    }

    /* Check children variables in case of VLC_VAR_VARIABLE */
    if( var_Change( p_object, psz_var, VLC_VAR_GETLIST, &val_list, NULL ) < 0 )
    {
        return TRUE;
    }

    for( i = 0, i_result = TRUE; i < val_list.p_list->i_count; i++ )
    {
        if( !IsMenuEmpty( val_list.p_list->p_values[i].psz_string,
                          p_object, FALSE ) )
        {
            i_result = FALSE;
            break;
        }
    }

    /* clean up everything */
    var_Change( p_object, psz_var, VLC_VAR_FREELIST, &val_list, NULL );

    return i_result;
}

void QVLCMenu::CreateItem( QMenu *menu, const char *psz_var,
                           vlc_object_t *p_object, bool b_submenu )
{
    vlc_value_t val, text;
    int i_type;

    /* Check the type of the object variable */
    i_type = var_Type( p_object, psz_var );

    switch( i_type & VLC_VAR_TYPE )
    {
    case VLC_VAR_VOID:
    case VLC_VAR_BOOL:
    case VLC_VAR_VARIABLE:
    case VLC_VAR_STRING:
    case VLC_VAR_INTEGER:
    case VLC_VAR_FLOAT:
        break;
    default:
        /* Variable doesn't exist or isn't handled */
        return;
    }

    /* Make sure we want to display the variable */
    if( IsMenuEmpty( psz_var, p_object ) )  return;

    /* Get the descriptive name of the variable */
    var_Change( p_object, psz_var, VLC_VAR_GETTEXT, &text, NULL );

    if( i_type & VLC_VAR_HASCHOICE )
    {
        /* Append choices menu */
        if( b_submenu )
        {
            QMenu *submenu = new QMenu();
            submenu->setTitle( qfu( text.psz_string ?
                                    text.psz_string : psz_var ) );
            if( CreateChoicesMenu( submenu, psz_var, p_object, true ) == 0)
                menu->addMenu( submenu );
        }
        else
            CreateChoicesMenu( menu, psz_var, p_object, true );
        FREENULL( text.psz_string );
        return;
    }

#define TEXT_OR_VAR qfu ( text.psz_string ? text.psz_string : psz_var )

    switch( i_type & VLC_VAR_TYPE )
    {
    case VLC_VAR_VOID:
        var_Get( p_object, psz_var, &val );
        CreateAndConnect( menu, psz_var, TEXT_OR_VAR, "", ITEM_NORMAL,
                          p_object->i_object_id, val, i_type );
        break;

    case VLC_VAR_BOOL:
        var_Get( p_object, psz_var, &val );
        val.b_bool = !val.b_bool;
        CreateAndConnect( menu, psz_var, TEXT_OR_VAR, "", ITEM_CHECK,
                          p_object->i_object_id, val, i_type, val.b_bool );
        break;
    }
    FREENULL( text.psz_string );
}


int QVLCMenu::CreateChoicesMenu( QMenu *submenu, const char *psz_var, 
                                 vlc_object_t *p_object, bool b_root )
{
    vlc_value_t val, val_list, text_list;
    int i_type, i;

    /* Check the type of the object variable */
    i_type = var_Type( p_object, psz_var );

    /* Make sure we want to display the variable */
    if( IsMenuEmpty( psz_var, p_object, b_root ) ) return VLC_EGENERIC;

    switch( i_type & VLC_VAR_TYPE )
    {
    case VLC_VAR_VOID:
    case VLC_VAR_BOOL:
    case VLC_VAR_VARIABLE:
    case VLC_VAR_STRING:
    case VLC_VAR_INTEGER:
    case VLC_VAR_FLOAT:
        break;
    default:
        /* Variable doesn't exist or isn't handled */
        return VLC_EGENERIC;
    }

    if( var_Change( p_object, psz_var, VLC_VAR_GETLIST,
                    &val_list, &text_list ) < 0 )
    {
        return VLC_EGENERIC;
    }
#define NORMAL_OR_RADIO i_type & VLC_VAR_ISCOMMAND ? ITEM_NORMAL: ITEM_RADIO
#define NOTCOMMAND !(i_type & VLC_VAR_ISCOMMAND)
#define CURVAL val_list.p_list->p_values[i]
#define CURTEXT text_list.p_list->p_values[i].psz_string

    for( i = 0; i < val_list.p_list->i_count; i++ )
    {
        vlc_value_t another_val;
        QString menutext;
        QMenu *subsubmenu = new QMenu();

        switch( i_type & VLC_VAR_TYPE )
        {
        case VLC_VAR_VARIABLE:
            CreateChoicesMenu( subsubmenu, CURVAL.psz_string, p_object, false );
            subsubmenu->setTitle( qfu( CURTEXT ? CURTEXT :CURVAL.psz_string ) );
            submenu->addMenu( subsubmenu );
            break;

        case VLC_VAR_STRING:
          var_Get( p_object, psz_var, &val );
          another_val.psz_string = strdup( CURVAL.psz_string );

          menutext = qfu( CURTEXT ? CURTEXT : another_val.psz_string );
          CreateAndConnect( submenu, psz_var, menutext, "", NORMAL_OR_RADIO,
                            p_object->i_object_id, another_val, i_type,
                            NOTCOMMAND && val.psz_string &&
                            !strcmp( val.psz_string, CURVAL.psz_string ) );

          if( val.psz_string ) free( val.psz_string );
          break;

        case VLC_VAR_INTEGER:
          var_Get( p_object, psz_var, &val );
          if( CURTEXT ) menutext = qfu( CURTEXT );
          else menutext.sprintf( "%d", CURVAL.i_int);
          CreateAndConnect( submenu, psz_var, menutext, "", NORMAL_OR_RADIO,
                            p_object->i_object_id, CURVAL, i_type,
                            NOTCOMMAND && CURVAL.i_int == val.i_int );
          break;

        case VLC_VAR_FLOAT:
          var_Get( p_object, psz_var, &val );
          if( CURTEXT ) menutext = qfu( CURTEXT );
          else menutext.sprintf( "%.2f", CURVAL.f_float );
          CreateAndConnect( submenu, psz_var, menutext, "", NORMAL_OR_RADIO,
                            p_object->i_object_id, CURVAL, i_type,
                            NOTCOMMAND && CURVAL.f_float == val.f_float );
          break;

        default:
          break;
        }
    }

    /* clean up everything */
    var_Change( p_object, psz_var, VLC_VAR_FREELIST, &val_list, &text_list );

#undef NORMAL_OR_RADIO
#undef NOTCOMMAND
#undef CURVAL
#undef CURTEXT
    return VLC_SUCCESS;
}

void QVLCMenu::CreateAndConnect( QMenu *menu, const char *psz_var,
                                 QString text, QString help,
                                 int i_item_type, int i_object_id,
                                 vlc_value_t val, int i_val_type,
                                 bool checked )
{
    QAction *action = new QAction( text, menu );
    action->setText( text );
    action->setToolTip( help );

    if( i_item_type == ITEM_CHECK )
    {
        action->setCheckable( true );
        currentGroup = NULL;
    }
    else if( i_item_type == ITEM_RADIO )
    {
        action->setCheckable( true );
        if( !currentGroup )
            currentGroup = new QActionGroup(menu);
        currentGroup->addAction( action );
    }
    else
        currentGroup = NULL;
    if( checked ) action->setChecked( true );

    MenuItemData *itemData = new MenuItemData( i_object_id, i_val_type,
                                               val, psz_var );
    connect( action, SIGNAL(triggered()), THEDP->menusMapper, SLOT(map()) );
    THEDP->menusMapper->setMapping( action, itemData );
    menu->addAction( action );
}

void QVLCMenu::DoAction( intf_thread_t *p_intf, QObject *data )
{
    MenuItemData *itemData = qobject_cast<MenuItemData *>(data);
    vlc_object_t *p_object = (vlc_object_t *)vlc_object_get( p_intf,
                                           itemData->i_object_id );
    if( p_object == NULL ) return;

    var_Set( p_object, itemData->psz_var, itemData->val );
    vlc_object_release( p_object );
}
