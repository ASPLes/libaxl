EXTRA_DIST = __init__.py

if ENABLE_AXL_LOG
LOG = -DSHOW_DEBUG_LOG
endif

INCLUDES = -Wall -Wstrict-prototypes -g -I$(top_srcdir) $(LIBRARIES_CFLAGS) -DVERSION=\""$(AXL_VERSION)"\" \
	-DPACKAGE_DTD_DIR=\""$(datadir)"\" -DPACKAGE_TOP_DIR=\""$(top_srcdir)"\" -Wall -ansi \
	-D__axl_disable_broken_bool_def__ \
	-DVERSION=\"$(AXL_VERSION)\" $(LOG) -I../src $(PYTHON_CFLAGS)

lib_LTLIBRARIES = libpy_axl.la

# libpy_axl.la
libpy_axl_la_include_HEADERS  = \
	py_axl.h \
	py_axl_doc.h \
	py_axl_error.h \
	py_axl_node.h \
	py_axl_dtd.h \
	py_axl_list.h \
	py_axl_hash.h \
	py_axl_stack.h \
	py_axl_stream.h
libpy_axl_la_SOURCES  = \
	py_axl.c \
	py_axl_doc.c \
	py_axl_error.c \
	py_axl_node.c \
	py_axl_dtd.c \
	py_axl_list.c \
	py_axl_hash.c \
	py_axl_stack.c \
	py_axl_stream.c
libpy_axl_la_LDFLAGS  = $(PYTHON_LIBS) ../src/libaxl.la 
libpy_axl_la_includedir = $(includedir)/py_axl

libdir = ${pythondir}/axl

install-exec-hook:
	@echo "site installation ${pythondir}/axl.."
	install -m 644 \
		__init__.py \
		${pythondir}/axl