
# modules/video_filter/Makefile.am automatically generated from modules/video_filter/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = video_filter
mods = mosaic transform invert adjust distort wall clone crop motionblur logo deinterlace blend scale time marq rss motiondetect rv32 osdmenu magnify

NULL =
libvlc_LTLIBRARIES =
noinst_LTLIBRARIES =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
clean_modules =
SUBDIRS = swscale
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libmosaic_plugin.a libmosaic.a libmosaic_pic.a libtransform_plugin.a libtransform.a libtransform_pic.a libinvert_plugin.a libinvert.a libinvert_pic.a libadjust_plugin.a libadjust.a libadjust_pic.a libdistort_plugin.a libdistort.a libdistort_pic.a libwall_plugin.a libwall.a libwall_pic.a libclone_plugin.a libclone.a libclone_pic.a libcrop_plugin.a libcrop.a libcrop_pic.a libmotionblur_plugin.a libmotionblur.a libmotionblur_pic.a liblogo_plugin.a liblogo.a liblogo_pic.a libdeinterlace_plugin.a libdeinterlace.a libdeinterlace_pic.a libblend_plugin.a libblend.a libblend_pic.a libscale_plugin.a libscale.a libscale_pic.a libtime_plugin.a libtime.a libtime_pic.a libmarq_plugin.a libmarq.a libmarq_pic.a librss_plugin.a librss.a librss_pic.a libmotiondetect_plugin.a libmotiondetect.a libmotiondetect_pic.a librv32_plugin.a librv32.a librv32_pic.a libosdmenu_plugin.a libosdmenu.a libosdmenu_pic.a libmagnify_plugin.a libmagnify.a libmagnify_pic.a
EXTRA_LTLIBRARIES =  libmosaic_plugin.la libmosaic_builtin.la libtransform_plugin.la libtransform_builtin.la libinvert_plugin.la libinvert_builtin.la libadjust_plugin.la libadjust_builtin.la libdistort_plugin.la libdistort_builtin.la libwall_plugin.la libwall_builtin.la libclone_plugin.la libclone_builtin.la libcrop_plugin.la libcrop_builtin.la libmotionblur_plugin.la libmotionblur_builtin.la liblogo_plugin.la liblogo_builtin.la libdeinterlace_plugin.la libdeinterlace_builtin.la libblend_plugin.la libblend_builtin.la libscale_plugin.la libscale_builtin.la libtime_plugin.la libtime_builtin.la libmarq_plugin.la libmarq_builtin.la librss_plugin.la librss_builtin.la libmotiondetect_plugin.la libmotiondetect_builtin.la librv32_plugin.la librv32_builtin.la libosdmenu_plugin.la libosdmenu_builtin.la libmagnify_plugin.la libmagnify_builtin.la

include Modules.am

LTLIBVLC = $(top_builddir)/src/libvlc.la

if BUILD_SHARED
LIBVLC = $(top_builddir)/src/libvlc$(LIBEXT)
LIBADD = $(LIBVLC)
endif

all: all-modules

# Find out which modules were enabled and tell make to build them
all-modules:
if USE_LIBTOOL
	@set fnord $$MAKEFLAGS; amf=$$2; targets=`\
	if test "$(plugin)" != "no"; then z=$$($(VLC_CONFIG) --list plugin); for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo lib$${mod}_plugin.la;; esac; done; fi; \
	if test "$(builtin)" != "no"; then z=$$($(VLC_CONFIG) --list builtin); for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo lib$${mod}_builtin.la;; esac; done; fi; \
	`; case "$$targets" in *lib*) $(MAKE) $(AM_MAKEFLAGS) $$targets || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; ;; esac; \
	test -z "$$fail"
else
	@set fnord $$MAKEFLAGS; amf=$$2; targets=`\
	if test "$(plugin)" != "no"; then z=$$($(VLC_CONFIG) --list plugin); for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo lib$${mod}_plugin$(LIBEXT);; esac; done; fi; \
	if test "$(builtin)" != "no"; then z=$$($(VLC_CONFIG) --list builtin); for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo lib$${mod}.a;; esac; done; fi; \
	if test "$(pic)" = "pic"; then z=$$($(VLC_CONFIG) --list builtin); for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo lib$${mod}_pic.a;; esac; done; fi; \
	`; case "$$targets" in *lib*) $(MAKE) $(AM_MAKEFLAGS) $$targets || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; ;; esac; \
	test -z "$$fail"
endif

# Build a plugin with the adequate linker and linker's flags
_plugin.a_plugin$(LIBEXT):
	@mod="$*" ; mod=$${mod#lib} ; 	ldfl="`$(VLC_CONFIG) --libs plugin $$mod` $(LIBADD) -u $(SYMPREF)$(VLC_ENTRY)" ; 	case `$(VLC_CONFIG) --linkage $$mod` in \
	  c++)  ld="$(CXXLINK)" ;; 	  objc) ld="$(OBJCLINK)" ;; 	  c|*)  ld="$(LINK)" ;; 	esac ; 	echo $$ld $< $$ldfl ; 	$$ld $< $$ldfl
#ifneq (,$(findstring cygwin,$(host)))
#	mv -f "$@.exe" "$@"
#endif
	@if test -f "$@.exe"; then mv -f "$@.exe" "$@"; fi

if MAINTAINER_MODE
$(srcdir)/Makefile.am: $(srcdir)/Modules.am $(top_srcdir)/bootstrap
	cd $(top_srcdir) && $(SHELL) ./bootstrap
endif

mostlyclean-local:
if USE_LIBTOOL
	-rm -f *.la
else
	-rm -f *.a *$(LIBEXT)
endif

clean-local: $(clean_modules)

### automake creates libvlcdir after running install-*-local
### so we have to create it ourselves first
install-exec-local: all-modules
if USE_LIBTOOL
	@if test -z "$(libvlc_LTLIBRARIES)"; then 	  z=$$($(VLC_CONFIG) --list plugin); 	  m=`for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo -n " lib$${mod}_plugin.la" ;; esac; done` ; 	  test -z "$$m" ||   	    $(MAKE) $(AM_MAKEFLAGS) libvlc_LTLIBRARIES="$$m" install-libvlcLTLIBRARIES || exit $$? ; 	fi
else
	mkdir -p -- "$(DESTDIR)$(libvlcdir)"
	@z=$$($(VLC_CONFIG) --list plugin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo $(INSTALL_PROGRAM) "lib$${mod}_plugin$(LIBEXT)" "$(DESTDIR)$(libvlcdir)/" ; 	      $(INSTALL_PROGRAM) "lib$${mod}_plugin$(LIBEXT)" "$(DESTDIR)$(libvlcdir)/" || exit $$?; 	      ;; 	  esac; 	done
	@z=$$($(VLC_CONFIG) --list builtin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo $(INSTALL_DATA) "lib$${mod}.a" "$(DESTDIR)$(libdir)/vlc/" ; 	      $(INSTALL_DATA) "lib$${mod}.a" "$(DESTDIR)$(libdir)/vlc/" || exit $$?; 	      ;; 	  esac; 	done
if BUILD_MOZILLA
	@z=$$($(VLC_CONFIG) --list builtin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo $(INSTALL_DATA) "lib$${mod}_pic.a" "$(DESTDIR)$(libdir)/vlc/" ; 	      $(INSTALL_DATA) "lib$${mod}_pic.a" "$(DESTDIR)$(libdir)/vlc/" || exit $$?; 	      ;; 	  esac; 	done
endif
endif

uninstall-local:
if USE_LIBTOOL
	@if test -z "$(libvlc_LTLIBRARIES)"; then 	  z=$$($(VLC_CONFIG) --list plugin); 	  m=`for mod in $(mods); do case "$$z " in *\ $${mod}\ *) echo -n " lib$${mod}_plugin.la" ;; esac; done` ; 	  test -z "$$m" || 	    $(MAKE) $(AM_MAKEFLAGS) libvlc_LTLIBRARIES="$$m" uninstall-libvlcLTLIBRARIES || exit $$?; 	fi
else
	@z=$$($(VLC_CONFIG) --list plugin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo rm -f "$(DESTDIR)$(libvlcdir)/lib$${mod}_plugin$(LIBEXT)" ; 	      rm -f "$(DESTDIR)$(libvlcdir)/lib$${mod}_plugin$(LIBEXT)" || true; 	      ;; 	  esac; 	done
	@z=$$($(VLC_CONFIG) --list builtin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo rm -f "$(DESTDIR)$(libdir)/vlc/lib$${mod}.a" ; 	      rm -f "$(DESTDIR)$(libdir)/vlc/lib$${mod}.a" || true; 	      ;; 	  esac; 	done
if BUILD_MOZILLA
	@z=$$($(VLC_CONFIG) --list builtin); 	for mod in $(mods); do 	  case "$$z " 	    in *\ $${mod}\ *) 	      echo rm -f "$(DESTDIR)$(libdir)/vlc/lib$${mod}_pic.a" ; 	      rm -f "$(DESTDIR)$(libdir)/vlc/lib$${mod}_pic.a" || true; 	      ;; 	  esac; 	done
endif
endif

# The mosaic plugin

libmosaic_plugin_a_SOURCES = $(SOURCES_mosaic)
libmosaic_plugin_la_SOURCES = $(SOURCES_mosaic)
libmosaic_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin mosaic`
libmosaic_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin mosaic`
libmosaic_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin mosaic`
libmosaic_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin mosaic`
libmosaic_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin mosaic`
libmosaic_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin mosaic`
libmosaic_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin mosaic` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libmosaic_plugin_la_LIBADD = $(LTLIBVLC)

libmosaic_pic_a_SOURCES = $(SOURCES_mosaic)
libmosaic_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic mosaic`
libmosaic_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic mosaic`
libmosaic_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic mosaic`

libmosaic_a_SOURCES = $(SOURCES_mosaic)
libmosaic_builtin_la_SOURCES = $(SOURCES_mosaic)
libmosaic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin mosaic`
libmosaic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin mosaic`
libmosaic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin mosaic`
libmosaic_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin mosaic`
libmosaic_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin mosaic`
libmosaic_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin mosaic`

# The transform plugin

libtransform_plugin_a_SOURCES = $(SOURCES_transform)
libtransform_plugin_la_SOURCES = $(SOURCES_transform)
libtransform_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin transform`
libtransform_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin transform`
libtransform_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin transform`
libtransform_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin transform`
libtransform_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin transform`
libtransform_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin transform`
libtransform_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin transform` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libtransform_plugin_la_LIBADD = $(LTLIBVLC)

libtransform_pic_a_SOURCES = $(SOURCES_transform)
libtransform_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic transform`
libtransform_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic transform`
libtransform_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic transform`

libtransform_a_SOURCES = $(SOURCES_transform)
libtransform_builtin_la_SOURCES = $(SOURCES_transform)
libtransform_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin transform`
libtransform_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin transform`
libtransform_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin transform`
libtransform_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin transform`
libtransform_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin transform`
libtransform_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin transform`

# The invert plugin

libinvert_plugin_a_SOURCES = $(SOURCES_invert)
libinvert_plugin_la_SOURCES = $(SOURCES_invert)
libinvert_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin invert`
libinvert_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin invert`
libinvert_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin invert`
libinvert_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin invert`
libinvert_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin invert`
libinvert_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin invert`
libinvert_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin invert` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libinvert_plugin_la_LIBADD = $(LTLIBVLC)

libinvert_pic_a_SOURCES = $(SOURCES_invert)
libinvert_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic invert`
libinvert_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic invert`
libinvert_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic invert`

libinvert_a_SOURCES = $(SOURCES_invert)
libinvert_builtin_la_SOURCES = $(SOURCES_invert)
libinvert_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin invert`
libinvert_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin invert`
libinvert_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin invert`
libinvert_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin invert`
libinvert_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin invert`
libinvert_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin invert`

# The adjust plugin

libadjust_plugin_a_SOURCES = $(SOURCES_adjust)
libadjust_plugin_la_SOURCES = $(SOURCES_adjust)
libadjust_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin adjust`
libadjust_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin adjust`
libadjust_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin adjust`
libadjust_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin adjust`
libadjust_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin adjust`
libadjust_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin adjust`
libadjust_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin adjust` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libadjust_plugin_la_LIBADD = $(LTLIBVLC)

libadjust_pic_a_SOURCES = $(SOURCES_adjust)
libadjust_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic adjust`
libadjust_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic adjust`
libadjust_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic adjust`

libadjust_a_SOURCES = $(SOURCES_adjust)
libadjust_builtin_la_SOURCES = $(SOURCES_adjust)
libadjust_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin adjust`
libadjust_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin adjust`
libadjust_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin adjust`
libadjust_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin adjust`
libadjust_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin adjust`
libadjust_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin adjust`

# The distort plugin

libdistort_plugin_a_SOURCES = $(SOURCES_distort)
libdistort_plugin_la_SOURCES = $(SOURCES_distort)
libdistort_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin distort`
libdistort_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin distort`
libdistort_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin distort`
libdistort_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin distort`
libdistort_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin distort`
libdistort_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin distort`
libdistort_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin distort` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libdistort_plugin_la_LIBADD = $(LTLIBVLC)

libdistort_pic_a_SOURCES = $(SOURCES_distort)
libdistort_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic distort`
libdistort_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic distort`
libdistort_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic distort`

libdistort_a_SOURCES = $(SOURCES_distort)
libdistort_builtin_la_SOURCES = $(SOURCES_distort)
libdistort_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin distort`
libdistort_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin distort`
libdistort_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin distort`
libdistort_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin distort`
libdistort_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin distort`
libdistort_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin distort`

# The wall plugin

libwall_plugin_a_SOURCES = $(SOURCES_wall)
libwall_plugin_la_SOURCES = $(SOURCES_wall)
libwall_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin wall`
libwall_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin wall`
libwall_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin wall`
libwall_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin wall`
libwall_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin wall`
libwall_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin wall`
libwall_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin wall` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libwall_plugin_la_LIBADD = $(LTLIBVLC)

libwall_pic_a_SOURCES = $(SOURCES_wall)
libwall_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic wall`
libwall_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic wall`
libwall_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic wall`

libwall_a_SOURCES = $(SOURCES_wall)
libwall_builtin_la_SOURCES = $(SOURCES_wall)
libwall_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin wall`
libwall_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin wall`
libwall_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin wall`
libwall_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin wall`
libwall_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin wall`
libwall_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin wall`

# The clone plugin

libclone_plugin_a_SOURCES = $(SOURCES_clone)
libclone_plugin_la_SOURCES = $(SOURCES_clone)
libclone_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin clone`
libclone_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin clone`
libclone_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin clone`
libclone_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin clone`
libclone_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin clone`
libclone_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin clone`
libclone_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin clone` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libclone_plugin_la_LIBADD = $(LTLIBVLC)

libclone_pic_a_SOURCES = $(SOURCES_clone)
libclone_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic clone`
libclone_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic clone`
libclone_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic clone`

libclone_a_SOURCES = $(SOURCES_clone)
libclone_builtin_la_SOURCES = $(SOURCES_clone)
libclone_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin clone`
libclone_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin clone`
libclone_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin clone`
libclone_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin clone`
libclone_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin clone`
libclone_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin clone`

# The crop plugin

libcrop_plugin_a_SOURCES = $(SOURCES_crop)
libcrop_plugin_la_SOURCES = $(SOURCES_crop)
libcrop_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin crop`
libcrop_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin crop`
libcrop_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin crop`
libcrop_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin crop`
libcrop_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin crop`
libcrop_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin crop`
libcrop_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin crop` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libcrop_plugin_la_LIBADD = $(LTLIBVLC)

libcrop_pic_a_SOURCES = $(SOURCES_crop)
libcrop_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic crop`
libcrop_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic crop`
libcrop_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic crop`

libcrop_a_SOURCES = $(SOURCES_crop)
libcrop_builtin_la_SOURCES = $(SOURCES_crop)
libcrop_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin crop`
libcrop_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin crop`
libcrop_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin crop`
libcrop_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin crop`
libcrop_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin crop`
libcrop_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin crop`

# The motionblur plugin

libmotionblur_plugin_a_SOURCES = $(SOURCES_motionblur)
libmotionblur_plugin_la_SOURCES = $(SOURCES_motionblur)
libmotionblur_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin motionblur`
libmotionblur_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin motionblur`
libmotionblur_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin motionblur`
libmotionblur_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin motionblur`
libmotionblur_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin motionblur`
libmotionblur_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin motionblur`
libmotionblur_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin motionblur` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libmotionblur_plugin_la_LIBADD = $(LTLIBVLC)

libmotionblur_pic_a_SOURCES = $(SOURCES_motionblur)
libmotionblur_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic motionblur`
libmotionblur_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic motionblur`
libmotionblur_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic motionblur`

libmotionblur_a_SOURCES = $(SOURCES_motionblur)
libmotionblur_builtin_la_SOURCES = $(SOURCES_motionblur)
libmotionblur_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin motionblur`
libmotionblur_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin motionblur`
libmotionblur_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin motionblur`
libmotionblur_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin motionblur`
libmotionblur_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin motionblur`
libmotionblur_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin motionblur`

# The logo plugin

liblogo_plugin_a_SOURCES = $(SOURCES_logo)
liblogo_plugin_la_SOURCES = $(SOURCES_logo)
liblogo_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin logo`
liblogo_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin logo`
liblogo_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin logo`
liblogo_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin logo`
liblogo_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin logo`
liblogo_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin logo`
liblogo_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin logo` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
liblogo_plugin_la_LIBADD = $(LTLIBVLC)

liblogo_pic_a_SOURCES = $(SOURCES_logo)
liblogo_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic logo`
liblogo_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic logo`
liblogo_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic logo`

liblogo_a_SOURCES = $(SOURCES_logo)
liblogo_builtin_la_SOURCES = $(SOURCES_logo)
liblogo_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin logo`
liblogo_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin logo`
liblogo_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin logo`
liblogo_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin logo`
liblogo_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin logo`
liblogo_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin logo`

# The deinterlace plugin

libdeinterlace_plugin_a_SOURCES = $(SOURCES_deinterlace)
libdeinterlace_plugin_la_SOURCES = $(SOURCES_deinterlace)
libdeinterlace_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin deinterlace`
libdeinterlace_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin deinterlace`
libdeinterlace_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin deinterlace`
libdeinterlace_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin deinterlace`
libdeinterlace_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin deinterlace`
libdeinterlace_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin deinterlace`
libdeinterlace_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin deinterlace` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libdeinterlace_plugin_la_LIBADD = $(LTLIBVLC)

libdeinterlace_pic_a_SOURCES = $(SOURCES_deinterlace)
libdeinterlace_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic deinterlace`
libdeinterlace_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic deinterlace`
libdeinterlace_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic deinterlace`

libdeinterlace_a_SOURCES = $(SOURCES_deinterlace)
libdeinterlace_builtin_la_SOURCES = $(SOURCES_deinterlace)
libdeinterlace_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin deinterlace`
libdeinterlace_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin deinterlace`
libdeinterlace_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin deinterlace`
libdeinterlace_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin deinterlace`
libdeinterlace_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin deinterlace`
libdeinterlace_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin deinterlace`

# The blend plugin

libblend_plugin_a_SOURCES = $(SOURCES_blend)
libblend_plugin_la_SOURCES = $(SOURCES_blend)
libblend_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin blend`
libblend_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin blend`
libblend_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin blend`
libblend_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin blend`
libblend_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin blend`
libblend_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin blend`
libblend_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin blend` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libblend_plugin_la_LIBADD = $(LTLIBVLC)

libblend_pic_a_SOURCES = $(SOURCES_blend)
libblend_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic blend`
libblend_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic blend`
libblend_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic blend`

libblend_a_SOURCES = $(SOURCES_blend)
libblend_builtin_la_SOURCES = $(SOURCES_blend)
libblend_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin blend`
libblend_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin blend`
libblend_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin blend`
libblend_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin blend`
libblend_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin blend`
libblend_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin blend`

# The scale plugin

libscale_plugin_a_SOURCES = $(SOURCES_scale)
libscale_plugin_la_SOURCES = $(SOURCES_scale)
libscale_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin scale`
libscale_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin scale`
libscale_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin scale`
libscale_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin scale`
libscale_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin scale`
libscale_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin scale`
libscale_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin scale` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libscale_plugin_la_LIBADD = $(LTLIBVLC)

libscale_pic_a_SOURCES = $(SOURCES_scale)
libscale_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic scale`
libscale_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic scale`
libscale_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic scale`

libscale_a_SOURCES = $(SOURCES_scale)
libscale_builtin_la_SOURCES = $(SOURCES_scale)
libscale_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin scale`
libscale_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin scale`
libscale_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin scale`
libscale_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin scale`
libscale_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin scale`
libscale_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin scale`

# The time plugin

libtime_plugin_a_SOURCES = $(SOURCES_time)
libtime_plugin_la_SOURCES = $(SOURCES_time)
libtime_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin time`
libtime_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin time`
libtime_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin time`
libtime_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin time`
libtime_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin time`
libtime_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin time`
libtime_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin time` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libtime_plugin_la_LIBADD = $(LTLIBVLC)

libtime_pic_a_SOURCES = $(SOURCES_time)
libtime_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic time`
libtime_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic time`
libtime_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic time`

libtime_a_SOURCES = $(SOURCES_time)
libtime_builtin_la_SOURCES = $(SOURCES_time)
libtime_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin time`
libtime_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin time`
libtime_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin time`
libtime_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin time`
libtime_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin time`
libtime_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin time`

# The marq plugin

libmarq_plugin_a_SOURCES = $(SOURCES_marq)
libmarq_plugin_la_SOURCES = $(SOURCES_marq)
libmarq_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin marq`
libmarq_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin marq`
libmarq_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin marq`
libmarq_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin marq`
libmarq_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin marq`
libmarq_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin marq`
libmarq_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin marq` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libmarq_plugin_la_LIBADD = $(LTLIBVLC)

libmarq_pic_a_SOURCES = $(SOURCES_marq)
libmarq_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic marq`
libmarq_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic marq`
libmarq_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic marq`

libmarq_a_SOURCES = $(SOURCES_marq)
libmarq_builtin_la_SOURCES = $(SOURCES_marq)
libmarq_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin marq`
libmarq_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin marq`
libmarq_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin marq`
libmarq_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin marq`
libmarq_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin marq`
libmarq_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin marq`

# The rss plugin

librss_plugin_a_SOURCES = $(SOURCES_rss)
librss_plugin_la_SOURCES = $(SOURCES_rss)
librss_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin rss`
librss_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin rss`
librss_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin rss`
librss_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin rss`
librss_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin rss`
librss_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin rss`
librss_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin rss` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
librss_plugin_la_LIBADD = $(LTLIBVLC)

librss_pic_a_SOURCES = $(SOURCES_rss)
librss_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic rss`
librss_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic rss`
librss_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic rss`

librss_a_SOURCES = $(SOURCES_rss)
librss_builtin_la_SOURCES = $(SOURCES_rss)
librss_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin rss`
librss_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin rss`
librss_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin rss`
librss_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin rss`
librss_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin rss`
librss_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin rss`

# The motiondetect plugin

libmotiondetect_plugin_a_SOURCES = $(SOURCES_motiondetect)
libmotiondetect_plugin_la_SOURCES = $(SOURCES_motiondetect)
libmotiondetect_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin motiondetect`
libmotiondetect_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin motiondetect`
libmotiondetect_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin motiondetect`
libmotiondetect_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin motiondetect`
libmotiondetect_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin motiondetect`
libmotiondetect_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin motiondetect`
libmotiondetect_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin motiondetect` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libmotiondetect_plugin_la_LIBADD = $(LTLIBVLC)

libmotiondetect_pic_a_SOURCES = $(SOURCES_motiondetect)
libmotiondetect_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic motiondetect`
libmotiondetect_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic motiondetect`
libmotiondetect_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic motiondetect`

libmotiondetect_a_SOURCES = $(SOURCES_motiondetect)
libmotiondetect_builtin_la_SOURCES = $(SOURCES_motiondetect)
libmotiondetect_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin motiondetect`
libmotiondetect_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin motiondetect`
libmotiondetect_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin motiondetect`
libmotiondetect_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin motiondetect`
libmotiondetect_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin motiondetect`
libmotiondetect_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin motiondetect`

# The rv32 plugin

librv32_plugin_a_SOURCES = $(SOURCES_rv32)
librv32_plugin_la_SOURCES = $(SOURCES_rv32)
librv32_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin rv32`
librv32_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin rv32`
librv32_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin rv32`
librv32_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin rv32`
librv32_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin rv32`
librv32_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin rv32`
librv32_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin rv32` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
librv32_plugin_la_LIBADD = $(LTLIBVLC)

librv32_pic_a_SOURCES = $(SOURCES_rv32)
librv32_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic rv32`
librv32_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic rv32`
librv32_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic rv32`

librv32_a_SOURCES = $(SOURCES_rv32)
librv32_builtin_la_SOURCES = $(SOURCES_rv32)
librv32_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin rv32`
librv32_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin rv32`
librv32_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin rv32`
librv32_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin rv32`
librv32_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin rv32`
librv32_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin rv32`

# The osdmenu plugin

libosdmenu_plugin_a_SOURCES = $(SOURCES_osdmenu)
libosdmenu_plugin_la_SOURCES = $(SOURCES_osdmenu)
libosdmenu_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin osdmenu`
libosdmenu_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin osdmenu`
libosdmenu_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin osdmenu`
libosdmenu_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin osdmenu`
libosdmenu_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin osdmenu`
libosdmenu_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin osdmenu`
libosdmenu_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin osdmenu` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libosdmenu_plugin_la_LIBADD = $(LTLIBVLC)

libosdmenu_pic_a_SOURCES = $(SOURCES_osdmenu)
libosdmenu_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic osdmenu`
libosdmenu_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic osdmenu`
libosdmenu_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic osdmenu`

libosdmenu_a_SOURCES = $(SOURCES_osdmenu)
libosdmenu_builtin_la_SOURCES = $(SOURCES_osdmenu)
libosdmenu_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin osdmenu`
libosdmenu_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin osdmenu`
libosdmenu_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin osdmenu`
libosdmenu_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin osdmenu`
libosdmenu_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin osdmenu`
libosdmenu_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin osdmenu`

# The magnify plugin

libmagnify_plugin_a_SOURCES = $(SOURCES_magnify)
libmagnify_plugin_la_SOURCES = $(SOURCES_magnify)
libmagnify_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin magnify`
libmagnify_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin magnify`
libmagnify_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin magnify`
libmagnify_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin magnify`
libmagnify_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin magnify`
libmagnify_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin magnify`
libmagnify_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin magnify` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libmagnify_plugin_la_LIBADD = $(LTLIBVLC)

libmagnify_pic_a_SOURCES = $(SOURCES_magnify)
libmagnify_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic magnify`
libmagnify_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic magnify`
libmagnify_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic magnify`

libmagnify_a_SOURCES = $(SOURCES_magnify)
libmagnify_builtin_la_SOURCES = $(SOURCES_magnify)
libmagnify_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin magnify`
libmagnify_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin magnify`
libmagnify_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin magnify`
libmagnify_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin magnify`
libmagnify_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin magnify`
libmagnify_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin magnify`

