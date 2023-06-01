#include <stdio.h>
#include <stdlib.h>
#include "map.h"

int main(int argc, char const *argv[]){
    Map *mp;
    Point *p[3][4];
    Point *aux;
    int i,j;

    mp=map_new(3,4);

    if(mp==NULL) return 1;

    for(i=0;i<3;i++){
        for(j=0;j<4;j++){
            p[i][j]=point_new(j,i,BARRIER);
            map_insertPoint(mp,p[i][j]);
        }
    }

    point_setSymbol(p[1][1],INPUT);
    point_setSymbol(p[1][2],OUTPUT);

    map_setInput(mp,p[1][1]);
    map_setOutput(mp,p[1][2]);

    fprintf(stdout,"Map:\n");
    map_print(stdout,mp);

    fprintf(stdout,"\nGet output neighboors:\n");

    aux=map_getNeighboor(mp,map_getOutput(mp),RIGHT);
    point_print(stdout,aux);
    aux=map_getNeighboor(mp,map_getOutput(mp),UP);
    point_print(stdout,aux);
    aux=map_getNeighboor(mp,map_getOutput(mp),LEFT);
    point_print(stdout,aux);
    aux=map_getNeighboor(mp,map_getOutput(mp),DOWN);
    point_print(stdout,aux);

    fprintf(stdout,"\n");

    map_free(mp);

    return 0;
}
