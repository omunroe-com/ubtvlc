-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba
Architecture: any all
Version: 2.2.6-4build1
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: https://www.videolan.org/vlc/
Standards-Version: 4.0.1
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, debhelper (>= 10), gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblirc-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopenmpt-modplug-dev | libmodplug-dev (>= 1:0.8.8.1), libopus-dev (>= 1.0.3), libpng-dev, libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, python3, qtbase5-dev (>= 5.1), zlib1g-dev, zsh, libbz2-dev, libgme-dev, libgsm1-dev, liblzma-dev, libmp3lame-dev, librtmp-dev, libsnappy-dev, libsoxr-dev, libssh-gcrypt-dev, libvpx-dev, libwavpack-dev, libwebp-dev, libxvidcore-dev, yasm
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
 aa64c9db721d4b0486acbb05f00ae63f4cd25428 7218032 vlc_2.2.6.orig-ffmpeg-2-8-12.tar.xz
 d299dce6a5c94af4292657b6cb99c44654024f70 22198720 vlc_2.2.6.orig.tar.xz
 325a497382ff5a4d0d574dd89c275f30039ceff1 79580 vlc_2.2.6-4build1.debian.tar.xz
Checksums-Sha256:
 6213f18a8bb93415724c40a1840a4591458c2949575978b9cf43d7b6a71283bd 7218032 vlc_2.2.6.orig-ffmpeg-2-8-12.tar.xz
 c403d3accd9a400eb2181c958f3e7bc5524fe5738425f4253d42883b425a42a8 22198720 vlc_2.2.6.orig.tar.xz
 131e488311e0ed3eabca425607774ce830a6129966d874983782ccb834f332f7 79580 vlc_2.2.6-4build1.debian.tar.xz
Files:
 4c1e962b5646e1c5964124d72ec7c582 7218032 vlc_2.2.6.orig-ffmpeg-2-8-12.tar.xz
 031d1bbef7737d44d18d78d6761ed26e 22198720 vlc_2.2.6.orig.tar.xz
 21618ac9432e7a87b5f9e1f9a48fac66 79580 vlc_2.2.6-4build1.debian.tar.xz
Original-Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----

iQI3BAEBCgAhBQJZn8YdGhxzdGV2ZS5sYW5nYXNla0B1YnVudHUuY29tAAoJEFaN
MPMhshM9sVIQAI4bjy7fqypsDw+ADsle7UqUClUSQYsNHgi0+QSnwgDBDE2/B1Ze
73UaNzHBOqIxALA7m+LV4zUTsAQ94/lIb124nzyNN/EdO+NWH7lqMevD2UrEOSrQ
bEwqMKtue1mguSvRTfKRqp3KFy/8EFHFfy7/BZPru15siTrxg5vhOTbBql8JeNqY
GHaJ2oFoEgQb4tbuq4PTnUPxvVD45R44omskdfYeBJ+8G9V4Y94Nqj5tE429f1sS
Bj++nElSRouvu79ldrrueRiHY7SW4WV6PxTpkIf1a4nD/4WQaUD9bxJHCjVMQTc4
ydNd8aDN/pRXJ9K8rLBi5AuUCItxSyzAGgNVc8QUiu8Dk4UK1mJRwflNmZz0QI3w
DzpYq2xQbM6FbhJfQ2P7MBBegeefbnL7BjqG7c6semjXWmDxIsVKn0PvoFZLST32
OaT54toLHZYZIZ8U9T24RfgPI8U2AbycYyfpdGljolYL4nDqwLhWKWoA3OjyLW9q
KWE9hsFzUrwqcjS013+6MP8OBmbY4t1bEdRtG9QzbT/KqHQfWnEGffphpLx4zhby
9JeR8W1pKkbR4aElL5j7h+TEiOvlNuhc47LRvItViQS1RxdM++N7rgdK808F4MPg
LQrLYmjvuKmeNP3LlzqlK9XyRCVeWPvrU+v0VyG1Xe/45Y2W4zZ9KynE
=F0f/
-----END PGP SIGNATURE-----