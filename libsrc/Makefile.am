# GNU Automake config

lib_LTLIBRARIES = libetree.la

libetree_la_SOURCES = \
	btree.c \
	buffer.c \
	code.c \
	dlink.c \
	etree.c \
	schema.c \
	schemax.c \
	wrapper.c \
	xplatform.c


pkginclude_HEADERS = \
	btree.h \
	buffer.h \
	code.h \
	dlink.h \
	etree.h \
	etree_inttypes.h \
	expandtable.h \
	extracttable.h \
	schema.h \
	schemax.h \
	wrapper.h \
	xplatform.h

libetree_la_CFLAGS = -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -DINTEL


# End of file
