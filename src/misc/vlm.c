/*****************************************************************************
 * vlm.c: VLM interface plugin
 *****************************************************************************
 * Copyright (C) 2000, 2001 VideoLAN
 * $Id: vlm.c 9083 2004-10-30 10:36:07Z gbazin $
 *
 * Authors: Simon Latapie <garf@videolan.org>
 *          Laurent Aimar <fenrir@videolan.org>
 *          Gildas Bazin <gbazin@videolan.org>
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
#include <stdlib.h>                                      /* malloc(), free() */
#include <ctype.h>                                              /* tolower() */

#include <vlc/vlc.h>

#ifdef ENABLE_VLM

#include <vlc/intf.h>
#include <vlc/input.h>

#ifdef HAVE_TIME_H
#   include <time.h>                                              /* ctime() */
#endif

#include "vlc_vlm.h"
#include "vlc_vod.h"

/*****************************************************************************
 * Local prototypes.
 *****************************************************************************/
static char          *vlm_Save( vlm_t * );
static int            vlm_Load( vlm_t *, char *);
static vlm_message_t *vlm_Show( vlm_t *, vlm_media_t *, vlm_schedule_t *, char * );
static vlm_message_t *vlm_Help( vlm_t *, char * );

static vlm_media_t *vlm_MediaNew    ( vlm_t *, char *, int );
static void         vlm_MediaDelete ( vlm_t *, vlm_media_t *, char * );
static vlm_media_t *vlm_MediaSearch ( vlm_t *, char * );
static int          vlm_MediaSetup  ( vlm_t *, vlm_media_t *, char *, char * );
static int          vlm_MediaControl( vlm_t *, vlm_media_t *, char *, char *, char * );
static vlm_media_instance_t *vlm_MediaInstanceSearch( vlm_t *, vlm_media_t *, char * );

static vlm_message_t *vlm_MessageNew( char *, const char *, ... );
static vlm_message_t *vlm_MessageAdd( vlm_message_t*, vlm_message_t* );

static vlm_schedule_t *vlm_ScheduleNew( vlm_t *, char *);
static void            vlm_ScheduleDelete( vlm_t *, vlm_schedule_t *, char *);
static int             vlm_ScheduleSetup( vlm_schedule_t *, char *, char *);
static vlm_schedule_t *vlm_ScheduleSearch( vlm_t *, char *);

static int vlm_MediaVodControl( void *, vod_media_t *, char *, int, va_list );

static int ExecuteCommand( vlm_t *, char *, vlm_message_t **);
static int Manage( vlc_object_t* );

/*****************************************************************************
 * vlm_New:
 *****************************************************************************/
vlm_t *__vlm_New ( vlc_object_t *p_this )
{
    vlc_value_t lockval;
    vlm_t *vlm = NULL;

    /* to be sure to avoid multiple creation */
    var_Create( p_this->p_libvlc, "vlm_mutex", VLC_VAR_MUTEX );
    var_Get( p_this->p_libvlc, "vlm_mutex", &lockval );
    vlc_mutex_lock( lockval.p_address );

    if( !(vlm = vlc_object_find( p_this, VLC_OBJECT_VLM, FIND_ANYWHERE )) )
    {
        msg_Info( p_this, "creating vlm" );
        if( ( vlm = vlc_object_create( p_this, VLC_OBJECT_VLM ) ) == NULL )
        {
            vlc_mutex_unlock( lockval.p_address );
            return NULL;
        }

        vlc_mutex_init( p_this->p_vlc, &vlm->lock );
        vlm->i_media      = 0;
        vlm->media        = NULL;
        vlm->i_vod        = 0;
        vlm->i_schedule   = 0;
        vlm->schedule     = NULL;

        vlc_object_yield( vlm );
        vlc_object_attach( vlm, p_this->p_vlc );
    }
    vlc_mutex_unlock( lockval.p_address );

    if( vlc_thread_create( vlm, "vlm thread",
                           Manage, VLC_THREAD_PRIORITY_LOW, VLC_FALSE ) )
    {
        vlc_mutex_destroy( &vlm->lock );
        vlc_object_destroy( vlm );
        return NULL;
    }

    return vlm;
}

/*****************************************************************************
 * vlm_Delete:
 *****************************************************************************/
void vlm_Delete( vlm_t *vlm )
{
    vlc_value_t lockval;

    var_Get( vlm->p_libvlc, "vlm_mutex", &lockval );
    vlc_mutex_lock( lockval.p_address );

    vlc_object_release( vlm );

    if( vlm->i_refcount > 0 )
    {
        vlc_mutex_unlock( lockval.p_address );
        return;
    }

    vlm->b_die = VLC_TRUE;
    vlc_thread_join( vlm );

    vlc_mutex_destroy( &vlm->lock );

    while( vlm->i_media ) vlm_MediaDelete( vlm, vlm->media[0], NULL );
    if( vlm->media ) free( vlm->media );

    while( vlm->i_schedule ) vlm_ScheduleDelete( vlm, vlm->schedule[0], NULL );
    if( vlm->schedule ) free( vlm->schedule );

    vlc_object_detach( vlm );
    vlc_object_destroy( vlm );
    vlc_mutex_unlock( lockval.p_address );
}

/*****************************************************************************
 * vlm_ExecuteCommand:
 *****************************************************************************/
int vlm_ExecuteCommand( vlm_t *vlm, char *command, vlm_message_t **message)
{
    int result;

    vlc_mutex_lock( &vlm->lock );
    result = ExecuteCommand( vlm, command, message );
    vlc_mutex_unlock( &vlm->lock );

    return result;
}

/*****************************************************************************
 * FindEndCommand
 *****************************************************************************/
static char *FindEndCommand( char *psz )
{
    char *psz_sent = psz;

    switch( *psz_sent )
    {
    case '\"':
        psz_sent++;

        while( ( *psz_sent != '\"' ) && ( *psz_sent != '\0' ) )
        {
            if( *psz_sent == '\'' )
            {
                psz_sent = FindEndCommand( psz_sent );
                if( psz_sent == NULL ) return NULL;
            }
            else psz_sent++;
        }

        if( *psz_sent == '\"' )
        {
            psz_sent++;
            return psz_sent;
        }

        /* *psz_sent == '\0' -> number of " is incorrect */
        else return NULL;

        break;

    case '\'':
        psz_sent++;

        while( ( *psz_sent != '\'' ) && ( *psz_sent != '\0' ) )
        {
            if( *psz_sent == '\"' )
            {
                psz_sent = FindEndCommand( psz_sent );
                if( psz_sent == NULL ) return NULL;
            }
            else psz_sent++;
        }

        if( *psz_sent == '\'' )
        {
            psz_sent++;
            return psz_sent;
        }

        /* *psz_sent == '\0' -> number of " is incorrect */
        else return NULL;

        break;

    default: /* now we can look for spaces */
        while( ( *psz_sent != ' ' ) && ( *psz_sent != '\0' ) )
        {
            if( ( *psz_sent == '\'' ) || ( *psz_sent == '\"' ) )
            {
                psz_sent = FindEndCommand( psz_sent );
            }
            else psz_sent++;
        }

        return psz_sent;
    }
}

/*****************************************************************************
 * ExecuteCommand: The main state machine
 *****************************************************************************
 * Execute a command which ends with '\0' (string)
 *****************************************************************************/
static int ExecuteCommand(vlm_t *vlm, char *command, vlm_message_t **p_message)
{
    int i_command = 0;
    char **p_command = NULL;
    char *cmd = command;
    vlm_message_t *message = NULL;
    int i, j;

    /* First, parse the line and cut it */
    while( *cmd != '\0' )
    {

        if( *cmd == ' ' )
        {
            cmd++;
        }
        else
        {
            char *p_temp;
            int   i_temp;

            p_temp = FindEndCommand( cmd );

            if( p_temp == NULL ) goto error;

            i_temp = p_temp - cmd;

            p_command = realloc( p_command, (i_command + 1) * sizeof(char*) );
            p_command[ i_command ] = malloc( (i_temp + 1) * sizeof(char) );
            strncpy( p_command[ i_command ], cmd, i_temp );
            p_command[ i_command ][ i_temp ] = '\0';

            i_command++;
            cmd = p_temp;
        }
    }

    /*
     * And then Interpret it
     */

    if( i_command == 0 )
    {
        message = vlm_MessageNew( "", NULL );
        goto success;
    }

    if( strcmp(p_command[0], "new") == 0 )
    {
        int i_type;

        /* Check the number of arguments */
        if( i_command < 3 ) goto syntax_error;

        /* Get type */
        if( strcmp(p_command[2], "vod") == 0 )
        {
            i_type = VOD_TYPE;
        }
        else if( strcmp(p_command[2], "broadcast") == 0 )
        {
            i_type = BROADCAST_TYPE;
        }
        else if( strcmp(p_command[2], "schedule") == 0 )
        {
            i_type = SCHEDULE_TYPE;
        }
        else
        {
            message = vlm_MessageNew( "new", "%s: Choose between vod, "
                                      "broadcast or schedule", p_command[1] );
            goto error;
        }

        /* Check for forbidden media names */
        if( strcmp(p_command[1], "all") == 0 ||
            strcmp(p_command[1], "media") == 0 ||
            strcmp(p_command[1], "schedule") == 0 )
        {
            message = vlm_MessageNew( "new", "\"all\", \"media\" and "
                                      "\"schedule\" are reserved names" );
            goto error;
        }

        /* Check the name is not already in use */
        if( vlm_ScheduleSearch( vlm, p_command[1] ) ||
            vlm_MediaSearch( vlm, p_command[1] ) )
        {
            message = vlm_MessageNew( "new", "%s: Name already in use",
                                      p_command[1] );
            goto error;
        }

        /* Schedule */
        if( i_type == SCHEDULE_TYPE )
        {
            vlm_schedule_t *schedule;
            schedule = vlm_ScheduleNew( vlm, p_command[1] );
            if( !schedule )
            {
                message = vlm_MessageNew( "new", "could not create schedule" );
                goto error;
            }
        }

        /* Media */
        else
        {
            vlm_media_t *media;
            media = vlm_MediaNew( vlm, p_command[1], i_type );
            if( !media )
            {
                message = vlm_MessageNew( "new", "could not create media" );
                goto error;
            }
        }

        if( i_command <= 3 )
        {
            message = vlm_MessageNew( "new", NULL );
            goto success;
        }

        /* Properties will be dealt with later on */
    }

    else if( strcmp(p_command[0], "setup") == 0 )
    {
        if( i_command < 2 ) goto syntax_error;

        /* Properties will be dealt with later on */
    }
 
    else if( strcmp(p_command[0], "del") == 0 )
    {
        vlm_media_t *media;
        vlm_schedule_t *schedule;

        if( i_command < 2 ) goto syntax_error;

        media = vlm_MediaSearch( vlm, p_command[1] );
        schedule = vlm_ScheduleSearch( vlm, p_command[1] );

        if( schedule != NULL )
        {
            vlm_ScheduleDelete( vlm, schedule, NULL );
        }
        else if( media != NULL )
        {
            vlm_MediaDelete( vlm, media, NULL );
        }
        else if( strcmp(p_command[1], "media") == 0 )
        {
            while( vlm->i_media ) vlm_MediaDelete( vlm, vlm->media[0], NULL );
        }
        else if( strcmp(p_command[1], "schedule") == 0 )
        {
            while( vlm->i_schedule )
                vlm_ScheduleDelete( vlm, vlm->schedule[0], NULL );
        }
        else if( strcmp(p_command[1], "all") == 0 )
        {
            while( vlm->i_media ) vlm_MediaDelete( vlm, vlm->media[0], NULL );

            while( vlm->i_schedule )
                vlm_ScheduleDelete( vlm, vlm->schedule[0], NULL );
        }
        else
        {
            message = vlm_MessageNew( "del", "%s: media unknown",
                                      p_command[1] );
            goto error;
        }

        message = vlm_MessageNew( "del", NULL );
        goto success;
    }

    else if( strcmp(p_command[0], "show") == 0 )
    {
        vlm_media_t *media;
        vlm_schedule_t *schedule;

        if( i_command == 1 )
        {
            message = vlm_Show( vlm, NULL, NULL, NULL );
            goto success;
        }
        else if( i_command > 2 ) goto syntax_error;

        media = vlm_MediaSearch( vlm, p_command[1] );
        schedule = vlm_ScheduleSearch( vlm, p_command[1] );

        if( schedule != NULL )
        {
            message = vlm_Show( vlm, NULL, schedule, NULL );
        }
        else if( media != NULL )
        {
            message = vlm_Show( vlm, media, NULL, NULL );
        }
        else
        {
            message = vlm_Show( vlm, NULL, NULL, p_command[1] );
        }

        goto success;
    }

    else if( strcmp(p_command[0], "help") == 0 )
    {
        if( i_command != 1 ) goto syntax_error;

        message = vlm_Help( vlm, NULL );
        goto success;
    }

    else if( strcmp(p_command[0], "control") == 0 )
    {
        vlm_media_t *media;

        if( i_command < 3 ) goto syntax_error;

        if( !(media = vlm_MediaSearch( vlm, p_command[1] ) ) )
        {
            message = vlm_MessageNew( "control", "%s: media unknown",
                                      p_command[1] );
            goto error;
        }
        else
        {
            char *psz_command, *psz_arg = 0, *psz_instance = 0;
            int i_index = 2;

            if( strcmp(p_command[2], "play") && strcmp(p_command[2], "stop") &&
                strcmp(p_command[2], "pause") && strcmp(p_command[2], "seek") )
            {
                i_index++;
                psz_instance = p_command[2];

                if( i_command < 4 ) goto syntax_error;
            }

            psz_command = p_command[i_index];

            if( i_command >= i_index + 2 ) psz_arg = p_command[i_index + 1];

            vlm_MediaControl( vlm, media, psz_instance, psz_command, psz_arg );
            message = vlm_MessageNew( "control", NULL );
            goto success;
        }
    }

    else if( strcmp(p_command[0], "save") == 0 )
    {
        FILE *file;

        if( i_command != 2 ) goto syntax_error;

        file = fopen( p_command[1], "w" );
        if( file == NULL )
        {
            message = vlm_MessageNew( "save", "Unable to save file" );
            goto error;
        }
        else
        {
            char *save = vlm_Save( vlm );
            fwrite( save, strlen( save ), 1, file );
            fclose( file );
            free( save );
            message = vlm_MessageNew( "save", NULL );
            goto success;
        }
    }

    else if( strcmp(p_command[0], "load") == 0 )
    {
        FILE *file;

        if( i_command != 2 ) goto syntax_error;

        file = fopen( p_command[1], "r" );

        if( file == NULL )
        {
            message = vlm_MessageNew( "load", "Unable to load file" );
            goto error;
        }
        else
        {
            int64_t size;
            char *buffer;

            if( fseek( file, 0, SEEK_END) == 0 )
            {
                size = ftell( file );
                fseek( file, 0, SEEK_SET);
                buffer = malloc( size + 1 );
                fread( buffer, 1, size, file);
                buffer[ size ] = '\0';
                if( vlm_Load( vlm, buffer ) )
                {
                    free( buffer );
                    message = vlm_MessageNew( "load", "error while loading "
                                              "file" );
                    goto error;
                }
                free( buffer );
            }
            else
            {
                message = vlm_MessageNew( "load", "read file error" );
                goto error;
            }

            fclose( file );
            message = vlm_MessageNew( "load", NULL );
            goto success;
        }
    }

    else
    {
        message = vlm_MessageNew( p_command[0], "Unknown command" );
        goto error;
    }

    /* Common code between "new" and "setup" */
    if( strcmp(p_command[0], "new") == 0 ||
        strcmp(p_command[0], "setup") == 0 )
    {
        int i_command_start = strcmp(p_command[0], "new") ? 2 : 3;
        vlm_media_t *media;
        vlm_schedule_t *schedule;

        if( i_command < i_command_start ) goto syntax_error;

        media = vlm_MediaSearch( vlm, p_command[1] );
        schedule = vlm_ScheduleSearch( vlm, p_command[1] );

        if( !media && !schedule )
        {
            message = vlm_MessageNew( p_command[0], "%s unknown",
                                      p_command[1] );
            goto error;
        }

        if( schedule != NULL )
        {
            for( i = i_command_start ; i < i_command ; i++ )
            {
                if( strcmp( p_command[i], "enabled" ) == 0 ||
                    strcmp( p_command[i], "disabled" ) == 0 )
                {
                    vlm_ScheduleSetup( schedule, p_command[i], NULL );
                }

                /* Beware: everything behind append is considered as 
                 * command line */
                else if( strcmp( p_command[i], "append" ) == 0 )
                {
                    if( ++i >= i_command ) break;

                    for( j = i + 1; j < i_command; j++ )
                    {
                        p_command[i] =
                            realloc( p_command[i], strlen(p_command[i]) +
                                     strlen(p_command[j]) + 1 + 1 );
                        strcat( p_command[i], " " );
                        strcat( p_command[i], p_command[j] );
                    }

                    vlm_ScheduleSetup( schedule, p_command[i - 1],
                                       p_command[i] );
                    break;
                }
                else
                {
                    if( i + 1 >= i_command && !strcmp(p_command[0], "new") )
                    {
                        vlm_ScheduleDelete( vlm, schedule, NULL );
                        message = vlm_MessageNew( p_command[0],
                                                  "Wrong properties syntax" );
                        goto error;
                    }
                    else if( i + 1 >= i_command )
                    {
                        message = vlm_MessageNew( p_command[0],
                                                  "Wrong properties syntax" );
                        goto error;
                    }

                    vlm_ScheduleSetup( schedule, p_command[i],
                                       p_command[i+1] );
                    i++;
                }
            }
        }

        else if( media != NULL )
        {
            for( i = i_command_start ; i < i_command ; i++ )
            {
                if( strcmp( p_command[i], "enabled" ) == 0 ||
                    strcmp( p_command[i], "disabled" ) == 0 )
                {
                    vlm_MediaSetup( vlm, media, p_command[i], NULL );
                }
                else if( i + 1 >= i_command && !strcmp( p_command[i], "mux" ) )
                {
                    if( media->i_type != VOD_TYPE )
                    {
                        message = vlm_MessageNew( p_command[0],
                                  "mux only available for broadcast" );
                    }
                    else
                    {
                        vlm_MediaSetup( vlm, media, p_command[i],
                                        p_command[i+1] );
                        i++;
                    }
                }
                else if( strcmp( p_command[i], "loop" ) == 0 ||
                         strcmp( p_command[i], "unloop" ) == 0 )
                {
                    if( media->i_type != BROADCAST_TYPE )
                    {
                        message = vlm_MessageNew( p_command[0],
                                  "loop only available for broadcast" );
                    }
                    else
                    {
                        vlm_MediaSetup( vlm, media, p_command[i], NULL );
                    }
                }
                else
                {
                    if( i + 1 >= i_command && !strcmp(p_command[0], "new") )
                    {
                        vlm_MediaDelete( vlm, media, NULL );
                        message = vlm_MessageNew( p_command[0],
                                                  "Wrong properties syntax" );
                        goto error;
                    }
                    else if( i + 1 >= i_command )
                    {
                        message = vlm_MessageNew( p_command[0],
                                                  "Wrong properties syntax" );
                        goto error;
                    }

                    vlm_MediaSetup( vlm, media, p_command[i], p_command[i+1] );
                    i++;
                }
            }
        }

        message = vlm_MessageNew( p_command[0], NULL );
        goto success;
    }

success:
    for( i = 0 ; i < i_command ; i++ ) free( p_command[i] );
    if( p_command ) free( p_command );
    *p_message = message;

    return VLC_SUCCESS;

syntax_error:
    message = vlm_MessageNew( p_command[0], "Wrong command syntax" );

error:
    for( i = 0 ; i < i_command ; i++ ) free( p_command[i] );
    if( p_command ) free( p_command );
    *p_message = message;

    return VLC_EGENERIC;
}

static vlm_media_t *vlm_MediaSearch( vlm_t *vlm, char *psz_name )
{
    int i;

    for( i = 0; i < vlm->i_media; i++ )
    {
        if( strcmp( psz_name, vlm->media[i]->psz_name ) == 0 )
        {
            return vlm->media[i];
        }
    }

    return NULL;
}

/*****************************************************************************
 * Media handling
 *****************************************************************************/
static vlm_media_instance_t *
vlm_MediaInstanceSearch( vlm_t *vlm, vlm_media_t *media, char *psz_name )
{
    int i;

    for( i = 0; i < media->i_instance; i++ )
    {
        if( ( !psz_name && !media->instance[i]->psz_name ) ||
            ( psz_name && media->instance[i]->psz_name &&
              !strcmp( psz_name, media->instance[i]->psz_name ) ) )
        {
            return media->instance[i];
        }
    }

    return NULL;
}

static vlm_media_t *vlm_MediaNew( vlm_t *vlm, char *psz_name, int i_type )
{
    vlm_media_t *media = malloc( sizeof( vlm_media_t ) );

    /* Check if we need to load the VOD server */
    if( i_type == VOD_TYPE && !vlm->i_vod )
    {
        vlm->vod = vlc_object_create( vlm, VLC_OBJECT_VOD );
        vlc_object_attach( vlm->vod, vlm );
        vlm->vod->p_module = module_Need( vlm->vod, "vod server", 0, 0 );
        if( !vlm->vod->p_module )
        {
            msg_Err( vlm, "cannot find vod server" );
            vlc_object_detach( vlm->vod );
            vlc_object_destroy( vlm->vod );
            vlm->vod = 0;
            free( media );
            return NULL;
        }

        vlm->vod->p_data = vlm;
        vlm->vod->pf_media_control = vlm_MediaVodControl;
    }
    if( i_type == VOD_TYPE ) vlm->i_vod++;

    media->psz_name = strdup( psz_name );
    media->b_enabled = VLC_FALSE;
    media->b_loop = VLC_FALSE;
    media->vod_media = NULL;
    media->psz_vod_output = NULL;
    media->psz_mux = NULL;
    media->i_input = 0;
    media->input = NULL;
    media->psz_output = NULL;
    media->i_option = 0;
    media->option = NULL;
    media->i_type = i_type;
    media->i_instance = 0;
    media->instance = NULL;

    media->item.psz_uri = strdup( psz_name );
    vlc_input_item_Init( VLC_OBJECT(vlm), &media->item );

    TAB_APPEND( vlm->i_media, vlm->media, media );

    return media;
}

/* for now, simple delete. After, del with options (last arg) */
static void vlm_MediaDelete( vlm_t *vlm, vlm_media_t *media, char *psz_name )
{
    if( media == NULL ) return;

    while( media->i_instance )
    {
        vlm_media_instance_t *p_instance = media->instance[0];
        vlm_MediaControl( vlm, media, p_instance->psz_name, "stop", 0 );
    }

    TAB_REMOVE( vlm->i_media, vlm->media, media );

    if( media->i_type == VOD_TYPE )
    {
        vlm_MediaSetup( vlm, media, "disabled", 0 );
        vlm->i_vod--;
    }

    /* Check if we need to unload the VOD server */
    if( media->i_type == VOD_TYPE && !vlm->i_vod )
    {
        module_Unneed( vlm->vod, vlm->vod->p_module );
        vlc_object_detach( vlm->vod );
        vlc_object_destroy( vlm->vod );
        vlm->vod = 0;
    }

    if( vlm->i_media == 0 && vlm->media ) free( vlm->media );

    free( media->psz_name );

    while( media->i_input-- ) free( media->input[media->i_input] );
    if( media->input ) free( media->input );

    if( media->psz_output ) free( media->psz_output );
    if( media->psz_mux ) free( media->psz_mux );

    while( media->i_option-- ) free( media->option[media->i_option] );
    if( media->option ) free( media->option );

    vlc_input_item_Clean( &media->item );

    free( media );
}

static int vlm_MediaSetup( vlm_t *vlm, vlm_media_t *media, char *psz_cmd,
                           char *psz_value )
{
    if( strcmp( psz_cmd, "loop" ) == 0 )
    {
        media->b_loop = VLC_TRUE;
    }
    else if( strcmp( psz_cmd, "unloop" ) == 0 )
    {
        media->b_loop = VLC_FALSE;
    }
    else if( strcmp( psz_cmd, "enabled" ) == 0 )
    {
        media->b_enabled = VLC_TRUE;
    }
    else if( strcmp( psz_cmd, "disabled" ) == 0 )
    {
        media->b_enabled = VLC_FALSE;
    }
    else if( strcmp( psz_cmd, "mux" ) == 0 )
    {
        if( media->psz_mux ) free( media->psz_mux );
        media->psz_mux = NULL;
        if( psz_value ) media->psz_mux = strdup( psz_value );
    }
    else if( strcmp( psz_cmd, "input" ) == 0 )
    {
        char *input;

        if( psz_value != NULL && strlen(psz_value) > 1 &&
            ( psz_value[0] == '\'' || psz_value[0] == '\"' ) &&
            ( psz_value[ strlen(psz_value) - 1 ] == '\'' ||
              psz_value[ strlen(psz_value) - 1 ] == '\"' )  )
        {
            input = malloc( strlen(psz_value) - 1 );

            memcpy( input, psz_value + 1, strlen(psz_value) - 2 );
            input[ strlen(psz_value) - 2 ] = '\0';
        }
        else
        {
            input = strdup( psz_value );
        }

        TAB_APPEND( media->i_input, media->input, input );
    }
    else if( strcmp( psz_cmd, "output" ) == 0 )
    {
        if( media->psz_output != NULL )
        {
            free( media->psz_output );
        }
        media->psz_output = strdup( psz_value );
    }
    else if( strcmp( psz_cmd, "option" ) == 0 )
    {
        char *option;
        option = strdup( psz_value );

        TAB_APPEND( media->i_option, media->option, option );
    }
    else
    {
        return VLC_EGENERIC;
    }

    /* Check if we need to create/delete a vod media */
    if( media->i_type == VOD_TYPE )
    {
        if( !media->b_enabled && media->vod_media )
        {
            vlm->vod->pf_media_del( vlm->vod, media->vod_media );
            media->vod_media = 0;
        }
        else if( media->b_enabled && !media->vod_media && media->i_input )
        {
            /* Pre-parse the input */
            input_thread_t *p_input;
            char *psz_output;
            int i;

            vlc_input_item_Clean( &media->item );
            vlc_input_item_Init( VLC_OBJECT(vlm), &media->item );

            if( media->psz_output )
                asprintf( &psz_output, "%s:description", media->psz_output );
            else
                asprintf( &psz_output, "#description" );

            media->item.psz_uri = strdup( media->input[0] );
            media->item.ppsz_options = malloc( sizeof( char* ) );
            asprintf( &media->item.ppsz_options[0], "sout=%s", psz_output);
            media->item.i_options = 1;
            for( i = 0; i < media->i_option; i++ )
            {
                media->item.i_options++;
                media->item.ppsz_options =
                    realloc( media->item.ppsz_options,
                             media->item.i_options * sizeof( char* ) );
                media->item.ppsz_options[ media->item.i_options - 1 ] =
                    strdup( media->option[i] );
            }

            if( (p_input = input_CreateThread( vlm, &media->item ) ) )
            {
                while( !p_input->b_eof && !p_input->b_error ) msleep( 100000 );

                input_StopThread( p_input );
                input_DestroyThread( p_input );
                vlc_object_detach( p_input );
                vlc_object_destroy( p_input );
            }
            free( psz_output );

            if( media->psz_mux )
            {
                input_item_t item;
                es_format_t es, *p_es = &es;
                char fourcc[5];

                sprintf( fourcc, "%4.4s", media->psz_mux );
                fourcc[0] = tolower(fourcc[0]); fourcc[1] = tolower(fourcc[1]);
                fourcc[2] = tolower(fourcc[2]); fourcc[3] = tolower(fourcc[3]);

                item = media->item;
                item.i_es = 1;
                item.es = &p_es;
                es_format_Init( &es, VIDEO_ES, *((int *)fourcc) );

                media->vod_media =
                  vlm->vod->pf_media_new( vlm->vod, media->psz_name, &item );
                return VLC_SUCCESS;
            }

            media->vod_media =
                vlm->vod->pf_media_new( vlm->vod, media->psz_name,
                                        &media->item );
        }
    }

    return VLC_SUCCESS;
}

static int vlm_MediaControl( vlm_t *vlm, vlm_media_t *media, char *psz_id,
                             char *psz_command, char *psz_args )
{
    vlm_media_instance_t *p_instance;
    int i;

    p_instance = vlm_MediaInstanceSearch( vlm, media, psz_id );

    if( strcmp( psz_command, "play" ) == 0 && !p_instance )
    {
        if( !media->b_enabled || media->i_input == 0 ) return 0;

        if( !p_instance )
        {
            p_instance = malloc( sizeof(vlm_media_instance_t) );
            memset( p_instance, 0, sizeof(vlm_media_instance_t) );
            vlc_input_item_Init( VLC_OBJECT(vlm), &p_instance->item );
            p_instance->p_input = 0;

            if( media->psz_output != NULL || media->psz_vod_output != NULL )
            {
                p_instance->item.ppsz_options = malloc( sizeof( char* ) );
                asprintf( &p_instance->item.ppsz_options[0], "sout=%s%s%s",
                          media->psz_output ? media->psz_output : "",
                          (media->psz_output && media->psz_vod_output) ?
                          ":" : media->psz_vod_output ? "#" : "",
                          media->psz_vod_output ? media->psz_vod_output : "" );
                p_instance->item.i_options = 1;
            }

            for( i = 0; i < media->i_option; i++ )
            {
                p_instance->item.i_options++;
                p_instance->item.ppsz_options =
                    realloc( p_instance->item.ppsz_options,
                             p_instance->item.i_options * sizeof( char* ) );
                p_instance->item.ppsz_options[p_instance->item.i_options - 1] =
                    strdup( media->option[i] );
            }

            p_instance->psz_name = psz_id ? strdup(psz_id) : 0;
            TAB_APPEND( media->i_instance, media->instance, p_instance );
        }

        if( psz_args && sscanf(psz_args, "%d", &i) == 1 && i < media->i_input )
        {
            p_instance->i_index = i;
        }

        if( p_instance->item.psz_uri ) free( p_instance->item.psz_uri );
        p_instance->item.psz_uri =
            strdup( media->input[p_instance->i_index] );

        if( p_instance->p_input )
        {
            input_StopThread( p_instance->p_input );
            input_DestroyThread( p_instance->p_input );
            vlc_object_detach( p_instance->p_input );
            vlc_object_destroy( p_instance->p_input );
        }

        p_instance->p_input = input_CreateThread( vlm, &p_instance->item );
        if( !p_instance->p_input )
        {
            TAB_REMOVE( media->i_instance, media->instance, p_instance );
            vlc_input_item_Clean( &p_instance->item );
            if( p_instance->psz_name ) free( p_instance->psz_name );
        }

        return VLC_SUCCESS;
    }

    if( !p_instance ) return VLC_EGENERIC;

    if( strcmp( psz_command, "seek" ) == 0 )
    {
        vlc_value_t val;
        float f_percentage;

        if( psz_args && sscanf( psz_args, "%f", &f_percentage ) == 1 )
        {
            val.f_float = f_percentage / 100.0 ;
            var_Set( p_instance->p_input, "position", val );
            return VLC_SUCCESS;
        }
    }
    else if( strcmp( psz_command, "stop" ) == 0 )
    {
        TAB_REMOVE( media->i_instance, media->instance, p_instance );

        if( p_instance->p_input )
        {
            input_StopThread( p_instance->p_input );
            input_DestroyThread( p_instance->p_input );
            vlc_object_detach( p_instance->p_input );
            vlc_object_destroy( p_instance->p_input );
        }

        vlc_input_item_Clean( &p_instance->item );
        if( p_instance->psz_name ) free( p_instance->psz_name );
        free( p_instance );

        return VLC_SUCCESS;
    }
    else if( strcmp( psz_command, "pause" ) == 0 )
    {
        vlc_value_t val;

        if( !p_instance->p_input ) return VLC_SUCCESS;

        var_Get( p_instance->p_input, "state", &val );

        if( val.i_int == PAUSE_S ) val.i_int = PLAYING_S;
        else val.i_int = PAUSE_S;
        var_Set( p_instance->p_input, "state", val );

        return VLC_SUCCESS;
    }

    return VLC_EGENERIC;
}

/*****************************************************************************
 * Schedule handling
 *****************************************************************************/
static vlm_schedule_t *vlm_ScheduleNew( vlm_t *vlm, char *psz_name )
{
    vlm_schedule_t *sched = malloc( sizeof( vlm_schedule_t ) );

    sched->psz_name = strdup( psz_name );
    sched->b_enabled = VLC_FALSE;
    sched->i_command = 0;
    sched->command = NULL;
    sched->i_date = 0;
    sched->i_period = 0;
    sched->i_repeat = -1;

    TAB_APPEND( vlm->i_schedule, vlm->schedule, sched );

    return sched;
}

/* for now, simple delete. After, del with options (last arg) */
static void vlm_ScheduleDelete( vlm_t *vlm, vlm_schedule_t *sched,
                                char *psz_name )
{
    if( sched == NULL ) return;

    TAB_REMOVE( vlm->i_schedule, vlm->schedule, sched );

    if( vlm->i_schedule == 0 && vlm->schedule ) free( vlm->schedule );
    free( sched->psz_name );
    while( sched->i_command-- ) free( sched->command[sched->i_command] );
    free( sched );
}

static vlm_schedule_t *vlm_ScheduleSearch( vlm_t *vlm, char *psz_name )
{
    int i;

    for( i = 0; i < vlm->i_schedule; i++ )
    {
        if( strcmp( psz_name, vlm->schedule[i]->psz_name ) == 0 )
        {
            return vlm->schedule[i];
        }
    }

    return NULL;
}

/* Ok, setup schedule command will be able to support only one (argument value) at a time  */
static int vlm_ScheduleSetup( vlm_schedule_t *schedule, char *psz_cmd,
                              char *psz_value )
{
    if( strcmp( psz_cmd, "enabled" ) == 0 )
    {
        schedule->b_enabled = VLC_TRUE;
    }
    else if( strcmp( psz_cmd, "disabled" ) == 0 )
    {
        schedule->b_enabled = VLC_FALSE;
    }
#if !defined( UNDER_CE )
    else if( strcmp( psz_cmd, "date" ) == 0 )
    {
        struct tm time;
        char *p;
        time_t date;

        time.tm_sec = 0;         /* seconds */
        time.tm_min = 0;         /* minutes */
        time.tm_hour = 0;        /* hours */
        time.tm_mday = 0;        /* day of the month */
        time.tm_mon = 0;         /* month */
        time.tm_year = 0;        /* year */
        time.tm_wday = 0;        /* day of the week */
        time.tm_yday = 0;        /* day in the year */
        time.tm_isdst = 0;       /* daylight saving time */

        /* date should be year/month/day-hour:minutes:seconds */
        p = strchr( psz_value, '-' );

        if( strcmp( psz_value, "now" ) == 0 )
        {
            schedule->i_date = 0;
        }
        else if( p == NULL && sscanf( psz_value, "%d:%d:%d", &time.tm_hour, &time.tm_min, &time.tm_sec ) != 3 ) /* it must be a hour:minutes:seconds */
        {
            return 1;
        }
        else
        {
            int i,j,k;

            switch( sscanf( p + 1, "%d:%d:%d", &i, &j, &k ) )
            {
                case 1:
                    time.tm_sec = i;
                    break;
                case 2:
                    time.tm_min = i;
                    time.tm_sec = j;
                    break;
                case 3:
                    time.tm_hour = i;
                    time.tm_min = j;
                    time.tm_sec = k;
                    break;
                default:
                    return 1;
            }

            *p = '\0';

            switch( sscanf( psz_value, "%d/%d/%d", &i, &j, &k ) )
            {
                case 1:
                    time.tm_mday = i;
                    break;
                case 2:
                    time.tm_mon = i - 1;
                    time.tm_mday = j;
                    break;
                case 3:
                    time.tm_year = i - 1900;
                    time.tm_mon = j - 1;
                    time.tm_mday = k;
                    break;
                default:
                    return 1;
            }

            date = mktime( &time );
            schedule->i_date = ((mtime_t) date) * 1000000;
        }
    }
    else if( strcmp( psz_cmd, "period" ) == 0 )
    {
        struct tm time;
        char *p;
        char *psz_time = NULL, *psz_date = NULL;
        time_t date;
        int i,j,k;

        /* First, if date or period are modified, repeat should be equal to -1 */
        schedule->i_repeat = -1;

        time.tm_sec = 0;         /* seconds */
        time.tm_min = 0;         /* minutes */
        time.tm_hour = 0;        /* hours */
        time.tm_mday = 0;        /* day of the month */
        time.tm_mon = 0;         /* month */
        time.tm_year = 0;        /* year */
        time.tm_wday = 0;        /* day of the week */
        time.tm_yday = 0;        /* day in the year */
        time.tm_isdst = 0;       /* daylight saving time */

        /* date should be year/month/day-hour:minutes:seconds */
        p = strchr( psz_value, '-' );
        if( p )
        {
            psz_date = psz_value;
            psz_time = p + 1;

            *p = '\0';
        }
        else
        {
            psz_time = psz_value;
        }


        switch( sscanf( psz_time, "%d:%d:%d", &i, &j, &k ) )
        {
            case 1:
                time.tm_sec = i;
                break;
            case 2:
                time.tm_min = i;
                time.tm_sec = j;
                break;
            case 3:
                time.tm_hour = i;
                time.tm_min = j;
                time.tm_sec = k;
                break;
            default:
                return 1;
        }
        if( psz_date )
        {
            switch( sscanf( psz_date, "%d/%d/%d", &i, &j, &k ) )
            {
                case 1:
                    time.tm_mday = i;
                    break;
                case 2:
                    time.tm_mon = i;
                    time.tm_mday = j;
                    break;
                case 3:
                    time.tm_year = i;
                    time.tm_mon = j;
                    time.tm_mday = k;
                    break;
                default:
                    return 1;
            }
        }

        /* ok, that's stupid... who is going to schedule streams every 42 years ? */
        date = (((( time.tm_year * 12 + time.tm_mon ) * 30 + time.tm_mday ) * 24 + time.tm_hour ) * 60 + time.tm_min ) * 60 + time.tm_sec ;
        schedule->i_period = ((mtime_t) date) * 1000000;
    }
#endif /* UNDER_CE */
    else if( strcmp( psz_cmd, "repeat" ) == 0 )
    {
        int i;

        if( sscanf( psz_value, "%d", &i ) == 1 )
        {
            schedule->i_repeat = i;
        }
        else
        {
            return 1;
        }
    }
    else if( strcmp( psz_cmd, "append" ) == 0 )
    {
        char *command = strdup( psz_value );

        TAB_APPEND( schedule->i_command, schedule->command, command );
    }
    else
    {
        return 1;
    }
    return 0;
}

/*****************************************************************************
 * Message handling functions
 *****************************************************************************/
static vlm_message_t *vlm_MessageNew( char *psz_name,
                                      const char *psz_format, ... )
{
    vlm_message_t *p_message;
    va_list args;

    if( !psz_name ) return 0;

    p_message = malloc( sizeof(vlm_message_t) );
    p_message->psz_value = 0;

    if( psz_format )
    {
        va_start( args, psz_format );
        if( vasprintf( &p_message->psz_value, psz_format, args ) < 0 )
        {
            va_end( args );
            free( p_message );
            return 0;
        }
        va_end( args );
    }

    p_message->psz_name = strdup( psz_name );
    p_message->i_child = 0;
    p_message->child = NULL;

    return p_message;
}

void vlm_MessageDelete( vlm_message_t *p_message )
{
    if( p_message->psz_name ) free( p_message->psz_name );
    if( p_message->psz_value ) free( p_message->psz_value );
    while( p_message->i_child-- )
        vlm_MessageDelete( p_message->child[p_message->i_child] );
    if( p_message->child ) free( p_message->child );
    free( p_message );
}

/* Add a child */
static vlm_message_t *vlm_MessageAdd( vlm_message_t *p_message,
                                      vlm_message_t *p_child )
{
    if( p_message == NULL ) return NULL;

    if( p_child )
    {
        TAB_APPEND( p_message->i_child, p_message->child, p_child );
    }

    return p_child;
}

/*****************************************************************************
 * Misc utility functions
 *****************************************************************************/
static vlm_message_t *vlm_Show( vlm_t *vlm, vlm_media_t *media,
                                vlm_schedule_t *schedule, char *psz_filter )
{
    if( media != NULL )
    {
        int i;
        vlm_message_t *msg;
        vlm_message_t *msg_media;
        vlm_message_t *msg_child;

        msg = vlm_MessageNew( "show", NULL );
        msg_media = vlm_MessageAdd( msg, vlm_MessageNew( media->psz_name, 0 ));

        vlm_MessageAdd( msg_media,
                        vlm_MessageNew( "type", media->i_type == VOD_TYPE ?
                                        "vod" : "broadcast" ) );
        vlm_MessageAdd( msg_media,
                        vlm_MessageNew( "enabled", media->b_enabled ?
                                        "yes" : "no" ) );

        vlm_MessageAdd( msg_media,
                        vlm_MessageNew( "loop", media->b_loop ?
                                        "yes" : "no" ) );

        if( media->i_type == VOD_TYPE && media->psz_mux )
            vlm_MessageAdd( msg_media,
                            vlm_MessageNew( "mux", media->psz_mux ) );

        msg_child = vlm_MessageAdd( msg_media,
                                    vlm_MessageNew( "inputs", NULL ) );

        for( i = 0; i < media->i_input; i++ )
        {
            vlm_MessageAdd( msg_child,
                            vlm_MessageNew( media->input[i], NULL ) );
        }

        vlm_MessageAdd( msg_media,
                        vlm_MessageNew( "output", media->psz_output ?
                                        media->psz_output : "" ) );

        msg_child = vlm_MessageAdd( msg_media, vlm_MessageNew( "options", 0 ));

        for( i = 0; i < media->i_option; i++ )
        {
            vlm_MessageAdd( msg_child, vlm_MessageNew( media->option[i], 0 ) );
        }

        msg_child = vlm_MessageAdd( msg_media,
                                    vlm_MessageNew( "instances", NULL ) );

        for( i = 0; i < media->i_instance; i++ )
        {
            vlm_media_instance_t *p_instance = media->instance[i];
            vlc_value_t val;

            if( !p_instance->p_input ) val.i_int = END_S;
            else var_Get( p_instance->p_input, "state", &val );

            vlm_MessageAdd( msg_child,
                vlm_MessageNew( p_instance->psz_name ?
                                p_instance->psz_name : "default",
                                val.i_int == PLAYING_S ? "playing" :
                                val.i_int == PAUSE_S ? "paused" :
                                "stopped" ) );
        }

        return msg;

    }

    else if( schedule != NULL )
    {
        int i;
        vlm_message_t *msg;
        vlm_message_t *msg_schedule;
        vlm_message_t *msg_child;
        char buffer[100];

        msg = vlm_MessageNew( "show", NULL );
        msg_schedule =
            vlm_MessageAdd( msg, vlm_MessageNew( schedule->psz_name, 0 ) );

        vlm_MessageAdd( msg_schedule, vlm_MessageNew("type", "schedule") );

        vlm_MessageAdd( msg_schedule,
                        vlm_MessageNew( "enabled", schedule->b_enabled ?
                                        "yes" : "no" ) );

#if !defined( UNDER_CE )
        if( schedule->i_date != 0 )
        {
            struct tm date;
            time_t i_time = (time_t)( schedule->i_date / 1000000 );
            char *psz_date;

#ifdef HAVE_LOCALTIME_R
            localtime_r( &i_time, &date);
#else
            struct tm *p_date = localtime( &i_time );
            date = *p_date;
#endif

            asprintf( &psz_date, "%d/%d/%d-%d:%d:%d",
                      date.tm_year + 1900, date.tm_mon + 1, date.tm_mday,
                      date.tm_hour, date.tm_min, date.tm_sec );

            vlm_MessageAdd( msg_schedule,
                            vlm_MessageNew( "date", psz_date ) );
            free( psz_date );
        }
        else
        {
            vlm_MessageAdd( msg_schedule, vlm_MessageNew("date", "now") );
        }

        if( schedule->i_period != 0 )
        {
            time_t i_time = (time_t) ( schedule->i_period / 1000000 );
            struct tm date;

            date.tm_sec = (int)( i_time % 60 );
            i_time = i_time / 60;
            date.tm_min = (int)( i_time % 60 );
            i_time = i_time / 60;
            date.tm_hour = (int)( i_time % 24 );
            i_time = i_time / 24;
            date.tm_mday = (int)( i_time % 30 );
            i_time = i_time / 30;
            /* okay, okay, months are not always 30 days long */
            date.tm_mon = (int)( i_time % 12 );
            i_time = i_time / 12;
            date.tm_year = (int)i_time;

            sprintf( buffer, "%d/%d/%d-%d:%d:%d", date.tm_year, date.tm_mon,
                     date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);

            vlm_MessageAdd( msg_schedule, vlm_MessageNew("period", buffer) );
        }
        else
        {
            vlm_MessageAdd( msg_schedule, vlm_MessageNew("period", "0") );
        }
#endif /* UNDER_CE */

        sprintf( buffer, "%d", schedule->i_repeat );
        vlm_MessageAdd( msg_schedule, vlm_MessageNew( "repeat", buffer ) );

        msg_child =
            vlm_MessageAdd( msg_schedule, vlm_MessageNew("commands", 0) );

        for( i = 0; i < schedule->i_command; i++ )
        {
           vlm_MessageAdd( msg_child,
                           vlm_MessageNew( schedule->command[i], NULL ) );
        }

        return msg;

    }

    else if( psz_filter && strcmp( psz_filter, "media" ) == 0 )
    {
        int i, j;
        vlm_message_t *msg;
        vlm_message_t *msg_child;

        msg = vlm_MessageNew( "show", NULL );
        msg_child = vlm_MessageAdd( msg, vlm_MessageNew( "media", NULL ) );

        for( i = 0; i < vlm->i_media; i++ )
        {
            vlm_media_t *m = vlm->media[i];
            vlm_message_t *msg_media, *msg_instance;

            msg_media = vlm_MessageAdd( msg_child,
                                        vlm_MessageNew( m->psz_name, 0 ) );

            vlm_MessageAdd( msg_media,
                            vlm_MessageNew( "type", m->i_type == VOD_TYPE ?
                                            "vod" : "broadcast" ) );

            vlm_MessageAdd( msg_media,
                            vlm_MessageNew( "enabled", m->b_enabled ?
                                            "yes" : "no" ) );

            if( m->i_type == VOD_TYPE && m->psz_mux )
                vlm_MessageAdd( msg_media,
                                vlm_MessageNew( "mux", m->psz_mux ) );

            msg_instance = vlm_MessageAdd( msg_media,
                                           vlm_MessageNew( "instances", 0 ) );

            for( j = 0; j < m->i_instance; j++ )
            {
                vlm_media_instance_t *p_instance = m->instance[j];
                vlc_value_t val;

                if( !p_instance->p_input ) val.i_int = END_S;
                else var_Get( p_instance->p_input, "state", &val );

                vlm_MessageAdd( msg_instance,
                    vlm_MessageNew( p_instance->psz_name ?
                                    p_instance->psz_name : "default",
                                    val.i_int == PLAYING_S ? "playing" :
                                    val.i_int == PAUSE_S ? "paused" :
                                    "stopped" ) );
            }
        }

        return msg;
    }

    else if( psz_filter && strcmp( psz_filter, "schedule") == 0 )
    {
        int i;
        vlm_message_t *msg;
        vlm_message_t *msg_child;

        msg = vlm_MessageNew( "show", NULL );
        msg_child = vlm_MessageAdd( msg, vlm_MessageNew( "schedule", NULL ) );

        for( i = 0; i < vlm->i_schedule; i++ )
        {
            vlm_schedule_t *s = vlm->schedule[i];
            vlm_message_t *msg_schedule;
            mtime_t i_time, i_next_date;

            msg_schedule = vlm_MessageAdd( msg_child,
                                           vlm_MessageNew( s->psz_name, 0 ) );
            vlm_MessageAdd( msg_schedule,
                            vlm_MessageNew( "enabled", s->b_enabled ?
                                            "yes" : "no" ) );

            if( !s->b_enabled ) return msg;


            vlm_MessageAdd( msg_schedule,
                            vlm_MessageNew( "enabled", "yes" ) );

            /* calculate next date */
            i_time = mdate();
            i_next_date = s->i_date;

            if( s->i_period != 0 )
            {
                int j = 0;
                while( s->i_date + j * s->i_period <= i_time &&
                       s->i_repeat > j )
                {
                    j++;
                }

                i_next_date = s->i_date + j * s->i_period;
            }

            if( i_next_date > i_time )
            {
                time_t i_date = (time_t) (i_next_date / 1000000) ;

#if !defined( UNDER_CE )
#ifdef HAVE_CTIME_R
                char psz_date[500];
                ctime_r( &i_date, psz_date );
#else
                char *psz_date = ctime( &i_date );
#endif

                vlm_MessageAdd( msg_schedule,
                                vlm_MessageNew( "next launch", psz_date ) );
#endif
            }
        }

        return msg;
    }

    else if( psz_filter == NULL && media == NULL && schedule == NULL )
    {
        vlm_message_t *show1 = vlm_Show( vlm, NULL, NULL, "media" );
        vlm_message_t *show2 = vlm_Show( vlm, NULL, NULL, "schedule" );

        vlm_MessageAdd( show1, show2->child[0] );

        /* We must destroy the parent node "show" of show2
         * and not the children */
        free( show2->psz_name );
        free( show2 );

        return show1;
    }

    else
    {
        return vlm_MessageNew( "show", NULL );
    }
}

static vlm_message_t *vlm_Help( vlm_t *vlm, char *psz_filter )
{
    vlm_message_t *message, *message_child;

#define MessageAdd( a ) \
        vlm_MessageAdd( message, vlm_MessageNew( a, NULL ) );
#define MessageAddChild( a ) \
        vlm_MessageAdd( message_child, vlm_MessageNew( a, NULL ) );

    if( psz_filter == NULL )
    {
        message = vlm_MessageNew( "help", NULL );

        message_child = MessageAdd( "Commands Syntax:" );
        MessageAddChild( "new (name) vod|broadcast|schedule [properties]" );
        MessageAddChild( "setup (name) (properties)" );
        MessageAddChild( "show [(name)|media|schedule]" );
        MessageAddChild( "del (name)|all|media|schedule" );
        MessageAddChild( "control (name) [instance_name] (command)" );
        MessageAddChild( "save (config_file)" );
        MessageAddChild( "load (config_file)" );

        message_child = MessageAdd( "Media Proprieties Syntax:" );
        MessageAddChild( "input (input_name)" );
        MessageAddChild( "output (output_name)" );
        MessageAddChild( "enabled|disabled" );
        MessageAddChild( "loop|unloop (broadcast only)" );
        MessageAddChild( "mux (mux_name)" );

        message_child = MessageAdd( "Schedule Proprieties Syntax:" );
        MessageAddChild( "enabled|disabled" );
        MessageAddChild( "append (command_until_rest_of_the_line)" );
        MessageAddChild( "date (year)/(month)/(day)-(hour):(minutes):"
                         "(seconds)|now" );
        MessageAddChild( "period (years_aka_12_months)/(months_aka_30_days)/"
                         "(days)-(hours):(minutes):(seconds)" );
        MessageAddChild( "repeat (number_of_repetitions)" );

        message_child = MessageAdd( "Control Commands Syntax:" );
        MessageAddChild( "play" );
        MessageAddChild( "pause" );
        MessageAddChild( "stop" );
        MessageAddChild( "seek (percentage)" );

        return message;
    }

    return vlm_MessageNew( "help", NULL );
}

/*****************************************************************************
 * Config handling functions
 *****************************************************************************/
static int vlm_Load( vlm_t *vlm, char *file )
{
    char *pf = file;

    while( *pf != '\0' )
    {
        vlm_message_t *message = NULL;
        int i_temp = 0;
        int i_next;

        while( pf[i_temp] != '\n' && pf[i_temp] != '\0' && pf[i_temp] != '\r' )
        {
            i_temp++;
        }

        if( pf[i_temp] == '\r' || pf[i_temp] == '\n' )
        {
            pf[i_temp] = '\0';
            i_next = i_temp + 1;
        }
        else
        {
            i_next = i_temp;
        }

        if( ExecuteCommand( vlm, pf, &message ) )
        {
            free( message );
            return 1;
        }
        free( message );

        pf += i_next;
    }

    return 0;
}

static char *vlm_Save( vlm_t *vlm )
{
    char *save = NULL;
    char *p;
    int i,j;
    int i_length = 0;

    for( i = 0; i < vlm->i_media; i++ )
    {
        vlm_media_t *media = vlm->media[i];

        if( media->i_type == VOD_TYPE )
        {
            i_length += strlen( "new  vod " ) + strlen(media->psz_name);
        }
        else
        {
            i_length += strlen( "new  broadcast " ) + strlen(media->psz_name);
        }

        if( media->b_enabled == VLC_TRUE )
        {
            i_length += strlen( "enabled" );
        }
        else
        {
            i_length += strlen( "disabled" );
        }

        if( media->b_loop == VLC_TRUE )
        {
            i_length += strlen( " loop\n" );
        }
        else
        {
            i_length += strlen( "\n" );
        }

        for( j = 0; j < media->i_input; j++ )
        {
            i_length += strlen( "setup  input \"\"\n" ) +
                strlen( media->psz_name ) + strlen( media->input[j] );
        }

        if( media->psz_output != NULL )
        {
            i_length += strlen(media->psz_name) + strlen(media->psz_output) +
                strlen( "setup  output \n" );
        }

        for( j=0 ; j < media->i_option ; j++ )
        {
            i_length += strlen(media->psz_name) + strlen(media->option[j]) +
                strlen("setup  option \n");
        }
    }

    for( i = 0; i < vlm->i_schedule; i++ )
    {
        vlm_schedule_t *schedule = vlm->schedule[i];

        i_length += strlen( "new  schedule " ) + strlen( schedule->psz_name );

        if( schedule->b_enabled == VLC_TRUE )
        {
            i_length += strlen( "date //-:: enabled\n" ) + 14;
        }
        else
        {
            i_length += strlen( "date //-:: disabled\n" ) + 14;
        }


        if( schedule->i_period != 0 )
        {
            i_length += strlen( "setup  " ) + strlen( schedule->psz_name ) +
                strlen( "period //-::\n" ) + 14;
        }

        if( schedule->i_repeat >= 0 )
        {
            char buffer[12];

            sprintf( buffer, "%d", schedule->i_repeat );
            i_length += strlen( "setup  repeat \n" ) +
                strlen( schedule->psz_name ) + strlen( buffer );
        }
        else
        {
            i_length++;
        }

        for( j = 0; j < schedule->i_command; j++ )
        {
            i_length += strlen( "setup  append \n" ) +
                strlen( schedule->psz_name ) + strlen( schedule->command[j] );
        }

    }

    /* Don't forget the '\0' */
    i_length++;
    /* now we have the length of save */

    p = save = malloc( i_length );
    *save = '\0';

    /* finally we can write in it */
    for( i = 0; i < vlm->i_media; i++ )
    {
        vlm_media_t *media = vlm->media[i];

        if( media->i_type == VOD_TYPE )
        {
            p += sprintf( p, "new %s vod ", media->psz_name);
        }
        else
        {
            p += sprintf( p, "new %s broadcast ", media->psz_name);
        }

        if( media->b_enabled == VLC_TRUE )
        {
            p += sprintf( p, "enabled" );
        }
        else
        {
            p += sprintf( p, "disabled" );
        }

        if( media->b_loop == VLC_TRUE )
        {
            p += sprintf( p, " loop\n" );
        }
        else
        {
            p += sprintf( p, "\n" );
        }

        for( j = 0; j < media->i_input; j++ )
        {
            p += sprintf( p, "setup %s input \"%s\"\n", media->psz_name,
                          media->input[j] );
        }

        if( media->psz_output != NULL )
        {
            p += sprintf( p, "setup %s output %s\n", media->psz_name,
                          media->psz_output );
        }

        for( j = 0; j < media->i_option; j++ )
        {
            p += sprintf( p, "setup %s option %s\n", media->psz_name,
                          media->option[j] );
        }
    }

    /* and now, the schedule scripts */
#if !defined( UNDER_CE )
    for( i = 0; i < vlm->i_schedule; i++ )
    {
        vlm_schedule_t *schedule = vlm->schedule[i];
        struct tm date;
        time_t i_time = (time_t) ( schedule->i_date / 1000000 );

#ifdef HAVE_LOCALTIME_R
        localtime_r( &i_time, &date);
#else
        struct tm *p_date = localtime( &i_time );
        date = *p_date;
#endif

        p += sprintf( p, "new %s schedule ", schedule->psz_name);

        if( schedule->b_enabled == VLC_TRUE )
        {
            p += sprintf( p, "date %d/%d/%d-%d:%d:%d enabled\n",
                          date.tm_year + 1900, date.tm_mon + 1, date.tm_mday,
                          date.tm_hour, date.tm_min, date.tm_sec );
        }
        else
        {
            p += sprintf( p, "date %d/%d/%d-%d:%d:%d disabled\n",
                          date.tm_year + 1900, date.tm_mon + 1, date.tm_mday,
                          date.tm_hour, date.tm_min, date.tm_sec);
        }

        if( schedule->i_period != 0 )
        {
            p += sprintf( p, "setup %s ", schedule->psz_name );

            i_time = (time_t) ( schedule->i_period / 1000000 );

            date.tm_sec = (int)( i_time % 60 );
            i_time = i_time / 60;
            date.tm_min = (int)( i_time % 60 );
            i_time = i_time / 60;
            date.tm_hour = (int)( i_time % 24 );
            i_time = i_time / 24;
            date.tm_mday = (int)( i_time % 30 );
            i_time = i_time / 30;
            /* okay, okay, months are not always 30 days long */
            date.tm_mon = (int)( i_time % 12 );
            i_time = i_time / 12;
            date.tm_year = (int)i_time;

            p += sprintf( p, "period %d/%d/%d-%d:%d:%d\n",
                          date.tm_year, date.tm_mon, date.tm_mday,
                          date.tm_hour, date.tm_min, date.tm_sec);
        }

        if( schedule->i_repeat >= 0 )
        {
            p += sprintf( p, "setup %s repeat %d\n",
                          schedule->psz_name, schedule->i_repeat );
        }
        else
        {
            p += sprintf( p, "\n" );
        }

        for( j = 0; j < schedule->i_command; j++ )
        {
            p += sprintf( p, "setup %s append %s\n",
                          schedule->psz_name, schedule->command[j] );
        }

    }
#endif /* UNDER_CE */

    return save;
}

/*****************************************************************************
 * Manage:
 *****************************************************************************/
static int vlm_MediaVodControl( void *p_private, vod_media_t *p_vod_media,
                                char *psz_id, int i_query, va_list args )
{
    vlm_t *vlm = (vlm_t *)p_private;
    int i, i_ret = VLC_EGENERIC;

    if( !p_private || !p_vod_media ) return VLC_EGENERIC;

    vlc_mutex_lock( &vlm->lock );

    /* Find media */
    for( i = 0; i < vlm->i_media; i++ )
    {
        if( p_vod_media == vlm->media[i]->vod_media ) break;
    }

    if( i == vlm->i_media )
    {
        vlc_mutex_unlock( &vlm->lock );
        return VLC_EGENERIC;
    }

    switch( i_query )
    {
    case VOD_MEDIA_PLAY:
        vlm->media[i]->psz_vod_output = (char *)va_arg( args, char * );
        i_ret = vlm_MediaControl( vlm, vlm->media[i], psz_id, "play", 0 );
        vlm->media[i]->psz_vod_output = 0;
        break;

    case VOD_MEDIA_PAUSE:
        i_ret = vlm_MediaControl( vlm, vlm->media[i], psz_id, "pause", 0 );
        break;

    case VOD_MEDIA_STOP:
        i_ret = vlm_MediaControl( vlm, vlm->media[i], psz_id, "stop", 0 );
        break;

    default:
        break;
    }

    vlc_mutex_unlock( &vlm->lock );

    return i_ret;
}

/*****************************************************************************
 * Manage:
 *****************************************************************************/
static int Manage( vlc_object_t* p_object )
{
    vlm_t *vlm = (vlm_t*)p_object;
    int i, j;
    mtime_t i_lastcheck;
    mtime_t i_time;

    i_lastcheck = mdate();

    msleep( 100000 );

    while( !vlm->b_die )
    {
        vlc_mutex_lock( &vlm->lock );

        /* destroy the inputs that wants to die, and launch the next input */
        for( i = 0; i < vlm->i_media; i++ )
        {
            vlm_media_t *p_media = vlm->media[i];

            for( j = 0; j < p_media->i_instance; j++ )
            {
                vlm_media_instance_t *p_instance = p_media->instance[j];

                if( !p_instance->p_input ||
                    ( !p_instance->p_input->b_eof &&
                      !p_instance->p_input->b_error ) ) continue;

                input_StopThread( p_instance->p_input );
                input_DestroyThread( p_instance->p_input );
                vlc_object_detach( p_instance->p_input );
                vlc_object_destroy( p_instance->p_input );

                p_instance->i_index++;
                if( p_instance->i_index == p_media->i_input &&
                    p_media->b_loop ) p_instance->i_index = 0;

                if( p_instance->i_index < p_media->i_input )
                {
                    /* FIXME, find a way to select the right instance */
                    char buffer[12];
                    sprintf( buffer, "%d", p_instance->i_index );
                    vlm_MediaControl( vlm, p_media, p_instance->psz_name,
                                      "play", buffer );
                }
                else
                {
                    if( vlm_MediaControl( vlm, p_media, p_instance->psz_name,
                                          "stop", 0 ) == VLC_SUCCESS ) i--;
                }
            }
        }

        /* scheduling */
        i_time = mdate();

        for( i = 0; i < vlm->i_schedule; i++ )
        {
            mtime_t i_real_date = vlm->schedule[i]->i_date;

            if( vlm->schedule[i]->b_enabled == VLC_TRUE )
            {
                if( vlm->schedule[i]->i_date == 0 ) // now !
                {
                    vlm->schedule[i]->i_date = (i_time / 1000000) * 1000000 ;
                    i_real_date = i_time;
                }
                else if( vlm->schedule[i]->i_period != 0 )
                {
                    int j = 0;
                    while( vlm->schedule[i]->i_date + j *
                           vlm->schedule[i]->i_period <= i_lastcheck &&
                           ( vlm->schedule[i]->i_repeat > j ||
                             vlm->schedule[i]->i_repeat == -1 ) )
                    {
                        j++;
                    }

                    i_real_date = vlm->schedule[i]->i_date + j *
                        vlm->schedule[i]->i_period;
                }

                if( i_real_date <= i_time && i_real_date > i_lastcheck )
                {
                    for( j = 0; j < vlm->schedule[i]->i_command; j++ )
                    {
                        vlm_message_t *message = NULL;

                        ExecuteCommand( vlm, vlm->schedule[i]->command[j],
                                        &message );

                        /* for now, drop the message */
                        free( message );
                    }
                }
            }
        }

        i_lastcheck = i_time;

        vlc_mutex_unlock( &vlm->lock );

        msleep( 100000 );
    }

    return VLC_SUCCESS;
}

#else /* ENABLE_VLM */

/* We just define an empty wrapper */
vlm_t *__vlm_New( vlc_object_t *a )
{
    msg_Err( a, "VideoLAN manager support is disabled" );
    return 0;
}
void vlm_Delete( vlm_t *a ){}
int vlm_ExecuteCommand( vlm_t *a, char *b, vlm_message_t **c ){ return -1; }
void vlm_MessageDelete( vlm_message_t *a ){}

#endif /* ENABLE_VLM */
