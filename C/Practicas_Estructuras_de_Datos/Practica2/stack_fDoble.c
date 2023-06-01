#include <stdlib.h>
#include <stdio.h>
#include "stack_fDoble.h"

#define INIT_CAPACITY 2 // init stack capacity
#define FCT_CAPACITY 2 // multiply the stack capacity

struct _Stack {
    void **item; /*!<Dynamic array of elements*/
    int top; /*!<index of the top element in the stack*/
    int capacity; /*!<xcapacity of the stack*/
};

//private functions declaration
Bool _stack_isFull(Stack *s);
//end of private functions declaration

Stack * stack_init(){
    Stack *s;
    int i;

    //reservamos memoria para un stack
    s=(Stack*) malloc(sizeof(Stack));
    if(s==NULL) return NULL;

    /*reservamos memoria para el array item, que va a tener como
    capacidad inicial el tamaño INIT_CAPACITY
    */
    s->item = (void **) malloc(INIT_CAPACITY*sizeof(void*));
    if(s->item==NULL){
        free(s);
        return NULL;
    }

    //Inicializamos todos los elementos del array item a NULL
    for(i=0;i<INIT_CAPACITY;i++){
        s->item[i]=NULL;
    }

    //guardamos la capacidad del array e inicializamos top a -1 por ser un stack vacío
    s->capacity=INIT_CAPACITY;
    s->top=-1;

    return s;
}

void stack_free(Stack *s){
    //comprobamos si el stack existe
    if(s==NULL) return;

    //liberamos el array item (no sus elementos) y el stack
    free(s->item);
    free(s);
}

Status stack_push (Stack *s, const void *ele){
    if(!s||!ele) return ERROR;
    int i;

    /*comprobamos si el array del stack está lleno, de ser así aumentamos 
    FCT_CAPACITY veces su tamaño e inicializamos los nuevos elementos del
    array a NULL*/
    if(_stack_isFull(s)==TRUE){
        s->item=(void**) realloc(s->item,sizeof(void*)*FCT_CAPACITY*s->capacity);
        if(s->item==NULL) return ERROR;
        s->capacity*=FCT_CAPACITY;
        for(i=s->top+1; i < s->capacity;i++){
            s->item[i]=NULL;
        }
    }

    //guardamos ele en la posición del array posterior a top y aumentamos en uno top
    s->item[s->top+1]=(void*)ele;
    s->top++;

    return OK;
}

void * stack_pop (Stack *s){
    if(!s || stack_isEmpty(s)==TRUE) return NULL;

    void *aux;

    //guardamos en aux el elemento del array item que está en la posición top
    aux=s->item[s->top];
    //igualamos el elemento del array item que está en la posición top
    s->item[s->top]=NULL;
    //decrementamos en uno top
    s->top--;

    //devolvemos aux
    return aux;
}

void * stack_top (const Stack *s){
    if(!s) return NULL;

    /*si el stack existe, devolvemos el elemento del array item 
    que está guardado en la posición top*/
    return s->item[s->top];
}

Bool stack_isEmpty (const Stack *s){
    if(!s) return TRUE;

    //devuelve verdadero si s->top vale -1 y falso en caso contrario
    return s->top ==-1 ? TRUE : FALSE;
}

size_t stack_size (const Stack *s){
    if(!s) return -1;

    /*Devuelve el número de elementos guardados en el array item
    como los arrays tiene índices de 0 a N-1, siendo N el tamaño
    del array, el tamaño de item es el índice del último elemento
    no nulo más uno*/
    return (size_t) (s->top+1);
}

int stack_print(FILE* fp, const Stack *s,  P_stack_ele_print f){
    if(!fp || !s) return -1;
    int i,aux,ret=0;

    /*intenta imprimir el tamaño del stack, si la impresión falla devuelve -1,
    si no, suma el número de caracteres que a impreso a ret*/
    aux=fprintf(fp,"SIZE: %ld\n",stack_size(s));
    if(aux<0) return-1;
    ret+=aux;

    /*intenta imprimir todos los elementos del array item, si la impresión falla 
    devuelve -1, si no, suma el número de caracteres que a impreso a ret*/
    for(i=s->top;i>=0;i--){
        aux=f(fp,s->item[i]);
        if(aux<0) return-1;
        ret+=aux;

        aux=fprintf(fp,"\n");
        if(aux<0) return-1;
        ret+=aux;
    }

    //devuelve ret
    return ret;
}

//private functions
Bool _stack_isFull(Stack *s){
    if(s==NULL) return TRUE;

    /*si s->top vale la capacidad del array item -1, significa que está lleno y 
    devuelve TRUE, en caso contrario devuelve FALSE*/
    if(s->top==(s->capacity-1)) return TRUE;

    return FALSE;
}