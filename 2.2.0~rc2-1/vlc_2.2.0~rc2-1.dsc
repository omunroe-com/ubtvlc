-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0~rc2-1
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
 cbc94a45cc062448dfd7b4660c8888637fd8a4b2 57760 vlc_2.2.0~rc2-1.debian.tar.xz
Checksums-Sha256:
 10ac93456f52e0ca11f7af4ae87179339ca76b767aedf978e87014b2fea7abb5 20821112 vlc_2.2.0~rc2.orig.tar.xz
 92b0313afdb93a1b66f3d064a1e8f33473eff30bcbfeac805fb1d769d271337e 57760 vlc_2.2.0~rc2-1.debian.tar.xz
Files:
 2b4d2288605d0b02c22aee4bbcce66ca 20821112 vlc_2.2.0~rc2.orig.tar.xz
 c1a808962ea8bef39e04f85cf2d1bb77 57760 vlc_2.2.0~rc2-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCAAGBQJUcdJuAAoJEGny/FFupxmT+5gP/1bU3xke5C8nJKAO1r7VVgfC
B3cN9D/WQZbYmpTFqUIWbp+LS7Ejtm4qkedp0l2Zt5OXtpUPAKi1wBh82VEH8uTg
O7feTKM27CbLUWz15yIhJI4HaXkbE34k6UM2XWnj7L/6StMlpBT3j/bz/hEYiuT5
EyssD8ippt24zCodjcBaGPwFn4pDt5d13qzIsuChmrrFNujd7vbIKlbl1kzD3YJy
+c4SD+BYGGi42HR8Y21084G+IAwSR+UEUca4fNwxZ6K2onKQ39lYHK2a28QYV9/p
b4zRSe89u+OB8bAt62Dz8oFDO3uS1FFfxxgF0UZ0kutq5MmcqdxZ4ZVyu3E/fZMv
8eT8s1gfGgEg7OHXNlumVVo1OlA894m7SM7HBJbw7O3Y01PazOl13vsBAJMhb7WE
RBO5N1Ouwe8w789WUlDqQAfsL55a+8ZOdaS06Io5a1SqLKOmSozmCoH8ctVwFS+S
DcSFNM94UCrTzzoGoZO/hnU0cBpymtza7T4z+vXg7xae0woN6Eu+4gWMiSf44wPm
71Fr4AIt1RAfbZ8TON+lAJbfZrjcX80R94E/G/rZk5uyYFZsiMd2b2fJARkvEdJF
YuoBr2rM4OUwzC6CNApl8Hfl9JRZIKONa+a6J+RT3H27nMI1RTFWmzbvfFhN9Dxi
pIvLgJVmnW8cmeuPWPvE
=Nx+h
-----END PGP SIGNATURE-----
