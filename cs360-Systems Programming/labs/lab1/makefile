C = gcc 
INCLUDES = -I/home/jplank/cs360/include
CFLAGS = $(INCLUDES)
LIBDIR = /home/jplank/cs360/lib
LIBS = $(LIBDIR)/libfdr.a 

EXECUTABLES = bin/famtree

all: $(EXECUTABLES)

bin/famtree: src/famtree.c
	$(CC) $(CFLAGS) -g -o bin/famtree src/famtree.c $(LIBS)

clean:
	rm core $(EXECUTABLES) *.o
