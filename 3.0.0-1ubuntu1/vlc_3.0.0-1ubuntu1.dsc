-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore9, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-svg, vlc-plugin-samba, vlc-plugin-zvbi
Architecture: any all
Version: 3.0.0-1ubuntu1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: https://www.videolan.org/vlc/
Standards-Version: 4.1.3
Vcs-Browser: https://salsa.debian.org/multimedia-team/vlc
Vcs-Git: https://salsa.debian.org/multimedia-team/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, bison, debhelper (>= 11), gettext, flex, liba52-0.7.4-dev, libaa1-dev, libarchive-dev (>= 3.1.0), libaribb24-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 7:3.0), libavformat-dev (>= 7:3.0), libavresample-dev (>= 7:3.0), libbluray-dev (>= 1:0.6.2), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdvbpsi-dev (>= 1.2.0), libdvdnav-dev (>= 4.9.0), libdvdread-dev (>= 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.3.6), libgtk-3-dev, libharfbuzz-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-jackd2-dev | libjack-dev, libkate-dev (>= 0.3.0), liblirc-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmicrodns-dev, libmpcdec-dev, libmpeg2-4-dev, libmpg123-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnfs-dev (>= 1.10.0) [linux-any], libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopenmpt-modplug-dev | libmodplug-dev (>= 1:0.8.8.1), libopus-dev (>= 1.0.3), libplacebo-dev (>= 0.2) [linux-any], libpng-dev, libpostproc-dev (>= 7:3.0), libprotobuf-dev (>= 2.5.0), libpulse-dev (>= 1.0), libqt5svg5-dev, libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libsecret-1-dev, libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libsoxr-dev (>= 0.1.2), libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 7:3.0), libsystemd-dev [linux-any], libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, protobuf-compiler, python3:native, qtbase5-dev (>= 5.6), qtbase5-private-dev (>= 5.6), wayland-protocols [linux-any], zlib1g-dev, zsh
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
 50610841b060321242793cf6ad3d58759dc5973b 25191148 vlc_3.0.0.orig.tar.xz
 61058d3ce97dd74e418e70b51bf16ac907dc8265 195 vlc_3.0.0.orig.tar.xz.asc
 653447e35b0e82eb6d1d14cb76ac126a6d67eafb 62808 vlc_3.0.0-1ubuntu1.debian.tar.xz
Checksums-Sha256:
 68d587999f50d58df5ca3d69998bba910bdb5a82e5a1a39247179932fae0c19c 25191148 vlc_3.0.0.orig.tar.xz
 42dcba962ccd30fe62121de6845c980f3e66fc5d04eba3cb5a443108afe238f8 195 vlc_3.0.0.orig.tar.xz.asc
 2d4205897d5cc27c9db6c8d018698789b235471f288b3ec0c14dbc30317d87ef 62808 vlc_3.0.0-1ubuntu1.debian.tar.xz
Files:
 a953d8b90e56f06828c4ca8e390c5c9b 25191148 vlc_3.0.0.orig.tar.xz
 cb39ff0155a5afa873a4341ea0109f35 195 vlc_3.0.0.orig.tar.xz.asc
 96bb24d0540d5eb3086704a0b1ecdc66 62808 vlc_3.0.0-1ubuntu1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIcBAEBCAAGBQJafwf9AAoJEPNPCXROn13Zkv4P/A71xvid28CG1BxneG2nwwVu
qO2qxBS6ZSNvWuAP/aWH/iVQJ9d/RG3UT4pVwcVn9WDGPQ2LiLRcDd4SMmS5EvuR
aIKYibbZs3J7FjtVrBFhyICBM9GHpIng04CIFvOGzDa1m/sTsn07LullpfrdDFf8
etUXnFSwHTYDo9R0zBve+/VGWebakFzKmdtMxcJcSEiuZ6q3J+cuvGyOAV0qTs66
j2T7K2EnUA1zA9FUtJq1r1R2KuqfMmMcxsDEsfHXd7DDYU6kc/qb/QWwtYU+KEfY
T9jnCGHI3Q/5ABbfDSeSMLfNrirJ2dnTaVP8p9bZsUEUzhBS9zFqA1p8+ygLbPb8
bc/GPUyHuGCkxf5k7L51RvLuussKvfgPsEpoqU8TMpwPYr2xYy9zbHZ2mvYTfA/R
Cz64itUpbEiRFAgVM5lcJdu30ijWQKaPYz5LRd8naVPem898Y2ayDc2uQhnS8Cnm
NOPqbQtPAck5P6FJw7vIhrmTMQy5VIfheENzntWhJhmNJIHEiGbALlPSqL4IZyTq
19mHaK5sMwv5MR+hPx6D5YUejN2WQ2vpZ0vGO8k37rjgTksSQWkHC0YSoxPzuKu6
KrnlFKuLuFapVMIgDC9G1emZt6kT3V0Cf2+Fa/d1pr2LaliWzjmCIvfb0mTTo1fc
t0jfWkSERa4XK3PIePRP
=U3/h
-----END PGP SIGNATURE-----
