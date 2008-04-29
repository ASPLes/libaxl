EXTRA_DIST = Makefile.win axl_config_win32.h libaxl.vcproj libaxl.def

if ENABLE_AXL_LOG
LOG = -DSHOW_DEBUG_LOG
endif

INCLUDES = -Wall -g -I$(top_srcdir) $(LIBRARIES_CFLAGS) -DVERSION=\""$(AXL_VERSION)"\" \
	-DPACKAGE_DTD_DIR=\""$(datadir)"\" -DPACKAGE_TOP_DIR=\""$(top_srcdir)"\" -Wall -ansi \
	-DVERSION=\"$(AXL_VERSION)\" $(LOG)

libaxlincludedir = $(includedir)/axl

lib_LTLIBRARIES = libaxl.la

libaxl_la_SOURCES     = axl.c  axl_decl.c axl_node.c axl_doc.c axl_error.c axl_stream.c axl_log.c axl_list.c axl_stack.c axl_dtd.c axl_hash.c axl_factory.c
libaxlinclude_HEADERS = axl.h axl_config.h axl_decl.h axl_node.h axl_doc.h axl_error.h axl_stream.h axl_log.h axl_list.h axl_stack.h axl_dtd.h axl_hash.h axl_factory.h

libaxl_la_LDFLAGS = -no-undefined -export-symbols-regex '^(axl|__axl|_axl).*'

libaxl_la_LIBADD = -lm

libaxl.def: update-def

update-def:
	echo "EXPORTS" > libaxl.def
	cat .libs/libaxl.exp >> libaxl.def

