
# modules/audio_output/Makefile.am automatically generated from modules/audio_output/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = audio_output
mods = alsa arts coreaudio aout_directx esd aout_file oss aout_sdl waveout hd1000a portaudio auhal

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = 
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libalsa_plugin.a libalsa.a libalsa_pic.a libarts_plugin.a libarts.a libarts_pic.a libcoreaudio_plugin.a libcoreaudio.a libcoreaudio_pic.a libaout_directx_plugin.a libaout_directx.a libaout_directx_pic.a libesd_plugin.a libesd.a libesd_pic.a libaout_file_plugin.a libaout_file.a libaout_file_pic.a liboss_plugin.a liboss.a liboss_pic.a libaout_sdl_plugin.a libaout_sdl.a libaout_sdl_pic.a libwaveout_plugin.a libwaveout.a libwaveout_pic.a libhd1000a_plugin.a libhd1000a.a libhd1000a_pic.a libportaudio_plugin.a libportaudio.a libportaudio_pic.a libauhal_plugin.a libauhal.a libauhal_pic.a
include Modules.am

all: all-modules

# Find out which modules were enabled and tell make to build them
all-modules:
	@set fnord $$MAKEFLAGS; amf=$$2; targets=`\
	if test "$(plugin)" != "no"; then z=$$($(VLC_CONFIG) --list plugin); for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo lib$${mod}_plugin$(LIBEXT);; esac; done; fi; \
	if test "$(builtin)" != "no"; then z=$$($(VLC_CONFIG) --list builtin); for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo lib$${mod}.a;; esac; done; fi; \
	if test "$(pic)" = "pic"; then z=$$($(VLC_CONFIG) --list builtin); for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo lib$${mod}_pic.a;; esac; done; fi; \
	`; case "$$targets" in *lib*) $(MAKE) $(AM_MAKEFLAGS) $$targets || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; ;; esac; \
	test -z "$$fail"

# Build a plugin with the adequate linker and linker's flags
_plugin.a_plugin$(LIBEXT):
	@mod="$*" ; mod=$${mod#lib} ; 	ldfl="`$(VLC_CONFIG) --libs plugin $$mod` -u $(SYMPREF)$(VLC_ENTRY)" ; 	case `$(VLC_CONFIG) --linkage $$mod` in \
	  c++)  ld="$(CXXLINK)" ;; 	  objc) ld="$(OBJCLINK)" ;; 	  c|*)  ld="$(LINK)" ;; 	esac ; 	echo $$ld $< $$ldfl ; 	$$ld $< $$ldfl

if MAINTAINER_MODE
$(srcdir)/Makefile.am: $(srcdir)/Modules.am $(top_srcdir)/bootstrap
	cd $(top_srcdir) && $(SHELL) ./bootstrap
endif

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

# The alsa plugin

libalsa_plugin_a_SOURCES = $(SOURCES_alsa)
libalsa_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin alsa`
libalsa_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin alsa`
libalsa_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin alsa`

libalsa_pic_a_SOURCES = $(SOURCES_alsa)
libalsa_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic alsa`
libalsa_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic alsa`
libalsa_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic alsa`

libalsa_a_SOURCES = $(SOURCES_alsa)
libalsa_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin alsa`
libalsa_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin alsa`
libalsa_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin alsa`

# The arts plugin

libarts_plugin_a_SOURCES = $(SOURCES_arts)
libarts_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin arts`
libarts_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin arts`
libarts_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin arts`

libarts_pic_a_SOURCES = $(SOURCES_arts)
libarts_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic arts`
libarts_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic arts`
libarts_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic arts`

libarts_a_SOURCES = $(SOURCES_arts)
libarts_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin arts`
libarts_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin arts`
libarts_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin arts`

# The coreaudio plugin

libcoreaudio_plugin_a_SOURCES = $(SOURCES_coreaudio)
libcoreaudio_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin coreaudio`
libcoreaudio_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin coreaudio`
libcoreaudio_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin coreaudio`

libcoreaudio_pic_a_SOURCES = $(SOURCES_coreaudio)
libcoreaudio_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic coreaudio`
libcoreaudio_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic coreaudio`
libcoreaudio_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic coreaudio`

libcoreaudio_a_SOURCES = $(SOURCES_coreaudio)
libcoreaudio_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin coreaudio`
libcoreaudio_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin coreaudio`
libcoreaudio_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin coreaudio`

# The aout_directx plugin

libaout_directx_plugin_a_SOURCES = $(SOURCES_aout_directx)
libaout_directx_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin aout_directx`
libaout_directx_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin aout_directx`
libaout_directx_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin aout_directx`

libaout_directx_pic_a_SOURCES = $(SOURCES_aout_directx)
libaout_directx_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic aout_directx`
libaout_directx_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic aout_directx`
libaout_directx_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic aout_directx`

libaout_directx_a_SOURCES = $(SOURCES_aout_directx)
libaout_directx_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin aout_directx`
libaout_directx_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin aout_directx`
libaout_directx_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin aout_directx`

# The esd plugin

libesd_plugin_a_SOURCES = $(SOURCES_esd)
libesd_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin esd`
libesd_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin esd`
libesd_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin esd`

libesd_pic_a_SOURCES = $(SOURCES_esd)
libesd_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic esd`
libesd_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic esd`
libesd_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic esd`

libesd_a_SOURCES = $(SOURCES_esd)
libesd_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin esd`
libesd_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin esd`
libesd_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin esd`

# The aout_file plugin

libaout_file_plugin_a_SOURCES = $(SOURCES_aout_file)
libaout_file_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin aout_file`
libaout_file_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin aout_file`
libaout_file_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin aout_file`

libaout_file_pic_a_SOURCES = $(SOURCES_aout_file)
libaout_file_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic aout_file`
libaout_file_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic aout_file`
libaout_file_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic aout_file`

libaout_file_a_SOURCES = $(SOURCES_aout_file)
libaout_file_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin aout_file`
libaout_file_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin aout_file`
libaout_file_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin aout_file`

# The oss plugin

liboss_plugin_a_SOURCES = $(SOURCES_oss)
liboss_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin oss`
liboss_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin oss`
liboss_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin oss`

liboss_pic_a_SOURCES = $(SOURCES_oss)
liboss_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic oss`
liboss_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic oss`
liboss_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic oss`

liboss_a_SOURCES = $(SOURCES_oss)
liboss_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin oss`
liboss_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin oss`
liboss_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin oss`

# The aout_sdl plugin

libaout_sdl_plugin_a_SOURCES = $(SOURCES_aout_sdl)
libaout_sdl_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin aout_sdl`
libaout_sdl_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin aout_sdl`
libaout_sdl_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin aout_sdl`

libaout_sdl_pic_a_SOURCES = $(SOURCES_aout_sdl)
libaout_sdl_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic aout_sdl`
libaout_sdl_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic aout_sdl`
libaout_sdl_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic aout_sdl`

libaout_sdl_a_SOURCES = $(SOURCES_aout_sdl)
libaout_sdl_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin aout_sdl`
libaout_sdl_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin aout_sdl`
libaout_sdl_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin aout_sdl`

# The waveout plugin

libwaveout_plugin_a_SOURCES = $(SOURCES_waveout)
libwaveout_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin waveout`
libwaveout_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin waveout`
libwaveout_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin waveout`

libwaveout_pic_a_SOURCES = $(SOURCES_waveout)
libwaveout_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic waveout`
libwaveout_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic waveout`
libwaveout_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic waveout`

libwaveout_a_SOURCES = $(SOURCES_waveout)
libwaveout_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin waveout`
libwaveout_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin waveout`
libwaveout_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin waveout`

# The hd1000a plugin

libhd1000a_plugin_a_SOURCES = $(SOURCES_hd1000a)
libhd1000a_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin hd1000a`
libhd1000a_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin hd1000a`
libhd1000a_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin hd1000a`

libhd1000a_pic_a_SOURCES = $(SOURCES_hd1000a)
libhd1000a_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic hd1000a`
libhd1000a_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic hd1000a`
libhd1000a_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic hd1000a`

libhd1000a_a_SOURCES = $(SOURCES_hd1000a)
libhd1000a_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin hd1000a`
libhd1000a_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin hd1000a`
libhd1000a_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin hd1000a`

# The portaudio plugin

libportaudio_plugin_a_SOURCES = $(SOURCES_portaudio)
libportaudio_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin portaudio`
libportaudio_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin portaudio`
libportaudio_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin portaudio`

libportaudio_pic_a_SOURCES = $(SOURCES_portaudio)
libportaudio_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic portaudio`
libportaudio_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic portaudio`
libportaudio_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic portaudio`

libportaudio_a_SOURCES = $(SOURCES_portaudio)
libportaudio_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin portaudio`
libportaudio_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin portaudio`
libportaudio_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin portaudio`

# The auhal plugin

libauhal_plugin_a_SOURCES = $(SOURCES_auhal)
libauhal_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin auhal`
libauhal_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin auhal`
libauhal_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin auhal`

libauhal_pic_a_SOURCES = $(SOURCES_auhal)
libauhal_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic auhal`
libauhal_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic auhal`
libauhal_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic auhal`

libauhal_a_SOURCES = $(SOURCES_auhal)
libauhal_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin auhal`
libauhal_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin auhal`
libauhal_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin auhal`

