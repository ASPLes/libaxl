EXTRA_DIST = Makefile.win

INCLUDES = -g -I$(top_srcdir) $(LIBRARIES_CFLAGS) -DVERSION=\""$(AXL_VERSION)"\" \
	-DPACKAGE_DTD_DIR=\""$(datadir)"\" -DPACKAGE_TOP_DIR=\""$(top_srcdir)"\" -Wall -ansi -pedantic \
	-DVERSION=\"$(AXL_VERSION)\"

libaxlincludedir = $(includedir)/axl

lib_LTLIBRARIES = libaxl.la

libaxl_la_SOURCES     = axl.c  axl_node.c axl_doc.c
libaxlinclude_HEADERS = axl.h axl_decl.h axl_node.h axl_doc.h


