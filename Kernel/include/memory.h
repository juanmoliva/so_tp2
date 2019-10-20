#ifndef _MEMORY_H_
#define _MEMORY_H_

uint64_t allocate_bytes(uint64_t bytes);
uint64_t free_block(uint64_t address);
uint64_t cur_free_mem();
uint64_t total_mem(uint64_t address);

#endif