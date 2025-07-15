#ifndef CHIP8_H
#define CHIP8_H

// things

#define MEMORY_SIZE 4096
#define PROGRAM_START 0x200
#define STACK_SIZE 16
#define NUM_V_REGISTERS 16

#include <stdlib.h>
#include <stdio.h>
#include "stdbool.h"

typedef uint64_t framebuffer_t[32];

typedef struct {
  uint8_t memory[MEMORY_SIZE];
  uint16_t stack[STACK_SIZE];
  uint8_t V[NUM_V_REGISTERS];

  uint16_t i_reg;
  uint16_t pc_reg;
  uint8_t sp_reg;

  uint8_t delay_timer;
  uint8_t sound_timer;

  uint16_t cur_opcode;
  framebuffer_t fb;

  bool paused;
} Chip8;

extern Chip8 *chip8;

int load_rom(char *rom_path);
int execute(void);

#endif
