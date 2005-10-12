
# modules/video_output/x11/Makefile.am automatically generated from modules/video_output/x11/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = video_output
mods = x11 xvideo glx

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = 
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libx11_plugin.a libx11.a libx11_pic.a libxvideo_plugin.a libxvideo.a libxvideo_pic.a libglx_plugin.a libglx.a libglx_pic.a
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

# The x11 plugin

libx11_plugin_a_SOURCES = $(SOURCES_x11)
libx11_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin x11`
libx11_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin x11`
libx11_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin x11`

libx11_pic_a_SOURCES = $(SOURCES_x11)
libx11_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic x11`
libx11_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic x11`
libx11_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic x11`

libx11_a_SOURCES = $(SOURCES_x11)
libx11_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin x11`
libx11_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin x11`
libx11_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin x11`

# The xvideo plugin

libxvideo_plugin_a_SOURCES = $(SOURCES_xvideo)
libxvideo_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin xvideo`
libxvideo_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin xvideo`
libxvideo_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin xvideo`

libxvideo_pic_a_SOURCES = $(SOURCES_xvideo)
libxvideo_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic xvideo`
libxvideo_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic xvideo`
libxvideo_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic xvideo`

libxvideo_a_SOURCES = $(SOURCES_xvideo)
libxvideo_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin xvideo`
libxvideo_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin xvideo`
libxvideo_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin xvideo`

# The glx plugin

libglx_plugin_a_SOURCES = $(SOURCES_glx)
libglx_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin glx`
libglx_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin glx`
libglx_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin glx`

libglx_pic_a_SOURCES = $(SOURCES_glx)
libglx_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic glx`
libglx_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic glx`
libglx_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic glx`

libglx_a_SOURCES = $(SOURCES_glx)
libglx_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin glx`
libglx_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin glx`
libglx_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin glx`

