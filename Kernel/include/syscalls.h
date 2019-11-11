#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include <stdint.h>
#include <lib.h>
#include <synchro.h>

void read_handler(uint64_t fd, char * buff, uint64_t count);
void write_handler(uint64_t fd, const char * buff, uint64_t count);
uint64_t time_handler();
void clear_handler();
uint64_t rtc_handler(uint8_t register);
void sleep_handler(uint64_t millis);
void beep_handler(uint16_t frequency, uint64_t time);
void pixel_handler(uint64_t x, uint64_t y, uint64_t rgb);
void *memory_handler(uint8_t flag,int num);
uint64_t memory_state_handler(uint8_t free);
uint64_t create_process_handler(void *rip, char *name, uint64_t param);
uint64_t update_process_priority_handler(int pid, int priority);
uint64_t update_process_state_handler(int pid, char state);
uint64_t list_processes_handler();
uint64_t list_pipes_handler();
int get_pid_handler();
int kill_process_handler(int pid);
sem_t *list_sem_handler();
int init_sem_handler(int id, int count);
int open_sem_handler(int id);
int close_sem_handler(int id);
int sem_wait_handler(int id);
int sem_post_handler(int id);
int create_pipe_handler(char *identifier);
int write_pipe_handler(char *identifier, char *str);
int read_pipe_handler(char *identifieri, char *buff);
int block_process_handler(int pid);
int nice_handler(int pid, int priority);
void loop_handler();
uint64_t handleSyscall(uint64_t sirq, uint64_t rdi, uint64_t rsi, uint64_t rdx);

#endif