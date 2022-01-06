# GNU Automake config

noinst_PROGRAMS = \
	definedomain \
	createcvm \
	packcvm \
	querycvm

AM_CPPFLAGS = -I$(top_srcdir)/libsrc

definedomain_SOURCES = definedomain.c domain.c

createcvm_SOURCES = createcvm.c domain.c
createcvm_LDADD = $(top_builddir)/libsrc/libetree.la -lm

packcvm_SOURCES = packcvm.c
packcvm_LDADD = $(top_builddir)/libsrc/libetree.la -lm

querycvm_SOURCES = querycvm.c
querycvm_LDADD = $(top_builddir)/libsrc/libetree.la -lm

noinst_HEADERS = domain.h
dist_noinst_DATA = labase.2 labase.domain labase.layers



# Parameters to createcvm 
DOMAIN = $(abs_srcdir)/labase.domain
LAYERS = $(abs_srcdir)/labase.2 #labase.layers
ETREENAME_UNPACKED = lasmall_unpacked.e
ETREENAME_PACKED = lasmall.e

create_cvm: createcvm packcvm
	./createcvm $(DOMAIN) $(LAYERS) $(abs_builddir) $(ETREENAME_UNPACKED)
	./packcvm $(ETREENAME_UNPACKED) $(ETREENAME_PACKED)
	rm -f $(ETREENAME_UNPACKED)

labase.e: create_cvm

query_point: querycvm labase.e
	./querycvm labase.e -118.5 33.6 0 -118.5 33.6 0 0 0 Vs point.txt

query_line: querycvm labase.e
	./querycvm labase.e -118.5 33.6 0 -118.25 33.8 1000 1 100 Vp line.txt

query_slice: querycvm labase.e
	./querycvm labase.e -118.5 33.6 0 -118.25 33.8 1000 2 100 all slice.txt

query_volume: querycvm labase.e
	./querycvm labase.e -118.5 33.6 0 -118.25 33.8 1000 3 100 density volume.txt


# End of file
