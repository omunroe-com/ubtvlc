-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0~pre2-4build1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.5
Vcs-Browser: http://anonscm.debian.org/gitweb/?p=pkg-multimedia/vlc.git
Vcs-Git: git://anonscm.debian.org/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 9), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 6:9) | libavcodec-ffmpeg-dev, libavformat-dev (>= 4:0.6) | libavformat-ffmpeg-dev, libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirac-dev, libdirectfb-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 1.0), libqt4-dev (>= 4:4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6) | libswscale-ffmpeg-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libvpx-dev, libx11-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev, pkg-config, zlib1g-dev
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
 3bbdd419a4ff2af21114ed1e1b20f621d9a4f9a6 67505 vlc_2.2.0~pre2-4build1.debian.tar.gz
Checksums-Sha256: 
 2370dd24aab71aeb4010aabd3fdfdf5da8763bb24948b590c088a845a7f4b067 20374604 vlc_2.2.0~pre2.orig.tar.xz
 e24e77d8f2e3fbfe9232b5a229a5f9f14aa734ad58663581ae37df80eb1e1ff4 67505 vlc_2.2.0~pre2-4build1.debian.tar.gz
Files: 
 6a4bda79c7af131c2e94e19bc6548843 20374604 vlc_2.2.0~pre2.orig.tar.xz
 1b27a15005d04ba1710066bdaf36b86b 67505 vlc_2.2.0~pre2-4build1.debian.tar.gz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1
Comment: Colin Watson <cjwatson@debian.org> -- Debian developer

iQIVAwUBVAkWMDk1h9l9hlALAQgVdw//T8+9J4lrIKHpjZPeUdxzcW47x8jIM1fS
fyFrxzWTnw4HgVGud7DA61Lo4N0xc8g8g8tN9ZFwX6+6WpWoccoktjqao3PTFEbp
mWUxYW1PjqB6B+II4bdGcmpk/UeFv+qeFcSgss2sVzDJEBWhSce3CtKhsyGcaqZL
6erpZoh2Q6fFRCgeIu7b05yrGF0DbVZGoojulMYWTxrxVfHBHws1qpKCMNDjrSAx
oCJ668jXIMfGnjgnppBnILgiSQrLM8HNPF3zoNVzgh3cqxxb6rYtpEKHnhsl9tPU
RB50c1UFBPQoc3uX/ns3BIsgEa85x3VaJ7HgYpOF4orF3eAC4+8beH/fC+hfdkGf
RbElbEpT851kodUSNBAymZAkjpNEe8be/z7TQyMBo9rN+0POsdxzmB8Q9Jab825O
C/3fqKzSfmpkfh3X6RbIQ5jD1MVFGWfEiQR+CpADqKv/55HlJHd+BdTNWCrCmWNn
eTOuZVM0ccA4rKH3/l83HkHFHmjbe7TnePrPbQWBW0RZ1YNEm/o1Oc2a1iA5C/gN
+2Vk+tl6xCBYFI5M2F4F+xG/H3Hqs7X+LcWfiifhMjQpUGZqN7yipgrMTCJfEgzb
dJu/6fD9KFdAjcx1+w/tNJSUUcTr6tKRuU1ubFm1uxm3mfFc5TueSHwAUCCNvoVM
vDtOjGN0DJI=
=a4+i
-----END PGP SIGNATURE-----
