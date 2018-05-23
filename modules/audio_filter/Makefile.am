
# modules/audio_filter/Makefile.am automatically generated from modules/audio_filter/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = audio_filter
mods = equalizer normvol audio_format

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = channel_mixer converter resampler
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libequalizer_plugin.a libequalizer.a libequalizer_pic.a libnormvol_plugin.a libnormvol.a libnormvol_pic.a libaudio_format_plugin.a libaudio_format.a libaudio_format_pic.a
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

# The equalizer plugin

libequalizer_plugin_a_SOURCES = $(SOURCES_equalizer)
libequalizer_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin equalizer`
libequalizer_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin equalizer`
libequalizer_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin equalizer`

libequalizer_pic_a_SOURCES = $(SOURCES_equalizer)
libequalizer_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic equalizer`
libequalizer_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic equalizer`
libequalizer_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic equalizer`

libequalizer_a_SOURCES = $(SOURCES_equalizer)
libequalizer_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin equalizer`
libequalizer_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin equalizer`
libequalizer_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin equalizer`

# The normvol plugin

libnormvol_plugin_a_SOURCES = $(SOURCES_normvol)
libnormvol_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin normvol`
libnormvol_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin normvol`
libnormvol_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin normvol`

libnormvol_pic_a_SOURCES = $(SOURCES_normvol)
libnormvol_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic normvol`
libnormvol_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic normvol`
libnormvol_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic normvol`

libnormvol_a_SOURCES = $(SOURCES_normvol)
libnormvol_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin normvol`
libnormvol_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin normvol`
libnormvol_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin normvol`

# The audio_format plugin

libaudio_format_plugin_a_SOURCES = $(SOURCES_audio_format)
libaudio_format_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin audio_format`
libaudio_format_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin audio_format`
libaudio_format_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin audio_format`

libaudio_format_pic_a_SOURCES = $(SOURCES_audio_format)
libaudio_format_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic audio_format`
libaudio_format_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic audio_format`
libaudio_format_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic audio_format`

libaudio_format_a_SOURCES = $(SOURCES_audio_format)
libaudio_format_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin audio_format`
libaudio_format_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin audio_format`
libaudio_format_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin audio_format`

