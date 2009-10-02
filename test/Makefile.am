h_files = fact.dtd.h channel.dtd.h xml-rpc.dtd.h tls.dtd.h 
c_files = gen-codes.c gen-table.c read-codes.c test_01.c 
	test_01a.c test_02a.c test_02b.c test_03a.c test_03c.c \
	test_04a.c test_04b.c 
dtd_files = channel.dtd fact.dtd test_30b.dtd test_30.dtd test_31a.dtd \
	test_31.dtd test_32.dtd test_38.dtd test3.dtd \
	test4.dtd test5.1.dtd test5.dtd test.dtd tls.dtd xml-rpc.dtd 
xml_files = \
	test_42.xml test_42a.xml test_42c.xml test_42d.xml \
        test_41.iso-8859-8.xml test_41.iso-8859-7.xml  \
	test_41.iso-8859-6.xml test_41.iso-8859-5.xml test_41.iso-8859-4.xml  \
	test_41.iso-8859-3.xml test_41.utf-8.withoutheader.xml  \
	test_41.utf-8.wrongwithoutheader.xml \
	test_41.utf-8.withoutencoding.xml test_13f.xml test_41.iso-8859-15.2.xml \
	test_41.iso-8859-15.xml test_41.iso-8859-1.xml test_41.iso-8859-2.xml  \
	test_41.iso-8859-9.xml test_41.utf-8.wrong.xml test_41.utf-8.xml  \
	test_41.utf-16.xml test_41.utf-32be.xml test_41.utf-32.xml \
	test_13e.xml test_40.xml test_01f2.xml test_01f.xml test_01e.xml \
	test_38.xml test_35.xml test_13d.xml test_32.xml test_31.xml \
	test_30b.xml test_30a.xml test_30.xml test_29.xml test_28.xml \
	test_27.xml test_26d.xml test_23d.xml test_26c.xml test_26b.xml  \
	test_26.xml large.xml test_23b.xml test_23.xml test_21.xml  \
	test_01d.xml test_20a.xml test_20.xml test_01b.xml test_13b.xml \
	test_13c.xml test_19a.xml test_19b.xml test_19c.xml test_19d.xml \
	test_19e.xml test_19f.xml test_19g.xml test_19.xml test13.xml  \
	channel5.xml channel4.xml channel2.xml channel3.xml channel.xml \
	test5.xml test3.xml test4.xml gmovil2.xml gmovil.xml small.xml  \
	test2.xml test.xml  test_22.xml test_22b.xml test_22c.xml
xdl_files = test17.xdl test03.xdl test04.xdl test01.xdl test02.xdl 
misc_files = Makefile.win test_01.vcproj

EXTRA_DIST      = $(h_files) $(c_files) $(dtd_files) $(xml_files) $(misc_files) $(xdl_files) test_01.py

if ENABLE_AXL_LOG
LOG = -DSHOW_DEBUG_LOG
endif

if ENABLE_AXL_NS
NS = -DAXL_NS_SUPPORT
NS_LIB = $(top_builddir)/ns/libaxl-ns.la
endif

if ENABLE_AXL_BABEL
BABEL = -DAXL_BABEL_SUPPORT
BABEL_LIB = $(top_builddir)/babel/libaxl-babel.la
endif

INCLUDES = -DTEST_DIR=$(top_srcdir)/test -I$(top_srcdir)/src/ -I$(top_srcdir)/ns -I$(top_srcdir)/babel -Wall $(LOG) $(NS) $(BABEL) -DVERSION=\""$(AXL_VERSION)"\" -D__axl_disable_broken_bool_def__ 

# replace with bin_PROGRAMS to check performance
noinst_PROGRAMS = test_01 test_01a test_02a test_03a
TESTS = test_01

if ENABLE_PY_AXL
AXL_PY_PREPARE = prepare_files
endif

bin_SCRIPTS = $(AXL_PY_PREPARE)

test_01_SOURCES = test_01.c
test_01_LDADD   = $(top_builddir)/src/libaxl.la $(NS_LIB) $(BABEL_LIB)

test_01a_SOURCES = test_01a.c
test_01a_LDADD   = $(top_builddir)/src/libaxl.la 

test_02a_SOURCES = test_02a.c
test_02a_LDADD   = $(top_builddir)/src/libaxl.la 

test_03a_SOURCES = test_03a.c
test_03a_LDADD   = $(top_builddir)/src/libaxl.la

# copy all test files 
prepare_files: 
	test -e test.xml || cp $(top_srcdir)/test/*.xml $(top_srcdir)/test/*.xdl $(top_srcdir)/test/*.dtd .
	@cd ../py-axl; make; cd -
	@test -d axl || mkdir axl
        # copy dynamic libraries
	@cp `find ../py-axl -name libpy_axl$(SHARED_EXTENSION) -type l` axl/
        # copy python imports
	@test -e axl/__init__.py || cp ../py-axl/__init__.py axl/
	@diff axl/__init__.py ../py-axl/__init__.py || cp ../py-axl/__init__.py axl/

refresh: 
	$(top_builddir)/knife/axl-knife --input fact.dtd --dtd-to-c --output fact.dtd.h --ifnewer
	$(top_builddir)/knife/axl-knife --input channel.dtd --dtd-to-c --output channel.dtd.h  --ifnewer
	$(top_builddir)/knife/axl-knife --input xml-rpc.dtd --dtd-to-c --output xml-rpc.dtd.h  --ifnewer
	$(top_builddir)/knife/axl-knife --input tls.dtd --dtd-to-c --output tls.dtd.h  --ifnewer

leak-check:
	libtool --mode=execute valgrind --leak-check=yes ./test_01

compref:
	gcc -g -Wall `pkg-config --libs --cflags libxml-2.0` test_01b.c -o test_01b
	gcc -g -Wall `pkg-config --libs --cflags libxml-2.0` test_02b.c -o test_02b
	gcc -g -Wall  `pkg-config --libs --cflags glib-2.0` -I$(top_srcdir)/src $(top_builddir)/src/.libs/libaxl.so test_03c.c -o test_03c

