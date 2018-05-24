-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0~rc2-2
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
 d124ad416dcc8171ea38d716250db58a1af0ec9b 59516 vlc_2.2.0~rc2-2.debian.tar.xz
Checksums-Sha256:
 10ac93456f52e0ca11f7af4ae87179339ca76b767aedf978e87014b2fea7abb5 20821112 vlc_2.2.0~rc2.orig.tar.xz
 202082c88e4a4b81b11eb7fe2c0a04f638c7fa08bee2d824711659313c8dc178 59516 vlc_2.2.0~rc2-2.debian.tar.xz
Files:
 2b4d2288605d0b02c22aee4bbcce66ca 20821112 vlc_2.2.0~rc2.orig.tar.xz
 3957db5553a882d682d8367d1e577828 59516 vlc_2.2.0~rc2-2.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCAAGBQJUwCNBAAoJEGny/FFupxmTIPgQAKhhQak66jcHdlgRtH4g2OK0
ZZHsciM68w+EF3uxRW4MjAeCSN8LdSpvnIOtJsXIOt0LXfIsPBXplVnqphVU6Gy/
lsWhymrkpy0nK2VN+CZNw72hBOKN8Ne09FHE10izE6WCOxjH7I5kp+9N0N83IwZI
mTZL+rP4ISWGAKcTvSrtMJpVWMsiKLn8DAo1cAun/JF9aTSrz5+elVhJokQKMXEp
zGa/VW3YQtAQZrx/MLN/OALayz/k0QREfc1/Va3nRKoP9hUdosizu/EnZAgqtq6a
vnNROrwmKXqaS20+RX+WR18c6QWCXvnzZruEKAtj42IBQtEVAcJsCcEN/+EvTsjt
kgWQ1CAyv9UipRyNw4AR+nYvdRFPqYTv5TMkyB4m0Uo2WL9cgU5Uc2sFjuD0W8sg
/hdWS57b+MHYCTeqEUNVbCFXh7D/MZ095dA04fU0xbftSDnjg041WYSRT1ckkvnl
t+p3K1bD/8IWQhGOhz2z2WG/2TxGfmylf0jO3pPgTPAEsq0Fhx3zDt7nR1XAP05b
QxNIGk6ckTAVFKGifLkNGrctoidhFt9V26HT5PZj80pO53D4iXFYsTS6ZL/n2Wrk
FbIER0F2pQ3h2lw5zWpRYITCQjGwJ0vR7e1XaImKV/k1g+oQyFwd4QGp9WgFnnMu
L8kIUncnMeDLHB9ilG78
=1biK
-----END PGP SIGNATURE-----
