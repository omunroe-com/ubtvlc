-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore9, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-svg, vlc-plugin-samba, vlc-plugin-zvbi
Architecture: any all
Version: 3.0.1-1ubuntu5
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: https://www.videolan.org/vlc/
Standards-Version: 4.1.3
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
 9eab63eeb525f200220fe7f99ca2a67805058517 25174040 vlc_3.0.1.orig.tar.xz
 9a2f78ac8eba1d623da496ca371fe5beccd9adc4 63552 vlc_3.0.1-1ubuntu5.debian.tar.xz
Checksums-Sha256:
 ce700cdfb49d721b75581ee727e98cc60dae97407ccec0f82124d5e9a1cb1301 25174040 vlc_3.0.1.orig.tar.xz
 2acc7dd600cb636fcef2dc1005e43b75dcbf2a9c60db3a06624d61ae6537b59f 63552 vlc_3.0.1-1ubuntu5.debian.tar.xz
Files:
 32292f9fc6eab0b00432cdd574fc9eae 25174040 vlc_3.0.1.orig.tar.xz
 4fd0bce301268c21a98f25901d056153 63552 vlc_3.0.1-1ubuntu5.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIcBAEBCAAGBQJamX11AAoJEPNPCXROn13ZFggP/0RfrDUGK10u59ynryLGyIiD
F2tpiy+E24/ndS1k8lVzuP3vYiXKU/Cis0/EQXf8g6d1bgwolZ2bQQ4zRdtewhR/
v3dzcS2n74HAnsT6wfhyYkQLG7PIhd/0FrAmUPbysXBcheJxcWUdacX4ItPAjjls
6hgaNDOIxRxo5C1K4mqW1+xgHrr8Ue2G9aDsni7XG3ARxaLm3PToS4UoJ9VxCs/+
di5aaYqwfSsXl63/PIxvcSQZz1fGWaZr3Q7LIuX2aLr8OqEFYkVaKbstMLSe44G7
W1gh2uN/5YVyJP19uW48PgkYUNap1KTR6r01qLoydD+ZUY4NJtWQayQ6fJw4TMVw
4JVv8vFJrUUugtoVB7wvIiGwGj1f9lii6x86GRqsqJ7fXjngPCzlKu08r0TB940n
xwCRHAXk+8vV7SSQ/LGTiOe66J/AoBYYB3wqFTjysSv/xU4/HR4PC3KOmerfOcj5
LxqDuNJT1VlLEFrj6X/p30MZE8CSuihHc/mIdZGUds5stdZ0nvJ/vdneUc2mJnvR
dhCl+MOEzXJcgeaJYdL+Yn9ZVSZ9GZ0jRNLZG8KzgjbfQTZQJGWRAlwYQmr2KcsR
IvppwPYsKtaSmEJh2zUb+xrd9tDeOVhn24X3d6LAzsDwWUkl2xH24wXCPChiMVCr
9RuDSSzdGE84LUHmXrFP
=20Nn
-----END PGP SIGNATURE-----