
# modules/access/Makefile.am automatically generated from modules/access/Modules.am by bootstrap
# DO NOT EDIT - edit Modules.am or $(top_srcdir)/bootstrap instead

basedir = access
mods = access_file access_directory access_udp access_tcp access_http access_ftp access_smb access_gnomevfs dvdnav dvdread access_fake cdda

NULL =
libvlc_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
EXTRA_DIST = Modules.am
BUILT_SOURCES =
SUBDIRS = dshow dvb mms pvr v4l cdda rtsp vcd vcdx screen
SUFFIXES = _plugin$(LIBEXT) _plugin.a

libvlcdir = $(libdir)/vlc/$(basedir)

EXTRA_LIBRARIES =  libaccess_file_plugin.a libaccess_file.a libaccess_file_pic.a libaccess_directory_plugin.a libaccess_directory.a libaccess_directory_pic.a libaccess_udp_plugin.a libaccess_udp.a libaccess_udp_pic.a libaccess_tcp_plugin.a libaccess_tcp.a libaccess_tcp_pic.a libaccess_http_plugin.a libaccess_http.a libaccess_http_pic.a libaccess_ftp_plugin.a libaccess_ftp.a libaccess_ftp_pic.a libaccess_smb_plugin.a libaccess_smb.a libaccess_smb_pic.a libaccess_gnomevfs_plugin.a libaccess_gnomevfs.a libaccess_gnomevfs_pic.a libdvdnav_plugin.a libdvdnav.a libdvdnav_pic.a libdvdread_plugin.a libdvdread.a libdvdread_pic.a libaccess_fake_plugin.a libaccess_fake.a libaccess_fake_pic.a libcdda_plugin.a libcdda.a libcdda_pic.a
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

# The access_file plugin

libaccess_file_plugin_a_SOURCES = $(SOURCES_access_file)
libaccess_file_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin access_file`
libaccess_file_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin access_file`
libaccess_file_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin access_file`

libaccess_file_pic_a_SOURCES = $(SOURCES_access_file)
libaccess_file_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic access_file`
libaccess_file_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic access_file`
libaccess_file_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic access_file`

libaccess_file_a_SOURCES = $(SOURCES_access_file)
libaccess_file_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin access_file`
libaccess_file_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin access_file`
libaccess_file_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin access_file`

# The access_directory plugin

libaccess_directory_plugin_a_SOURCES = $(SOURCES_access_directory)
libaccess_directory_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin access_directory`
libaccess_directory_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin access_directory`
libaccess_directory_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin access_directory`

libaccess_directory_pic_a_SOURCES = $(SOURCES_access_directory)
libaccess_directory_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic access_directory`
libaccess_directory_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic access_directory`
libaccess_directory_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic access_directory`

libaccess_directory_a_SOURCES = $(SOURCES_access_directory)
libaccess_directory_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin access_directory`
libaccess_directory_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin access_directory`
libaccess_directory_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin access_directory`

# The access_udp plugin

libaccess_udp_plugin_a_SOURCES = $(SOURCES_access_udp)
libaccess_udp_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin access_udp`
libaccess_udp_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin access_udp`
libaccess_udp_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin access_udp`

libaccess_udp_pic_a_SOURCES = $(SOURCES_access_udp)
libaccess_udp_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic access_udp`
libaccess_udp_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic access_udp`
libaccess_udp_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic access_udp`

libaccess_udp_a_SOURCES = $(SOURCES_access_udp)
libaccess_udp_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin access_udp`
libaccess_udp_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin access_udp`
libaccess_udp_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin access_udp`

# The access_tcp plugin

libaccess_tcp_plugin_a_SOURCES = $(SOURCES_access_tcp)
libaccess_tcp_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin access_tcp`
libaccess_tcp_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin access_tcp`
libaccess_tcp_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin access_tcp`

libaccess_tcp_pic_a_SOURCES = $(SOURCES_access_tcp)
libaccess_tcp_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic access_tcp`
libaccess_tcp_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic access_tcp`
libaccess_tcp_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic access_tcp`

libaccess_tcp_a_SOURCES = $(SOURCES_access_tcp)
libaccess_tcp_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin access_tcp`
libaccess_tcp_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin access_tcp`
libaccess_tcp_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin access_tcp`

# The access_http plugin

libaccess_http_plugin_a_SOURCES = $(SOURCES_access_http)
libaccess_http_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin access_http`
libaccess_http_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin access_http`
libaccess_http_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin access_http`

libaccess_http_pic_a_SOURCES = $(SOURCES_access_http)
libaccess_http_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic access_http`
libaccess_http_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic access_http`
libaccess_http_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic access_http`

libaccess_http_a_SOURCES = $(SOURCES_access_http)
libaccess_http_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin access_http`
libaccess_http_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin access_http`
libaccess_http_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin access_http`

# The access_ftp plugin

libaccess_ftp_plugin_a_SOURCES = $(SOURCES_access_ftp)
libaccess_ftp_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin access_ftp`
libaccess_ftp_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin access_ftp`
libaccess_ftp_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin access_ftp`

libaccess_ftp_pic_a_SOURCES = $(SOURCES_access_ftp)
libaccess_ftp_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic access_ftp`
libaccess_ftp_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic access_ftp`
libaccess_ftp_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic access_ftp`

libaccess_ftp_a_SOURCES = $(SOURCES_access_ftp)
libaccess_ftp_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin access_ftp`
libaccess_ftp_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin access_ftp`
libaccess_ftp_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin access_ftp`

# The access_smb plugin

libaccess_smb_plugin_a_SOURCES = $(SOURCES_access_smb)
libaccess_smb_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin access_smb`
libaccess_smb_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin access_smb`
libaccess_smb_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin access_smb`

libaccess_smb_pic_a_SOURCES = $(SOURCES_access_smb)
libaccess_smb_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic access_smb`
libaccess_smb_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic access_smb`
libaccess_smb_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic access_smb`

libaccess_smb_a_SOURCES = $(SOURCES_access_smb)
libaccess_smb_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin access_smb`
libaccess_smb_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin access_smb`
libaccess_smb_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin access_smb`

# The access_gnomevfs plugin

libaccess_gnomevfs_plugin_a_SOURCES = $(SOURCES_access_gnomevfs)
libaccess_gnomevfs_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin access_gnomevfs`
libaccess_gnomevfs_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin access_gnomevfs`
libaccess_gnomevfs_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin access_gnomevfs`

libaccess_gnomevfs_pic_a_SOURCES = $(SOURCES_access_gnomevfs)
libaccess_gnomevfs_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic access_gnomevfs`
libaccess_gnomevfs_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic access_gnomevfs`
libaccess_gnomevfs_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic access_gnomevfs`

libaccess_gnomevfs_a_SOURCES = $(SOURCES_access_gnomevfs)
libaccess_gnomevfs_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin access_gnomevfs`
libaccess_gnomevfs_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin access_gnomevfs`
libaccess_gnomevfs_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin access_gnomevfs`

# The dvdnav plugin

libdvdnav_plugin_a_SOURCES = $(SOURCES_dvdnav)
libdvdnav_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin dvdnav`
libdvdnav_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin dvdnav`
libdvdnav_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin dvdnav`

libdvdnav_pic_a_SOURCES = $(SOURCES_dvdnav)
libdvdnav_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic dvdnav`
libdvdnav_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic dvdnav`
libdvdnav_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic dvdnav`

libdvdnav_a_SOURCES = $(SOURCES_dvdnav)
libdvdnav_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin dvdnav`
libdvdnav_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin dvdnav`
libdvdnav_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin dvdnav`

# The dvdread plugin

libdvdread_plugin_a_SOURCES = $(SOURCES_dvdread)
libdvdread_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin dvdread`
libdvdread_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin dvdread`
libdvdread_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin dvdread`

libdvdread_pic_a_SOURCES = $(SOURCES_dvdread)
libdvdread_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic dvdread`
libdvdread_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic dvdread`
libdvdread_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic dvdread`

libdvdread_a_SOURCES = $(SOURCES_dvdread)
libdvdread_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin dvdread`
libdvdread_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin dvdread`
libdvdread_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin dvdread`

# The access_fake plugin

libaccess_fake_plugin_a_SOURCES = $(SOURCES_access_fake)
libaccess_fake_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin access_fake`
libaccess_fake_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin access_fake`
libaccess_fake_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin access_fake`

libaccess_fake_pic_a_SOURCES = $(SOURCES_access_fake)
libaccess_fake_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic access_fake`
libaccess_fake_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic access_fake`
libaccess_fake_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic access_fake`

libaccess_fake_a_SOURCES = $(SOURCES_access_fake)
libaccess_fake_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin access_fake`
libaccess_fake_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin access_fake`
libaccess_fake_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin access_fake`

# The cdda plugin

libcdda_plugin_a_SOURCES = $(SOURCES_cdda)
libcdda_plugin_a_CFLAGS = `$(VLC_CONFIG) --cflags plugin cdda`
libcdda_plugin_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags plugin cdda`
libcdda_plugin_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags plugin cdda`

libcdda_pic_a_SOURCES = $(SOURCES_cdda)
libcdda_pic_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin pic cdda`
libcdda_pic_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin pic cdda`
libcdda_pic_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin pic cdda`

libcdda_a_SOURCES = $(SOURCES_cdda)
libcdda_a_CFLAGS = `$(VLC_CONFIG) --cflags builtin cdda`
libcdda_a_CXXFLAGS = `$(VLC_CONFIG) --cxxflags builtin cdda`
libcdda_a_OBJCFLAGS = `$(VLC_CONFIG) --objcflags builtin cdda`

