// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memory.h>

static uint64_t start_freemem = 0x1000000;

uint64_t allocate_bytes(uint64_t bytes) {
    // devolvemos el comienzo de la zona de memoria libre.
    unsigned long addr = start_freemem;
    
    // aumentamos el puntero al comienzo de zona libre.
    start_freemem += bytes;
    return addr;
}