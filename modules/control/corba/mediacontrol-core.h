#ifndef __mediacontrol_core_h

#define __mediacontrol_core_h

#ifndef __VLC__
#define __VLC__
#endif
#include <vlc/vlc.h>

/************************************************************************
 * Position Object Manipulation
 *************************************************************************/
typedef enum  {
  mediacontrol_AbsolutePosition,
  mediacontrol_RelativePosition,
  mediacontrol_ModuloPosition
} mediacontrol_PositionOrigin;

typedef enum {
  mediacontrol_ByteCount,
  mediacontrol_SampleCount,
  mediacontrol_MediaTime
} mediacontrol_PositionKey;

typedef struct {
  mediacontrol_PositionOrigin origin;
  mediacontrol_PositionKey key;
  long value;
} mediacontrol_Position;

typedef struct {
  int  width;
  int  height;
  long type;
  long long date;
  int  size;
  char* data;
} mediacontrol_RGBPicture;

typedef struct {
  int size;
  char** data;
} mediacontrol_PlaylistSeq;

typedef struct {
  int code;
  char* message;
} mediacontrol_Exception;

/* Exception codes */
#define mediacontrol_PositionKeyNotSupported    1
#define mediacontrol_PositionOriginNotSupported 2
#define mediacontrol_InvalidPosition            3
#define mediacontrol_PlaylistException          4
#define mediacontrol_InternalException          5

typedef struct {
  vlc_object_t  *p_vlc;
  playlist_t    *p_playlist;
  intf_thread_t *p_intf;
  int           vlc_object_id;
} mediacontrol_Instance;

/* Cf stream_control.h */
enum mediacontrol_PlayerStatusList
  { 
    mediacontrol_PlayingStatus, mediacontrol_PauseStatus, 
    mediacontrol_ForwardStatus, mediacontrol_BackwardStatus, 
    mediacontrol_InitStatus,    mediacontrol_EndStatus, 
    mediacontrol_UndefinedStatus 
  };
typedef enum mediacontrol_PlayerStatusList mediacontrol_PlayerStatus;

typedef struct {
    mediacontrol_PlayerStatus streamstatus;
    char* url;         /* The URL of the current media stream */
    long long position;     /* actual location in the stream (in ms) */
    long long length;         /* total length of the stream (in ms) */
  } mediacontrol_StreamInformation;

/* Helper functions */
long long mediacontrol_unit_convert (input_thread_t *p_input,
				     mediacontrol_PositionKey from,
				     mediacontrol_PositionKey to,
				     long long value);
long long
mediacontrol_position2microsecond (input_thread_t* p_input, const mediacontrol_Position * pos);

mediacontrol_RGBPicture* mediacontrol_RGBPicture__alloc (int datasize);
void mediacontrol_RGBPicture__free (mediacontrol_RGBPicture* pic);
mediacontrol_PlaylistSeq* mediacontrol_PlaylistSeq__alloc (int size);
void mediacontrol_PlaylistSeq__free (mediacontrol_PlaylistSeq* ps);

mediacontrol_Exception* mediacontrol_exception_init(mediacontrol_Exception *exception);
void mediacontrol_exception_free(mediacontrol_Exception *exception);

mediacontrol_Instance* mediacontrol_new(char** args, mediacontrol_Exception *exception);
mediacontrol_Instance* mediacontrol_new_from_object(vlc_object_t* p_object,
						    mediacontrol_Exception *exception);
mediacontrol_Position* mediacontrol_get_media_position(mediacontrol_Instance *self,
						       mediacontrol_PositionOrigin an_origin,
						       mediacontrol_PositionKey a_key,
						       mediacontrol_Exception *exception);
void mediacontrol_set_media_position(mediacontrol_Instance *self,
				     const mediacontrol_Position* a_position, 
				     mediacontrol_Exception *exception);
void mediacontrol_start(mediacontrol_Instance *self,
			const mediacontrol_Position* a_position, 
			mediacontrol_Exception *exception);
void mediacontrol_pause(mediacontrol_Instance *self,
			const mediacontrol_Position* a_position, 
			mediacontrol_Exception *exception);
void mediacontrol_resume(mediacontrol_Instance *self,
			 const mediacontrol_Position* a_position,
			 mediacontrol_Exception *exception);
void mediacontrol_stop(mediacontrol_Instance *self,
		       const mediacontrol_Position* a_position,
		       mediacontrol_Exception *exception);
void mediacontrol_exit(mediacontrol_Instance *self);

void mediacontrol_playlist_add_item (mediacontrol_Instance *self,
				     const char* psz_file,
				     mediacontrol_Exception *exception);
void mediacontrol_playlist_clear (mediacontrol_Instance *self,
				  mediacontrol_Exception *exception);
mediacontrol_PlaylistSeq* mediacontrol_playlist_get_list (mediacontrol_Instance *self,
							  mediacontrol_Exception *exception);

mediacontrol_RGBPicture* mediacontrol_snapshot (mediacontrol_Instance *self,
						const mediacontrol_Position* a_position,
						mediacontrol_Exception *exception);
/* Return a NULL terminated list */
mediacontrol_RGBPicture** mediacontrol_all_snapshots (mediacontrol_Instance *self,
						      mediacontrol_Exception *exception);

// Displays the message string, between "begin" and "end" positions
void mediacontrol_display_text (mediacontrol_Instance *self,
				const char* message, 
				const mediacontrol_Position* begin, 
				const mediacontrol_Position* end,
				mediacontrol_Exception *exception);

mediacontrol_StreamInformation* 
mediacontrol_get_stream_information(mediacontrol_Instance* self,
				    mediacontrol_PositionKey a_key,
				    mediacontrol_Exception *exception);

unsigned short mediacontrol_sound_get_volume(mediacontrol_Instance* self,
					     mediacontrol_Exception *exception);
void mediacontrol_sound_set_volume(mediacontrol_Instance* self,
				   const unsigned short volume,
				   mediacontrol_Exception *exception);

#endif

