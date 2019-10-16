#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

#define TAB_SIZE        6
#define STDOUT_FORMAT   2
#define STDERR_FORMAT   4

#define STDIN   0
#define STDOUT  1
#define STDERR  2

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncTab();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();
void ncPrintOnAddress(char * address, char * text);

void ncPrintError(const char * string);

#endif