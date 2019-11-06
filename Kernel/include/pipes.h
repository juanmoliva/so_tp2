#ifndef PIPES_H_
#define PIPES_H_

#include <stdint.h>

int create_pipe(int identifier);
int write_pipe(int identifier, const void *str);
int read_pipe(int identifier, void *destination);

#endif