-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba
Architecture: any all
Version: 2.2.8-1ubuntu2
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: https://www.videolan.org/vlc/
Standards-Version: 4.1.1
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, debhelper (>= 10), gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblirc-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopenmpt-modplug-dev | libmodplug-dev (>= 1:0.8.8.1), libopus-dev (>= 1.0.3), libpng-dev, libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp1.8-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, python3, qtbase5-dev (>= 5.1), zlib1g-dev, zsh, libbz2-dev, libgme-dev, libgsm1-dev, liblzma-dev, libmp3lame-dev, librtmp-dev, libsnappy-dev, libsoxr-dev, libssh-gcrypt-dev, libvpx-dev, libwavpack-dev, libwebp-dev, libxvidcore-dev, yasm
Package-List:
 libvlc-bin deb video optional arch=any
 libvlc-dev deb libdevel optional arch=any
 libvlc5 deb libs optional arch=any
 libvlccore-dev deb libdevel optional arch=any
 libvlccore8 deb libs optional arch=any
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
 vlc-plugin-samba deb video optional arch=linux-any,kfreebsd-any
 vlc-plugin-sdl deb video optional arch=any
 vlc-plugin-skins2 deb video optional arch=any
 vlc-plugin-svg deb video optional arch=any
 vlc-plugin-video-output deb video optional arch=any
 vlc-plugin-video-splitter deb video optional arch=any
 vlc-plugin-visualization deb video optional arch=any
 vlc-plugin-zvbi deb video optional arch=any
Checksums-Sha1:
 ebb676b482b6004270035e3f8f4d67fc8971c385 7221224 vlc_2.2.8.orig-ffmpeg-2-8-13.tar.xz
 b960ec5bdb9a51da285430fc68962927ccc87187 22137276 vlc_2.2.8.orig.tar.xz
 b41691396f41206d7cc08130c6b6ab2161e2792b 195 vlc_2.2.8.orig.tar.xz.asc
 085a94922eddb0a1f48e07448d844aa472494d34 78088 vlc_2.2.8-1ubuntu2.debian.tar.xz
Checksums-Sha256:
 c8c807cddfaf6659de81453e8719be8d87f0995a206debdf266ac61bd2c8d843 7221224 vlc_2.2.8.orig-ffmpeg-2-8-13.tar.xz
 9bf046848fb56d93518881b39099b8288ee005d5ba0ddf705b6f6643b8d562ec 22137276 vlc_2.2.8.orig.tar.xz
 65fed4719af6fc8df11028ed9cdb92cbb7870e81397f0c07480152233f232a0d 195 vlc_2.2.8.orig.tar.xz.asc
 e4658d31fda16a1b07fd36b14ad1e62b5692ed8a9baf4b7407b65bff77f9149e 78088 vlc_2.2.8-1ubuntu2.debian.tar.xz
Files:
 71314748d96efb7ed80ecbe37de33447 7221224 vlc_2.2.8.orig-ffmpeg-2-8-13.tar.xz
 b721fddf65aaf64eeee5629aa9bf7c9e 22137276 vlc_2.2.8.orig.tar.xz
 6faebf727bf6f58e5ececcfef4f8e9e9 195 vlc_2.2.8.orig.tar.xz.asc
 c7f9e77a2ae08ba31ade0b2d18e7e7d0 78088 vlc_2.2.8-1ubuntu2.debian.tar.xz
Original-Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----

iQIcBAEBCAAGBQJaKAxKAAoJEPNPCXROn13ZuJcP/j7KzXWXSNELDFjjOASmjSE9
Ro/rhvXrNItTFFARbVtLQgnD3QXE67Q2xbCnla5+iC6t/1DoWc6Yj6wCixTXe3AQ
tc3Q3Cnv/eiQJWqeuOeQBmowVh6HpkwUshCe7DjEUkTf0pLXYO11q20lDvQjQhGS
OEnlFbJoy7/XveGv0J/cKOoMqU6cXopKGKBfCj7Fre+oqe+d16J1ex7UzTWxcIFs
XWUy3Lx+N8fOu5Owa2LMXkxT/UUa/3w5AqMzV/xPJxYs2Ur/moWvnxJ3Uw1exYRG
fvkcppdwGPlSiPO+iyGCHuNxdurmEqv5DZDtFfqhVhmDgtvCP0cRaZM2Dr6DySBh
HpE4y1TmIWZFwMZ6i3jQfyL3nnobyz0/0fWxwLeTVvyKrHpbuZWj5Gfc6wbbLjQC
JVY6juDNRuuT91mP1VAf7jBQxeqmF+uSL08y+vlc377f6sF1nlKynVQg+73IijIi
r1kMj1T/wcDPFXuQ0nu9Kr7a9i1KC+kTzSUgJeUeEvzPxyUlG5wfpjwE/fakTQeJ
RVL7DTco4KF+pXB1xBqF6XcPlvLxl/f75HDcyNy1qVnViTt5c4Ju9lS8WEFh3+zo
7GIUn/YBYAHuzIcpZRMOh/Xz2NicnmRWYQqjSSD7p7gvMRdhVqyzg5DjsCzPB7vv
DAW9wF2kPU4S5uUmc46U
=NOYL
-----END PGP SIGNATURE-----
