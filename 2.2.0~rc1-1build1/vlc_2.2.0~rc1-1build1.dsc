-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0~rc1-1build1
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
 c9d746a0769c6c2d8ae2c539694e3e32daa82bcb 20566044 vlc_2.2.0~rc1.orig.tar.xz
 7a5c5a22166907194f8c021147b499abf919f39d 57660 vlc_2.2.0~rc1-1build1.debian.tar.xz
Checksums-Sha256:
 3e53d336002ae7814d52c06b1f411eebf15321e1817d43035fbbf374b376e856 20566044 vlc_2.2.0~rc1.orig.tar.xz
 7dd98cfcd9c66df1c9256f7b46b44ec9428a72c9eb841026e4fb864554c92cbd 57660 vlc_2.2.0~rc1-1build1.debian.tar.xz
Files:
 0aac1f5f1f6e659770db225768f31f29 20566044 vlc_2.2.0~rc1.orig.tar.xz
 0edbe07aab61641c695500372394c38e 57660 vlc_2.2.0~rc1-1build1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCgAGBQJUY062AAoJEONS1cUcUEHU5FoQAIfFo1RMCjAdCe33E0AKhd3D
fsLaur6vD3088nYUQMRp1JglfNZSnr7M0Xy0Arw8wVDvbRPEkgIB/JzoufDxHc5W
xY56yH+0L6+6sgaYfa1ptzzqUB3/TZzXLISGpgtAZxestYnh0BPXl9OCg5e++frm
CpFiAX6pweRv9dhqO7eisJrjKq+JeQ2Qd7i6eAn6mFFe0yBlzkVU2J1nIGvzD3Gm
yhQwk+53mj0rYAl9SQAvy2E+XX4uAw1EcklFBTMOHDUrDsx/6rm4POGvoUz+Uey/
sQp1hpwGddlvofVjn+z9k2m5AUrBBxWQ1pLBOVlcVh1rvt/R84Z1kVLKN5AkTGIq
N/+d5TZhnNQqps6HHva/NRlUvva/oG+r35eiA3mLYqjVKSPi31MIsEKTmbp6ql9W
fIqtHqef/B5zdb847V9lL6aqN3CKVzH28fu0vA7mFGSEj4XkM1ZAPznH4Bis0gcr
pa4hJIiHLEHlnmKtx7eViV+HdA9C06BKW3u1iXAm0NguOkkao3B+1ULZJCtwkcQA
oULtuAK3yxqhGjQUkFVS5L9t0Oee9PM8t5apvNgsatKAGY8eN6hgUS4vRsPhg6Ae
lJ+LAZCp0RT9H/xiyz200RSXiZHA3Pnz2sG5TC4xWwfzuE3zcU+Y+kc7jFpsmJ0T
D467EJRxNWx6jSsuP3X+
=IWbb
-----END PGP SIGNATURE-----
