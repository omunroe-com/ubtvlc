
# modules/misc/Makefile.am automatically generated from modules/misc/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = misc
mods = gtk_main gnome_main gtk2_main gnome2_main screensaver qte_main freetype win32text logger vod_rtsp gnutls svg

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = dummy memcpy network testsuite playlist xml
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libgtk_main_plugin.a libgtk_main.a libgtk_main_pic.a libgnome_main_plugin.a libgnome_main.a libgnome_main_pic.a libgtk2_main_plugin.a libgtk2_main.a libgtk2_main_pic.a libgnome2_main_plugin.a libgnome2_main.a libgnome2_main_pic.a libscreensaver_plugin.a libscreensaver.a libscreensaver_pic.a libqte_main_plugin.a libqte_main.a libqte_main_pic.a libfreetype_plugin.a libfreetype.a libfreetype_pic.a libwin32text_plugin.a libwin32text.a libwin32text_pic.a liblogger_plugin.a liblogger.a liblogger_pic.a libvod_rtsp_plugin.a libvod_rtsp.a libvod_rtsp_pic.a libgnutls_plugin.a libgnutls.a libgnutls_pic.a libsvg_plugin.a libsvg.a libsvg_pic.a
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

# The gtk_main plugin

libgtk_main_plugin_a_SOURCES = $(SOURCES_gtk_main)
libgtk_main_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gtk_main`
libgtk_main_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gtk_main`
libgtk_main_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gtk_main`

libgtk_main_pic_a_SOURCES = $(SOURCES_gtk_main)
libgtk_main_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gtk_main`
libgtk_main_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gtk_main`
libgtk_main_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gtk_main`

libgtk_main_a_SOURCES = $(SOURCES_gtk_main)
libgtk_main_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gtk_main`
libgtk_main_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gtk_main`
libgtk_main_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gtk_main`

# The gnome_main plugin

libgnome_main_plugin_a_SOURCES = $(SOURCES_gnome_main)
libgnome_main_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gnome_main`
libgnome_main_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gnome_main`
libgnome_main_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gnome_main`

libgnome_main_pic_a_SOURCES = $(SOURCES_gnome_main)
libgnome_main_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gnome_main`
libgnome_main_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gnome_main`
libgnome_main_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gnome_main`

libgnome_main_a_SOURCES = $(SOURCES_gnome_main)
libgnome_main_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gnome_main`
libgnome_main_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gnome_main`
libgnome_main_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gnome_main`

# The gtk2_main plugin

libgtk2_main_plugin_a_SOURCES = $(SOURCES_gtk2_main)
libgtk2_main_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gtk2_main`
libgtk2_main_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gtk2_main`
libgtk2_main_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gtk2_main`

libgtk2_main_pic_a_SOURCES = $(SOURCES_gtk2_main)
libgtk2_main_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gtk2_main`
libgtk2_main_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gtk2_main`
libgtk2_main_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gtk2_main`

libgtk2_main_a_SOURCES = $(SOURCES_gtk2_main)
libgtk2_main_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gtk2_main`
libgtk2_main_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gtk2_main`
libgtk2_main_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gtk2_main`

# The gnome2_main plugin

libgnome2_main_plugin_a_SOURCES = $(SOURCES_gnome2_main)
libgnome2_main_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gnome2_main`
libgnome2_main_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gnome2_main`
libgnome2_main_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gnome2_main`

libgnome2_main_pic_a_SOURCES = $(SOURCES_gnome2_main)
libgnome2_main_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gnome2_main`
libgnome2_main_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gnome2_main`
libgnome2_main_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gnome2_main`

libgnome2_main_a_SOURCES = $(SOURCES_gnome2_main)
libgnome2_main_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gnome2_main`
libgnome2_main_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gnome2_main`
libgnome2_main_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gnome2_main`

# The screensaver plugin

libscreensaver_plugin_a_SOURCES = $(SOURCES_screensaver)
libscreensaver_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin screensaver`
libscreensaver_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin screensaver`
libscreensaver_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin screensaver`

libscreensaver_pic_a_SOURCES = $(SOURCES_screensaver)
libscreensaver_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic screensaver`
libscreensaver_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic screensaver`
libscreensaver_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic screensaver`

libscreensaver_a_SOURCES = $(SOURCES_screensaver)
libscreensaver_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin screensaver`
libscreensaver_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin screensaver`
libscreensaver_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin screensaver`

# The qte_main plugin

libqte_main_plugin_a_SOURCES = $(SOURCES_qte_main)
libqte_main_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin qte_main`
libqte_main_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin qte_main`
libqte_main_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin qte_main`

libqte_main_pic_a_SOURCES = $(SOURCES_qte_main)
libqte_main_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic qte_main`
libqte_main_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic qte_main`
libqte_main_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic qte_main`

libqte_main_a_SOURCES = $(SOURCES_qte_main)
libqte_main_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin qte_main`
libqte_main_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin qte_main`
libqte_main_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin qte_main`

# The freetype plugin

libfreetype_plugin_a_SOURCES = $(SOURCES_freetype)
libfreetype_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin freetype`
libfreetype_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin freetype`
libfreetype_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin freetype`

libfreetype_pic_a_SOURCES = $(SOURCES_freetype)
libfreetype_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic freetype`
libfreetype_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic freetype`
libfreetype_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic freetype`

libfreetype_a_SOURCES = $(SOURCES_freetype)
libfreetype_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin freetype`
libfreetype_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin freetype`
libfreetype_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin freetype`

# The win32text plugin

libwin32text_plugin_a_SOURCES = $(SOURCES_win32text)
libwin32text_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin win32text`
libwin32text_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin win32text`
libwin32text_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin win32text`

libwin32text_pic_a_SOURCES = $(SOURCES_win32text)
libwin32text_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic win32text`
libwin32text_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic win32text`
libwin32text_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic win32text`

libwin32text_a_SOURCES = $(SOURCES_win32text)
libwin32text_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin win32text`
libwin32text_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin win32text`
libwin32text_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin win32text`

# The logger plugin

liblogger_plugin_a_SOURCES = $(SOURCES_logger)
liblogger_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin logger`
liblogger_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin logger`
liblogger_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin logger`

liblogger_pic_a_SOURCES = $(SOURCES_logger)
liblogger_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic logger`
liblogger_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic logger`
liblogger_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic logger`

liblogger_a_SOURCES = $(SOURCES_logger)
liblogger_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin logger`
liblogger_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin logger`
liblogger_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin logger`

# The vod_rtsp plugin

libvod_rtsp_plugin_a_SOURCES = $(SOURCES_vod_rtsp)
libvod_rtsp_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin vod_rtsp`
libvod_rtsp_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin vod_rtsp`
libvod_rtsp_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin vod_rtsp`

libvod_rtsp_pic_a_SOURCES = $(SOURCES_vod_rtsp)
libvod_rtsp_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic vod_rtsp`
libvod_rtsp_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic vod_rtsp`
libvod_rtsp_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic vod_rtsp`

libvod_rtsp_a_SOURCES = $(SOURCES_vod_rtsp)
libvod_rtsp_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin vod_rtsp`
libvod_rtsp_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin vod_rtsp`
libvod_rtsp_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin vod_rtsp`

# The gnutls plugin

libgnutls_plugin_a_SOURCES = $(SOURCES_gnutls)
libgnutls_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin gnutls`
libgnutls_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin gnutls`
libgnutls_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin gnutls`

libgnutls_pic_a_SOURCES = $(SOURCES_gnutls)
libgnutls_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic gnutls`
libgnutls_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic gnutls`
libgnutls_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic gnutls`

libgnutls_a_SOURCES = $(SOURCES_gnutls)
libgnutls_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin gnutls`
libgnutls_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin gnutls`
libgnutls_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin gnutls`

# The svg plugin

libsvg_plugin_a_SOURCES = $(SOURCES_svg)
libsvg_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin svg`
libsvg_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin svg`
libsvg_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin svg`

libsvg_pic_a_SOURCES = $(SOURCES_svg)
libsvg_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic svg`
libsvg_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic svg`
libsvg_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic svg`

libsvg_a_SOURCES = $(SOURCES_svg)
libsvg_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin svg`
libsvg_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin svg`
libsvg_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin svg`

