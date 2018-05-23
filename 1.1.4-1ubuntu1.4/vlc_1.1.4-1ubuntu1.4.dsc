-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc5, libvlc-dev, libvlccore4, libvlccore-dev, mozilla-plugin-vlc, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-ggi, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-pulse, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-svgalib, vlc-plugin-zvbi
Architecture: any
Version: 1.1.4-1ubuntu1.4
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Sam Hocevar (Debian packages) <sam+deb@zoy.org>,           Clément Stenac <zorglub@debian.org>,           Loic Minier <lool@dooz.org>,           Christophe Mutricy <xtophe@videolan.org>,           Mohammed Adnène Trojette <adn+deb@diwi.org>,           Benjamin Drung <bdrung@ubuntu.com>
Dm-Upload-Allowed: yes
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.1
Vcs-Browser: http://git.debian.org/?p=pkg-multimedia/vlc.git;a=summary
Vcs-Git: git://git.debian.org/git/pkg-multimedia/vlc.git
Build-Depends: debhelper (>= 7.2.3~), dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 0.9.0beta10a) [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libass-dev (>= 0.9.5-2), libavahi-client-dev, libavc1394-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libavcodec-dev (>= 4:0.6), libavformat-dev (>= 4:0.6), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev, libdc1394-22-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libdca-dev, libdirac-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev, libdvdread-dev (>= 0.9.5), libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev, libfreetype6-dev, libfribidi-dev, libggi2-dev, libgl1-mesa-dev, libglib2.0-0, libgnutls-dev (>= 1.2.8), libjack-dev, libkate-dev, liblircclient-dev, liblivemedia-dev (>= 2009.11.27), liblua5.1-0-dev, libmad0-dev, libmatroska-dev (>= 0.8.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev, libncursesw5-dev, libnotify-dev, libogg-dev, libpng12-dev, libpostproc-dev (>= 4:0.6), libproxy-dev, libpulse-dev, libqt4-dev, libraw1394-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], librsvg2-dev, libschroedinger-dev (>= 1.0.7-1~), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.7+1.2.8cvs20041007-5.3), libshout3-dev, libsmbclient-dev, libspeex-dev, libsvga1-dev [amd64 i386], libswscale-dev (>= 4:0.6), libtag1-dev, libtar-dev, libtheora-dev, libtwolame-dev (>= 0.3.8), libudev-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libupnp3-dev, libv4l-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libva-dev [!kfreebsd-i386 !kfreebsd-amd64 !hurd-i386], libvcdinfo-dev, libvorbis-dev, libx11-dev, libx11-xcb-dev, libx264-dev, libxcb-keysyms1-dev, libxcb-randr0-dev, libxcb-shm0-dev, libxcb-xv0-dev, libxcb1-dev, libxext-dev, libxml2-dev, libxpm-dev, libxt-dev, libzvbi-dev, lua5.1, nasm, pkg-config, xulrunner-dev (>= 1.9.2), yasm [amd64 kfreebsd-amd64], zlib1g-dev
Checksums-Sha1: 
 2c4f85fbccbd11946cb4b2cf94464e5db38db78a 23923550 vlc_1.1.4.orig.tar.bz2
 9d18ef855250bd77af4b699ba8c07901efc8feed 60680 vlc_1.1.4-1ubuntu1.4.debian.tar.gz
Checksums-Sha256: 
 0951493d27af3f950ed57ff123d9cb17ed1d17c8ae51741d3a90f4973af397d7 23923550 vlc_1.1.4.orig.tar.bz2
 9ccab2a6d2cde3f6563c9f1e08f652b83ebdad116a7ff9d3aeb3b210d8e87d79 60680 vlc_1.1.4-1ubuntu1.4.debian.tar.gz
Files: 
 b71aacaa242f8050a64c6ffe09d6f067 23923550 vlc_1.1.4.orig.tar.bz2
 2c4a4c7ee8f0998f7a7cbce53c45f818 60680 vlc_1.1.4-1ubuntu1.4.debian.tar.gz
Original-Maintainer: Debian multimedia packages maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1.4.10 (GNU/Linux)

iQIbBAEBCgAGBQJNWpNdAAoJEFHb3FjMVZVzaC4P91YZvcWOWGKpL3KVnySHvxZd
2ZOwAhg0xHYb/A0NQDdOaJR3JFPjBb12NPSkDemp3b7o5Rhx5+gs8Pnf+wnoDscJ
Q6+2ftbRiUgDo5jookpUyslWLeL84T7FR6OjwzGpD9IbssJVKYua/UWVIPGe+d/V
u/QWXQar1qef9xjpn1d6TmB5gZoxykvClts6bVDnsPo2Q9WKJSztQrVHUj53Sb/3
OCa9kqsuGxevtDfrmhaqMSKO3WALH4MdLdoUjNGgj3W0NyHTrgj60AFsR763+VRA
0TMyKrVrWYX0V5MDrIUemt2ZkCOydnd/pd6a/pCOOqHyRuQmV8rrklVTWyPeDcjc
3tUKybIDGuffP2rVXNB7f+T98/Jsg8fzzr5ChFG7Z91LaDVakDDgtcRAQtPj0iwi
AZ6gCeZ+YGz1VuhSzTBLsgwpdd1A1Q6chAiJ+hwv3Qcj6mNnzjhVE9MEOFBbLHAD
rchZlOTlSRbg5RPYiKKiPZPI461LegsL/5tFTMW0mIWh60w4BlJDtcN01IfmKWt4
oORVrbaG1KBmuFzxf9EvmO2cz8NnMUht4n5lrei34kpm0UYDl6sDEUVxAwEPW+TA
i4qNcKQcmdEX4oXyqotqS5jMctDaJ4u8yLnLxIywLyCKgpldF8UOcqwFAYx4oINO
jCtxB7oBXBseTE8iIvI=
=cHjS
-----END PGP SIGNATURE-----
