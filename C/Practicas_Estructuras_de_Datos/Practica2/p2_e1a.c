#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h> 
#include "point.h"

#define MAX_RAND 100


int main(int argc, char *argv[]){

    //comprobamos si el número de argumentos es el indicado
    if(argc!=2){
        fprintf(stderr,"Numero de argumentos invalido, modo de uso: ./p2_e1a <NUM_PUNTOS>.\n");
        return 1;
    }


    Point **p;
    Point *origen;
    int i,j,num,res;
    double d;

    //pasamos el segundo argumento de char* a int
    num=atoi(argv[1]);

    /*creamos un array de num punteros a punto siendo num,
    el número de puntos (argumento 2 de la función)*/
    p=(Point**)malloc(num*sizeof(Point*));
    if(p==NULL) return 1;

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
            free(p);
            return 1;
        }
        /*imprimimos que punto del array es al que nos referimos
        y su distancia al origen*/
        fprintf(stdout,"Point p[%d]=",i);
        point_print(stdout, p[i]);
        point_euDistance(p[i], origen, &d);
        fprintf(stdout, " distance: %lf\n", d);
    }

    /*comparamos cada punto del array consigo mismo y con cada uno
    de los puntos del array con indices superiores al suyo e imprime
    si es verdadero o falso que este punto es más pequeño*/
    for(i=0;i<num;i++){
        for(j=i;j<num;j++){
            res=point_cmpEuDistance(p[i],p[j]);
            if(res==INT_MIN) return 1;
            fprintf(stdout,"p[%d] < p[%d]:",i,j);
            if(res<0){
                fprintf(stdout,"True\n");
            }else{
                fprintf(stdout,"False\n");
            }
        }
    }

    //liberamos los puntos del array
    for(i=0;i<num;i++){
        point_free(p[i]);
    }

    //liberamos el punto de origen que hemos creado
    point_free(origen);

    //liberamos el array de punteros a punto
    free(p);

    return 0;
}

