#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

void *allocate_blocks(int bytes);
int free_block(void *address);
int cur_free_mem();
int total_mem();
void init_mm();
int get_system();

#endif