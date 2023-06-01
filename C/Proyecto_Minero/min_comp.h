#ifndef MINCOMP_H
#define MINCOMP_H

#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "pow.h"

#define MAXMINEROS 100
#define MQ_NAME "/mq_miner"
#define SEMMON "/semmon"

/*Estados que puede tener la flag que indica el estado del minado*/
typedef enum{
    BUSCANDO=0,
    ENCONTRADO=1,
    OTROENCONTRO=2
}FlagSt;

/*Estados que indican en algunas funciones si estas se han comportado de la manera esperada*/
typedef enum{
    OK=0,
    ERROR=1
}Status;

typedef enum{
    TRUE=0,
    FALSE=1,
    NONE=2
}Bool;

/*Tipo de voto posible, SI (acepta el resultado), NO (lo rechaza), BLANCO (no ha votado),VACIO (no hay votante)*/
typedef enum{
    SI=0,
    NO=1,
    BLANCO=2,
    VACIO=3
}Voto;

typedef struct _Carteras
{
    pid_t pids[MAXMINEROS];
    int monedas[MAXMINEROS];
}Carteras;

typedef struct _Datos
{
    long int ini;
    long int fin;
    long int numbusq;
    volatile FlagSt *flag;
    long int *sol;
} Datos;

typedef struct _Bloque{
    int id;
    long int target;
    long int solucion;
    pid_t ganador;
    int votostotales;
    int votosafavor;
    Carteras wallts;
    Bool correcto;
}Bloque;

/*Información correspondiente a cada uno de los mineros y de la que solo dispone el
propio minero al que hace referencia esta información*/
typedef struct _Infominero
{
    pid_t pid;
    int monedas;
    int indice;
}Infominero;

typedef struct _SegmentStruct
{
    Carteras wallets;
    Voto vts[MAXMINEROS];
    Bloque bl_ultimo;
    Bloque bl_actual;
    int num_mineros;
    sem_t pidsmutex;
    sem_t segm_sem;
    sem_t cuenta;
    sem_t light_mutex;
    sem_t ganador;
}SegmentStruct;

/**
 * @brief Función que se encarga de realizar la búsqueda del objetivo, es decir, el minado.
 * 
 * @param arg 
 * @return void* 
 */
void *minar(void *arg);

/**
 * @brief Función que comprueba si la solución encontrada en una ronda de minado
 * corresponde al objetivo que se buscaba
 * 
 * @param objetivo 
 * @param solucion 
 * @return Bool
 */
Bool comprobar(long int objetivo, long int solucion);

/**
 * @brief Función que en base al resultado de una comprobación (usando la función 
 * comprobar) emite un voto.
 * 
 * @param segm 
 * @param info 
 * @return Status 
 */
Status votar(SegmentStruct *segm, Infominero *info);

/**
 * @brief Función utilizada por los mineros para entrar en el sistema de minado si no es
 * el primer minero.
 * 
 * @param info 
 * @param segm 
 * @return Status 
 */
Status entrar(Infominero *info, SegmentStruct *segm);

/**
 * @brief Función utilizada para que un minero salga del sistema, si es el último minero, 
 * también libera los recursos compartidos.
 * 
 * @param info 
 * @param segm 
 * @param segmname 
 * @param tuberia 
 * @return Status 
 */
Status salir(Infominero *info, SegmentStruct *segm, char *segmname, int * tuberia, int fd_segm,sem_t *sem,mqd_t mq);

/**
 * @brief Función genérica usada para la sincronización del segmento de memoria compartida
 * usando semáforos, esta función se encarga en el programa de inpedir el acceso al segmento
 * de memoria compartida al ganador tras finalizar una ronda, esto con el fin de que si algún
 * minero ha intentado conectarse durante una ronda activa pueda hacerlo antes de que empiece
 * la siguiente.
 * 
 * @param mutex 
 * @param cuenta 
 * @param segmsem 
 * @return Status 
 */
Status light_switch_on(sem_t *mutex,sem_t *cuenta, sem_t *segmsem);

/**
 * @brief Función genérica usada para la sincronización del segmento de memoria compartida
 * usando semáforos, esta función se encarga en el programa de devolver el acceso al segmento
 * de memoria compartida al ganador después de que hayan entrado al sistema los mineros que
 * se intentaron conectar durantela ronda activa, con el fin de que al empezar la nueva ronda
 * el ganador disponga de la información de todos los mineros activos.
 * 
 * @param mutex 
 * @param cuenta 
 * @param segmsem 
 * @return Status 
 */
Status light_switch_off(sem_t *mutex,sem_t *cuenta, sem_t *segmsem);

/**
 * @brief Función a la que llaman todos los mineros perdedores, esta comprueba si la señal de 
 * votar se ha recibido y de no ser así realiza una espera no activa hasta que le llegue la se-
 * ñal, una vez le ha llegado vota.
 * 
 * @param segm 
 * @param info 
 * @param flagvote 
 * @return Status 
 */
Status perdedor(SegmentStruct *segm, Infominero *info, volatile Bool *flagvote);

/**
 * @brief Función a la que llama el minero ganador, en ella el minero ganador vota, y tras esto
 * pone los votos de todos los mineros activos en blanco y les envía la señal de votar. Tras esto
 * comprueba los votos de los mineros de manera que si alguno tiene su voto en blanco, realiza una
 * espera no activa y vuelve a comprobar los votos desde el principio. Una vez ha terminado el re-
 * cuento de votos, actualiza los bloques del segmento de memoria compartida, si hay un monitor acti-
 * vo (usa el semáforo para ello) le envía el bloque resuelto, tras esto envía la señal para comenzar 
 * una nueva ronda.
 * 
 * @param segm 
 * @param info 
 * @param sem
 * @param mq
 * @return Status 
 */
Status ganador(SegmentStruct *segm,Infominero *info, sem_t *sem, mqd_t mq);

void iniciar(SegmentStruct *segm, int fd_segment,Infominero *info);

mqd_t abrircola();

sem_t * abrirsemaforo();

#endif