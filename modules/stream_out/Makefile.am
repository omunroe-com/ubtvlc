
# modules/stream_out/Makefile.am automatically generated from modules/stream_out/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = stream_out
mods = stream_out_dummy stream_out_description stream_out_standard stream_out_transcode stream_out_duplicate stream_out_es stream_out_display stream_out_gather stream_out_rtp stream_out_switcher stream_out_bridge stream_out_mosaic_bridge

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = transrate
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libstream_out_dummy_plugin.a libstream_out_dummy.a libstream_out_dummy_pic.a libstream_out_description_plugin.a libstream_out_description.a libstream_out_description_pic.a libstream_out_standard_plugin.a libstream_out_standard.a libstream_out_standard_pic.a libstream_out_transcode_plugin.a libstream_out_transcode.a libstream_out_transcode_pic.a libstream_out_duplicate_plugin.a libstream_out_duplicate.a libstream_out_duplicate_pic.a libstream_out_es_plugin.a libstream_out_es.a libstream_out_es_pic.a libstream_out_display_plugin.a libstream_out_display.a libstream_out_display_pic.a libstream_out_gather_plugin.a libstream_out_gather.a libstream_out_gather_pic.a libstream_out_rtp_plugin.a libstream_out_rtp.a libstream_out_rtp_pic.a libstream_out_switcher_plugin.a libstream_out_switcher.a libstream_out_switcher_pic.a libstream_out_bridge_plugin.a libstream_out_bridge.a libstream_out_bridge_pic.a libstream_out_mosaic_bridge_plugin.a libstream_out_mosaic_bridge.a libstream_out_mosaic_bridge_pic.a
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

# The stream_out_dummy plugin

libstream_out_dummy_plugin_a_SOURCES = $(SOURCES_stream_out_dummy)
libstream_out_dummy_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_dummy`
libstream_out_dummy_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_dummy`
libstream_out_dummy_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_dummy`

libstream_out_dummy_pic_a_SOURCES = $(SOURCES_stream_out_dummy)
libstream_out_dummy_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_dummy`
libstream_out_dummy_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_dummy`
libstream_out_dummy_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_dummy`

libstream_out_dummy_a_SOURCES = $(SOURCES_stream_out_dummy)
libstream_out_dummy_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_dummy`
libstream_out_dummy_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_dummy`
libstream_out_dummy_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_dummy`

# The stream_out_description plugin

libstream_out_description_plugin_a_SOURCES = $(SOURCES_stream_out_description)
libstream_out_description_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_description`
libstream_out_description_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_description`
libstream_out_description_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_description`

libstream_out_description_pic_a_SOURCES = $(SOURCES_stream_out_description)
libstream_out_description_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_description`
libstream_out_description_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_description`
libstream_out_description_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_description`

libstream_out_description_a_SOURCES = $(SOURCES_stream_out_description)
libstream_out_description_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_description`
libstream_out_description_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_description`
libstream_out_description_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_description`

# The stream_out_standard plugin

libstream_out_standard_plugin_a_SOURCES = $(SOURCES_stream_out_standard)
libstream_out_standard_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_standard`
libstream_out_standard_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_standard`
libstream_out_standard_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_standard`

libstream_out_standard_pic_a_SOURCES = $(SOURCES_stream_out_standard)
libstream_out_standard_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_standard`
libstream_out_standard_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_standard`
libstream_out_standard_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_standard`

libstream_out_standard_a_SOURCES = $(SOURCES_stream_out_standard)
libstream_out_standard_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_standard`
libstream_out_standard_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_standard`
libstream_out_standard_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_standard`

# The stream_out_transcode plugin

libstream_out_transcode_plugin_a_SOURCES = $(SOURCES_stream_out_transcode)
libstream_out_transcode_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_transcode`
libstream_out_transcode_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_transcode`
libstream_out_transcode_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_transcode`

libstream_out_transcode_pic_a_SOURCES = $(SOURCES_stream_out_transcode)
libstream_out_transcode_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_transcode`
libstream_out_transcode_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_transcode`
libstream_out_transcode_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_transcode`

libstream_out_transcode_a_SOURCES = $(SOURCES_stream_out_transcode)
libstream_out_transcode_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_transcode`
libstream_out_transcode_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_transcode`
libstream_out_transcode_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_transcode`

# The stream_out_duplicate plugin

libstream_out_duplicate_plugin_a_SOURCES = $(SOURCES_stream_out_duplicate)
libstream_out_duplicate_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_duplicate`
libstream_out_duplicate_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_duplicate`
libstream_out_duplicate_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_duplicate`

libstream_out_duplicate_pic_a_SOURCES = $(SOURCES_stream_out_duplicate)
libstream_out_duplicate_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_duplicate`
libstream_out_duplicate_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_duplicate`
libstream_out_duplicate_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_duplicate`

libstream_out_duplicate_a_SOURCES = $(SOURCES_stream_out_duplicate)
libstream_out_duplicate_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_duplicate`
libstream_out_duplicate_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_duplicate`
libstream_out_duplicate_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_duplicate`

# The stream_out_es plugin

libstream_out_es_plugin_a_SOURCES = $(SOURCES_stream_out_es)
libstream_out_es_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_es`
libstream_out_es_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_es`
libstream_out_es_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_es`

libstream_out_es_pic_a_SOURCES = $(SOURCES_stream_out_es)
libstream_out_es_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_es`
libstream_out_es_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_es`
libstream_out_es_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_es`

libstream_out_es_a_SOURCES = $(SOURCES_stream_out_es)
libstream_out_es_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_es`
libstream_out_es_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_es`
libstream_out_es_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_es`

# The stream_out_display plugin

libstream_out_display_plugin_a_SOURCES = $(SOURCES_stream_out_display)
libstream_out_display_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_display`
libstream_out_display_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_display`
libstream_out_display_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_display`

libstream_out_display_pic_a_SOURCES = $(SOURCES_stream_out_display)
libstream_out_display_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_display`
libstream_out_display_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_display`
libstream_out_display_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_display`

libstream_out_display_a_SOURCES = $(SOURCES_stream_out_display)
libstream_out_display_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_display`
libstream_out_display_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_display`
libstream_out_display_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_display`

# The stream_out_gather plugin

libstream_out_gather_plugin_a_SOURCES = $(SOURCES_stream_out_gather)
libstream_out_gather_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_gather`
libstream_out_gather_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_gather`
libstream_out_gather_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_gather`

libstream_out_gather_pic_a_SOURCES = $(SOURCES_stream_out_gather)
libstream_out_gather_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_gather`
libstream_out_gather_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_gather`
libstream_out_gather_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_gather`

libstream_out_gather_a_SOURCES = $(SOURCES_stream_out_gather)
libstream_out_gather_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_gather`
libstream_out_gather_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_gather`
libstream_out_gather_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_gather`

# The stream_out_rtp plugin

libstream_out_rtp_plugin_a_SOURCES = $(SOURCES_stream_out_rtp)
libstream_out_rtp_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_rtp`
libstream_out_rtp_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_rtp`
libstream_out_rtp_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_rtp`

libstream_out_rtp_pic_a_SOURCES = $(SOURCES_stream_out_rtp)
libstream_out_rtp_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_rtp`
libstream_out_rtp_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_rtp`
libstream_out_rtp_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_rtp`

libstream_out_rtp_a_SOURCES = $(SOURCES_stream_out_rtp)
libstream_out_rtp_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_rtp`
libstream_out_rtp_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_rtp`
libstream_out_rtp_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_rtp`

# The stream_out_switcher plugin

libstream_out_switcher_plugin_a_SOURCES = $(SOURCES_stream_out_switcher)
libstream_out_switcher_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_switcher`
libstream_out_switcher_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_switcher`
libstream_out_switcher_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_switcher`

libstream_out_switcher_pic_a_SOURCES = $(SOURCES_stream_out_switcher)
libstream_out_switcher_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_switcher`
libstream_out_switcher_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_switcher`
libstream_out_switcher_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_switcher`

libstream_out_switcher_a_SOURCES = $(SOURCES_stream_out_switcher)
libstream_out_switcher_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_switcher`
libstream_out_switcher_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_switcher`
libstream_out_switcher_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_switcher`

# The stream_out_bridge plugin

libstream_out_bridge_plugin_a_SOURCES = $(SOURCES_stream_out_bridge)
libstream_out_bridge_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_bridge`
libstream_out_bridge_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_bridge`
libstream_out_bridge_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_bridge`

libstream_out_bridge_pic_a_SOURCES = $(SOURCES_stream_out_bridge)
libstream_out_bridge_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_bridge`
libstream_out_bridge_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_bridge`
libstream_out_bridge_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_bridge`

libstream_out_bridge_a_SOURCES = $(SOURCES_stream_out_bridge)
libstream_out_bridge_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_bridge`
libstream_out_bridge_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_bridge`
libstream_out_bridge_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_bridge`

# The stream_out_mosaic_bridge plugin

libstream_out_mosaic_bridge_plugin_a_SOURCES = $(SOURCES_stream_out_mosaic_bridge)
libstream_out_mosaic_bridge_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin stream_out_mosaic_bridge`
libstream_out_mosaic_bridge_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin stream_out_mosaic_bridge`
libstream_out_mosaic_bridge_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin stream_out_mosaic_bridge`

libstream_out_mosaic_bridge_pic_a_SOURCES = $(SOURCES_stream_out_mosaic_bridge)
libstream_out_mosaic_bridge_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic stream_out_mosaic_bridge`
libstream_out_mosaic_bridge_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic stream_out_mosaic_bridge`
libstream_out_mosaic_bridge_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic stream_out_mosaic_bridge`

libstream_out_mosaic_bridge_a_SOURCES = $(SOURCES_stream_out_mosaic_bridge)
libstream_out_mosaic_bridge_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin stream_out_mosaic_bridge`
libstream_out_mosaic_bridge_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin stream_out_mosaic_bridge`
libstream_out_mosaic_bridge_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin stream_out_mosaic_bridge`

