CC=gcc
CFLAGS=-Wall -pedantic -ggdb
IFLAGS=-I./
LDFLAGS=-L./
LDLIBS=-lstack_fDoble

LIBS = -lm -pthread

all: p2_e1a p2_e1b p2_e1c p2_e2 p2_e1aprueba p2_e1bprueba p2_e1cprueba p2_e2prueba
######################################################################
# $@ es el item que aparece a la izquierda de ':' 
# $< es el primer item en la lista de dependencias
# $^ son todos los archivos que se encuentran a la derecha de ':' (dependencias)
########################################################################

# compilamos el tad point y sus funciones
point.o: point.c point.h types.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

# compilamos el tad map y sus funciones
map.o: map.c map.h types.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

# compilamos nuestra implementación del tad stack (Ejercicio 3) y sus funciones
stack.o: stack_fDoble.c stack_fDoble.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

######################################################################
# Los programas que tienen por nombre solo el ejercicio que compilan,
# usan la implementación de stack que hemos descargado de moodle. Mientras
# que los programas que terminan con la palabra prueba, usan nuestra imple-
# mentación de stack realizada en el ejercicio 3.
#
# Ambos tipos usan el mismo .o de los programas .c, por ejemplo, tanto
# p2_e1a como p2_e1aprueba, usan p2_e1a.o para compilarse, este p2_e1a.o
# es el resultado de una única implementación y compilación de p2_e1a.c
######################################################################	

p2_e1a: p2_e1a.o libstack_fDoble.a point.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)

p2_e1b: p2_e1b.o libstack_fDoble.a point.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)

p2_e1c: p2_e1c.o libstack_fDoble.a 
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)

p2_e2: p2_e2.o map.o point.o libstack_fDoble.a
	$(CC) -o $@ $^ $(LDFLAGS)  $(LDLIBS) $(LIBS)

p2_e1aprueba: p2_e1a.o point.o stack.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

p2_e1bprueba: p2_e1b.o point.o stack.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

p2_e1cprueba: p2_e1c.o stack.o 
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

p2_e2prueba: p2_e2.o map.o point.o stack.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

p2_e1a.o: p2_e1a.c point.h stack_fDoble.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

p2_e1b.o: p2_e1b.c point.h stack_fDoble.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

p2_e1c.o: p2_e1c.c stack_fDoble.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

p2_e2.o: p2_e2.c map.h stack_fDoble.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)
clean:
	rm *o