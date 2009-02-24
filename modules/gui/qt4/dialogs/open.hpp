/*****************************************************************************
 * open.hpp : advanced open dialog
 ****************************************************************************
 * Copyright (C) 2006-2007 the VideoLAN team
 * $Id$
 *
 * Authors: Jean-Baptiste Kempf <jb@videolan.org>
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
 ******************************************************************************/

#ifndef _OPEN_DIALOG_H_
#define _OPEN_DIALOG_H_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>

#include "util/qvlcframe.hpp"
#include "dialogs_provider.hpp"
#include "ui/open.h"
#include "components/open_panels.hpp"

enum {
    OPEN_FILE_TAB,
    OPEN_DISC_TAB,
    OPEN_NETWORK_TAB,
    OPEN_CAPTURE_TAB,
    OPEN_TAB_MAX
};

enum {
    OPEN_AND_PLAY,
    OPEN_AND_ENQUEUE,
    OPEN_AND_STREAM,
    OPEN_AND_SAVE,
    SELECT              /* Special mode to select a MRL (for VLM or similar */
};


class QString;
class QTabWidget;

class OpenDialog : public QVLCDialog
{
    friend class FileOpenBox;

    Q_OBJECT;
public:
    static OpenDialog * getInstance( QWidget *parent, intf_thread_t *p_intf,
                                bool b_rawInstance = false, int _action_flag = 0,
                                bool b_selectMode = false, bool b_pl = true );

    static void killInstance()
    {
        if( instance ) delete instance;
        instance = NULL;
    }
    virtual ~OpenDialog();

    void showTab( int = OPEN_FILE_TAB );
    QString getMRL(){ return mrl; }

public slots:
    void selectSlots();
    void play();
    void stream( bool b_transode_only = false );
    void enqueue();
    void transcode();

private:
    OpenDialog( QWidget *parent, intf_thread_t *, bool b_selectMode,
                int _action_flag = 0, bool b_pl = true );

    static OpenDialog *instance;
    input_thread_t *p_input;

    QString mrl;
    QString mainMRL;
    QString storedMethod;

    Ui::Open ui;
    FileOpenPanel *fileOpenPanel;
    NetOpenPanel *netOpenPanel;
    DiscOpenPanel *discOpenPanel;
    CaptureOpenPanel *captureOpenPanel;

    int i_action_flag;
    bool b_pl;
    QStringList SeparateEntries( QString );

    QPushButton *cancelButton, *selectButton;
    QPushButton *playButton;

    void finish( bool );

private slots:
    void setMenuAction();
    void cancel();
    void close();
    void toggleAdvancedPanel();
    void updateMRL( QString );
    void updateMRL();
    void newCachingMethod( QString );
    void signalCurrent( int );
    void browseInputSlave();
};

#endif
