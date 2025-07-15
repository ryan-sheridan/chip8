#include "debug.h"
#include "chip8.h"

bool verbose;

void vlog(const char *format, ...) {
  if(!verbose) return;
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

void fvlog(FILE *stream, const char *format, ...) {
  if(!verbose) return;
  va_list args;
  va_start(args, format);
  vfprintf(stream, format, args);
  va_end(args);
}

void vlog_pc_opcode(void) {
  vlog("[v] pc: %04X, cur_opcode: %04X\n", chip8->pc_reg, chip8->cur_opcode);
}

void print_debug_info(void) {
  // TODO: print all other chip8 info, registers, memory layout, etc..
  vlog("----- printing debug info -----\n");
  vlog("pc: %04X\nsp: %02X\n", chip8->pc_reg, chip8->sp_reg);
  vlog("------- debug info end --------\n");
}

void start_debug_shell(void) {
  // TODO: you know what bro
  fvlog(stderr, "debug shell not yet implemented\n");
}

void hex_dump(const void *data, size_t size) {
  char ascii[17];
  size_t i, j;
  ascii[16] = '\0';
  for (i = 0; i < size; ++i) {
    if (i % 2 == 0) {
      vlog("%02X", ((unsigned char *)data)[i]);
    } else {
      vlog("%02X ", ((unsigned char *)data)[i]);
    }

    if (((unsigned char *)data)[i] >= ' ' &&
        ((unsigned char *)data)[i] <= '~') {
      ascii[i % 16] = ((unsigned char *)data)[i];
    } else {
      ascii[i % 16] = '.';
    }

    if ((i + 1) % 8 == 0 || i + 1 == size) {
      vlog(" ");
      if ((i + 1) % 16 == 0) {
        vlog("|  %s \n", ascii);
      } else if (i + 1 == size) {
        ascii[(i + 1) % 16] = '\0';
        if ((i + 1) % 16 <= 8) {
          vlog(" ");
        }
        for (j = (i + 1) % 16; j < 16; ++j) {
          vlog("   ");
        }
        vlog("|  %s \n", ascii);
      }
    }
  }
}
