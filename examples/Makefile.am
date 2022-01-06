# GNU Automake config

noinst_PROGRAMS = \
	tiny \
	query \
	txt2etree \
	etree2txt \
	refine

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

tiny_SOURCES = tiny.c
tiny_LDADD = $(top_builddir)/libsrc/libetree.la -lm

txt2etree_SOURCES = txt2etree.c
txt2etree_LDADD = $(top_builddir)/libsrc/libetree.la -lm

etree2txt_SOURCES = etree2txt.c traverse.c
etree2txt_LDADD = $(top_builddir)/libsrc/libetree.la -lm

query_SOURCES = query.c
query_LDADD = $(top_builddir)/libsrc/libetree.la -lm

refine_SOURCES = refine.c traverse.c
refine_LDADD = $(top_builddir)/libsrc/libetree.la -lm

noinst_DATA = tree.txt

check: tiny txt2etree etree2txt refine query
	./tiny tree.e
	./txt2etree tree.e < $(abs_srcdir)/tree.txt
	./etree2txt tree.e
	./refine tree.e
	./query tree.e < $(abs_srcdir)/tree.txt


# End of file
