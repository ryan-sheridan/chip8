#include "graphics.h"
#include "include/chip8.h"
#include "debug.h"
#include <stdio.h>

uint8_t memory[MEMORY_SIZE];
uint8_t V[16];
uint16_t stack[16];
uint16_t I;
uint16_t PC;
uint8_t SP;
uint8_t delay_timer;
uint8_t sound_timer;
uint16_t opcode;

void init_chip8(void) {
  I = 0;
  PC = 0x200;
  SP = 0;
  delay_timer = 0;
  sound_timer = 0;
  opcode = 0;
}

int main(int argc, char **argv) {
  init_chip8();

  if (argc < 2) {
    printf("Usage: %s <rom_path>\n", argv[0]);
    return 1;
  }

  char *rom_path = argv[1];
  if (load_rom(rom_path) != 0)
    return 1;

  // hex_dump((const void*)memory, MEMORY_SIZE);

  SDL_Window *main_window = NULL;

  create_window(&main_window);
  if (!main_window) {
    fprintf(stderr, "failed at create_window\n");
    return 1;
  }

  // keep window open unless quit is pressed
  SDL_Event e;
  int quit = 0;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = 1;
      }
    }
    execute();
    SDL_Delay(16); // ~60 FPS
  }

  destroy_window(main_window);
  return 0;
}
