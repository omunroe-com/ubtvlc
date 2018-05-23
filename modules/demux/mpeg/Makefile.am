
# modules/demux/mpeg/Makefile.am automatically generated from modules/demux/mpeg/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = demux
mods = m4a m4v mpga mpgv h264

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = 
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libm4a_plugin.a libm4a.a libm4a_pic.a libm4v_plugin.a libm4v.a libm4v_pic.a libmpga_plugin.a libmpga.a libmpga_pic.a libmpgv_plugin.a libmpgv.a libmpgv_pic.a libh264_plugin.a libh264.a libh264_pic.a
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

# The m4a plugin

libm4a_plugin_a_SOURCES = $(SOURCES_m4a)
libm4a_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin m4a`
libm4a_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin m4a`
libm4a_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin m4a`

libm4a_pic_a_SOURCES = $(SOURCES_m4a)
libm4a_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic m4a`
libm4a_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic m4a`
libm4a_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic m4a`

libm4a_a_SOURCES = $(SOURCES_m4a)
libm4a_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin m4a`
libm4a_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin m4a`
libm4a_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin m4a`

# The m4v plugin

libm4v_plugin_a_SOURCES = $(SOURCES_m4v)
libm4v_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin m4v`
libm4v_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin m4v`
libm4v_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin m4v`

libm4v_pic_a_SOURCES = $(SOURCES_m4v)
libm4v_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic m4v`
libm4v_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic m4v`
libm4v_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic m4v`

libm4v_a_SOURCES = $(SOURCES_m4v)
libm4v_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin m4v`
libm4v_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin m4v`
libm4v_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin m4v`

# The mpga plugin

libmpga_plugin_a_SOURCES = $(SOURCES_mpga)
libmpga_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin mpga`
libmpga_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin mpga`
libmpga_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin mpga`

libmpga_pic_a_SOURCES = $(SOURCES_mpga)
libmpga_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic mpga`
libmpga_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic mpga`
libmpga_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic mpga`

libmpga_a_SOURCES = $(SOURCES_mpga)
libmpga_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin mpga`
libmpga_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin mpga`
libmpga_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin mpga`

# The mpgv plugin

libmpgv_plugin_a_SOURCES = $(SOURCES_mpgv)
libmpgv_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin mpgv`
libmpgv_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin mpgv`
libmpgv_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin mpgv`

libmpgv_pic_a_SOURCES = $(SOURCES_mpgv)
libmpgv_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic mpgv`
libmpgv_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic mpgv`
libmpgv_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic mpgv`

libmpgv_a_SOURCES = $(SOURCES_mpgv)
libmpgv_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin mpgv`
libmpgv_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin mpgv`
libmpgv_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin mpgv`

# The h264 plugin

libh264_plugin_a_SOURCES = $(SOURCES_h264)
libh264_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin h264`
libh264_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin h264`
libh264_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin h264`

libh264_pic_a_SOURCES = $(SOURCES_h264)
libh264_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic h264`
libh264_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic h264`
libh264_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic h264`

libh264_a_SOURCES = $(SOURCES_h264)
libh264_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin h264`
libh264_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin h264`
libh264_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin h264`

