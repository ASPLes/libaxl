EXTRA_DIST = $(wildcard *.c)

all: $(patsubst %.c, %.exe, $(EXTRA_DIST))

%.o:
	libtool --mode=compile $(CC) $(patsubst %.o, %.c, $@) -c -o $@ -I../src/

%.exe: %.o
	libtool --mode=link  $(CC) $? -o $@ ../src/libaxl.la

.PHONY: clean
clean: 
	rm -f *.o *.exe



