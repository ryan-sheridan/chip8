#ifndef CHIP8_H
#define CHIP8_H

// things

#define MEMORY_SIZE 4096
#define PROGRAM_START 0x200

#include <stdlib.h>
#include <stdio.h>

extern uint8_t memory[MEMORY_SIZE];
extern uint8_t V[16];
extern uint16_t stack[16];
extern uint16_t I;
extern uint16_t PC;
extern uint8_t SP;
extern uint8_t delay_timer;
extern uint8_t sound_timer;

int load_rom(char *rom_path);
int execute(void);

#endif
