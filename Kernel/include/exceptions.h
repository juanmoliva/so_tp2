#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <stdint.h>

void exceptionDispatcher(int exc);
void getRegisters();

// Estructura que queda en el stack luego de saltar a una excepcion
// Solo buscamos el rip (instruction pointer)
typedef struct {
    uint64_t stack_segment;
    uint64_t stack_ptr;
    uint64_t rflags;
    uint64_t cs;
    uint64_t rip;
    uint64_t errCode;
} ExceptionStackFrame;

// Estructura para completar con los valores de todos los registros
typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t r8;
    uint64_t r9;
} RegisterSet;

#endif /* EXCEPTIONS_H_ */