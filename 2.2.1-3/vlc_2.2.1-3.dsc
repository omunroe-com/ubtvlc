-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.1-3
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.6
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: git://anonscm.debian.org/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 9), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 6:9), libavformat-dev (>= 4:0.6), libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6), libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, qtbase5-dev (>= 5.1), zlib1g-dev, zsh
Package-List:
 libvlc-dev deb libdevel optional arch=any
 libvlc5 deb libs optional arch=any
 libvlccore-dev deb libdevel optional arch=any
 libvlccore8 deb libs optional arch=any
 vlc deb video optional arch=any
 vlc-data deb video optional arch=all
 vlc-dbg deb debug extra arch=any
 vlc-nox deb video optional arch=any
 vlc-plugin-fluidsynth deb video optional arch=any
 vlc-plugin-jack deb video optional arch=any
 vlc-plugin-notify deb video optional arch=any
 vlc-plugin-pulse deb video optional arch=all
 vlc-plugin-samba deb video optional arch=linux-any,kfreebsd-any
 vlc-plugin-sdl deb video optional arch=any
 vlc-plugin-svg deb video optional arch=any
 vlc-plugin-zvbi deb video optional arch=any
Checksums-Sha1:
 ee8535c5966217bc2069a74c1d4e8c32609656e9 20940288 vlc_2.2.1.orig.tar.xz
 98b1ef70f44eb546bc9d86ee5b0b8faba3ef34fb 58968 vlc_2.2.1-3.debian.tar.xz
Checksums-Sha256:
 543d9d7e378ec0fa1ee2e7f7f5acf8c456c7d0ecc32037171523197ef3cf1fcb 20940288 vlc_2.2.1.orig.tar.xz
 c184d9a7cee03b047d235bd388739accea8d77cd85da10252e0caa04b7418f43 58968 vlc_2.2.1-3.debian.tar.xz
Files:
 42273945758b521c408fabc7fd6d9946 20940288 vlc_2.2.1.orig.tar.xz
 d611ae7ac3ba135c847881f025f3866d 58968 vlc_2.2.1-3.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCAAGBQJV1sdIAAoJEGny/FFupxmTFqYQAMLgXMaZ+t8Sz310UESMuNH8
o6RourawjUklocGVvf/Cu9ucyhecDg6QDP1Kff8aZi9WhSRUIHE3PIjGV0bZF3Be
G/9mTWIFqiXqBw/yDLefpzhvJ74jJ63d9MqLMh/1C++Y8Vbk62loklOE9gP+zyj5
N7rnOdhHeibcvkiXw5VjDfy2lNgAOO0NwsWZVSkX4lGnOThwg6PFebWTaV9d09Ey
lH4m2bvUyu0xPjPf0v9kiPirbbqSI8P9s4gV2/O/dRmaXRL/O9BcNKgnguRsQL7F
QcLY9xx9nw1aS9s7DSZXOoXOqM4t1psRLtrdyBD9Cb3nTLC95xAtZmxwYW3p0flL
4quM+xyzK5gLT6ndZSV0elyLIOwxruuBHPZJE3P+p/KWo0V+wmEz3/iCXoqyLnXs
E8J37TwnQYI8XEC3pzafLba3A8SPOUU4py0RQl3ih/bEwmv5Ey3FKVCYsD69JH4u
uC0zCuypDih3wFmyGedRmjWLHL8WYtBU6YLRPv79AzaNL3mPisQH7W636Ije/r17
QmpDf6P0ZcVOHbqzUcrGDK8S08dTi1aG7eYQzsmHyA0SzSjuPV46ZnergcjZx19u
6QlYO5gMrWMCS96Eqq487N9gGeS94BmZ09BlmyCkl6qfSowYFOUZ3Cq1PRqrfWQO
G4af+UlMH11zqrsbBr3K
=+IXz
-----END PGP SIGNATURE-----
