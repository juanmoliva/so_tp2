#ifndef SYNCHRO_H_
#define SYNCHRO_H_

#include <process.h>

typedef struct p_blocked{
    int pid;
    struct p_blocked * next;
    
} p_blocked_t;


//Creo la struct de semaforos
typedef struct sem {
    const char *identifier;
    int counter;
    struct sem* next;
    //lista de procesos bloqueados en un semaforo
    p_blocked_t * blocked_processes;
    
} sem_t;


int sem_init(const char *, int initial_count );
sem_t *sem_open(const char *str);
int sem_wait(const char *identifier );
int sem_post(const char *identifier ); 
void sem_list();
int sem_close(const char *identifier);

#endif