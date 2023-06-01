CC=gcc
CFLAGS=-Wall -pedantic -ggdb
IFLAGS=-I./
LDFLAGS=-L./
LIBS= -lm

all: p4 p4alternativo

p4: p4.o point.o bstree.o
	$(CC) -o $@ $^ $(IFLAGS) $(LIBS)

p4alternativo: p4.o bstreealternativo.o point.o
	$(CC) -o $@ $^ $(IFLAGS) $(LIBS)

p4.o: p4.c bstree.h point.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

point.o: point.c point.h types.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

bstree.o: bstree.c bstree.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

bstreealternativo.o: bstreealternativo.c bstree.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

clear:
	rm -f *.o

clean: clear
	rm -f p4 p4alternativo