-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0~rc2-2+deb8u1
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
 4baea7abce4f887b9ebd462a862c2c8731d84ce0 20821112 vlc_2.2.0~rc2.orig.tar.xz
 9a1f6a5d39b95d1fa7664d113c4e488a1d5ea5f6 59972 vlc_2.2.0~rc2-2+deb8u1.debian.tar.xz
Checksums-Sha256:
 10ac93456f52e0ca11f7af4ae87179339ca76b767aedf978e87014b2fea7abb5 20821112 vlc_2.2.0~rc2.orig.tar.xz
 a1970977f049f96adb40a6ab9a7c5dc0e9aa2a582affec348524bc2b0da3fd38 59972 vlc_2.2.0~rc2-2+deb8u1.debian.tar.xz
Files:
 2b4d2288605d0b02c22aee4bbcce66ca 20821112 vlc_2.2.0~rc2.orig.tar.xz
 1ffc23264059f0f8294affb7e866201d 59972 vlc_2.2.0~rc2-2+deb8u1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCgAGBQJV1JFHAAoJEAVMuPMTQ89EoqEP/jPP9mU79LNc7y3cc8ynA1eb
yiZ8hebYGVdiIuRojX9QPveqnpWEtEVrzAZqddgq7VmC9WEbp/CC2PaRIZfiEFD4
v225e2poX6vg6gmWmTLV7iOfoSemT9o0VPCV+Nsi9az3TgQJyb5iFP77+C36mR1G
XJqFSfcN0TsWk7r4F4XDWUOCufxLu4djikl1sGvy8wMHpORL+kS4xp4hFUlqdnRv
CkThgUGwt7D8cdSEuPEaWGFvM1WOXpdHvLweYI4sGUsL1FlTGssdQtIBYOUqhzgE
gWnamUhPyrdD/bjrpa0+NVbjtf8pRA8XoJxez/wlg9fIwYz5INfd+Kbq7Jp1GCrS
yfkV4Nm5usDDH1aCTPEPmTkmYutYXXg7mfi1r1m15zFCKC85fuRE2ml6j4SHJu0w
j8t/DcOHbHuabFmIIrHkgZ0U2NvQt8CzC73lx4h9zxuJATUPWQDstGZ5UeL6ToXb
l9rGhH1a26vKmxV3BBjDjSLfZoeuxAAJeX2E29pTven6/YwiW3iQ+yzht6evZllF
x7fWMATF4lq/sBNOIBsx7mv40QqBVi+GgMu95BN+MH6a2dX+ZoJ3+2YbhlOerkx9
O1gj87n9GKvxDVqrwZPqkJ0tYiaEz665bPOiZe5lDXr5970DU6LRy1LE8i8vIt8N
dq2xnHwsfb9TQ7IdktYD
=TMom
-----END PGP SIGNATURE-----
