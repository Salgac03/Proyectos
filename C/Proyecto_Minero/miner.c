#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "min_comp.h"

#define MINSEGM "/minsegm"
#define FILESIZE 1024

volatile  FlagSt flag=BUSCANDO;
volatile  Bool voteflag=FALSE;
volatile  Bool mineflag=FALSE;

typedef struct _AuxExitParams
{
    Infominero *info;
    SegmentStruct *segm;
    int * tuberia;
    int fd_segm;
    sem_t *sem;
    mqd_t mq;
}AuxExitParams;

AuxExitParams params;

void handler (int sig){
    if(sig==SIGUSR2){
        flag=OTROENCONTRO;
        voteflag=TRUE;
        mineflag=FALSE;
    }else if(sig==SIGUSR1){
        flag=BUSCANDO;
        mineflag=TRUE;
        voteflag=FALSE;
    }else if(sig==SIGINT){
        salir(params.info,params.segm,MINSEGM,params.tuberia,params.fd_segm,params.sem,params.mq);
        exit(EXIT_SUCCESS);
    }
}

int minero(int* tuberia, int numhilos, int rounds, Infominero *info);
int registrador(int* tuberia, int fd_archivo);

int main(int argc, char const *argv[]){
    int rounds, numhilos,error,tuberia[2],fd_archivo;
    pid_t pid;
    Infominero info;
    char archivo[FILESIZE];
    sigset_t set, oldset;
    struct sigaction act;

    if (argc != 3)
    {
        printf("Número de argumentos no válido, debe escribir de la forma siguiente:\n./miner rounds numdehilos\n");
        return -1;
    }

    sigemptyset(&set);
    sigemptyset(&oldset);
    sigaddset(&set,SIGUSR2);
    sigaddset(&set,SIGUSR1);
    sigaddset(&set,SIGINT);
    sigprocmask(SIG_BLOCK,&set,&oldset);

    act.sa_handler=handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;

    sigaction(SIGUSR1,&act,NULL);

    sigaction(SIGUSR2,&act,NULL);

    sigaction(SIGINT,&act,NULL);

    rounds = atoi(argv[1]);
    numhilos = atoi(argv[2]);

    error=pipe(tuberia);
    if(error == -1) {
        perror("pipe");
        exit(EXIT_FAILURE); 
    }

    info.pid=getpid();

    sprintf(archivo,"%d",info.pid);
    strcat(archivo,".txt");

    fd_archivo=open(archivo,O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    
    if(fd_archivo==-1){
        perror("Error al crear el archivo registrador.\n");
        exit(EXIT_FAILURE);
    }

    pid=fork();

    if(pid<0){
        perror("Error al crear al proceso registrador.\n");
        exit(EXIT_FAILURE);
    }else if(pid>0){
        close(fd_archivo);
        minero(tuberia,numhilos,rounds,&info);
    }else{
        registrador(tuberia,fd_archivo);
    }

    wait(NULL);

    exit(EXIT_SUCCESS);

}

int minero(int* tuberia, int numhilos, int rounds, Infominero *info){
    close(tuberia[0]);
    
    long int sol,rang;
    int i,error,try=-1,fd_segment;
    Datos *d;
    pthread_t *hilos;
    ssize_t nbytes;
    SegmentStruct *segm;
    sigset_t auxset;
    mqd_t mq;
    sem_t *sem;
    
    fd_segment=shm_open(MINSEGM,O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd_segment == -1){
        if (errno == EEXIST){
            fd_segment=shm_open(MINSEGM,O_RDWR,0);
            if (fd_segment == -1){
                perror(" Error opening the shared memory segment ");
                exit(EXIT_FAILURE);
            }

            struct stat statussegm;

            fstat(fd_segment,&statussegm);
            while(statussegm.st_size==0){
                usleep(250000);
            }
            segm=mmap(NULL,sizeof(SegmentStruct),PROT_READ | PROT_WRITE,MAP_SHARED,fd_segment,0);
            
            light_switch_on(&segm->light_mutex,&segm->cuenta,&segm->segm_sem);
            entrar(info,segm);
            light_switch_off(&segm->light_mutex,&segm->cuenta,&segm->segm_sem);

        }else{
            perror("Error creating the shared memory segment\n");
            exit(EXIT_FAILURE);
        }
    }else{
        ftruncate(fd_segment,sizeof(SegmentStruct));
        segm=mmap(NULL,sizeof(SegmentStruct),PROT_READ | PROT_WRITE,MAP_SHARED,fd_segment,0);
        for(i=0;i<MAXMINEROS;i++){
            segm->wallets.pids[i]=-1;
            segm->wallets.monedas[i]=-1;
            segm->vts[i]=VACIO;
        }
        iniciar(segm,fd_segment,info);
    }

    mq=abrircola();
    sem=abrirsemaforo();

    params.fd_segm=fd_segment;
    params.info=info;
    params.segm=segm;
    params.tuberia=tuberia;
    params.mq=mq;
    params.sem=sem;

    rang = (POW_LIMIT / numhilos) + 1;

    d = (Datos *)malloc(numhilos * sizeof(Datos));
    if (!d)
    {
        exit(EXIT_FAILURE);
    }
    hilos = (pthread_t *)malloc(numhilos * sizeof(pthread_t));
    if (!hilos)
    {   
        free(d);
        exit(EXIT_FAILURE);
    }

    sigemptyset(&auxset);
    if(mineflag==FALSE){
        sigsuspend(&auxset);
    }

    for (i = 0; i < rounds; i++)
    {

        for (int j = 0, ranghilo = 0; j < numhilos; j++, ranghilo += rang){
            d[j].ini = ranghilo;
            d[j].fin = ranghilo + rang - 1;
            if( j+1 == numhilos){
                d[j].fin = POW_LIMIT - 1;
            }
            d[j].flag = &flag;
            d[j].sol = &sol;
            d[j].numbusq=segm->bl_actual.target;
            error = pthread_create(&hilos[j], NULL, minar, &d[j]);
            if (error != 0){
                free(hilos);
                free(d);
                exit(EXIT_FAILURE);
            }
        }
        for (int j = 0; j < numhilos; j++){
            error = pthread_join(hilos[j], NULL);
            if (error != 0){
                free(hilos);
                free(d);
                exit(EXIT_FAILURE);
            }
        }

        if(flag==OTROENCONTRO){
            perdedor(segm,info,&voteflag);
        }else if(flag==ENCONTRADO){
            try=sem_trywait(&segm->ganador);
            if(try==0){
                segm->bl_actual.ganador=info->pid;
                segm->bl_actual.solucion=sol;
                mineflag=FALSE;
                ganador(segm,info,sem,mq);
            }else{
                perdedor(segm,info,&voteflag);
            }
        }

        while(mineflag==FALSE){
            sigsuspend(&auxset);
        }
        nbytes = write(tuberia[1], &segm->bl_ultimo,sizeof(Bloque));
        if (nbytes == -1){
            perror("write 1");
            free(hilos);
            free(d);
            exit(EXIT_FAILURE);
        }
    }
    salir(info,segm,MINSEGM,tuberia,fd_segment,sem,mq);
    free(hilos); 
    free(d);
    exit(EXIT_SUCCESS);
}


int registrador(int* tuberia, int fd_archivo){
    close(tuberia[1]);

    ssize_t nbytes;
    int i;
    Bloque bloq;

    do
    {
        nbytes = read(tuberia[0],&bloq, sizeof(Bloque));

        if(nbytes==0) break;

        if (nbytes == -1){
            perror("read 1");
            exit(EXIT_FAILURE);
        }

        dprintf(fd_archivo,"Id: %04d\nWinner: %d\nTarget: %08ld\nSolution: %08ld",bloq.id,bloq.ganador,bloq.target,bloq.solucion);

        if(bloq.correcto==TRUE){
            dprintf(fd_archivo," (validated)\n");
        }else{
            dprintf(fd_archivo," (rejected)\n");
        }

        dprintf(fd_archivo,"Votes: %d/%d\nWallets: ",bloq.votosafavor,bloq.votostotales);

        for(i=0;i<MAXMINEROS;i++){
            if(bloq.wallts.pids[i] != -1){
                dprintf(fd_archivo,"%d:%d ",bloq.wallts.pids[i],bloq.wallts.monedas[i]);
            }
        }
        dprintf(fd_archivo,"\n\n");
    } while(1);

    close(fd_archivo);

    exit(EXIT_SUCCESS);
}