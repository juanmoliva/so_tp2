#include <lib.h>
#include <process.h>
#include <string.h>
#include <synchro.h>
#include <memory.h>

// semaforos:
// - se inicializan en 1, los identifica un numero que es el ID.
// - se inicializan con una función tipo sem_init();
// - si se hace wait el contador baja uno ( si no estaba en cero ) y si se hace post sube en uno.
// - antes del wait se verifica el contador, si esta en 0:
//          1 - bloquear el proceso y ponerlo en la lista de bloqueados para ese semaforo.
// - si se hace post y el semaforo estaba en 0:

//          2 - desbloquear un proceso de la lista de bloqueados de ese semaforo.

int next_identifier = 0;
sem_t *first_sem;

int sem_init( int identifier, int initial_count ) {
    // fijarnos que identificador no se haya usado 
    
    // malloc para nuevo semaforo
    sem_t *new_sem = malloc(sizeof(sem_t));
    new_sem->identifier = identifier;
    new_sem->counter = initial_count;

    //agrego el nuevo semaforo a la lista
    sem_t *current = first_sem;
    while( current != NULL ) {
        current = current->next;
    }
    current = new_sem;

    return 0;
}

//Buscar el semaforo con el ID = identifier
sem_t *sem_open( int identifier) {
    //busco el semaforo en la lista
    sem_t *current = first_sem;
    while( current != NULL && current->identifier != identifier) { 
        current = current->next;
    }

    // si es NULL no se encontró el semaforo.
    return current;
}

int sem_wait( int identifier ) {
    sem_t *sem = sem_open(identifier);
    // si sem es null, el identificador es invalido.
    if (sem == NULL) {
        return 1;
    }

    if ( sem->counter == 0 ){
        // bloquear proceso.
        int pid = getpid();
        update_process_state(pid, 'b');
        
    }
    else {
        sem_down( &sem->counter );
    }

    return 0;
}

int sem_post( int identifier ) {
    sem_t *sem = sem_open(identifier);
    // si sem es null, el identificador es invalido.
    if (sem == NULL) {
        return 1;
    }

    sem_up( &sem->counter );

    return 0;
}

//Aca cerramos un semaforo. Liberamos la memoria y lo sacamos de la lista
int sem_close(int identifier) {
    //Lo saco de la lista
    sem_t * prev_current;
    sem_t * current = first_sem;
    while (current != NULL && current->identifier!=identifier)
    {
        prev_current = current;
        current = current->next;
    }
    if (current == NULL)
    {
        //no esta ese semaforo abierto papa
        return 1;
    }else
    {
        //lo tengo q sacar de la lista
        prev_current->next = current->next;
        //y libero la memoria
        free_block(current);
        return 0;
    }
}

//Aca mostramos la lista de todos los semaforos actuales -----------------------------------------------------
void sem_list() {
    //Recorro e imprimo
    sem_t * current = first_sem;
    while (current != NULL)
    {
        //PRINT
        current = current->next;
    }
    return;
}