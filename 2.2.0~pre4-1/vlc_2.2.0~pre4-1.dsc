-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0~pre4-1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.6
Vcs-Browser: http://anonscm.debian.org/gitweb/?p=pkg-multimedia/vlc.git
Vcs-Git: git://anonscm.debian.org/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 9), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 6:9) | libavcodec-ffmpeg-dev, libavformat-dev (>= 4:0.6) | libavformat-ffmpeg-dev, libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 1.0), libqt4-dev (>= 4:4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6) | libswscale-ffmpeg-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, zlib1g-dev
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
 b40781aef151e0ff7f0107792fa2faaf8cddaefa 57592 vlc_2.2.0~pre4-1.debian.tar.xz
Checksums-Sha256:
 2a765b8ba5238f464c0ce16762fc5adc192cf8481fd10f239eb0d37fb67e0785 20416452 vlc_2.2.0~pre4.orig.tar.xz
 3722468b26c5e1b96cb1c7f8fcc6cb180412bf92581037d6941643982a1d0261 57592 vlc_2.2.0~pre4-1.debian.tar.xz
Files:
 11f1a7b2a1f16af0ee92b42b2d688300 20416452 vlc_2.2.0~pre4.orig.tar.xz
 78613c16c0a35d749ccf36f07fdabba4 57592 vlc_2.2.0~pre4-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCAAGBQJUMxhTAAoJEGny/FFupxmTDlkP/3iLP/9DOoUYz978RYOEY++K
D/Znoi7e8YGI6oOiL22ieHOWypihoFoWzAd3DEnulmFhoAo5zXqKz1Ji5C8DEON3
umQOlK7/6g+e7uc5LIAvvrG/nC6FiVdtZ02WMOarY/3c8EdmFFcobt0nBppWy2eL
4M7IvoY0Db/GqXr4Gauwd7zhpEWHyLzaIzU+4ilaBzzR5qRYxJ3a7e17msXAaLoH
v09Kw8/uitQKUP+VBNrtv11vftTnL4YFNqh6gvUY6aVKuC7orhJeUmfvDPPEY87D
Tb9Yv8RmaqBTAFmgHBzKmmQ4yjkxCmKOBx0G16a3aET5bmx2b4kDg3Ff44qAcyb5
y9IYYpxKcvDl2jTTn91mBGisVabGNB0VvCByzOJZZW3EFF/Cqj+lF3omZoeS9Qfd
ZqBGC6RvUqvDdgQhGHFoGZGYWp5hURx/GulTvwSn5V35iHc52DQODrcQr9E+dBTc
tK+HvDyeZVtR7I+cJp9OmRnuO4gqkHlKNwG8MNyZfm//b5+3C0WpC5ONSbC6NhwI
GfB/3f5TkZOx1mRPhXdbpVqBxNQKkRVttsm8Qui8JT022Icj3yvAuNTi4HoYVixk
6u+Kh7Yo51tKrbdXuqjCP14Z0Ghmn9wR9AKT+T5NhDnLYRVBlmHzlIRks2ukAQsP
QxbucmDhiKChrmXEhtNi
=2AcP
-----END PGP SIGNATURE-----
