-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-nox
Architecture: any all
Version: 2.2.4-9
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.8
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, debhelper (>= 10), gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblirc-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, python3, qtbase5-dev (>= 5.1), zlib1g-dev, zsh, libbz2-dev, libgme-dev, libgsm1-dev, liblzma-dev, libmp3lame-dev, librtmp-dev, libsnappy-dev, libsoxr-dev, libssh-gcrypt-dev, libvpx-dev, libwavpack-dev, libwebp-dev, libxvidcore-dev, yasm
Package-List:
 libvlc-bin deb video optional arch=any
 libvlc-dev deb libdevel optional arch=any
 libvlc5 deb libs optional arch=any
 libvlccore-dev deb libdevel optional arch=any
 libvlccore8 deb libs optional arch=any
 vlc deb video optional arch=any
 vlc-bin deb video optional arch=any
 vlc-data deb video optional arch=all
 vlc-l10n deb localization optional arch=all
 vlc-nox deb oldlibs extra arch=any
 vlc-plugin-access-extra deb video optional arch=any
 vlc-plugin-base deb video optional arch=any
 vlc-plugin-fluidsynth deb video optional arch=any
 vlc-plugin-jack deb video optional arch=any
 vlc-plugin-notify deb video optional arch=any
 vlc-plugin-qt deb video optional arch=any
 vlc-plugin-samba deb video optional arch=linux-any,kfreebsd-any
 vlc-plugin-sdl deb video optional arch=any
 vlc-plugin-skins2 deb video optional arch=any
 vlc-plugin-svg deb video optional arch=any
 vlc-plugin-video-output deb video optional arch=any
 vlc-plugin-video-splitter deb video optional arch=any
 vlc-plugin-visualization deb video optional arch=any
 vlc-plugin-zvbi deb video optional arch=any
Checksums-Sha1:
 33d8b3775684f5379fd2ab8fb20a19a428c0147a 7207252 vlc_2.2.4.orig-ffmpeg-2-8-8.tar.xz
 ec171b0ad731d9d114540cd7b7fcb41fc3293696 22199316 vlc_2.2.4.orig.tar.xz
 736cb415c438d29505cd53cfe3d2bbb75ee91fba 78056 vlc_2.2.4-9.debian.tar.xz
Checksums-Sha256:
 f94e2f71aacd1fdb1e28aac8cf36aa457c13b0fc9bf704b467d91489705d2199 7207252 vlc_2.2.4.orig-ffmpeg-2-8-8.tar.xz
 1632e91d2a0087e0ef4c3fb4c95c3c2890f7715a9d1d43ffd46329f428cf53be 22199316 vlc_2.2.4.orig.tar.xz
 7d5b138efc00a445064a2f687eb36a48484f86eff6c576648189d66f7e3ee4e5 78056 vlc_2.2.4-9.debian.tar.xz
Files:
 5fae1ba5a5d37a2d0de750479b7270d4 7207252 vlc_2.2.4.orig-ffmpeg-2-8-8.tar.xz
 55666c9898f658c7fcca12725bf7dd1b 22199316 vlc_2.2.4.orig.tar.xz
 d30356ae8be9bd99f710a2e1c49c4886 78056 vlc_2.2.4-9.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEE94y6B4F7sUmhHTOQafL8UW6nGZMFAlgzgl4ACgkQafL8UW6n
GZM0shAAryN0dsbLrqY18ju4EFeDCi3yiu57UQm/PGFM/X+YdKVHDYuB3pcFKlhh
JfnPFGpsRbeWOJWNIkZz3o1pFqY9wGUHmteAitB7tXYjT4CXcPuNNtFxAhskGXlR
F7Q/kr4PrkdTcunpONccZUH6KXTWLCgS9RTcBNDunzmBUid9GxMO0dPyQ4NaBaAG
LHD0Kkh5D6GoSnmmgDeREYBErSuK8eqXTqhl5HhiC+WK/yAkxdmwq6p6Qoyf9MC4
E172RZOvLafLNsraJwpZ9R8Du6L2SGFeL+1RPOVHpqufFS0me1iOA6PFP5gjg6UJ
kToj6r6v6KFJ5hEZPcgGnKfIXigKMFFFjVOwyaylZ5sSyFDO6rIB76iG0tsHKuOA
jZIhxZoWgzt3RbQsP2881WgJ3KlNHfHZfG8PVYCwfTKYC8NQ8Iaf2Ty4W9eHYLeu
ZYj+HP391LnG/rwgumUeKx6usIJiLZLrypAPHWW7Gfaf1GJynCKDO2YGx1hRdRF+
y3zA5zxMGZz3olBN4aKUkfeGrJecV9Yv7g7G/ty1yexKnyjq/jbo3Env4GdCdY3u
7CtxlL3w2RjVLGmGNZu0JoMLnmdiFGhsnnBLxeaeQNkYk9G5D/Sytr15a5JH0wfd
kdwWsmncVeQBDPxmWFH2ZBpeopxa+tF5hspTHLeICvJxcAYLkN8=
=gX2p
-----END PGP SIGNATURE-----
