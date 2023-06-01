#include <stdio.h>
#include <stdlib.h>
#include "map.h"

#define MAX_NCOLS 64 // Maximum map cols
#define MAX_NROWS 64 // Maximum map rows
#define MAX_BUFFER 64 // Maximum file line size

struct _Map {
    unsigned int nrows, ncols;
    Point *array[MAX_NROWS][MAX_NCOLS]; // array with the Map points
    Point *input, *output; // points input/output
};

/*Private functions headers*/
Bool position_exist(const Map *mp,const Point *p);

/*Public functions*/

Map * map_new (unsigned int nrows,  unsigned int ncols){
    if(nrows<0||ncols<0) return NULL;

    int i,j;
    Map * newmap;

    newmap=(Map*) malloc(sizeof(Map));

    if(newmap==NULL) return NULL;

    for(i=0;i<nrows;i++){
        for(j=0;j<ncols;j++){
            newmap->array[i][j]=NULL;
        }
    }

    newmap->nrows=nrows;
    newmap->ncols=ncols;

    return newmap;
}

void map_free (Map *mp){
    if(!mp) return;

    int i,j;

    for(i=0;i<map_getNrows(mp);i++){
        for(j=0;j<map_getNcols(mp);j++){
            point_free(mp->array[i][j]);
        }
    }

    free(mp);
}

Point *map_insertPoint (Map *mp, Point *p){
    if(!mp||!p||position_exist(mp,p)==FALSE) return NULL;

    mp->array[point_getCoordinateY(p)][point_getCoordinateX(p)]=p;

    return p;
}

int map_getNcols (const Map *mp){
    if(!mp) return -1;

    return mp->ncols;
}

int map_getNrows (const Map *mp){
    if(!mp) return -1;

    return mp->nrows;
}

Point * map_getInput(const Map *mp){
    if(!mp) return NULL;

    return mp->input;
}

Point * map_getOutput(const Map *mp){
    if(!mp) return NULL;

    return mp->output;
}

Point *map_getPoint (const Map *mp, const Point *p){
    if(!mp || !p || position_exist(mp,p)==FALSE) return NULL;

    return mp->array[point_getCoordinateY(p)][point_getCoordinateX(p)];
}

Point *map_getNeighboor(const Map *mp, const Point *p, Position pos){
    if(!mp || !p || position_exist(mp,p)==FALSE||pos<0||pos>4) return NULL;

    switch (pos){
    case UP:
        if(point_getCoordinateY(p)==0) return NULL;
        return mp->array[point_getCoordinateY(p)-1][point_getCoordinateX(p)];
        break;
    case DOWN:
        if(point_getCoordinateY(p)==(map_getNrows(mp)-1)) return NULL;
        return mp->array[point_getCoordinateY(p)+1][point_getCoordinateX(p)];
    case RIGHT:
        if(point_getCoordinateX(p)==(map_getNcols(mp)-1)) return NULL;
        return mp->array[point_getCoordinateY(p)][point_getCoordinateX(p)+1];
    case LEFT:
        if(point_getCoordinateX(p)==0) return NULL;
        return mp->array[point_getCoordinateY(p)][point_getCoordinateX(p)-1];
    default:
        break;
    }

    return map_getPoint(mp,p);
}

Status map_setInput(Map *mp, Point *p){
    if(!mp||!p||position_exist(mp,p)==FALSE) return ERROR;

    mp->input=p;

    return OK;
}

Status map_setOutput (Map *mp,Point *p){
    if(!mp||!p||position_exist(mp,p)==FALSE) return ERROR;

    mp->output=p;

    return OK;
}

int map_print (FILE*pf, Map *mp){
    if(!pf||!mp) return -1;

    int i,j,y,x=0;

    y=fprintf(pf,"%d,%d\n",map_getNrows(mp),map_getNcols(mp));

    if(y==-1) return -1;
    x+=y;

    for(i=0;i<map_getNrows(mp);i++){
        for(j=0;j<map_getNcols(mp);j++){
            y=point_print(pf,mp->array[i][j]);
            if(y==-1) return -1;
            x+=y;
        }
    }

    return x;
}

Map * map_readFromFile (FILE *pf){
    if(!pf) return NULL;
    
    int rows,cols,i,j;
    Map *mp;
    Point *aux;
    char c;

    if(!fscanf(pf,"%d %d\n",&rows,&cols))return NULL;

    mp=map_new(rows,cols);

    if(mp==NULL) return NULL;

    for(i=0;i<rows;i++){
        for(j=0;j<cols;j++){
            fscanf(pf,"%c",&c);
            aux=point_new(j,i,c);
            map_insertPoint(mp,aux);
            if(c==INPUT){
                map_setInput(mp,aux);
            }else if(c==OUTPUT){
                map_setOutput(mp,aux);
            }
        }
        fscanf(pf,"\n");
    }

    return mp;

}

Bool map_equal (const void *_mp1, const void *_mp2){
    if(!_mp1||!_mp2) return FALSE;

    Map *mp1,*mp2;
    int i,j;
    Bool aux;

    mp1=(Map*)_mp1;
    mp2=(Map*)_mp2;

    if(map_getNcols(mp1)!=map_getNcols(mp2)||map_getNrows(mp1)!=map_getNrows(mp2)) return FALSE;

    for(i=0;i<map_getNrows(mp1);i++){
        for(j=0;j<map_getNcols(mp1);j++){
            aux=point_equal(mp1->array[i][j],mp2->array[i][j]);
            if(aux==FALSE) return aux;
        }
    }

    return aux;
}

Point * map_dfs (FILE *pf,Map *mp){
    if(!pf||!mp) return NULL;

    Point *p, *pin, *pout,*paux;
    Stack *s;
    int i;

    pin=map_getInput(mp);
    if(pin==NULL) return NULL;

    pout=map_getOutput(mp);
    if(pout==NULL) return NULL;

    p=NULL;

    s=stack_init();
    if(s==NULL) return NULL;

    if(stack_push(s,pin)==ERROR){
        stack_free(s);
        return NULL;
    }
    

    while(point_equal(p,pout)==FALSE && stack_isEmpty(s)==FALSE){
        p=(Point*)stack_pop(s);
        if(p==NULL){
            stack_free(s);
            return NULL;
        }

        if(point_getVisited(p)==FALSE){
            if(point_setVisited(p,TRUE)==ERROR){
                stack_free(s);
                return NULL;
            }
            if(point_print(pf,p)==-1){
                stack_free(s);
                return NULL;
            }

            for(i=0;i<=4;i++){
                paux=map_getNeighboor(mp,p,i);
                if(paux!=NULL && point_getSymbol(paux)!=BARRIER && point_getVisited(paux)==FALSE){
                    if(stack_push(s,paux)==ERROR){
                        stack_free(s);
                        return NULL;
                    }
                }
            }
        }
    }

    stack_free(s);

    if(point_equal(p,pout)==TRUE) return p;

    return NULL;
}

/*Private functions */
Bool position_exist(const Map *mp,const Point *p){
    if(!mp || !p ||  point_getCoordinateX(p)>=map_getNcols(mp) 
    || point_getCoordinateY(p)>=map_getNrows(mp)) return FALSE;

    return TRUE;
}
