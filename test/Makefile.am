EXTRA_DIST      = $(wildcard *.c) $(wildcard *.dtd) $(wildcard *.xml) $(wildcard *.xdl) Makefile.win

INCLUDES = -I../src/ -Wall

noinst_PROGRAMS = test_01 test_02 test_01a test_02a

test_01_SOURCES = test_01.c
test_01_LDADD   = ../src/libaxl.la 

test_02_SOURCES = test_02.c
test_02_LDADD   = ../src/libaxl.la

test_01a_SOURCES = test_01a.c
test_01a_LDADD   = ../src/libaxl.la 

test_02a_SOURCES = test_02a.c
test_02a_LDADD   = ../src/libaxl.la 

compref:
	gcc `pkg-config --libs --cflags libxml-2.0` test_01b.c -o test_01b
	gcc `pkg-config --libs --cflags libxml-2.0` test_02b.c -o test_02b

