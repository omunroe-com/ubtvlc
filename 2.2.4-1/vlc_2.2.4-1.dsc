-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba
Architecture: any all
Version: 2.2.4-1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.8
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, debhelper (>= 9.20151219), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, python3, qtbase5-dev (>= 5.1), zlib1g-dev, zsh, libbz2-dev, libgme-dev, libgsm1-dev, liblzma-dev, libmp3lame-dev, libopenjpeg-dev, librtmp-dev, libsnappy-dev, libsoxr-dev, libssh-gcrypt-dev, libvpx-dev, libwavpack-dev, libwebp-dev, libxvidcore-dev, yasm
Package-List:
 libvlc-dev deb libdevel optional arch=any
 libvlc5 deb libs optional arch=any
 libvlccore-dev deb libdevel optional arch=any
 libvlccore8 deb libs optional arch=any
 vlc deb video optional arch=any
 vlc-data deb video optional arch=all
 vlc-nox deb video optional arch=any
 vlc-plugin-fluidsynth deb video optional arch=any
 vlc-plugin-jack deb video optional arch=any
 vlc-plugin-notify deb video optional arch=any
 vlc-plugin-samba deb video optional arch=linux-any,kfreebsd-any
 vlc-plugin-sdl deb video optional arch=any
 vlc-plugin-svg deb video optional arch=any
 vlc-plugin-zvbi deb video optional arch=any
Checksums-Sha1:
 8c0fb21703cf77442338a26f2f645eda68bde6b2 7205628 vlc_2.2.4.orig-ffmpeg-2-8-7.tar.xz
 ec171b0ad731d9d114540cd7b7fcb41fc3293696 22199316 vlc_2.2.4.orig.tar.xz
 370d1b28834b5c1161d3bbd2c4493bf022cdeb00 73292 vlc_2.2.4-1.debian.tar.xz
Checksums-Sha256:
 97be8a6c1cab2c52015544f76d40f6fd0babcb1b40cf601b4e57fb818564157c 7205628 vlc_2.2.4.orig-ffmpeg-2-8-7.tar.xz
 1632e91d2a0087e0ef4c3fb4c95c3c2890f7715a9d1d43ffd46329f428cf53be 22199316 vlc_2.2.4.orig.tar.xz
 f713f7c1d002c5bb91376557d53a3418cc1a3ba75e0484f44c93c9d8c79f18cd 73292 vlc_2.2.4-1.debian.tar.xz
Files:
 7e5b1c98eedfc3a364fa8c4095deeae5 7205628 vlc_2.2.4.orig-ffmpeg-2-8-7.tar.xz
 55666c9898f658c7fcca12725bf7dd1b 22199316 vlc_2.2.4.orig.tar.xz
 958497e824b3fb62eba33d4191e9e613 73292 vlc_2.2.4-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCAAGBQJXVEbiAAoJEGny/FFupxmT5xoP/jgHjU4qy0hYk+8WpXjKIhc4
RG8hBYpA6fENYUDN9IaM+BtVVb+KMvBGskd2h8zRmaSE1S53eXRFUaRmMJO5LGdi
tXjU0dfWV87DyECC5wD0FgnhHEI9yNHFH500dWw49KFvckpsuvV69JE9DB/YKVbD
U1MSqzcEAT4gh0dAVisDXe5gvRjRRQcX+kVhwPGS3mtTENO8OXbuZOfxPGzPyyn8
W+gcDmrH5bRixRDoltnxtfBsFHjDeFLwunGuX7I95rOefim/KBRmlU/9NNehisKN
cNk7sEEOizzslpA+PMNXnYzyn3RevKqv5JPWcf8HDR+N1aG7VzXokogQqqo58Hql
tLlD+51ZtFHnN3mphtbkqOgk2HpmOK/UjDg1X643EYuuZTbgr7WCmeag4r0qyX0v
N8UQ6x2i+il/UZS/99GqzVjmrlql86wPld+dYl6UP2oy8/0/sgdX4LEvMYMc3Vhh
LsSfXaJE2RzjQc3ovz191nHxMTiTyksGk9//odSZ4TZ1ARAQO3y9gNW0LCAyQ8F9
FTqJgcZkyu6ZreNW5HQIJQo8SOKJO3hM0+dweOsx+/X4NsmQWH1IK+HUt2+oVT9n
C/E4TtOSWiCp3w+LZZ6TunUhyoCoDvBuUmjTyIeghwUNeZf+gL91e9t1LzTGqy1y
+X3E6Z28eXF/qh2sOoPC
=8XAg
-----END PGP SIGNATURE-----
