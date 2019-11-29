// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memory.h>
#include <stdint.h>
#define NULL 0
#define BUDDY 1

static void * start_memsegment = (void*)0x10000000;
static void * end_memsegment = (void*)0x14000000;

typedef struct node_alloc {
    unsigned long size;
    void *startAddr;
    struct node_alloc * next;
} node_alloc_t;

typedef struct node_free {
    unsigned long size;
    struct node_free * next;
} node_free_t;

node_alloc_t *allocated_nodes = NULL;
node_free_t *buddy_free_lists[26];

/*
    2^26 bytes de memoria.
    | En Buddy System --> Bloques de 2^x bytes, donde x <= 26
*/

/* Buddy System.
*/

//Aca inicializamos el Memory Manager
void init_mm() {
    node_free_t *first_mem_node = (node_free_t *)0x10000000;

    first_mem_node->next = NULL;
    first_mem_node->size = end_memsegment-start_memsegment;
    buddy_free_lists[25] = first_mem_node;
    
}

//Aca alocamos los bytes que nos pide el usuario
void *allocate_blocks(int bytes) {

    unsigned long size_needed = 1;
    int index = 0;
    while ( bytes + sizeof(node_free_t) > size_needed ) {
        index++;
        size_needed = size_needed*2;
    }

    if(index > 25) { // too much bytes
        return NULL;
    }

    // we look for the smallest block bigger than what we want to allocate
    int i = index;
    while ( buddy_free_lists[i] == NULL && i<=25) {
        i++;
    }

    if( buddy_free_lists[i] != NULL ) {
        // we found a free block.
        while( i > index ) {
            node_free_t *aux = buddy_free_lists[i];
            buddy_free_lists[i] = buddy_free_lists[i]->next;
            
            i--;
            int new_size = (aux->size)/2;
            
            node_free_t *new_free = aux + new_size;
            new_free->size = new_size;
            new_free->next = NULL;

            node_free_t *new_aux = aux;
            new_aux->size = new_size;
            new_aux->next = new_free;

            buddy_free_lists[i] = new_aux;
        }
        node_free_t *return_block = buddy_free_lists[i];
        buddy_free_lists[i] = buddy_free_lists[i]->next;

        node_alloc_t *aux = allocated_nodes;
        if (allocated_nodes == NULL ) {
            allocated_nodes->size=return_block->size;
            allocated_nodes->startAddr=return_block;
            allocated_nodes->next = NULL;
        }
        else {
            while ( aux->next != NULL ) {
                aux = aux->next;
            }
            node_alloc_t *new;
            new->startAddr = return_block;
            new->size = return_block->size;
            new->next = NULL;

            aux->next = new;
        }
        
        return (void *) return_block + sizeof(node_free_t);
    } else {
        // no block was found
        return NULL;
    }
}

int free_block(void *address) {
    // Si el bloque de memoria al que pertenece address fue asignado previamente, se "libera".
    // sino, no se hace nada.

    node_alloc_t *aux = allocated_nodes;
    if (allocated_nodes == NULL ) {
        return -1;
    }
    else {
        while( aux != NULL && aux->startAddr != (address-sizeof(node_free_t)) ) {
            aux = aux->next;
        }
        if (aux == NULL ) {
            return -1;
        }
    }

    // aux tiene el bloque a liberar.

    // indice??
    int i = 0;
    int value = 2;
    while ( aux->size != value && i<26 ) {
        value = value*2;
        i++;
    }

    if( buddy_free_lists[i] == NULL ) {
        // solo agrego el bloque.
        buddy_free_lists[i] = (node_free_t *) aux;
        buddy_free_lists[i]->size = aux->size;
        buddy_free_lists[i]->next= (node_free_t *) aux->next;
    } else {
        // reviso si tengo que fusionar segmentos.

        // calculo su Buddy Number
        unsigned long buddy_num = (aux->startAddr-start_memsegment)/aux->size;

        // calculo buddy Address
        void *buddy_addr = NULL;
        if ( buddy_num % 2 == 0 ) {
            buddy_addr = aux->startAddr + aux->size;
        } else {
            buddy_addr = aux->startAddr - aux->size;
        }

        node_free_t *free = buddy_free_lists[i];
        node_free_t *prev;
        while( free!=NULL && free != buddy_addr ) {
            prev = free;
            free = free->next;
        }

        if ( free == NULL ){ 
            // no hay nodo para fusionar.
            prev->next = aux->startAddr;
            prev->next->size = aux->size;
            prev->next->next = NULL;
        }
        else {
            // fusion.
            // free has buddy_addr.
            void *new_startaddr = NULL;
            
            if ( buddy_addr <= aux->startAddr ) { new_startaddr = buddy_addr; }
            else { new_startaddr = aux->startAddr; }

            prev->next = prev->next->next;

            node_free_t *new = buddy_free_lists[i + 1];
            if ( new == NULL ) {
                buddy_free_lists[i + 1] = new_startaddr;
                buddy_free_lists[i +1]->size = aux->size * 2;
                buddy_free_lists[i +1]->next = NULL;
            } else {
                while ( new->next != NULL ) {
                    new = new->next;
                }
                new->next = new_startaddr;
                new->next->size = aux->size * 2;
                new->next->next = NULL;
            }
        }

    }

    return 0;

}

int cur_free_mem() {
    // devuelve la cantidad de memoria libre que hay.
    int total = 0;
    // Voy a sumar bloques libres.
    int value = 2;
    for( int i = 0; i<=25 ; i++ ) {
        int count = 0;
        node_free_t *aux = buddy_free_lists[i];
        while ( aux != NULL ){
            aux = aux->next;
            count++;
        }

        total = total + count*value;

        value = value*2;
    }
    
    return total;
}

int total_mem(void *address) {
    // devuelve la cantidad de memoria total que se administra.

    return (int) (end_memsegment - start_memsegment);
}

int get_system(){
    return BUDDY;
}