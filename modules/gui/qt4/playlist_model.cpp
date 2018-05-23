/*****************************************************************************
 * playlist_model.cpp : Manage playlist model
 ****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 * $Id: playlist_model.cpp 16235 2006-08-06 17:12:01Z zorglub $
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
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

#include <QIcon>
#include <QFont>
#include "qt4.hpp"
#include <QApplication>
#include "playlist_model.hpp"
#include <assert.h>

#include "pixmaps/type_unknown.xpm"
#include "pixmaps/type_afile.xpm"
#include "pixmaps/type_vfile.xpm"
#include "pixmaps/type_net.xpm"
#include "pixmaps/type_card.xpm"
#include "pixmaps/type_disc.xpm"
#include "pixmaps/type_cdda.xpm"
#include "pixmaps/type_directory.xpm"
#include "pixmaps/type_playlist.xpm"
#include "pixmaps/type_node.xpm"

QIcon PLModel::icons[ITEM_TYPE_NUMBER];

static int PlaylistChanged( vlc_object_t *, const char *,
                            vlc_value_t, vlc_value_t, void * );
static int PlaylistNext( vlc_object_t *, const char *,
                         vlc_value_t, vlc_value_t, void * );
static int ItemChanged( vlc_object_t *, const char *,
                        vlc_value_t, vlc_value_t, void * );
static int ItemAppended( vlc_object_t *p_this, const char *psz_variable,
                         vlc_value_t oval, vlc_value_t nval, void *param );
static int ItemDeleted( vlc_object_t *p_this, const char *psz_variable,
                        vlc_value_t oval, vlc_value_t nval, void *param );

/*************************************************************************
 * Playlist item implementation
 *************************************************************************/

/**
 * Column strings
 *      Title
 *      Artist
 *      Duration
 */

void PLItem::init( int _i_id, int _i_input_id, PLItem *parent, PLModel *m)
{
    parentItem = parent;
    i_id = _i_id; i_input_id = _i_input_id;
    model = m;
    strings.append( "" );
    strings.append( "" );
    strings.append( "" );
}

PLItem::PLItem( int _i_id, int _i_input_id, PLItem *parent, PLModel *m)
{
    init( _i_id, _i_input_id, parent, m );
}

PLItem::PLItem( playlist_item_t * p_item, PLItem *parent, PLModel *m )
{
    init( p_item->i_id, p_item->p_input->i_id, parent, m );
}

PLItem::~PLItem()
{
    qDeleteAll(children);
}

void PLItem::insertChild( PLItem *item, int i_pos, bool signal )
{
    assert( model );
    if( signal )
        model->beginInsertRows( model->index( this , 0 ), i_pos, i_pos );
    children.append( item );
    if( signal )
        model->endInsertRows();
}

void PLItem::remove( PLItem *removed )
{
    assert( model && parentItem );
    int i_index = parentItem->children.indexOf( removed );
    model->beginRemoveRows( model->index( parentItem, 0 ), i_index, i_index );
    parentItem->children.removeAt( i_index );
    model->endRemoveRows();
}

int PLItem::row() const
{
    if (parentItem)
        return parentItem->children.indexOf(const_cast<PLItem*>(this));
    return 0;
}

void PLItem::update( playlist_item_t *p_item, bool iscurrent )
{
    assert( p_item->p_input->i_id == i_input_id );
    strings[0] = QString::fromUtf8( p_item->p_input->psz_name );
    if( p_item->p_input->p_meta )
    {
        strings[1] = QString::fromUtf8( p_item->p_input->p_meta->psz_artist );
    }
    type = p_item->p_input->i_type;
    current = iscurrent;
}

/*************************************************************************
 * Playlist model implementation
 *************************************************************************/

PLModel::PLModel( playlist_t *_p_playlist,
                  playlist_item_t * p_root, int _i_depth, QObject *parent)
                                    : QAbstractItemModel(parent)
{
    i_depth = _i_depth;
    assert( i_depth == 1 || i_depth == -1 );
    p_playlist= _p_playlist;
    i_items_to_append = 0;
    b_need_update     = false;
    i_cached_id       = -1;
    i_cached_input_id = -1;

#define ADD_ICON(type, x) icons[ITEM_TYPE_##type] = QIcon( QPixmap( type_##x##_xpm ) );
    ADD_ICON( UNKNOWN , unknown );
    ADD_ICON( AFILE,afile );
    ADD_ICON( VFILE, vfile );
    ADD_ICON( DIRECTORY, directory );
    ADD_ICON( DISC, disc );
    ADD_ICON( CDDA, cdda );
    ADD_ICON( CARD, card );
    ADD_ICON( NET, net );
    ADD_ICON( PLAYLIST, playlist );
    ADD_ICON( NODE, node );

    rootItem = NULL;
    rebuildRoot( p_root );
    addCallbacks();

}

void PLModel::rebuildRoot( playlist_item_t *p_root )
{
    if( rootItem ) delete rootItem;
    rootItem = new PLItem( p_root, NULL, this );
    rootItem->strings[0] = qtr("Name");
    rootItem->strings[1] = qtr("Artist");
}

PLModel::~PLModel()
{
    delCallbacks();
    delete rootItem;
}

void PLModel::addCallbacks()
{
    /* Some global changes happened -> Rebuild all */
    var_AddCallback( p_playlist, "intf-change", PlaylistChanged, this );
    /* We went to the next item */
    var_AddCallback( p_playlist, "playlist-current", PlaylistNext, this );
    /* One item has been updated */
    var_AddCallback( p_playlist, "item-change", ItemChanged, this );
    var_AddCallback( p_playlist, "item-append", ItemAppended, this );
    var_AddCallback( p_playlist, "item-deleted", ItemDeleted, this );
}

void PLModel::delCallbacks()
{
    var_DelCallback( p_playlist, "item-change", ItemChanged, this );
    var_DelCallback( p_playlist, "playlist-current", PlaylistNext, this );
    var_DelCallback( p_playlist, "intf-change", PlaylistChanged, this );
    var_DelCallback( p_playlist, "item-append", ItemAppended, this );
    var_DelCallback( p_playlist, "item-deleted", ItemDeleted, this );
}

void PLModel::activateItem( const QModelIndex &index )
{
    assert( index.isValid() );
    PLItem *item = static_cast<PLItem*>(index.internalPointer());
    assert( item );
    PL_LOCK;
    playlist_item_t *p_item = playlist_ItemGetById( p_playlist, item->i_id );
    playlist_item_t *p_parent = p_item;
    while( p_parent )
    {
        if( p_parent->i_id == rootItem->i_id ) break;
        p_parent = p_parent->p_parent;
    }
    if( p_parent )
    {
        playlist_Control( p_playlist, PLAYLIST_VIEWPLAY, p_parent, p_item );
    }
    PL_UNLOCK;
}

/****************** Base model mandatory implementations *****************/
QVariant PLModel::data(const QModelIndex &index, int role) const
{
    assert( index.isValid() );
    PLItem *item = static_cast<PLItem*>(index.internalPointer());
    if( role == Qt::DisplayRole )
    {
        return QVariant( item->columnString( index.column() ) );
    }
    else if( role == Qt::DecorationRole && index.column() == 0  )
    {
        if( item->type >= 0 )
            return QVariant( PLModel::icons[item->type] );
    }
    else if( role == Qt::FontRole )
    {
        if( item->current == true )
        {
            QFont f; f.setBold( true ); return QVariant( f );
        }
    }
    return QVariant();
}

bool PLModel::isCurrent( const QModelIndex &index )
{
    assert( index.isValid() );
    return static_cast<PLItem*>(index.internalPointer())->current;
}

int PLModel::itemId( const QModelIndex &index ) const
{
    assert( index.isValid() );
    return static_cast<PLItem*>(index.internalPointer())->i_id;
}

Qt::ItemFlags PLModel::flags(const QModelIndex &index) const
{
    if( !index.isValid() ) return Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant PLModel::headerData( int section, Qt::Orientation orientation,
                              int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
            return QVariant( rootItem->columnString( section ) );
    return QVariant();
}

QModelIndex PLModel::index(int row, int column, const QModelIndex &parent)
                  const
{
    PLItem *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<PLItem*>(parent.internalPointer());

    PLItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

/* Return the index of a given item */
QModelIndex PLModel::index( PLItem *item, int column ) const
{
    if( !item ) return QModelIndex();
    const PLItem *parent = item->parent();
    if( parent )
        return createIndex( parent->children.lastIndexOf( item ), column, item );
    return QModelIndex();
}

QModelIndex PLModel::parent(const QModelIndex &index) const
{
    if( !index.isValid() ) return QModelIndex();

    PLItem *childItem = static_cast<PLItem*>(index.internalPointer());
    PLItem *parentItem = childItem->parent();

    if (parentItem == rootItem) return QModelIndex();
    return createIndex(parentItem->row(), 0, parentItem);
}

int PLModel::columnCount( const QModelIndex &i) const
{
    if( i_depth == 1 ) return 1;
    return 2;
}

int PLModel::childrenCount(const QModelIndex &parent) const
{
    return rowCount( parent );
}

int PLModel::rowCount(const QModelIndex &parent) const
{
    PLItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<PLItem*>(parent.internalPointer());

    return parentItem->childCount();
}

/************************* General playlist status ***********************/

bool PLModel::hasRandom()
{
    if( var_GetBool( p_playlist, "random" ) ) return true;
    return false;
}
bool PLModel::hasRepeat()
{
    if( var_GetBool( p_playlist, "repeat" ) ) return true;
    return false;
}
bool PLModel::hasLoop()
{
    if( var_GetBool( p_playlist, "loop" ) ) return true;
    return false;
}
void PLModel::setLoop( bool on )
{
    var_SetBool( p_playlist, "loop", on ? VLC_TRUE:VLC_FALSE );
}
void PLModel::setRepeat( bool on )
{
    var_SetBool( p_playlist, "repeat", on ? VLC_TRUE:VLC_FALSE );
}
void PLModel::setRandom( bool on )
{
    var_SetBool( p_playlist, "random", on ? VLC_TRUE:VLC_FALSE );
}

/************************* Lookups *****************************/

PLItem *PLModel::FindById( PLItem *root, int i_id )
{
    return FindInner( root, i_id, false );
}

PLItem *PLModel::FindByInput( PLItem *root, int i_id )
{
    return FindInner( root, i_id, true );
}

#define CACHE( i, p ) { i_cached_id = i; p_cached_item = p; }
#define ICACHE( i, p ) { i_cached_input_id = i; p_cached_item_bi = p; }

PLItem * PLModel::FindInner( PLItem *root, int i_id, bool b_input )
{
    if( ( !b_input && i_cached_id == i_id) ||
        ( b_input && i_cached_input_id ==i_id ) )
    {
        return b_input ? p_cached_item_bi : p_cached_item;
    }

    if( !b_input && root->i_id == i_id )
    {
        CACHE( i_id, root );
        return root;
    }
    else if( b_input && root->i_input_id == i_id )
    {
        ICACHE( i_id, root );
        return root;
    }

    QList<PLItem *>::iterator it = root->children.begin();
    while ( it != root->children.end() )
    {
        if( !b_input && (*it)->i_id == i_id )
        {
            CACHE( i_id, (*it) );
            return p_cached_item;
        }
        else if( b_input && (*it)->i_input_id == i_id )
        {
            ICACHE( i_id, (*it) );
            return p_cached_item_bi;
        }
        if( (*it)->children.size() )
        {
            PLItem *childFound = FindInner( (*it), i_id, b_input );
            if( childFound )
            {
                if( b_input )
                    ICACHE( i_id, childFound )
                else
                    CACHE( i_id, childFound )
                return childFound;
            }
        }
        it++;
    }
    return NULL;
}
#undef CACHE
#undef ICACHE


/************************* Updates handling *****************************/
void PLModel::customEvent( QEvent *event )
{
    int type = event->type();
    if( type != ItemUpdate_Type && type != ItemAppend_Type &&
        type != ItemDelete_Type )
        return;

    PLEvent *ple = static_cast<PLEvent *>(event);

    if( type == ItemUpdate_Type )
        ProcessInputItemUpdate( ple->i_id );
    else if( type == ItemAppend_Type )
        ProcessItemAppend( ple->p_add );
    else
        ProcessItemRemoval( ple->i_id );
}

/**** Events processing ****/
void PLModel::ProcessInputItemUpdate( int i_input_id )
{
    if( i_input_id <= 0 ) return;
    PLItem *item = FindByInput( rootItem, i_input_id );
    if( item )
        UpdateTreeItem( item, true );
}

void PLModel::ProcessItemRemoval( int i_id )
{
    if( i_id <= 0 ) return;
    if( i_id == i_cached_id ) i_cached_id = -1;
    i_cached_input_id = -1;

    PLItem *item = FindById( rootItem, i_id );
    if( item )
        item->remove( item );
}

void PLModel::ProcessItemAppend( playlist_add_t *p_add )
{
    playlist_item_t *p_item = NULL;
    PLItem *newItem = NULL;
    i_items_to_append--;
    if( b_need_update ) return;

    PLItem *nodeItem = FindById( rootItem, p_add->i_node );
    if( !nodeItem ) goto end;

    PL_LOCK;
    p_item = playlist_ItemGetById( p_playlist, p_add->i_item );
    if( !p_item || p_item->i_flags & PLAYLIST_DBL_FLAG ) goto end;
    if( i_depth == 1 && p_item->p_parent &&
                        p_item->p_parent->i_id != rootItem->i_id )
        goto end;

    newItem = new PLItem( p_item, nodeItem, this );
    nodeItem->appendChild( newItem );
    UpdateTreeItem( p_item, newItem, true );
end:
    PL_UNLOCK;
    return;
}

void PLModel::Rebuild()
{
    /* Remove callbacks before locking to avoid deadlocks */
    delCallbacks();
    /* Invalidate cache */
    i_cached_id = i_cached_input_id = -1;

    PL_LOCK;
    /* Clear the tree */
    qDeleteAll( rootItem->children );
    /* Recreate from root */
    UpdateNodeChildren( rootItem );
    if( p_playlist->status.p_item )
    {
        fprintf( stderr, "Playlist is playing" );
        PLItem *currentItem = FindByInput( rootItem,
                                     p_playlist->status.p_item->p_input->i_id );
        if( currentItem )
        {
            fprintf( stderr, "Updating item\n" );
            UpdateTreeItem( p_playlist->status.p_item, currentItem,
                            true, false );
        }
    }
    PL_UNLOCK;

    /* And signal the view */
    emit layoutChanged();
    addCallbacks();
}

/* This function must be entered WITH the playlist lock */
void PLModel::UpdateNodeChildren( PLItem *root )
{
    playlist_item_t *p_node = playlist_ItemGetById( p_playlist, root->i_id );
    UpdateNodeChildren( p_node, root );
}

/* This function must be entered WITH the playlist lock */
void PLModel::UpdateNodeChildren( playlist_item_t *p_node, PLItem *root )
{
    for( int i = 0; i < p_node->i_children ; i++ )
    {
        PLItem *newItem =  new PLItem( p_node->pp_children[i], root, this );
        root->appendChild( newItem, false );
        UpdateTreeItem( newItem, false, true );
        if( i_depth != 1 && p_node->pp_children[i]->i_children != -1 )
            UpdateNodeChildren( p_node->pp_children[i], newItem );
    }
}

/* This function must be entered WITH the playlist lock */
void PLModel::UpdateTreeItem( PLItem *item, bool signal, bool force )
{
    playlist_item_t *p_item = playlist_ItemGetById( p_playlist, item->i_id );
    UpdateTreeItem( p_item, item, signal, force );
}

/* This function must be entered WITH the playlist lock */
void PLModel::UpdateTreeItem( playlist_item_t *p_item, PLItem *item,
                              bool signal, bool force )
{
    if( !force && i_depth == 1 && p_item->p_parent &&
                                 p_item->p_parent->i_id != rootItem->i_id )
        return;
    item->update( p_item, p_item == p_playlist->status.p_item );
    if( signal )
        emit dataChanged( index( item, 0 ) , index( item, 1 ) );
}

/**********************************************************************
 * Playlist callbacks
 **********************************************************************/
static int PlaylistChanged( vlc_object_t *p_this, const char *psz_variable,
                            vlc_value_t oval, vlc_value_t nval, void *param )
{
    PLModel *p_model = (PLModel *) param;
    p_model->b_need_update = VLC_TRUE;
    return VLC_SUCCESS;
}

static int PlaylistNext( vlc_object_t *p_this, const char *psz_variable,
                         vlc_value_t oval, vlc_value_t nval, void *param )
{
    PLModel *p_model = (PLModel *) param;
    PLEvent *event = new PLEvent( ItemUpdate_Type, oval.i_int );
    QApplication::postEvent( p_model, static_cast<QEvent*>(event) );
    event = new PLEvent( ItemUpdate_Type, nval.i_int );
    QApplication::postEvent( p_model, static_cast<QEvent*>(event) );
    return VLC_SUCCESS;
}

static int ItemChanged( vlc_object_t *p_this, const char *psz_variable,
                        vlc_value_t oval, vlc_value_t nval, void *param )
{
    PLModel *p_model = (PLModel *) param;
    PLEvent *event = new PLEvent( ItemUpdate_Type, nval.i_int );
    QApplication::postEvent( p_model, static_cast<QEvent*>(event) );
    return VLC_SUCCESS;
}

static int ItemDeleted( vlc_object_t *p_this, const char *psz_variable,
                        vlc_value_t oval, vlc_value_t nval, void *param )
{
    PLModel *p_model = (PLModel *) param;
    PLEvent *event = new PLEvent( ItemDelete_Type, nval.i_int );
    QApplication::postEvent( p_model, static_cast<QEvent*>(event) );
    return VLC_SUCCESS;
}

static int ItemAppended( vlc_object_t *p_this, const char *psz_variable,
                         vlc_value_t oval, vlc_value_t nval, void *param )
{
    PLModel *p_model = (PLModel *) param;
    playlist_add_t *p_add = (playlist_add_t *)malloc( sizeof( playlist_add_t));
    memcpy( p_add, nval.p_address, sizeof( playlist_add_t ) );

    if( ++p_model->i_items_to_append >= 50 )
    {
        p_model->b_need_update = VLC_TRUE;
        return VLC_SUCCESS;
    }
    PLEvent *event = new PLEvent(  p_add );
    QApplication::postEvent( p_model, static_cast<QEvent*>(event) );
    return VLC_SUCCESS;
}
