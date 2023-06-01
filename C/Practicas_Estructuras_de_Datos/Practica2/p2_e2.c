#include <stdio.h>
#include <stdlib.h>
#include "map.h"

int main(int argc,char *argv[]){
    FILE *f;
    Map *mp;

    //comprobamos si el número de argumentos es el indicado
    if(argc!=2){
        fprintf(stderr,"Numero de argumentos invalido. modo de uso: ./p2_e2 <NOMBRE_DE_ARCHIVO>.\n");
        return 1;
    }

    //abrimos el archivo que tiene por nombre argv[1]
    f=fopen(argv[1],"r");
    if(f==NULL){
        fprintf(stderr,"Error al abrir el archivo");
        return 1;
    }

    //creamos un mapa a través del archivo abierto en f
    mp=map_readFromFile(f);

    if(mp==NULL){
        fclose(f);
        fprintf(stderr,"Error al crear mapa desde el archivo");
        return 1;
    }

    //imprimimos el mapa
    fprintf(stdout,"Maze:\n");

    map_print(stdout,mp);

    //recorremos el mapa desde input hasta output usando map_dfs
    fprintf(stdout,"\nDFS traverse:\n");

    map_dfs(stdout,mp);

    //liberamos el mapa creado
    map_free(mp);

    fprintf(stdout,"\n");

    //cerramos el archivo
    fclose(f);
    return 0;
}