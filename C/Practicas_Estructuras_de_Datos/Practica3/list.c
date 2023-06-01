#include "list.h"

struct _Node {
    void * info ;
    struct _Node * next ;
};

typedef struct _Node Node ;

struct _List {
    Node *first ;
    Node *last;
    int size;
};


//Private functions
void _list_free_rec(Node* pn);

//Public functions

Node * node_create(){
    Node * newnode;

    newnode=(Node*)malloc(sizeof(Node));
    if(newnode==NULL) return NULL;

    newnode->info=NULL;
    newnode->next=NULL;

    return newnode;
}

List *list_new(){
    List * newlist;

    newlist=(List*)malloc(sizeof(List));
    if(newlist==NULL) return NULL;

    newlist->first=NULL;
    newlist->last=NULL;
    newlist->size=0;

    return newlist;
}

Bool list_isEmpty(const List *pl){
    if(pl==NULL || pl->first==NULL) return TRUE;

    return FALSE;
}

Status list_pushFront(List *pl, const void *e){
    if(pl==NULL || e==NULL) return ERROR;

    Node *pn;

    pn=node_create();

    if(pn==NULL) return ERROR;

    pn->info=(void*)e;

    pn->next=pl->first;

    if(list_isEmpty(pl)==TRUE){
        pl->last=pn;
    }

    pl->first=pn;

    pl->size++;

    return OK;
}

Status list_pushBack(List *pl, const void *e){
    if(pl==NULL || e==NULL) return ERROR;

    Node *pn;

    pn=node_create();

    if(pn==NULL) return ERROR;

    pn->info=(void*)e;

    if(list_isEmpty(pl)==TRUE){
        pl->first=pn;
    }else{
        pl->last->next=pn;
    }

    pl->last=pn;

    pl->size++;

    return OK;
}

void *list_popFront(List *pl){
    if(pl==NULL||list_isEmpty(pl)==TRUE) return NULL;

    void *ele=NULL;
    Node *aux=NULL;

    aux=pl->first;
    ele=(void*)aux->info;

    if(pl->last==pl->first){
        pl->last=NULL;
        pl->first=NULL;
    }else{
        pl->first=aux->next;
    }

    free(aux);

    pl->size--;

    return ele;
}

void *list_popBack(List *pl){
    if(pl==NULL||list_isEmpty(pl)==TRUE) return NULL;

    void *ele=NULL;
    Node *aux=NULL;

    ele=(void*)pl->last->info;

    if(pl->last==pl->first){
        free(pl->last);
        pl->last=NULL;
        pl->first=NULL;
        pl->size--;
        return ele;
    }

    aux=pl->first;

    while(aux->next!=pl->last){
        aux=aux->next;
    }

    free(pl->last);

    pl->last=aux;

    pl->size--;

    return ele;
}

void *list_getFront(List *pl){
    if(pl==NULL || list_isEmpty(pl)==TRUE) return NULL;

    return pl->first->info;
}

void *list_getBack(List *pl){
    if(pl==NULL || list_isEmpty(pl)==TRUE) return NULL;

    return pl->last->info;
}

void list_free(List *pl){
    if(pl==NULL) return;

    Node *aux;

    aux=pl->first;

    free(pl);

    _list_free_rec(aux);
}

int list_size(const List *pl){
    if(!pl) return -1;

    return pl->size;
}

int list_print(FILE *fp, const List *pl, p_list_ele_print f){
    if (!fp||!pl||!f||list_isEmpty(pl)==TRUE) return -1;

    int aux,ret=0;

    Node* naux;

    naux=pl->first;

    while(naux!=NULL){
        aux=f(fp,naux->info);

        if(aux==-1) return aux;

        ret+=aux;

        naux=naux->next;
    }

    return ret;
    
}

//Private functions implementation
void _list_free_rec(Node* pn){
    if(pn==NULL) return;

    Node *aux;

    aux=pn->next;
    free(pn);
    _list_free_rec(aux);
}