#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

void hex_dump(const void *data, size_t size);
void print_debug_info(void);
void start_debug_shell(void);

#endif
