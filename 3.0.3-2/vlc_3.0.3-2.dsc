-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore9, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-svg, vlc-plugin-samba, vlc-plugin-zvbi
Architecture: any all
Version: 3.0.3-2
Maintainer: Debian Multimedia Maintainers <debian-multimedia@lists.debian.org>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: https://www.videolan.org/vlc/
Standards-Version: 4.1.4
Vcs-Browser: https://salsa.debian.org/multimedia-team/vlc
Vcs-Git: https://salsa.debian.org/multimedia-team/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, bison, debhelper (>= 11), gettext, flex, liba52-0.7.4-dev, libaa1-dev, libarchive-dev (>= 3.1.0), libaribb24-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 7:3.0), libavformat-dev (>= 7:3.0), libavresample-dev (>= 7:3.0), libbluray-dev (>= 1:0.6.2), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdvbpsi-dev (>= 1.2.0), libdvdnav-dev (>= 4.9.0), libdvdread-dev (>= 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.3.6), libgtk-3-dev, libharfbuzz-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-jackd2-dev | libjack-dev, libkate-dev (>= 0.3.0), liblirc-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmicrodns-dev, libmpcdec-dev, libmpeg2-4-dev, libmpg123-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnfs-dev (>= 1.10.0) [linux-any], libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopenmpt-modplug-dev | libmodplug-dev (>= 1:0.8.8.1), libopus-dev (>= 1.0.3), libplacebo-dev (>= 0.2.1) [linux-any], libpng-dev, libpostproc-dev (>= 7:3.0), libprotobuf-dev (>= 2.5.0), libpulse-dev (>= 1.0), libqt5svg5-dev, libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libsecret-1-dev, libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libsoxr-dev (>= 0.1.2), libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 7:3.0), libsystemd-dev [linux-any], libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, protobuf-compiler, python3:native, qtbase5-dev (>= 5.6), qtbase5-private-dev (>= 5.6), wayland-protocols [linux-any], zlib1g-dev, zsh
Package-List:
 libvlc-bin deb video optional arch=any
 libvlc-dev deb libdevel optional arch=any
 libvlc5 deb libs optional arch=any
 libvlccore-dev deb libdevel optional arch=any
 libvlccore9 deb libs optional arch=any
 vlc deb video optional arch=any
 vlc-bin deb video optional arch=any
 vlc-data deb video optional arch=all
 vlc-l10n deb localization optional arch=all
 vlc-plugin-access-extra deb video optional arch=any
 vlc-plugin-base deb video optional arch=any
 vlc-plugin-fluidsynth deb video optional arch=any
 vlc-plugin-jack deb video optional arch=any
 vlc-plugin-notify deb video optional arch=any
 vlc-plugin-qt deb video optional arch=any
 vlc-plugin-samba deb video optional arch=kfreebsd-any,linux-any
 vlc-plugin-skins2 deb video optional arch=any
 vlc-plugin-svg deb video optional arch=any
 vlc-plugin-video-output deb video optional arch=any
 vlc-plugin-video-splitter deb video optional arch=any
 vlc-plugin-visualization deb video optional arch=any
 vlc-plugin-zvbi deb oldlibs optional arch=any
Checksums-Sha1:
 dd1b7350bb0a7848e172e9dbbe0705ec5bb3d48c 24941592 vlc_3.0.3.orig.tar.xz
 5cbd1ef59127da4f6d2afdd382452a0f3b55f391 195 vlc_3.0.3.orig.tar.xz.asc
 30267a51cd468699060f698fa348a3d22448b407 63144 vlc_3.0.3-2.debian.tar.xz
Checksums-Sha256:
 9ba8b04bdb13f7860a2041768ac83b47b397a36549c71c530b94028a3cfd5b51 24941592 vlc_3.0.3.orig.tar.xz
 21f61a6aa792de35f77b7e9cc727f4b0e99f497532834f3e001e1fe2bcccf003 195 vlc_3.0.3.orig.tar.xz.asc
 081acf3d06bce5f7a8122a8767e32b5bd2510b1b9785b95a319cae866b8c33ba 63144 vlc_3.0.3-2.debian.tar.xz
Files:
 cf01d4755c719fe1c8605ceb40d8f1d2 24941592 vlc_3.0.3.orig.tar.xz
 5e5e452f09309ef7e6f874f0b94d6adc 195 vlc_3.0.3.orig.tar.xz.asc
 874f31c21617cf9bae9355cadeb794c6 63144 vlc_3.0.3-2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEE94y6B4F7sUmhHTOQafL8UW6nGZMFAls3OxUACgkQafL8UW6n
GZOHAw//akfUpdvL8scgMk3+HwxL1gJTVFt5b1ysVJRgQtWGW8/IZ7Wx+ywREV4s
3zacWMmKjS4/Ke24+HPnklTd+XmvV/4d+9T3AWMzWvLsIB2hYds0/1t/uFkZncgS
WzpGaoxf+4lly3x4bTCoTBYYvUh4Iy1ua3etVGWpW5SCWndIewMQ5uZal5wvgRko
YUONnvA/a2+r0E4xwfLD4ffeIOfvIdLzEGIOgvAaChDvXjf7xYiDwSJ95+9I9787
icTQBst/fGHBqvWd9inVyhKI/t/cSgtqk0iifdDjZqkaUlKoz7MtIz5FMqNjm8Xb
cnH/2DdDpIUpsUgR3lwL/sfE59NZYJ0jgqk4xCS8Q28dZPTo6bVC4nCQZd6BX/+T
nKj/b+H0Y7Ol7Cb1ZdP1FnZU9hDhlJIJ7+IQsQIZ5OtNerPAA7GiSdaqaNH9r3HU
G3tNkfyGYRDIfV+RxssbK7SHoR0sSRwiK+xTlBbx1OI/lV2OQRjpjA5rM5Gd8Huq
s7i+J57SJUE52EfkSYV9NtpECl1xgTIyWH08Qs5mj4HXvvIZqytGXC2TJq36x1Vn
jE3eC+gAtTD1yB1P5Eo+OiBKVDNc4/bruXb1FYM2fx/izo/UHFH3f23lOWodsTQ4
yy88v1seWi6TrrFuTDMneBt7SsMLAQli/0HgJ0DEry8Dold6DRw=
=O3ID
-----END PGP SIGNATURE-----
