#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <types.h>
#include <pixelMap.h>
#include <strings.h>
#include <stdarg.h>
#include <videoDriver.h>

void init_console();

void backspace();

void printWithColors(Color chosenForeground, Color chosenBackground, char * str, va_list list);

void print(char * str, ...);

void print_N(const char * src, int length);

void printError_N(const char * str, int length);

void print_char(char c);

void printError_char(char c);

void printError(char * str, ...);

void clear_line(unsigned int line);

void move_all_up();

void clear_console();


#endif /* CONSOLE_H_ */
