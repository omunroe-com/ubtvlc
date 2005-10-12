
# modules/packetizer/Makefile.am automatically generated from modules/packetizer/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = packetizer
mods = packetizer_copy packetizer_mpegvideo packetizer_mpeg4video packetizer_mpeg4audio packetizer_h264

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = 
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libpacketizer_copy_plugin.a libpacketizer_copy.a libpacketizer_copy_pic.a libpacketizer_mpegvideo_plugin.a libpacketizer_mpegvideo.a libpacketizer_mpegvideo_pic.a libpacketizer_mpeg4video_plugin.a libpacketizer_mpeg4video.a libpacketizer_mpeg4video_pic.a libpacketizer_mpeg4audio_plugin.a libpacketizer_mpeg4audio.a libpacketizer_mpeg4audio_pic.a libpacketizer_h264_plugin.a libpacketizer_h264.a libpacketizer_h264_pic.a
include Modules.am

all: all-modules

# Find out which modules were enabled and tell make to build them
all-modules:
	@set fnord $$MAKEFLAGS; amf=$$2; targets=`\
	if test "$(plugin)" != "no"; then z=$$($(VLC_CONFIG) --list plugin); for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo lib$${mod}_plugin$(LIBEXT);; esac; done; fi; \
	if test "$(builtin)" != "no"; then z=$$($(VLC_CONFIG) --list builtin); for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo lib$${mod}.a;; esac; done; fi; \
	if test "$(pic)" == "pic"; then z=$$($(VLC_CONFIG) --list builtin); for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo lib$${mod}_pic.a;; esac; done; fi; \
	`; case "$$targets" in *lib*) $(MAKE) $(AM_MAKEFLAGS) $$targets || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; ;; esac; \
	test -z "$$fail"

# Build a plugin with the adequate linker and linker's flags
_plugin.a_plugin$(LIBEXT):
	@mod="$*" ; mod=$${mod:3} ; 	ldfl="`$(VLC_CONFIG) --libs plugin $$mod` -u $(SYMPREF)$(VLC_ENTRY)" ; 	case `$(VLC_CONFIG) --linkage $$mod` in \
	  c++)  ld="$(CXXLINK)" ;; 	  objc) ld="$(OBJCLINK)" ;; 	  c|*)  ld="$(LINK)" ;; 	esac ; 	echo $$ld $< $$ldfl ; 	$$ld $< $$ldfl

$(srcdir)/Makefile.am: $(srcdir)/Modules.am $(top_srcdir)/bootstrap
	cd $(top_srcdir) && $(SHELL) ./bootstrap

mostlyclean-local:
	-rm -f *.a *$(LIBEXT)

### automake creates libvlcdir after running install-*-local
### so we have to create it ourselves first
install-exec-local: all-modules
	mkdir -p -- "$(DESTDIR)$(libvlcdir)"
	@z=$$($(VLC_CONFIG) --list plugin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo $(INSTALL_PROGRAM) "lib$${mod}_plugin$(LIBEXT)" "$(DESTDIR)$(libvlcdir)/" ; 	      $(INSTALL_PROGRAM) "lib$${mod}_plugin$(LIBEXT)" "$(DESTDIR)$(libvlcdir)/" || exit $$?; 	      ;; 	  esac; 	done
	@z=$$($(VLC_CONFIG) --list builtin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo $(INSTALL_DATA) "lib$${mod}.a" "$(DESTDIR)$(libdir)/vlc/" ; 	      $(INSTALL_DATA) "lib$${mod}.a" "$(DESTDIR)$(libdir)/vlc/" || exit $$?; 	      ;; 	  esac; 	done
if BUILD_MOZILLA
	@z=$$($(VLC_CONFIG) --list builtin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo $(INSTALL_DATA) "lib$${mod}_pic.a" "$(DESTDIR)$(libdir)/vlc/" ; 	      $(INSTALL_DATA) "lib$${mod}_pic.a" "$(DESTDIR)$(libdir)/vlc/" || exit $$?; 	      ;; 	  esac; 	done
endif

uninstall-local:
	@z=$$($(VLC_CONFIG) --list plugin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo rm -f "$(DESTDIR)$(libvlcdir)/lib$${mod}_plugin$(LIBEXT)" ; 	      rm -f "$(DESTDIR)$(libvlcdir)/lib$${mod}_plugin$(LIBEXT)" || true; 	      ;; 	  esac; 	done
	@z=$$($(VLC_CONFIG) --list builtin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo rm -f "$(DESTDIR)$(libdir)/vlc/lib$${mod}.a" ; 	      rm -f "$(DESTDIR)$(libdir)/vlc/lib$${mod}.a" || true; 	      ;; 	  esac; 	done
if BUILD_MOZILLA
	@z=$$($(VLC_CONFIG) --list builtin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo rm -f "$(DESTDIR)$(libdir)/vlc/lib$${mod}_pic.a" ; 	      rm -f "$(DESTDIR)$(libdir)/vlc/lib$${mod}_pic.a" || true; 	      ;; 	  esac; 	done
endif

# The packetizer_copy plugin

libpacketizer_copy_plugin_a_SOURCES = $(SOURCES_packetizer_copy)
libpacketizer_copy_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin packetizer_copy`
libpacketizer_copy_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin packetizer_copy`
libpacketizer_copy_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin packetizer_copy`

libpacketizer_copy_pic_a_SOURCES = $(SOURCES_packetizer_copy)
libpacketizer_copy_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic packetizer_copy`
libpacketizer_copy_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic packetizer_copy`
libpacketizer_copy_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic packetizer_copy`

libpacketizer_copy_a_SOURCES = $(SOURCES_packetizer_copy)
libpacketizer_copy_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin packetizer_copy`
libpacketizer_copy_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin packetizer_copy`
libpacketizer_copy_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin packetizer_copy`

# The packetizer_mpegvideo plugin

libpacketizer_mpegvideo_plugin_a_SOURCES = $(SOURCES_packetizer_mpegvideo)
libpacketizer_mpegvideo_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin packetizer_mpegvideo`
libpacketizer_mpegvideo_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin packetizer_mpegvideo`
libpacketizer_mpegvideo_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin packetizer_mpegvideo`

libpacketizer_mpegvideo_pic_a_SOURCES = $(SOURCES_packetizer_mpegvideo)
libpacketizer_mpegvideo_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic packetizer_mpegvideo`
libpacketizer_mpegvideo_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic packetizer_mpegvideo`
libpacketizer_mpegvideo_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic packetizer_mpegvideo`

libpacketizer_mpegvideo_a_SOURCES = $(SOURCES_packetizer_mpegvideo)
libpacketizer_mpegvideo_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin packetizer_mpegvideo`
libpacketizer_mpegvideo_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin packetizer_mpegvideo`
libpacketizer_mpegvideo_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin packetizer_mpegvideo`

# The packetizer_mpeg4video plugin

libpacketizer_mpeg4video_plugin_a_SOURCES = $(SOURCES_packetizer_mpeg4video)
libpacketizer_mpeg4video_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin packetizer_mpeg4video`
libpacketizer_mpeg4video_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin packetizer_mpeg4video`
libpacketizer_mpeg4video_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin packetizer_mpeg4video`

libpacketizer_mpeg4video_pic_a_SOURCES = $(SOURCES_packetizer_mpeg4video)
libpacketizer_mpeg4video_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic packetizer_mpeg4video`
libpacketizer_mpeg4video_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic packetizer_mpeg4video`
libpacketizer_mpeg4video_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic packetizer_mpeg4video`

libpacketizer_mpeg4video_a_SOURCES = $(SOURCES_packetizer_mpeg4video)
libpacketizer_mpeg4video_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin packetizer_mpeg4video`
libpacketizer_mpeg4video_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin packetizer_mpeg4video`
libpacketizer_mpeg4video_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin packetizer_mpeg4video`

# The packetizer_mpeg4audio plugin

libpacketizer_mpeg4audio_plugin_a_SOURCES = $(SOURCES_packetizer_mpeg4audio)
libpacketizer_mpeg4audio_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin packetizer_mpeg4audio`
libpacketizer_mpeg4audio_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin packetizer_mpeg4audio`
libpacketizer_mpeg4audio_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin packetizer_mpeg4audio`

libpacketizer_mpeg4audio_pic_a_SOURCES = $(SOURCES_packetizer_mpeg4audio)
libpacketizer_mpeg4audio_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic packetizer_mpeg4audio`
libpacketizer_mpeg4audio_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic packetizer_mpeg4audio`
libpacketizer_mpeg4audio_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic packetizer_mpeg4audio`

libpacketizer_mpeg4audio_a_SOURCES = $(SOURCES_packetizer_mpeg4audio)
libpacketizer_mpeg4audio_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin packetizer_mpeg4audio`
libpacketizer_mpeg4audio_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin packetizer_mpeg4audio`
libpacketizer_mpeg4audio_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin packetizer_mpeg4audio`

# The packetizer_h264 plugin

libpacketizer_h264_plugin_a_SOURCES = $(SOURCES_packetizer_h264)
libpacketizer_h264_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin packetizer_h264`
libpacketizer_h264_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin packetizer_h264`
libpacketizer_h264_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin packetizer_h264`

libpacketizer_h264_pic_a_SOURCES = $(SOURCES_packetizer_h264)
libpacketizer_h264_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic packetizer_h264`
libpacketizer_h264_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic packetizer_h264`
libpacketizer_h264_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic packetizer_h264`

libpacketizer_h264_a_SOURCES = $(SOURCES_packetizer_h264)
libpacketizer_h264_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin packetizer_h264`
libpacketizer_h264_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin packetizer_h264`
libpacketizer_h264_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin packetizer_h264`

