-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore5, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-pulse, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi
Architecture: any all
Version: 2.0.0~unix-0ubuntu2
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Sam Hocevar (Debian packages) <sam+deb@zoy.org>, Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.2
Vcs-Browser: http://git.debian.org/?p=pkg-multimedia/vlc.git;a=summary
Vcs-Git: git://git.debian.org/git/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 7.2.3~), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.16) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev [linux-any], libavcodec-dev (>= 4:0.6), libavformat-dev (>= 4:0.6), libbluray-dev (>= 1:0.2.1), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirac-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev, libdvdread-dev (>= 0.9.5), libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev, libfreetype6-dev, libfribidi-dev, libggi2-dev, libgl1-mesa-dev, libglib2.0-0, libgnutls-dev (>= 2.0.0), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.1-0-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libpng12-dev, libpostproc-dev (>= 4:0.6), libproxy-dev, libpulse-dev (>= 0.9.11), libqt4-dev (>= 4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libsvga1-dev [amd64 i386], libswscale-dev (>= 4:0.6), libtag1-dev (>= 1.5), libtar-dev, libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [kfreebsd-any linux-any], libva-dev [linux-any], libvcdinfo-dev (>= 0.7.22), libvorbis-dev, libx11-dev, libx11-xcb-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxml2-dev, libxpm-dev, libxt-dev, libzvbi-dev (>= 0.2.28), lua5.1, nasm, pkg-config, yasm [amd64 kfreebsd-amd64], zlib1g-dev
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
 98cd9997280e0f4912dfcc6af5ca9346d54d4839 17166840 vlc_2.0.0~unix.orig.tar.xz
 b6f2a083553670fe0f7bba201c5ffc20d47c171c 54614 vlc_2.0.0~unix-0ubuntu2.debian.tar.gz
Checksums-Sha256: 
 ddce2d42b40112a3080f9cdecee6d241a8ff5605aa7ecdb490db4041cc3c618c 17166840 vlc_2.0.0~unix.orig.tar.xz
 cd74adc84081efcee7094a98fb0614df43d4cabe7759eea83ed38efecde2076a 54614 vlc_2.0.0~unix-0ubuntu2.debian.tar.gz
Files: 
 ac1713e5ef1e84d6e7156e2148fae72f 17166840 vlc_2.0.0~unix.orig.tar.xz
 f73a9f421c354525b4a5b0d1e6d0680a 54614 vlc_2.0.0~unix-0ubuntu2.debian.tar.gz
Original-Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1.4.11 (GNU/Linux)

iQIcBAEBCAAGBQJPPMlLAAoJEPmIJawmtHufqvIQAKo6osbJfGI05+QXpvd9rkzb
pxBppLSUu2d0yTtF/vUwuRgg+uhPm4XcC9wuDJAr4POY6g44lxzdD0+EcBS3nRAK
J01loJuTxYihijzK3efPx99RsQ1wv9Ba8PnLdF3xboVKj+DtT3hDPnxUNoYBWH5d
gaZPlLvmYdVdIROMpWvi2rs3y4JHlQCTW4s3K3BGVDdiyq7p16sG3gl5EmmdAYSf
KqV4x7iEyO4FOvigJWxrQIU2GEB07W/7/2299gkOPzC0eGVOaGKJUx54K9pQhIFC
rHnfWcDFIeuIM2eQHENCNAx7YJCvOS/nvkMCu9ddFUkUoXzKEloNPdiEzTxHlWfY
Co5mlSfb3Ej6jl7IFc0XOLg2b9gs4T0BRzqliFA3zpv3YE5v21kL3v0mh2Wz4tDf
yB1FzfiZoZ4VBRE2DEsbfCE0eAFN0QAClXdXwYdFCR5lKVvpCC3AHj9jwPJpMIVm
teW2oGWxdtM7M/fcV32SD1TRviiQEaJdV/Zc/7l/ZoJfAHMbmgeiVNF3zZfy+AB7
nJr5NRQiHtAeJmLhUoqFYNl8PxTM6lC4R9Ywu83PyWKBrBI9PC+F2BlQMAf6EJ42
DeZnT/LmomqlSGP0VZxr+JfkxsUgYfDeFmTZG8b4DORX0I5BotcTbGVDjEN2OZjJ
FmfOfHpzpagmBNyNfMma
=Uu8W
-----END PGP SIGNATURE-----
