// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memory.h>
#include <string.h>
#include <stdint.h>
#define BLOCKSIZE 0x40

static void * start_memsegment = (void*)0x1000000;
static void * end_memsegment = (void*)0x1100000;

typedef struct node {
    unsigned char free;
    unsigned long blocks;
    void *startAddr;
    struct node * next;
} node_t;

static node_t *first = (node_t *) 0x1000000;

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

//Aca inicializamos el Memory Manager
void init_mm() {
    first->free = 1;
    first->next = NULL;
    first->startAddr = start_memsegment;
    first->blocks = 0x100000 / BLOCKSIZE;
}

//Aca alocamos los bytes que nos pide el usuario
uint64_t allocate_blocks(uint64_t bytes) {
    
    // si da justo entero se rompe.
/////////////////////////////////////////////////////////////////////////////////////////////
    //Aca calculamos cant de pags
    unsigned long needed_blocks = ( bytes + sizeof(node_t) / 0x40 ) + 1; 

    //Preparo un puntero para iterar
    node_t *current = first;

    //itero hasta encontrar el que sirve o se me acabo la lista
    while( current != NULL ) {
        if (current->free && current->blocks >= needed_blocks ) {
            break;
        }
        else {
            current = current->next;
        }
    }

    //Nos fuimos alfinal y no hay lugar
    if (current == NULL ) {
        return -1;
    }
    // creamos un puntero al a la direccion correspondiente + tam requerido y metemos un nuevo nodo 
    else {
        node_t *aux = current->startAddr + needed_blocks*BLOCKSIZE;
        
        
        aux->next = current->next;
        aux->blocks = current->blocks - needed_blocks;
        aux->free = 1;
        aux->startAddr = current->startAddr + needed_blocks*BLOCKSIZE;

        current->free = 0;
        current ->blocks = needed_blocks;
        current->next = aux->startAddr;


        return (uint64_t)(current->startAddr + sizeof(node_t));
    }
}

uint64_t free_block(uint64_t address) {
    // Si el bloque de memoria al que pertenece address fue asignado previamente, se "libera".
    // sino, no se hace nada.

    node_t *current = first;

    //Recorro todo hasta encontrar el lugar que me pasaron
    while ( current != NULL ) {
        if ( (uint64_t)(current->startAddr + sizeof(node_t)) == address )
        {
            break;
        }
        else {
            current = current->next;
        }
    }
    //Sale por dos casos: 1) Se termino la lista     2) Lo encontro
    if( current == NULL ) {
        return 1;
    }
    else {
        current->free = 1;

        // FUSION DE SEGMENTOS
//////////////////////////////////////////////////////////////////////////////////////////////


        return 0;
    }
}

uint64_t cur_free_mem() {
    // devuelve la cantidad de memoria libre que hay.

    // Voy a tomar el first y recorrer todo sumando los free blocks
    node_t *current = first;
    unsigned long freeBlocks = 0;

    while (current != NULL)
    {
        if(current->free == 1){
            freeBlocks += current->blocks;
        }
        current=current->next;
    }
    
    return freeBlocks * BLOCKSIZE;
}

uint64_t total_mem(uint64_t address) {
    // devuelve la cantidad de memoria total que se administra.

    return end_memsegment - start_memsegment;
}