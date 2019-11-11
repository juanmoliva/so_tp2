#ifndef SYNCHRO_H_
#define SYNCHRO_H_

#include <process.h>
#include <stdint.h>


#define MAX_SEM 100

typedef struct p_blocked{
    int pid;
    struct p_blocked * next;
    
} p_blocked_t;


//Creo la struct de semaforos
typedef struct sem {
    uint64_t identifier;
    uint64_t counter;
    struct sem* next;
    //lista de procesos bloqueados en un semaforo
    p_blocked_t * blocked_processes;
    
} sem_t;


int sem_init(int id, int initial_count );
sem_t *sem_open(int id);
int sem_wait(int id);
int sem_post(int id); 
sem_t *sem_list();
int sem_close(int id);

#endif