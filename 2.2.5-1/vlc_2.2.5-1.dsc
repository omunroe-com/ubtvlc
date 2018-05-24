-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: vlc, libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, libvlc-bin, vlc-bin, vlc-data, vlc-l10n, vlc-plugin-base, vlc-plugin-access-extra, vlc-plugin-video-output, vlc-plugin-video-splitter, vlc-plugin-visualization, vlc-plugin-skins2, vlc-plugin-qt, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-nox
Architecture: any all
Version: 2.2.5-1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.8
Vcs-Browser: https://anonscm.debian.org/cgit/pkg-multimedia/vlc.git
Vcs-Git: https://anonscm.debian.org/git/pkg-multimedia/vlc.git
Testsuite: autopkgtest
Build-Depends: autopoint, debhelper (>= 10), gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcairo2-dev (>= 1.13.1), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreerdp-dev (>= 1.0.1), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblirc-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopenmpt-modplug-dev | libmodplug-dev (>= 1:0.8.8.1), libopus-dev (>= 1.0.3), libpng-dev, libpulse-dev (>= 1.0), libqt5x11extras5-dev, libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev [!hurd-any], libsndio-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libx265-dev, libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxi-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, python3, qtbase5-dev (>= 5.1), zlib1g-dev, zsh, libbz2-dev, libgme-dev, libgsm1-dev, liblzma-dev, libmp3lame-dev, librtmp-dev, libsnappy-dev, libsoxr-dev, libssh-gcrypt-dev, libvpx-dev, libwavpack-dev, libwebp-dev, libxvidcore-dev, yasm
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
 vlc-nox deb oldlibs extra arch=any
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
 c008b4ee94fc29527fa270ef957a7a5f400910c5 7214980 vlc_2.2.5.orig-ffmpeg-2-8-11.tar.xz
 3a79a87d424e1896106a063e09843e957a8c89a8 22067640 vlc_2.2.5.orig.tar.xz
 d66bfe27c4ad64577211365cb1baccf594b53045 74584 vlc_2.2.5-1.debian.tar.xz
Checksums-Sha256:
 38aa3aec93c480b34d33f3648aff9278581d151488b521995c084258613e1fe6 7214980 vlc_2.2.5.orig-ffmpeg-2-8-11.tar.xz
 7cada0f86f5da9ceac294ba4cbef435e399223bc8873aa3db4135eeedb5975fd 22067640 vlc_2.2.5.orig.tar.xz
 1185e0e852ace08f9b80000d09ac92805622df5cf314755b860928866532ec9a 74584 vlc_2.2.5-1.debian.tar.xz
Files:
 b8c6953c65d0c9ef120a878064282c75 7214980 vlc_2.2.5.orig-ffmpeg-2-8-11.tar.xz
 66a75b5f2a2ce739fba3459831070bd2 22067640 vlc_2.2.5.orig.tar.xz
 da329e23d2e9ae8e50184b3f5bbc7bed 74584 vlc_2.2.5-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----

iQIzBAEBCAAdFiEE94y6B4F7sUmhHTOQafL8UW6nGZMFAljO8VUACgkQafL8UW6n
GZP1CQ//SmmNmTdpQRtbZUE6vBCdWrER2Kph5y17QZWz5qOwg9oAyd56a595NHj6
n45kfZg9invxA5kaQIWIwvhJ6AHzwvGjCPeF+ig2P9F8JaTqFNS3utBPbCqy6Tf5
LW4T/lU3xWhFTI1PRVDxN8Wh0CDJjrofWdVqh8+sKb+RtlyZjiaIFixh1gIuOMu5
Lw697zPPrZSmaHsTP7Ab9bCxCrQlTDwIZxum3TnGpd+6Z6OfoZmDrWOyqOB7By5X
cfqYny5ylRMOxK6k4Wg4wiOSlnZjBb48b6uPgTTd1cuwmGBcInj9aV+8GM4wWlPt
FFPOR5XSgMI/0fQEAogb7MdOzy9rXpeez0uY5dNa4I6b0rdC6v0NGUkl+tN22i/u
3AqzoLdwWRsIq5aBOXGQgyRBixZrSwlOfLSxuqlJUGYbbbzjpIPAhKAH4tnFK8xO
qAYWUae9SmbdIOAuIdY51hjboQz0MX0vxb4dJNP1XdRkufr2Y2dIFbjPl9Siov6x
N9OHO4gy/fblG7ZbG/Y4ZCwmELq+f8jxAm5dI6dq2ovfN+zc/Fd7BE/LNw+PfjzC
E006kCcEHTygE6ItEvETX6PY7V5cVleKFmtoYwe5P6/OBDW6audA5bR30MLV1m4n
kqs+w61q9NzpiQRJwkCzoMhqkRoEXKUX21YmYqgvlxDVW7cPSyw=
=dwdz
-----END PGP SIGNATURE-----
