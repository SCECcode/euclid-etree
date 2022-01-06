# GNU Automake config

noinst_PROGRAMS = \
	etree4d2txt \
	txt2etree4d \
	bin2etree4d \
	query4d

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

etree4d2txt_SOURCES = etree4d2txt.c
etree4d2txt_LDADD = $(top_builddir)/libsrc/libetree.la -lm

txt2etree4d_SOURCES = txt2etree4d.c
txt2etree4d_LDADD = $(top_builddir)/libsrc/libetree.la -lm

bin2etree4d_SOURCES = bin2etree4d.c
bin2etree4d_LDADD = $(top_builddir)/libsrc/libetree.la -lm

query4d_SOURCES = query4d.c
query4d_LDADD = $(top_builddir)/libsrc/libetree.la -lm


# End of file
