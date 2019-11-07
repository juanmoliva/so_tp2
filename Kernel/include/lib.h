#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#define NULL 0

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char * cpu_vendor(char *result);

void *malloc( unsigned long bytes );
int strcmp (const char *t, const char *s);
char read_port(char port);
void write_port(char port, char reg);
void *set_stack(void *addr,void *rip);
void sem_up(void *addr);
void sem_down(void *addr);
void int_20();

#endif