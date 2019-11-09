#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

typedef struct node_mem {
    unsigned char free;
    unsigned long blocks;
    void *startAddr;
    struct node_mem * next;
} node_mem_t;

void *allocate_blocks(int bytes);
int free_block(void *address);
int cur_free_mem();
int total_mem();
void init_mm();

#endif