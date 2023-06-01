#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "sorted_queue.h"
#include "point.h"

#define MAX_ELEM 10

int str_print(FILE*pf,const void *string);
int str_cmp(const void *ele1, const void *ele2);
int int_cmp(const void *c1, const void *c2);
int int_print(FILE *pf,const void *ele);


int main(int argc, char const *argv[]){

    SortedQueue *intq=NULL, *charq=NULL, *ranintq=NULL,*rancharq=NULL,*pointq=NULL;
    int i,indice;
    
    //nombres que vamos a introducir en la cola que queremos ordenar
    char *nombres[MAX_ELEM]={"Alberto","Javier","Aitana","Ruben","Luis","Alma","Cecilia","Laura","Jesus","Antonio"};
    //números que vamos a introducir en la cola que queremos ordenar
    int nums[MAX_ELEM]={12,15,23,45,11,6,80,7,31,10};
    Point *p[MAX_ELEM];

    //iniciamos "semilla" aleatoria
    srand(time(NULL));


    //Creamos la "cola de enteros ordenada"
    intq=squeue_new();
    if(intq==NULL){
        fprintf(stderr,"Error al crear cola de enteros.\n");
        return 1;
    }

    //Introducimos todos los enteros del array en la "cola de enteros ordenada"
    for(i=0;i<MAX_ELEM;i++){
        if(squeue_push(intq,&nums[i],int_cmp)==ERROR){
            squeue_free(intq);
            fprintf(stderr,"Error en squeue_push con enteros.\n");
        }
    }

    //Imprimimos por pantalla la "cola de enteros ordenada"
    squeue_print(stdout,intq,int_print);

    fprintf(stdout,"\n");

    //Creamos la "cola de cadena de caracteres ordenada"
    charq=squeue_new();
    if(charq==NULL){
        fprintf(stderr,"Error al crear cola de cadenas de caracteres.\n");
        return 1;
    }

    //Introducimos todos los nombres del array en la "cola de cadena de caracteres ordenada"
    for(i=0;i<MAX_ELEM;i++){
        if(squeue_push(charq,nombres[i],str_cmp)==ERROR){
            squeue_free(charq);
            fprintf(stderr,"Error en squeue_push con cadena de caracteres.\n");
        }
    }

    //Imprimimos por pantalla la "cola de cadena de caracteres ordenada"
    squeue_print(stdout,charq,str_print);

    fprintf(stdout,"\n");
    
    pointq=squeue_new();
    if(pointq==NULL){
        fprintf(stderr,"Error al crear cola de puntos.\n");
        return 1;
    }
    for (i = 0; i < MAX_ELEM; i++){
        indice=random()%MAX_ELEM;
        p[i]=point_new(indice,indice+2,SPACE);
        if (squeue_push(pointq, p[i], point_cmpEuDistance) == ERROR){
            squeue_free(pointq);
            fprintf(stderr, "Error en squeue_push con cadena de caracteres.\n");
        }
    }

    squeue_print(stdout,pointq,point_print);

    fprintf(stdout,"\n");

    /*Probamos a realizar el mismo test que hasta ahora pero ahora introduciendo
    los elementos de cada array de manera aleatoria permitiendo así que haya elementos
    de la cola repetidos y comprobamos si la función continua ordenando bien los elementos*/

    ranintq=squeue_new();
    if(ranintq==NULL){
        fprintf(stderr,"Error al crear cola de enteros aleatoria.\n");
        return 1;
    }

    for(i=0;i<MAX_ELEM;i++){
        indice=random()%MAX_ELEM;
        if(squeue_push(ranintq,&nums[indice],int_cmp)==ERROR){
            squeue_free(intq);
            fprintf(stderr,"Error en squeue_push con enteros aleatorios.\n");
        }
    }

    squeue_print(stdout,ranintq,int_print);

    fprintf(stdout,"\n");

    rancharq=squeue_new();
    if(rancharq==NULL){
        fprintf(stderr,"Error al crear cola de cadenas de caracteres aleatoria.\n");
        return 1;
    }

    for(i=0;i<MAX_ELEM;i++){
        indice=random()%MAX_ELEM;
        if(squeue_push(rancharq,nombres[indice],str_cmp)==ERROR){
            squeue_free(charq);
            fprintf(stderr,"Error en squeue_push con cadena de caracteres aleatoria.\n");
        }
    }

    squeue_print(stdout,rancharq,str_print);

    squeue_free(intq);
    squeue_free(charq);
    squeue_free(pointq);
    squeue_free(ranintq);
    squeue_free(rancharq);

    return 0;
}

int str_print(FILE*pf,const void *string){
    if(!string || !pf) return -1;

    return fprintf(pf,"%s ",(char*)string);
}

int str_cmp(const void *ele1, const void *ele2){
    char *str1, *str2;

    str1=(char*) ele1;
    str2=(char*) ele2;

    return strcmp(str1,str2);
}

/**
 * @brief devuelve la diferencia entre c1 y c2
 * 
 * @param c1 
 * @param c2 
 * @return int 
 */
int int_cmp(const void *c1, const void *c2) {
    if (!c1 || !c2)
        return INT_MIN;

    return (*(int *)c1 - *(int *)c2);
}

/**
 * @brief imprime el elemento ele (un entero) en el archivo pasado por pf
 * 
 * @param pf 
 * @param ele 
 * @return int 
 */
int int_print(FILE *pf,const void *ele){
    if(!pf||!ele) return -1;

    return fprintf(pf,"%d ",*(int*)ele);
}