-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.6-1~deb8u1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
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
 d299dce6a5c94af4292657b6cb99c44654024f70 22198720 vlc_2.2.6.orig.tar.xz
 a813708a32816de4040447d201c32b227e011812 58672 vlc_2.2.6-1~deb8u1.debian.tar.xz
Checksums-Sha256:
 c403d3accd9a400eb2181c958f3e7bc5524fe5738425f4253d42883b425a42a8 22198720 vlc_2.2.6.orig.tar.xz
 231a9ad1f8bcd907538fa9a7032a8b076b1f72670f6af15824481a49662c8783 58672 vlc_2.2.6-1~deb8u1.debian.tar.xz
Files:
 031d1bbef7737d44d18d78d6761ed26e 22198720 vlc_2.2.6.orig.tar.xz
 d30fd6d248ca705a6a1cd09c87e22ad1 58672 vlc_2.2.6-1~deb8u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEE94y6B4F7sUmhHTOQafL8UW6nGZMFAllQIfkACgkQafL8UW6n
GZNrZhAAkc/W0lQdm6x2jPcC0h2l8mJ4Mii/fDmXGkMoIe5YjAYBVx4dehvBkLOO
sl1N5xil+gedQ8a1EDfe01NBZ7AUXFl8tzTEf4sUN5qyaZ28wPziV3UWL1dni6Z0
RCPZYP4HLbLQPjkNIvOMgWaR1b3Ruk772yKtD80H1tTr3qnF7GFe6n/6U4UmBnf3
iAoMGj0VcMm5Wn+OKbEISxfmReGs2xHRHFkdPa8ZhrbL6NusldyXfiVy91cC5izu
5D6sByQpmSA8kLqf1YtdRkVBadHwkDVQMvpHjvmHEVSO274qpn1LV7j0/Vm8Xz8C
GFhF7Oa+lBf+Smd8A35sBxdAe3duOOkL+GwUBpWF/aekH/C3umyMhGOiGU/cdlY4
8TS+LjJofQ2OsHbXdUQHqnUd792M/+uRfna/xz563OOmeO01tVXkZ2ayPqSLboNg
fmQgLEsgr3Y/ZWC/6/Lkzzew7y/5qnZ8QGpmdH/GayEeoF/xYMCGgKfiG1pEZ3pt
0ivtS0SEFP5k1FbSos6dYh7mgo5G7yS45ya/zbPj8C4GuD5rtHimxYw/ve8rev5U
ZmqDuyZMuqnRO3FV/PDGCPDwDK2YtkusQQMZL7oKALBy4CvHiFP/0WwSQwZbCNGd
UsXdcivNIlAmOljuz2D9b7TPtULQhBuGhwv6z4an2Lmxkia0qiM=
=Qpsj
-----END PGP SIGNATURE-----
