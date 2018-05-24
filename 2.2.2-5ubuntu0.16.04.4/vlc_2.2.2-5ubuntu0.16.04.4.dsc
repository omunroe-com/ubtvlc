-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba
Architecture: any all
Version: 2.2.2-5ubuntu0.16.04.4
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.7
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, debhelper (>= 9.20151219), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 6:9), libavformat-dev (>= 4:0.6), libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6), libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, qtbase5-dev (>= 5.1), zlib1g-dev, zsh
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
 92372258881b6e53393f3a1bad38bdce48bb5860 66556 vlc_2.2.2-5ubuntu0.16.04.4.debian.tar.xz
Checksums-Sha256:
 7f729245357f337f51f34b91fa896e5b62dd7bd06dc996bab88d9f5623eaedd3 22132140 vlc_2.2.2.orig.tar.xz
 e91ebe6c2b4232cf699b23622e32c983923324c060e648b455ef6ac4671225c9 66556 vlc_2.2.2-5ubuntu0.16.04.4.debian.tar.xz
Files:
 342fe7d6ae9b0ae1580e18c3326cdca2 22132140 vlc_2.2.2.orig.tar.xz
 995060ca6e21ac0a5a156e0a5705f248 66556 vlc_2.2.2-5ubuntu0.16.04.4.debian.tar.xz
Original-Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCgAGBQJZjboUAAoJEGVp2FWnRL6TFKsP/AoLSkv5/CPFi0ELPqBWgYmR
qCZb/cITcF2+lp7E6HjmBsNaXcjWFIWER/1PHI1l5CdM3WhY5EGvSHIxH+wLk8uC
pV4hUZymACtMcoNLbKZO1uidWHgs8aVPDNTIvtlzBPa46QaClsDEzuG+2sDGtt8m
BRmeuovbXz/rKNxCEJ1bZ0b+8WOe9f64JBE06quZS5W1Na30ShTwubnXjTbZ5+n+
m4ekkg/w3GPI7D9GrO6joV1kB70wY/XwHvljgJW4/8dFg73ZzEJ8Ei3zlXgaFggx
6hBJ3Rzrc0vNWWIMjhUK/we6piayeD0YOW8kXn2Qgs5E+PR+00WFmmoYq05lcmbC
DluAq3X4MNib2UJWpd5c7B79YjsXGXrg16LHXvXx4zfvgWTne/rN0SAlD2BUYXjO
a1qMOp5y/zCBPKnoqCbaQhhsnQkMxquqzGGKHqtFaGLnF8A3ozTCz6L9rpdFHra2
T6SlyJbcKGpOjviVUFyr4PqowjyzF1xq+QLMK/bsJZah+SKneAbNmq6lnTbgeWD6
v21D2hO+JpsJIjfVmUhe9WG9cwgyjHc7AckBelYx6k+RGVW1lETs5depIWFQFALv
z8VJBlBmN8mHiXiXGL07+unJazFkVydSr4tGDVskmm5SxriRr59NsxpCRyVP5IMn
4R3JyXUsonfwe/tLp9ft
=VVTi
-----END PGP SIGNATURE-----
