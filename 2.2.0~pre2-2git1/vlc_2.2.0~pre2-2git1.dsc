-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0~pre2-2git1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.5
Vcs-Browser: http://anonscm.debian.org/gitweb/?p=pkg-multimedia/vlc.git
Vcs-Git: git://anonscm.debian.org/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 9), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 6:9) | libavcodec-ffmpeg-dev, libavformat-dev (>= 4:0.6) | libavformat-ffmpeg-dev, libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirac-dev, libdirectfb-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 1.0), libqt4-dev (>= 4:4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6) | libswscale-ffmpeg-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev, pkg-config, zlib1g-dev
Package-List: 
 libvlc-dev deb libdevel optional
 libvlc5 deb libs optional
 libvlccore-dev deb libdevel optional
 libvlccore8 deb libs optional
 vlc deb video optional
 vlc-data deb video optional
 vlc-dbg deb debug extra
 vlc-nox deb video optional
 vlc-plugin-fluidsynth deb video optional
 vlc-plugin-jack deb video optional
 vlc-plugin-notify deb video optional
 vlc-plugin-pulse deb video optional
 vlc-plugin-samba deb video optional
 vlc-plugin-sdl deb video optional
 vlc-plugin-svg deb video optional
 vlc-plugin-zvbi deb video optional
Checksums-Sha1: 
 83ebd626919f24085340aaf45ac192cd51ba0b84 20374604 vlc_2.2.0~pre2.orig.tar.xz
 eb480b847dd205be1f43dca4f825ca128791bf9b 67202 vlc_2.2.0~pre2-2git1.debian.tar.gz
Checksums-Sha256: 
 2370dd24aab71aeb4010aabd3fdfdf5da8763bb24948b590c088a845a7f4b067 20374604 vlc_2.2.0~pre2.orig.tar.xz
 8c8237974c46a9f4ee47fcb778a1218974d410e742b77e28dbae080a2580117a 67202 vlc_2.2.0~pre2-2git1.debian.tar.gz
Files: 
 6a4bda79c7af131c2e94e19bc6548843 20374604 vlc_2.2.0~pre2.orig.tar.xz
 3e290436a2e6f821db3c294db80b03c9 67202 vlc_2.2.0~pre2-2git1.debian.tar.gz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCgAGBQJT8+P5AAoJEBWetcTvyHdMkRQP/25Tvp1Y27PY9ZKgVY8+2oL9
t9encEdY9XE2BwwaJt809qLLQnvqJYkkhp73/3gI9fGKxB8sPAA5/IIcpLa7MXpI
aSEGSj3ag28vQ0bPK3nzHE7zSCNPEJlAsuj5lQmgZuL1TNivHhAp931kybuBVHlK
CATMYHfq0p3hLzo0Ed2LcfFcvBYVC+8cXj1+sbBNnLbJxRoLapbhaRnEqVR5GtPK
O4zuXUjH1fO2Q3DX7QgcbmDhviCrpy/1CwfGq1lPQlHianzYutMFE031vnA8GWc7
YSFOfWASRXy9uHq8zZWgFHjXrCQ8lzuG2ToW6DebYIzpOB2Iu1nDaZc9XT1lDL19
IGoo25CvOaz8YWQsGB+jywNIcI3rUT2SKvxoqH19aymd24lDw5zpUcV9sZTs7WZZ
Qk94xITZa2Wyt1dnrxksUZ2ehEPgmGggN+PVoAlNgsiXHbJdBWH3KR4kO1T31xWb
8QeU++bZfHtKHQD7fwLMUBcY1c0XrmHpda+k2FLGhLFHAchQ00tP1L07smw9lvXC
Qgf2ucbmXS1uB7OnuCWpFSNzBurP8+chsm33Q0IX9t/+/abpDW1Ws/yTNUN2ppTK
ID8C1S4MxqHXi59zvSwjaFotwBp/eTJZgiv69zCS824L8l3JhI8DCfyOJnTmBUZd
qLTS3TqXAMDM5HsmupnU
=OIoe
-----END PGP SIGNATURE-----
