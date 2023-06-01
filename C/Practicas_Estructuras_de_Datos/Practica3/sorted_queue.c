#include "sorted_queue.h"
#include <stdio.h>
#include <stdlib.h>


//--------------------------------------------------------------------------
SortedQueue *squeue_new() {
  return queue_new();  
}

//--------------------------------------------------------------------------
void squeue_free(SortedQueue *q) {
  queue_free(q);
}

/**
 * @brief  Inserta un elemento en la cola ordenada.
 *
 * TO-DO: 
 * input: Queue q 
 * input: ele
 * input: pcmp
 *
 * output: Status st
 *
 * aux
 * st=OK
 * 
 * if queue_isEmpty(q)=TRUE then
 *   queue_push(q,ele)
 * end
 * 
 * while pcmp(queue_getFront(q),ele) <0 do
 *   aux=queue_pop(q);
 *   queue_push(q,aux)
 * end
 * 
 * queue_push(q,ele)
 * 
 * while pcmp(queue_getFront(q),ele) >=0 do
 *   aux=queue_pop(q);
 *   queue_push(q,aux)
 * end
 * 
 * return st
 * 
 * @param q, puntero a la cola.
 * @param ele, puntero al elemento a insertar.
 * @param pcmp, puntero a la funcion de comparacion.
 *
 * @return OK si la insercion se realiza con exito, ERROR en caso contrario.
 **/
Status squeue_push(SortedQueue *q, void *ele, p_queue_ele_cmp pcmp) {
  Status st = OK;

  if(!q || !ele || !pcmp) return ERROR;

  void *aux=NULL;
  int i=0,size=0;

  if(squeue_isEmpty(q)==TRUE){
    if(queue_push(q,ele)==ERROR) return ERROR;
    return st;
  }

  size=squeue_size(q);

  for(i=0;st==OK && i<size;i++){
    if(pcmp(squeue_getFront(q),ele)<0){
      aux=(void*)squeue_pop(q);
      if(aux==NULL) return ERROR;
      st=queue_push(q,aux);
    }else{
      break;
    }
  }

  if(st==ERROR) return st;

  if(queue_push(q,ele)==ERROR) return ERROR;

  for(i=0;st==OK && i<size;i++){
    if(pcmp(squeue_getFront(q),ele)>=0){
      aux=(void*)squeue_pop(q);
      if(aux==NULL) return ERROR;
      st=queue_push(q,aux);
    }else{
      break;
    }
  }

  return st;
}

//--------------------------------------------------------------------------
void *squeue_pop(SortedQueue *q) {
  return queue_pop(q);
}

//--------------------------------------------------------------------------
void *squeue_getFront(const SortedQueue *q) {
  return queue_getFront(q);
}

//--------------------------------------------------------------------------
void *squeue_getBack(const SortedQueue *q) {
  return queue_getBack(q);
}

//--------------------------------------------------------------------------
Bool squeue_isEmpty(const SortedQueue *q) {
  return queue_isEmpty(q);
}

//--------------------------------------------------------------------------
size_t squeue_size(const SortedQueue *q) {
  return queue_size(q);
}

//--------------------------------------------------------------------------
int squeue_print(FILE *fp, const SortedQueue *q, p_queue_ele_print f) {
  return queue_print(fp, q, f);
}

