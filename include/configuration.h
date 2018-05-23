/*****************************************************************************
 * configuration.h : configuration management module
 * This file describes the programming interface for the configuration module.
 * It includes functions allowing to declare, get or set configuration options.
 *****************************************************************************
 * Copyright (C) 1999, 2000 VideoLAN
 * $Id: configuration.h 8803 2004-09-26 12:59:50Z gbazin $
 *
 * Authors: Gildas Bazin <gbazin@videolan.org>
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
 * Macros used to build the configuration structure.
 *****************************************************************************/

/* Configuration hint types */
#define CONFIG_HINT_END                     0x0001  /* End of config */
#define CONFIG_HINT_CATEGORY                0x0002  /* Start of new category */
#define CONFIG_HINT_SUBCATEGORY             0x0003  /* Start of sub-category */
#define CONFIG_HINT_SUBCATEGORY_END         0x0004  /* End of sub-category */
#define CONFIG_HINT_USAGE                   0x0005  /* Usage information */

#define CONFIG_HINT                         0x000F

/* Configuration item types */
#define CONFIG_ITEM_STRING                  0x0010  /* String option */
#define CONFIG_ITEM_FILE                    0x0020  /* File option */
#define CONFIG_ITEM_MODULE                  0x0030  /* Module option */
#define CONFIG_ITEM_INTEGER                 0x0040  /* Integer option */
#define CONFIG_ITEM_BOOL                    0x0050  /* Bool option */
#define CONFIG_ITEM_FLOAT                   0x0060  /* Float option */
#define CONFIG_ITEM_DIRECTORY               0x0070  /* Directory option */
#define CONFIG_ITEM_KEY                     0x0080  /* Hot key option */

#define CONFIG_ITEM                         0x00F0

struct module_config_t
{
    int          i_type;                               /* Configuration type */
    char        *psz_type;                          /* Configuration subtype */
    char        *psz_name;                                    /* Option name */
    char         i_short;                      /* Optional short option name */
    char        *psz_text;      /* Short comment on the configuration option */
    char        *psz_longtext;   /* Long comment on the configuration option */
    char        *psz_value;                                  /* Option value */
    int          i_value;                                    /* Option value */
    float        f_value;                                    /* Option value */
    int         i_min;                               /* Option minimum value */
    int         i_max;                               /* Option maximum value */
    float       f_min;                               /* Option minimum value */
    float       f_max;                               /* Option maximum value */

    /* Function to call when commiting a change */
    vlc_callback_t pf_callback;
    void          *p_callback_data;

    /* Values list */
    char       **ppsz_list;        /* List of possible values for the option */
    int         *pi_list;          /* Idem for integers */
    char       **ppsz_list_text;   /* Friendly names for list values */
    int          i_list;           /* Options list size */

    /* Actions list */
    vlc_callback_t *ppf_action;    /* List of possible actions for a config */
    char           **ppsz_action_text;         /* Friendly names for actions */
    int            i_action;                            /* actions list size */

    /* Misc */
    vlc_mutex_t *p_lock;            /* Lock to use when modifying the config */
    vlc_bool_t   b_dirty;          /* Dirty flag to indicate a config change */
    vlc_bool_t   b_advanced;          /* Flag to indicate an advanced option */

    /* Original option values */
    char        *psz_value_orig;
    int          i_value_orig;
    float        f_value_orig;
};

/*****************************************************************************
 * Prototypes - these methods are used to get, set or manipulate configuration
 * data.
 *****************************************************************************/
VLC_EXPORT( int,    __config_GetType,  (vlc_object_t *, const char *) );
VLC_EXPORT( int,    __config_GetInt,   (vlc_object_t *, const char *) );
VLC_EXPORT( void,   __config_PutInt,   (vlc_object_t *, const char *, int) );
VLC_EXPORT( float,  __config_GetFloat, (vlc_object_t *, const char *) );
VLC_EXPORT( void,   __config_PutFloat, (vlc_object_t *, const char *, float) );
VLC_EXPORT( char *, __config_GetPsz,   (vlc_object_t *, const char *) );
VLC_EXPORT( void,   __config_PutPsz,   (vlc_object_t *, const char *, const char *) );

VLC_EXPORT( int,    __config_LoadCmdLine,  ( vlc_object_t *, int *, char *[], vlc_bool_t ) );
VLC_EXPORT( char *,   config_GetHomeDir,     ( void ) );
VLC_EXPORT( int,    __config_LoadConfigFile, ( vlc_object_t *, const char * ) );
VLC_EXPORT( int,    __config_SaveConfigFile, ( vlc_object_t *, const char * ) );
VLC_EXPORT( void,   __config_ResetAll, ( vlc_object_t * ) );

VLC_EXPORT( module_config_t *, config_FindConfig,( vlc_object_t *, const char * ) );
VLC_EXPORT( module_t *, config_FindModule,( vlc_object_t *, const char * ) );

VLC_EXPORT( void, config_Duplicate, ( module_t *, module_config_t * ) );
            void  config_Free       ( module_t * );

VLC_EXPORT( void, config_SetCallbacks, ( module_config_t *, module_config_t * ) );
VLC_EXPORT( void, config_UnsetCallbacks, ( module_config_t * ) );

#define config_GetType(a,b) __config_GetType(VLC_OBJECT(a),b)
#define config_GetInt(a,b) __config_GetInt(VLC_OBJECT(a),b)
#define config_PutInt(a,b,c) __config_PutInt(VLC_OBJECT(a),b,c)
#define config_GetFloat(a,b) __config_GetFloat(VLC_OBJECT(a),b)
#define config_PutFloat(a,b,c) __config_PutFloat(VLC_OBJECT(a),b,c)
#define config_GetPsz(a,b) __config_GetPsz(VLC_OBJECT(a),b)
#define config_PutPsz(a,b,c) __config_PutPsz(VLC_OBJECT(a),b,c)

#define config_LoadCmdLine(a,b,c,d) __config_LoadCmdLine(VLC_OBJECT(a),b,c,d)
#define config_LoadConfigFile(a,b) __config_LoadConfigFile(VLC_OBJECT(a),b)
#define config_SaveConfigFile(a,b) __config_SaveConfigFile(VLC_OBJECT(a),b)
#define config_ResetAll(a) __config_ResetAll(VLC_OBJECT(a))

/* internal only */
int config_CreateDir( vlc_object_t *, char * );

/*****************************************************************************
 * Macros used to build the configuration structure.
 *
 * Note that internally we support only 3 types of config data: int , float
 *   and string.
 *   The other types declared here just map to one of these 3 basic types but
 *   have the advantage of also providing very good hints to a configuration
 *   interface so as to make it more user friendly.
 * The configuration structure also includes category hints. These hints can
 *   provide a configuration interface with some very useful data and again
 *   allow for a more user friendly interface.
 *****************************************************************************/

#define add_category_hint( text, longtext, advc ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_HINT_CATEGORY, NULL, NULL, '\0', text, longtext }; p_config[ i_config ] = tmp; p_config[i_config].b_advanced = advc; }
#define add_subcategory_hint( text, longtext ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_HINT_SUBCATEGORY, NULL, NULL, '\0', text, longtext }; p_config[ i_config ] = tmp; }
#define end_subcategory_hint \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_HINT_SUBCATEGORY_END, NULL, NULL, '\0' }; p_config[ i_config ] = tmp; }
#define add_usage_hint( text ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_HINT_USAGE, NULL, NULL, '\0', text }; p_config[ i_config ] = tmp; }

#define add_string( name, psz_value, p_callback, text, longtext, advc ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_ITEM_STRING, NULL, name, '\0', text, longtext, psz_value }; tmp.b_advanced = advc; p_config[ i_config ] = tmp; p_config[ i_config ].pf_callback = p_callback; }
#define add_file( name, psz_value, p_callback, text, longtext, advc ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_ITEM_FILE, NULL, name, '\0', text, longtext, psz_value, 0, 0 }; p_config[ i_config ] = tmp; p_config[ i_config ].pf_callback = p_callback; p_config[i_config].b_advanced = advc; }
#define add_directory( name, psz_value, p_callback, text, longtext, advc ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_ITEM_DIRECTORY, NULL, name, '\0', text, longtext, psz_value, 0, 0 }; p_config[ i_config ] = tmp; p_config[ i_config ].pf_callback = p_callback; p_config[i_config].b_advanced = advc; }
#define add_module( name, psz_caps, psz_value, p_callback, text, longtext, advc ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_ITEM_MODULE, psz_caps, name, '\0', text, longtext, psz_value }; p_config[ i_config ] = tmp; p_config[ i_config ].pf_callback = p_callback; p_config[i_config].b_advanced = advc; }
#define add_integer( name, i_value, p_callback, text, longtext, advc ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_ITEM_INTEGER, NULL, name, '\0', text, longtext, NULL, i_value }; p_config[ i_config ] = tmp; p_config[ i_config ].pf_callback = p_callback; p_config[i_config].b_advanced = advc; }
#define add_key( name, i_value, p_callback, text, longtext, advc ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_ITEM_KEY, NULL, name, '\0', text, longtext, NULL, i_value }; p_config[ i_config ] = tmp; p_config[ i_config ].pf_callback = p_callback; p_config[i_config].b_advanced = advc; }
#define add_integer_with_range( name, i_value, i_min, i_max, p_callback, text, longtext, advc ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_ITEM_INTEGER, NULL, name, '\0', text, longtext, NULL, i_value, 0, i_min, i_max }; p_config[ i_config ] = tmp; p_config[ i_config ].pf_callback = p_callback; p_config[i_config].b_advanced = advc; }
#define add_float( name, f_value, p_callback, text, longtext, advc ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_ITEM_FLOAT, NULL, name, '\0', text, longtext, NULL, 0, f_value }; p_config[ i_config ] = tmp; p_config[ i_config ].pf_callback = p_callback; p_config[i_config].b_advanced = advc; }
#define add_float_with_range( name, f_value, f_min, f_max, p_callback, text, longtext, advc ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_ITEM_FLOAT, NULL, name, '\0', text, longtext, NULL, 0, f_value, 0, 0, f_min, f_max }; p_config[ i_config ] = tmp; p_config[ i_config ].pf_callback = p_callback; p_config[i_config].b_advanced = advc; }
#define add_bool( name, b_value, p_callback, text, longtext, advc ) \
    i_config++; \
    if(!(i_config%10)) p_config = (module_config_t* )realloc(p_config, \
        (i_config+11) * sizeof(module_config_t)); \
    { static module_config_t tmp = { CONFIG_ITEM_BOOL, NULL, name, '\0', text, longtext, NULL, b_value }; p_config[ i_config ] = tmp; p_config[ i_config ].pf_callback = p_callback; p_config[i_config].b_advanced = advc; }

/* Modifier macros for the config options (used for fine tuning) */
#define change_short( ch ) \
    p_config[i_config].i_short = ch;

#define change_string_list( list, list_text, list_update_func ) \
    p_config[i_config].i_list = sizeof(list)/sizeof(char *); \
    p_config[i_config].ppsz_list = list; \
    p_config[i_config].ppsz_list_text = list_text;

#define change_integer_list( list, list_text, list_update_func ) \
    p_config[i_config].i_list = sizeof(list)/sizeof(int); \
    p_config[i_config].pi_list = list; \
    p_config[i_config].ppsz_list_text = list_text;

#define change_integer_range( min, max ) \
    p_config[i_config].i_min = min; \
    p_config[i_config].i_max = max;

#define change_float_range( min, max ) \
    p_config[i_config].f_min = min; \
    p_config[i_config].f_max = max;

#define change_action_add( pf_action, action_text ) \
    if( !p_config[i_config].i_action ) \
    { p_config[i_config].ppsz_action_text = 0; \
      p_config[i_config].ppf_action = 0; } \
    p_config[i_config].ppf_action = (vlc_callback_t *) \
      realloc( p_config[i_config].ppf_action, \
      (p_config[i_config].i_action + 1) * sizeof(void *) ); \
    p_config[i_config].ppsz_action_text = (char **)\
      realloc( p_config[i_config].ppsz_action_text, \
      (p_config[i_config].i_action + 1) * sizeof(void *) ); \
    p_config[i_config].ppf_action[p_config[i_config].i_action] = pf_action; \
    p_config[i_config].ppsz_action_text[p_config[i_config].i_action] = \
      action_text; \
    p_config[i_config].i_action++;
