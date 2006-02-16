EXTRA_DIST = $(wildcard *.c)

all: $(patsubst %.c, %.exe, $(EXTRA_DIST))

%.o: %.c
	libtool --mode=compile $(CC) -g -Wall $(patsubst %.o, %.c, $@) -c -o $@ -I../src/

%.exe: %.o
	libtool --mode=link $(CC) $< -g -Wall -o $@ ../src/libaxl.la

clean:
	rm -rf *.o *.exe


