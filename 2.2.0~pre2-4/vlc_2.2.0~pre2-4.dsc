-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0~pre2-4
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.5
Vcs-Browser: http://anonscm.debian.org/gitweb/?p=pkg-multimedia/vlc.git
Vcs-Git: git://anonscm.debian.org/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 9), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 6:9) | libavcodec-ffmpeg-dev, libavformat-dev (>= 4:0.6) | libavformat-ffmpeg-dev, libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirac-dev, libdirectfb-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 1.0), libqt4-dev (>= 4:4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6) | libswscale-ffmpeg-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libvpx-dev, libx11-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev, pkg-config, zlib1g-dev
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
 vlc-plugin-samba deb video optional arch=any
 vlc-plugin-sdl deb video optional arch=any
 vlc-plugin-svg deb video optional arch=any
 vlc-plugin-zvbi deb video optional arch=any
Checksums-Sha1:
 83ebd626919f24085340aaf45ac192cd51ba0b84 20374604 vlc_2.2.0~pre2.orig.tar.xz
 5e9857100d7f0790a0fceb05ce5152fec2a697db 57908 vlc_2.2.0~pre2-4.debian.tar.xz
Checksums-Sha256:
 2370dd24aab71aeb4010aabd3fdfdf5da8763bb24948b590c088a845a7f4b067 20374604 vlc_2.2.0~pre2.orig.tar.xz
 48de6c6221dcba68101f9d8a21bc97dd951c3542e2d518fefd357fecf24a5a02 57908 vlc_2.2.0~pre2-4.debian.tar.xz
Files:
 6a4bda79c7af131c2e94e19bc6548843 20374604 vlc_2.2.0~pre2.orig.tar.xz
 b2d0418b0db8db1091500571a01957b7 57908 vlc_2.2.0~pre2-4.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCAAGBQJT+6W8AAoJEGny/FFupxmTTTUP/Aq71m1TrS0PzQMywYLx6Rn2
BnkoITTpfwsFm0te10eeL5F7SbCHzkG21DpCGNMh8NiJZchJa90h/9hIlVFqqPZE
fyS5dwy3owdrwQiFVTwQwh5IA4+qcle6bHiANZ0UYGicpJDYUOtR0wNVSqYjtAs0
mQ5I38s3OIBe3pJkBirFMFjUan+wQ9WJCrykZfgYsppsI1TWBYH8IdD04HGcenvH
nG/JZqLB3sx2fPG9xYuZmut3+w/Dd14YLZaHKy08g8uC61OBssNf6SRIQ+ujzH4g
d1Pt5jb+DTlOP4Z/0vuZDYW1L6tRkRvBCI+A97V9xDF3WVydvqeVCg62lwzFln8u
bCXjVMDwluia87GHLotZEI6mx8xArYuROxbUs+rCH5VJKgkTmP5z2jrnTBIInRmo
6RNExL48zrmtS50D2vmSwdgJG/Hb8pk6/B8qqTRee3Rg1K33CndnWNqRDW3LnsiU
LNmPk9T03K2PlXog+07ODG0nijWUFVoW01Wlddng5DsfwemIa5I/cUZiDOJNiYS1
0NlhV/lBQ8GJEZi/BQ/Nlktc3GOprMurz2RuZ1O2lRPQCvt+SWLVMsGhCSjhbsP3
uX8xwzRxGu1CbGKxVSA6rnOxS0kCpDr1qjPSg29mTVrvXWE3wevOKHK0xWwDmtvJ
drTD3doHPe564CMKaL0f
=+kCM
-----END PGP SIGNATURE-----
