#include <synchro.h>
#include <lib.h>
#include <strings.h>
#include <pipes.h>

#define CRIT_REGION_SIZE 3000
#define NULL 0

// Creamos un pipe con un identificador.
// Le asigna el pipe a un proceso para que pueda leer o escribir
// 



pipe_t * first = NULL;

int create_pipe(const char *identifier) {
    // verificar que no haya pipe con el mismo identifier.

    //Hacemos una nueva lista o Agregamos el pipe alfinal de la lista 
    pipe_t  * pipe;
    if ( first == NULL ) {
       first = malloc(sizeof(pipe_t));
       pipe = first;
    }
    else {
        pipe = first;
        while(pipe->next != NULL){
            pipe = pipe->next;
        }
        
        pipe->next = malloc(sizeof(pipe_t));
        pipe = pipe->next;
    }
    //Llenamos el pipe nuevo
    pipe->identifier = identifier;
    // pipe->read_sem = sem_init(5*identifier, 0);
    // pipe->write_sem = sem_init(3*identifier, 1);
    pipe->global_sem = sem_init(identifier, 1);
    pipe->critical_region = malloc(CRIT_REGION_SIZE);
    pipe->next = NULL;

    return 0;
}

//Escribir en el pipe
int write_pipe(const char *identifier,const char *str) {
    pipe_t * current = first;
    while (current != NULL && strcmp(identifier, current->identifier))
    {
        current = current->next;
    }
    if (current == NULL)
    {
        //No esta el pipe
        return 1;
    }else
    {
        //Tengo el pipe que quiero modificar
        
        sem_wait(current->global_sem->identifier);
        // sem_wait(current->write_sem->identifier);
        
        //Aca estoy habilitado a escribir
        // usamos memcpy de lib.c y strlen de strings.c
        memcpy(current->critical_region, str , strlen(str) );
        
        sem_post(current->global_sem->identifier);
        //sem_post(current->write_sem->identifier);
    }
    
    return 0;
    
}

//Leer el pipe 
int read_pipe(const char *identifier, char *destination) { //Aca lo podemos hacer de varias formas. 1) Que nos pasen un puntero donde quieren que escribamos lo que lee. 2) Que se lo devolvamos por return
    pipe_t * current = first;
    while (current != NULL && strcmp(identifier, current->identifier))
    {
        current = current->next;
    }
    if (current == NULL)
    {
        //No esta el pipe
        return 1;
    }else
    {
        //Tengo el pipe que quiero modificar
        
        sem_wait(current->global_sem->identifier);
        // sem_wait(current->write_sem->identifier);
        
        //Aca estoy habilitado a escribir
        // usamos memcpy de lib.c y strlen de strings.c
        memcpy(destination, current->critical_region, strlen(current->critical_region) );
        
        sem_post(current->global_sem->identifier);
        //sem_post(current->write_sem->identifier);
    }
    
    return 0;
}


pipe_t * list_pipes(){
    return first;
}