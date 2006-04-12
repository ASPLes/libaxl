EXTRA_DIST      = $(wildcard *.c)

INCLUDES = -I../src/ -pg -Wall

bin_PROGRAMS    = test_01 test_02

test_01_SOURCES = test_01.c
test_01_LDADD   = ../src/libaxl.la 

test_02_SOURCES = test_02.c
test_02_LDADD   = ../src/libaxl.la