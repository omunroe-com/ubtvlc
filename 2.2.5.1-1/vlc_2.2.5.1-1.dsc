-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba
Architecture: any all
Version: 2.2.5.1-1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: https://www.videolan.org/vlc/
Standards-Version: 3.9.8
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, debhelper (>= 10), gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblirc-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopenmpt-modplug-dev | libmodplug-dev (>= 1:0.8.8.1), libopus-dev (>= 1.0.3), libpng-dev, libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, python3, qtbase5-dev (>= 5.1), zlib1g-dev, zsh, libbz2-dev, libgme-dev, libgsm1-dev, liblzma-dev, libmp3lame-dev, librtmp-dev, libsnappy-dev, libsoxr-dev, libssh-gcrypt-dev, libvpx-dev, libwavpack-dev, libwebp-dev, libxvidcore-dev, yasm
Build-Conflicts: libqt5x11extras5-dev (>= 5.8), qtbase5-dev (>= 5.8)
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
 c008b4ee94fc29527fa270ef957a7a5f400910c5 7214980 vlc_2.2.5.1.orig-ffmpeg-2-8-11.tar.xz
 042962dba68e1414aa563883b0172ee121cf9555 21946020 vlc_2.2.5.1.orig.tar.xz
 9fca78fe34ca9f7304c602856e34368c434f33c3 77008 vlc_2.2.5.1-1.debian.tar.xz
Checksums-Sha256:
 38aa3aec93c480b34d33f3648aff9278581d151488b521995c084258613e1fe6 7214980 vlc_2.2.5.1.orig-ffmpeg-2-8-11.tar.xz
 b28b8a28f578c0c6cb1ebed293aca2a3cd368906cf777d1ab599e2784ddda1cc 21946020 vlc_2.2.5.1.orig.tar.xz
 274d169f060a2a30eb52aadfd849620dffb1da5e22b903dcea1c411c9544b25a 77008 vlc_2.2.5.1-1.debian.tar.xz
Files:
 b8c6953c65d0c9ef120a878064282c75 7214980 vlc_2.2.5.1.orig-ffmpeg-2-8-11.tar.xz
 7ab63964ffec4c92a54deb018f23318b 21946020 vlc_2.2.5.1.orig.tar.xz
 82706564a9cdbce9812f08f05d674747 77008 vlc_2.2.5.1-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEE94y6B4F7sUmhHTOQafL8UW6nGZMFAlkXB5cACgkQafL8UW6n
GZNaBg/+PVQOGRgLW/OeKD/RvU6RdX0Cj5sIJ7s3A8w88RhqrfjcAsNmSDanGs2C
oiSk1YzrYoFhSB2YNY9lrEiJSOFrT2m6Lki+KkK0ouXDvSf1zdwIot4qy3IoxY6m
PyUjWDBh7knl/TBj7dU2HFe+QkshwG7wYvdrru0cnYwWLmw2ggN3iKnDb2l8iXtB
vt+NI3hf9nqoQOdvp8XpXQHdhf+iMwLc2AOI0FxzYIEQ0ZCZCLuJNSOsWHg8MiqR
E3OgxtrVgPWv7JTx48870FhxE6Sos5qQGO1rLYBuMe9RHaQq6dlMGy6a45tYOHZm
9ilbRWX7nSnsCNTec6VB9JjtM3i5U1Vh6+W+OdR/99Njl1Ptmika6oOjbzDdR+p1
DI9XgJruXYDgmFlkoYUJtKJVsROKFMEpGCofs+XqhJItGx6IUnk3MvhjOOXKch/7
QMTfFHrbsY1BmJltECBa80mMyzixq9r9XPro3yKVCQbhhs2VHW/UMFSIa8GdcSYT
MGXYy2tyGjvRmGjQWl4M1ct5YAEjoBiNuVz6n15ZmIMXyWTZDhvKr6KL39pOZCHJ
1eY6UDRgc32vt04a4D43ZYEJrCySI67NmVT+DnZoX6MwGMY8Wm1fUUewcEXX8liY
FVn45H+FcXPo1J8GiV7mp2tA7rU8pmYih6kr5k4EFtgRaUNfsEw=
=DLMf
-----END PGP SIGNATURE-----