
# modules/codec/Makefile.am automatically generated from modules/codec/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = codec
mods = a52 cinepak dts flacdec lpcm araw vorbis tarkin theora tremor speex adpcm mpeg_audio libmpeg2 rawvideo quicktime subsdec faad dvbsub mash x264 twolame dirac png svcdsub cvdsub fake realaudio sdl_image

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = cmml dmo ffmpeg spudec
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  liba52_plugin.a liba52.a liba52_pic.a libcinepak_plugin.a libcinepak.a libcinepak_pic.a libdts_plugin.a libdts.a libdts_pic.a libflacdec_plugin.a libflacdec.a libflacdec_pic.a liblpcm_plugin.a liblpcm.a liblpcm_pic.a libaraw_plugin.a libaraw.a libaraw_pic.a libvorbis_plugin.a libvorbis.a libvorbis_pic.a libtarkin_plugin.a libtarkin.a libtarkin_pic.a libtheora_plugin.a libtheora.a libtheora_pic.a libtremor_plugin.a libtremor.a libtremor_pic.a libspeex_plugin.a libspeex.a libspeex_pic.a libadpcm_plugin.a libadpcm.a libadpcm_pic.a libmpeg_audio_plugin.a libmpeg_audio.a libmpeg_audio_pic.a liblibmpeg2_plugin.a liblibmpeg2.a liblibmpeg2_pic.a librawvideo_plugin.a librawvideo.a librawvideo_pic.a libquicktime_plugin.a libquicktime.a libquicktime_pic.a libsubsdec_plugin.a libsubsdec.a libsubsdec_pic.a libfaad_plugin.a libfaad.a libfaad_pic.a libdvbsub_plugin.a libdvbsub.a libdvbsub_pic.a libmash_plugin.a libmash.a libmash_pic.a libx264_plugin.a libx264.a libx264_pic.a libtwolame_plugin.a libtwolame.a libtwolame_pic.a libdirac_plugin.a libdirac.a libdirac_pic.a libpng_plugin.a libpng.a libpng_pic.a libsvcdsub_plugin.a libsvcdsub.a libsvcdsub_pic.a libcvdsub_plugin.a libcvdsub.a libcvdsub_pic.a libfake_plugin.a libfake.a libfake_pic.a librealaudio_plugin.a librealaudio.a librealaudio_pic.a libsdl_image_plugin.a libsdl_image.a libsdl_image_pic.a
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

# The a52 plugin

liba52_plugin_a_SOURCES = $(SOURCES_a52)
liba52_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin a52`
liba52_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin a52`
liba52_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin a52`

liba52_pic_a_SOURCES = $(SOURCES_a52)
liba52_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic a52`
liba52_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic a52`
liba52_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic a52`

liba52_a_SOURCES = $(SOURCES_a52)
liba52_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin a52`
liba52_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin a52`
liba52_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin a52`

# The cinepak plugin

libcinepak_plugin_a_SOURCES = $(SOURCES_cinepak)
libcinepak_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin cinepak`
libcinepak_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin cinepak`
libcinepak_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin cinepak`

libcinepak_pic_a_SOURCES = $(SOURCES_cinepak)
libcinepak_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic cinepak`
libcinepak_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic cinepak`
libcinepak_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic cinepak`

libcinepak_a_SOURCES = $(SOURCES_cinepak)
libcinepak_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin cinepak`
libcinepak_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin cinepak`
libcinepak_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin cinepak`

# The dts plugin

libdts_plugin_a_SOURCES = $(SOURCES_dts)
libdts_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin dts`
libdts_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin dts`
libdts_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin dts`

libdts_pic_a_SOURCES = $(SOURCES_dts)
libdts_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic dts`
libdts_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic dts`
libdts_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic dts`

libdts_a_SOURCES = $(SOURCES_dts)
libdts_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin dts`
libdts_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin dts`
libdts_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin dts`

# The flacdec plugin

libflacdec_plugin_a_SOURCES = $(SOURCES_flacdec)
libflacdec_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin flacdec`
libflacdec_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin flacdec`
libflacdec_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin flacdec`

libflacdec_pic_a_SOURCES = $(SOURCES_flacdec)
libflacdec_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic flacdec`
libflacdec_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic flacdec`
libflacdec_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic flacdec`

libflacdec_a_SOURCES = $(SOURCES_flacdec)
libflacdec_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin flacdec`
libflacdec_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin flacdec`
libflacdec_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin flacdec`

# The lpcm plugin

liblpcm_plugin_a_SOURCES = $(SOURCES_lpcm)
liblpcm_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin lpcm`
liblpcm_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin lpcm`
liblpcm_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin lpcm`

liblpcm_pic_a_SOURCES = $(SOURCES_lpcm)
liblpcm_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic lpcm`
liblpcm_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic lpcm`
liblpcm_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic lpcm`

liblpcm_a_SOURCES = $(SOURCES_lpcm)
liblpcm_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin lpcm`
liblpcm_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin lpcm`
liblpcm_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin lpcm`

# The araw plugin

libaraw_plugin_a_SOURCES = $(SOURCES_araw)
libaraw_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin araw`
libaraw_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin araw`
libaraw_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin araw`

libaraw_pic_a_SOURCES = $(SOURCES_araw)
libaraw_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic araw`
libaraw_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic araw`
libaraw_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic araw`

libaraw_a_SOURCES = $(SOURCES_araw)
libaraw_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin araw`
libaraw_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin araw`
libaraw_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin araw`

# The vorbis plugin

libvorbis_plugin_a_SOURCES = $(SOURCES_vorbis)
libvorbis_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin vorbis`
libvorbis_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin vorbis`
libvorbis_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin vorbis`

libvorbis_pic_a_SOURCES = $(SOURCES_vorbis)
libvorbis_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic vorbis`
libvorbis_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic vorbis`
libvorbis_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic vorbis`

libvorbis_a_SOURCES = $(SOURCES_vorbis)
libvorbis_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin vorbis`
libvorbis_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin vorbis`
libvorbis_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin vorbis`

# The tarkin plugin

libtarkin_plugin_a_SOURCES = $(SOURCES_tarkin)
libtarkin_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin tarkin`
libtarkin_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin tarkin`
libtarkin_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin tarkin`

libtarkin_pic_a_SOURCES = $(SOURCES_tarkin)
libtarkin_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic tarkin`
libtarkin_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic tarkin`
libtarkin_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic tarkin`

libtarkin_a_SOURCES = $(SOURCES_tarkin)
libtarkin_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin tarkin`
libtarkin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin tarkin`
libtarkin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin tarkin`

# The theora plugin

libtheora_plugin_a_SOURCES = $(SOURCES_theora)
libtheora_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin theora`
libtheora_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin theora`
libtheora_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin theora`

libtheora_pic_a_SOURCES = $(SOURCES_theora)
libtheora_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic theora`
libtheora_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic theora`
libtheora_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic theora`

libtheora_a_SOURCES = $(SOURCES_theora)
libtheora_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin theora`
libtheora_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin theora`
libtheora_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin theora`

# The tremor plugin

libtremor_plugin_a_SOURCES = $(SOURCES_tremor)
libtremor_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin tremor`
libtremor_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin tremor`
libtremor_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin tremor`

libtremor_pic_a_SOURCES = $(SOURCES_tremor)
libtremor_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic tremor`
libtremor_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic tremor`
libtremor_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic tremor`

libtremor_a_SOURCES = $(SOURCES_tremor)
libtremor_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin tremor`
libtremor_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin tremor`
libtremor_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin tremor`

# The speex plugin

libspeex_plugin_a_SOURCES = $(SOURCES_speex)
libspeex_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin speex`
libspeex_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin speex`
libspeex_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin speex`

libspeex_pic_a_SOURCES = $(SOURCES_speex)
libspeex_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic speex`
libspeex_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic speex`
libspeex_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic speex`

libspeex_a_SOURCES = $(SOURCES_speex)
libspeex_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin speex`
libspeex_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin speex`
libspeex_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin speex`

# The adpcm plugin

libadpcm_plugin_a_SOURCES = $(SOURCES_adpcm)
libadpcm_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin adpcm`
libadpcm_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin adpcm`
libadpcm_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin adpcm`

libadpcm_pic_a_SOURCES = $(SOURCES_adpcm)
libadpcm_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic adpcm`
libadpcm_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic adpcm`
libadpcm_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic adpcm`

libadpcm_a_SOURCES = $(SOURCES_adpcm)
libadpcm_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin adpcm`
libadpcm_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin adpcm`
libadpcm_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin adpcm`

# The mpeg_audio plugin

libmpeg_audio_plugin_a_SOURCES = $(SOURCES_mpeg_audio)
libmpeg_audio_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin mpeg_audio`
libmpeg_audio_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin mpeg_audio`
libmpeg_audio_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin mpeg_audio`

libmpeg_audio_pic_a_SOURCES = $(SOURCES_mpeg_audio)
libmpeg_audio_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic mpeg_audio`
libmpeg_audio_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic mpeg_audio`
libmpeg_audio_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic mpeg_audio`

libmpeg_audio_a_SOURCES = $(SOURCES_mpeg_audio)
libmpeg_audio_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin mpeg_audio`
libmpeg_audio_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin mpeg_audio`
libmpeg_audio_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin mpeg_audio`

# The libmpeg2 plugin

liblibmpeg2_plugin_a_SOURCES = $(SOURCES_libmpeg2)
liblibmpeg2_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin libmpeg2`
liblibmpeg2_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin libmpeg2`
liblibmpeg2_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin libmpeg2`

liblibmpeg2_pic_a_SOURCES = $(SOURCES_libmpeg2)
liblibmpeg2_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic libmpeg2`
liblibmpeg2_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic libmpeg2`
liblibmpeg2_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic libmpeg2`

liblibmpeg2_a_SOURCES = $(SOURCES_libmpeg2)
liblibmpeg2_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin libmpeg2`
liblibmpeg2_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin libmpeg2`
liblibmpeg2_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin libmpeg2`

# The rawvideo plugin

librawvideo_plugin_a_SOURCES = $(SOURCES_rawvideo)
librawvideo_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin rawvideo`
librawvideo_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin rawvideo`
librawvideo_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin rawvideo`

librawvideo_pic_a_SOURCES = $(SOURCES_rawvideo)
librawvideo_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic rawvideo`
librawvideo_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic rawvideo`
librawvideo_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic rawvideo`

librawvideo_a_SOURCES = $(SOURCES_rawvideo)
librawvideo_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin rawvideo`
librawvideo_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin rawvideo`
librawvideo_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin rawvideo`

# The quicktime plugin

libquicktime_plugin_a_SOURCES = $(SOURCES_quicktime)
libquicktime_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin quicktime`
libquicktime_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin quicktime`
libquicktime_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin quicktime`

libquicktime_pic_a_SOURCES = $(SOURCES_quicktime)
libquicktime_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic quicktime`
libquicktime_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic quicktime`
libquicktime_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic quicktime`

libquicktime_a_SOURCES = $(SOURCES_quicktime)
libquicktime_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin quicktime`
libquicktime_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin quicktime`
libquicktime_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin quicktime`

# The subsdec plugin

libsubsdec_plugin_a_SOURCES = $(SOURCES_subsdec)
libsubsdec_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin subsdec`
libsubsdec_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin subsdec`
libsubsdec_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin subsdec`

libsubsdec_pic_a_SOURCES = $(SOURCES_subsdec)
libsubsdec_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic subsdec`
libsubsdec_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic subsdec`
libsubsdec_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic subsdec`

libsubsdec_a_SOURCES = $(SOURCES_subsdec)
libsubsdec_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin subsdec`
libsubsdec_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin subsdec`
libsubsdec_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin subsdec`

# The faad plugin

libfaad_plugin_a_SOURCES = $(SOURCES_faad)
libfaad_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin faad`
libfaad_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin faad`
libfaad_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin faad`

libfaad_pic_a_SOURCES = $(SOURCES_faad)
libfaad_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic faad`
libfaad_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic faad`
libfaad_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic faad`

libfaad_a_SOURCES = $(SOURCES_faad)
libfaad_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin faad`
libfaad_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin faad`
libfaad_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin faad`

# The dvbsub plugin

libdvbsub_plugin_a_SOURCES = $(SOURCES_dvbsub)
libdvbsub_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin dvbsub`
libdvbsub_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin dvbsub`
libdvbsub_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin dvbsub`

libdvbsub_pic_a_SOURCES = $(SOURCES_dvbsub)
libdvbsub_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic dvbsub`
libdvbsub_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic dvbsub`
libdvbsub_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic dvbsub`

libdvbsub_a_SOURCES = $(SOURCES_dvbsub)
libdvbsub_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin dvbsub`
libdvbsub_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin dvbsub`
libdvbsub_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin dvbsub`

# The mash plugin

libmash_plugin_a_SOURCES = $(SOURCES_mash)
libmash_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin mash`
libmash_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin mash`
libmash_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin mash`

libmash_pic_a_SOURCES = $(SOURCES_mash)
libmash_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic mash`
libmash_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic mash`
libmash_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic mash`

libmash_a_SOURCES = $(SOURCES_mash)
libmash_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin mash`
libmash_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin mash`
libmash_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin mash`

# The x264 plugin

libx264_plugin_a_SOURCES = $(SOURCES_x264)
libx264_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin x264`
libx264_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin x264`
libx264_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin x264`

libx264_pic_a_SOURCES = $(SOURCES_x264)
libx264_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic x264`
libx264_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic x264`
libx264_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic x264`

libx264_a_SOURCES = $(SOURCES_x264)
libx264_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin x264`
libx264_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin x264`
libx264_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin x264`

# The twolame plugin

libtwolame_plugin_a_SOURCES = $(SOURCES_twolame)
libtwolame_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin twolame`
libtwolame_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin twolame`
libtwolame_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin twolame`

libtwolame_pic_a_SOURCES = $(SOURCES_twolame)
libtwolame_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic twolame`
libtwolame_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic twolame`
libtwolame_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic twolame`

libtwolame_a_SOURCES = $(SOURCES_twolame)
libtwolame_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin twolame`
libtwolame_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin twolame`
libtwolame_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin twolame`

# The dirac plugin

libdirac_plugin_a_SOURCES = $(SOURCES_dirac)
libdirac_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin dirac`
libdirac_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin dirac`
libdirac_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin dirac`

libdirac_pic_a_SOURCES = $(SOURCES_dirac)
libdirac_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic dirac`
libdirac_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic dirac`
libdirac_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic dirac`

libdirac_a_SOURCES = $(SOURCES_dirac)
libdirac_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin dirac`
libdirac_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin dirac`
libdirac_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin dirac`

# The png plugin

libpng_plugin_a_SOURCES = $(SOURCES_png)
libpng_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin png`
libpng_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin png`
libpng_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin png`

libpng_pic_a_SOURCES = $(SOURCES_png)
libpng_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic png`
libpng_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic png`
libpng_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic png`

libpng_a_SOURCES = $(SOURCES_png)
libpng_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin png`
libpng_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin png`
libpng_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin png`

# The svcdsub plugin

libsvcdsub_plugin_a_SOURCES = $(SOURCES_svcdsub)
libsvcdsub_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin svcdsub`
libsvcdsub_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin svcdsub`
libsvcdsub_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin svcdsub`

libsvcdsub_pic_a_SOURCES = $(SOURCES_svcdsub)
libsvcdsub_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic svcdsub`
libsvcdsub_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic svcdsub`
libsvcdsub_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic svcdsub`

libsvcdsub_a_SOURCES = $(SOURCES_svcdsub)
libsvcdsub_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin svcdsub`
libsvcdsub_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin svcdsub`
libsvcdsub_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin svcdsub`

# The cvdsub plugin

libcvdsub_plugin_a_SOURCES = $(SOURCES_cvdsub)
libcvdsub_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin cvdsub`
libcvdsub_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin cvdsub`
libcvdsub_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin cvdsub`

libcvdsub_pic_a_SOURCES = $(SOURCES_cvdsub)
libcvdsub_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic cvdsub`
libcvdsub_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic cvdsub`
libcvdsub_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic cvdsub`

libcvdsub_a_SOURCES = $(SOURCES_cvdsub)
libcvdsub_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin cvdsub`
libcvdsub_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin cvdsub`
libcvdsub_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin cvdsub`

# The fake plugin

libfake_plugin_a_SOURCES = $(SOURCES_fake)
libfake_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin fake`
libfake_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin fake`
libfake_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin fake`

libfake_pic_a_SOURCES = $(SOURCES_fake)
libfake_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic fake`
libfake_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic fake`
libfake_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic fake`

libfake_a_SOURCES = $(SOURCES_fake)
libfake_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin fake`
libfake_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin fake`
libfake_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin fake`

# The realaudio plugin

librealaudio_plugin_a_SOURCES = $(SOURCES_realaudio)
librealaudio_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin realaudio`
librealaudio_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin realaudio`
librealaudio_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin realaudio`

librealaudio_pic_a_SOURCES = $(SOURCES_realaudio)
librealaudio_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic realaudio`
librealaudio_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic realaudio`
librealaudio_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic realaudio`

librealaudio_a_SOURCES = $(SOURCES_realaudio)
librealaudio_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin realaudio`
librealaudio_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin realaudio`
librealaudio_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin realaudio`

# The sdl_image plugin

libsdl_image_plugin_a_SOURCES = $(SOURCES_sdl_image)
libsdl_image_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin sdl_image`
libsdl_image_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin sdl_image`
libsdl_image_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin sdl_image`

libsdl_image_pic_a_SOURCES = $(SOURCES_sdl_image)
libsdl_image_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic sdl_image`
libsdl_image_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic sdl_image`
libsdl_image_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic sdl_image`

libsdl_image_a_SOURCES = $(SOURCES_sdl_image)
libsdl_image_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin sdl_image`
libsdl_image_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin sdl_image`
libsdl_image_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin sdl_image`

