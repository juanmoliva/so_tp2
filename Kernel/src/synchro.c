#include <lib.h>
#include <process.h>
#include <strings.h>
#include <synchro.h>
#include <memory.h>

// semaforos:
// - se inicializan en 1, los identifica una string que es el identificador.
// - se inicializan con una función sem_init();
// - si se hace wait el contador baja uno ( si no estaba en cero ) y si se hace post sube en uno.
// - antes del wait se verifica el contador, si esta en 0:
//          1 - bloquear el proceso y ponerlo en la lista de bloqueados para ese semaforo.
// - si se hace post y el semaforo estaba en 0:
//          2 - desbloquear un proceso de la lista de bloqueados de ese semaforo. 

sem_t *first_sem = NULL;

// crear un nuevo semaforo
int sem_init(const char *str, int initial_count ) {
    // fijarnos que identificador no se haya usado 
    
    sem_t  * sem;
    if ( first_sem == NULL ) {
       first_sem = malloc(sizeof(sem_t));
       sem = first_sem;
    }
    else {
        sem = first_sem;
        while(sem->next != NULL){
            sem = sem->next;
        }

        sem->next = malloc(sizeof(sem_t));
        sem = sem->next;
    }

    sem->identifier = str;
    sem->counter = initial_count;
    sem->blocked_processes = NULL;
    sem->next = NULL;

    return 0;
}

//Buscar el semaforo con el ID = identifier
sem_t *sem_open(const char* str) {
    //busco el semaforo en la lista
    sem_t *current = first_sem;
    while( current != NULL && strcmp(current->identifier,str)) { 
        current = current->next;
    }

    // si es NULL no se encontró el semaforo.
    return current;
}

int sem_wait(const char *identifier) {
    sem_t *sem = sem_open(identifier);
    // si sem es null, el identificador es invalido.
    if (sem == NULL) {
        return 1;
    }

    if ( sem->counter == 0 ){
        // bloquear proceso.

        //Obtengo el pid del proceso running
        int pid = get_pid();

        p_blocked_t  * pblocked;
        if ( sem->blocked_processes == NULL ) {
        sem->blocked_processes = malloc(sizeof(p_blocked_t));
        pblocked = sem->blocked_processes;
        }
        else {
            pblocked = sem->blocked_processes;
            while(pblocked->next != NULL){
                pblocked = pblocked->next;
            }
            
            pblocked->next = malloc(sizeof(p_blocked_t));
            pblocked = pblocked->next;
        }

        pblocked->pid = pid;
        pblocked->next = NULL;

       /*  //Obtengo el nodo del puntero running
        process_t * processNode = get_Pnode(pid);

        // agrego nodo al final de la lista de procesos bloqueados
        
        process_t * temp = sem->blocked_processes;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = processNode; */
        
        //Updateamos el state

        update_process_state(pid, 'b');

        int_20();

        // aca dejo de estar bloqueado, hay que sacarlo de la lista.
        p_blocked_t * prev_c;
        p_blocked_t * c = sem->blocked_processes;
        while (c->next != NULL && c->pid != pid )
        {   
            prev_c = c;
            c = c->next;
        }
        if (c->pid == pid)
        {
            //lo vuelo
            prev_c->next = c->next;
        }else
        {
            //sino retorno error
            return 2;
        }
        
        

        // cuando retoma la ejecucion, sem->counter va a estar en 1.        
    }
    if (sem_down >= 1) {
        sem_down( &sem->counter );
    }

    return 0;
}

int sem_post( const char *identifier ) {
    sem_t *sem = sem_open(identifier);
    // si sem es null, el identificador es invalido.
    if (sem == NULL) {
        return 1;
    }

    if (sem->counter == 0) {
        // vuelo un proceso de la lista de bloqueados
        if( sem->blocked_processes == NULL ) {
            // ningun proceso bloqueado.
        }
        else{
            // saco el primer nodo de la lista.
            p_blocked_t *temp = sem->blocked_processes;

            update_process_state(temp->pid, 'a');

            sem->blocked_processes = sem->blocked_processes->next;

            free_block(temp);
        }
    }

    sem_up( &sem->counter );

    return 0;
}

//Aca cerramos un semaforo. Liberamos la memoria y lo sacamos de la lista
int sem_close(const char *identifier) {
    //Lo saco de la lista
    sem_t * prev_current;
    sem_t * current = first_sem;
    while (current != NULL && strcmp(current->identifier,identifier))
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