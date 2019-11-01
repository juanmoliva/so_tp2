#ifndef SYNCHRO_H_
#define SYNCHRO_H_

#include <process.h>

//Creo la struct de semaforos
typedef struct sem {
    int identifier;
    int counter;
    struct sem* next;
    //lista de procesos bloqueados en un semaforo
    p_blocked_t * blocked_processes;
    
} sem_t;

typedef struct p_blocked{
    int pid;
    process_t * info:
    struct p_blocked * next;
    
} p_blocked_t;



int sem_init( int identifier, int initial_count );
sem_t *sem_open( int identifier);
int sem_wait( int identifier );
int sem_post( int identifier ); 
void sem_list();
int sem_close(int identifier);

#endif