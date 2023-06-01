#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h> 
#include "stack_fDoble.h"

#define MAX_RAND 11

Stack* stack_order (Stack *s, int (*f_cmp)(const void *, const void *));
int int_cmp(const void *c1, const void *c2);
int int_print(FILE *pf,const void *ele);

int main(int argc, char *argv[]){

    //comprobamos si el número de argumentos es el indicado
    if(argc!=2){
        fprintf(stderr,"Numero de argumentos invalido. modo de uso: ./p2_e1c <NUM_ENTEROS>.\n");
        return 1;
    }

    int i,num,*array;
    Stack *sini;
    Stack *sfin;

    //pasamos el segundo argumento de char* a int
    num=atoi(argv[1]);

    /*creamos un array de num enteros siendo num, el número
    de puntos (argumento 2 de la función)*/
    array=(int*)malloc(num*sizeof(int));
    if(array==NULL) return 1;

    //creamos un stack
    sini=stack_init();
    if(sini==NULL){
        free(array);
        return 1;
    }

    // iniciamos una "semilla" aleatoria
    srand(time(NULL));
    for (i = 0; i < num; i++)
    {
        /*creamos un entero aleatorio entre 0 y MAX_RAND, y lo guardamos
        en el array de enteros*/
        array[i] = rand() % MAX_RAND;

        //guardamos el entero en el stack sini
        stack_push(sini,&array[i]);
    }

    //imprimimos el stack sini sin ordenar
    fprintf(stdout,"Original stack:\n");

    stack_print(stdout,sini,int_print);

    /*ordenamos sini con la función stack_order e igualamos
    su retorno a la variable de tipo Stack* sfin, después imprimimos
    el stack ordenado sfin*/

    sfin=stack_order(sini,int_cmp);

    fprintf(stdout,"Ordered stack:\n");

    stack_print(stdout,sfin,int_print);

    //imprimimos el stack original que ahora está vacío (ver stack_order)
    fprintf(stdout,"Original stack:\n");

    stack_print(stdout,sini,int_print);

    //liberamos los stacks
    stack_free(sini);
    stack_free(sfin);

    //liberamos el array de enteros
    free(array);

    return 0;
}

/**
 * @brief ordena los elementos del stack s en el stack sout
 * 
 * @param s 
 * @param f_cmp funcion para comparar los elementos del stack
 * @return Stack* sout
 */
Stack* stack_order (Stack *s, int (*f_cmp)(const void *, const void *)){
    if(!s) return NULL;
    Stack *sout;
    void *aux1,*aux2;

    //creamos un stack para el retorno
    sout=stack_init();

    //bucle hasta que el stack pasado por parámetro sin esté vacío
    while(stack_isEmpty(s)==FALSE){
        /*extraemos el último elemento del stack y se lo asignamos a la
        variable auxiliar aux1*/ 
        aux1=stack_pop(s);

        /*bucle que se ejecuta hasta que sout esté vacío o el elemento aux1 sea
        mayor que el último elemento del stack*/
        while(stack_isEmpty(sout)==FALSE && f_cmp(aux1,stack_top(sout))<0){
            //extraemos el último elemento del stack sout y lo introducimos en sin
            aux2= stack_pop(sout);
            if(stack_push(s,aux2)==ERROR) break;
        }

        /*introducimos el elemento aux1 en sout una vez que hemos comprobado que es el
        más pequeño de los que quedaban por comprobar*/
        stack_push(sout,aux1);
    }

    return sout;
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

    return fprintf(pf,"%d",*(int*)ele);
}