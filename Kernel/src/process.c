#include <stdint.h>
#include <lib.h>
#include <string.h>
#include <process.h>
#include <memory.h>

/* TO DO
base pointer de primer proceso??
ERROR handling en schedule()
*/

//Define el tamaño del stack
#define STACK_SIZE 2000
#define MAX_PID 50
#define QUANTUM 3

//Este .c va a crear un proceso nuevo y administrar los existentes. Para que esto suceda se va a llevar un trackeo de todos los procesos en una lista de structs. 
//Teniendo en cada struct toda la informacion de cada proceso. Esto va a permitir cambiar el proceso que corre facilmente

int get_free_pid();

// PROCESOS QUE EXISTEN.process_t *process_list[MAX_PID];
process_t *process_list[MAX_PID]; 

int prior_counter = -1;
int scheduler = 0;

//Creo el HEAD de la lista del scheduler.
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
    temp->fd[0] = 0 ;// STDIN
    temp->fd[1] = 1 ;// STDOUT

    // Malloc espacio para el stack
    void *process_stack = malloc( STACK_SIZE );
    if ( process_stack == NULL ) {
        // error en malloc.
        return -1;
    }

    if ( pid!= 0) {
        // no es el primer proceso, va a ponerse en la lista del scheduler y ser administrado por el mismo.
    
        //Inserto en array
        process_list[pid]= temp;

        process_list[pid]->bp = process_stack;
        process_list[pid]->sp = process_list[pid]->bp + STACK_SIZE ;

        //Seteo el STACK
        set_stack(process_list[pid]->sp,rip);

        // agrego el proceso a la lista del scheduler.
        node_t *process_node = (node_t *) malloc(sizeof(node_t));
        if(process_node == NULL) {
            return -1;
        }
        process_node->pid = pid;
        process_node->next = NULL;
        
        node_t *curr = process_list_first;
        if (curr == NULL) {
            // primer proceso en lista.
            process_list_first =  process_node;
            process_list_current = process_list_first;
        } else {
             while(curr->next != NULL ) {
            curr=curr->next;
            }
            curr->next = process_node;
        }
        
        scheduler = 1;
    } else {
        // si pid es 0, es init(), tiene coronita y con él se inicializa el scheduler.
        // no se agrega a la lista del scheduler.
       
        //Inserto en array
        process_list[pid]= temp;

        process_list[pid]->bp = process_stack;
        process_list[pid]->sp = process_list[pid]->bp + STACK_SIZE ;

        //Seteo el STACK
        set_stack(process_list[pid]->sp,rip);
    }

    return pid;
}


void *schedule(void *prev_rsp) {
    // si process_list[0] es NULL --> init() no se inicializó, 
    //                                  devuelvo el mismo sp y que el kernel siga con la inicializacion.
    // si prior_counter es mayor o igual a 0 --> todavia no termino el ciclo del proceso actual, así que el scheduler no hace nada.
    if ( !scheduler ) {
        return prev_rsp;
    }
    if ( prior_counter >= 0 ) {
        prior_counter--;
        return process_list[process_list_current->pid]->sp;
    }

    // si la lista del scheduler está vacía, no hay ningún proceso: llamamos a init, que va a crear la shell.
    if ( process_list_current == NULL) {
        return  NULL;
        return process_list[0]->sp;
    }

   int curr_pid = process_list_current->pid;

   // cambiamos la info del proceso viejo.
   process_list[curr_pid]->sp = prev_rsp;
   process_list[curr_pid]->status = 'a';

   // cambiamos el proceso actual.
   if (process_list_current->next == NULL) {
       process_list_current = process_list_first;
   }
   else {
       process_list_current = process_list_current->next;
   }

   int new_pid = process_list_current->pid;
   
   process_list[new_pid]->status = 'r';
    
    // si la prioridad del nuevo proceso es 0 (top priority), seteamos la priority flag para la proxima llamada a esta funcion.
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

