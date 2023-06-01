#include <stdio.h>
#include <limits.h> 
#include <math.h>
#include "point.h"

struct _Point {
    int x, y;
    char symbol;
    Bool visited; // for DFS
};

/*Private functions headers*/
Bool valid_symbol(char symbol);

/*Public functions*/

Point * point_new (int x, int y, char symbol){
    if(x<0 || y<0 || valid_symbol(symbol)==FALSE) return NULL;
    
    Point* newpoint;

    newpoint=(Point*)malloc(sizeof(Point));

    if(newpoint==NULL) return NULL;

    newpoint->x=x;
    newpoint->y=y;
    newpoint->symbol=symbol;
    newpoint->visited=FALSE;

    return newpoint;
}

void point_free (Point *p){
    free(p);
}

int point_getCoordinateX (const Point *p){
    if(!p) return INT_MAX;

    return p->x;
}

int point_getCoordinateY (const Point *p){
    if(!p) return INT_MAX;

    return p->y;
}

char point_getSymbol (const Point *p){
    if(!p) return ERRORCHAR;

    return p->symbol;
}

Status point_setCoordinateX (Point *p, int x){
    if(!p || x<0) return ERROR;

    p->x=x;

    return OK;
}

Status point_setCoordinateY (Point *p, int y){
    if(!p || y<0) return ERROR;

    p->y=y;
    
    return OK;
}

Status  point_setSymbol (Point *p, char c){
    if(!p || valid_symbol(c)==FALSE) return ERROR;

    p->symbol=c;

    return OK;
}

Bool point_getVisited (const Point *p){
    if(!p) return FALSE;

    return p->visited;
}

Status point_setVisited (Point *p, Bool bol){
    if(!p) return ERROR;

    p->visited=bol;

    return OK;
}

Point *point_hardcpy (const Point *src){
    if(src==NULL) return NULL;
    
    Point *trg;

    trg=point_new(point_getCoordinateX(src),point_getCoordinateY(src),point_getSymbol(src));

    if(trg==NULL) return NULL;

    return trg;
}

Bool point_equal (const void *p1, const void *p2){
    if(!p1 || !p2) return FALSE;

    Point *po1,*po2;

    po1=(Point*)p1;

    po2=(Point*)p2;

    if(point_getCoordinateX(po1)==point_getCoordinateX(po2) && point_getCoordinateY(po1)==point_getCoordinateY(po2)
        && point_getSymbol(po1)==point_getSymbol(po2)){
            
        return TRUE; 
    }

    return FALSE;
}

int point_print (FILE *pf, const void *p){
    if(!pf || !p) return -1;

    Point* po;

    po=(Point*) p;

    return fprintf(pf,"[(%d, %d): %c]"
    ,point_getCoordinateX(po),point_getCoordinateY(po),point_getSymbol(po));
}

Status point_euDistance(const Point *p1, const Point *p2, double *distance){
    if(!p1||!p2||!distance) return ERROR;

    *distance=sqrt(pow((point_getCoordinateX(p1)-point_getCoordinateX(p2)),2)+
        pow((point_getCoordinateY(p1)-point_getCoordinateY(p2)),2));

    return OK;
}

int point_cmpEuDistance(const void *p1, const void *p2){
    if(!p1||!p2) return INT_MIN;

    double dist1,dist2;
    Point *orig,*aux1,*aux2;

    orig=point_new(0,0,BARRIER);

    if(orig==NULL) return INT_MIN;

    aux1=(Point*)p1;
    aux2=(Point*)p2;

    if(point_euDistance(aux1, orig, &dist1)==ERROR)return INT_MIN;
    if(point_euDistance(aux2, orig, &dist2)==ERROR)return INT_MIN;

    if(dist1-dist2<0){
        point_free(orig);
        return -1;
    }
    if(dist1-dist2>0){
        point_free(orig);
        return 1;
    }
    point_free(orig);
    return 0;
}

/*Private functions */

Bool valid_symbol(char symbol){
    if(symbol!=ERRORCHAR && symbol!=INPUT && symbol!=OUTPUT && 
    symbol!=BARRIER && symbol!=SPACE) return FALSE;

    return TRUE;
}