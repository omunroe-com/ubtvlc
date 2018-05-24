-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0-0ubuntu1
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.6
Vcs-Browser: http://anonscm.debian.org/gitweb/?p=pkg-multimedia/vlc.git
Vcs-Git: git://anonscm.debian.org/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 9), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 6:9) | libavcodec-ffmpeg-dev, libavformat-dev (>= 4:0.6) | libavformat-ffmpeg-dev, libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 1.0), libqt4-dev (>= 4:4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6) | libswscale-ffmpeg-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, zlib1g-dev
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
 11f0384980052c7ed73753af2073965f59b27cd2 20861760 vlc_2.2.0.orig.tar.xz
 7b2240ecadda37d6d0ed451ea45acf3a3088f947 58620 vlc_2.2.0-0ubuntu1.debian.tar.xz
Checksums-Sha256:
 5e0a27e248c47da723a696420aca3a4dd15cc3ed8cc81618d3f1dc8661b25517 20861760 vlc_2.2.0.orig.tar.xz
 86a1b1424f725c5553dce36764467fd52e0a0a88993e4627a7889676f33b9e73 58620 vlc_2.2.0-0ubuntu1.debian.tar.xz
Files:
 faeceb614bf3946e5f90ef0d1f51db45 20861760 vlc_2.2.0.orig.tar.xz
 e8acd62ac7e7cb3b8fcdd6c1f8690cfb 58620 vlc_2.2.0-0ubuntu1.debian.tar.xz
Original-Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCgAGBQJVE/2JAAoJEGVp2FWnRL6T/goP/RqLbxUKVTXLPosn/06a4qid
X9pe4MjlXzDM7veKSl5Am5sn7ZhuEFrMfGdwzhkX/g2j9uFbS4ESujod5pgOJ8wm
0hO8Rl659YWep9QXIdVctrsVoEBOnsGZo1RnuUUtq7SF3/mYoJx+dR2hnd61KNEU
iEn0z+lnZNGPCjyWIyJ9LHbHHM3crMgxYA/4z+gXY/vDdRTLLyX9vsSpc+OKHgxa
VhuCe2y4tG18IQ0vxkegcYqu6O4N9BWk3Z8Fbzwe6d0ONj++6y5Xzud4U1ji2QCx
GVf3YZTCk7mVynU9T4/813ViugphcGYoYkRyuell3Ui6JdGq8wRmXaM9CPe0mIcE
yTmqisBB5xlXhgG3Hk54W/testtawQqQvwUB5DnCNyVqFlO0bL0SAJ9/AZZmWI3S
sUu9YvkuZcTSHXAbVNBvRl4a9hV9mXUSfr7h20TjCdmU7ZXTpI84mj0pNBcigvo8
gAAp0IRH67oufD0xk+3wNo1LW+y/IRVf4WJkhFsnGgx5k8hYuOnRxccXkhkrB6ym
FzGtx9MCRqgWb4xP4Suc1RMToIaDLeXXMc34D/4WITS1SYiJMwmmBB2QXBGcP5bw
CxNZ9U5ZkYNFrmc8my+nw5M9m6iSfiQO9lwY5phAx54c/9QJg2InCYHSk5NzrfGN
tcJs/AtS4Stltj77s+0m
=tG+v
-----END PGP SIGNATURE-----
