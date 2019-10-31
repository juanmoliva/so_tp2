#ifndef PROCESS_H_
#define PROCESS_H_

void init_process_list();
void init_scheduler();
void *schedule(void *prev_rsp);
uint64_t create_process();



#endif