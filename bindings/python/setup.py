from distutils.core import setup, Extension
import os

def get_vlcconfig():
    vlcconfig=None
    for n in ( 'vlc-config',
               os.path.sep.join( ('..', '..', 'vlc-config' ))):
        if os.path.exists(n):
            vlcconfig=n
            break
    if vlcconfig is None:
        print "*** Warning *** Cannot find vlc-config"
    elif os.sys.platform == 'win32':
        # Win32 does not know how to invoke the shell itself.
        vlcconfig="sh %s" % vlcconfig
    return vlcconfig

def get_cflags():
    vlcconfig=get_vlcconfig()
    if vlcconfig is None:
        return []
    else:
        cflags=os.popen('%s --cflags' % vlcconfig, 'r').readline().rstrip().split()
        return cflags

def get_ldflags():
    vlcconfig=get_vlcconfig()
    if vlcconfig is None:
        return []
    else:
	os.environ['top_builddir'] = '../..'
        ldflags=os.popen('%s --libs vlc builtin' % vlcconfig, 'r').readline().rstrip().split()
        return ldflags

# To compile in a local vlc tree
vlclocal = Extension('vlc',
                sources = ['vlcglue.c', '../../src/control/init.c'],
                include_dirs = ['../../include', '../../', '/usr/win32/include' ],
                extra_objects = [ '../../lib/libvlc.a' ],
                extra_compile_args = get_cflags(),
		extra_link_args = [ '-L../..' ]  + get_ldflags(),
                )

setup (name = 'MediaControl',
       version = '0.8.2-1',
       scripts = [ 'vlcdebug.py' ],
       description = """VLC bindings for python.

This module provides a MediaControl object, which implements an API
inspired from the OMG Audio/Video Stream 1.0 specification. Moreover,
the module provides a Object type, which gives a low-level access to
the vlc objects and their variables.

Example session:

import vlc
mc=vlc.MediaControl(['--verbose', '1'])
mc.playlist_add_item('movie.mpg')

# Start the movie at 2000ms
p=vlc.Position()
p.origin=vlc.RelativePosition
p.key=vlc.MediaTime
p.value=2000
mc.start(p)
# which could be abbreviated as
# mc.start(2000)
# for the default conversion from int is to make a RelativePosition in MediaTime

# Display some text during 2000ms
mc.display_text('Some useless information', 0, 2000)

# Pause the video
mc.pause(0)

# Get status information
mc.get_stream_information()

# Access lowlevel objets
o=vlc.Object(1)
o.info()
i=o.find_object('input')
i.list()
i.get('time')
       """,
       ext_modules = [ vlclocal ])
