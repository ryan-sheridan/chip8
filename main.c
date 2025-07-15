#include "graphics.h"
#include "include/chip8.h"
#include "debug.h"
#include <stdio.h>

Chip8 *chip8;
bool verbose;

void init_chip8(void) {
  chip8 = malloc(sizeof(Chip8));

  chip8->i_reg = 0;
  chip8->pc_reg = 0x200;
  chip8->sp_reg = 0;
  chip8->delay_timer = 0;
  chip8->sound_timer = 0;
  chip8->cur_opcode = 0;
  chip8->paused = false;
}

int main(int argc, char **argv) {
  init_chip8();

  if (argc < 2) {
    printf("Usage: %s <rom_path> [-v verbose]\n", argv[0]);
    return 1;
  }

  if(argc > 2) {
    if(strcmp(argv[2], "-v") == 0) {
      verbose = true;
      vlog("verbose mode activated, logging everything!\n");
    }
  }

  char *rom_path = argv[1];
  if (load_rom(rom_path) != 0)
    return 1;

  SDL_Window *main_window = NULL;

  create_window(&main_window);
  if (!main_window) {
    fprintf(stderr, "failed create_window\n");
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
    if(chip8->paused) {
      start_debug_shell();
      return 1;
    }

    SDL_Delay(16); // ~60 FPS
  }

  destroy_window(main_window);
  return 0;
}
