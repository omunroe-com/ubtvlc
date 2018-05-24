-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore4, mozilla-plugin-vlc, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-ggi, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-pulse, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-svgalib, vlc-plugin-zvbi
Architecture: any
Version: 1.1.11-1ubuntu1
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Sam Hocevar (Debian packages) <sam+deb@zoy.org>,           Clément Stenac <zorglub@debian.org>,           Loic Minier <lool@dooz.org>,           Christophe Mutricy <xtophe@videolan.org>,           Mohammed Adnène Trojette <adn+deb@diwi.org>,           Reinhard Tartler <siretart@tauware.de>,           Benjamin Drung <bdrung@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.2
Vcs-Browser: http://git.debian.org/?p=pkg-multimedia/vlc.git;a=summary
Vcs-Git: git://git.debian.org/git/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 7.2.3~), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 0.9.0beta10a) [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libass-dev (>= 0.9.6), libavahi-client-dev, libavc1394-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libavcodec-dev (>= 4:0.6), libavformat-dev (>= 4:0.6), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libdc1394-22-dev (>= 2.1.0) [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libdca-dev, libdirac-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev, libdvdread-dev (>= 0.9.5), libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev, libfreetype6-dev, libfribidi-dev, libggi2-dev, libgl1-mesa-dev, libglib2.0-0, libgnutls-dev (>= 1.7.4), libgtk2.0-dev, libjack-dev, libkate-dev (>= 0.1.5), liblircclient-dev, liblivemedia-dev (>= 2009.11.27), liblua5.1-0-dev, libmad0-dev, libmatroska-dev (>= 0.8.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev, libpng12-dev, libpostproc-dev (>= 4:0.6), libproxy-dev, libpulse-dev (>= 0.9.11), libqt4-dev (>= 4.4.0), libraw1394-dev (>= 2.0.1) [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], librsvg2-dev, libschroedinger-dev (>= 1.0.6), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshout3-dev, libsmbclient-dev, libspeex-dev, libsvga1-dev [amd64 i386], libswscale-dev (>= 4:0.6), libtag1-dev (>= 1.5), libtar-dev, libtheora-dev, libtwolame-dev (>= 0.3.8), libudev-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libupnp3-dev, libv4l-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libva-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libvcdinfo-dev (>= 0.7.22), libvorbis-dev, libx11-dev, libx11-xcb-dev, libx264-dev (>= 2:0.76), libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev, libxext-dev, libxml2-dev, libxpm-dev, libxt-dev, libzvbi-dev (>= 0.2.28), lua5.1, nasm, pkg-config, xulrunner-dev (>= 1.9.1), yasm [amd64 kfreebsd-amd64], zlib1g-dev
Checksums-Sha1: 
 068e75bdbfe6e595a4db14ad49e05688c8b1d5ad 26319862 vlc_1.1.11.orig.tar.bz2
 b0066dd5c1f1979f5954d9d4d1ac42f1d460762c 59618 vlc_1.1.11-1ubuntu1.debian.tar.gz
Checksums-Sha256: 
 682560be08b82bedfaf30d8a611d80093c5883c1de72fcbcf05715b8e9f4e1cb 26319862 vlc_1.1.11.orig.tar.bz2
 4da4f26dba3ae8410b808e10a2ff57c73a46e21466f62707fe5de70dad0c3311 59618 vlc_1.1.11-1ubuntu1.debian.tar.gz
Files: 
 a64846d6f21ea179ae8e8bfb6f9447fe 26319862 vlc_1.1.11.orig.tar.bz2
 658c1259b305fff2797577374365340c 59618 vlc_1.1.11-1ubuntu1.debian.tar.gz
Original-Maintainer: Debian multimedia packages maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1.4.11 (GNU/Linux)

iQIcBAEBCgAGBQJOJAy5AAoJEBWetcTvyHdMjaMP/2cDKTd4lX64vK8AVTkTCGF6
FuJYJ0ZsVf/uqiuaN0pi5I7K0RltFBryj9wPE9tadg4L25yjpyKHLusIcQpZkr6B
cl0+zM2ToLEW7cpz2dwD0qaovuYlAyMYWhE1HM31r2QqUOOv9tG5QNXsA+VuJdhN
Z1XZa6SHImkRgnp44Y77f/Jh9lTV7Z+rnx8d/crVOmNYGG4aVZboE7UtCETFZmaq
oShY32BgDaDmFLpYyDs/OyLMAkfaoTlcndtNRvFCEqlUHVyD7hgYLZJ46O/oQ9YF
DHB6gAL80gFW77FoMwbtYGQ2L2q6rTjJpVrsEYJedid2TwTeS4SABZZENzmvkUFS
W4aYSP1vkXuAhuTnMxmCG1qqC0kwrZYh9K3zZ1mR9PYLVNzSSx0Ecj9IH+lGRD1V
ctA5zYoXvG2ZzQv8oxtmVqqHIh0hTWtMHY34m6vxvDKOXVAlsOopQVL2gy8gksUy
VyD6FyXKUJC/AR6ACyJXwuuPN0kxZlmuNAt82PFf3iZqD+Tu4GT2yM4W1mBIvKF5
K7HCp/rqV1fGwYaIRxQjg7ILVpFVL9cVq2bU3llemgL6R1dqqxCXLThLisfDXahe
WG2N9UpEk7DlDv2o9+SwPjhJbwScMzMY0YcYbpB53oV5EE1YmMWfTS3ZQG00oQjN
guZti+hw+XADdbW1spph
=+x5x
-----END PGP SIGNATURE-----
