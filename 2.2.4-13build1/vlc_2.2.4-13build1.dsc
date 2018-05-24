-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-nox
Architecture: any all
Version: 2.2.4-13build1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.8
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, debhelper (>= 10), gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblirc-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopenmpt-modplug-dev | libmodplug-dev (>= 1:0.8.8.1), libopus-dev (>= 1.0.3), libpng-dev, libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, python3, qtbase5-dev (>= 5.1), zlib1g-dev, zsh, libbz2-dev, libgme-dev, libgsm1-dev, liblzma-dev, libmp3lame-dev, librtmp-dev, libsnappy-dev, libsoxr-dev, libssh-gcrypt-dev, libvpx-dev, libwavpack-dev, libwebp-dev, libxvidcore-dev, yasm
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
 d723fdc09a557eb808aea7e89514375ba9cb1a13 7212032 vlc_2.2.4.orig-ffmpeg-2-8-10.tar.xz
 ec171b0ad731d9d114540cd7b7fcb41fc3293696 22199316 vlc_2.2.4.orig.tar.xz
 51c09e73460fcb48df80c64751b6873a0c38ca54 78784 vlc_2.2.4-13build1.debian.tar.xz
Checksums-Sha256:
 2c48930205305a865ae698110f7b3e0cc90e9d243f48bb2a3fb59770d82448b1 7212032 vlc_2.2.4.orig-ffmpeg-2-8-10.tar.xz
 1632e91d2a0087e0ef4c3fb4c95c3c2890f7715a9d1d43ffd46329f428cf53be 22199316 vlc_2.2.4.orig.tar.xz
 f971957bc600767f289818fce0bd4e300c2b8c3129115ae75a5976eac97f9446 78784 vlc_2.2.4-13build1.debian.tar.xz
Files:
 9289aec26808c8ea242ed8c29b2b72d7 7212032 vlc_2.2.4.orig-ffmpeg-2-8-10.tar.xz
 55666c9898f658c7fcca12725bf7dd1b 22199316 vlc_2.2.4.orig.tar.xz
 a46efc89e69775eb16c4ca34753ecd19 78784 vlc_2.2.4-13build1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEi3hoeGwz5cZMTQpICBa54Yx2K60FAliRBjAACgkQCBa54Yx2
K61Z0xAAlGhCB9rlU65Lwwfa1GRxtPQMNoD6ebX/++sym22pykj1k9A4EWFFeDkh
5epS0adJ59ccuXHQGpnnFn8rLtYoI5Yfwj0NPEdV0Kx7YdSpB1KIvFMEUyH369bL
Mu0aX3/yftWG2UOYuxgsbxY5/kjxRz7Q3lQkoSAQ54s7DACqWUS5TB7qC/7VrEju
pU4RdKV6s+Glq1tJBuAGWFooUEIr7XR32Or6MhYoQuftlELixJxeUgfLyT+fWmCX
lcjHq7I7B5wC6vXa584yOGd17rRoJ3KE4f2iNP88ndrUO4vuSLFj3W4Hwq2c9nUM
qYNAVCRL0U3dT+B/Gjxb/B2LMmW5UQQhjv5q9+kwV+DnglnjpnX3IMEa8WuQC43A
fLToYHHqfqVmt2tIBvPCJtHJ5F0bF63NezlLiF8caY26YdiLpnz6sUpkQdtk/hVI
+qjR8yPPEzAfP4W+OLXwuZhJlrPDYo4yByEusM4nkjuJEXYkDMbC627uDQfZILqz
FAXEPj0IlOSd7ASW1GNzmTCYTng+odN65bSzRYlcb42VuTTh6XiLaAJ+KeVF0t8c
bmcZExzYxjNPbD/lNwstk79Wfa006B6zIgIEBwZmNKkWvorFtS+V/1TEtD0plNo0
RespcLRkWszzGuAAqJTUuisSEJuq1U7f+iiKmnfidMuYHlcSkvg=
=1wxo
-----END PGP SIGNATURE-----
