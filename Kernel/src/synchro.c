
//Creo la struct de semaforos
typedef struct sem {
    int identifier;
    int counter;
    struct sem* next;
} sem_t;

// semaforos:
// - se inicializan en 1, los identifica un numero que es el ID.
// - se inicializan con una función tipo sem_init();
// - si se hace wait el contador baja uno ( si no estaba en cero ) y si se hace post sube en uno.
// - si se hace wait y el contador está en 0:
//          1 - bloquear el proceso y ponerlo en la lista de bloqueados para ese semaforo.
// - si se hace post y el semaforo estaba en 0:
//          2 - desbloquear un proceso de la lista de bloqueados.

int next_identifier = 0;
sem_t *first_sem;

sem_init( int identifier, int initial_count ) {
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
}