EXTRA_DIST      = fact.dtd.h channel.dtd.h xml-rpc.dtd.h tls.dtd.h \
	$(wildcard *.c) $(wildcard *.dtd) $(wildcard *.xml) \
	$(wildcard *.xdl) Makefile.win test_01.vcproj

if ENABLE_AXL_LOG
LOG = -DSHOW_DEBUG_LOG
endif

if ENABLE_AXL_NS
NS = -DAXL_NS_SUPPORT
NS_LIB = ../ns/libaxl-ns.la
endif

if ENABLE_AXL_BABEL
BABEL = -DAXL_BABEL_SUPPORT
BABEL_LIB = ../babel/libaxl-babel.la
endif

INCLUDES = -I../src/ -I../ns -I../babel -Wall $(LOG) $(NS) $(BABEL) -DVERSION=\""$(AXL_VERSION)"\" 

# replace with bin_PROGRAMS to check performance
bin_PROGRAMS = test_01 test_01a test_02a test_03a

test_01_SOURCES = test_01.c
test_01_LDADD   = ../src/libaxl.la $(NS_LIB) $(BABEL_LIB)

test_01a_SOURCES = test_01a.c
test_01a_LDADD   = ../src/libaxl.la 

test_02a_SOURCES = test_02a.c
test_02a_LDADD   = ../src/libaxl.la 

test_03a_SOURCES = test_03a.c
test_03a_LDADD   = ../src/libaxl.la

refresh: 
	../knife/axl-knife --input fact.dtd --dtd-to-c --output fact.dtd.h --ifnewer
	../knife/axl-knife --input channel.dtd --dtd-to-c --output channel.dtd.h  --ifnewer
	../knife/axl-knife --input xml-rpc.dtd --dtd-to-c --output xml-rpc.dtd.h  --ifnewer
	../knife/axl-knife --input tls.dtd --dtd-to-c --output tls.dtd.h  --ifnewer

leak-check:
	libtool --mode=execute valgrind --leak-check=yes ./test_01

compref:
	gcc -g -Wall `pkg-config --libs --cflags libxml-2.0` test_01b.c -o test_01b
	gcc -g -Wall `pkg-config --libs --cflags libxml-2.0` test_02b.c -o test_02b
	gcc -g -Wall  `pkg-config --libs --cflags glib-2.0` -I ../src ../src/.libs/libaxl.so test_03c.c -o test_03c

