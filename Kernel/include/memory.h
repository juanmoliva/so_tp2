#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

typedef struct node {
    unsigned char free;
    unsigned long blocks;
    void *startAddr;
    struct node * next;
} node_t;

void *allocate_blocks(int bytes);
int free_block(void *address);
int cur_free_mem();
int total_mem();
void init_mm();

#endif