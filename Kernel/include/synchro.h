#ifndef SYNCHRO_H_
#define SYNCHRO_H_

//Creo la struct de semaforos
typedef struct sem {
    int identifier;
    int counter;
    struct sem* next;
} sem_t;

int sem_init( int identifier, int initial_count );
sem_t *sem_open( int identifier);
int sem_wait( int identifier );
int sem_post( int identifier ); 
void sem_list();
int sem_close(int identifier);

#endif