#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "list.h"

struct _Queue {
    List *pl;
};

Queue *queue_new(){
    Queue *newq;

    newq=(Queue*) malloc(sizeof(Queue));

    newq->pl=list_new();

    if(newq->pl==NULL){
        free(newq);
        return NULL;
    }

    return newq;
}

void queue_free(Queue *q){
    if(q==NULL) return;

    list_free(q->pl);

    free(q);
}

Bool queue_isEmpty(const Queue *q){
    if(q==NULL || list_isEmpty(q->pl)) return TRUE;

    return FALSE;
}

Status queue_push(Queue *q, void *ele){
    if(!q || !ele) return ERROR;

    return list_pushBack(q->pl,ele);
}

void *queue_pop(Queue *q){
    if(!q) return NULL;

    return list_popFront(q->pl);
}

void *queue_getFront(const Queue *q){
    if(!q) return NULL;

    return list_getFront(q->pl);
}

void *queue_getBack(const Queue *q){
    if(!q) return NULL;

    return list_getBack(q->pl);
}

size_t queue_size(const Queue *q){
    if(!q) return -1;

    return (size_t) list_size(q->pl);
}

int queue_print(FILE *fp, const Queue *q, p_queue_ele_print f){
    if(!fp||!q||!f) return -1;

    return list_print(fp,q->pl,f);
}