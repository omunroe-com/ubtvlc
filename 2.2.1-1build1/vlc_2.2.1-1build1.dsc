-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.1-1build1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.6
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: git://anonscm.debian.org/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 9), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 6:9) | libavcodec-ffmpeg-dev, libavformat-dev (>= 4:0.6) | libavformat-ffmpeg-dev, libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 1.0), libqt4-dev (>= 4:4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6) | libswscale-ffmpeg-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, zlib1g-dev, zsh
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
 ee8535c5966217bc2069a74c1d4e8c32609656e9 20940288 vlc_2.2.1.orig.tar.xz
 42fd105a0363f26bccbb80faf3c398cfebd5c5b4 68254 vlc_2.2.1-1build1.debian.tar.gz
Checksums-Sha256: 
 543d9d7e378ec0fa1ee2e7f7f5acf8c456c7d0ecc32037171523197ef3cf1fcb 20940288 vlc_2.2.1.orig.tar.xz
 9c6ae5c7943cb5e920bc02a21e2f21422873130c1eabc98a4501ae46d91c3782 68254 vlc_2.2.1-1build1.debian.tar.gz
Files: 
 42273945758b521c408fabc7fd6d9946 20940288 vlc_2.2.1.orig.tar.xz
 db844d9c807f91f21232a9f253082ba8 68254 vlc_2.2.1-1build1.debian.tar.gz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCAAGBQJVbNpFAAoJEJzmobXpLZXXZc0QAKS2slmbWi+VAm9KOiQqzd87
82kTDEFE9PAeRyP2eg4Jd0fw8G0eVQp+SheahfY5oFEs9Rofn00jAsimJDk29L8P
od93NKLpYiFapRHVOkUTZapTaRVU10bTyLkgQ3cflYJf+JbPMCPL7AZF5pGb1Nx9
hKkRBrgNTVABUkp8hxBPCITd7z+/HobzYxwhgFPNoGtrRpidXL3cHkXrIADaLPuC
WUcUh9dih6dMAP/ZqCqxQXvOtWjLUkROnWllTQkG4BuxreSg5G89hZoyM3MjkdJm
zUmTWSBA7uL8DtBoXaKhlXlgePpTk4eHQKus/aaDx94t8s2M5xwnoSUSsBo04+Rc
FqJQ9fOp+5qrrBCfJuuTQgGZYlb4jL6txRPsVxZA6Bn1C7YRFMCFGNlznf1WD0cn
wPXuxhVbneEinx5oYcONJxDYpC3lJ1XkAOxxDYSSEh13YWP6IY8W/jmaEQPZ690/
QPouN16EFNuVt+ZE1W+RhOXJftG86aTUq8tA/TIY35VD0nFtFKPPHiICQQom9QDb
RCAlF0YrCSvt1VXctVibXftDonsA9txBRe/edqvF/5MFvXNMMucYv4IZZlogrWap
ukOZ5wy+BRFFTTVFH6lhCYam4r6LWeRfltNmRQA7RNAb5V0JpVTCcbsPrN4a7kO0
sLQXguyuFkoHxTFE3qDU
=4Ysw
-----END PGP SIGNATURE-----
