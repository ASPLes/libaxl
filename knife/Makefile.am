EXTRA_DIST      = $(wildcard *.c) $(wildcard *.dtd) $(wildcard *.xml) $(wildcard *.xdl) Makefile.win 

if ENABLE_AXL_LOG
LOG = -DSHOW_DEBUG_LOG
endif

if ENABLE_AXL_NS
NS = -DAXL_NS_SUPPORT
NS_LIB = ../ns/libaxl-ns.la
endif

INCLUDES = -I../src/ -I../ns -Wall $(LOG) $(NS) $(EXARG_CFLAGS) -DVERSION=\"$(AXL_VERSION)\" -D__COMPILING_AXL_KNIFE__

# replace with bin_PROGRAMS to check performance
bin_PROGRAMS = axl-knife

axl_knife_SOURCES = axl-knife.c axl-knife.h
axl_knife_LDADD   = ../src/libaxl.la $(NS_LIB) $(EXARG_LIBS)


