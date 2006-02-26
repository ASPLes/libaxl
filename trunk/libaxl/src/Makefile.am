EXTRA_DIST = Makefile.win

INCLUDES = -g -I$(top_srcdir) $(LIBRARIES_CFLAGS) -DVERSION=\""$(AXL_VERSION)"\" \
	-DPACKAGE_DTD_DIR=\""$(datadir)"\" -DPACKAGE_TOP_DIR=\""$(top_srcdir)"\" -g -Wall \
	-DVERSION=\"$(AXL_VERSION)\"

libaxlincludedir = $(includedir)/axl

lib_LTLIBRARIES = libaxl.la

libaxl_la_SOURCES     = axl.c  axl_decl.c axl_node.c axl_doc.c axl_error.c axl_stream.c axl_log.c axl_list.c axl_stack.c axl_dtd.c
libaxlinclude_HEADERS = axl.h axl_decl.h axl_node.h axl_doc.h axl_error.h axl_stream.h axl_log.h axl_list.h axl_stack.h axl_dtd.h

