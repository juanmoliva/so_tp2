// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memory.h>
#include <stdint.h>
#define BLOCKSIZE 0x40
#define NULL 0

static void * start_memsegment = (void*)0x1000000;
static void * end_memsegment = (void*)0x1500000;



node_mem_t *first_mem_node = (node_mem_t *)0x1000000;

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
    first_mem_node->free = 1;
    first_mem_node->next = NULL;
    first_mem_node->startAddr = start_memsegment;
    first_mem_node->blocks = (int) (end_memsegment - start_memsegment) / BLOCKSIZE;
}

//Aca alocamos los bytes que nos pide el usuario
void *allocate_blocks(int bytes) {
    
    // si da justo entero se rompe.
/////////////////////////////////////////////////////////////////////////////////////////////
    //Aca calculamos cant de pags
    unsigned long needed_blocks = ( bytes + sizeof(node_mem_t) / 0x40 ) + 1; 

    //Preparo un puntero para iterar
    node_mem_t *current = first_mem_node;

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
        return NULL;
    }
    // creamos un puntero al a la direccion correspondiente + tam requerido y metemos un nuevo nodo 
    else {
        node_mem_t *aux = current->startAddr + needed_blocks*BLOCKSIZE;
        
        
        aux->next = current->next;
        aux->blocks = current->blocks - needed_blocks;
        aux->free = 1;
        aux->startAddr = current->startAddr + needed_blocks*BLOCKSIZE;

        current->free = 0;
        current ->blocks = needed_blocks;
        current->next = aux->startAddr;


        return (void *)(current->startAddr + sizeof(node_mem_t));
    }
}

int free_block(void *address) {
    // Si el bloque de memoria al que pertenece address fue asignado previamente, se "libera".
    // sino, no se hace nada.

    node_mem_t *current = first_mem_node;

    //Recorro todo hasta encontrar el lugar que me pasaron
    while ( current != NULL ) {
        if ( (void *)(current->startAddr + sizeof(node_mem_t)) == address )
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

int cur_free_mem() {
    // devuelve la cantidad de memoria libre que hay.

    // Voy a tomar el first y recorrer todo sumando los free blocks
    node_mem_t *current = first_mem_node;
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

int total_mem(void *address) {
    // devuelve la cantidad de memoria total que se administra.

    return (int) (end_memsegment - start_memsegment);
}