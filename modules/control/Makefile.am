
# modules/control/Makefile.am automatically generated from modules/control/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = control
mods = gestures showintf telnet netsync ntservice hotkeys lirc rc

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = http corba
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libgestures_plugin.a libgestures.a libgestures_pic.a libshowintf_plugin.a libshowintf.a libshowintf_pic.a libtelnet_plugin.a libtelnet.a libtelnet_pic.a libnetsync_plugin.a libnetsync.a libnetsync_pic.a libntservice_plugin.a libntservice.a libntservice_pic.a libhotkeys_plugin.a libhotkeys.a libhotkeys_pic.a liblirc_plugin.a liblirc.a liblirc_pic.a librc_plugin.a librc.a librc_pic.a
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

# The gestures plugin

libgestures_plugin_a_SOURCES = $(SOURCES_gestures)
libgestures_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gestures`
libgestures_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gestures`
libgestures_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gestures`

libgestures_pic_a_SOURCES = $(SOURCES_gestures)
libgestures_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gestures`
libgestures_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gestures`
libgestures_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gestures`

libgestures_a_SOURCES = $(SOURCES_gestures)
libgestures_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gestures`
libgestures_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gestures`
libgestures_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gestures`

# The showintf plugin

libshowintf_plugin_a_SOURCES = $(SOURCES_showintf)
libshowintf_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin showintf`
libshowintf_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin showintf`
libshowintf_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin showintf`

libshowintf_pic_a_SOURCES = $(SOURCES_showintf)
libshowintf_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic showintf`
libshowintf_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic showintf`
libshowintf_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic showintf`

libshowintf_a_SOURCES = $(SOURCES_showintf)
libshowintf_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin showintf`
libshowintf_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin showintf`
libshowintf_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin showintf`

# The telnet plugin

libtelnet_plugin_a_SOURCES = $(SOURCES_telnet)
libtelnet_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin telnet`
libtelnet_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin telnet`
libtelnet_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin telnet`

libtelnet_pic_a_SOURCES = $(SOURCES_telnet)
libtelnet_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic telnet`
libtelnet_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic telnet`
libtelnet_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic telnet`

libtelnet_a_SOURCES = $(SOURCES_telnet)
libtelnet_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin telnet`
libtelnet_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin telnet`
libtelnet_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin telnet`

# The netsync plugin

libnetsync_plugin_a_SOURCES = $(SOURCES_netsync)
libnetsync_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin netsync`
libnetsync_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin netsync`
libnetsync_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin netsync`

libnetsync_pic_a_SOURCES = $(SOURCES_netsync)
libnetsync_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic netsync`
libnetsync_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic netsync`
libnetsync_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic netsync`

libnetsync_a_SOURCES = $(SOURCES_netsync)
libnetsync_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin netsync`
libnetsync_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin netsync`
libnetsync_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin netsync`

# The ntservice plugin

libntservice_plugin_a_SOURCES = $(SOURCES_ntservice)
libntservice_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin ntservice`
libntservice_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin ntservice`
libntservice_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin ntservice`

libntservice_pic_a_SOURCES = $(SOURCES_ntservice)
libntservice_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic ntservice`
libntservice_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic ntservice`
libntservice_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic ntservice`

libntservice_a_SOURCES = $(SOURCES_ntservice)
libntservice_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin ntservice`
libntservice_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin ntservice`
libntservice_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin ntservice`

# The hotkeys plugin

libhotkeys_plugin_a_SOURCES = $(SOURCES_hotkeys)
libhotkeys_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin hotkeys`
libhotkeys_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin hotkeys`
libhotkeys_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin hotkeys`

libhotkeys_pic_a_SOURCES = $(SOURCES_hotkeys)
libhotkeys_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic hotkeys`
libhotkeys_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic hotkeys`
libhotkeys_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic hotkeys`

libhotkeys_a_SOURCES = $(SOURCES_hotkeys)
libhotkeys_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin hotkeys`
libhotkeys_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin hotkeys`
libhotkeys_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin hotkeys`

# The lirc plugin

liblirc_plugin_a_SOURCES = $(SOURCES_lirc)
liblirc_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin lirc`
liblirc_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin lirc`
liblirc_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin lirc`

liblirc_pic_a_SOURCES = $(SOURCES_lirc)
liblirc_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic lirc`
liblirc_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic lirc`
liblirc_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic lirc`

liblirc_a_SOURCES = $(SOURCES_lirc)
liblirc_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin lirc`
liblirc_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin lirc`
liblirc_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin lirc`

# The rc plugin

librc_plugin_a_SOURCES = $(SOURCES_rc)
librc_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin rc`
librc_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin rc`
librc_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin rc`

librc_pic_a_SOURCES = $(SOURCES_rc)
librc_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic rc`
librc_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic rc`
librc_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic rc`

librc_a_SOURCES = $(SOURCES_rc)
librc_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin rc`
librc_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin rc`
librc_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin rc`

