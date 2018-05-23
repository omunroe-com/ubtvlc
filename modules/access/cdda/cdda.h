/*****************************************************************************
 * cdda.h : CD-DA input module header for vlc using libcdio.
 *****************************************************************************
 * Copyright (C) 2003 VideoLAN
 * $Id: cdda.h 8952 2004-10-07 22:50:44Z rocky $
 *
 * Author: Rocky Bernstein <rocky@panix.com>
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

#include <vlc/input.h>
#include <cdio/cdio.h>
#include <cdio/cdtext.h>
#include "vlc_meta.h"
#include "codecs.h"

#ifdef HAVE_LIBCDDB
#include <cddb/cddb.h>
#endif

/* Frequency of sample in bits per second. */
#define CDDA_FREQUENCY_SAMPLE 44100

/*****************************************************************************
 * Debugging
 *****************************************************************************/
#define INPUT_DBG_META        1 /* Meta information */
#define INPUT_DBG_EVENT       2 /* Trace keyboard events */
#define INPUT_DBG_MRL         4 /* MRL debugging */
#define INPUT_DBG_EXT         8 /* Calls from external routines */
#define INPUT_DBG_CALL       16 /* all calls */
#define INPUT_DBG_LSN        32 /* LSN changes */
#define INPUT_DBG_SEEK       64 /* Seeks to set location */
#define INPUT_DBG_CDIO      128 /* Debugging from CDIO */
#define INPUT_DBG_CDDB      256 /* CDDB debugging  */

#define INPUT_DEBUG 1
#if INPUT_DEBUG
#define dbg_print(mask, s, args...) \
   if (p_cdda->i_debug & mask) \
     msg_Dbg(p_access, "%s: "s, __func__ , ##args)
#else
#define dbg_print(mask, s, args...)
#endif

/*****************************************************************************
 * cdda_data_t: CD audio information
 *****************************************************************************/
typedef struct cdda_data_s
{
  CdIo          *p_cdio;                   /* libcdio CD device */
  track_t        i_tracks;                 /* # of tracks */
  track_t        i_first_track;            /* # of first track */
  track_t        i_titles;                 /* # of titles in playlist */
  
  /* Current position */
  track_t        i_track;                  /* Current track */
  lsn_t          i_lsn;                    /* Current Logical Sector Number */
  lsn_t          lsn[CDIO_CD_MAX_TRACKS];  /* Track LSNs. Origin is NOT 
					      0 origin but origin of track
					      number (usually 1).
					    */
  
  int            i_blocks_per_read;        /* # blocks to get in a read */
  int            i_debug;                  /* Debugging mask */

  /* Information about CD */
  vlc_meta_t    *p_meta;
  char *         psz_mcn;                  /* Media Catalog Number */
  input_title_t *p_title[CDIO_CD_MAX_TRACKS]; /* This *is* 0 origin, not
					         track number origin */
  
  
#ifdef HAVE_LIBCDDB
  vlc_bool_t     b_cddb_enabled;      /* Use CDDB at all? */
  struct  {
    vlc_bool_t   have_info;           /* True if we have any info */
    cddb_disc_t *disc;                /* libcdio uses this to get disc
					 info */
    int          disc_length;         /* Length in frames of cd. Used
					 in CDDB lookups */
  } cddb;
#endif

  vlc_bool_t   b_cdtext_enabled;      /* Use CD-Text at all? If not,
					 cdtext_preferred is meaningless. */
  vlc_bool_t   b_cdtext_prefer;       /* Prefer CD-Text info over
					 CDDB? If no CDDB, the issue
					 is moot. */

  const cdtext_t *p_cdtext[CDIO_CD_MAX_TRACKS]; /* CD-Text info. Origin is NOT 
						   0 origin but origin of track
						   number (usually 1).
						 */

  WAVEHEADER   waveheader;            /* Wave header for the output data  */
  vlc_bool_t   b_header;
  
  input_thread_t *p_input;
  
} cdda_data_t;

/* FIXME: This variable is a hack. Would be nice to eliminate. */
extern access_t *p_cdda_input;
