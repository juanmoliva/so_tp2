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
#include <phylo.h>

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

uint64_t memory_handler(uint8_t flag,uint64_t num){
    if(flag) {
        return (uint64_t)free_block((void *)num); // casteo solo por claridad, no tiene uso.
    }
    else {
        return (uint64_t) allocate_blocks(num);
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

uint64_t create_process_handler(void *rip, char *name, uint64_t param) {
    return create_process(3, rip, name, param);
}

uint64_t update_process_priority_handler(int pid, int priority) {
    return update_process_priority(pid, priority);
}

uint64_t update_process_state_handler(int pid, char state) {
    return update_process_state(pid ,state);
}

/// tp2

uint64_t list_processes_handler() {
    return (uint64_t) process_list_returner();
}


////PIPES
uint64_t list_pipes_handler() {
    return (uint64_t) list_pipes();
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

uint64_t open_sem_handler(int identifier) {
    return (uint64_t) sem_open(identifier);
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
    int res;
    if ( is_blocked(pid) ) {
        res = update_process_state(pid,'a');
        if (res) {
            return -1;
        }
        else return 0;
    }
    else {
        res = update_process_state(pid,'b');
        if (res) {
            return -1;
        }
        else return 1;
    }
    return 0;
}

int nice_handler(int pid, int priority){

    int result = update_process_priority(pid, priority);
    if (result == -1){
        //puts("Pid invalido");
    }
    else if ( result == -2){
        // puts("prioridad invalida, pruebe con numeros enteros positivos menores o iguales a 10");
    }
    else {
        // puts("Prioridad cambiada con exito");
        
    }
    return result;

}

void loop_handler(){
    int mypid = get_pid();
    every_n_seconds_procecess(mypid);
}

int phylo_handler(int flag){
    //Si es un "1" -> add , si es "0" -> remove
    if(flag){
        return add_phylo();
    }
    else{
        return remove_phylo();
    }
    return 0;
}

int phylo_state_handler(int id_phylo){
    //Esto tiene que devolver el estado , THINKING 0, HUNGRY 1, EATING 2
    return get_state_phylosopher(id_phylo);
}

int get_mm_handler() {
    return get_system();
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