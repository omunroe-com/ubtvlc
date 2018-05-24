-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore5, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-pulse, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi
Architecture: any
Version: 2.0.0~unix-0ubuntu1
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Sam Hocevar (Debian packages) <sam+deb@zoy.org>, Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.2
Vcs-Browser: http://git.debian.org/?p=pkg-multimedia/vlc.git;a=summary
Vcs-Git: git://git.debian.org/git/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 7.2.3~), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.16) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev [linux-any], libavcodec-dev (>= 4:0.6), libavformat-dev (>= 4:0.6), libbluray-dev (>= 1:0.2.1), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirac-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev, libdvdread-dev (>= 0.9.5), libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev, libfreetype6-dev, libfribidi-dev, libggi2-dev, libgl1-mesa-dev, libglib2.0-0, libgnutls-dev (>= 2.0.0), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.1-0-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libpng12-dev, libpostproc-dev (>= 4:0.6), libproxy-dev, libpulse-dev (>= 0.9.11), libqt4-dev (>= 4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libsvga1-dev [amd64 i386], libswscale-dev (>= 4:0.6), libtag1-dev (>= 1.5), libtar-dev, libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [kfreebsd-any linux-any], libva-dev [linux-any], libvcdinfo-dev (>= 0.7.22), libvorbis-dev, libx11-dev, libx11-xcb-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxml2-dev, libxpm-dev, libxt-dev, libzvbi-dev (>= 0.2.28), lua5.1, nasm, pkg-config, yasm [amd64 kfreebsd-amd64], zlib1g-dev
Checksums-Sha1: 
 98cd9997280e0f4912dfcc6af5ca9346d54d4839 17166840 vlc_2.0.0~unix.orig.tar.xz
 6d2561634cc7e2cedd111a5985ce629947136d61 55606 vlc_2.0.0~unix-0ubuntu1.debian.tar.gz
Checksums-Sha256: 
 ddce2d42b40112a3080f9cdecee6d241a8ff5605aa7ecdb490db4041cc3c618c 17166840 vlc_2.0.0~unix.orig.tar.xz
 d508a3360fe7f24394d40490d6d28babf40c6211393c2a7134fdb4cad5390364 55606 vlc_2.0.0~unix-0ubuntu1.debian.tar.gz
Files: 
 ac1713e5ef1e84d6e7156e2148fae72f 17166840 vlc_2.0.0~unix.orig.tar.xz
 066d4012b7e6b4d9544b2f81d03d1217 55606 vlc_2.0.0~unix-0ubuntu1.debian.tar.gz
Original-Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1.4.11 (GNU/Linux)

iQIcBAEBCgAGBQJPOF3dAAoJEBWetcTvyHdMrRQP/AqoQVYvLnFcY072AXV3H4uA
FNRWJbXINzQe/sOgq4yalQ6uOgFPuger5hUaMRAaLemrPdbitNxeaA3887jSYxVX
OskCoSffjj1ulU13x46a10lkHrABJ67QjbmBzqmlD7LPq2IFnJjBFYv13J5Wb6UW
fx1KbiCjoO/57feaV+HtNe1nZsdUS8YoUmnmqgK7/SWVBzO5HutYqfXOnD1SYKFu
LB9X1IJPrNzbf7Wp6u3UIgc2nosqn+7Yz4j0HDTNe7VX1v7WJTjP7QT2ATzLx8Fx
M4bOJMQVqKVG+TmfHL2m6lal4GvPt3ybm2srgP/1cN/oNZ0BscLHsg/M3IAwqv+F
nhnvKDdJIfqhXGzKIIb4tHTTvhc6ok5xQGFf5D3Ub2XDS7nPtjS9NHmU7xB8x+pi
ezzETfHBnNN6Rd12Jw5QuBUATevfvjSVf41FluY/I+oMKFvcC7kmHbmwOppkJe29
zf3pLv2CYm5zOANL3/v11LhsT2JwMEYyF/yBn/yuKmTNDGr3IV6wWgnem+myseGz
I2tzAZGEa8p2kx6v3zTPRHXBfGJonsQcZTPV06eKLQ6RFOwGBn5vCFQrriPsKpCq
B0ZBxZTQ9hj8r0Rn5DLp5wCxtB4luElYCjWcQxiCVy6K7fVFlJAm1xtic6J5Iv78
mqTvtESaSZuxGYK+Nw0x
=yTwL
-----END PGP SIGNATURE-----
