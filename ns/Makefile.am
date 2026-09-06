EXTRA_DIST = Makefile.win libaxl-ns.def libaxl-ns.vcproj

if ENABLE_AXL_LOG
LOG = -DSHOW_DEBUG_LOG
endif

INCLUDES = $(compiler_options) -g -I$(top_srcdir)/src -I$(top_srcdir) $(LIBRARIES_CFLAGS) -DVERSION=\""$(AXL_VERSION)"\" \
	-DPACKAGE_DTD_DIR=\""$(datadir)"\" -DPACKAGE_TOP_DIR=\""$(top_srcdir)"\"  \
	-D__axl_disable_broken_bool_def__ \
	-DVERSION=\"$(AXL_VERSION)\" $(LOG)

libaxl_nsincludedir = $(includedir)/axl

lib_LTLIBRARIES = libaxl-ns.la

libaxl_ns_la_SOURCES     = axl_ns_doc.c axl_ns_node.c
libaxl_nsinclude_HEADERS = axl_ns.h axl_ns_doc.h axl_ns_node.h
libaxl_ns_la_LIBADD      = $(top_builddir)/src/libaxl.la

libaxl_ns_la_LDFLAGS = -no-undefined -export-symbols-regex '^(axl|__axl|_axl).*'

# libaxl-ns.def is a checked-in file, refreshed by hand with "make
# update-def". See the note at src/Makefile.am about why it must not
# be declared as a target depending on update-def.
.PHONY: update-def

update-def:
	test -f .libs/libaxl-ns.exp || { echo "ERROR: .libs/libaxl-ns.exp not found, run make first"; exit 1; }
	echo "EXPORTS" > libaxl-ns.def
	cat .libs/libaxl-ns.exp >> libaxl-ns.def

