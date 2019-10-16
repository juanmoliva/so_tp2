#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include <stdint.h>
#include <lib.h>

void read_handler(uint64_t fd, char * buff, uint64_t count);
void write_handler(uint64_t fd, const char * buff, uint64_t count);
uint64_t time_handler();
void clear_handler();
uint64_t rtc_handler(uint8_t register);
void sleep_handler(uint64_t millis);
void beep_handler(uint16_t frequency, uint64_t time);
void pixel_handler(uint64_t x, uint64_t y, uint64_t rgb);

uint64_t handleSyscall(uint64_t sirq, uint64_t rdi, uint64_t rsi, uint64_t rdx);

#endif