-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA1

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore4, mozilla-plugin-vlc, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-ggi, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-pulse, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-svgalib, vlc-plugin-zvbi
Architecture: any all
Version: 1.1.12-2ubuntu1
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Sam Hocevar (Debian packages) <sam+deb@zoy.org>, Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.2
Vcs-Browser: http://git.debian.org/?p=pkg-multimedia/vlc.git;a=summary
Vcs-Git: git://git.debian.org/git/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 7.2.3~), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 0.9.0beta10a) [linux-any], libass-dev (>= 0.9.6), libavahi-client-dev, libavc1394-dev [linux-any], libavcodec-dev (>= 4:0.6), libavformat-dev (>= 4:0.6), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirac-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev, libdvdread-dev (>= 0.9.5), libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev, libfreetype6-dev, libfribidi-dev, libggi2-dev, libgl1-mesa-dev, libglib2.0-0, libgnutls-dev (>= 1.7.4), libgtk2.0-dev, libjack-dev, libkate-dev (>= 0.1.5), liblircclient-dev, liblivemedia-dev (>= 2009.11.27), liblua5.1-0-dev, libmad0-dev, libmatroska-dev (>= 0.8.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev, libpng12-dev, libpostproc-dev (>= 4:0.6), libproxy-dev, libpulse-dev (>= 0.9.11), libqt4-dev (>= 4.4.0), libraw1394-dev (>= 2.0.1) [linux-any], librsvg2-dev, libschroedinger-dev (>= 1.0.6), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshout3-dev, libsmbclient-dev, libspeex-dev, libsvga1-dev [amd64 i386], libswscale-dev (>= 4:0.6), libtag1-dev (>= 1.5), libtar-dev, libtheora-dev, libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp3-dev, libv4l-dev [linux-any], libva-dev [linux-any], libvcdinfo-dev (>= 0.7.22), libvorbis-dev, libx11-dev, libx11-xcb-dev, libx264-dev (>= 2:0.76), libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev, libxext-dev, libxml2-dev, libxpm-dev, libxt-dev, libzvbi-dev (>= 0.2.28), lua5.1, nasm, pkg-config, xulrunner-dev (>= 1.9.1) | firefox-dev (>= 4.0), yasm [amd64 kfreebsd-amd64], zlib1g-dev
Package-List: 
 libvlc-dev deb libdevel optional
 libvlc5 deb libs optional
 libvlccore-dev deb libdevel optional
 libvlccore4 deb libs optional
 mozilla-plugin-vlc deb video optional
 vlc deb video optional
 vlc-data deb video optional
 vlc-dbg deb debug extra
 vlc-nox deb video optional
 vlc-plugin-fluidsynth deb video optional
 vlc-plugin-ggi deb video optional
 vlc-plugin-jack deb video optional
 vlc-plugin-notify deb video optional
 vlc-plugin-pulse deb video optional
 vlc-plugin-sdl deb video optional
 vlc-plugin-svg deb video optional
 vlc-plugin-svgalib deb video optional
 vlc-plugin-zvbi deb video optional
Checksums-Sha1: 
 bd4204ed8302a11e67aaf230a442bb84d0c5ca99 14871744 vlc_1.1.12.orig.tar.xz
 6a012ed9e9c23dced1664931a1330a19fce4c695 244932 vlc_1.1.12-2ubuntu1.debian.tar.gz
Checksums-Sha256: 
 d79c60a2c999b718844469c1889047d1b58cc29e1ee05670d4ce7de4fc2f7907 14871744 vlc_1.1.12.orig.tar.xz
 333483cab594798f8846d870a9d3314819ac3bda586c0f54c094d42b20bc3299 244932 vlc_1.1.12-2ubuntu1.debian.tar.gz
Files: 
 56cb9975725a12da8c84882bc00fdce0 14871744 vlc_1.1.12.orig.tar.xz
 6b43cab0ce17b603218a2a126d6f7ef4 244932 vlc_1.1.12-2ubuntu1.debian.tar.gz
Original-Maintainer: Debian multimedia packages maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1.4.11 (GNU/Linux)

iQIcBAEBAgAGBQJOw+x4AAoJEMEmM+HuAYyTQ6EP/iHgpgG90dhw3VUjjB4lESM9
TuGA4+EOHkY2FsSilMj798N1l6NCXupwD1WYfDNj2fwQKzEJ79AGhOxJh3d5VN/Q
cb2f2BkSETbOm4c1Hy8mwNZt8IcLapm39rEnnLnRp55lyWCsiiENEOmgCl7fobxt
axqa6KVCO7P1poF6hHfM4bBO9yWWeAptutsfRgHTzFRNpBFIvqJWeW563dCpefGv
m4IMzOWBu4AtkqYx3flk1zVOTsFAz3VwSbqOsn9D+C+qaveLcCmr88JyTBWeffjP
+0DWPLPDZ72E3+dAmqgI8daXH+4Aoh1HrsJEnpGleelSdpUgE5GimZx4FxrWN0Q1
xa0i0i6IQTb9vd3vHFFIYZ0UIrTa9+lmEA11SCgmhz0BvTCjECGueV88rRwCom6f
v0YB+YyABTYXLJEsY3e+7xJP9mesgSUDM9rUEYbM+7r//oW7N/WVR9yL3NcTQTZw
F1A/7CiHq1SYE3RceZFK7RXRStNFPVYUQ/24hUFs9U2b9wis2kpZPIV2DcUXVVie
csIq5BI5uZWiK+PUSWfkV+aEmTW7mma3Bqu16AWY2xik2/gDsPMUUWIIhQMq/Pst
us3cWXe/AWx9mvp8URZW4+ecI9469QYSHGOYoOlnCaVZMdEJULcAGZ2/pnSlfqMw
ZvXevme32lVfO0kDzjGn
=I2i/
-----END PGP SIGNATURE-----
