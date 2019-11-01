

//Creo la struct de fds
typedef struct fd {
    int fd_number;

} fd_t;

// semaforos:
// - se inicializan en 1, los identifica un numero que es el ID.
// - se inicializan con una función tipo sem_init();
// - si se hace wait el contador baja uno ( si no estaba en cero ) y si se hace post sube en uno.
// - antes del wait se verifica el contador, si esta en 0:
//          1 - bloquear el proceso y ponerlo en la lista de bloqueados para ese semaforo.
// - si se hace post y el semaforo estaba en 0:
//          2 - desbloquear un proceso de la lista de bloqueados de ese semaforo.