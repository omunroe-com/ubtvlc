
# modules/misc/xml/Makefile.am automatically generated from modules/misc/xml/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = misc
mods = xtag xml

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = 
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libxtag_plugin.a libxtag.a libxtag_pic.a libxml_plugin.a libxml.a libxml_pic.a
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

# The xtag plugin

libxtag_plugin_a_SOURCES = $(SOURCES_xtag)
libxtag_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin xtag`
libxtag_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin xtag`
libxtag_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin xtag`

libxtag_pic_a_SOURCES = $(SOURCES_xtag)
libxtag_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic xtag`
libxtag_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic xtag`
libxtag_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic xtag`

libxtag_a_SOURCES = $(SOURCES_xtag)
libxtag_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin xtag`
libxtag_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin xtag`
libxtag_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin xtag`

# The xml plugin

libxml_plugin_a_SOURCES = $(SOURCES_xml)
libxml_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin xml`
libxml_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin xml`
libxml_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin xml`

libxml_pic_a_SOURCES = $(SOURCES_xml)
libxml_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic xml`
libxml_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic xml`
libxml_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic xml`

libxml_a_SOURCES = $(SOURCES_xml)
libxml_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin xml`
libxml_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin xml`
libxml_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin xml`

