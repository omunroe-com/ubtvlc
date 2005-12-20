/*****************************************************************************
 * libvlc.h: main libvlc header
 *****************************************************************************
 * Copyright (C) 1998-2002 VideoLAN
 * $Id: libvlc.h 9134 2004-11-04 16:39:40Z hartman $
 *
 * Authors: Vincent Seguin <seguin@via.ecp.fr>
 *          Samuel Hocevar <sam@zoy.org>
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

#define Nothing here, this is just to prevent update-po from being stupid
#include "vlc_keys.h"

static char *ppsz_language[] =
{ "auto", "en", "en_GB", "es", "de",
  "fr", "hu", "it", "ja", "nl", "no",
  "pl", "pt_BR", "ru", "sv" };
static char *ppsz_language_text[] =
{ N_("Auto"), N_("American"), N_("British"), N_("Spanish"), N_("German"),
  N_("French"), N_("Hungarian"), N_("Italian"), N_("Japanese"), N_("Dutch"),
  N_("Norwegian"), N_("Polish"), N_("Brazilian"), N_("Russian"),
  N_("Swedish") };

/*****************************************************************************
 * Configuration options for the main program. Each module will also separatly
 * define its own configuration options.
 * Look into configuration.h if you need to know more about the following
 * macros.
 *****************************************************************************/

#define INTF_CAT_LONGTEXT N_( \
    "These options allow you to configure the interfaces used by VLC.\n" \
    "You can select the main interface, additional " \
    "interface modules, and define various related options." )

#define INTF_TEXT N_("Interface module")
#define INTF_LONGTEXT N_( \
    "This option allows you to select the interface used by VLC.\n" \
    "The default behavior is to automatically select the best module " \
    "available.")

#define EXTRAINTF_TEXT N_("Extra interface modules")
#define EXTRAINTF_LONGTEXT N_( \
    "This option allows you to select additional interfaces used by VLC. " \
    "They will be launched in the background in addition to the default " \
    "interface. Use a comma separated list of interface modules. (common " \
    "values are logger, gestures, sap, rc, http or screensaver)")

#define VERBOSE_TEXT N_("Verbosity (0,1,2)")
#define VERBOSE_LONGTEXT N_( \
    "This options sets the verbosity level (0=only errors and " \
    "standard messages, 1=warnings, 2=debug).")

#define QUIET_TEXT N_("Be quiet")
#define QUIET_LONGTEXT N_( \
    "This options turns off all warning and information messages.")

#define LANGUAGE_TEXT N_("Language")
#define LANGUAGE_LONGTEXT N_( "This option allows you to set the language " \
    "of the interface. The system language is auto-detected if \"auto\" is " \
    "specified here." )

#define COLOR_TEXT N_("Color messages")
#define COLOR_LONGTEXT N_( \
    "When this option is turned on, the messages sent to the console will " \
    "be colorized. Your terminal needs Linux color support for this to work.")

#define ADVANCED_TEXT N_("Show advanced options")
#define ADVANCED_LONGTEXT N_( \
    "When this option is turned on, the preferences and/or interfaces  will " \
    "show all the available options, including those that most users should " \
    "never touch.")

#define AOUT_CAT_LONGTEXT N_( \
    "These options allow you to modify the behavior of the audio " \
    "subsystem, and to add audio filters which can be used for " \
    "post processing or visual effects (spectrum analyzer, etc.).\n" \
    "Enable these filters here, and configure them in the \"audio filters\" " \
    "modules section.")

#define AOUT_TEXT N_("Audio output module")
#define AOUT_LONGTEXT N_( \
    "This option allows you to select the audio output method used by VLC. " \
    "The default behavior is to automatically select the best method " \
    "available.")

#define AUDIO_TEXT N_("Enable audio")
#define AUDIO_LONGTEXT N_( \
    "You can completely disable the audio output. In this case, the audio " \
    "decoding stage will not take place, thus saving some processing power.")

#define MONO_TEXT N_("Force mono audio")
#define MONO_LONGTEXT N_("This will force a mono audio output.")

#define VOLUME_TEXT N_("Audio output volume")
#define VOLUME_LONGTEXT N_( \
    "You can set the default audio output volume here, in a range from 0 to " \
    "1024.")

#define VOLUME_SAVE_TEXT N_("Audio output saved volume")
#define VOLUME_SAVE_LONGTEXT N_( \
    "This saves the audio output volume when you select mute.")

#define AOUT_RATE_TEXT N_("Audio output frequency (Hz)")
#define AOUT_RATE_LONGTEXT N_( \
    "You can force the audio output frequency here. Common values are " \
    "-1 (default), 48000, 44100, 32000, 22050, 16000, 11025, 8000.")

#if !defined( SYS_DARWIN )
#define AOUT_RESAMP_TEXT N_("High quality audio resampling")
#define AOUT_RESAMP_LONGTEXT N_( \
    "This uses a high quality audio resampling algorithm. High quality "\
    "audio resampling can be processor intensive so you can " \
    "disable it and a cheaper resampling algorithm will be used instead.")
#endif

#define DESYNC_TEXT N_("Audio desynchronization compensation")
#define DESYNC_LONGTEXT N_( \
    "This option allows you to delay the audio output. You must give a " \
    "number of milliseconds. This can be handy if you notice a lag " \
    "between the video and the audio.")

#define MULTICHA_TEXT N_("Preferred audio output channels mode")
#define MULTICHA_LONGTEXT N_( \
    "This option allows you to set the audio output channels mode that will " \
    "be used by default when possible (ie. if your hardware supports it as " \
    "well as the audio stream being played).")

#define SPDIF_TEXT N_("Use the S/PDIF audio output when available")
#define SPDIF_LONGTEXT N_( \
    "This option allows you to use the S/PDIF audio output by default when " \
    "your hardware supports it as well as the audio stream being played.")

#define AUDIO_FILTER_TEXT N_("Audio filters")
#define AUDIO_FILTER_LONGTEXT N_( \
    "This allows you to add audio post processing filters, to modify " \
    "the sound, or audio visualization modules (spectrum analyzer, etc.).")

#define AUDIO_CHANNEL_MIXER N_("Channel mixer")
#define AUDIO_CHANNEL_MIXER_LONGTEXT N_( \
     "This allows you to choose a specific audio channel mixer. For " \
     "instance, you can use the \"headphone\" mixer that gives 5.1 feeling " \
     "with a headphone.")

#define VOUT_CAT_LONGTEXT N_( \
    "These options allow you to modify the behavior of the video output " \
    "subsystem. You can for example enable video filters (deinterlacing, " \
    "image adjusting, etc.). Enable these filters here and configure " \
    "them in the \"video filters\" modules section. You can also set many " \
    "miscellaneous video options." )

#define VOUT_TEXT N_("Video output module")
#define VOUT_LONGTEXT N_( \
    "This option allows you to select the video output method used by VLC. " \
    "The default behavior is to automatically select the best " \
    "method available.")

#define VIDEO_TEXT N_("Enable video")
#define VIDEO_LONGTEXT N_( \
    "You can completely disable the video output. In this case, the video " \
    "decoding stage will not take place, thus saving some processing power.")

#define WIDTH_TEXT N_("Video width")
#define WIDTH_LONGTEXT N_( \
    "You can enforce the video width here. By default (-1) VLC will " \
    "adapt to the video characteristics.")

#define HEIGHT_TEXT N_("Video height")
#define HEIGHT_LONGTEXT N_( \
    "You can enforce the video height here. By default (-1) VLC will " \
    "adapt to the video characteristics.")

#define VIDEOX_TEXT N_("Video x coordinate")
#define VIDEOX_LONGTEXT N_( \
    "You can enforce the position of the top left corner of the video window "\
    "here (x coordinate).")

#define VIDEOY_TEXT N_("Video y coordinate")
#define VIDEOY_LONGTEXT N_( \
    "You can enforce the position of the top left corner of the video window "\
    "here (y coordinate).")

#define VIDEO_TITLE_TEXT N_("Video title")
#define VIDEO_TITLE_LONGTEXT N_( \
    "You can specify a custom video window title here.")

#define ALIGN_TEXT N_("Video alignment")
#define ALIGN_LONGTEXT N_( \
    "You can enforce the video alignment in its window. By default (0) it " \
    "will be centered (0=center, 1=left, 2=right, 4=top, 8=bottom, you can " \
    "also use combinations of these values).")
static int pi_align_values[] = { 0, 1, 2, 4, 8, 5, 6, 9, 10 };
static char *ppsz_align_descriptions[] =
{ N_("Center"), N_("Left"), N_("Right"), N_("Top"), N_("Bottom"),
  N_("Top-Left"), N_("Top-Right"), N_("Bottom-Left"), N_("Bottom-Right") };

#define ZOOM_TEXT N_("Zoom video")
#define ZOOM_LONGTEXT N_( \
    "You can zoom the video by the specified factor.")

#define GRAYSCALE_TEXT N_("Grayscale video output")
#define GRAYSCALE_LONGTEXT N_( \
    "When enabled, the color information from the video won't be decoded " \
    "(this can also allow you to save some processing power).")

#define FULLSCREEN_TEXT N_("Fullscreen video output")
#define FULLSCREEN_LONGTEXT N_( \
    "If this option is enabled, VLC will always start a video in fullscreen " \
    "mode.")

#define OVERLAY_TEXT N_("Overlay video output")
#define OVERLAY_LONGTEXT N_( \
    "If enabled, VLC will try to take advantage of the overlay capabilities " \
    "of your graphics card (hardware acceleration).")

#define VIDEO_ON_TOP_TEXT N_("Always on top")
#define VIDEO_ON_TOP_LONGTEXT N_("Always place the video window on top of " \
    "other windows." )

#define FILTER_TEXT N_("Video filter module")
#define FILTER_LONGTEXT N_( \
    "This will allow you to add a post-processing filter to enhance the " \
    "picture quality, for instance deinterlacing, or to clone or distort " \
    "the video window.")

#define ASPECT_RATIO_TEXT N_("Source aspect ratio")
#define ASPECT_RATIO_LONGTEXT N_( \
    "This will force the source aspect ratio. For instance, some DVDs claim " \
    "to be 16:9 while they are actually 4:3. This can also be used as a " \
    "hint for VLC when a movie does not have aspect ratio information. " \
    "Accepted formats are x:y (4:3, 16:9, etc.) expressing the global image " \
    "aspect, or a float value (1.25, 1.3333, etc.) expressing pixel " \
    "squareness.")

#define INPUT_CAT_LONGTEXT N_( \
    "These options allow you to modify the behavior of the input " \
    "subsystem, such as the DVD or VCD device, the network interface " \
    "settings or the subtitle channel.")

#define CR_AVERAGE_TEXT N_("Clock reference average counter")
#define CR_AVERAGE_LONGTEXT N_( \
    "When using the PVR input (or a very irregular source), you should " \
    "set this to 10000.")

#define SERVER_PORT_TEXT N_("Server port")
#define SERVER_PORT_LONGTEXT N_( \
    "This is the port used for UDP streams. By default, we chose 1234.")

#define MTU_TEXT N_("MTU of the network interface")
#define MTU_LONGTEXT N_( \
    "This is the typical size of UDP packets that we expect. On Ethernet " \
    "it is usually 1500.")

#define IFACE_ADDR_TEXT N_("Network interface address")
#define IFACE_ADDR_LONGTEXT N_( \
    "If you have several interfaces on your machine and use the " \
    "multicast solution, you will probably have to indicate the IP address " \
    "of your multicasting interface here.")

#define TTL_TEXT N_("Time to live")
#define TTL_LONGTEXT N_( \
    "Indicate here the Time To Live of the multicast packets sent by " \
    "the stream output.")

#define INPUT_PROGRAM_TEXT N_("Choose program (SID)")
#define INPUT_PROGRAM_LONGTEXT N_( \
    "Choose the program to select by giving its Service ID.")

#define INPUT_PROGRAMS_TEXT N_("Choose programs")
#define INPUT_PROGRAMS_LONGTEXT N_( \
    "Choose the programs to select by giving a comma-separated list of " \
    "SIDs.")

#define INPUT_AUDIO_TEXT N_("Choose audio")
#define INPUT_AUDIO_LONGTEXT N_( \
    "Give the default type of audio you want to use in a DVD. " \
    "(Developers only)")

#define INPUT_CHAN_TEXT N_("Choose audio channel")
#define INPUT_CHAN_LONGTEXT N_( \
    "Give the stream number of the audio channel you want to use in a DVD " \
    "(from 0 to n).")

#define INPUT_SUB_TEXT N_("Choose subtitle track")
#define INPUT_SUB_LONGTEXT N_( \
    "Give the stream number of the subtitle channel you want to use " \
    "(from 0 to n).")

#define INPUT_REPEAT_TEXT N_("Number of time the same input will be repeated")
#define INPUT_REPEAT_LONGTEXT N_("Number of time the same input will be repeated")

#define START_TIME_TEXT N_("Input start time (seconds)")
#define START_TIME_LONGTEXT N_("Input start time (seconds)")

#define STOP_TIME_TEXT N_("Input stop time (seconds)")
#define STOP_TIME_LONGTEXT N_("Input stop time (seconds)")

#define INPUT_SLAVE_TEXT N_("Input slave (experimental)")
#define INPUT_SLAVE_LONGTEXT N_("Input slave (experimental)")

#define BOOKMARKS_TEXT N_("Bookmarks list for a stream")
#define BOOKMARKS_LONGTEXT N_("You can specify a list of bookmarks for a stream in " \
    "the form \"{name=bookmark-name,time=optional-time-offset," \
    "bytes=optional-byte-offset},{...}\"")

#define SUB_CAT_LONGTEXT N_( \
    "These options allow you to modify the behavior of the subpictures " \
    "subsystem. You can for example enable subpictures filters (logo, etc.). " \
    "Enable these filters here and configure them in the " \
    "\"subpictures filters\" modules section. You can also set many " \
    "miscellaneous subpictures options." )

#define SPUMARGIN_TEXT N_("Force SPU position")
#define SPUMARGIN_LONGTEXT N_( \
    "You can use this option to place the subtitles under the movie, " \
    "instead of over the movie. Try several positions.")

#define OSD_TEXT N_("On Screen Display")
#define OSD_LONGTEXT N_( \
    "VLC can display messages on the video. This is called OSD (On Screen " \
    "Display). You can disable this feature here.")

#define SUB_FILTER_TEXT N_("Subpictures filter module")
#define SUB_FILTER_LONGTEXT N_( \
    "This will allow you to add a subpictures filter for instance to overlay "\
    "a logo.")

#define SUB_AUTO_TEXT N_("Autodetect subtitle files")
#define SUB_AUTO_LONGTEXT \
    N_("Automatically detect a subtitle file, if no subtitle filename is " \
    "specified.")

#define SUB_FUZZY_TEXT N_("Subtitle autodetection fuzziness")
#define SUB_FUZZY_LONGTEXT \
    N_("This determines how fuzzy subtitle and movie filename matching " \
    "will be. Options are:\n" \
    "0 = no subtitles autodetected\n" \
    "1 = any subtitle file\n" \
    "2 = any subtitle file containing the movie name\n" \
    "3 = subtitle file matching the movie name with additional chars\n" \
    "4 = subtitle file matching the movie name exactly")

#define SUB_PATH_TEXT N_("Subtitle autodetection paths")
#define SUB_PATH_LONGTEXT \
    N_("Look for a subtitle file in those paths too, if your subtitle " \
    "file was not found in the current directory.")

#define SUB_FILE_TEXT N_("Use subtitle file")
#define SUB_FILE_LONGTEXT \
    N_("Load this subtitle file. To be used when autodetect cannot detect " \
    "your subtitle file.")

#define DVD_DEV_TEXT N_("DVD device")
#ifdef WIN32
#define DVD_DEV_LONGTEXT N_( \
    "This is the default DVD drive (or file) to use. Don't forget the colon " \
    "after the drive letter (eg. D:)")
#else
#define DVD_DEV_LONGTEXT N_( \
    "This is the default DVD device to use.")
#endif

#define VCD_DEV_TEXT N_("VCD device")
#ifdef HAVE_VCDX
#define VCD_DEV_LONGTEXT N_( \
    "This is the default VCD device to use. " \
    "If you don't specify anything, we'll scan for a suitable CD-ROM device." )
#else
#define VCD_DEV_LONGTEXT N_( \
    "This is the default VCD device to use." )
#endif

#define CDAUDIO_DEV_TEXT N_("Audio CD device")
#ifdef HAVE_CDDAX
#define CDAUDIO_DEV_LONGTEXT N_( \
    "This is the default Audio CD device to use. " \
    "If you don't specify anything, we'll scan for a suitable CD-ROM device." )
#else
#define CDAUDIO_DEV_LONGTEXT N_( \
    "This is the default Audio CD device to use." )
#endif

#define IPV6_TEXT N_("Force IPv6")
#define IPV6_LONGTEXT N_( \
    "If you check this box, IPv6 will be used by default for all UDP and " \
    "HTTP connections.")

#define IPV4_TEXT N_("Force IPv4")
#define IPV4_LONGTEXT N_( \
    "If you check this box, IPv4 will be used by default for all UDP and " \
    "HTTP connections.")

#define META_TITLE_TEXT N_("Title metadata")
#define META_TITLE_LONGTEXT N_( \
     "Allows you to specify a \"title\" metadata for an input.")

#define META_AUTHOR_TEXT N_("Author metadata")
#define META_AUTHOR_LONGTEXT N_( \
     "Allows you to specify an \"author\" metadata for an input.")

#define META_ARTIST_TEXT N_("Artist metadata")
#define META_ARTIST_LONGTEXT N_( \
     "Allows you to specify an \"artist\" metadata for an input.")

#define META_GENRE_TEXT N_("Genre metadata")
#define META_GENRE_LONGTEXT N_( \
     "Allows you to specify a \"genre\" metadata for an input.")

#define META_CPYR_TEXT N_("Copyright metadata")
#define META_CPYR_LONGTEXT N_( \
     "Allows you to specify a \"copyright\" metadata for an input.")

#define META_DESCR_TEXT N_("Description metadata")
#define META_DESCR_LONGTEXT N_( \
     "Allows you to specify a \"description\" metadata for an input.")

#define META_DATE_TEXT N_("Date metadata")
#define META_DATE_LONGTEXT N_( \
     "Allows you to specify a \"date\" metadata for an input.")

#define META_URL_TEXT N_("URL metadata")
#define META_URL_LONGTEXT N_( \
     "Allows you to specify a \"url\" metadata for an input.")

#define CODEC_CAT_LONGTEXT N_( \
    "This option can be used to alter the way VLC selects " \
    "its codecs (decompression methods). Only advanced users should " \
    "alter this option as it can break playback of all your streams." )

#define CODEC_TEXT N_("Preferred codecs list")
#define CODEC_LONGTEXT N_( \
    "This allows you to select a list of codecs that VLC will use in " \
    "priority. For instance, 'dummy,a52' will try the dummy and a52 codecs " \
    "before trying the other ones.")

#define ENCODER_TEXT N_("Preferred encoders list")
#define ENCODER_LONGTEXT N_( \
    "This allows you to select a list of encoders that VLC will use in " \
    "priority")

#define SOUT_CAT_LONGTEXT N_( \
    "These options allow you to set default global options for the " \
    "stream output subsystem." )

#define SOUT_TEXT N_("Choose a stream output")
#define SOUT_LONGTEXT N_( \
    "Empty if no stream output.")

#define SOUT_ALL_TEXT N_("Enable streaming of all ES")
#define SOUT_ALL_LONGTEXT N_( \
    "This allows you to stream all ES (video, audio and subtitles)")

#define SOUT_DISPLAY_TEXT N_("Display while streaming")
#define SOUT_DISPLAY_LONGTEXT N_( \
    "This allows you to play the stream while streaming it.")

#define SOUT_VIDEO_TEXT N_("Enable video stream output")
#define SOUT_VIDEO_LONGTEXT N_( \
    "This allows you to choose if the video stream should be redirected to " \
    "the stream output facility when this last one is enabled.")

#define SOUT_AUDIO_TEXT N_("Enable audio stream output")
#define SOUT_AUDIO_LONGTEXT N_( \
    "This allows you to choose if the video stream should be redirected to " \
    "the stream output facility when this last one is enabled.")

#define SOUT_KEEP_TEXT N_("Keep stream output open" )
#define SOUT_KEEP_LONGTEXT N_( \
    "This allows you to keep an unique stream output instance across " \
    "multiple playlist item (automatically insert the gather stream output " \
    "if not specified)" )

#define PACKETIZER_TEXT N_("Preferred packetizer list")
#define PACKETIZER_LONGTEXT N_( \
    "This allows you to select the order in which VLC will choose its " \
    "packetizers."  )

#define MUX_TEXT N_("Mux module")
#define MUX_LONGTEXT N_( \
    "This is a legacy entry to let you configure mux modules")

#define ACCESS_OUTPUT_TEXT N_("Access output module")
#define ACCESS_OUTPUT_LONGTEXT N_( \
    "This is a legacy entry to let you configure access output modules")

#define ANN_SAPCTRL_TEXT N_("Control SAP flow")
#define ANN_SAPCTRL_LONGTEXT N_("If this option is enabled, the flow on " \
    "the SAP multicast address will be controlled. This is needed if you " \
    "want to make announcements on the MBone" )

#define ANN_SAPINTV_TEXT N_("SAP announcement interval")
#define ANN_SAPINTV_LONGTEXT N_("When the SAP flow control is disabled, " \
    "this lets you set the fixed interval between SAP announcements" )

#define CPU_CAT_LONGTEXT N_( \
    "These options allow you to enable special CPU optimizations.\n" \
    "You should always leave all these enabled." )

#define MMX_TEXT N_("Enable CPU MMX support")
#define MMX_LONGTEXT N_( \
    "If your processor supports the MMX instructions set, VLC can take " \
    "advantage of them.")

#define THREE_DN_TEXT N_("Enable CPU 3D Now! support")
#define THREE_DN_LONGTEXT N_( \
    "If your processor supports the 3D Now! instructions set, VLC can take " \
    "advantage of them.")

#define MMXEXT_TEXT N_("Enable CPU MMX EXT support")
#define MMXEXT_LONGTEXT N_( \
    "If your processor supports the MMX EXT instructions set, VLC can take " \
    "advantage of them.")

#define SSE_TEXT N_("Enable CPU SSE support")
#define SSE_LONGTEXT N_( \
    "If your processor supports the SSE instructions set, VLC can take " \
    "advantage of them.")

#define SSE2_TEXT N_("Enable CPU SSE2 support")
#define SSE2_LONGTEXT N_( \
    "If your processor supports the SSE2 instructions set, VLC can take " \
    "advantage of them.")

#define ALTIVEC_TEXT N_("Enable CPU AltiVec support")
#define ALTIVEC_LONGTEXT N_( \
    "If your processor supports the AltiVec instructions set, VLC can take " \
    "advantage of them.")

#define PLAYLIST_CAT_LONGTEXT N_( \
     "These options define the behavior of the playlist. Some " \
     "of them can be overridden in the playlist dialog box." )

#define RANDOM_TEXT N_("Play files randomly forever")
#define RANDOM_LONGTEXT N_( \
    "When selected, VLC will randomly play files in the playlist until " \
    "interrupted.")

#define LOOP_TEXT N_("Loop playlist on end")
#define LOOP_LONGTEXT N_( \
    "If you want VLC to keep playing the playlist indefinitely then enable " \
    "this option.")

#define REPEAT_TEXT N_("Repeat the current item")
#define REPEAT_LONGTEXT N_( \
    "When this is active, VLC will keep playing the current playlist item " \
    "over and over again.")

#define PAS_TEXT N_("Play and stop")
#define PAS_LONGTEXT N_( \
    "Stop the playlist after each played playlist item. " \
    "Does advance the playlist index.")

#define MISC_CAT_LONGTEXT N_( \
    "These options allow you to select default modules. Leave these " \
    "alone unless you really know what you are doing." )

#define MEMCPY_TEXT N_("Memory copy module")
#define MEMCPY_LONGTEXT N_( \
    "You can select which memory copy module you want to use. By default " \
    "VLC will select the fastest one supported by your hardware.")

#define ACCESS_TEXT N_("Access module")
#define ACCESS_LONGTEXT N_( \
    "This is a legacy entry to let you configure access modules.")

#define DEMUX_TEXT N_("Demux module")
#define DEMUX_LONGTEXT N_( \
    "This is a legacy entry to let you configure demux modules.")

#define RT_PRIORITY_TEXT N_("Allow real-time priority")
#define RT_PRIORITY_LONGTEXT N_( \
    "Running VLC in real-time priority will allow for much more precise " \
    "scheduling and yield better, especially when streaming content. " \
    "It can however lock up your whole machine, or make it very very " \
    "slow. You should only activate this if you know what you're " \
    "doing.")

#define RT_OFFSET_TEXT N_("Adjust VLC priority")
#define RT_OFFSET_LONGTEXT N_( \
    "This option adds an offset (positive or negative) to VLC default " \
    "priorities. You can use it to tune VLC priority against other " \
    "programs, or against other VLC instances.")

#define MINIMIZE_THREADS_TEXT N_("Minimize number of threads")
#define MINIMIZE_THREADS_LONGTEXT N_( \
     "This option minimizes the number of threads needed to run VLC")

#define PLUGIN_PATH_TEXT N_("Modules search path")
#define PLUGIN_PATH_LONGTEXT N_( \
    "This option allows you to specify an additional path for VLC to look " \
    "for its modules.")

#define PLUGINS_CACHE_TEXT N_("Use a plugins cache")
#define PLUGINS_CACHE_LONGTEXT N_( \
    "This option allows you to use a plugins cache which will greatly " \
    "improve the start time of VLC.")

#define DAEMON_TEXT N_("Run as daemon process")
#define DAEMON_LONGTEXT N_( \
     "Runs VLC as a background daemon process.")

#define ONEINSTANCE_TEXT N_("Allow only one running instance")
#define ONEINSTANCE_LONGTEXT N_( \
    "Allowing only one running instance of VLC can sometimes be useful, " \
    "for instance if you associated VLC with some media types and you " \
    "don't want a new instance of VLC to be opened each time you " \
    "double-click on a file in the explorer. This option will allow you " \
    "to play the file with the already running instance or enqueue it.")

#define HPRIORITY_TEXT N_("Increase the priority of the process")
#define HPRIORITY_LONGTEXT N_( \
    "Increasing the priority of the process will very likely improve your " \
    "playing experience as it allows VLC not to be disturbed by other " \
    "applications that could otherwise take too much processor time.\n" \
    "However be advised that in certain circumstances (bugs) VLC could take " \
    "all the processor time and render the whole system unresponsive which " \
    "might require a reboot of your machine.")

#define FAST_MUTEX_TEXT N_("Fast mutex on NT/2K/XP (developers only)")
#define FAST_MUTEX_LONGTEXT N_( \
    "On Windows NT/2K/XP we use a slow mutex implementation but which " \
    "allows us to correctly implement condition variables. " \
    "You can also use the faster Win9x implementation but you might " \
    "experience problems with it.")

#define WIN9X_CV_TEXT N_("Condition variables implementation for Win9x " \
    "(developers only)")
#define WIN9X_CV_LONGTEXT N_( \
    "On Windows 9x/Me you can use a fast but incorrect condition variables " \
    "implementation (more precisely there is a possibility for a race " \
    "condition to happen). " \
    "However it is possible to use slower alternatives which are more " \
    "robust. " \
    "Currently you can choose between implementation 0 (which is the " \
    "fastest but slightly incorrect), 1 (default) and 2.")

#define HOTKEY_CAT_LONGTEXT N_( "These settings are the global VLC key " \
    "bindings, known as \"hotkeys\"." )

#define FULLSCREEN_KEY_TEXT N_("Fullscreen")
#define FULLSCREEN_KEY_LONGTEXT N_("Select the hotkey to use to swap fullscreen state.")
#define PLAY_PAUSE_KEY_TEXT N_("Play/Pause")
#define PLAY_PAUSE_KEY_LONGTEXT N_("Select the hotkey to use to swap paused state.")
#define PAUSE_KEY_TEXT N_("Pause only")
#define PAUSE_KEY_LONGTEXT N_("Select the hotkey to use to pause.")
#define PLAY_KEY_TEXT N_("Play only")
#define PLAY_KEY_LONGTEXT N_("Select the hotkey to use to play.")
#define FASTER_KEY_TEXT N_("Faster")
#define FASTER_KEY_LONGTEXT N_("Select the hotkey to use for fast forward playback.")
#define SLOWER_KEY_TEXT N_("Slower")
#define SLOWER_KEY_LONGTEXT N_("Select the hotkey to use for slow motion playback.")
#define NEXT_KEY_TEXT N_("Next")
#define NEXT_KEY_LONGTEXT N_("Select the hotkey to use to skip to the next item in the playlist.")
#define PREV_KEY_TEXT N_("Previous")
#define PREV_KEY_LONGTEXT N_("Select the hotkey to use to skip to the previous item in the playlist.")
#define STOP_KEY_TEXT N_("Stop")
#define STOP_KEY_LONGTEXT N_("Select the hotkey to stop the playback.")
#define POSITION_KEY_TEXT N_("Position")
#define POSITION_KEY_LONGTEXT N_("Select the hotkey to display the position.")

#define JB10SEC_KEY_TEXT N_("Jump 10 seconds backwards")
#define JB10SEC_KEY_LONGTEXT N_("Select the hotkey to jump 10 seconds backwards.")

#define JB1MIN_KEY_TEXT N_("Jump 1 minute backwards")
#define JB1MIN_KEY_LONGTEXT N_("Select the hotkey to jump 1 minute backwards.")
#define JB5MIN_KEY_TEXT N_("Jump 5 minutes backwards")
#define JB5MIN_KEY_LONGTEXT N_("Select the hotkey to jump 5 minutes backwards.")
#define JF10SEC_KEY_TEXT N_("Jump 10 seconds forward")
#define JF10SEC_KEY_LONGTEXT N_("Select the hotkey to jump 10 seconds forward.")

#define JF1MIN_KEY_TEXT N_("Jump 1 minute forward")
#define JF1MIN_KEY_LONGTEXT N_("Select the hotkey to jump 1 minute forward.")

#define JF5MIN_KEY_TEXT N_("Jump 5 minutes forward")
#define JF5MIN_KEY_LONGTEXT N_("Select the hotkey to jump 5 minutes forward.")

#define QUIT_KEY_TEXT N_("Quit")
#define QUIT_KEY_LONGTEXT N_("Select the hotkey to quit the application.")
#define NAV_UP_KEY_TEXT N_("Navigate up")
#define NAV_UP_KEY_LONGTEXT N_("Select the key to move the selector up in DVD menus.")
#define NAV_DOWN_KEY_TEXT N_("Navigate down")
#define NAV_DOWN_KEY_LONGTEXT N_("Select the key to move the selector down in DVD menus.")
#define NAV_LEFT_KEY_TEXT N_("Navigate left")
#define NAV_LEFT_KEY_LONGTEXT N_("Select the key to move the selector left in DVD menus.")
#define NAV_RIGHT_KEY_TEXT N_("Navigate right")
#define NAV_RIGHT_KEY_LONGTEXT N_("Select the key to move the selector right in DVD menus.")
#define NAV_ACTIVATE_KEY_TEXT N_("Activate")
#define NAV_ACTIVATE_KEY_LONGTEXT N_("Select the key to activate selected item in DVD menus.")
#define VOL_UP_KEY_TEXT N_("Volume up")
#define VOL_UP_KEY_LONGTEXT N_("Select the key to increase audio volume.")
#define VOL_DOWN_KEY_TEXT N_("Volume down")
#define VOL_DOWN_KEY_LONGTEXT N_("Select the key to decrease audio volume.")
#define VOL_MUTE_KEY_TEXT N_("Mute")
#define VOL_MUTE_KEY_LONGTEXT N_("Select the key to turn off audio volume.")
#define SUBDELAY_UP_KEY_TEXT N_("Subtitle delay up")
#define SUBDELAY_UP_KEY_LONGTEXT N_("Select the key to increase the subtitle delay.")
#define SUBDELAY_DOWN_KEY_TEXT N_("Subtitle delay down")
#define SUBDELAY_DOWN_KEY_LONGTEXT N_("Select the key to decrease the subtitle delay.")
#define PLAY_BOOKMARK1_KEY_TEXT N_("Play playlist bookmark 1")
#define PLAY_BOOKMARK2_KEY_TEXT N_("Play playlist bookmark 2")
#define PLAY_BOOKMARK3_KEY_TEXT N_("Play playlist bookmark 3")
#define PLAY_BOOKMARK4_KEY_TEXT N_("Play playlist bookmark 4")
#define PLAY_BOOKMARK5_KEY_TEXT N_("Play playlist bookmark 5")
#define PLAY_BOOKMARK6_KEY_TEXT N_("Play playlist bookmark 6")
#define PLAY_BOOKMARK7_KEY_TEXT N_("Play playlist bookmark 7")
#define PLAY_BOOKMARK8_KEY_TEXT N_("Play playlist bookmark 8")
#define PLAY_BOOKMARK9_KEY_TEXT N_("Play playlist bookmark 9")
#define PLAY_BOOKMARK10_KEY_TEXT N_("Play playlist bookmark 10")
#define PLAY_BOOKMARK_KEY_LONGTEXT N_("Select the key to play this bookmark.")
#define SET_BOOKMARK1_KEY_TEXT N_("Set playlist bookmark 1")
#define SET_BOOKMARK2_KEY_TEXT N_("Set playlist bookmark 2")
#define SET_BOOKMARK3_KEY_TEXT N_("Set playlist bookmark 3")
#define SET_BOOKMARK4_KEY_TEXT N_("Set playlist bookmark 4")
#define SET_BOOKMARK5_KEY_TEXT N_("Set playlist bookmark 5")
#define SET_BOOKMARK6_KEY_TEXT N_("Set playlist bookmark 6")
#define SET_BOOKMARK7_KEY_TEXT N_("Set playlist bookmark 7")
#define SET_BOOKMARK8_KEY_TEXT N_("Set playlist bookmark 8")
#define SET_BOOKMARK9_KEY_TEXT N_("Set playlist bookmark 9")
#define SET_BOOKMARK10_KEY_TEXT N_("Set playlist bookmark 10")
#define SET_BOOKMARK_KEY_LONGTEXT N_("Select the key to set this playlist bookmark.")

#define HISTORY_BACK_TEXT N_("Go back in browsing history")
#define HISTORY_BACK_LONGTEXT N_("Select the key to go back (to the previous media item) in the browsing history.")
#define HISTORY_FORWARD_TEXT N_("Go forward in browsing history")
#define HISTORY_FORWARD_LONGTEXT N_("Select the key to go forward (to the next media item) in the browsing history.")

#define AUDIO_TRACK_KEY_TEXT N_("Cycle audio track")
#define AUDIO_TRACK_KEY_LONGTEXT N_("Cycle through the available audio tracks(languages)")
#define SUBTITLE_TRACK_KEY_TEXT N_("Cycle subtitle track")
#define SUBTITLE_TRACK_KEY_LONGTEXT N_("Cycle through the available subtitle tracks")
#define INTF_SHOW_KEY_TEXT N_("Show interface")
#define INTF_SHOW_KEY_LONGTEXT N_("Raise the interface above all other windows")

#define PLAYLIST_USAGE N_( \
    "\nPlaylist MRL syntax:" \
    "\n  URL[@[title][:chapter][-[title][:chapter]]] [:option=value]" \
    "\nURL syntax:" \
    "\n  [file://]filename              plain media file" \
    "\n  http://ip:port/file            HTTP URL" \
    "\n  ftp://ip:port/file             FTP URL" \
    "\n  mms://ip:port/file             MMS URL" \
    "\n  screen://                      Screen capture" \
    "\n  [dvd://][device][@raw_device]  DVD device" \
    "\n  [vcd://][device]               VCD device" \
    "\n  [cdda://][device]              Audio CD device" \
    "\n  udp:[[<source address>]@[<bind address>][:<bind port>]]" \
    "\n                                 UDP stream sent by a streaming server"\
    "\n  vlc:pause                      pause execution of playlist items" \
    "\n  vlc:quit                       quit VLC" \
    "\n")


/*
 * Quick usage guide for the configuration options:
 *
 * add_category_hint( N_(text), N_(longtext), b_advanced_option );
 * add_subcategory_hint( N_(text), N_(longtext), b_advanced_option );
 * add_usage_hint( N_(text), b_advanced_option );
 * add_string( option_name, value, p_callback, N_(text), N_(longtext),
               b_advanced_option );
 * add_file( option_name, psz_value, p_callback, N_(text), N_(longtext) );
 * add_module( option_name, psz_value, i_capability, p_callback,
 *             N_(text), N_(longtext) );
 * add_integer( option_name, i_value, p_callback, N_(text), N_(longtext),
                b_advanced_option );
 * add_bool( option_name, b_value, p_callback, N_(text), N_(longtext), 
             b_advanced_option );
 */

vlc_module_begin();
    /* Interface options */
    add_category_hint( N_("Interface"), INTF_CAT_LONGTEXT , VLC_FALSE );
    add_module( "intf", "interface", NULL, NULL, INTF_TEXT,
                INTF_LONGTEXT, VLC_FALSE );
        change_short('I');
    add_string( "extraintf", NULL, NULL, EXTRAINTF_TEXT,
                     EXTRAINTF_LONGTEXT, VLC_FALSE );
    add_integer( "verbose", 0, NULL, VERBOSE_TEXT, VERBOSE_LONGTEXT,
                 VLC_FALSE );
        change_short('v');
    add_bool( "quiet", 0, NULL, QUIET_TEXT, QUIET_LONGTEXT, VLC_TRUE );
        change_short('q');
    add_string( "language", "auto", NULL, LANGUAGE_TEXT, LANGUAGE_LONGTEXT,
                VLC_FALSE );
        change_string_list( ppsz_language, ppsz_language_text, 0 );
    add_bool( "color", 0, NULL, COLOR_TEXT, COLOR_LONGTEXT, VLC_TRUE );
    add_bool( "advanced", 0, NULL, ADVANCED_TEXT,
                            ADVANCED_LONGTEXT, VLC_FALSE );

    /* Audio options */
    add_category_hint( N_("Audio"), AOUT_CAT_LONGTEXT , VLC_FALSE );
    add_module( "aout", "audio output", NULL, NULL, AOUT_TEXT, AOUT_LONGTEXT,
                VLC_TRUE );
        change_short('A');
    add_bool( "audio", 1, NULL, AUDIO_TEXT, AUDIO_LONGTEXT, VLC_FALSE );
    add_integer_with_range( "volume", AOUT_VOLUME_DEFAULT, AOUT_VOLUME_MIN,
                            AOUT_VOLUME_MAX, NULL, VOLUME_TEXT,
                            VOLUME_LONGTEXT, VLC_FALSE );
    add_integer_with_range( "saved-volume", AOUT_VOLUME_DEFAULT,
                            AOUT_VOLUME_MIN, AOUT_VOLUME_MAX, NULL,
                            VOLUME_SAVE_TEXT, VOLUME_SAVE_LONGTEXT, VLC_TRUE );
    add_integer( "aout-rate", -1, NULL, AOUT_RATE_TEXT,
                 AOUT_RATE_LONGTEXT, VLC_TRUE );
#if !defined( SYS_DARWIN )
    add_bool( "hq-resampling", 1, NULL, AOUT_RESAMP_TEXT,
              AOUT_RESAMP_LONGTEXT, VLC_TRUE );
#endif
    add_bool( "spdif", 0, NULL, SPDIF_TEXT, SPDIF_LONGTEXT, VLC_FALSE );
    add_integer( "audio-desync", 0, NULL, DESYNC_TEXT,
                 DESYNC_LONGTEXT, VLC_TRUE );
    add_string( "audio-filter", 0, NULL,AUDIO_FILTER_TEXT,
                AUDIO_FILTER_LONGTEXT, VLC_FALSE );
    add_module( "audio-channel-mixer", "audio filter", NULL, NULL,
                AUDIO_CHANNEL_MIXER, AUDIO_CHANNEL_MIXER_LONGTEXT, VLC_FALSE );

    /* Video options */
    add_category_hint( N_("Video"), VOUT_CAT_LONGTEXT , VLC_FALSE );
    add_module( "vout", "video output", NULL, NULL, VOUT_TEXT, VOUT_LONGTEXT,
                VLC_TRUE );
        change_short('V');
    add_bool( "video", 1, NULL, VIDEO_TEXT, VIDEO_LONGTEXT, VLC_TRUE );
    add_integer( "width", -1, NULL, WIDTH_TEXT, WIDTH_LONGTEXT, VLC_TRUE );
    add_integer( "height", -1, NULL, HEIGHT_TEXT, HEIGHT_LONGTEXT, VLC_TRUE );
    add_integer( "video-x", -1, NULL, VIDEOX_TEXT, VIDEOX_LONGTEXT, VLC_TRUE );
    add_integer( "video-y", -1, NULL, VIDEOY_TEXT, VIDEOY_LONGTEXT, VLC_TRUE );
    add_string( "video-title", NULL, NULL, VIDEO_TITLE_TEXT,
                 VIDEO_TITLE_LONGTEXT, VLC_TRUE );
    add_integer( "align", 0, NULL, ALIGN_TEXT, ALIGN_LONGTEXT, VLC_TRUE );
        change_integer_list( pi_align_values, ppsz_align_descriptions, 0 );
    add_float( "zoom", 1, NULL, ZOOM_TEXT, ZOOM_LONGTEXT, VLC_TRUE );
    add_bool( "grayscale", 0, NULL, GRAYSCALE_TEXT,
              GRAYSCALE_LONGTEXT, VLC_TRUE );
    add_bool( "fullscreen", 0, NULL, FULLSCREEN_TEXT,
              FULLSCREEN_LONGTEXT, VLC_FALSE );
        change_short('f');
#ifndef SYS_DARWIN
    add_bool( "overlay", 1, NULL, OVERLAY_TEXT, OVERLAY_LONGTEXT, VLC_TRUE );
#endif

    add_bool( "video-on-top", 0, NULL, VIDEO_ON_TOP_TEXT,
              VIDEO_ON_TOP_LONGTEXT, VLC_FALSE );
    add_module( "filter", "video filter", NULL, NULL,
                FILTER_TEXT, FILTER_LONGTEXT, VLC_FALSE );
    add_string( "aspect-ratio", "", NULL,
                ASPECT_RATIO_TEXT, ASPECT_RATIO_LONGTEXT, VLC_TRUE );
#if 0
    add_string( "pixel-ratio", "1", NULL, PIXEL_RATIO_TEXT, PIXEL_RATIO_TEXT );
#endif

    /* Subpictures options */
    add_category_hint( N_("Subpictures"), SUB_CAT_LONGTEXT , VLC_FALSE );
    add_bool( "osd", 1, NULL, OSD_TEXT, OSD_LONGTEXT, VLC_FALSE );
    add_bool( "sub-autodetect-file", VLC_TRUE, NULL,
                 SUB_AUTO_TEXT, SUB_AUTO_LONGTEXT, VLC_FALSE );
    add_integer( "sub-autodetect-fuzzy", 3, NULL,
                 SUB_FUZZY_TEXT, SUB_FUZZY_LONGTEXT, VLC_TRUE );
#ifdef WIN32
#   define SUB_PATH ".\\subtitles"
#else
#   define SUB_PATH "./Subtitles, ./subtitles"
#endif
    add_string( "sub-autodetect-path", SUB_PATH, NULL,
                 SUB_PATH_TEXT, SUB_PATH_LONGTEXT, VLC_TRUE );
    add_file( "sub-file", NULL, NULL, SUB_FILE_TEXT,
              SUB_FILE_LONGTEXT, VLC_TRUE );
    add_integer( "spumargin", -1, NULL, SPUMARGIN_TEXT,
                 SPUMARGIN_LONGTEXT, VLC_TRUE );
    add_module( "sub-filter", "sub filter", NULL, NULL,
                SUB_FILTER_TEXT, SUB_FILTER_LONGTEXT, VLC_TRUE );

    /* Input options */
    add_category_hint( N_("Input"), INPUT_CAT_LONGTEXT , VLC_FALSE );
    add_integer( "cr-average", 40, NULL, CR_AVERAGE_TEXT,
                 CR_AVERAGE_LONGTEXT, VLC_FALSE );
    add_integer( "server-port", 1234, NULL,
                 SERVER_PORT_TEXT, SERVER_PORT_LONGTEXT, VLC_FALSE );
    add_integer( "mtu", 1500, NULL, MTU_TEXT, MTU_LONGTEXT, VLC_TRUE );
    add_string( "iface-addr", "", NULL, IFACE_ADDR_TEXT,
                IFACE_ADDR_LONGTEXT, VLC_TRUE );

    add_integer( "program", 0, NULL,
                 INPUT_PROGRAM_TEXT, INPUT_PROGRAM_LONGTEXT, VLC_TRUE );
    add_string( "programs", "", NULL,
                INPUT_PROGRAMS_TEXT, INPUT_PROGRAMS_LONGTEXT, VLC_FALSE );
    add_integer( "audio-type", -1, NULL,
                 INPUT_AUDIO_TEXT, INPUT_AUDIO_LONGTEXT, VLC_TRUE );
    add_integer( "audio-channel", -1, NULL,
                 INPUT_CHAN_TEXT, INPUT_CHAN_LONGTEXT, VLC_FALSE );
    add_integer( "spu-channel", -1, NULL,
                 INPUT_SUB_TEXT, INPUT_SUB_LONGTEXT, VLC_FALSE );
    add_integer( "input-repeat", 0, NULL,
                 INPUT_REPEAT_TEXT, INPUT_REPEAT_LONGTEXT, VLC_TRUE );
    add_integer( "start-time", 0, NULL,
                 START_TIME_TEXT, START_TIME_LONGTEXT, VLC_TRUE );
    add_integer( "stop-time", 0, NULL,
                 STOP_TIME_TEXT, STOP_TIME_LONGTEXT, VLC_TRUE );
    add_string( "input-slave", NULL, NULL,
                 INPUT_SLAVE_TEXT, INPUT_SLAVE_LONGTEXT, VLC_TRUE );

    add_string( "bookmarks", NULL, NULL,
                 BOOKMARKS_TEXT, BOOKMARKS_LONGTEXT, VLC_TRUE );

    add_file( "dvd", DVD_DEVICE, NULL, DVD_DEV_TEXT, DVD_DEV_LONGTEXT,
              VLC_FALSE );
    add_file( "vcd", VCD_DEVICE, NULL, VCD_DEV_TEXT, VCD_DEV_LONGTEXT,
              VLC_FALSE );
    add_file( "cd-audio", CDAUDIO_DEVICE, NULL, CDAUDIO_DEV_TEXT,
              CDAUDIO_DEV_LONGTEXT, VLC_FALSE );

    add_bool( "ipv6", 0, NULL, IPV6_TEXT, IPV6_LONGTEXT, VLC_FALSE );
        change_short('6');
    add_bool( "ipv4", 0, NULL, IPV4_TEXT, IPV4_LONGTEXT, VLC_FALSE );
        change_short('4');

    add_string( "meta-title", NULL, NULL, META_TITLE_TEXT,
                META_TITLE_LONGTEXT, VLC_TRUE );
    add_string( "meta-author", NULL, NULL, META_AUTHOR_TEXT,
                META_AUTHOR_LONGTEXT, VLC_TRUE );
    add_string( "meta-artist", NULL, NULL, META_ARTIST_TEXT,
                META_ARTIST_LONGTEXT, VLC_TRUE );
    add_string( "meta-genre", NULL, NULL, META_GENRE_TEXT,
                META_GENRE_LONGTEXT, VLC_TRUE );
    add_string( "meta-copyright", NULL, NULL, META_CPYR_TEXT,
                META_CPYR_LONGTEXT, VLC_TRUE );
    add_string( "meta-description", NULL, NULL, META_DESCR_TEXT,
                META_DESCR_LONGTEXT, VLC_TRUE );
    add_string( "meta-date", NULL, NULL, META_DATE_TEXT,
                META_DATE_LONGTEXT, VLC_TRUE );
    add_string( "meta-url", NULL, NULL, META_URL_TEXT,
                META_URL_LONGTEXT, VLC_TRUE );

    /* Decoder options */
    add_category_hint( N_("Decoders"), CODEC_CAT_LONGTEXT , VLC_TRUE );
    add_module( "codec", "decoder", NULL, NULL, CODEC_TEXT,
                CODEC_LONGTEXT, VLC_TRUE );
    add_module( "encoder", "encoder", NULL, NULL, ENCODER_TEXT,
                ENCODER_LONGTEXT, VLC_TRUE );


    /* Stream output options */
    add_category_hint( N_("Stream output"), SOUT_CAT_LONGTEXT , VLC_TRUE );
    add_string( "sout", NULL, NULL, SOUT_TEXT, SOUT_LONGTEXT, VLC_TRUE );
    add_bool( "sout-display", VLC_FALSE, NULL, SOUT_DISPLAY_TEXT,
                                SOUT_DISPLAY_LONGTEXT, VLC_TRUE );
    add_bool( "sout-keep", VLC_FALSE, NULL, SOUT_KEEP_TEXT,
                                SOUT_KEEP_LONGTEXT, VLC_TRUE );
    add_bool( "sout-all", 0, NULL, SOUT_ALL_TEXT,
                                SOUT_ALL_LONGTEXT, VLC_TRUE );
    add_bool( "sout-audio", 1, NULL, SOUT_AUDIO_TEXT,
                                SOUT_AUDIO_LONGTEXT, VLC_TRUE );
    add_bool( "sout-video", 1, NULL, SOUT_VIDEO_TEXT,
                                SOUT_VIDEO_LONGTEXT, VLC_TRUE );

    add_module( "packetizer", "packetizer", NULL, NULL,
                PACKETIZER_TEXT, PACKETIZER_LONGTEXT, VLC_TRUE );
    add_module( "mux", "sout mux", NULL, NULL, MUX_TEXT,
                                MUX_LONGTEXT, VLC_TRUE );
    add_module( "access_output", "sout access", NULL, NULL,
                ACCESS_OUTPUT_TEXT, ACCESS_OUTPUT_LONGTEXT, VLC_TRUE );
    add_integer( "ttl", 1, NULL, TTL_TEXT, TTL_LONGTEXT, VLC_TRUE );

    add_bool( "sap-flow-control", VLC_FALSE, NULL, ANN_SAPCTRL_TEXT,
                               ANN_SAPCTRL_LONGTEXT, VLC_TRUE );
    add_integer( "sap-interval", 5, NULL, ANN_SAPINTV_TEXT,
                               ANN_SAPINTV_LONGTEXT, VLC_TRUE );

    /* CPU options */
    add_category_hint( N_("CPU"), CPU_CAT_LONGTEXT, VLC_TRUE );
#if defined( __i386__ )
    add_bool( "mmx", 1, NULL, MMX_TEXT, MMX_LONGTEXT, VLC_TRUE );
    add_bool( "3dn", 1, NULL, THREE_DN_TEXT, THREE_DN_LONGTEXT, VLC_TRUE );
    add_bool( "mmxext", 1, NULL, MMXEXT_TEXT, MMXEXT_LONGTEXT, VLC_TRUE );
    add_bool( "sse", 1, NULL, SSE_TEXT, SSE_LONGTEXT, VLC_TRUE );
    add_bool( "sse2", 1, NULL, SSE2_TEXT, SSE2_LONGTEXT, VLC_TRUE );
#endif
#if defined( __powerpc__ ) || defined( SYS_DARWIN )
    add_bool( "altivec", 1, NULL, ALTIVEC_TEXT, ALTIVEC_LONGTEXT, VLC_TRUE );
#endif

    /* Playlist options */
    add_category_hint( N_("Playlist"), PLAYLIST_CAT_LONGTEXT , VLC_FALSE );
    add_bool( "random", 0, NULL, RANDOM_TEXT, RANDOM_LONGTEXT, VLC_FALSE );
        change_short('Z');
    add_bool( "loop", 0, NULL, LOOP_TEXT, LOOP_LONGTEXT, VLC_FALSE );
        change_short('L');
    add_bool( "repeat", 0, NULL, REPEAT_TEXT, REPEAT_LONGTEXT, VLC_TRUE );
        change_short('R');
    add_bool( "play-and-stop", 0, NULL, PAS_TEXT, PAS_LONGTEXT, VLC_TRUE );

    /* Misc options */
    add_category_hint( N_("Miscellaneous"), MISC_CAT_LONGTEXT, VLC_TRUE );
    add_module( "memcpy", "memcpy", NULL, NULL, MEMCPY_TEXT,
                MEMCPY_LONGTEXT, VLC_TRUE );
    add_module( "access", "access", NULL, NULL, ACCESS_TEXT,
                ACCESS_LONGTEXT, VLC_TRUE );
    add_module( "demux", "demux", NULL, NULL, DEMUX_TEXT,
                DEMUX_LONGTEXT, VLC_TRUE );
    add_bool( "minimize-threads", 0, NULL, MINIMIZE_THREADS_TEXT,
              MINIMIZE_THREADS_LONGTEXT, VLC_TRUE );
    add_bool( "plugins-cache", VLC_TRUE, NULL, PLUGINS_CACHE_TEXT,
              PLUGINS_CACHE_LONGTEXT, VLC_TRUE );
    add_directory( "plugin-path", NULL, NULL, PLUGIN_PATH_TEXT,
                   PLUGIN_PATH_LONGTEXT, VLC_TRUE );

#if !defined(WIN32)
    add_bool( "daemon", 0, NULL, DAEMON_TEXT, DAEMON_LONGTEXT, VLC_TRUE );
        change_short('d');
#endif

#if !defined(SYS_DARWIN) && !defined(SYS_BEOS) && defined(PTHREAD_COND_T_IN_PTHREAD_H)
    add_bool( "rt-priority", 0, NULL, RT_PRIORITY_TEXT,
              RT_PRIORITY_LONGTEXT, VLC_TRUE );
#endif

#if !defined(SYS_BEOS) && defined(PTHREAD_COND_T_IN_PTHREAD_H)
    add_integer( "rt-offset", 0, NULL, RT_OFFSET_TEXT,
                 RT_OFFSET_LONGTEXT, VLC_TRUE );
#endif

#if defined(WIN32)
    add_bool( "one-instance", 0, NULL, ONEINSTANCE_TEXT,
              ONEINSTANCE_LONGTEXT, VLC_TRUE );
    add_bool( "high-priority", 0, NULL, HPRIORITY_TEXT,
              HPRIORITY_LONGTEXT, VLC_FALSE );
    add_bool( "fast-mutex", 0, NULL, FAST_MUTEX_TEXT,
              FAST_MUTEX_LONGTEXT, VLC_TRUE );
    add_integer( "win9x-cv-method", 1, NULL, WIN9X_CV_TEXT,
                  WIN9X_CV_LONGTEXT, VLC_TRUE );
#endif

    /* Hotkey options*/
    add_category_hint( N_("Hot keys"), HOTKEY_CAT_LONGTEXT , VLC_FALSE );

#if defined(SYS_DARWIN)
#   define KEY_FULLSCREEN         KEY_MODIFIER_COMMAND|'f'
#   define KEY_PLAY_PAUSE         KEY_MODIFIER_COMMAND|'p'
#   define KEY_PAUSE              KEY_UNSET
#   define KEY_PLAY               KEY_UNSET
#   define KEY_FASTER             KEY_MODIFIER_COMMAND|'='
#   define KEY_SLOWER             KEY_MODIFIER_COMMAND|'-'
#   define KEY_NEXT               KEY_MODIFIER_COMMAND|KEY_RIGHT
#   define KEY_PREV               KEY_MODIFIER_COMMAND|KEY_LEFT
#   define KEY_STOP               KEY_MODIFIER_COMMAND|'.'
#   define KEY_POSITION           't'
#   define KEY_JUMP_M10SEC        KEY_MODIFIER_COMMAND|KEY_MODIFIER_ALT|KEY_LEFT
#   define KEY_JUMP_P10SEC        KEY_MODIFIER_COMMAND|KEY_MODIFIER_ALT|KEY_RIGHT
#   define KEY_JUMP_M1MIN         KEY_MODIFIER_COMMAND|KEY_MODIFIER_SHIFT|KEY_LEFT
#   define KEY_JUMP_P1MIN         KEY_MODIFIER_COMMAND|KEY_MODIFIER_SHIFT|KEY_RIGHT
#   define KEY_JUMP_M5MIN         KEY_MODIFIER_COMMAND|KEY_MODIFIER_SHIFT|KEY_MODIFIER_ALT|KEY_LEFT
#   define KEY_JUMP_P5MIN         KEY_MODIFIER_COMMAND|KEY_MODIFIER_SHIFT|KEY_MODIFIER_ALT|KEY_RIGHT
#   define KEY_NAV_ACTIVATE       KEY_ENTER
#   define KEY_NAV_UP             KEY_UP
#   define KEY_NAV_DOWN           KEY_DOWN
#   define KEY_NAV_LEFT           KEY_LEFT
#   define KEY_NAV_RIGHT          KEY_RIGHT
#   define KEY_QUIT               KEY_MODIFIER_COMMAND|'q'
#   define KEY_VOL_UP             KEY_MODIFIER_COMMAND|KEY_UP
#   define KEY_VOL_DOWN           KEY_MODIFIER_COMMAND|KEY_DOWN
#   define KEY_VOL_MUTE           KEY_MODIFIER_COMMAND|KEY_MODIFIER_SHIFT|'m'
#   define KEY_SUBDELAY_UP        KEY_MODIFIER_COMMAND|'k'
#   define KEY_SUBDELAY_DOWN      KEY_MODIFIER_COMMAND|'j'
#   define KEY_AUDIO_TRACK        'l'
#   define KEY_SUBTITLE_TRACK     's'
#   define KEY_INTF_SHOW          'i'

#   define KEY_SET_BOOKMARK1      KEY_MODIFIER_COMMAND|KEY_F1
#   define KEY_SET_BOOKMARK2      KEY_MODIFIER_COMMAND|KEY_F2
#   define KEY_SET_BOOKMARK3      KEY_MODIFIER_COMMAND|KEY_F3
#   define KEY_SET_BOOKMARK4      KEY_MODIFIER_COMMAND|KEY_F4
#   define KEY_SET_BOOKMARK5      KEY_MODIFIER_COMMAND|KEY_F5
#   define KEY_SET_BOOKMARK6      KEY_MODIFIER_COMMAND|KEY_F6
#   define KEY_SET_BOOKMARK7      KEY_MODIFIER_COMMAND|KEY_F7
#   define KEY_SET_BOOKMARK8      KEY_MODIFIER_COMMAND|KEY_F8
#   define KEY_SET_BOOKMARK9      KEY_UNSET
#   define KEY_SET_BOOKMARK10     KEY_UNSET
#   define KEY_PLAY_BOOKMARK1     KEY_F1
#   define KEY_PLAY_BOOKMARK2     KEY_F2
#   define KEY_PLAY_BOOKMARK3     KEY_F3
#   define KEY_PLAY_BOOKMARK4     KEY_F4
#   define KEY_PLAY_BOOKMARK5     KEY_F5
#   define KEY_PLAY_BOOKMARK6     KEY_F6
#   define KEY_PLAY_BOOKMARK7     KEY_F7
#   define KEY_PLAY_BOOKMARK8     KEY_F8
#   define KEY_PLAY_BOOKMARK9     KEY_UNSET
#   define KEY_PLAY_BOOKMARK10    KEY_UNSET
#   define KEY_HISTORY_BACK       KEY_MODIFIER_COMMAND|'['
#   define KEY_HISTORY_FORWARD    KEY_MODIFIER_COMMAND|']'

#else
#   define KEY_FULLSCREEN         'f'
#   define KEY_PLAY_PAUSE         KEY_SPACE
#   define KEY_PAUSE              KEY_UNSET
#   define KEY_PLAY               KEY_UNSET
#   define KEY_FASTER             '+'
#   define KEY_SLOWER             '-'
#   define KEY_NEXT               'n'
#   define KEY_PREV               'p'
#   define KEY_STOP               's'
#   define KEY_POSITION           't'
#   define KEY_JUMP_M10SEC        KEY_MODIFIER_ALT|KEY_LEFT
#   define KEY_JUMP_P10SEC        KEY_MODIFIER_ALT|KEY_RIGHT
#   define KEY_JUMP_M1MIN         KEY_MODIFIER_CTRL|KEY_LEFT
#   define KEY_JUMP_P1MIN         KEY_MODIFIER_CTRL|KEY_RIGHT
#   define KEY_JUMP_M5MIN         KEY_MODIFIER_CTRL|KEY_MODIFIER_ALT|KEY_LEFT
#   define KEY_JUMP_P5MIN         KEY_MODIFIER_CTRL|KEY_MODIFIER_ALT|KEY_RIGHT
#   define KEY_NAV_ACTIVATE       KEY_ENTER
#   define KEY_NAV_UP             KEY_UP
#   define KEY_NAV_DOWN           KEY_DOWN
#   define KEY_NAV_LEFT           KEY_LEFT
#   define KEY_NAV_RIGHT          KEY_RIGHT
#   define KEY_QUIT               KEY_MODIFIER_CTRL|'q'
#   define KEY_VOL_UP             KEY_MODIFIER_CTRL|KEY_UP
#   define KEY_VOL_DOWN           KEY_MODIFIER_CTRL|KEY_DOWN
#   define KEY_VOL_MUTE           'm'
#   define KEY_SUBDELAY_UP        KEY_MODIFIER_CTRL|'h'
#   define KEY_SUBDELAY_DOWN      KEY_MODIFIER_CTRL|'j'
#   define KEY_AUDIO_TRACK        'l'
#   define KEY_SUBTITLE_TRACK     'k'
#   define KEY_INTF_SHOW          'i'

#   define KEY_SET_BOOKMARK1      KEY_MODIFIER_CTRL|KEY_F1
#   define KEY_SET_BOOKMARK2      KEY_MODIFIER_CTRL|KEY_F2
#   define KEY_SET_BOOKMARK3      KEY_MODIFIER_CTRL|KEY_F3
#   define KEY_SET_BOOKMARK4      KEY_MODIFIER_CTRL|KEY_F4
#   define KEY_SET_BOOKMARK5      KEY_MODIFIER_CTRL|KEY_F5
#   define KEY_SET_BOOKMARK6      KEY_MODIFIER_CTRL|KEY_F6
#   define KEY_SET_BOOKMARK7      KEY_MODIFIER_CTRL|KEY_F7
#   define KEY_SET_BOOKMARK8      KEY_MODIFIER_CTRL|KEY_F8
#   define KEY_SET_BOOKMARK9      KEY_MODIFIER_CTRL|KEY_F9
#   define KEY_SET_BOOKMARK10     KEY_MODIFIER_CTRL|KEY_F10
#   define KEY_PLAY_BOOKMARK1     KEY_F1
#   define KEY_PLAY_BOOKMARK2     KEY_F2
#   define KEY_PLAY_BOOKMARK3     KEY_F3
#   define KEY_PLAY_BOOKMARK4     KEY_F4
#   define KEY_PLAY_BOOKMARK5     KEY_F5
#   define KEY_PLAY_BOOKMARK6     KEY_F6
#   define KEY_PLAY_BOOKMARK7     KEY_F7
#   define KEY_PLAY_BOOKMARK8     KEY_F8
#   define KEY_PLAY_BOOKMARK9     KEY_F9
#   define KEY_PLAY_BOOKMARK10    KEY_F10
#   define KEY_HISTORY_BACK       KEY_MODIFIER_CTRL|'['
#   define KEY_HISTORY_FORWARD    KEY_MODIFIER_CTRL|']'
#endif

    add_key( "key-fullscreen", KEY_FULLSCREEN, NULL, FULLSCREEN_KEY_TEXT,
             FULLSCREEN_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-play-pause", KEY_PLAY_PAUSE, NULL, PLAY_PAUSE_KEY_TEXT,
             PLAY_PAUSE_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-pause", KEY_PAUSE, NULL, PAUSE_KEY_TEXT,
             PAUSE_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-play", KEY_PLAY, NULL, PLAY_KEY_TEXT,
             PLAY_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-faster", KEY_FASTER, NULL, FASTER_KEY_TEXT,
             FASTER_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-slower", KEY_SLOWER, NULL, SLOWER_KEY_TEXT,
             SLOWER_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-next", KEY_NEXT, NULL, NEXT_KEY_TEXT,
             NEXT_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-prev", KEY_PREV, NULL, PREV_KEY_TEXT,
             PREV_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-stop", KEY_STOP, NULL, STOP_KEY_TEXT,
             STOP_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-position", KEY_POSITION, NULL, POSITION_KEY_TEXT,
             POSITION_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-jump-10sec", KEY_JUMP_M10SEC, NULL, JB10SEC_KEY_TEXT,
             JB10SEC_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-jump+10sec", KEY_JUMP_P10SEC, NULL, JF10SEC_KEY_TEXT,
             JF10SEC_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-jump-1min", KEY_JUMP_M1MIN, NULL, JB1MIN_KEY_TEXT,
             JB1MIN_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-jump+1min", KEY_JUMP_P1MIN, NULL, JF1MIN_KEY_TEXT,
             JF1MIN_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-jump-5min", KEY_JUMP_M5MIN, NULL, JB5MIN_KEY_TEXT,
             JB5MIN_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-jump+5min", KEY_JUMP_P5MIN, NULL, JF5MIN_KEY_TEXT,
             JF5MIN_KEY_LONGTEXT, VLC_FALSE );

    add_key( "key-nav-activate", KEY_NAV_ACTIVATE, NULL, NAV_ACTIVATE_KEY_TEXT,
             NAV_ACTIVATE_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-nav-up", KEY_NAV_UP, NULL, NAV_UP_KEY_TEXT,
             NAV_UP_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-nav-down", KEY_NAV_DOWN, NULL, NAV_DOWN_KEY_TEXT,
             NAV_DOWN_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-nav-left", KEY_NAV_LEFT, NULL, NAV_LEFT_KEY_TEXT,
             NAV_LEFT_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-nav-right", KEY_NAV_RIGHT, NULL, NAV_RIGHT_KEY_TEXT,
             NAV_RIGHT_KEY_LONGTEXT, VLC_TRUE );

    add_key( "key-quit", KEY_QUIT, NULL, QUIT_KEY_TEXT,
             QUIT_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-vol-up", KEY_VOL_UP, NULL, VOL_UP_KEY_TEXT,
             VOL_UP_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-vol-down", KEY_VOL_DOWN, NULL, VOL_DOWN_KEY_TEXT,
             VOL_DOWN_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-vol-mute", KEY_VOL_MUTE, NULL, VOL_MUTE_KEY_TEXT,
             VOL_MUTE_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-subdelay-up", KEY_SUBDELAY_UP, NULL,
             SUBDELAY_UP_KEY_TEXT, SUBDELAY_UP_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-subdelay-down", KEY_SUBDELAY_DOWN, NULL,
             SUBDELAY_DOWN_KEY_TEXT, SUBDELAY_DOWN_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-audio-track", KEY_AUDIO_TRACK, NULL, AUDIO_TRACK_KEY_TEXT,
             AUDIO_TRACK_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-subtitle-track", KEY_SUBTITLE_TRACK, NULL,
             SUBTITLE_TRACK_KEY_TEXT, SUBTITLE_TRACK_KEY_LONGTEXT, VLC_FALSE );
    add_key( "key-intf-show", KEY_INTF_SHOW, NULL,
             INTF_SHOW_KEY_TEXT, INTF_SHOW_KEY_LONGTEXT, VLC_TRUE );

    add_key( "key-set-bookmark1", KEY_SET_BOOKMARK1, NULL,
             SET_BOOKMARK1_KEY_TEXT, SET_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-set-bookmark2", KEY_SET_BOOKMARK2, NULL,
             SET_BOOKMARK2_KEY_TEXT, SET_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-set-bookmark3", KEY_SET_BOOKMARK3, NULL,
             SET_BOOKMARK3_KEY_TEXT, SET_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-set-bookmark4", KEY_SET_BOOKMARK4, NULL,
             SET_BOOKMARK4_KEY_TEXT, SET_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-set-bookmark5", KEY_SET_BOOKMARK5, NULL,
             SET_BOOKMARK5_KEY_TEXT, SET_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-set-bookmark6", KEY_SET_BOOKMARK6, NULL,
             SET_BOOKMARK6_KEY_TEXT, SET_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-set-bookmark7", KEY_SET_BOOKMARK7, NULL,
             SET_BOOKMARK7_KEY_TEXT, SET_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-set-bookmark8", KEY_SET_BOOKMARK8, NULL,
             SET_BOOKMARK8_KEY_TEXT, SET_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-set-bookmark9", KEY_SET_BOOKMARK9, NULL,
             SET_BOOKMARK9_KEY_TEXT, SET_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-set-bookmark10", KEY_SET_BOOKMARK10, NULL,
             SET_BOOKMARK10_KEY_TEXT, SET_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-play-bookmark1", KEY_PLAY_BOOKMARK1, NULL,
             PLAY_BOOKMARK1_KEY_TEXT, PLAY_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-play-bookmark2", KEY_PLAY_BOOKMARK2, NULL,
             PLAY_BOOKMARK2_KEY_TEXT, PLAY_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-play-bookmark3", KEY_PLAY_BOOKMARK3, NULL,
             PLAY_BOOKMARK3_KEY_TEXT, PLAY_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-play-bookmark4", KEY_PLAY_BOOKMARK4, NULL,
             PLAY_BOOKMARK4_KEY_TEXT, PLAY_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-play-bookmark5", KEY_PLAY_BOOKMARK5, NULL,
             PLAY_BOOKMARK5_KEY_TEXT, PLAY_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-play-bookmark6", KEY_PLAY_BOOKMARK6, NULL,
             PLAY_BOOKMARK6_KEY_TEXT, PLAY_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-play-bookmark7", KEY_PLAY_BOOKMARK7, NULL,
             PLAY_BOOKMARK7_KEY_TEXT, PLAY_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-play-bookmark8", KEY_PLAY_BOOKMARK8, NULL,
             PLAY_BOOKMARK8_KEY_TEXT, PLAY_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-play-bookmark9", KEY_PLAY_BOOKMARK9, NULL,
             PLAY_BOOKMARK9_KEY_TEXT, PLAY_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-play-bookmark10", KEY_PLAY_BOOKMARK10, NULL,
             PLAY_BOOKMARK10_KEY_TEXT, PLAY_BOOKMARK_KEY_LONGTEXT, VLC_TRUE );
    add_key( "key-history-back", KEY_HISTORY_BACK, NULL, HISTORY_BACK_TEXT,
             HISTORY_BACK_LONGTEXT, VLC_TRUE );
    add_key( "key-history-forward", KEY_HISTORY_FORWARD, NULL,
             HISTORY_FORWARD_TEXT, HISTORY_FORWARD_LONGTEXT, VLC_TRUE );

    /* Usage (mainly useful for cmd line stuff) */
    add_usage_hint( PLAYLIST_USAGE );

    set_description( N_("main program") );
    set_capability( "main", 100 );
vlc_module_end();

static module_config_t p_help_config[] =
{
    { CONFIG_ITEM_BOOL, NULL, "help", 'h',
      N_("print help (can be combined with --advanced)") },
    { CONFIG_ITEM_BOOL, NULL, "longhelp", 'H',
      N_("print detailed help (can be combined with --advanced)") },
    { CONFIG_ITEM_BOOL, NULL, "list", 'l',
      N_("print a list of available modules") },
    { CONFIG_ITEM_STRING, NULL, "module", 'p',
      N_("print help on module (can be combined with --advanced)") },
    { CONFIG_ITEM_BOOL, NULL, "save-config", '\0',
      N_("save the current command line options in the config") },
    { CONFIG_ITEM_BOOL, NULL, "reset-config", '\0',
      N_("reset the current config to the default values") },
    { CONFIG_ITEM_STRING, NULL, "config", '\0',
      N_("use alternate config file") },
    { CONFIG_ITEM_BOOL, NULL, "reset-plugins-cache", '\0',
      N_("resets the current plugins cache") },
    { CONFIG_ITEM_BOOL, NULL, "version", '\0',
      N_("print version information") },
    { CONFIG_HINT_END, NULL, NULL, '\0', NULL }
};

/*****************************************************************************
 * End configuration.
 *****************************************************************************/

/*****************************************************************************
 * Initializer for the vlc_t structure storing the action / key associations
 *****************************************************************************/
static struct hotkey p_hotkeys[] =
{
    { "key-quit", ACTIONID_QUIT, 0 },
    { "key-play-pause", ACTIONID_PLAY_PAUSE, 0 },
    { "key-play", ACTIONID_PLAY, 0 },
    { "key-pause", ACTIONID_PAUSE, 0 },
    { "key-stop", ACTIONID_STOP, 0 },
    { "key-position", ACTIONID_POSITION, 0 },
    { "key-jump-10sec", ACTIONID_JUMP_BACKWARD_10SEC, 0 },
    { "key-jump+10sec", ACTIONID_JUMP_FORWARD_10SEC, 0 },
    { "key-jump-1min", ACTIONID_JUMP_BACKWARD_1MIN, 0 },
    { "key-jump+1min", ACTIONID_JUMP_FORWARD_1MIN, 0 },
    { "key-jump-5min", ACTIONID_JUMP_BACKWARD_5MIN, 0 },
    { "key-jump+5min", ACTIONID_JUMP_FORWARD_5MIN, 0 },
    { "key-prev", ACTIONID_PREV, 0 },
    { "key-next", ACTIONID_NEXT, 0 },
    { "key-faster", ACTIONID_FASTER, 0 },
    { "key-slower", ACTIONID_SLOWER, 0 },
    { "key-fullscreen", ACTIONID_FULLSCREEN, 0 },
    { "key-vol-up", ACTIONID_VOL_UP, 0 },
    { "key-vol-down", ACTIONID_VOL_DOWN, 0 },
    { "key-vol-mute", ACTIONID_VOL_MUTE, 0 },
    { "key-subdelay-down", ACTIONID_SUBDELAY_DOWN, 0 },
    { "key-subdelay-up", ACTIONID_SUBDELAY_UP, 0 },
    { "key-audio-track", ACTIONID_AUDIO_TRACK, 0},
    { "key-subtitle-track", ACTIONID_SUBTITLE_TRACK, 0},
    { "key-intf-show", ACTIONID_INTF_SHOW, 0},
    { "key-nav-activate", ACTIONID_NAV_ACTIVATE, 0 },
    { "key-nav-up", ACTIONID_NAV_UP, 0 },
    { "key-nav-down", ACTIONID_NAV_DOWN, 0 },
    { "key-nav-left", ACTIONID_NAV_LEFT, 0 },
    { "key-nav-right", ACTIONID_NAV_RIGHT, 0 },
    { "key-set-bookmark1", ACTIONID_SET_BOOKMARK1, 0},
    { "key-set-bookmark2", ACTIONID_SET_BOOKMARK2, 0},
    { "key-set-bookmark3", ACTIONID_SET_BOOKMARK3, 0},
    { "key-set-bookmark4", ACTIONID_SET_BOOKMARK4, 0},
    { "key-set-bookmark5", ACTIONID_SET_BOOKMARK5, 0},
    { "key-set-bookmark6", ACTIONID_SET_BOOKMARK6, 0},
    { "key-set-bookmark7", ACTIONID_SET_BOOKMARK7, 0},
    { "key-set-bookmark8", ACTIONID_SET_BOOKMARK8, 0},
    { "key-set-bookmark9", ACTIONID_SET_BOOKMARK9, 0},
    { "key-set-bookmark10", ACTIONID_SET_BOOKMARK10, 0},
    { "key-play-bookmark1", ACTIONID_PLAY_BOOKMARK1, 0},
    { "key-play-bookmark2", ACTIONID_PLAY_BOOKMARK2, 0},
    { "key-play-bookmark3", ACTIONID_PLAY_BOOKMARK3, 0},
    { "key-play-bookmark4", ACTIONID_PLAY_BOOKMARK4, 0},
    { "key-play-bookmark5", ACTIONID_PLAY_BOOKMARK5, 0},
    { "key-play-bookmark6", ACTIONID_PLAY_BOOKMARK6, 0},
    { "key-play-bookmark7", ACTIONID_PLAY_BOOKMARK7, 0},
    { "key-play-bookmark8", ACTIONID_PLAY_BOOKMARK8, 0},
    { "key-play-bookmark9", ACTIONID_PLAY_BOOKMARK9, 0},
    { "key-play-bookmark10", ACTIONID_PLAY_BOOKMARK10, 0},
    { "key-history-back", ACTIONID_HISTORY_BACK, 0},
    { "key-history-forward", ACTIONID_HISTORY_FORWARD, 0},
    { NULL, 0, 0 }
};

