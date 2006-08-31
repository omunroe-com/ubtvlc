/*****************************************************************************
 * playlist_model.hpp : Model for a playlist tree
 ****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 * $Id: playlist_model.hpp 16235 2006-08-06 17:12:01Z zorglub $
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

#ifndef _PLAYLIST_MODEL_H_
#define _PLAYLIST_MODEL_H_

#include <QObject>
#include <QEvent>
#include <vlc/vlc.h>
#include <vlc/input.h>
#include <vlc_playlist.h>

class PLModel;

class PLItem
{
public:
    PLItem( int, int, PLItem *parent , PLModel *);
    PLItem( playlist_item_t *, PLItem *parent, PLModel *);
    ~PLItem();

    int row() const;
    void insertChild( PLItem *, int p, bool signal = true );

    void appendChild( PLItem *item, bool signal = true )
    {
        insertChild( item, children.count(), signal );
    };
    void remove( PLItem *removed );
    PLItem *child( int row ) { return children.value( row ); };
    int childCount() const { return children.count(); };
    QString columnString( int col ) { return strings.value( col ); };
    PLItem *parent() { return parentItem; };

    void update( playlist_item_t *, bool);
protected:
    QList<PLItem*> children;
    QList<QString> strings;
    bool current;
    int type;
    int i_id;
    int i_input_id;
    friend class PLModel;
private:
    void init( int, int, PLItem *, PLModel * );
    PLItem *parentItem;
    PLModel *model;
};

static int ItemUpdate_Type = QEvent::User + 2;
static int ItemDelete_Type = QEvent::User + 3;
static int ItemAppend_Type = QEvent::User + 4;

class PLEvent : public QEvent
{
public:
    PLEvent( int type, int id ) : QEvent( (QEvent::Type)(type) )
    { i_id = id; p_add = NULL; };
    PLEvent(  playlist_add_t  *a ) : QEvent( (QEvent::Type)(ItemAppend_Type) )
    { p_add = a; };
    virtual ~PLEvent() {};

    int i_id;
    playlist_add_t *p_add;
};

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class PLModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    PLModel( playlist_t *, playlist_item_t *, int, QObject *parent = 0);
    ~PLModel();

    void customEvent( QEvent * );

    /* QModel stuff */
    QVariant data( const QModelIndex &index, int role) const;
    Qt::ItemFlags flags( const QModelIndex &index) const;
    QVariant headerData( int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;

    QModelIndex index( int r, int c, const QModelIndex &parent ) const;
    QModelIndex index( PLItem *, int c ) const;
    int itemId( const QModelIndex &index ) const;
    bool isCurrent( const QModelIndex &index );

    QModelIndex parent( const QModelIndex &index) const;
    int childrenCount( const QModelIndex &parent = QModelIndex() ) const;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;

    bool b_need_update;
    int i_items_to_append;
    void Rebuild();
    void rebuildRoot( playlist_item_t * );
    bool hasRandom(); bool hasLoop(); bool hasRepeat();
private:
    void addCallbacks();
    void delCallbacks();
    PLItem *rootItem;

    playlist_t *p_playlist;
    int i_depth;

    static QIcon icons[ITEM_TYPE_NUMBER];

    /* Update processing */
    void ProcessInputItemUpdate( int i_input_id );
    void ProcessItemRemoval( int i_id );
    void ProcessItemAppend( playlist_add_t *p_add );

    void UpdateTreeItem( PLItem *, bool, bool force = false );
    void UpdateTreeItem( playlist_item_t *, PLItem *, bool, bool forc = false );
    void UpdateNodeChildren( PLItem * );
    void UpdateNodeChildren( playlist_item_t *, PLItem * );

    /* Lookups */
    PLItem *FindById( PLItem *, int );
    PLItem *FindByInput( PLItem *, int );
    PLItem *FindInner( PLItem *, int , bool );
    PLItem *p_cached_item;
    PLItem *p_cached_item_bi;
    int i_cached_id;
    int i_cached_input_id;
public slots:
    void activateItem( const QModelIndex &index );
    void setRandom( bool );
    void setLoop( bool );
    void setRepeat( bool );
friend class PLItem;
};

#endif
