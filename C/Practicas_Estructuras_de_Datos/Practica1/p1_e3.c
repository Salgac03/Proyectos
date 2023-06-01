#include <stdio.h>
#include <stdlib.h>
#include "map.h"

int main(int argc,char *argv[]){
    FILE *f;
    Map *mp;
    Point *aux,*aux1;

    if(argc!=2){
        fprintf(stderr,"Numero de argumentos invalido");
        return 1;
    }

    f=fopen(argv[1],"r");
    if(f==NULL){
        fprintf(stderr,"Error al abrir el archivo");
        return 1;
    }

    mp=map_readFromFile(f);

    if(mp==NULL){
        fclose(f);
        fprintf(stderr,"Error al crear mapa desde el archivo");
        return 1;
    }

    fprintf(stdout,"Maze:\n");

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

    aux1=point_new(map_getNcols(mp)-1,map_getNrows(mp)-1,SPACE);

    if(aux1==NULL){
        fclose(f);
        map_free(mp);
        fprintf(stderr,"Error al crear punto auxiliar");
        return 1;
    }

    fprintf(stdout,"\nGet output neighboors:\n");

    aux=map_getNeighboor(mp,map_getPoint(mp,aux1),UP);
    point_print(stdout,aux);
    aux=map_getNeighboor(mp,map_getPoint(mp,aux1),LEFT);
    point_print(stdout,aux);

    point_free(aux1);

    map_free(mp);

    fprintf(stdout,"\n");

    fclose(f);
    return 0;
}