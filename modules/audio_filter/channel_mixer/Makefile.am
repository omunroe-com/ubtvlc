
# modules/audio_filter/channel_mixer/Makefile.am automatically generated from modules/audio_filter/channel_mixer/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = audio_filter
mods = trivial_channel_mixer simple_channel_mixer headphone_channel_mixer

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = 
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libtrivial_channel_mixer_plugin.a libtrivial_channel_mixer.a libtrivial_channel_mixer_pic.a libsimple_channel_mixer_plugin.a libsimple_channel_mixer.a libsimple_channel_mixer_pic.a libheadphone_channel_mixer_plugin.a libheadphone_channel_mixer.a libheadphone_channel_mixer_pic.a
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

# The trivial_channel_mixer plugin

libtrivial_channel_mixer_plugin_a_SOURCES = $(SOURCES_trivial_channel_mixer)
libtrivial_channel_mixer_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin trivial_channel_mixer`
libtrivial_channel_mixer_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin trivial_channel_mixer`
libtrivial_channel_mixer_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin trivial_channel_mixer`

libtrivial_channel_mixer_pic_a_SOURCES = $(SOURCES_trivial_channel_mixer)
libtrivial_channel_mixer_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic trivial_channel_mixer`
libtrivial_channel_mixer_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic trivial_channel_mixer`
libtrivial_channel_mixer_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic trivial_channel_mixer`

libtrivial_channel_mixer_a_SOURCES = $(SOURCES_trivial_channel_mixer)
libtrivial_channel_mixer_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin trivial_channel_mixer`
libtrivial_channel_mixer_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin trivial_channel_mixer`
libtrivial_channel_mixer_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin trivial_channel_mixer`

# The simple_channel_mixer plugin

libsimple_channel_mixer_plugin_a_SOURCES = $(SOURCES_simple_channel_mixer)
libsimple_channel_mixer_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin simple_channel_mixer`
libsimple_channel_mixer_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin simple_channel_mixer`
libsimple_channel_mixer_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin simple_channel_mixer`

libsimple_channel_mixer_pic_a_SOURCES = $(SOURCES_simple_channel_mixer)
libsimple_channel_mixer_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic simple_channel_mixer`
libsimple_channel_mixer_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic simple_channel_mixer`
libsimple_channel_mixer_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic simple_channel_mixer`

libsimple_channel_mixer_a_SOURCES = $(SOURCES_simple_channel_mixer)
libsimple_channel_mixer_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin simple_channel_mixer`
libsimple_channel_mixer_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin simple_channel_mixer`
libsimple_channel_mixer_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin simple_channel_mixer`

# The headphone_channel_mixer plugin

libheadphone_channel_mixer_plugin_a_SOURCES = $(SOURCES_headphone_channel_mixer)
libheadphone_channel_mixer_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin headphone_channel_mixer`
libheadphone_channel_mixer_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin headphone_channel_mixer`
libheadphone_channel_mixer_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin headphone_channel_mixer`

libheadphone_channel_mixer_pic_a_SOURCES = $(SOURCES_headphone_channel_mixer)
libheadphone_channel_mixer_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic headphone_channel_mixer`
libheadphone_channel_mixer_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic headphone_channel_mixer`
libheadphone_channel_mixer_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic headphone_channel_mixer`

libheadphone_channel_mixer_a_SOURCES = $(SOURCES_headphone_channel_mixer)
libheadphone_channel_mixer_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin headphone_channel_mixer`
libheadphone_channel_mixer_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin headphone_channel_mixer`
libheadphone_channel_mixer_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin headphone_channel_mixer`

