
# modules/audio_filter/converter/Makefile.am automatically generated from modules/audio_filter/converter/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = audio_filter
mods = float32tos16 float32tos8 float32tou16 float32tou8 a52tospdif a52tofloat32 dtstospdif dtstofloat32 fixed32tos16 s16tofixed32 fixed32tofloat32 s16tofloat32 s16tofloat32swab s8tofloat32 u8tofixed32 u8tofloat32 mpgatofixed32

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = 
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libfloat32tos16_plugin.a libfloat32tos16.a libfloat32tos16_pic.a libfloat32tos8_plugin.a libfloat32tos8.a libfloat32tos8_pic.a libfloat32tou16_plugin.a libfloat32tou16.a libfloat32tou16_pic.a libfloat32tou8_plugin.a libfloat32tou8.a libfloat32tou8_pic.a liba52tospdif_plugin.a liba52tospdif.a liba52tospdif_pic.a liba52tofloat32_plugin.a liba52tofloat32.a liba52tofloat32_pic.a libdtstospdif_plugin.a libdtstospdif.a libdtstospdif_pic.a libdtstofloat32_plugin.a libdtstofloat32.a libdtstofloat32_pic.a libfixed32tos16_plugin.a libfixed32tos16.a libfixed32tos16_pic.a libs16tofixed32_plugin.a libs16tofixed32.a libs16tofixed32_pic.a libfixed32tofloat32_plugin.a libfixed32tofloat32.a libfixed32tofloat32_pic.a libs16tofloat32_plugin.a libs16tofloat32.a libs16tofloat32_pic.a libs16tofloat32swab_plugin.a libs16tofloat32swab.a libs16tofloat32swab_pic.a libs8tofloat32_plugin.a libs8tofloat32.a libs8tofloat32_pic.a libu8tofixed32_plugin.a libu8tofixed32.a libu8tofixed32_pic.a libu8tofloat32_plugin.a libu8tofloat32.a libu8tofloat32_pic.a libmpgatofixed32_plugin.a libmpgatofixed32.a libmpgatofixed32_pic.a
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

# The float32tos16 plugin

libfloat32tos16_plugin_a_SOURCES = $(SOURCES_float32tos16)
libfloat32tos16_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin float32tos16`
libfloat32tos16_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin float32tos16`
libfloat32tos16_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin float32tos16`

libfloat32tos16_pic_a_SOURCES = $(SOURCES_float32tos16)
libfloat32tos16_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic float32tos16`
libfloat32tos16_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic float32tos16`
libfloat32tos16_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic float32tos16`

libfloat32tos16_a_SOURCES = $(SOURCES_float32tos16)
libfloat32tos16_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin float32tos16`
libfloat32tos16_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin float32tos16`
libfloat32tos16_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin float32tos16`

# The float32tos8 plugin

libfloat32tos8_plugin_a_SOURCES = $(SOURCES_float32tos8)
libfloat32tos8_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin float32tos8`
libfloat32tos8_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin float32tos8`
libfloat32tos8_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin float32tos8`

libfloat32tos8_pic_a_SOURCES = $(SOURCES_float32tos8)
libfloat32tos8_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic float32tos8`
libfloat32tos8_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic float32tos8`
libfloat32tos8_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic float32tos8`

libfloat32tos8_a_SOURCES = $(SOURCES_float32tos8)
libfloat32tos8_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin float32tos8`
libfloat32tos8_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin float32tos8`
libfloat32tos8_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin float32tos8`

# The float32tou16 plugin

libfloat32tou16_plugin_a_SOURCES = $(SOURCES_float32tou16)
libfloat32tou16_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin float32tou16`
libfloat32tou16_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin float32tou16`
libfloat32tou16_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin float32tou16`

libfloat32tou16_pic_a_SOURCES = $(SOURCES_float32tou16)
libfloat32tou16_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic float32tou16`
libfloat32tou16_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic float32tou16`
libfloat32tou16_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic float32tou16`

libfloat32tou16_a_SOURCES = $(SOURCES_float32tou16)
libfloat32tou16_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin float32tou16`
libfloat32tou16_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin float32tou16`
libfloat32tou16_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin float32tou16`

# The float32tou8 plugin

libfloat32tou8_plugin_a_SOURCES = $(SOURCES_float32tou8)
libfloat32tou8_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin float32tou8`
libfloat32tou8_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin float32tou8`
libfloat32tou8_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin float32tou8`

libfloat32tou8_pic_a_SOURCES = $(SOURCES_float32tou8)
libfloat32tou8_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic float32tou8`
libfloat32tou8_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic float32tou8`
libfloat32tou8_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic float32tou8`

libfloat32tou8_a_SOURCES = $(SOURCES_float32tou8)
libfloat32tou8_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin float32tou8`
libfloat32tou8_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin float32tou8`
libfloat32tou8_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin float32tou8`

# The a52tospdif plugin

liba52tospdif_plugin_a_SOURCES = $(SOURCES_a52tospdif)
liba52tospdif_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin a52tospdif`
liba52tospdif_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin a52tospdif`
liba52tospdif_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin a52tospdif`

liba52tospdif_pic_a_SOURCES = $(SOURCES_a52tospdif)
liba52tospdif_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic a52tospdif`
liba52tospdif_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic a52tospdif`
liba52tospdif_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic a52tospdif`

liba52tospdif_a_SOURCES = $(SOURCES_a52tospdif)
liba52tospdif_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin a52tospdif`
liba52tospdif_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin a52tospdif`
liba52tospdif_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin a52tospdif`

# The a52tofloat32 plugin

liba52tofloat32_plugin_a_SOURCES = $(SOURCES_a52tofloat32)
liba52tofloat32_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin a52tofloat32`
liba52tofloat32_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin a52tofloat32`
liba52tofloat32_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin a52tofloat32`

liba52tofloat32_pic_a_SOURCES = $(SOURCES_a52tofloat32)
liba52tofloat32_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic a52tofloat32`
liba52tofloat32_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic a52tofloat32`
liba52tofloat32_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic a52tofloat32`

liba52tofloat32_a_SOURCES = $(SOURCES_a52tofloat32)
liba52tofloat32_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin a52tofloat32`
liba52tofloat32_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin a52tofloat32`
liba52tofloat32_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin a52tofloat32`

# The dtstospdif plugin

libdtstospdif_plugin_a_SOURCES = $(SOURCES_dtstospdif)
libdtstospdif_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin dtstospdif`
libdtstospdif_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin dtstospdif`
libdtstospdif_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin dtstospdif`

libdtstospdif_pic_a_SOURCES = $(SOURCES_dtstospdif)
libdtstospdif_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic dtstospdif`
libdtstospdif_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic dtstospdif`
libdtstospdif_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic dtstospdif`

libdtstospdif_a_SOURCES = $(SOURCES_dtstospdif)
libdtstospdif_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin dtstospdif`
libdtstospdif_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin dtstospdif`
libdtstospdif_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin dtstospdif`

# The dtstofloat32 plugin

libdtstofloat32_plugin_a_SOURCES = $(SOURCES_dtstofloat32)
libdtstofloat32_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin dtstofloat32`
libdtstofloat32_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin dtstofloat32`
libdtstofloat32_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin dtstofloat32`

libdtstofloat32_pic_a_SOURCES = $(SOURCES_dtstofloat32)
libdtstofloat32_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic dtstofloat32`
libdtstofloat32_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic dtstofloat32`
libdtstofloat32_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic dtstofloat32`

libdtstofloat32_a_SOURCES = $(SOURCES_dtstofloat32)
libdtstofloat32_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin dtstofloat32`
libdtstofloat32_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin dtstofloat32`
libdtstofloat32_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin dtstofloat32`

# The fixed32tos16 plugin

libfixed32tos16_plugin_a_SOURCES = $(SOURCES_fixed32tos16)
libfixed32tos16_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin fixed32tos16`
libfixed32tos16_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin fixed32tos16`
libfixed32tos16_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin fixed32tos16`

libfixed32tos16_pic_a_SOURCES = $(SOURCES_fixed32tos16)
libfixed32tos16_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic fixed32tos16`
libfixed32tos16_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic fixed32tos16`
libfixed32tos16_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic fixed32tos16`

libfixed32tos16_a_SOURCES = $(SOURCES_fixed32tos16)
libfixed32tos16_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin fixed32tos16`
libfixed32tos16_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin fixed32tos16`
libfixed32tos16_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin fixed32tos16`

# The s16tofixed32 plugin

libs16tofixed32_plugin_a_SOURCES = $(SOURCES_s16tofixed32)
libs16tofixed32_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin s16tofixed32`
libs16tofixed32_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin s16tofixed32`
libs16tofixed32_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin s16tofixed32`

libs16tofixed32_pic_a_SOURCES = $(SOURCES_s16tofixed32)
libs16tofixed32_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic s16tofixed32`
libs16tofixed32_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic s16tofixed32`
libs16tofixed32_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic s16tofixed32`

libs16tofixed32_a_SOURCES = $(SOURCES_s16tofixed32)
libs16tofixed32_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin s16tofixed32`
libs16tofixed32_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin s16tofixed32`
libs16tofixed32_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin s16tofixed32`

# The fixed32tofloat32 plugin

libfixed32tofloat32_plugin_a_SOURCES = $(SOURCES_fixed32tofloat32)
libfixed32tofloat32_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin fixed32tofloat32`
libfixed32tofloat32_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin fixed32tofloat32`
libfixed32tofloat32_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin fixed32tofloat32`

libfixed32tofloat32_pic_a_SOURCES = $(SOURCES_fixed32tofloat32)
libfixed32tofloat32_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic fixed32tofloat32`
libfixed32tofloat32_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic fixed32tofloat32`
libfixed32tofloat32_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic fixed32tofloat32`

libfixed32tofloat32_a_SOURCES = $(SOURCES_fixed32tofloat32)
libfixed32tofloat32_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin fixed32tofloat32`
libfixed32tofloat32_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin fixed32tofloat32`
libfixed32tofloat32_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin fixed32tofloat32`

# The s16tofloat32 plugin

libs16tofloat32_plugin_a_SOURCES = $(SOURCES_s16tofloat32)
libs16tofloat32_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin s16tofloat32`
libs16tofloat32_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin s16tofloat32`
libs16tofloat32_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin s16tofloat32`

libs16tofloat32_pic_a_SOURCES = $(SOURCES_s16tofloat32)
libs16tofloat32_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic s16tofloat32`
libs16tofloat32_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic s16tofloat32`
libs16tofloat32_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic s16tofloat32`

libs16tofloat32_a_SOURCES = $(SOURCES_s16tofloat32)
libs16tofloat32_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin s16tofloat32`
libs16tofloat32_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin s16tofloat32`
libs16tofloat32_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin s16tofloat32`

# The s16tofloat32swab plugin

libs16tofloat32swab_plugin_a_SOURCES = $(SOURCES_s16tofloat32swab)
libs16tofloat32swab_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin s16tofloat32swab`
libs16tofloat32swab_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin s16tofloat32swab`
libs16tofloat32swab_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin s16tofloat32swab`

libs16tofloat32swab_pic_a_SOURCES = $(SOURCES_s16tofloat32swab)
libs16tofloat32swab_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic s16tofloat32swab`
libs16tofloat32swab_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic s16tofloat32swab`
libs16tofloat32swab_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic s16tofloat32swab`

libs16tofloat32swab_a_SOURCES = $(SOURCES_s16tofloat32swab)
libs16tofloat32swab_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin s16tofloat32swab`
libs16tofloat32swab_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin s16tofloat32swab`
libs16tofloat32swab_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin s16tofloat32swab`

# The s8tofloat32 plugin

libs8tofloat32_plugin_a_SOURCES = $(SOURCES_s8tofloat32)
libs8tofloat32_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin s8tofloat32`
libs8tofloat32_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin s8tofloat32`
libs8tofloat32_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin s8tofloat32`

libs8tofloat32_pic_a_SOURCES = $(SOURCES_s8tofloat32)
libs8tofloat32_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic s8tofloat32`
libs8tofloat32_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic s8tofloat32`
libs8tofloat32_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic s8tofloat32`

libs8tofloat32_a_SOURCES = $(SOURCES_s8tofloat32)
libs8tofloat32_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin s8tofloat32`
libs8tofloat32_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin s8tofloat32`
libs8tofloat32_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin s8tofloat32`

# The u8tofixed32 plugin

libu8tofixed32_plugin_a_SOURCES = $(SOURCES_u8tofixed32)
libu8tofixed32_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin u8tofixed32`
libu8tofixed32_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin u8tofixed32`
libu8tofixed32_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin u8tofixed32`

libu8tofixed32_pic_a_SOURCES = $(SOURCES_u8tofixed32)
libu8tofixed32_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic u8tofixed32`
libu8tofixed32_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic u8tofixed32`
libu8tofixed32_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic u8tofixed32`

libu8tofixed32_a_SOURCES = $(SOURCES_u8tofixed32)
libu8tofixed32_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin u8tofixed32`
libu8tofixed32_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin u8tofixed32`
libu8tofixed32_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin u8tofixed32`

# The u8tofloat32 plugin

libu8tofloat32_plugin_a_SOURCES = $(SOURCES_u8tofloat32)
libu8tofloat32_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin u8tofloat32`
libu8tofloat32_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin u8tofloat32`
libu8tofloat32_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin u8tofloat32`

libu8tofloat32_pic_a_SOURCES = $(SOURCES_u8tofloat32)
libu8tofloat32_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic u8tofloat32`
libu8tofloat32_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic u8tofloat32`
libu8tofloat32_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic u8tofloat32`

libu8tofloat32_a_SOURCES = $(SOURCES_u8tofloat32)
libu8tofloat32_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin u8tofloat32`
libu8tofloat32_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin u8tofloat32`
libu8tofloat32_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin u8tofloat32`

# The mpgatofixed32 plugin

libmpgatofixed32_plugin_a_SOURCES = $(SOURCES_mpgatofixed32)
libmpgatofixed32_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin mpgatofixed32`
libmpgatofixed32_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin mpgatofixed32`
libmpgatofixed32_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin mpgatofixed32`

libmpgatofixed32_pic_a_SOURCES = $(SOURCES_mpgatofixed32)
libmpgatofixed32_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic mpgatofixed32`
libmpgatofixed32_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic mpgatofixed32`
libmpgatofixed32_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic mpgatofixed32`

libmpgatofixed32_a_SOURCES = $(SOURCES_mpgatofixed32)
libmpgatofixed32_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin mpgatofixed32`
libmpgatofixed32_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin mpgatofixed32`
libmpgatofixed32_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin mpgatofixed32`

