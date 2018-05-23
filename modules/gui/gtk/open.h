/*****************************************************************************
 * gtk_open.h: prototypes for open functions
 *****************************************************************************
 * Copyright (C) 1999, 2000 VideoLAN
 * $Id: open.h 6961 2004-03-05 17:34:23Z sam $
 *
 * Authors: Samuel Hocevar <sam@zoy.org>
 *          St�phane Borel <stef@via.ecp.fr>
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

gboolean GtkFileOpenShow        ( GtkWidget *, gpointer );
void     GtkFileOpenCancel      ( GtkButton *, gpointer );
void     GtkFileOpenOk          ( GtkButton *, gpointer );

gboolean GtkDiscOpenShow        ( GtkWidget *, gpointer );
void     GtkDiscOpenDvd         ( GtkToggleButton *, gpointer );
void     GtkDiscOpenVcd         ( GtkToggleButton *, gpointer );
void     GtkDiscOpenCDDA        ( GtkToggleButton *, gpointer );
void     GtkDiscOpenOk          ( GtkButton *, gpointer );
void     GtkDiscOpenCancel      ( GtkButton *, gpointer );

gboolean GtkNetworkOpenShow     ( GtkWidget *, gpointer );
void     GtkNetworkOpenOk       ( GtkButton *, gpointer );
void     GtkNetworkOpenCancel   ( GtkButton *, gpointer );
void     GtkNetworkOpenBroadcast( GtkToggleButton *, gpointer );
void     GtkNetworkOpenChannel  ( GtkToggleButton *, gpointer );

