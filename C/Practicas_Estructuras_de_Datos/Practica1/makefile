CC=gcc
CFLAGS=-Wall -pedantic -ggdb
IFLAGS=-I./
LDFLAGS=-L./

all: p1_e1 p1_e2 p1_e3
######################################################################
# $@ es el item que aparece a la izquierda de ':' 
# $< es el primer item en la lista de dependencias
# $^ son todos los archivos que se encuentran a la derecha de ':' (dependencias)
########################################################################
point.o: point.c point.h types.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)
map.o: map.c map.h types.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)
	
p1_e1: p1_e1.o point.o
	$(CC) -o $@ $^ $(LDFLAGS)
p1_e1.o: p1_e1.c point.h types.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

p1_e2: p1_e2.o map.o point.o
	$(CC) -o $@ $^ $(LDFLAGS)
p1_e2.o: p1_e2.c point.h map.h types.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)	

p1_e3: p1_e3.o map.o point.o
	$(CC) -o $@ $^ $(LDFLAGS)
p1_e3.o: p1_e3.c point.h map.h types.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)
clean:
	rm *o