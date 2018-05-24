-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore5, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-pulse, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi
Architecture: any all
Version: 2.0.4-0ubuntu0.12.04.1
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Sam Hocevar (Debian packages) <sam+deb@zoy.org>, Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.3
Vcs-Browser: http://git.debian.org/?p=pkg-multimedia/vlc.git;a=summary
Vcs-Git: git://git.debian.org/git/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 8), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.16) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev [linux-any], libavcodec-dev (>= 4:0.6), libavformat-dev (>= 4:0.6), libbluray-dev (>= 1:0.2.1), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libcrystalhd-dev [amd64 i386], libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirac-dev, libdirectfb-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev, libdvdread-dev (>= 0.9.5), libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev, libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libglib2.0-0, libgnutls-dev (>= 2.0.0), libgtk2.0-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.1-0-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libpng-dev, libpostproc-dev (>= 4:0.6), libproxy-dev, libpulse-dev (>= 0.9.11), libqt4-dev (>= 4:4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libsvga1-dev [amd64 i386], libswscale-dev (>= 4:0.6), libtag1-dev (>= 1.5), libtar-dev, libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [kfreebsd-any linux-any], libva-dev [linux-any], libvcdinfo-dev (>= 0.7.22), libvorbis-dev, libx11-dev, libx11-xcb-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libxt-dev, libzvbi-dev (>= 0.2.28), lua5.1, pkg-config, zlib1g-dev
Package-List: 
 libvlc-dev deb libdevel optional
 libvlc5 deb libs optional
 libvlccore-dev deb libdevel optional
 libvlccore5 deb libs optional
 vlc deb video optional
 vlc-data deb video optional
 vlc-dbg deb debug extra
 vlc-nox deb video optional
 vlc-plugin-fluidsynth deb video optional
 vlc-plugin-jack deb video optional
 vlc-plugin-notify deb video optional
 vlc-plugin-pulse deb video optional
 vlc-plugin-sdl deb video optional
 vlc-plugin-svg deb video optional
 vlc-plugin-zvbi deb video optional
Checksums-Sha1: 
 4d9ec642593a5bac282f0793d9c78140b28523f9 18518272 vlc_2.0.4.orig.tar.xz
 d0469fd86ca5a56da5029ae4161e31a2bed29afa 58186 vlc_2.0.4-0ubuntu0.12.04.1.debian.tar.gz
Checksums-Sha256: 
 4e066ed0d48ddc85aa1f01964945af666b5713cb6230d76347aebbc9a6553db6 18518272 vlc_2.0.4.orig.tar.xz
 3ec4701e4c84d4c522fc3687326ff0172d1d77e4cfbfbf1ea754c96adc439b08 58186 vlc_2.0.4-0ubuntu0.12.04.1.debian.tar.gz
Files: 
 f36dab8f126922c56b372388b7fade47 18518272 vlc_2.0.4.orig.tar.xz
 24bf19d53a5d5c42f9d9924dabea5bd1 58186 vlc_2.0.4-0ubuntu0.12.04.1.debian.tar.gz
Original-Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1.4.11 (GNU/Linux)

iQIcBAEBCgAGBQJQtgh4AAoJEGVp2FWnRL6TjQgQAKxpb6h4+i74+V1Y8WZu9EhP
EnaiElxjmv22YGujC3yDOu8GSQWFYF65VYYHNY7A/zwL+O9lIL3qqiOGHP1tJV4s
GfGhC9LaE1nhjsT6W/aN9xz2D2XS1fPr/RPNkLvWzHEAUcB4fFdFv413VCOz2Wuv
a+YOHzSAe3Nf+IBegP/EWSSMYobKcr46ugAe83vetGx+/3DkpMvUtir5PZeQIU8O
Z3erfQ19gtkeYd/F8CF/V3TWJp8j/LWRndjNQJ3AVSll9D71p2HqUCNffnqhZDbJ
de9LN2of0puO0VT5Ey59Y9NDky9XlCR+hrDa9KlSiFA64uz0Bz2w8PXrULMo1k8K
CX0M8m3cJ+EXx3m4myZ4zPORSJiA5q+BT3tYkNpnmoSGnCQnUbmMd6Ki2TxcmU2K
GUM32hhd9RUai3HgT2Dd4kJIw9C1at5barslxpaWLhVkXwPdRi3XOxt0oW7eKNfK
l7aVcKzutkky0dv3/EYK3of0+UHcLsU0BDCwDrdrs84yIilRgtMFQsFINIPWYKCc
VP2OBRVSSENvVHUrxAQZYFQFYcN+APS0O7hHn7DF5px1uafbcz6C4h3DXhpUDA/w
G/cv6htVsYBDoNZohXbJUgZVdmDSutEgpouH23GuroCqew35xok1Fa1Cilcxiw9H
PrivK4kLnQ7GrqSLXWVI
=ttWK
-----END PGP SIGNATURE-----
