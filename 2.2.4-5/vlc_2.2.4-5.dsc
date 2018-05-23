-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba
Architecture: any all
Version: 2.2.4-5
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.8
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, debhelper (>= 10), dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, python3, qtbase5-dev (>= 5.1), zlib1g-dev, zsh, libbz2-dev, libgme-dev, libgsm1-dev, liblzma-dev, libmp3lame-dev, librtmp-dev, libsnappy-dev, libsoxr-dev, libssh-gcrypt-dev, libvpx-dev, libwavpack-dev, libwebp-dev, libxvidcore-dev, yasm
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
 33d8b3775684f5379fd2ab8fb20a19a428c0147a 7207252 vlc_2.2.4.orig-ffmpeg-2-8-8.tar.xz
 ec171b0ad731d9d114540cd7b7fcb41fc3293696 22199316 vlc_2.2.4.orig.tar.xz
 8cca80a141960dde0d8f56f6fc4e2626d444b3b8 75936 vlc_2.2.4-5.debian.tar.xz
Checksums-Sha256:
 f94e2f71aacd1fdb1e28aac8cf36aa457c13b0fc9bf704b467d91489705d2199 7207252 vlc_2.2.4.orig-ffmpeg-2-8-8.tar.xz
 1632e91d2a0087e0ef4c3fb4c95c3c2890f7715a9d1d43ffd46329f428cf53be 22199316 vlc_2.2.4.orig.tar.xz
 62ea3adc2684533320a37e29906e8b5790fcebd45730a8d66afc43b82f21f153 75936 vlc_2.2.4-5.debian.tar.xz
Files:
 5fae1ba5a5d37a2d0de750479b7270d4 7207252 vlc_2.2.4.orig-ffmpeg-2-8-8.tar.xz
 55666c9898f658c7fcca12725bf7dd1b 22199316 vlc_2.2.4.orig.tar.xz
 819b2daad995159e07f7c02249cdb4f5 75936 vlc_2.2.4-5.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIcBAEBCAAGBQJX4tPrAAoJEGny/FFupxmT9JAQAJRk3eLLfKI+9aEWj+yl71Sf
Xg/IgRoDfkl4hsLtiEeT9KmLGhGsAhMq/5SkUCNcOQEb81rhSWDx04Pquicaqlsa
Jre/jizaaFCoRhUPDFOGYQqoYA/ZP77T1bHWFTjqm4wX57zeGe5UesAxWh1bLO5E
rovJtUes3/BRy6K+Qwab8Nkd6mBFH3Kmr74CWwyvUh6mhp96sZb7K7/BOBIzStp3
rGhUlIyYN/4r4scfMVXhvZVrfB3Nk54e3zF8/vcgQH1T3JfefHh0Xk7WS5iOszaR
zY3hVBAHo4OUwdB9p7G5yuyqOs9AdZnAl4Jo4Vm0Vk04mmMrRvvzSHbn9/YspehA
elLQ4SDK8sbBFp4nF4lKTsdPNvqYc7gQgxm0OJ8siomqSxvTwEgomAo3+5Qr5QOd
ugCaPDnpCy0rX/JT0ML1lX4Ryk3dvxAiqVpFTFvjHUdioAg4XRMJK9F5M/L77Nx6
6a22fLT/Khl7/7eckxSYo0nOAAG8OiFPTlIv35+AgrkSQfzMRhtJFI3sFpHytRsm
TTLoL+MUYKVYS06XN1vsKIz1aY5G8zGNCfhvhh7fA+rWxfCQLvUqX1BKvUQg4Dz+
g0wyjHT5dC1hDvmVqZfs4fCH2NkGimjZpD6asp1PjClNdVxuuCJ0EHcUOkcDBr3d
yteaAjFKlDcxgDMVnGxx
=B9bE
-----END PGP SIGNATURE-----
