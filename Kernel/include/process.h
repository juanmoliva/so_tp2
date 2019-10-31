#ifndef PROCESS_H_
#define PROCESS_H_

void init_process_list();
int init_scheduler();
void *schedule(void *prev_rsp);
int create_process();



#endif