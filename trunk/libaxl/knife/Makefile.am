EXTRA_DIST      = \
	axl-knife.c axl-knife.h \
	exarg.c exarg.h \
	Makefile.win 

if ENABLE_AXL_LOG
LOG = -DSHOW_DEBUG_LOG
endif

if ENABLE_AXL_NS
NS = -DAXL_NS_SUPPORT
NS_LIB = $(top_builddir)/ns/libaxl-ns.la
endif

INCLUDES = -I$(top_srcdir)/src/ -I$(top_srcdir)/ns $(compiler_options) $(LOG) $(NS) -DVERSION=\"$(AXL_VERSION)\" -D__COMPILING_AXL_KNIFE__ 	-D__axl_disable_broken_bool_def__  $(EXARG_FLAGS)

# replace with bin_PROGRAMS to check performance
bin_PROGRAMS = axl-knife

axl_knife_SOURCES = axl-knife.c axl-knife.h exarg.h exarg.c
axl_knife_LDADD   = $(top_builddir)/src/libaxl.la $(NS_LIB)


# copy exarg dependency to not force people to download a library with
# two files
exarg.h exarg.c: 
	test -e exarg.h || cp $(top_srcdir)/../libexarg/src/exarg.h .
	test -e exarg.c || cp $(top_srcdir)/../libexarg/src/exarg.c .