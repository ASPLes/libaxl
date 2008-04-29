EXTRA_DIST = Makefile.win libaxl-ns.def libaxl-ns.vcproj

if ENABLE_AXL_LOG
LOG = -DSHOW_DEBUG_LOG
endif

INCLUDES = -Wall -g -I$(top_srcdir)/src -I$(top_srcdir) $(LIBRARIES_CFLAGS) -DVERSION=\""$(AXL_VERSION)"\" \
	-DPACKAGE_DTD_DIR=\""$(datadir)"\" -DPACKAGE_TOP_DIR=\""$(top_srcdir)"\" -Wall -ansi \
	-DVERSION=\"$(AXL_VERSION)\" $(LOG)

libaxl_nsincludedir = $(includedir)/axl

lib_LTLIBRARIES = libaxl-ns.la

libaxl_ns_la_SOURCES     = axl_ns_doc.c axl_ns_node.c
libaxl_nsinclude_HEADERS = axl_ns.h axl_ns_doc.h axl_ns_node.h
libaxl_ns_LIDADD         = ../src/libaxl.la

libaxl_ns_la_LDFLAGS = -no-undefined -export-symbols-regex '^(axl|__axl|_axl).*'

libaxl-ns.def: update-def

update-def:
	echo "EXPORTS" > libaxl-ns.def
	cat .libs/libaxl-ns.exp >> libaxl-ns.def

