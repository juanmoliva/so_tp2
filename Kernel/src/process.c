

static int top_id = 0;




void init_scheduler() {

}

uint64_t create_process() {
    uint64_t new_stack = malloc( 2000 );
    

    // rellenar el stack, que parezca un contexto de proceso.
    // le pasamos la direccion que es nuestro stack pointer al scheduler.

    top_id++;
    int new_pid = top_id;

    return new_pid;
}
