#ifndef PIPES_H_
#define PIPES_H_

#include <stdint.h>

int create_pipe(const char *identifier);
int write_pipe(const char *identifier, const char *str);
int read_pipe(const char *identifier,char *destination);

#endif