CC=gcc
CFLAGS=-Wall -pedantic -ggdb
IFLAGS=-I./
LDFLAGS=-L./

LIBS = -lm -pthread

all: miner monitor

pow.o: pow.c pow.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS) 

miner: miner.o min_comp.o pow.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)
monitor: monitor.o min_comp.o pow.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)
miner.o: miner.c min_comp.h pow.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS) 
min_comp.o:min_comp.c min_comp.h pow.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS) 
monitor.o:monitor.c min_comp.h pow.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS) 

clear:
	rm *o
clean: clear
	rm -f *.txt && rm -f miner