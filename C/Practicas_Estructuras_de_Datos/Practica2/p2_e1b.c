#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h> 
#include "stack_fDoble.h"
#include "point.h"

#define MAX_RAND 10

Stack *stack_orderPoints(Stack *sin);

int main(int argc, char *argv[]){

    //comprobamos si el número de argumentos es el indicado
    if(argc!=2){
        fprintf(stderr,"Numero de argumentos invalido. modo de uso: ./p2_e1b <NUM_PUNTOS>.\n");
        return 1;
    }


    Point **p;
    Point *origen;
    int i,j,num;
    double d;
    Stack *sini;
    Stack *sfin;

    //pasamos el segundo argumento de char* a int
    num=atoi(argv[1]);

    /*creamos un array de num punteros a punto siendo num,
    el número de puntos (argumento 2 de la función)*/
    p=(Point**)malloc(num*sizeof(Point*));
    if(p==NULL) return 1;

    //creamos un stack
    sini=stack_init();
    if(sini==NULL){
        free(p);
        return 1;
    }

    // iniciamos una "semilla" aleatoria
    srand(time(NULL));
    // creamos una coordenada al punto de origen
    origen = point_new(0, 0, BARRIER);
    for (i = 0; i < num; i++)
    {
        /*creamos un punto con coordenadas aleatorias entre 0
        y MAX_RAND, y lo guardamos en el array de punteros a punto
        */
        p[i] = point_new(rand() % MAX_RAND, rand() % MAX_RAND,
                         BARRIER);
        if(!p[i]){
            for(j=i-1;j>=0;j--){
                point_free(p[j]);
            }
        }

        /*imprimimos que punto del array es al que nos referimos
        y su distancia al origen*/
        fprintf(stdout,"Point p[%d]=",i);
        point_print(stdout, p[i]);
        point_euDistance(p[i], origen, &d);
        fprintf(stdout, " distance: %lf\n", d);

        //guardamos el punto en el stack sini
        stack_push(sini,p[i]);
    }

    //imprimimos el stack sini sin ordenar
    fprintf(stdout,"Original stack:\n");

    stack_print(stdout,sini,point_print);

    /*ordenamos sini con la función stack_orderPoints e igualamos
    su retorno a la variable de tipo Stack* sfin, después imprimimos
    el stack ordenado sfin*/
    sfin=stack_orderPoints(sini);

    fprintf(stdout,"Ordered stack:\n");

    stack_print(stdout,sfin,point_print);

    //imprimimos el stack original que ahora está vacío (ver stack_orderPoints)
    fprintf(stdout,"Original stack:\n");

    stack_print(stdout,sini,point_print);

    //liberamos los stacks
    stack_free(sini);
    stack_free(sfin);

    //liberamos los puntos del array
    for(i=0;i<num;i++){
        point_free(p[i]);
    }

    //liberamos el array de punteros a punto
    free(p);
    //liberamos el punto de origen que hemos creado
    point_free(origen);

    return 0;
}

/**
 * @brief ordena un stack de puntos según su distancia euclidea
 * 
 * @param sin 
 * @return sout  
 */
Stack * stack_orderPoints(Stack *sin){
    if(!sin) return NULL;
    Stack *sout;
    Point *p1,*p2;

    //creamos un stack para el retorno
    sout=stack_init();

    //bucle hasta que el stack pasado por parámetro sin esté vacío
    while(stack_isEmpty(sin)==FALSE){
        /*extraemos el último punto del stack y se lo asignamos a la
        variable auxiliar p1*/ 
        p1=(Point*) stack_pop(sin);

        /*bucle que se ejecuta hasta que sout esté vacío o el punto p1 sea
        mayor que el último elemento del stack*/
        while(stack_isEmpty(sout)==FALSE && point_cmpEuDistance(p1,stack_top(sout))<0){
            //extraemos el último punto del stack sout y lo introducimos en sin
            p2=(Point*) stack_pop(sout);
            if(stack_push(sin,p2)==ERROR) break;
        }
        
        /*introducimos el punto p1 en sout una vez que hemos comprobado que es el
        más pequeño de los que quedaban por comprobar*/
        stack_push(sout,p1);
    }

    return sout;
}