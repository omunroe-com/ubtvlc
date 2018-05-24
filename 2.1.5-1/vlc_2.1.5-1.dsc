-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore7, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-pulse, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi
Architecture: any all
Version: 2.1.5-1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.5
Vcs-Browser: http://anonscm.debian.org/gitweb/?p=pkg-multimedia/vlc.git
Vcs-Git: git://anonscm.debian.org/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 9), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 4:0.6) | libavcodec-ffmpeg-dev, libavformat-dev (>= 4:0.6) | libavformat-ffmpeg-dev, libbluray-dev (>= 1:0.2.1), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirac-dev, libdirectfb-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev, libdvdread-dev (>= 0.9.5), libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev, libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 0.9.11), libqt4-dev (>= 4:4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6) | libswscale-ffmpeg-dev, libtag1-dev (>= 1.6.1), libtar-dev, libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev, pkg-config, zlib1g-dev
Package-List:
 libvlc-dev deb libdevel optional arch=any
 libvlc5 deb libs optional arch=any
 libvlccore-dev deb libdevel optional arch=any
 libvlccore7 deb libs optional arch=any
 vlc deb video optional arch=any
 vlc-data deb video optional arch=all
 vlc-dbg deb debug extra arch=any
 vlc-nox deb video optional arch=any
 vlc-plugin-fluidsynth deb video optional arch=any
 vlc-plugin-jack deb video optional arch=any
 vlc-plugin-notify deb video optional arch=any
 vlc-plugin-pulse deb video optional arch=any
 vlc-plugin-sdl deb video optional arch=any
 vlc-plugin-svg deb video optional arch=any
 vlc-plugin-zvbi deb video optional arch=any
Checksums-Sha1:
 7f1cb6324a04cf393896bbb5976ca9febd7b3efc 19574208 vlc_2.1.5.orig.tar.xz
 f4655bf83ac7a74eff19fa714550fbe2b6716318 57068 vlc_2.1.5-1.debian.tar.xz
Checksums-Sha256:
 6f6566ab6cd90d381395b7f0e401060b044cd3843e50ceb252b558a88e5d1f72 19574208 vlc_2.1.5.orig.tar.xz
 cf8c08c52bf7d87f273b54706f3f9f8a283142afb6344666cbc99a0576bcd00a 57068 vlc_2.1.5-1.debian.tar.xz
Files:
 3941b561f590cc95ca5e795213cba2f2 19574208 vlc_2.1.5.orig.tar.xz
 425da15fe791158a075ac827c93a3021 57068 vlc_2.1.5-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCgAGBQJT2BLcAAoJEBWetcTvyHdMnkkQAJ2XnGZgLfLlUcnzgMEuCSIc
ioLC2K8mUAxCyQXZhxzi+H2Wh6HqEp2NTAZMQzEkgY3pLbveQjX6H40TU0Ln2OrF
zuxRbgr31ZOEx29NIXzTscJpl44cuefRZ+qydV72SufXyLmhhXOdM6bgJMIebmcH
TMiwaQocnDnklTwUTVxUN6eigyw7hxuAppan5rcJ248Eim6Tj0UyRQTTobFsQLFE
c3peV2rr6kDdiKul0NkZR77TD1jc3zc9bddxuMHjrEGQUcV3cDh/hPIPUmtbmOj5
MDf7zTy4wXNeYOS6OqqJSP67JXcKBycL4v1DpBpbRrqMMfmWYloFf4+SR89Leiep
LqVprN1iIOQ00fYY7Z+GUmgB+N59bWLVTTwGFSjHtesLdRpvUHqpb/STUpvsZ8/x
qctKuE8nljrB5/21qvRee3GOSZBYGlHPVFL147sLSnCsmGPcvbdGJY+ZbcchLAh3
HF+1QPjSKa9Ok1SyVlJ8WshTHSgX6T+otOXr42TTkx7yRP221Njiyt+bo4dtMU+B
mXaGYmLQxh0R/w0tbQ8NfSCeVqgIkVu8sOncOZSh9lKE8YqR6rq+mfxI2gwfcU5e
+R9rq0qy9RVVtxj/U2aUx2Cz/SWKvOfpoEhCJwI+ivhMelgHSyZREMaQzQnIQmOi
UbLrF7HG895PV8EL4nxb
=cmeR
-----END PGP SIGNATURE-----
