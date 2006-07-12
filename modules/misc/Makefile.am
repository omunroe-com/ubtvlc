
# modules/misc/Makefile.am automatically generated from modules/misc/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = misc
mods = gtk_main gnome_main gtk2_main gnome2_main screensaver qte_main freetype win32text logger vod_rtsp gnutls svg msn growl

NULL =
libvlc_LTLIBRARIES =
noinst_LTLIBRARIES =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
clean_modules =
SUBDIRS = dummy memcpy network testsuite playlist xml
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libgtk_main_plugin.a libgtk_main.a libgtk_main_pic.a libgnome_main_plugin.a libgnome_main.a libgnome_main_pic.a libgtk2_main_plugin.a libgtk2_main.a libgtk2_main_pic.a libgnome2_main_plugin.a libgnome2_main.a libgnome2_main_pic.a libscreensaver_plugin.a libscreensaver.a libscreensaver_pic.a libqte_main_plugin.a libqte_main.a libqte_main_pic.a libfreetype_plugin.a libfreetype.a libfreetype_pic.a libwin32text_plugin.a libwin32text.a libwin32text_pic.a liblogger_plugin.a liblogger.a liblogger_pic.a libvod_rtsp_plugin.a libvod_rtsp.a libvod_rtsp_pic.a libgnutls_plugin.a libgnutls.a libgnutls_pic.a libsvg_plugin.a libsvg.a libsvg_pic.a libmsn_plugin.a libmsn.a libmsn_pic.a libgrowl_plugin.a libgrowl.a libgrowl_pic.a
EXTRA_LTLIBRARIES =  libgtk_main_plugin.la libgtk_main_builtin.la libgnome_main_plugin.la libgnome_main_builtin.la libgtk2_main_plugin.la libgtk2_main_builtin.la libgnome2_main_plugin.la libgnome2_main_builtin.la libscreensaver_plugin.la libscreensaver_builtin.la libqte_main_plugin.la libqte_main_builtin.la libfreetype_plugin.la libfreetype_builtin.la libwin32text_plugin.la libwin32text_builtin.la liblogger_plugin.la liblogger_builtin.la libvod_rtsp_plugin.la libvod_rtsp_builtin.la libgnutls_plugin.la libgnutls_builtin.la libsvg_plugin.la libsvg_builtin.la libmsn_plugin.la libmsn_builtin.la libgrowl_plugin.la libgrowl_builtin.la

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

# The gtk_main plugin

libgtk_main_plugin_a_SOURCES = $(SOURCES_gtk_main)
libgtk_main_plugin_la_SOURCES = $(SOURCES_gtk_main)
libgtk_main_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gtk_main`
libgtk_main_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gtk_main`
libgtk_main_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gtk_main`
libgtk_main_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin gtk_main`
libgtk_main_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gtk_main`
libgtk_main_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gtk_main`
libgtk_main_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin gtk_main` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libgtk_main_plugin_la_LIBADD = $(LTLIBVLC)

libgtk_main_pic_a_SOURCES = $(SOURCES_gtk_main)
libgtk_main_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gtk_main`
libgtk_main_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gtk_main`
libgtk_main_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gtk_main`

libgtk_main_a_SOURCES = $(SOURCES_gtk_main)
libgtk_main_builtin_la_SOURCES = $(SOURCES_gtk_main)
libgtk_main_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gtk_main`
libgtk_main_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gtk_main`
libgtk_main_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gtk_main`
libgtk_main_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin gtk_main`
libgtk_main_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gtk_main`
libgtk_main_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gtk_main`

# The gnome_main plugin

libgnome_main_plugin_a_SOURCES = $(SOURCES_gnome_main)
libgnome_main_plugin_la_SOURCES = $(SOURCES_gnome_main)
libgnome_main_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gnome_main`
libgnome_main_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gnome_main`
libgnome_main_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gnome_main`
libgnome_main_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin gnome_main`
libgnome_main_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gnome_main`
libgnome_main_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gnome_main`
libgnome_main_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin gnome_main` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libgnome_main_plugin_la_LIBADD = $(LTLIBVLC)

libgnome_main_pic_a_SOURCES = $(SOURCES_gnome_main)
libgnome_main_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gnome_main`
libgnome_main_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gnome_main`
libgnome_main_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gnome_main`

libgnome_main_a_SOURCES = $(SOURCES_gnome_main)
libgnome_main_builtin_la_SOURCES = $(SOURCES_gnome_main)
libgnome_main_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gnome_main`
libgnome_main_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gnome_main`
libgnome_main_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gnome_main`
libgnome_main_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin gnome_main`
libgnome_main_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gnome_main`
libgnome_main_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gnome_main`

# The gtk2_main plugin

libgtk2_main_plugin_a_SOURCES = $(SOURCES_gtk2_main)
libgtk2_main_plugin_la_SOURCES = $(SOURCES_gtk2_main)
libgtk2_main_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gtk2_main`
libgtk2_main_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gtk2_main`
libgtk2_main_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gtk2_main`
libgtk2_main_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin gtk2_main`
libgtk2_main_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gtk2_main`
libgtk2_main_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gtk2_main`
libgtk2_main_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin gtk2_main` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libgtk2_main_plugin_la_LIBADD = $(LTLIBVLC)

libgtk2_main_pic_a_SOURCES = $(SOURCES_gtk2_main)
libgtk2_main_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gtk2_main`
libgtk2_main_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gtk2_main`
libgtk2_main_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gtk2_main`

libgtk2_main_a_SOURCES = $(SOURCES_gtk2_main)
libgtk2_main_builtin_la_SOURCES = $(SOURCES_gtk2_main)
libgtk2_main_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gtk2_main`
libgtk2_main_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gtk2_main`
libgtk2_main_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gtk2_main`
libgtk2_main_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin gtk2_main`
libgtk2_main_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gtk2_main`
libgtk2_main_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gtk2_main`

# The gnome2_main plugin

libgnome2_main_plugin_a_SOURCES = $(SOURCES_gnome2_main)
libgnome2_main_plugin_la_SOURCES = $(SOURCES_gnome2_main)
libgnome2_main_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gnome2_main`
libgnome2_main_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gnome2_main`
libgnome2_main_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gnome2_main`
libgnome2_main_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin gnome2_main`
libgnome2_main_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gnome2_main`
libgnome2_main_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gnome2_main`
libgnome2_main_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin gnome2_main` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libgnome2_main_plugin_la_LIBADD = $(LTLIBVLC)

libgnome2_main_pic_a_SOURCES = $(SOURCES_gnome2_main)
libgnome2_main_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gnome2_main`
libgnome2_main_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gnome2_main`
libgnome2_main_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gnome2_main`

libgnome2_main_a_SOURCES = $(SOURCES_gnome2_main)
libgnome2_main_builtin_la_SOURCES = $(SOURCES_gnome2_main)
libgnome2_main_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gnome2_main`
libgnome2_main_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gnome2_main`
libgnome2_main_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gnome2_main`
libgnome2_main_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin gnome2_main`
libgnome2_main_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gnome2_main`
libgnome2_main_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gnome2_main`

# The screensaver plugin

libscreensaver_plugin_a_SOURCES = $(SOURCES_screensaver)
libscreensaver_plugin_la_SOURCES = $(SOURCES_screensaver)
libscreensaver_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin screensaver`
libscreensaver_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin screensaver`
libscreensaver_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin screensaver`
libscreensaver_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin screensaver`
libscreensaver_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin screensaver`
libscreensaver_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin screensaver`
libscreensaver_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin screensaver` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libscreensaver_plugin_la_LIBADD = $(LTLIBVLC)

libscreensaver_pic_a_SOURCES = $(SOURCES_screensaver)
libscreensaver_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic screensaver`
libscreensaver_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic screensaver`
libscreensaver_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic screensaver`

libscreensaver_a_SOURCES = $(SOURCES_screensaver)
libscreensaver_builtin_la_SOURCES = $(SOURCES_screensaver)
libscreensaver_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin screensaver`
libscreensaver_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin screensaver`
libscreensaver_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin screensaver`
libscreensaver_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin screensaver`
libscreensaver_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin screensaver`
libscreensaver_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin screensaver`

# The qte_main plugin

libqte_main_plugin_a_SOURCES = $(SOURCES_qte_main)
libqte_main_plugin_la_SOURCES = $(SOURCES_qte_main)
libqte_main_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin qte_main`
libqte_main_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin qte_main`
libqte_main_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin qte_main`
libqte_main_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin qte_main`
libqte_main_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin qte_main`
libqte_main_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin qte_main`
libqte_main_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin qte_main` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libqte_main_plugin_la_LIBADD = $(LTLIBVLC)

libqte_main_pic_a_SOURCES = $(SOURCES_qte_main)
libqte_main_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic qte_main`
libqte_main_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic qte_main`
libqte_main_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic qte_main`

libqte_main_a_SOURCES = $(SOURCES_qte_main)
libqte_main_builtin_la_SOURCES = $(SOURCES_qte_main)
libqte_main_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin qte_main`
libqte_main_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin qte_main`
libqte_main_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin qte_main`
libqte_main_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin qte_main`
libqte_main_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin qte_main`
libqte_main_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin qte_main`

# The freetype plugin

libfreetype_plugin_a_SOURCES = $(SOURCES_freetype)
libfreetype_plugin_la_SOURCES = $(SOURCES_freetype)
libfreetype_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin freetype`
libfreetype_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin freetype`
libfreetype_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin freetype`
libfreetype_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin freetype`
libfreetype_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin freetype`
libfreetype_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin freetype`
libfreetype_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin freetype` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libfreetype_plugin_la_LIBADD = $(LTLIBVLC)

libfreetype_pic_a_SOURCES = $(SOURCES_freetype)
libfreetype_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic freetype`
libfreetype_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic freetype`
libfreetype_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic freetype`

libfreetype_a_SOURCES = $(SOURCES_freetype)
libfreetype_builtin_la_SOURCES = $(SOURCES_freetype)
libfreetype_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin freetype`
libfreetype_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin freetype`
libfreetype_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin freetype`
libfreetype_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin freetype`
libfreetype_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin freetype`
libfreetype_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin freetype`

# The win32text plugin

libwin32text_plugin_a_SOURCES = $(SOURCES_win32text)
libwin32text_plugin_la_SOURCES = $(SOURCES_win32text)
libwin32text_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin win32text`
libwin32text_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin win32text`
libwin32text_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin win32text`
libwin32text_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin win32text`
libwin32text_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin win32text`
libwin32text_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin win32text`
libwin32text_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin win32text` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libwin32text_plugin_la_LIBADD = $(LTLIBVLC)

libwin32text_pic_a_SOURCES = $(SOURCES_win32text)
libwin32text_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic win32text`
libwin32text_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic win32text`
libwin32text_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic win32text`

libwin32text_a_SOURCES = $(SOURCES_win32text)
libwin32text_builtin_la_SOURCES = $(SOURCES_win32text)
libwin32text_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin win32text`
libwin32text_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin win32text`
libwin32text_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin win32text`
libwin32text_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin win32text`
libwin32text_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin win32text`
libwin32text_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin win32text`

# The logger plugin

liblogger_plugin_a_SOURCES = $(SOURCES_logger)
liblogger_plugin_la_SOURCES = $(SOURCES_logger)
liblogger_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin logger`
liblogger_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin logger`
liblogger_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin logger`
liblogger_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin logger`
liblogger_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin logger`
liblogger_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin logger`
liblogger_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin logger` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
liblogger_plugin_la_LIBADD = $(LTLIBVLC)

liblogger_pic_a_SOURCES = $(SOURCES_logger)
liblogger_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic logger`
liblogger_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic logger`
liblogger_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic logger`

liblogger_a_SOURCES = $(SOURCES_logger)
liblogger_builtin_la_SOURCES = $(SOURCES_logger)
liblogger_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin logger`
liblogger_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin logger`
liblogger_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin logger`
liblogger_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin logger`
liblogger_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin logger`
liblogger_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin logger`

# The vod_rtsp plugin

libvod_rtsp_plugin_a_SOURCES = $(SOURCES_vod_rtsp)
libvod_rtsp_plugin_la_SOURCES = $(SOURCES_vod_rtsp)
libvod_rtsp_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin vod_rtsp`
libvod_rtsp_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin vod_rtsp`
libvod_rtsp_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin vod_rtsp`
libvod_rtsp_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin vod_rtsp`
libvod_rtsp_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin vod_rtsp`
libvod_rtsp_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin vod_rtsp`
libvod_rtsp_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin vod_rtsp` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libvod_rtsp_plugin_la_LIBADD = $(LTLIBVLC)

libvod_rtsp_pic_a_SOURCES = $(SOURCES_vod_rtsp)
libvod_rtsp_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic vod_rtsp`
libvod_rtsp_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic vod_rtsp`
libvod_rtsp_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic vod_rtsp`

libvod_rtsp_a_SOURCES = $(SOURCES_vod_rtsp)
libvod_rtsp_builtin_la_SOURCES = $(SOURCES_vod_rtsp)
libvod_rtsp_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin vod_rtsp`
libvod_rtsp_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin vod_rtsp`
libvod_rtsp_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin vod_rtsp`
libvod_rtsp_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin vod_rtsp`
libvod_rtsp_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin vod_rtsp`
libvod_rtsp_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin vod_rtsp`

# The gnutls plugin

libgnutls_plugin_a_SOURCES = $(SOURCES_gnutls)
libgnutls_plugin_la_SOURCES = $(SOURCES_gnutls)
libgnutls_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gnutls`
libgnutls_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gnutls`
libgnutls_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gnutls`
libgnutls_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin gnutls`
libgnutls_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gnutls`
libgnutls_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gnutls`
libgnutls_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin gnutls` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libgnutls_plugin_la_LIBADD = $(LTLIBVLC)

libgnutls_pic_a_SOURCES = $(SOURCES_gnutls)
libgnutls_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gnutls`
libgnutls_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gnutls`
libgnutls_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gnutls`

libgnutls_a_SOURCES = $(SOURCES_gnutls)
libgnutls_builtin_la_SOURCES = $(SOURCES_gnutls)
libgnutls_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gnutls`
libgnutls_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gnutls`
libgnutls_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gnutls`
libgnutls_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin gnutls`
libgnutls_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gnutls`
libgnutls_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gnutls`

# The svg plugin

libsvg_plugin_a_SOURCES = $(SOURCES_svg)
libsvg_plugin_la_SOURCES = $(SOURCES_svg)
libsvg_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin svg`
libsvg_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin svg`
libsvg_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin svg`
libsvg_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin svg`
libsvg_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin svg`
libsvg_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin svg`
libsvg_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin svg` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libsvg_plugin_la_LIBADD = $(LTLIBVLC)

libsvg_pic_a_SOURCES = $(SOURCES_svg)
libsvg_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic svg`
libsvg_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic svg`
libsvg_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic svg`

libsvg_a_SOURCES = $(SOURCES_svg)
libsvg_builtin_la_SOURCES = $(SOURCES_svg)
libsvg_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin svg`
libsvg_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin svg`
libsvg_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin svg`
libsvg_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin svg`
libsvg_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin svg`
libsvg_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin svg`

# The msn plugin

libmsn_plugin_a_SOURCES = $(SOURCES_msn)
libmsn_plugin_la_SOURCES = $(SOURCES_msn)
libmsn_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin msn`
libmsn_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin msn`
libmsn_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin msn`
libmsn_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin msn`
libmsn_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin msn`
libmsn_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin msn`
libmsn_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin msn` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libmsn_plugin_la_LIBADD = $(LTLIBVLC)

libmsn_pic_a_SOURCES = $(SOURCES_msn)
libmsn_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic msn`
libmsn_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic msn`
libmsn_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic msn`

libmsn_a_SOURCES = $(SOURCES_msn)
libmsn_builtin_la_SOURCES = $(SOURCES_msn)
libmsn_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin msn`
libmsn_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin msn`
libmsn_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin msn`
libmsn_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin msn`
libmsn_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin msn`
libmsn_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin msn`

# The growl plugin

libgrowl_plugin_a_SOURCES = $(SOURCES_growl)
libgrowl_plugin_la_SOURCES = $(SOURCES_growl)
libgrowl_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin growl`
libgrowl_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin growl`
libgrowl_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin growl`
libgrowl_plugin_la_CFLAGS = `$(VLC_CONFIG) --cflags plugin growl`
libgrowl_plugin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin growl`
libgrowl_plugin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin growl`
libgrowl_plugin_la_LDFLAGS = `$(VLC_CONFIG) --libs plugin growl` \
	-rpath '$(libvlcdir)' -module -shrext $(LIBEXT)
libgrowl_plugin_la_LIBADD = $(LTLIBVLC)

libgrowl_pic_a_SOURCES = $(SOURCES_growl)
libgrowl_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic growl`
libgrowl_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic growl`
libgrowl_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic growl`

libgrowl_a_SOURCES = $(SOURCES_growl)
libgrowl_builtin_la_SOURCES = $(SOURCES_growl)
libgrowl_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin growl`
libgrowl_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin growl`
libgrowl_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin growl`
libgrowl_builtin_la_CFLAGS = `$(VLC_CONFIG) --cflags builtin growl`
libgrowl_builtin_la_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin growl`
libgrowl_builtin_la_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin growl`

