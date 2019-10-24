#include <stdint.h>

#define STACK_SIZE 2000

static int top_id = 0;




void init_scheduler() {

}

uint64_t create_process() {
    uint64_t new_stack = malloc( STACK_SIZE );
    

    // rellenar el stack, que parezca un contexto de proceso.
    // le pasamos la direccion que es nuestro stack pointer al scheduler.

    top_id++;
    int new_pid = top_id;

    return new_pid;
}
