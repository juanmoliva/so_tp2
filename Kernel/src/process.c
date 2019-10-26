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
    unsigned char status; // READY/AVAILABLE ('a') , BLOCKED ('b') OR RUNNING ('r')
    unsigned char ppriority;
    uint64_t ip; // instruction pointer
    uint64_t sp; // stack pointer

    struct process * next;
} process_t;

static unsigned char priority_flag = 0;

//Creo el HEAD de la lista y el current (para iterarla)

// current es el proceso que está en ejecución ahora

process_t * process_list_first, * process_list_current;

void init_scheduler() {
    //Inicializo el puntero al espacio donde va a ir el primer proceso
    process_list_first = malloc( sizeof(process_t) );
    process_list_current = process_list_first;
    // llenar info de primer proceso
    // -----------------------------------------------------------------------------
}

void context_switch() {
    // si la flag de priority está en 1, no hay que cambiar contexto aun.
    if( priority_flag ){
        priority_flag = 0;
        return;
    }

    // llamar scheduler / obtener siguiente proceso.
    // buscar direccion del stack del nuevo proceso.
    // cambiar contexto

    process_t *process_list_previous = process_list_current;

    // pongo en current el siguiente proceso
    if ( process_list_current->next != NULL)
        process_list_current = process_list_current->next;
    else
    {
        process_list_current = process_list_first;
    }
    
    // si la prioridad del nuevo proceso es 0 (top priority), seteamos la priority flag para la proxima llamada a esta funcion.
    if (process_list_current->ppriority == 0) {
        // top priority
        priority_flag = 1;
    }

    // cambiar contextos con funcion en assembler.
    // ( no implementada )
    context_asm(process_list_previous->sp, process_list_current->sp);
    // -----------------------------------------------------------------------------


}

uint64_t create_process(int priority) {
    // Crear estructura del proceso e Inicializar todo
    process_t * temp = malloc( sizeof(process_t) );
    //temp->pid Se hace mas adelante
    temp->ppriority = priority; //Hay que ver este tema ----------------------------------------------
    temp->sp = ;
    temp->ip = ;
    temp->status = 'a';
    temp->next = NULL; 

    // Malloc espacio para el stack
    void * process_stack = malloc( STACK_SIZE );
    //Seteo el STACK

    

    // Lo agrego a la lista de procesos

        //Creo un iterador de la lista
        process_t * current = process_list_first;

        //Ciclo mientras que tenga otro elemento
        while(current->next != NULL){
            //Consumo hasta llegar al final
            current->next;
        }

        temp->pid = (current->pid) + 1;
        current->next = temp;
        
    // Le aviso al scheduler que tiene un proceso mas para ejecutar

    

    //El unico momento donde tocan ASM es en cuanto al timertick. ( Lo puso el cordoba en el campus ).
    // llamar al scheduler con tu SP atcual 


    
    // funcion de assembler que llena el stack con lo que tiene que tener 
    // ( no implementada )
    set_stack(process_stack);
    // -----------------------------------------------------------------------------

}

//Recorro la lista de procesos hasta encontrar el indicado y modifico su prioridad
uint64_t update_process_priority(int pid, int priority) {
    process_t * temp = process_list_first;
    process_finder(pid, temp);
    if(temp != NULL){
        temp->ppriority = priority;
    }else
    {
        return; //ERROR, NO ESTA EL PROCESO
    }
    
}

//Recorro la lista de procesos hasta encontrar el indicado y modifico su state
uint64_t update_process_state(int pid, char state) {
    process_t * temp = process_list_first;
    process_finder(pid, temp);
    if(temp != NULL){
        temp->status = state;
    }else
    {
        return; //ERROR, NO ESTA EL PROCESO
    }
}

void process_finder(int pid, process_t * temp) {
    while(temp != NULL){
        if(temp->pid == pid){
            break;
        }
        temp = temp->next;
    }
    return;
}


