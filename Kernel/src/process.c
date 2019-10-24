#include <stdint.h>
#include <string.h>

//Define el tamaño del stack
#define STACK_SIZE 2000
//Define la dimension de la lista de procesos posibles
#define DIM 50

static int top_id = 0;

//Este .c va a crear un proceso nuevo. Para que esto suceda se va a llevar un trackeo de todos los procesos en un arreglo de structs. 
//Teniendo en cada struct toda la informacion de cada proceso. Esto va a permitir cambiar el proceso que corre facilmente

//Creo la struct
typedef struct process {
    //Aca hay que poner toda la info del proceso!!!
    struct process * next;
} process_t;

//Creo el HEAD de la lista
    process_t * process_list_first;

void init_process_list() {
    //Inicializo el puntero al primer proceso
    process_list_first = malloc( sizeof(process_t) );
}

void init_scheduler() {
    // crear struct??
}

void context_switch() {
    // llamar scheduler / obtener siguiente proceso.
    // buscar direccion del stack del nuevo proceso.
    // cambiar contexto
}

uint64_t create_process() {
    //Creo un iterador de la lista
    process_t * current = process_list_first;
    //Ciclo mientras que tenga otro elemento
    while(current->next != NULL){
        //Consumo hasta llegar al final
        current->next;
    }

    //Una vez llegado tengo que agregar el nuevo proceso a la lista
    //........
     












    uint64_t new_stack = malloc( STACK_SIZE );
    

    // rellenar el stack, que parezca un contexto de proceso.
    // le pasamos la direccion que es nuestro stack pointer al scheduler.

    top_id++;
    int new_pid = top_id;

    return new_pid;
}
