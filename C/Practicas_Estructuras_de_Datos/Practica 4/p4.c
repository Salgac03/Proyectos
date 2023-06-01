#include <stdlib.h>
#include <stdio.h>
#include "bstree.h"
#include "point.h"


BSTree * tree_read_points_from_file(FILE * pf);


int main (int argc, char *argv[]){

    if(argc != 2){
        fprintf(stderr,"Numero de argumentos insuficiente\nmodo de uso: ./p4 <NombreArchivo>\n");
        return 1;
    }

    BSTree *tree=NULL;
    FILE *f;
    Point *min,*max;

    f=fopen(argv[1],"r");
    if(f==NULL){
        fprintf(stderr,"Error al abrir archivo\n");
        return 1;
    }

    tree=tree_read_points_from_file(f);
    if(tree==NULL){
        fprintf(stderr,"Error al crear arbol\n");
        return 1;
    }

    tree_inOrder(stdout,tree);

    min=tree_find_min(tree);
    max=tree_find_max(tree);

    point_print(stdout,min);
    point_print(stdout,max);

    printf("\n");

    tree_remove(tree,max);

    tree_inOrder(stdout,tree);

    tree_destroy(tree);

    return 0;
}


BSTree * tree_read_points_from_file(FILE * pf){
    BSTree * t;
    int nnodes=0, i;
    Status st = OK;
    int x, y;
    char symbol;
    Point * p;

    if (!pf){
        return NULL;
    }

    /* Read number of nodes */
    if (fscanf(pf, "%d\n", &nnodes) != 1){
        return NULL;
    }

    /* Create tree */
    t = tree_init(point_print, point_cmpEuDistance);
    if(!t){
        return NULL;
    }

    /* Read nodes */
    for (i=0; i<nnodes && st==OK ;i++){
        if(fscanf(pf, "%d %d %c", &x, &y, &symbol)!=3){
            return NULL;
        }
        p=point_new(x, y, symbol);
        if(!p){
            tree_destroy(t);
            return NULL;
        }

        st=tree_insert(t, p); 
        if(st==ERROR){
            tree_destroy(t);
            point_free(p);
            return NULL;      
        }

    }


    return t;






}