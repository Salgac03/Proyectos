#include <stdio.h>
#include "point.h"
#include "types.h"

int main(){
    Point *p[4];
    Bool res;
    int i;

    p[0]=point_new(0,0,BARRIER);
    p[1]=point_new(0,1,BARRIER);

    point_print(stdout,p[0]);
    point_print(stdout,p[1]);
    fprintf(stdout,"\n");

    fprintf(stdout,"Equal points p[0] and p[1]? ");

    res=point_equal(p[0],p[1]);

    if(res==TRUE){
        fprintf(stdout,"yes\n");
    }else{
        fprintf(stdout,"no\n");
    }

    p[2]=point_hardcpy(p[0]);

    fprintf(stdout,"Creating p[2]: ");
    point_print(stdout,p[2]);

    fprintf(stdout,"\nEqual points p[0] and p[2]? ");

    res=point_equal(p[0],p[2]);

    if(res==TRUE){
        fprintf(stdout,"yes\n");
    }else{
        fprintf(stdout,"no\n");
    }

    point_setSymbol(p[2],SPACE);

    fprintf(stdout,"Modifying p[2]: ");
    point_print(stdout,p[2]);

    fprintf(stdout,"\nEqual points p[0] and p[2]? ");

    res=point_equal(p[0],p[2]);

    if(res==TRUE){
        fprintf(stdout,"yes\n");
    }else{
        fprintf(stdout,"no\n");
    }

    p[3]=p[0];

    fprintf(stdout,"Assign p[3] = p[0]\n");

    point_setSymbol(p[3],OUTPUT);

    fprintf(stdout,"Modifying p[3]: ");
    point_print(stdout,p[3]);

    fprintf(stdout,"\n");

    for(i=0;i<4;i++){
        point_print(stdout,p[i]);
    }

    fprintf(stdout,"\n");

    for(i=0;i<3;i++){
        point_free(p[i]);
    }

    return 0;
}