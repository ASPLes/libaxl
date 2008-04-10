EXTRA_DIST = Makefile.win # libaxl-babel.def libaxl-babel.vcproj

if ENABLE_AXL_LOG
LOG = -DSHOW_DEBUG_LOG
endif

INCLUDES = -Wall -g -I$(top_srcdir)/src -I$(top_srcdir) $(LIBRARIES_CFLAGS) -DVERSION=\""$(AXL_VERSION)"\" \
	-DPACKAGE_DTD_DIR=\""$(datadir)"\" -DPACKAGE_TOP_DIR=\""$(top_srcdir)"\" -Wall -ansi \
	-DVERSION=\"$(AXL_VERSION)\" $(LOG)

libaxl_babelincludedir = $(includedir)/axl

lib_LTLIBRARIES = libaxl-babel.la

libaxl_babel_la_SOURCES     = axl_babel.c axl_babel_iso885915.c axl_babel_iso88591.c axl_babel_iso88592.c
libaxl_babelinclude_HEADERS = axl_babel.h axl_babel_iso885915.h axl_babel_iso88591.h axl_babel_iso88592.h

libaxl_babel_la_LDFLAGS = -no-undefined -export-symbols-regex '^(axl|__axl).*'

libaxl-babel.def: update-def

update-def:
	echo "EXPORTS" > libaxl-babel.def
	cat .libs/libaxl-babel.exp >> libaxl-babel.def

refresh-tables: gen-table
	./gen-table codes.txt iso885915.utf8.txt iso-8859-15
	./gen-table codes.txt iso88591.utf8.txt iso-8859-15


# replace with bin_PROGRAMS to check performance
noinst_PROGRAMS = gen-codes gen-table read-codes

gen_codes_SOURCES  = gen-codes.c
gen_codes_LDADD   = ../src/libaxl.la 

gen_table_SOURCES  = gen-table.c
gen_table_LDADD   = ../src/libaxl.la 

read_codes_SOURCES = read-codes.c
read_codes_LDADD   = ../src/libaxl.la 


