#ifndef STRINGS_H_
#define STRINGS_H_

#include <stdint.h>

int strlen(char * str);

uint32_t int_to_string(uint64_t value, char * buffer, uint32_t base);

#endif /* STRINGS_H_ */
