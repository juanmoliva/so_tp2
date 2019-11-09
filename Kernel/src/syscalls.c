// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscalls.h>
#include <keyboard.h>
#include <time.h>
#include <console.h>
#include <naiveConsole.h>
#include <sound.h>
#include <memory.h>
#include <process.h>
#include <videoDriver.h>
#include <strings.h>
#include <pipes.h>
#include <synchro.h>

void read_handler(uint64_t fd, char * buff, uint64_t count) {
    // File descriptor doesn't matter
    for (int i = 0; i < count; i ++)
        buff[i] = read_character();
}

void write_handler(uint64_t fd, const char * buff, uint64_t count) {
    switch (fd) {
        case STDIN:
        case STDOUT:
            print_N(buff, count);
            //ncPrint(buff);
            break;
        case STDERR:
            printError_N(buff, count);
            //ncPrintError(buff);
            break;
        default:
            break;
    }
}

uint64_t time_handler() {
    return ticks_elapsed();
}

void clear_handler() {
    clear_console();
	// ncClear();
}

uint64_t rtc_handler(uint8_t reg){
    write_port(0x70, reg);
    uint64_t aux = read_port(0x71);
    return (aux >> 4) * 10 + (aux & 0x0F);
}

void sleep_handler(uint64_t millis){
    sleep(millis);
}

void beep_handler(uint16_t frequency, uint64_t millis) {
    play_sound(frequency);
    sleep_handler(millis);
    no_sound();
}

void pixel_handler(uint64_t x, uint64_t y, uint64_t rgb) {
    Vector2 auxPos = {x, y};
    Color auxColor = {(rgb & 0xFF0000) >> 16, (rgb & 0x00FF00) >> 8, rgb & 0x0000FF};
    draw_pixel(auxPos, auxColor);

    // ncPrintDec(rgb);
    // ncNewline();
    // ncPrintDec(auxColor.r);
    // ncNewline();
    // ncPrintDec(auxColor.g);
    // ncNewline();
    // ncPrintDec(auxColor.b);
    // ncNewline();
}

void *memory_handler(uint8_t flag,int num){
    if(flag) {
        return free_block(num);
    }
    else {
        return allocate_blocks(num);
    }
}

uint64_t memory_state_handler(uint8_t free){
    if (free) {
        return cur_free_mem();
    }
    else {
        return total_mem();
    }
}

uint64_t create_process_handler(void *rip, int priority, char *name) {
    return create_process(priority, rip, name);
}

uint64_t update_process_priority_handler(int pid, int priority) {
    return update_process_priority(pid, priority);
}

uint64_t update_process_state_handler(int pid, char state) {
    return update_process_state(pid ,state);
}

/// tp2

char* list_processes_handler() {
    char ps[500]; 
    int j = 0;
    process_t ** list = process_list_returner();
     for (int i = 0; i < MAX_PID; i++)
        {
            if(list[i]!= NULL){
               // ps[j++] = int_to_string(i,  ,10);
                int_to_string(i, &ps[j++] ,10);
            }
       int_to_string(-1, &ps[j++] ,10);
        }
    ps[j]= "endofdtring";
    
    return ps;
}


////PIPES
uint64_t list_pipes_handler() {
    return list_pipes();
}
////////////
int get_pid_handler() {
    return get_pid();
}

int kill_process_handler(int pid) {
    return kill_process(pid);
}



// SEMAFOROS

sem_t * list_sem_handler() {
    return sem_list();
}

// int init_sem_handler(char *identifier, int count) {
//     return sem_init(identifier, count);
// }

// int open_sem_handler(char *identifier) {
//     return sem_open(identifier);
// }

// int close_sem_handler(char *identifier) {
//     return sem_close(identifier);
// }

// int sem_wait_handler(char *identifier) {
//     return sem_wait(identifier);
// }

// int sem_post_handler(char *identifier) {
//     return sem_post(identifier);
// }

int init_sem_handler(int identifier, int count) {
    return sem_init(identifier, count);
}

int open_sem_handler(int identifier) {
    return sem_open(identifier);
}

int close_sem_handler(int identifier) {
    return sem_close(identifier);
}

int sem_wait_handler(int identifier) {
    return sem_wait(identifier);
}

int sem_post_handler(int identifier) {
    return sem_post(identifier);
}

///////////////////////////////////////////////////






int create_pipe_handler(char *identifier) {
    return create_pipe(identifier);
}

int write_pipe_handler(char *identifier, char *str) {
    return write_pipe(identifier, str);
}

int read_pipe_handler(char *identifier, char *buff) {
    return read_pipe(identifier, buff);
}

int block_process_handler(int pid){
    return update_process_state(pid,'b');
}

void nice_handler(int pid, int priority){

    int result = update_process_priority(pid, priority);
    if (result == -1){
        puts("Pid invalido")
    }
    else if ( result == -2){
        puts("prioridad invalida, pruebe con numeros enteros positivos menores o iguales a 10");
    }
    else {
        puts("Prioridad cambiada con exito");
    }
}

void loop_handler(){
    int mypid = getpid();
    every_n_second_procecess(mypid);
}

//Done & Tested:

// Done:
// ● help:​ muestra una lista con todos los comandos disponibles
// ● mem: ​Imprime el estado de la memoria.

//by juli
// ● kill: ​Mata un proceso dado su ID.
// ● sem: Imprime la lista de todos los semáforos con sus propiedades: estado, los
// procesos bloqueados en cada uno y cualquier otra variable que consideren
// necesaria.
// ● pipe: Imprime la lista de todos los pipes con sus propiedades: estado, los procesos
// bloqueados en cada uno y cualquier otra variable que consideren necesaria.


//by davor
// ● nice:​ Cambia la prioridad de un proceso dado su ID y la nueva prioridad.
// ● block:​ Cambia el estado de un proceso entre ​bloqueado ​y ​listo​ dado su ID.
// ● ps: ​Imprime la lista de todos los procesos con sus propiedades: nombre, ID,
// prioridad, stack y base pointer, foreground y cualquier otra variable que consideren
// necesaria.
// ● loop:​ Imprime su ID con un saludo cada una determinada cantidad de segundos.




// A ESTO LE FALTA RECIBIR MAS DE UN PARAMETRO!!!!!!!!
// sh: Shell de usuario que permita ejecutar las aplicaciones. ​
//DEBERÁ contar con algún mecanismo para determinar si va a ceder o no el foreground al proceso que se ejecuta, 
//por ejemplo, bash ejecuta un programa en background cuando se agrega el símbolo “&” al final de un comando. 
//Este requisito es ​MUY importante para poder demostrar el funcionamiento del sistema en general ya que 
//en la mayoría de los casos es necesario ejecutar más de 1 proceso. 
//La shell también ​DEBERÁ permitir conectar 2 procesos mediante un pipe, 
//por ejemplo, bash hace esto al agregar el símbolo “|” entre los 2 programas a ejecutar.

// Missing:
// ● cat: ​Imprime el stdin tal como lo recibe.
// ● wc: ​Cuenta la cantidad de líneas del input.(((CANTIDA DE /n's)))
// ● filter​: Filtra las vocales del input.

//VERY HARD
// ● phylo: Implementa el problema de los filósofos comensales. ​
// DEBERÁ permitir cambiar la cantidad de filósofos en runtime con las teclas “a” (add 1) y “r” (remove 1).
// También ​DEBERÁ ​mostrar el estado de la mesa de forma sencilla pero legible.


