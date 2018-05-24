-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-nox
Architecture: any all
Version: 2.2.4-13ubuntu2
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.8
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, debhelper (>= 10), gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblirc-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopenmpt-modplug-dev | libmodplug-dev (>= 1:0.8.8.1), libopus-dev (>= 1.0.3), libpng-dev, libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, python3, qtbase5-dev (>= 5.1), zlib1g-dev, zsh, libbz2-dev, libgme-dev, libgsm1-dev, liblzma-dev, libmp3lame-dev, librtmp-dev, libsnappy-dev, libsoxr-dev, libssh-gcrypt-dev, libvpx-dev, libwavpack-dev, libwebp-dev, libxvidcore-dev, yasm
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
 8c0fb21703cf77442338a26f2f645eda68bde6b2 7205628 vlc_2.2.4.orig-ffmpeg-2-8-7.tar.xz
 ec171b0ad731d9d114540cd7b7fcb41fc3293696 22199316 vlc_2.2.4.orig.tar.xz
 6d2535363810d99956bf7954ee48d82a1f4f1b4c 78884 vlc_2.2.4-13ubuntu2.debian.tar.xz
Checksums-Sha256:
 2c48930205305a865ae698110f7b3e0cc90e9d243f48bb2a3fb59770d82448b1 7212032 vlc_2.2.4.orig-ffmpeg-2-8-10.tar.xz
 97be8a6c1cab2c52015544f76d40f6fd0babcb1b40cf601b4e57fb818564157c 7205628 vlc_2.2.4.orig-ffmpeg-2-8-7.tar.xz
 1632e91d2a0087e0ef4c3fb4c95c3c2890f7715a9d1d43ffd46329f428cf53be 22199316 vlc_2.2.4.orig.tar.xz
 fe899fff12b194e1611d370b7eb8e1cb84209cac26ec071315b1eb2dd0fa6432 78884 vlc_2.2.4-13ubuntu2.debian.tar.xz
Files:
 9289aec26808c8ea242ed8c29b2b72d7 7212032 vlc_2.2.4.orig-ffmpeg-2-8-10.tar.xz
 7e5b1c98eedfc3a364fa8c4095deeae5 7205628 vlc_2.2.4.orig-ffmpeg-2-8-7.tar.xz
 55666c9898f658c7fcca12725bf7dd1b 22199316 vlc_2.2.4.orig.tar.xz
 7a346a490dd40e27529d59094a66a494 78884 vlc_2.2.4-13ubuntu2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEi3hoeGwz5cZMTQpICBa54Yx2K60FAlipwbwACgkQCBa54Yx2
K63F9w//UqWwB7fbyaYlg/b4OttPkScLaXX4TfB6TCDGh6rjxEMwxIFx8rQ5WPew
nB31A+bgLwXTrL9JEwE20+wk0uEADqAqC2j0bJVzSmmKhiuavrgWxoyIDgTjRkr2
G0ENxpGZopYXDW7UtaK4l1TBzHyna/HMQVgNguMRW/2uYMouEZmyEgjJVBNOdEtK
MDXSZdiQoJjnU0rxsXXQrozWW1jlMhQ2VsKPgLKf0Q5NnDWzvpfT5E49tS2cyQfp
hA5bYNl08Qoy//usMBnHnZpd+7o6YCgVj2Z5477jTv+qERJhzfisQqkhgRg8FLXC
o2pdoKgrt0ZplQ6uy478PCVcsfh9E72RXFJdlQ0Y5PcEQmSQCx2a9HZgWa+Pg2W/
953KvKIpEF6TpgVKK8QQvfhwxZcip/+GNoHEw51KRuFcAdHnEO+qZTwJ8fORiS9j
+xul5xqUSTgMts1H27GZ+FfY66n44U00OzqOSf5H55L56WGdykCKwfy4C+g2X2Hm
3LgdGKPakAMlpA9lsPwJQ+srOF8sLQDttPnxPT+LwcruZ7QIlaZgipZhOUzT+fbE
WWRRclG2l0P39nimY8QZlJQvT53ueIitkrMfs+3AHMM3vZVn5Sz9uE5NhMSvQ8lY
EXdbbevDjZ+/YMrKfRQJQ4O+VX2Aqtx9CR00pBIQbfPtmwpwBsw=
=97/t
-----END PGP SIGNATURE-----
