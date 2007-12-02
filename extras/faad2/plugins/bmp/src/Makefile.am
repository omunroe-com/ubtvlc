libdir = `pkg-config --variable=input_plugin_dir bmp`
lib_LTLIBRARIES = libmp4.la

libmp4_la_CFLAGS = `pkg-config --cflags bmp` -Wall \
       -I$(top_srcdir)/include -I$(top_srcdir)/common/mp4ff

libmp4_la_LIBADD = $(top_builddir)/libfaad/libfaad.la \
       $(top_builddir)/common/mp4ff/libmp4ff.la

libmp4_la_LDFLAGS = -module -avoid-version `pkg-config --libs bmp` -lpthread

libmp4_la_SOURCES = libmp4.c mp4_utils.c aac_utils.c
