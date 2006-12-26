EXTRA_DIST      = $(wildcard *.c) $(wildcard *.dtd) $(wildcard *.xml) $(wildcard *.xdl) Makefile.win

if ENABLE_AXL_LOG
LOG = -DSHOW_DEBUG_LOG
endif

if ENABLE_AXL_NS
NS = -DAXL_NS_SUPPORT
NS_LIB = ../ns/libaxl-ns.la
endif

INCLUDES = -I../src/ -I../ns -Wall $(LOG) $(NS)

# replace with bin_PROGRAMS to check performance
bin_PROGRAMS = test_01 test_01a test_02a test_03a

test_01_SOURCES = test_01.c
test_01_LDADD   = ../src/libaxl.la $(NS_LIB)

test_01a_SOURCES = test_01a.c
test_01a_LDADD   = ../src/libaxl.la 

test_02a_SOURCES = test_02a.c
test_02a_LDADD   = ../src/libaxl.la 

test_03a_SOURCES = test_03a.c
test_03a_LDADD   = ../src/libaxl.la

leak-check:
	libtool --mode=execute valgrind --leak-check=yes ./test_01

compref:
	gcc -g -Wall `pkg-config --libs --cflags libxml-2.0` test_01b.c -o test_01b
	gcc -g -Wall `pkg-config --libs --cflags libxml-2.0` test_02b.c -o test_02b
	gcc -g -Wall  `pkg-config --libs --cflags glib-2.0` -I ../src ../src/.libs/libaxl.so test_03c.c -o test_03c

