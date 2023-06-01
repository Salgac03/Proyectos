#include <stdlib.h>
#include "pow.h"
#include "min_comp.h"

#define SHM_NAME "/monitorsegment"
#define MAX_QUEUE 5


typedef struct
{
    sem_t mutex;
    sem_t full;
    sem_t empty;
    Bloque blocks[MAX_QUEUE];
    int front;
    int rear;
} Shmstruct;

Shmstruct *aux=NULL;
int fd_aux;

Status salircomprobador(Shmstruct *str,int fd);

void handler(int sig){
    salircomprobador(aux,fd_aux);
    exit(EXIT_SUCCESS);
}

void Productor(Shmstruct *s);
void Consumidor(Shmstruct *s);

int main(int argc, char const *argv[]){
    int fd_shm;
    Shmstruct *stru = NULL;
    pid_t pid;
    sigset_t set,oldset;

    sigemptyset(&oldset);
    sigemptyset(&set);
    sigaddset(&set,SIGINT);

    sigprocmask(SIG_BLOCK,&set,&oldset);

    struct sigaction act;

    act.sa_handler=handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;

    sigaction(SIGINT,&act,NULL);
    
    sigprocmask(SIG_UNBLOCK,&set,&oldset);

    fd_shm = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd_shm == -1){
        perror("Error creating the shared memory segment\n");
        exit(EXIT_FAILURE);
    }
    ftruncate(fd_shm,sizeof(Shmstruct));
    stru=mmap(NULL,sizeof(Shmstruct),PROT_READ | PROT_WRITE,MAP_SHARED,fd_shm,0);
    aux=stru;
    fd_aux=fd_shm;

    sem_init(&stru->empty,fd_shm,MAX_QUEUE);
    sem_init(&stru->mutex,fd_shm,1);
    sem_init(&stru->full,fd_shm,0);

    pid = fork();
    if (pid < 0){
        
    }else if(pid>0){
        Productor(stru);
        salircomprobador(stru,fd_shm);
        wait(NULL);
    }else{
        Consumidor(stru);
        salircomprobador(stru,fd_shm);
    }
    

    return 0;
}

void Productor(Shmstruct *s)
{
    if (!s)
        return;

    mqd_t mq;
    Bloque aux;
    sem_t *sem;

    mq=abrircola();
    if(mq==-1){
        perror("Error al abrir/crear la cola de mensajes.");
        return;
    }

    sem=abrirsemaforo();
    if(sem==NULL){
        fprintf(stderr,"Error al abrir/crear el semaforo con nombre");
        return;
    }

    while (1){
        sem_post(sem);
        if (mq_receive(mq, (char *)&aux, sizeof(Bloque) + 1, NULL) == -1){
            perror("Error al recibir el mensaje");
            return;
        }
        sem_wait(&s->empty);
        sem_wait(&s->mutex);

        if(comprobar(aux.target,aux.solucion)==TRUE){
            aux.correcto=TRUE;
        }else{
            aux.correcto=FALSE;
        }
        
        s->blocks[s->rear] = aux;

        s->rear = (s->rear + 1) % MAX_QUEUE;
        sem_post(&s->mutex);
        sem_post(&s->full);

        if(aux.id==-1) break;
    }

    sem_close(sem);
    sem_unlink(SEMMON);

    return;
}

void Consumidor(Shmstruct *s)
{
    if (!s) return;
    fprintf(stdout, "Printing blocks...\n");
    Bloque bloq;

    while (1)
    {
        sem_wait(&s->full);
        sem_wait(&s->mutex);

        bloq=s->blocks[s->front];

        if(bloq.id==-1){
            return;
        }

        fprintf(stdout,"Id: %04d\nWinner: %d\nTarget: %08ld\nSolution: %08ld",bloq.id,bloq.ganador,bloq.target,bloq.solucion);

        if(bloq.correcto==TRUE){
            fprintf(stdout," (validated)\n");
        }else{
            fprintf(stdout," (rejected)\n");
        }

        fprintf(stdout,"Votes: %d/%d\nWallets: ",bloq.votosafavor,bloq.votostotales);

        for(int i=0;i<MAXMINEROS;i++){
            if(bloq.wallts.pids[i] != -1){
                fprintf(stdout,"%d:%d ",bloq.wallts.pids[i],bloq.wallts.monedas[i]);
            }
        }
        fprintf(stdout,"\n\n");

        s->front = (s->front + 1) % MAX_QUEUE;
        
        sem_post(&s->mutex);
        sem_post(&s->empty);
    }

    return;
}

Status salircomprobador(Shmstruct *str,int fd){
    sem_destroy(&str->mutex);
    sem_destroy(&str->full);
    sem_destroy(&str->empty);
    munmap(str,sizeof(Shmstruct));
    shm_unlink(SHM_NAME);
    close(fd);

    return OK;
}