#ifndef PIPES_H_
#define PIPES_H_

#include <stdint.h>

typedef struct pipe {
    const char *identifier;
    // sem_t * read_sem;
    // sem_t * write_sem;
    sem_t * global_sem;
    //Region donde se va a escribir
    void * critical_region;

    struct pipe * next;
} pipe_t;

int create_pipe(const char *identifier);
int write_pipe(const char *identifier, const char *str);
int read_pipe(const char *identifier,char *destination);
pipe_t * list_pipes();

#endif