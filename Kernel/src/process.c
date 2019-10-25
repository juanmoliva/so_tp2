#include <stdint.h>
#include <lib.h>
#include <string.h>

//Define el tamaño del stack
#define STACK_SIZE 2000

//Este .c va a crear un proceso nuevo y administrar los existentes. Para que esto suceda se va a llevar un trackeo de todos los procesos en una lista de structs. 
//Teniendo en cada struct toda la informacion de cada proceso. Esto va a permitir cambiar el proceso que corre facilmente

//Creo la struct
typedef struct process {
    unsigned long pid;
    unsigned char status; // READY, BLOCKED OR RUNNING
    unsigned char ptiority;
    uint64_t ip; // instruction pointer
    uint64_t sp; // stack pointer

    struct process * next;
} process_t;

//Creo el HEAD de la lista
process_t * process_list_first, *process_list_current;

void init_scheduler() {
    //Inicializo el puntero al primer proceso
    process_list_first = malloc( sizeof(process_t) );
    process_list_current = process_list_first;
    // llenar info de primer proceso
    // -----------------------------------------------------------------------------
}

void context_switch() {
    // llamar scheduler / obtener siguiente proceso.
    // buscar direccion del stack del nuevo proceso.
    // cambiar contexto

    process_t *process_list_previous = process_list_current;

    // pongo en current el siguiente proceso
    // esto deberia ser basado en prioridad
    // ------------------------------------------------------------------------------
    if ( process_list_current->next != NULL)
        process_list_current = process_list_current->next;
    else
    {
        process_list_current = process_list_first;
    }
    
    // cambiar contextos con funcion en assembler.
    // ( no implementada )
    context_asm(process_list_previous->sp, process_list_current->sp);
    // -----------------------------------------------------------------------------


}

uint64_t create_process() {
    //Creo un iterador de la lista
    process_t * current = process_list_first;

    //Ciclo mientras que tenga otro elemento
    while(current->next != NULL){
        //Consumo hasta llegar al final
        current->next;
    }

    current->next = malloc( sizeof(process_t) ); 
    
    // llenar el nodo nuevo con info.
    // -----------------------------------------------------------------------------

    uint64_t new_stack = malloc( STACK_SIZE );
    
    // funcion de assembler que llena el stack con lo que tiene que tener 
    // ( no implementada )
    set_stack(new_stack);
    // -----------------------------------------------------------------------------
    

    top_id++;
    int new_pid = top_id;

    return new_pid;
}
