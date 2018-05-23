
# modules/misc/network/Makefile.am automatically generated from modules/misc/network/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = misc
mods = ipv4 ipv6

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = 
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libipv4_plugin.a libipv4.a libipv4_pic.a libipv6_plugin.a libipv6.a libipv6_pic.a
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

# The ipv4 plugin

libipv4_plugin_a_SOURCES = $(SOURCES_ipv4)
libipv4_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin ipv4`
libipv4_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin ipv4`
libipv4_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin ipv4`

libipv4_pic_a_SOURCES = $(SOURCES_ipv4)
libipv4_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic ipv4`
libipv4_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic ipv4`
libipv4_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic ipv4`

libipv4_a_SOURCES = $(SOURCES_ipv4)
libipv4_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin ipv4`
libipv4_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin ipv4`
libipv4_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin ipv4`

# The ipv6 plugin

libipv6_plugin_a_SOURCES = $(SOURCES_ipv6)
libipv6_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin ipv6`
libipv6_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin ipv6`
libipv6_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin ipv6`

libipv6_pic_a_SOURCES = $(SOURCES_ipv6)
libipv6_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic ipv6`
libipv6_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic ipv6`
libipv6_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic ipv6`

libipv6_a_SOURCES = $(SOURCES_ipv6)
libipv6_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin ipv6`
libipv6_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin ipv6`
libipv6_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin ipv6`

