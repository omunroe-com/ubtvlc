
# modules/services_discovery/Makefile.am automatically generated from modules/services_discovery/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = services_discovery
mods = sap hal daap shout upnp bonjour

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = 
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libsap_plugin.a libsap.a libsap_pic.a libhal_plugin.a libhal.a libhal_pic.a libdaap_plugin.a libdaap.a libdaap_pic.a libshout_plugin.a libshout.a libshout_pic.a libupnp_plugin.a libupnp.a libupnp_pic.a libbonjour_plugin.a libbonjour.a libbonjour_pic.a
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

# The sap plugin

libsap_plugin_a_SOURCES = $(SOURCES_sap)
libsap_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin sap`
libsap_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin sap`
libsap_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin sap`

libsap_pic_a_SOURCES = $(SOURCES_sap)
libsap_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic sap`
libsap_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic sap`
libsap_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic sap`

libsap_a_SOURCES = $(SOURCES_sap)
libsap_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin sap`
libsap_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin sap`
libsap_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin sap`

# The hal plugin

libhal_plugin_a_SOURCES = $(SOURCES_hal)
libhal_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin hal`
libhal_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin hal`
libhal_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin hal`

libhal_pic_a_SOURCES = $(SOURCES_hal)
libhal_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic hal`
libhal_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic hal`
libhal_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic hal`

libhal_a_SOURCES = $(SOURCES_hal)
libhal_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin hal`
libhal_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin hal`
libhal_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin hal`

# The daap plugin

libdaap_plugin_a_SOURCES = $(SOURCES_daap)
libdaap_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin daap`
libdaap_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin daap`
libdaap_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin daap`

libdaap_pic_a_SOURCES = $(SOURCES_daap)
libdaap_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic daap`
libdaap_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic daap`
libdaap_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic daap`

libdaap_a_SOURCES = $(SOURCES_daap)
libdaap_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin daap`
libdaap_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin daap`
libdaap_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin daap`

# The shout plugin

libshout_plugin_a_SOURCES = $(SOURCES_shout)
libshout_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin shout`
libshout_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin shout`
libshout_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin shout`

libshout_pic_a_SOURCES = $(SOURCES_shout)
libshout_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic shout`
libshout_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic shout`
libshout_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic shout`

libshout_a_SOURCES = $(SOURCES_shout)
libshout_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin shout`
libshout_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin shout`
libshout_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin shout`

# The upnp plugin

libupnp_plugin_a_SOURCES = $(SOURCES_upnp)
libupnp_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin upnp`
libupnp_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin upnp`
libupnp_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin upnp`

libupnp_pic_a_SOURCES = $(SOURCES_upnp)
libupnp_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic upnp`
libupnp_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic upnp`
libupnp_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic upnp`

libupnp_a_SOURCES = $(SOURCES_upnp)
libupnp_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin upnp`
libupnp_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin upnp`
libupnp_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin upnp`

# The bonjour plugin

libbonjour_plugin_a_SOURCES = $(SOURCES_bonjour)
libbonjour_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin bonjour`
libbonjour_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin bonjour`
libbonjour_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin bonjour`

libbonjour_pic_a_SOURCES = $(SOURCES_bonjour)
libbonjour_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic bonjour`
libbonjour_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic bonjour`
libbonjour_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic bonjour`

libbonjour_a_SOURCES = $(SOURCES_bonjour)
libbonjour_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin bonjour`
libbonjour_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin bonjour`
libbonjour_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin bonjour`

