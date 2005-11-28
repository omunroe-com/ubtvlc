
# modules/demux/Makefile.am automatically generated from modules/demux/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = demux
mods = a52sys dtssys flac ogg m3u demuxdump rawdv au wav mkv livedotcom nsv real ts ps mod pva aiff sgimb mjpeg subtitle ty vobsub voc xa nuv nsc mpc

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = asf avi mp4 mpeg playlist util
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  liba52sys_plugin.a liba52sys.a liba52sys_pic.a libdtssys_plugin.a libdtssys.a libdtssys_pic.a libflac_plugin.a libflac.a libflac_pic.a libogg_plugin.a libogg.a libogg_pic.a libm3u_plugin.a libm3u.a libm3u_pic.a libdemuxdump_plugin.a libdemuxdump.a libdemuxdump_pic.a librawdv_plugin.a librawdv.a librawdv_pic.a libau_plugin.a libau.a libau_pic.a libwav_plugin.a libwav.a libwav_pic.a libmkv_plugin.a libmkv.a libmkv_pic.a liblivedotcom_plugin.a liblivedotcom.a liblivedotcom_pic.a libnsv_plugin.a libnsv.a libnsv_pic.a libreal_plugin.a libreal.a libreal_pic.a libts_plugin.a libts.a libts_pic.a libps_plugin.a libps.a libps_pic.a libmod_plugin.a libmod.a libmod_pic.a libpva_plugin.a libpva.a libpva_pic.a libaiff_plugin.a libaiff.a libaiff_pic.a libsgimb_plugin.a libsgimb.a libsgimb_pic.a libmjpeg_plugin.a libmjpeg.a libmjpeg_pic.a libsubtitle_plugin.a libsubtitle.a libsubtitle_pic.a libty_plugin.a libty.a libty_pic.a libvobsub_plugin.a libvobsub.a libvobsub_pic.a libvoc_plugin.a libvoc.a libvoc_pic.a libxa_plugin.a libxa.a libxa_pic.a libnuv_plugin.a libnuv.a libnuv_pic.a libnsc_plugin.a libnsc.a libnsc_pic.a libmpc_plugin.a libmpc.a libmpc_pic.a
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

# The a52sys plugin

liba52sys_plugin_a_SOURCES = $(SOURCES_a52sys)
liba52sys_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin a52sys`
liba52sys_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin a52sys`
liba52sys_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin a52sys`

liba52sys_pic_a_SOURCES = $(SOURCES_a52sys)
liba52sys_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic a52sys`
liba52sys_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic a52sys`
liba52sys_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic a52sys`

liba52sys_a_SOURCES = $(SOURCES_a52sys)
liba52sys_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin a52sys`
liba52sys_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin a52sys`
liba52sys_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin a52sys`

# The dtssys plugin

libdtssys_plugin_a_SOURCES = $(SOURCES_dtssys)
libdtssys_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin dtssys`
libdtssys_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin dtssys`
libdtssys_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin dtssys`

libdtssys_pic_a_SOURCES = $(SOURCES_dtssys)
libdtssys_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic dtssys`
libdtssys_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic dtssys`
libdtssys_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic dtssys`

libdtssys_a_SOURCES = $(SOURCES_dtssys)
libdtssys_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin dtssys`
libdtssys_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin dtssys`
libdtssys_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin dtssys`

# The flac plugin

libflac_plugin_a_SOURCES = $(SOURCES_flac)
libflac_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin flac`
libflac_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin flac`
libflac_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin flac`

libflac_pic_a_SOURCES = $(SOURCES_flac)
libflac_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic flac`
libflac_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic flac`
libflac_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic flac`

libflac_a_SOURCES = $(SOURCES_flac)
libflac_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin flac`
libflac_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin flac`
libflac_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin flac`

# The ogg plugin

libogg_plugin_a_SOURCES = $(SOURCES_ogg)
libogg_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin ogg`
libogg_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin ogg`
libogg_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin ogg`

libogg_pic_a_SOURCES = $(SOURCES_ogg)
libogg_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic ogg`
libogg_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic ogg`
libogg_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic ogg`

libogg_a_SOURCES = $(SOURCES_ogg)
libogg_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin ogg`
libogg_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin ogg`
libogg_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin ogg`

# The m3u plugin

libm3u_plugin_a_SOURCES = $(SOURCES_m3u)
libm3u_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin m3u`
libm3u_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin m3u`
libm3u_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin m3u`

libm3u_pic_a_SOURCES = $(SOURCES_m3u)
libm3u_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic m3u`
libm3u_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic m3u`
libm3u_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic m3u`

libm3u_a_SOURCES = $(SOURCES_m3u)
libm3u_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin m3u`
libm3u_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin m3u`
libm3u_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin m3u`

# The demuxdump plugin

libdemuxdump_plugin_a_SOURCES = $(SOURCES_demuxdump)
libdemuxdump_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin demuxdump`
libdemuxdump_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin demuxdump`
libdemuxdump_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin demuxdump`

libdemuxdump_pic_a_SOURCES = $(SOURCES_demuxdump)
libdemuxdump_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic demuxdump`
libdemuxdump_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic demuxdump`
libdemuxdump_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic demuxdump`

libdemuxdump_a_SOURCES = $(SOURCES_demuxdump)
libdemuxdump_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin demuxdump`
libdemuxdump_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin demuxdump`
libdemuxdump_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin demuxdump`

# The rawdv plugin

librawdv_plugin_a_SOURCES = $(SOURCES_rawdv)
librawdv_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin rawdv`
librawdv_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin rawdv`
librawdv_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin rawdv`

librawdv_pic_a_SOURCES = $(SOURCES_rawdv)
librawdv_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic rawdv`
librawdv_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic rawdv`
librawdv_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic rawdv`

librawdv_a_SOURCES = $(SOURCES_rawdv)
librawdv_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin rawdv`
librawdv_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin rawdv`
librawdv_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin rawdv`

# The au plugin

libau_plugin_a_SOURCES = $(SOURCES_au)
libau_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin au`
libau_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin au`
libau_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin au`

libau_pic_a_SOURCES = $(SOURCES_au)
libau_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic au`
libau_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic au`
libau_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic au`

libau_a_SOURCES = $(SOURCES_au)
libau_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin au`
libau_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin au`
libau_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin au`

# The wav plugin

libwav_plugin_a_SOURCES = $(SOURCES_wav)
libwav_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin wav`
libwav_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin wav`
libwav_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin wav`

libwav_pic_a_SOURCES = $(SOURCES_wav)
libwav_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic wav`
libwav_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic wav`
libwav_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic wav`

libwav_a_SOURCES = $(SOURCES_wav)
libwav_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin wav`
libwav_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin wav`
libwav_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin wav`

# The mkv plugin

libmkv_plugin_a_SOURCES = $(SOURCES_mkv)
libmkv_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin mkv`
libmkv_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin mkv`
libmkv_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin mkv`

libmkv_pic_a_SOURCES = $(SOURCES_mkv)
libmkv_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic mkv`
libmkv_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic mkv`
libmkv_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic mkv`

libmkv_a_SOURCES = $(SOURCES_mkv)
libmkv_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin mkv`
libmkv_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin mkv`
libmkv_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin mkv`

# The livedotcom plugin

liblivedotcom_plugin_a_SOURCES = $(SOURCES_livedotcom)
liblivedotcom_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin livedotcom`
liblivedotcom_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin livedotcom`
liblivedotcom_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin livedotcom`

liblivedotcom_pic_a_SOURCES = $(SOURCES_livedotcom)
liblivedotcom_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic livedotcom`
liblivedotcom_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic livedotcom`
liblivedotcom_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic livedotcom`

liblivedotcom_a_SOURCES = $(SOURCES_livedotcom)
liblivedotcom_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin livedotcom`
liblivedotcom_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin livedotcom`
liblivedotcom_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin livedotcom`

# The nsv plugin

libnsv_plugin_a_SOURCES = $(SOURCES_nsv)
libnsv_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin nsv`
libnsv_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin nsv`
libnsv_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin nsv`

libnsv_pic_a_SOURCES = $(SOURCES_nsv)
libnsv_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic nsv`
libnsv_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic nsv`
libnsv_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic nsv`

libnsv_a_SOURCES = $(SOURCES_nsv)
libnsv_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin nsv`
libnsv_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin nsv`
libnsv_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin nsv`

# The real plugin

libreal_plugin_a_SOURCES = $(SOURCES_real)
libreal_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin real`
libreal_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin real`
libreal_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin real`

libreal_pic_a_SOURCES = $(SOURCES_real)
libreal_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic real`
libreal_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic real`
libreal_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic real`

libreal_a_SOURCES = $(SOURCES_real)
libreal_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin real`
libreal_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin real`
libreal_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin real`

# The ts plugin

libts_plugin_a_SOURCES = $(SOURCES_ts)
libts_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin ts`
libts_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin ts`
libts_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin ts`

libts_pic_a_SOURCES = $(SOURCES_ts)
libts_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic ts`
libts_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic ts`
libts_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic ts`

libts_a_SOURCES = $(SOURCES_ts)
libts_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin ts`
libts_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin ts`
libts_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin ts`

# The ps plugin

libps_plugin_a_SOURCES = $(SOURCES_ps)
libps_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin ps`
libps_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin ps`
libps_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin ps`

libps_pic_a_SOURCES = $(SOURCES_ps)
libps_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic ps`
libps_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic ps`
libps_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic ps`

libps_a_SOURCES = $(SOURCES_ps)
libps_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin ps`
libps_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin ps`
libps_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin ps`

# The mod plugin

libmod_plugin_a_SOURCES = $(SOURCES_mod)
libmod_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin mod`
libmod_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin mod`
libmod_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin mod`

libmod_pic_a_SOURCES = $(SOURCES_mod)
libmod_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic mod`
libmod_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic mod`
libmod_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic mod`

libmod_a_SOURCES = $(SOURCES_mod)
libmod_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin mod`
libmod_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin mod`
libmod_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin mod`

# The pva plugin

libpva_plugin_a_SOURCES = $(SOURCES_pva)
libpva_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin pva`
libpva_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin pva`
libpva_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin pva`

libpva_pic_a_SOURCES = $(SOURCES_pva)
libpva_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic pva`
libpva_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic pva`
libpva_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic pva`

libpva_a_SOURCES = $(SOURCES_pva)
libpva_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pva`
libpva_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pva`
libpva_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pva`

# The aiff plugin

libaiff_plugin_a_SOURCES = $(SOURCES_aiff)
libaiff_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin aiff`
libaiff_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin aiff`
libaiff_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin aiff`

libaiff_pic_a_SOURCES = $(SOURCES_aiff)
libaiff_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic aiff`
libaiff_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic aiff`
libaiff_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic aiff`

libaiff_a_SOURCES = $(SOURCES_aiff)
libaiff_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin aiff`
libaiff_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin aiff`
libaiff_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin aiff`

# The sgimb plugin

libsgimb_plugin_a_SOURCES = $(SOURCES_sgimb)
libsgimb_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin sgimb`
libsgimb_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin sgimb`
libsgimb_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin sgimb`

libsgimb_pic_a_SOURCES = $(SOURCES_sgimb)
libsgimb_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic sgimb`
libsgimb_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic sgimb`
libsgimb_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic sgimb`

libsgimb_a_SOURCES = $(SOURCES_sgimb)
libsgimb_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin sgimb`
libsgimb_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin sgimb`
libsgimb_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin sgimb`

# The mjpeg plugin

libmjpeg_plugin_a_SOURCES = $(SOURCES_mjpeg)
libmjpeg_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin mjpeg`
libmjpeg_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin mjpeg`
libmjpeg_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin mjpeg`

libmjpeg_pic_a_SOURCES = $(SOURCES_mjpeg)
libmjpeg_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic mjpeg`
libmjpeg_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic mjpeg`
libmjpeg_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic mjpeg`

libmjpeg_a_SOURCES = $(SOURCES_mjpeg)
libmjpeg_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin mjpeg`
libmjpeg_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin mjpeg`
libmjpeg_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin mjpeg`

# The subtitle plugin

libsubtitle_plugin_a_SOURCES = $(SOURCES_subtitle)
libsubtitle_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin subtitle`
libsubtitle_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin subtitle`
libsubtitle_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin subtitle`

libsubtitle_pic_a_SOURCES = $(SOURCES_subtitle)
libsubtitle_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic subtitle`
libsubtitle_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic subtitle`
libsubtitle_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic subtitle`

libsubtitle_a_SOURCES = $(SOURCES_subtitle)
libsubtitle_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin subtitle`
libsubtitle_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin subtitle`
libsubtitle_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin subtitle`

# The ty plugin

libty_plugin_a_SOURCES = $(SOURCES_ty)
libty_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin ty`
libty_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin ty`
libty_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin ty`

libty_pic_a_SOURCES = $(SOURCES_ty)
libty_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic ty`
libty_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic ty`
libty_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic ty`

libty_a_SOURCES = $(SOURCES_ty)
libty_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin ty`
libty_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin ty`
libty_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin ty`

# The vobsub plugin

libvobsub_plugin_a_SOURCES = $(SOURCES_vobsub)
libvobsub_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin vobsub`
libvobsub_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin vobsub`
libvobsub_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin vobsub`

libvobsub_pic_a_SOURCES = $(SOURCES_vobsub)
libvobsub_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic vobsub`
libvobsub_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic vobsub`
libvobsub_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic vobsub`

libvobsub_a_SOURCES = $(SOURCES_vobsub)
libvobsub_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin vobsub`
libvobsub_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin vobsub`
libvobsub_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin vobsub`

# The voc plugin

libvoc_plugin_a_SOURCES = $(SOURCES_voc)
libvoc_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin voc`
libvoc_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin voc`
libvoc_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin voc`

libvoc_pic_a_SOURCES = $(SOURCES_voc)
libvoc_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic voc`
libvoc_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic voc`
libvoc_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic voc`

libvoc_a_SOURCES = $(SOURCES_voc)
libvoc_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin voc`
libvoc_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin voc`
libvoc_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin voc`

# The xa plugin

libxa_plugin_a_SOURCES = $(SOURCES_xa)
libxa_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin xa`
libxa_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin xa`
libxa_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin xa`

libxa_pic_a_SOURCES = $(SOURCES_xa)
libxa_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic xa`
libxa_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic xa`
libxa_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic xa`

libxa_a_SOURCES = $(SOURCES_xa)
libxa_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin xa`
libxa_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin xa`
libxa_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin xa`

# The nuv plugin

libnuv_plugin_a_SOURCES = $(SOURCES_nuv)
libnuv_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin nuv`
libnuv_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin nuv`
libnuv_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin nuv`

libnuv_pic_a_SOURCES = $(SOURCES_nuv)
libnuv_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic nuv`
libnuv_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic nuv`
libnuv_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic nuv`

libnuv_a_SOURCES = $(SOURCES_nuv)
libnuv_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin nuv`
libnuv_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin nuv`
libnuv_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin nuv`

# The nsc plugin

libnsc_plugin_a_SOURCES = $(SOURCES_nsc)
libnsc_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin nsc`
libnsc_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin nsc`
libnsc_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin nsc`

libnsc_pic_a_SOURCES = $(SOURCES_nsc)
libnsc_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic nsc`
libnsc_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic nsc`
libnsc_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic nsc`

libnsc_a_SOURCES = $(SOURCES_nsc)
libnsc_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin nsc`
libnsc_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin nsc`
libnsc_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin nsc`

# The mpc plugin

libmpc_plugin_a_SOURCES = $(SOURCES_mpc)
libmpc_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin mpc`
libmpc_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin mpc`
libmpc_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin mpc`

libmpc_pic_a_SOURCES = $(SOURCES_mpc)
libmpc_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic mpc`
libmpc_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic mpc`
libmpc_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic mpc`

libmpc_a_SOURCES = $(SOURCES_mpc)
libmpc_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin mpc`
libmpc_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin mpc`
libmpc_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin mpc`

