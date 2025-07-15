#ifndef CHIP8_H
#define CHIP8_H

// things

#define MEMORY_SIZE 4096
#define PROGRAM_START 0x200

#include <stdlib.h>
#include <stdio.h>

typedef uint64_t framebuffer_t[32];

typedef struct {
  uint8_t memory[MEMORY_SIZE];
  uint8_t V[16];
  uint16_t stack[16];
  uint16_t I;
  uint16_t PC;
  uint8_t SP;
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint16_t opcode;
  framebuffer_t fb;
} Chip8;

extern Chip8 *chip8;

int load_rom(char *rom_path);
int execute(void);

#endif
