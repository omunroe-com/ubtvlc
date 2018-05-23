-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore7, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-pulse, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi
Architecture: any all
Version: 2.1.4-1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.5
Vcs-Browser: http://anonscm.debian.org/gitweb/?p=pkg-multimedia/vlc.git
Vcs-Git: git://anonscm.debian.org/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 9), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 4:0.6), libavformat-dev (>= 4:0.6), libbluray-dev (>= 1:0.2.1), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirac-dev, libdirectfb-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev, libdvdread-dev (>= 0.9.5), libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev, libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 0.9.11), libqt4-dev (>= 4:4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6), libtag1-dev (>= 1.6.1), libtar-dev, libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev, pkg-config, zlib1g-dev
Package-List: 
 libvlc-dev deb libdevel optional arch=any
 libvlc5 deb libs optional arch=any
 libvlccore-dev deb libdevel optional arch=any
 libvlccore7 deb libs optional arch=any
 vlc deb video optional arch=any
 vlc-data deb video optional arch=all
 vlc-dbg deb debug extra arch=any
 vlc-nox deb video optional arch=any
 vlc-plugin-fluidsynth deb video optional arch=any
 vlc-plugin-jack deb video optional arch=any
 vlc-plugin-notify deb video optional arch=any
 vlc-plugin-pulse deb video optional arch=any
 vlc-plugin-sdl deb video optional arch=any
 vlc-plugin-svg deb video optional arch=any
 vlc-plugin-zvbi deb video optional arch=any
Checksums-Sha1: 
 53896c88947803193a21fb4619f34034bad05920 19560884 vlc_2.1.4.orig.tar.xz
 6819b9eb6823e21581a8d68d4a02e9241f818838 56348 vlc_2.1.4-1.debian.tar.xz
Checksums-Sha256: 
 3e566c7525478167e18cc53dc75d621e4af91eb40aabb6231e47db25d682d5d3 19560884 vlc_2.1.4.orig.tar.xz
 b271dab7c52c5cb78f87b14359cb88003883c5571cce54de751079c555a73c54 56348 vlc_2.1.4-1.debian.tar.xz
Files: 
 7ed67d22f7425011078772bfc62ac222 19560884 vlc_2.1.4.orig.tar.xz
 a437f71bbcae20459f0ceeecae9160dd 56348 vlc_2.1.4-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCgAGBQJTbrU8AAoJEBWetcTvyHdMY9QQAJ5GfTHhr9bwxzyIXKxVH527
XC5QgyoDd6AxsePaOx8Ra2FWQHf/rgEOo35g+T9+d3qJJ1L9/ZMvQpHxe8Y8CneQ
qNPAgJm6tFo/9owgl55+ZPRoLdZ0NNiEUp8tRDAm1+R/Q2aYgtWdyBlKcVnb2xrl
yHCJFyvja5eZyrgDRVilwKnYXF5aOYHlkzJZtOM74Vm6op9lrZTlxybeJYhGhFh4
0AIzbIR6Lv+Sd6mW2Pz2sj6jT7ZROEFb44PVFS+1XvI4YLRHs1PQLaRLp/No3m/I
ebd3U5p7OpRSr+Q0Rwpi9mNR1ImvPXnWx9llkQD7UFFxvQAz86giu0RB21JeqBnM
3YfO+yjgAzyDxY9/2zzahMudfqmFwfzbckkr9iq4tPc2gmIu6t9+FAPUkCISk1hM
Nu7C/wrQXpwhWHAFav4FRbALfzu17afQV8WABI97IaFAxdi6Ok9BM/7LNtrH/DAO
+V6UNtkwFIh4yHzcEFeljQAxVx2w1PSaAJPI2c/jztQIkWPLr3CPd/IvrzD++Z/I
dgg0+8SygPUKaxU7Hi1v1MpqxEm4evsdYUfTkSjovhc1eqmofRclNR7fmC90sTmQ
F1y9eaUlnYP4HCpGDdOjoCbliHHLiW0uXqXTHMVSLkDYQbJ3PSAmBUod7KLBfGk7
VXFw15j4oCSykdXeh2lY
=IRMA
-----END PGP SIGNATURE-----
