#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

extern bool verbose;

void hex_dump(const void *data, size_t size);
void print_debug_info(void);
void start_debug_shell(void);
void vlog(const char *format, ...);
void fvlog(FILE *stream, const char *format, ...);

#endif
