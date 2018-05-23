
# modules/misc/memcpy/Makefile.am automatically generated from modules/misc/memcpy/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = misc
mods = memcpy memcpymmx memcpymmxext memcpy3dn memcpyaltivec

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = 
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libmemcpy_plugin.a libmemcpy.a libmemcpy_pic.a libmemcpymmx_plugin.a libmemcpymmx.a libmemcpymmx_pic.a libmemcpymmxext_plugin.a libmemcpymmxext.a libmemcpymmxext_pic.a libmemcpy3dn_plugin.a libmemcpy3dn.a libmemcpy3dn_pic.a libmemcpyaltivec_plugin.a libmemcpyaltivec.a libmemcpyaltivec_pic.a
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

# The memcpy plugin

libmemcpy_plugin_a_SOURCES = $(SOURCES_memcpy)
libmemcpy_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin memcpy`
libmemcpy_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin memcpy`
libmemcpy_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin memcpy`

libmemcpy_pic_a_SOURCES = $(SOURCES_memcpy)
libmemcpy_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic memcpy`
libmemcpy_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic memcpy`
libmemcpy_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic memcpy`

libmemcpy_a_SOURCES = $(SOURCES_memcpy)
libmemcpy_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin memcpy`
libmemcpy_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin memcpy`
libmemcpy_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin memcpy`

# The memcpymmx plugin

libmemcpymmx_plugin_a_SOURCES = $(SOURCES_memcpymmx)
libmemcpymmx_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin memcpymmx`
libmemcpymmx_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin memcpymmx`
libmemcpymmx_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin memcpymmx`

libmemcpymmx_pic_a_SOURCES = $(SOURCES_memcpymmx)
libmemcpymmx_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic memcpymmx`
libmemcpymmx_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic memcpymmx`
libmemcpymmx_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic memcpymmx`

libmemcpymmx_a_SOURCES = $(SOURCES_memcpymmx)
libmemcpymmx_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin memcpymmx`
libmemcpymmx_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin memcpymmx`
libmemcpymmx_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin memcpymmx`

# The memcpymmxext plugin

libmemcpymmxext_plugin_a_SOURCES = $(SOURCES_memcpymmxext)
libmemcpymmxext_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin memcpymmxext`
libmemcpymmxext_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin memcpymmxext`
libmemcpymmxext_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin memcpymmxext`

libmemcpymmxext_pic_a_SOURCES = $(SOURCES_memcpymmxext)
libmemcpymmxext_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic memcpymmxext`
libmemcpymmxext_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic memcpymmxext`
libmemcpymmxext_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic memcpymmxext`

libmemcpymmxext_a_SOURCES = $(SOURCES_memcpymmxext)
libmemcpymmxext_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin memcpymmxext`
libmemcpymmxext_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin memcpymmxext`
libmemcpymmxext_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin memcpymmxext`

# The memcpy3dn plugin

libmemcpy3dn_plugin_a_SOURCES = $(SOURCES_memcpy3dn)
libmemcpy3dn_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin memcpy3dn`
libmemcpy3dn_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin memcpy3dn`
libmemcpy3dn_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin memcpy3dn`

libmemcpy3dn_pic_a_SOURCES = $(SOURCES_memcpy3dn)
libmemcpy3dn_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic memcpy3dn`
libmemcpy3dn_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic memcpy3dn`
libmemcpy3dn_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic memcpy3dn`

libmemcpy3dn_a_SOURCES = $(SOURCES_memcpy3dn)
libmemcpy3dn_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin memcpy3dn`
libmemcpy3dn_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin memcpy3dn`
libmemcpy3dn_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin memcpy3dn`

# The memcpyaltivec plugin

libmemcpyaltivec_plugin_a_SOURCES = $(SOURCES_memcpyaltivec)
libmemcpyaltivec_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin memcpyaltivec`
libmemcpyaltivec_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin memcpyaltivec`
libmemcpyaltivec_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin memcpyaltivec`

libmemcpyaltivec_pic_a_SOURCES = $(SOURCES_memcpyaltivec)
libmemcpyaltivec_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic memcpyaltivec`
libmemcpyaltivec_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic memcpyaltivec`
libmemcpyaltivec_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic memcpyaltivec`

libmemcpyaltivec_a_SOURCES = $(SOURCES_memcpyaltivec)
libmemcpyaltivec_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin memcpyaltivec`
libmemcpyaltivec_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin memcpyaltivec`
libmemcpyaltivec_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin memcpyaltivec`

