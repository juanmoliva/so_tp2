#ifndef PROCESS_H_
#define PROCESS_H_

//Creo la struct
typedef struct process {
    unsigned char status; // READY/AVAILABLE ('a') , BLOCKED ('b') OR RUNNING ('r')
    unsigned char ppriority;
    void *bp;
    void *sp; // stack pointer
    int fd[2]; // fd[0] es para LECTURA. fd[1] es para escritura.
    struct process * next;
} process_t;

//struct para scheduler
typedef struct node {
    int pid;
    struct node *next;
} node_t;

void init_process_list();
int init_scheduler();
void *schedule(void *prev_rsp);
int create_process();



#endif