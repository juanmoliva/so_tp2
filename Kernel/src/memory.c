// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memory.h>

static void * start_memsegment = (void*)0x1000000;
static void * end_memsegment = (void*)0x1100000;

/*
    2^20 bytes de memoria.
    bloques o paginas de 64 bytes.
*/

/* tenemos una cierta cantidad de memoria libre para administrar en bytes:
    - separamos la memoria en bloques
    - cuando llega una syscall pidiendo una cierta cantidad de bytes, nos fijamos cuantos bloques necesita,
    y devolvemos una dirección con esa cantidad de bloques libres, y guardamos en un array o algo la direccion asignada y la cantidad de bloques.
    - cuando llega una syscall pidiendo liberar la memoria de una direccion, liberamos los bloques que asignamos previamente.
*/


uint64_t allocate_block(uint64_t bytes) {
    unsigned long needed_blocks = bytes / 64 + 1; 


    // devolvemos el comienzo de la zona de memoria libre.
    unsigned long addr = start_memsegment;
    
    // aumentamos el puntero al comienzo de zona libre.
    start_memsegment += bytes;
    return addr;
}

uint64_t free_block(uint64_t address) {
    // Si el bloque de memoria al que pertenece address fue asignado previamente, se "libera".
    // sino, no se hace nada.

    return 1;
}

uint64_t cur_free_mem() {
    // devuelve la cantidad de memoria libre que hay.
    
    // HARCODEADO
    return end_memsegment - start_memsegment;
}

uint64_t total_mem(uint64_t address) {
    // devuelve la cantidad de memoria total que se administra.

    return end_memsegment - start_memsegment;
}