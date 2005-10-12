
# modules/visualization/Makefile.am automatically generated from modules/visualization/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = visualization
mods = goom xosd

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = visual galaktos
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libgoom_plugin.a libgoom.a libgoom_pic.a libxosd_plugin.a libxosd.a libxosd_pic.a
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

# The goom plugin

libgoom_plugin_a_SOURCES = $(SOURCES_goom)
libgoom_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin goom`
libgoom_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin goom`
libgoom_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin goom`

libgoom_pic_a_SOURCES = $(SOURCES_goom)
libgoom_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic goom`
libgoom_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic goom`
libgoom_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic goom`

libgoom_a_SOURCES = $(SOURCES_goom)
libgoom_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin goom`
libgoom_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin goom`
libgoom_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin goom`

# The xosd plugin

libxosd_plugin_a_SOURCES = $(SOURCES_xosd)
libxosd_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin xosd`
libxosd_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin xosd`
libxosd_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin xosd`

libxosd_pic_a_SOURCES = $(SOURCES_xosd)
libxosd_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic xosd`
libxosd_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic xosd`
libxosd_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic xosd`

libxosd_a_SOURCES = $(SOURCES_xosd)
libxosd_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin xosd`
libxosd_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin xosd`
libxosd_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin xosd`

