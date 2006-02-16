EXTRA_DIST = $(wildcard *.c)

all: $(patsubst %.c, %.exe, $(EXTRA_DIST))

%.o:
	$(CC) $(patsubst %.o, %.c, $@) -c -o $@ -I../src/

%.exe: %.o
	$(CC) $? -o $@ -L../src/ 

.PHONY: clean
clean: 
	rm -f *.o *.exe



