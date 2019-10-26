#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

uint64_t allocate_blocks(uint64_t bytes);
uint64_t free_block(uint64_t address);
uint64_t cur_free_mem();
uint64_t total_mem();
void init_mm();

#endif