#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

extern bool verbose;

#include <sys/time.h>

#define TIME_FUNCTION(func_call) do { \
  long long start_time = get_time_us(); \
  func_call; \
  long long end_time = get_time_us(); \
  vlog("[TIMING] %s took %lld μs\n", #func_call, end_time - start_time); \
} while(0)

long long get_time_us(void);
void hex_dump(const void *data, size_t size);
void print_debug_info(void);
void start_debug_shell(void);
void vlog(const char *format, ...);
void fvlog(FILE *stream, const char *format, ...);
void vlog_pc_opcode(void);

#endif
