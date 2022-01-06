# GNU Automake config

noinst_PROGRAMS = \
	findpixel \
	etreeinfo \
	esetappmeta

AM_CPPFLAGS = -I$(top_srcdir)/libsrc
AM_CFLAGS = -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64

findpixel_SOURCES = findpixel.c
findpixel_LDADD = $(top_builddir)/libsrc/libetree.la -lm

etreeinfo_SOURCES = etreeinfo.c
etreeinfo_LDADD = $(top_builddir)/libsrc/libetree.la -lm

esetappmeta_SOURCES = esetappmeta.c
esetappmeta_LDADD = $(top_builddir)/libsrc/libetree.la -lm


# End of file
