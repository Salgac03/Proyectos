CC=gcc
CFLAGS=-Wall -pedantic -ggdb
IFLAGS=-I./
LDFLAGS=-L./
LDLIBS=libqueue.a libstack_fDoble.a

LIBS = -lm -pthread

all: p3_e1 p3_e2 p3_e1prueba p3_e2prueba
######################################################################
# $@ es el item que aparece a la izquierda de ':' 
# $< es el primer item en la lista de dependencias
# $^ son todos los archivos que se encuentran a la derecha de ':' (dependencias)
########################################################################

# compilamos el tad point y sus funciones
point.o: point.c point.h types.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

# compilamos el tad map y sus funciones
map.o: map.c map.h types.h queue.h stack_fDoble.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

# compilamos nuestra implementación del tad stack (Ejercicio 3) y sus funciones
sorted_queue.o: sorted_queue.c sorted_queue.h queue.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS) 

list.o: list.c list.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

queue.o: queue.c queue.h
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

p3_e1: p3_e1.o sorted_queue.o point.o libqueue.a
	$(CC) -o $@ $^ $(LDFLAGS)  $(LDLIBS) $(LIBS)

p3_e1prueba: p3_e1.o sorted_queue.o queue.o list.o point.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)

p3_e1.o: p3_e1.c point.h sorted_queue.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

p3_e2: p3_e2.o map.o point.o libqueue.a libstack_fDoble.a
	$(CC) -o $@ $^ $(LDFLAGS)  $(LDLIBS) $(LIBS)

p3_e2prueba: p3_e2.o sorted_queue.o queue.o list.o map.o point.o libstack_fDoble.a
	$(CC) -o $@ $^ $(LDFLAGS)  $(LDLIBS) $(LIBS)

p3_e2.o: p3_e2.c map.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)
clear:
	rm *o
clean: clear
	rm -f p3_e1 p3_e2 listprueba p3_e1prueba p3_e2prueba
