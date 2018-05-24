-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0~pre4-2
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.6
Vcs-Browser: http://anonscm.debian.org/gitweb/?p=pkg-multimedia/vlc.git
Vcs-Git: git://anonscm.debian.org/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 9), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 6:9) | libavcodec-ffmpeg-dev, libavformat-dev (>= 4:0.6) | libavformat-ffmpeg-dev, libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 1.0), libqt4-dev (>= 4:4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6) | libswscale-ffmpeg-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, zlib1g-dev
Package-List:
 libvlc-dev deb libdevel optional arch=any
 libvlc5 deb libs optional arch=any
 libvlccore-dev deb libdevel optional arch=any
 libvlccore8 deb libs optional arch=any
 vlc deb video optional arch=any
 vlc-data deb video optional arch=all
 vlc-dbg deb debug extra arch=any
 vlc-nox deb video optional arch=any
 vlc-plugin-fluidsynth deb video optional arch=any
 vlc-plugin-jack deb video optional arch=any
 vlc-plugin-notify deb video optional arch=any
 vlc-plugin-pulse deb video optional arch=all
 vlc-plugin-samba deb video optional arch=any
 vlc-plugin-sdl deb video optional arch=any
 vlc-plugin-svg deb video optional arch=any
 vlc-plugin-zvbi deb video optional arch=any
Checksums-Sha1:
 de765f925609275c60e32cdd3e5fd550ba61b6c6 20416452 vlc_2.2.0~pre4.orig.tar.xz
 3e5437286e29f7650233f8f3f755402b517708dd 57468 vlc_2.2.0~pre4-2.debian.tar.xz
Checksums-Sha256:
 2a765b8ba5238f464c0ce16762fc5adc192cf8481fd10f239eb0d37fb67e0785 20416452 vlc_2.2.0~pre4.orig.tar.xz
 a46928a2b7475eec1608cd4414c1677f31e345fb2601683cd62461539757ff7d 57468 vlc_2.2.0~pre4-2.debian.tar.xz
Files:
 11f1a7b2a1f16af0ee92b42b2d688300 20416452 vlc_2.2.0~pre4.orig.tar.xz
 a99b1d1a298efddbb0526d2dbf711d4f 57468 vlc_2.2.0~pre4-2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCAAGBQJUTB+kAAoJEGny/FFupxmTykkP/if/niyUHvvWoz2pVAvn6JVW
Ka6ybb6fAeUVucNvgNE/y8F0fKF1TDrBteTnpWucuUFLAZ34z7NnAzsB8WoEaqUN
K6bk2foon4ZDS2eDKk2W3DyPB+QPXM1JeQdpCxCB4zJM8+LXRABurz5aCZekhmxD
PkUmlYbW/Jy+7LOZ4Y/gVUiDwi36ADnpas9cWqTqtPTHNIxTGVHM0kVXiX3B9Zmo
1u4Gb8nNVRWhlk8XEExPKbk4w0DGXR3dezhKFaGa4PhWik3vzstG0k1YQ6xq7zfZ
G/+13LWYYt1PdnmiQDzKtVwOeez54j87m+KEepDxZNZVErK/HrfzGPwIrvhyzdKo
GJe2+8kSpt44q6OFe96VsvrRdnm6NL2A8xZh0Qa0LjMwQF5ozcAtEWX3bWQWiYy5
xXHAIeu00cllCgpG6S0XuxFZ5QP2hSkYDF5X6g7jYR+BLDZsjwnZlLoZpm0AA3cP
/tRxzbEaAS5qvuHX+YfehAIbb3p+yPuYR1im3DzS+dAvipGMW/d6x7k/5JmFMaLk
5XvukNnoNnBzD7uysQpRVOb1HwNQIkOYDHMe2W8N7Zqi0QHcmF5j9vDQCXx5GtnY
EqzYnqpoSkKAagW5qWrmTxz7jn0wn/ODOIp3+afQEfy9Dy0PJLuK8OjvPnDQ8p0k
IJrtzZ7rCveOC1d7oRDF
=Iwbt
-----END PGP SIGNATURE-----
