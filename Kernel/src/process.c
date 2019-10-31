#include <stdint.h>
#include <lib.h>
#include <string.h>
#include <linked_list.h>
#include <process.h>

/* TO DO
base pointer de primer proceso??
ERROR handling en schedule()
*/

//Define el tamaño del stack
#define STACK_SIZE 2000
#define MAX_PID 50

//Este .c va a crear un proceso nuevo y administrar los existentes. Para que esto suceda se va a llevar un trackeo de todos los procesos en una lista de structs. 
//Teniendo en cada struct toda la informacion de cada proceso. Esto va a permitir cambiar el proceso que corre facilmente

int get_free_pid();

//Creo la struct
typedef struct process {
    unsigned char status; // READY/AVAILABLE ('a') , BLOCKED ('b') OR RUNNING ('r')
    unsigned char ppriority;
    void *bp;
    void *sp; // stack pointer

    struct process * next;
} process_t;

// PROCESOS QUE EXISTEN.process_t *process_list[MAX_PID];
process_t *process_list[MAX_PID]; 

static unsigned char priority_flag = 0;

//Creo el HEAD de la lista del scheduler.
process_t * process_list_first, *process_list_current;

int init_scheduler() {
    // se llena la info del primer proceso.

    int pid = get_free_pid();
    
    // devuelve -1 si no quedan lugares para el proceso
    if(pid == -1) {
        return -1;
    }

    // Crear estructura del proceso e Inicializar todo
    process_t * first = (process_t *) malloc( sizeof(process_t) );
    if( first == NULL ){
        return -1;
    }
    first->ppriority = 1;
    first->status = 'r'; 
    
    //Inserto en array
    process_list[pid]= first;

    // agrego el proceso a la lista del scheduler.
    add( (void **) &process_list_first, process_list[pid], sizeof(process_t));
    process_list_current = process_list_first;
}

int create_process(int priority, void *rip) {
    int pid = get_free_pid();
    
    // devuelve -1 si no quedan lugares para el proceso
    if(pid == -1) {
        return -1;
    }

    // Crear estructura del proceso e Inicializar todo
    process_t * temp = (process_t *) malloc( sizeof(process_t) );
    if( temp == NULL ){
        return -1;
    }

    //temp->pid Se hace mas adelante
    temp->ppriority = priority;
    temp->status = 'a'; 
    
    //Inserto en array
    process_list[pid]= temp;


    // Malloc espacio para el stack
    void *process_stack = malloc( STACK_SIZE );
    if ( process_stack == NULL ) {
        // error en malloc.
        return -1;
    }

    process_list[pid]->bp = process_stack;
    process_list[pid]->sp = process_list[pid]->bp + STACK_SIZE ;

    //Seteo el STACK
    set_stack(process_list[pid]->sp,rip);

    // Lo agrego a la lista de procesos
    add( (void **) &process_list_first, process_list[pid], sizeof(process_t));

    return pid;
}

void *schedule(void *prev_rsp) {

    // si la flag de priority está en 1, no hay que cambiar contexto aun.
    if( priority_flag ){
        priority_flag = 0;
        return prev_rsp;
    }
   
   process_list_current->sp = prev_rsp;
   process_list_current->status = 'a';

   // cambiamos el proceso actual.
   process_list_current = next(process_list_first, process_list_current);
   if ( process_list_current == NULL ) {
       // ERROR
   }

   process_list_current->status = 'r';

    // si la prioridad del nuevo proceso es 0 (top priority), seteamos la priority flag para la proxima llamada a esta funcion.
    if(process_list_current->ppriority == 0) {
        // top priority
        priority_flag = 1;
    }

    return process_list_current->sp;
}

int get_free_pid() {
    // Devuelve -1 si no hay lugar, sino el lugar.
    for (int i = 0; i < MAX_PID; i++)
    {
        if (process_list[i]==0)
        {
            return i;
        }
        
    }
    return -1;
}

//Recorro la lista de procesos hasta encontrar el indicado y modifico su prioridad
uint64_t update_process_priority(int pid, int priority) {
    
    if ( pid >= MAX_PID || pid < 0 || process_list[pid] == NULL ) {
        //pid invalido
        return -1;
    }
    if (priority != 0 && priority != 1) {
        // prioridad invalida
        return -2;
    }

    process_list[pid]->ppriority = priority;
    return 0;
}

//Recorro la lista de procesos hasta encontrar el indicado y modifico su state
uint64_t update_process_state(int pid, char state) {
    if ( pid >= MAX_PID || pid < 0 || process_list[pid] == NULL ) {
        //pid invalido
        return -1;
    }
    if ( state != 'a' && state != 'b' ) {
        // estado invalido
        return -2;
    }

    process_list[pid]->status = state;
    return 0;
}


/* uint64_t list_processes() {
    process_t * temp = process_list_first;
    while(temp != NULL){
        //printf !!!! Aca printeo cada uno
        temp = temp->next;
    }

} */