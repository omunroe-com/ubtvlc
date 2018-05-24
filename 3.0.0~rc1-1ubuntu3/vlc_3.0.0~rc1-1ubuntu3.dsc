-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore9, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-svg, vlc-plugin-samba, vlc-plugin-zvbi
Architecture: any all
Version: 3.0.0~rc1-1ubuntu3
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: https://www.videolan.org/vlc/
Standards-Version: 4.1.2
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git -b experimental
Testsuite: autopkgtest
Build-Depends: autopoint, bison, debhelper (>= 10), gettext, flex, liba52-0.7.4-dev, libaa1-dev, libarchive-dev (>= 3.1.0), libaribb24-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 7:3.0), libavformat-dev (>= 7:3.0), libavresample-dev (>= 7:3.0), libbluray-dev (>= 1:0.6.2), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdvbpsi-dev (>= 1.2.0), libdvdnav-dev (>= 4.9.0), libdvdread-dev (>= 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.3.6), libgtk-3-dev, libharfbuzz-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-jackd2-dev | libjack-dev, libkate-dev (>= 0.3.0), liblirc-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmicrodns-dev, libmpcdec-dev, libmpeg2-4-dev, libmpg123-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnfs-dev (>= 1.10.0) [linux-any], libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopenmpt-modplug-dev | libmodplug-dev (>= 1:0.8.8.1), libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 7:3.0), libpulse-dev (>= 1.0), libqt5svg5-dev, libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libsecret-1-dev, libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libsoxr-dev (>= 0.1.2), libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 7:3.0), libsystemd-dev [linux-any], libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, protobuf-compiler, python3, qtbase5-dev (>= 5.6), qtbase5-private-dev (>= 5.6), wayland-protocols [linux-any], zlib1g-dev, zsh
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
 7a17107ce4d20d1b7e01c3860877be3fcb938f7e 24894860 vlc_3.0.0~rc1.orig.tar.xz
 7bda9c1193d3e5bb50cdbe5b8dc8e74fc8dc4815 63904 vlc_3.0.0~rc1-1ubuntu3.debian.tar.xz
Checksums-Sha256:
 e7132a63f06cd91a17f7b7abb70f13df9e0dd6c182c5c79c1fd420120c0fb3bf 24894860 vlc_3.0.0~rc1.orig.tar.xz
 2b0b25502c0d33859d41d9508d33462ab1aa464481d8f10a408d380cef949f3d 63904 vlc_3.0.0~rc1-1ubuntu3.debian.tar.xz
Files:
 81be6c316989571f21f6c7e90156363a 24894860 vlc_3.0.0~rc1.orig.tar.xz
 69f558feb8c914a72c8547969d8b7515 63904 vlc_3.0.0~rc1-1ubuntu3.debian.tar.xz
Original-Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----

iQIyBAEBCAAcBQJaOMiXFRxyaWttaWxsc0BrdWJ1bnR1Lm9yZwAKCRAioEvvkWi6
1dZpD/0S9YszefIYrLkIPZgrrSiIzgYog4pkZe9v6YJZifpwCPkPIpcuOXDEKJsO
JKD1u1CDwXwptLeq/Em0tjpAxmo1OYAtMxqKM3MhI+JqlRkfunqEpSdJsGQKCtCa
HUj+Pcs3DUfREctf6XnvJlohWX6wey2W/DaD9VSw4Adi9QCzTRrSgqmQJKyXB0yR
9nKqwnUurFy3nemywIwBo2uVHwUkAPFs1wqA36ZQfZpJna6uMUdi12J/KpyWvPm3
nLYTqlVXxXENNxFbnMvzxbGcEfzAGoBS0VmGLMT3e0H+b4/PIbBwZAwtHtlGrjVI
8vs/7wT678OHaYfyunVhTAfnIjztkGG8P/i9HkUwR0Khj1iBEHWIYDpqjV9DtlVB
0L+a+cvxhzCf2gt4O+x60T+SP8adzUc35tSOc+fVYA2lLrR0zRp7NXCbz4FOM9Lp
eo505k0JYpQptJgLmRt4+qdlZPZza9SKk904z87ZSQJNWbrRFa0boBK+unU+qI0M
I2rmdcy8tMOwQwEO0q2cN4OIv2QBl8YSEoC9VvmWx45r0e6tzjenudVPZ8uO12LG
ZRtIY6eL1gTbwDR6tEYZSGnaSz8gjIhF/fY/Pi3P+v3eei95tUgllfumLzFttJfC
ticJfQq2TeHUcYpHfJUVWWc4lANLdoiQzMkM0v3XI490yc4LKw==
=f7HP
-----END PGP SIGNATURE-----
