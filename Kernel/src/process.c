#include <stdint.h>
#include <lib.h>
#include <string.h>
#include <process.h>
#include <memory.h>

//Define el tamaño del stack
#define STACK_SIZE 2000
//Definimos maxima cantidad de ID's
#define MAX_PID 50
//Minima cantidad de timer ticks
#define QUANTUM 3

// Este .c va a crear un proceso nuevo y administrar los existentes. 
// Para que esto suceda se va a llevar un trackeo de todos los procesos en un array de structs. 
// Teniendo en cada struct toda la informacion de cada proceso. 
// Esto va a permitir cambiar el proceso que corre facilmente.

int get_free_pid();

// PROCESOS QUE EXISTEN (Todos los procesos que estan creados!!! Bloqueados, Running, Available)
process_t *process_list[MAX_PID]; 

// contador actual de prioridad.
int prior_counter = -1;

// Flag que dice si el scheduler esta andando o no. Sirve para el caso de que no tenga ningun
// proceso en la lista y no rompa.
int scheduler = 0;
int virgin = 1;

//Creo el HEAD y el CURRENT de la lista del scheduler.
node_t * process_list_first = NULL;
node_t *process_list_current = NULL;

int init_scheduler() {
   /*  // se llena la info del primer proceso.

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
    first->fd[0] = 0; // STDIN
    first->fd[1] = 1; // STDOUT
    
    //Inserto en array
    process_list[pid]= first;

    // agrego el proceso a la lista del scheduler.
    process_list_first = malloc(sizeof(node_t));
    if(process_list_first == NULL) {
        return -1;
    }
    process_list_first->pid = pid;
    process_list_first->next = NULL;
    process_list_current = process_list_first; */

    return 0;
}

// Crea un procesos con la respectiva PRIORIDAD y su IP
int create_process(int priority, void *rip, char * name) { ///////
    // Asignamos al nuevo proceso un ID libre
    int pid = get_free_pid();
    // Devuelve -1 si no quedan lugares para el proceso
    if(pid == -1) {
        return -1;
    }

    // Crear estructura del proceso e Inicializar todo
    process_t * temp = (process_t *) malloc( sizeof(process_t) );
    //Verificamos lo que devuelve el malloc!
    if( temp == NULL ){
        return -1;
    }

    //temp->pid Se hace mas adelante
    //temp->pname = name;
    temp->ppriority = priority;
    temp->status = 'a'; 
    temp->fd[0] = 0 ;// STDIN
    temp->fd[1] = 1 ;// STDOUT

    // Malloc espacio para el stack del proceso
    void *process_stack = malloc( STACK_SIZE );
    // Chequeo error
    if ( process_stack == NULL ) {
        // error en malloc.
        return -1;
    }

    if ( pid!= 0) {
        // no es el primer proceso, va a ponerse en la lista del scheduler y ser administrado por el mismo.

        //Inserto en array
        process_list[pid]= temp;
        // Asignamos un stack para el proceso. El BP arranca con el puntero abajo de todo
        // El SP arranca arriba de todo porque POP y PUSH funcionan desde arriba!!!!
        process_list[pid]->bp = process_stack;
        process_list[pid]->sp = process_list[pid]->bp + STACK_SIZE ;

        // Seteo el STACK, le pasamos el SP y el IP.
        void *updated_sp = set_stack(process_list[pid]->sp,rip);
        // Actualizo el SP una vez que lo tocamos en ASM
        process_list[pid]->sp = updated_sp;

        // Agrego el proceso a la lista del scheduler.
        // Hago lugar para el NODO
        node_t *process_node = (node_t *) malloc(sizeof(node_t));
        //Verifico error
        if(process_node == NULL) {
            return -1;
        }
        //Seteo NODO. Pongo el PID y va con NULL porque va a final de la lista
        process_node->pid = pid;
        process_node->next = NULL;
        
        //Seteo Curr al primer proceso de scheduler
        node_t *curr = process_list_first;
        
        if (curr == NULL) { 
            // primer proceso en lista.
            process_list_first =  process_node;
            process_list_current = process_list_first;

            //Como tenemos procesos en la lista el scheduler lo prendemos!!!
            scheduler = 1;
        } else {
            //Si no es el primer procesos lo encadenamos alfinal
             while(curr->next != NULL ) {
            curr=curr->next;
            }
            curr->next = process_node;
        }
        
        
    } else {
        // si pid es 0, es init(), tiene coronita y con él se inicializa el scheduler.
        // no se agrega a la lista del scheduler.
       
        //Inserto en array de procesos
        process_list[pid]= temp;
        // Asignamos un stack para el proceso. El BP arranca con el puntero abajo de todo
        // El SP arranca arriba de todo porque POP y PUSH funcionan desde arriba!!!!
        process_list[pid]->bp = process_stack;
        process_list[pid]->sp = process_list[pid]->bp + STACK_SIZE ;

        // Seteo el STACK, le pasamos el SP y el IP.
        void *updated_sp = set_stack(process_list[pid]->sp,rip);
        // Actualizo el SP una vez que lo tocamos en ASM
        process_list[pid]->sp = updated_sp;
    }

    return pid;
}

//Esta funcion se llama cada timer tick. Recibe el SP del proceso que se estaba ejecutando antes de la interrupcion.
void *schedule(void *prev_rsp) {
    // 3 CASOS:
    // 1) Si process_list[0] es NULL --> init() no se inicializó, 
    //    devuelvo el mismo sp y que el kernel siga con la inicializacion.
    // 2) Si la lista del scheduler esta vacia
    // 3) Si prior_counter es mayor o igual a 0 --> todavia no termino el ciclo del proceso actual, así que el scheduler no hace nada.
    
    // CASO 1 (y 2 solo antes de que el caso 1 pase) 
    if ( !scheduler ) {
        return prev_rsp;
    }
    // CASO 3
    if ( prior_counter >= 0 ) {
        prior_counter--;
        return process_list[process_list_current->pid]->sp;
    }

    // CASO 2 se retoma aca  
    // Si la lista del scheduler se quedó sin procesos en el medio de ejecucion (voló la shell) 
    // Llamamos a init, que va a crear la shell.
    if ( process_list_first == NULL) {
        return process_list[0]->sp;
    }

    //Si llegamos hasta aca hay que hacer un swapp de procesos!!!
    // Objetivos: 
    // 1) Actualizar el SP del proceso viejo para la proxima vez que se llame (Actualizo los movimientos de ASM en C)
    // 2) Pasar al proximo proceso (Recorro de forma circular, podria repetirse)
    // 3) Actualizo los estados de los procesos en el arreglo. El viejo en "AVAILABLE" y el nuevo en "RUNNING".

    // CASO 1 //////////////////////////////////////
    // Me guardo el PID del viejo para luego poder modificar el estado en el array.
    int curr_pid = process_list_current->pid;
    // Actualizamos el SP y el ESTADO del proceso viejo.
    // el primer stack pointer recibido no lo metemos en el nodo.
    if ( !virgin ) {
        process_list[curr_pid]->sp = prev_rsp;
    } else {
        virgin = 0;
    }
    process_list[curr_pid]->status = 'a';
    ////////////////////////////////////////////////

    // CASO 2 //////////////////////////////////////
    // Pasamos el proximo proceso.
    if (process_list_current->next == NULL) {
        process_list_current = process_list_first;
    }
    else {
        process_list_current = process_list_current->next;
    }
    ////////////////////////////////////////////////
    
    // CASO 3 //////////////////////////////////////
    // Obtengo nuevo PID
    int new_pid = process_list_current->pid;
    // Actualizo el estado del nuevo Proceso
    process_list[new_pid]->status = 'r';
    
    // el prior_counter tiene el numero de ciclos que va a correr el proceso, se calcula con el QUANUM + ppriority
    prior_counter = QUANTUM + process_list[new_pid]->ppriority;

    return process_list[new_pid]->sp;
}

int get_free_pid() {
    // Devuelve -1 si no hay lugar, sino el lugar.
    for (int i = 0; i < MAX_PID; i++)
    {
        if (process_list[i]==NULL)
        {
            return i;
        }
        
    }
    return -1;
}

//Recorro la lista de procesos hasta encontrar el indicado y modifico su prioridad
int update_process_priority(int pid, int priority) {
    
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
int update_process_state(int pid, char state) {
    if ( pid >= MAX_PID || pid < 0 || process_list[pid] == NULL ) {
        //pid invalido
        return -1;
    }
    if ( state != 'a' && state != 'b' ) {
        // estado invalido
        return -2;
    }
    //si es 'b' va a haber que sacarlo de running y ponerlo en una lista de 
    // bloqueado
    process_list[pid]->status = state;
    return 0;
}

//Recorro los procesos y devuelvo el running.
int get_pid() {
    for (int i = 0; i < MAX_PID; i++)
    {if(process_list[i]->status=='r') return i;}
    return -1;
}

// devuelvo el nodo del processo ; recibo el pid
process_t* get_Pnode(int pid){
    if (pid >= MAX_PID || pid < 0){
        return NULL;
    }
    return process_list[pid];
}

// devuelve pid ; pasandole el puntero
int get_Process_Pid(process_t* process){
    for (int i = 0; i < MAX_PID ; i++){
        if(process_list[i]== process){return i;}
    }
    return -1;
    
}

// Kill process
uint64_t kill_process(int pid) {
    // CHECKEAR MAX_PID
    // no se puede matar el proceso 0.
    // Liberamos la memoria del proceso.
    // 
    free_block(process_list[pid]->bp);

    return 0;

}


/* uint64_t list_processes() {
    process_t * temp = process_list_first;
    while(temp != NULL){
        //printf !!!! Aca printeo cada uno
        temp = temp->next;
    }

} */

