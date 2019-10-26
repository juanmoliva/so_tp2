#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>
#include <interrupts.h>

#define TICK_LENGTH     55  // Cantidad de ms que equivalen a un tick

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void sleep(uint64_t millis);

#endif
