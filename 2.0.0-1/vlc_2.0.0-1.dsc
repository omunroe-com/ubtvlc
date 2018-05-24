-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore5, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-pulse, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi
Architecture: any all
Version: 2.0.0-1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
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
 c28c4c8128110fd02a7bdf55c9dad5cc20eb91b8 17144876 vlc_2.0.0.orig.tar.xz
 f80b93626241f1599b9848efc61138f221b0c95d 54559 vlc_2.0.0-1.debian.tar.gz
Checksums-Sha256: 
 455fc04b5f7ce3d7294ed71a9dd172ff4eb97875cfc30b554ef4ce55ec6f5106 17144876 vlc_2.0.0.orig.tar.xz
 2e14d83b75562d602d7c009361b43ac75636e9fdc096d24f66f236e290cfdb7d 54559 vlc_2.0.0-1.debian.tar.gz
Files: 
 8806bff2ea9c76791123d444a92f708c 17144876 vlc_2.0.0.orig.tar.xz
 2b9236cc2ef2c2200e7d6889f3acd30f 54559 vlc_2.0.0-1.debian.tar.gz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1.4.11 (GNU/Linux)

iQIcBAEBCgAGBQJPPvbNAAoJEBWetcTvyHdMjIQQAJSt+x3sE0pR65asnjKa+7qH
APztVl0oHclg/nTHqsLisEgtevAmjnxyX2IMQ7E6A+cEZYk2S2ZenTyxBiEhH609
aq9ILSwTS2/V92Ob8pFWyFgbX8TIx7FxWim+ViFF9h+0H5/MYvltrfg489KVKXWN
aEpWeSP+x6ZpPzQZ7VIjoesuAKWX0tyiq0aPGnbcX/WwCjtmueODRD5wlMhUJnT5
0ZJNaYSSVAvL2lYr6+sa9ymdlxFF8ngXLijIKi7o3zmIxVQHhMpofjiouyGrnApY
o4Vo0ESxuTdsZsGuVNoOQtWi17QxXV1cm9H8T3iw4sXUpAhssxpK1B5GI0D7aclK
Rc5Ai/yotSnOFHq482rSY3j/ZAe6zh77dE9bchsKTp+5lUF+NSy1EnU1/zHh7txZ
+337Iq3Ta4YyoSLJCIRlerat8QodFQmZkd7EWCYMVmNp5WTMrGDrYhEDQ5+NtWTT
CEsu/w8/SB+bkKNrRHb7FxCaSThQT56dlFrh8HxtaJ1kpqa7zSgGyWUoZWtoyZnk
jCyfLYzhSyb9ToijEWYTyRJ8mMkiorrTgPqn5eGTgC6UJU0syB72z49wwAeab/bc
/gmD/kr0bwqe88Fdh2LuyPd52W3CjA3CFF7bK54QDEk/xRxzVcwGBR2DjvTTD3GN
Yxx6IPlmPaaD1a9LivGt
=zVkh
-----END PGP SIGNATURE-----
