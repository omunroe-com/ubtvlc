-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0-0ubuntu0.14.10.1
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
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
 11f0384980052c7ed73753af2073965f59b27cd2 20861760 vlc_2.2.0.orig.tar.xz
 462531a22ff7df6bab0aaee714f37f1c9feb4944 58792 vlc_2.2.0-0ubuntu0.14.10.1.debian.tar.xz
Checksums-Sha256:
 5e0a27e248c47da723a696420aca3a4dd15cc3ed8cc81618d3f1dc8661b25517 20861760 vlc_2.2.0.orig.tar.xz
 204bd6248c464e9d5a96a44f7c09684d45e71653ce1a4e05f1e9083711f54a94 58792 vlc_2.2.0-0ubuntu0.14.10.1.debian.tar.xz
Files:
 faeceb614bf3946e5f90ef0d1f51db45 20861760 vlc_2.2.0.orig.tar.xz
 b2404c297074d143f021eac838880e9f 58792 vlc_2.2.0-0ubuntu0.14.10.1.debian.tar.xz
Original-Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCgAGBQJVFAuOAAoJEGVp2FWnRL6TAA8P/3TOFIyt6wHmyDmJcvnvOFI8
T1m4OT8UE2aWcmpGG4vkPvHq3BO3Bpit9kvecjMeuxvNLkHPd54/k3YNn9P8NOab
OHMGRuo8DUih046VeOQx//a2HO4P3Ig3/yMY09/CFXTdd9sPm08SEOLKDyv4KkU3
Ck1ID2NU1bkF3QJjrlQNvMXt3Y060fJHlb99vzMbN/BxWabpHag6wHW0hbF5zpPb
5jAIzZGiNt39z+FL5y+WWs5G7zc6ihvXneDhURADt9iEfhMHDmbsPNHUDt4IX04P
UM2mz3UOH8oW+EtUH8LgtUE2QzrqSABqcUeuOYynRHZgXhamOnUsbB9jBwtz4d33
IjskdH+4k5kdKccHW8fCEScWjOTpLbFvsHDmoUjMxm/MlgaMmNzpEpK4MbCV/y1N
k03YMK7lHAB+vffg1vlABgIwWJcgZw2p3OTTbK1i1XClbN/ORTxIDfNdfdo6yKqn
xQDTe4G/M/un3cOFK1rB5eHHI/rzwY5cmiM8eSAqTlWgEpOS/7yyFxWj999dQN4k
tYBU6zLrRZxLT+xF39lvsfM/6WNgn19MrbPfPxre0N4X3Gxhf0osXxHdVfvcQazl
vZq0m86RLK6FZ7+0VJdJLsjqF7N0bihAiyFqp7D/FmSTY8rpSZBMVi5i9hzoFwsE
A+CAf1IPT+/HtT8BRHIV
=2JJX
-----END PGP SIGNATURE-----
