-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba
Architecture: any all
Version: 2.2.2-5build1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.7
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, debhelper (>= 9.20151219), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 6:9), libavformat-dev (>= 4:0.6), libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6), libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, qtbase5-dev (>= 5.1), zlib1g-dev, zsh
Package-List:
 libvlc-dev deb libdevel optional arch=any
 libvlc5 deb libs optional arch=any
 libvlccore-dev deb libdevel optional arch=any
 libvlccore8 deb libs optional arch=any
 vlc deb video optional arch=any
 vlc-data deb video optional arch=all
 vlc-nox deb video optional arch=any
 vlc-plugin-fluidsynth deb video optional arch=any
 vlc-plugin-jack deb video optional arch=any
 vlc-plugin-notify deb video optional arch=any
 vlc-plugin-samba deb video optional arch=linux-any,kfreebsd-any
 vlc-plugin-sdl deb video optional arch=any
 vlc-plugin-svg deb video optional arch=any
 vlc-plugin-zvbi deb video optional arch=any
Checksums-Sha1:
 608e27ffdcb303e1de8c81e1917dcd573d141aa9 22132140 vlc_2.2.2.orig.tar.xz
 1dab451be7ed088bef598fcdd791ba8645f5e698 62124 vlc_2.2.2-5build1.debian.tar.xz
Checksums-Sha256:
 7f729245357f337f51f34b91fa896e5b62dd7bd06dc996bab88d9f5623eaedd3 22132140 vlc_2.2.2.orig.tar.xz
 f31988374eb5b16aba7d8d632043928e7618d3c77149f2ad8e48eb122abd7c28 62124 vlc_2.2.2-5build1.debian.tar.xz
Files:
 342fe7d6ae9b0ae1580e18c3326cdca2 22132140 vlc_2.2.2.orig.tar.xz
 033149ab536c850ea6fc6127d362ee61 62124 vlc_2.2.2-5build1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCAAGBQJXHJOoAAoJEL1+qmB3j6b1e4EQAKEvlexZPF3eC5CQI1AWbst4
gpKYDIb6aMiYzl6ib4oX+y2CZwGN9kNhkRvfOkxnqJ64sAAYcasq165b137hjaOB
1q5Oega5Usv8Rdv7w6p7wY+rBVFkLePTzOVONVPr8AOBRISfhIUj3uJbn2A8lHSs
p862NyKVoXaEH7+fn87ojMl3r5lonHb+F5XLU78AsHj2ffUpRj5xr/fU/cSQ851d
W5/+WzStpgIfVEtHy1I9NnhHySA9HPlNdl2xHzFlDUsYUVy1Om5N294VUjjexXdb
R4jXtnWCYSafHl7YwOCqNd0KM75PyNO4YA+NmfxE3WDOrqgoG+5HQsd6x05Sz0Rv
6wsSu2511essFQS83abdD03EXgxoCETJG7evGs36RnRVenciqVDPFl7CXtEzNlr6
jERoHDSyDiR7ZlS4MyQHzK2b/ihIe1vgkkxGgbZIARniBefRmJhb1J2QyYkhOjSJ
zNL9CQH/cYrn8RnluD/H2TZ1MEhlW0RJSp7AJ7IEuQLBt26CsLQl8/NqylSUjkmp
n1RSHyei7l2s1PIsqLu3IgbKc7vyG5azaQ23UA//UBZUGiYn/Gxgc2CmnsLoWU8c
Yf9ZNhcDXrHdDLgclqR/7A/i4kxWAuVAHgYocCvDPREjkJiQjSoEPXWG+f11oORB
yZflJNrWOjapB58YPMIq
=cepr
-----END PGP SIGNATURE-----
