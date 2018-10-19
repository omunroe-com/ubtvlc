-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore9, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-svg, vlc-plugin-samba, vlc-plugin-zvbi
Architecture: any all
Version: 3.0.4-1ubuntu0.2
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
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
 218603f05ab5f30de0cc87b0ccceb57bb1efcfa4 24934112 vlc_3.0.4.orig.tar.xz
 498268ed00782d64000763f33e86984ccc42adcc 195 vlc_3.0.4.orig.tar.xz.asc
 e24dd8e4a25589896a200cc94b0f1b634e5963b8 63320 vlc_3.0.4-1ubuntu0.2.debian.tar.xz
Checksums-Sha256:
 01f3db3790714038c01f5e23c709e31ecd6f1c046ac93d19e1dde38b3fc05a9e 24934112 vlc_3.0.4.orig.tar.xz
 4ace6dc3bd2f89c9d8dda16364139daee4d678fa62ecbd550e3ca948099b3ccb 195 vlc_3.0.4.orig.tar.xz.asc
 421071f1d4e79528885c21132560cf26efaf6e66d05ce15e53802939eba3075c 63320 vlc_3.0.4-1ubuntu0.2.debian.tar.xz
Files:
 f5e49a0fb9594ab8debf934a710e92f1 24934112 vlc_3.0.4.orig.tar.xz
 62ab6d8fa4495b876dd38f7b3e1bca6b 195 vlc_3.0.4.orig.tar.xz.asc
 fc330d3aee9d69494bbe9fc950112bc5 63320 vlc_3.0.4-1ubuntu0.2.debian.tar.xz
Original-Maintainer: Debian Multimedia Maintainers <debian-multimedia@lists.debian.org>

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCgAdFiEEXHq+og+GMEWcyMi14n8s+EWML6QFAlu5X4AACgkQ4n8s+EWM
L6Ruxg//cwFTRy1qHGZKdCVesRJMFu7PIWo9vsMmEMNDW8eZ7IbPbFGyDGk1wLle
+a+/XN1+l+Vb3fpNU5iXSuYSBhvk49RABAH7epbeVDHdpsaLI6smKa+BA04JIWIi
whVm3rX3q1JQO4YMcyy1yXaEJ6z5xGLsaKi2FwJ4WaFvYgm5PY9kqVmKnzrmg3xt
IQNBla6vDBcWERLqMtiuH8Gzw4sH7dp6rSBrnCijqJ61INc4wrgwlZsHxgoAkVSH
d3UVQAsfGQGAVHzTMVGicy6QadRgRXWM0Qtj7GOhiDL0zGUDpJcRyAiSg0ZLavVS
5lGYgJnml+CmFs/dz85aMzdzhsLz9y+ngRtmdqxY0WhP0ycj3//NZXSjtVmtJmz6
NYbfsNTIU1DIChUDi1smPA357vfbNSTqvZnq2z9iP9lNZB1qR3LldXi9MP3CTSfo
URVslLpfXbZyf/njUwmDcp3uCveHWq+N8tFrGRBgkiuYRrSB2NQ+qus0hSmhCrsv
HFWLqumbs4rkqWtX1RMw0riBCntOtI9l4MQ1t/0AUP4ndLYwWJe8SZaAUAK941u6
AbrUm5vG6MAUsfRfe8/5xbiwUcLed8tolfc+/U7Qtl2NvWDP4ZfQhUrjrHWA48UD
4Q3NRORNZcyvIyu/l4B2RJbI2wT/v8QEOgY0MYWLbKWWFm0D17Y=
=rG+b
-----END PGP SIGNATURE-----
