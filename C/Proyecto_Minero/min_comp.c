#include "min_comp.h"

void *minar(void *arg){
    
    Datos *d;
    d = (Datos*)arg;
    for (long int i = d->ini; i < d->fin && (*d->flag)==BUSCANDO; i++)
    {
        if(pow_hash(i)==d->numbusq){
            (*d->flag)=ENCONTRADO;
            (*d->sol)=i;  
        }
    }
    return NULL;
}

Bool comprobar(long int objetivo, long int solucion){
    if(pow_hash(solucion)==objetivo) return TRUE;

    return FALSE;
}

Status votar(SegmentStruct *segm, Infominero *info){
    if(!info || !segm) return ERROR;

    if(comprobar(segm->bl_actual.target,segm->bl_actual.solucion)==TRUE){
        segm->vts[info->indice]=SI;
    }else{
        segm->vts[info->indice]=NO;
    }

    return OK;
}

Status entrar(Infominero *info, SegmentStruct *segm){
    if(segm->num_mineros==MAXMINEROS || !info || !segm) return ERROR;

    int i;
    
    for(i=0;i<MAXMINEROS;i++){
        sem_wait(&segm->pidsmutex);
        if(segm->wallets.pids[i]==-1){
            segm->wallets.pids[i]=info->pid;
            segm->wallets.monedas[i]=0;
            segm->vts[i]=BLANCO;
            segm->num_mineros++;
            info->indice=i;
            info->monedas=0;
            sem_post(&segm->pidsmutex);
            break;
        }
        sem_post(&segm->pidsmutex);
    }

    return OK;
}

Status salir(Infominero *info, SegmentStruct *segm, char *segmname, int * tuberia, int fd_segm,sem_t *sem,mqd_t mq){
    if(!info || !segm) return ERROR;

    Bloque aux;

    sem_wait(&segm->pidsmutex);
    segm->wallets.pids[info->indice]=-1;
    segm->wallets.monedas[info->indice]=-1;
    segm->vts[info->indice]=VACIO;
    segm->num_mineros--;
    sem_post(&segm->pidsmutex);

    if(segm->num_mineros==0){
        sem_destroy(&segm->cuenta);
        sem_destroy(&segm->light_mutex);
        sem_destroy(&segm->pidsmutex);
        sem_destroy(&segm->segm_sem);
        sem_destroy(&segm->ganador);
        shm_unlink(segmname);
        sem_unlink(SEMMON);
        mq_unlink(MQ_NAME);

        aux.id=-1;
        mq_send(mq,(char *)&aux,sizeof(Bloque)+1,1);
    }

    mq_close(mq);

    sem_close(sem);

    munmap(segm,sizeof(SegmentStruct));


    close(fd_segm);

    close(tuberia[1]);

    return OK;
}

Status light_switch_on(sem_t *mutex,sem_t *cuenta, sem_t *segmsem){
    if(!mutex || !cuenta || !segmsem) return ERROR;
    int size=-1;


    if(sem_wait(mutex)==-1) return ERROR;

    if(sem_post(cuenta)==-1)return ERROR;
    
    if(sem_getvalue(cuenta,&size)==-1)return ERROR;

    if(size==1){
        if(sem_wait(segmsem)==-1)return ERROR;
    }

    if(sem_post(mutex)==-1)return ERROR;

    return OK;
}

Status light_switch_off(sem_t *mutex,sem_t *cuenta, sem_t *segmsem){
    if(!mutex || !cuenta || !segmsem) return ERROR;
    int size=-1;


    if(sem_wait(mutex)==-1) return ERROR;

    if(sem_wait(cuenta)==-1)return ERROR;
    
    if(sem_getvalue(cuenta,&size)==-1)return ERROR;

    if(size==0){
        if(sem_post(segmsem)==-1)return ERROR;
    }

    if(sem_post(mutex)==-1)return ERROR;

    return OK;
}

Status ganador(SegmentStruct *segm,Infominero *info, sem_t *sem, mqd_t mq){
    if(!segm) return ERROR;

    int i,numvots=0,try=1;

    votar(segm,info);
    for(i=0;i<MAXMINEROS;i++){
        if(segm->wallets.pids[i]!=-1 && segm->wallets.pids[i]!= info->pid){
            segm->vts[i]=BLANCO;
            kill(segm->wallets.pids[i],SIGUSR2);
        }
    }

    for(i=0;numvots < segm->num_mineros;i++){
        if(segm->vts[i]==SI){
            segm->bl_actual.votostotales++;
            segm->bl_actual.votosafavor++;
            numvots++;
        }else if(segm->vts[i]==NO){
            segm->bl_actual.votostotales++;
            numvots++;
        }if(segm->vts[i]==BLANCO){
            segm->bl_actual.votostotales=0;
            segm->bl_actual.votosafavor=0;
            numvots=0;
            i=-1;
        }

        usleep(250000);
    }



    if(segm->bl_actual.votosafavor > numvots/2){
        info->monedas++;
        segm->wallets.monedas[info->indice]++;
        segm->bl_actual.correcto=TRUE;
    }

    segm->bl_ultimo=segm->bl_actual;
    segm->bl_ultimo.wallts=segm->wallets;

    try=sem_trywait(sem);

    if(try==0){
        mq_send(mq,(char *) &segm->bl_ultimo,sizeof(Bloque)+1,1);
    }

    segm->bl_actual.ganador=-1;
    segm->bl_actual.id++;
    segm->bl_actual.solucion=-1;
    segm->bl_actual.target=segm->bl_ultimo.solucion;
    segm->bl_actual.votosafavor=0;
    segm->bl_actual.votostotales=0;
    segm->bl_actual.correcto=NONE;

    sem_post(&segm->segm_sem);
    usleep(0.1);
    sem_post(&segm->ganador);
    sem_wait(&segm->segm_sem);
  

    for(i=0;i < MAXMINEROS;i++){
        if(segm->wallets.pids[i]!=-1){
            kill(segm->wallets.pids[i],SIGUSR1);
        }
    }

    return OK;
}

Status perdedor(SegmentStruct *segm, Infominero *info, volatile Bool *flagvote){
    if(!info || !segm || !flagvote) return ERROR;
    
    sigset_t auxset;

    sigemptyset(&auxset);

    while(*flagvote==FALSE){
        sigsuspend(&auxset);
    }

    votar(segm,info);

    return OK;
}

void iniciar(SegmentStruct *segm, int fd_segment,Infominero *info){
    int i;

    segm->bl_actual.target=0;
    segm->bl_actual.id=0;
    segm->bl_actual.votostotales=0;
    segm->bl_actual.votosafavor=0;
    segm->bl_actual.correcto=NONE;
    segm->num_mineros=0;
    sem_init(&segm->ganador,fd_segment,1);
    sem_init(&segm->pidsmutex,fd_segment,1);
    sem_init(&segm->light_mutex,fd_segment,1);
    sem_init(&segm->segm_sem,fd_segment,1);
    sem_init(&segm->cuenta,fd_segment,0);

    entrar(info,segm);

    sem_wait(&segm->segm_sem);

    for(i=0;i < MAXMINEROS;i++){
        if(segm->wallets.pids[i]!=-1){
            kill(segm->wallets.pids[i],SIGUSR1);
        }
    }
}

mqd_t abrircola(){
    struct mq_attr attributes;
    mqd_t mq;

    attributes.mq_maxmsg = 10;
    attributes.mq_msgsize = sizeof(Bloque) + 1;
    attributes.mq_flags = 0;
    attributes.mq_curmsgs = 0;

    if ((mq = mq_open(MQ_NAME, O_CREAT| O_RDWR, S_IRUSR | S_IWUSR, &attributes)) == (mqd_t)-1){
        perror("Error al crear/abrir la cola de mensajes");
        return -1;
    }

    return mq;
}

sem_t * abrirsemaforo(){
    sem_t *ret;

    if((ret=sem_open(SEMMON, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR,0))==SEM_FAILED){
        if(errno==EEXIST){
            ret=sem_open(SEMMON,O_CREAT);
            if(ret==SEM_FAILED){
                return NULL;
            }
        }else{
            return NULL;
        }
    }

    return ret;
}
