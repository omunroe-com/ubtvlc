-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA256

Format: 3.0 (quilt)
Source: vlc
Binary: libvlc-dev, libvlc5, libvlccore-dev, libvlccore8, vlc, vlc-data, vlc-dbg, vlc-nox, vlc-plugin-fluidsynth, vlc-plugin-jack, vlc-plugin-notify, vlc-plugin-sdl, vlc-plugin-svg, vlc-plugin-zvbi, vlc-plugin-samba, vlc-plugin-pulse
Architecture: any all
Version: 2.2.0~pre3-1
Maintainer: Debian Multimedia Maintainers <pkg-multimedia-maintainers@lists.alioth.debian.org>
Uploaders: Clément Stenac <zorglub@debian.org>, Loic Minier <lool@dooz.org>, Christophe Mutricy <xtophe@videolan.org>, Mohammed Adnène Trojette <adn+deb@diwi.org>, Reinhard Tartler <siretart@tauware.de>, Benjamin Drung <bdrung@debian.org>, Edward Wang <edward.c.wang@compdigitec.com>, Mateusz Łukasik <mati75@linuxmint.pl>, Sebastian Ramacher <sramacher@debian.org>
Homepage: http://www.videolan.org/vlc/
Standards-Version: 3.9.6
Vcs-Browser: http://anonscm.debian.org/gitweb/?p=pkg-multimedia/vlc.git
Vcs-Git: git://anonscm.debian.org/pkg-multimedia/vlc.git
Build-Depends: autopoint, debhelper (>= 9), dh-autoreconf, dh-buildinfo, gettext, liba52-0.7.4-dev, libaa1-dev, libasound2-dev (>= 1.0.24) [linux-any], libass-dev (>= 0.9.8), libavahi-client-dev, libavc1394-dev (>= 0.5.3) [linux-any], libavcodec-dev (>= 6:9) | libavcodec-ffmpeg-dev, libavformat-dev (>= 4:0.6) | libavformat-ffmpeg-dev, libbluray-dev (>= 1:0.3.0), libcaca-dev (>= 0.99.beta4), libcddb2-dev, libcdio-dev (>= 0.78.2), libchromaprint-dev (>= 0.6.0), libcrystalhd-dev [amd64 i386], libdbus-1-dev (>= 1.6.0), libdc1394-22-dev (>= 2.1.0) [linux-any], libdca-dev, libdirectfb-dev, libdvbpsi-dev | libdvbpsi5-dev, libdvdnav-dev (>> 4.9.0), libdvdread-dev (>> 4.9.0), libegl1-mesa-dev, libfaad-dev, libflac-dev (>= 1.1.2-3), libfluidsynth-dev (>= 1.1.2), libfreetype6-dev, libfribidi-dev, libgl1-mesa-dev, libgles1-mesa-dev, libgles2-mesa-dev, libgnutls28-dev (>= 3.0.20), libgtk2.0-dev, libidn11-dev, libiso9660-dev (>= 0.72), libjack-dev, libkate-dev (>= 0.3.0), liblircclient-dev, liblivemedia-dev (>= 2011.12.23), liblua5.2-dev, libmad0-dev, libmatroska-dev (>= 1.0.0), libmodplug-dev (>= 1:0.8.8.1), libmpcdec-dev, libmpeg2-4-dev, libmtp-dev (>= 1.0.0), libncursesw5-dev, libnotify-dev, libogg-dev (>= 1.0), libomxil-bellagio-dev [linux-any], libopus-dev (>= 1.0.3), libpng-dev, libpostproc-dev (>= 4:0.6), libpulse-dev (>= 1.0), libqt4-dev (>= 4:4.6.0), libraw1394-dev (>= 2.0.1) [linux-any], libresid-builder-dev, librsvg2-dev, libsamplerate0-dev, libschroedinger-dev (>= 1.0.10), libsdl-image1.2-dev, libsdl1.2-dev (>= 1.2.10), libshine-dev (>= 3.0.0), libshout3-dev, libsidplay2-dev, libsmbclient-dev, libspeex-dev (>= 1.0.5), libspeexdsp-dev (>= 1.0.5), libssh2-1-dev, libswscale-dev (>= 4:0.6) | libswscale-ffmpeg-dev, libtag1-dev (>= 1.9), libtheora-dev (>= 1.0), libtwolame-dev (>= 0.3.8), libudev-dev [linux-any], libupnp-dev, libv4l-dev [linux-any], libva-dev [kfreebsd-any linux-any], libvcdinfo-dev (>= 0.7.22), libvdpau-dev, libvncserver-dev (>= 0.9.9), libvorbis-dev, libx11-dev, libx264-dev (>= 2:0.86), libxcb-composite0-dev, libxcb-keysyms1-dev (>= 0.3.4), libxcb-randr0-dev (>= 1.3), libxcb-shm0-dev, libxcb-xv0-dev (>= 1.1.90.1), libxcb1-dev (>= 1.6), libxext-dev, libxinerama-dev, libxml2-dev, libxpm-dev, libzvbi-dev (>= 0.2.28), lua5.2, oss4-dev [kfreebsd-any], pkg-config, zlib1g-dev
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
 857a86cd96999a74851bb7f1ddb8bcbf6714ca8c 20381496 vlc_2.2.0~pre3.orig.tar.xz
 4c44ef7dd5c9f6a9001fb356da33770a2041a31f 57448 vlc_2.2.0~pre3-1.debian.tar.xz
Checksums-Sha256:
 7a8a11bb66192e7ddbb827767ab97cf3d8a55ce3946c1b05e3e46b8ccd5a45d7 20381496 vlc_2.2.0~pre3.orig.tar.xz
 90de356f77b75cedb22772131e0495dac8484815736e2fe79efd50413688ca9f 57448 vlc_2.2.0~pre3-1.debian.tar.xz
Files:
 7c7159df66480bb653685c8f1fa4dad0 20381496 vlc_2.2.0~pre3.orig.tar.xz
 6cd59ddb566468c73ae127252ba83dbd 57448 vlc_2.2.0~pre3-1.debian.tar.xz

-----BEGIN PGP SIGNATURE-----
Version: GnuPG v1

iQIcBAEBCAAGBQJUKDINAAoJEGny/FFupxmTcFMP/RK9uq5LXKj7FQLtyeFPC8Nk
mLxvIi4AZh9L0f94C8KhLIwhzanrc/oUNylHPYkkoPBB5RPN5eqtgux/2RUdlI3A
mQiJ9b/nIKNEjoMgZSIA2SkDX/AEr4hHHr4+Pfks9LSZTUfSXkDvR7Z+wFTUIKKg
6Q0Cyro5kf0TtUcmi6hz2x8jTOslRfkf0HDaRvN6s/Y7hXldA8+7UiZz/F9ox7Fw
p5vkU5Y2fpnuPdQ6WcVCj6wjWp6SeSz4P391bP8duaA/bTx8nxP+mvVOjeq4wO7t
swVKJPt7LLIDbtxFFFeynYq/gkv747fbDUT4oyclzG0Smhunjfc+JJBWhQqSubr7
XXgdfgAi07Jplh7yCU8P7EgGbYncH6MpiZO+MNdhgkRilNBRLs905kjws6Ee7kTG
z8sl1H94nv7i8hGSZdNnuM+UHfys0uU2f2yxOD4ISouZqkAHDNnhUzAXR796siFR
vL2E1OWRUAB7dCK8W2V9pVK84j+VkZ6ncUOGFu1FZF9Zr3Ys13P/yZXN9xvVLnX2
wPzFuXjlk4zXCZ1YtkmIa+8LllL3bL8EiDCXxQ9I3yFcgyyV/Xgz8HExNDxPs1BG
3HHXaY/jNwB7F9ue34wFQQzpBAkV7YIfTYGH/RuwTr15Gd8XFbJDBH4ncbrz3etB
ziKhRBcdm9jKAnTWMBnX
=H2lD
-----END PGP SIGNATURE-----
