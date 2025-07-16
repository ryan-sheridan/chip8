#ifndef CHIP8_H
#define CHIP8_H

// things

#define MEMORY_SIZE 4096
#define PROGRAM_START 0x200
#define STACK_SIZE 16
#define NUM_V_REGISTERS 16
#define NUM_KEYS 16
#define TIMER_MAX 255

#define CPU_HZ 700
#define TIMER_HZ 60
#define MS_PER_CYCLE (1000 / CPU_HZ)
#define MS_PER_TIMER_TICK (1000 / TIMER_HZ)

#define TRUE 1
#define FALSE 0

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

  uint8_t keyboard[NUM_KEYS];

  bool paused;
  bool draw_flag;
  bool was_key_pressed;
} Chip8;

extern Chip8 *chip8;

int load_rom(char *rom_path);
int execute(void);

#endif
