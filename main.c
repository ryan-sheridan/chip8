#include "chip8.h"
#include "debug.h"
#include "graphics.h"
#include "input.h"
#include "sound.h"
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
  chip8->draw_flag = false;

  for (int i = 0; i < NUM_KEYS; i++) {
    chip8->keyboard[i] = false;
  }
  chip8->was_key_pressed = false;
}

int main(int argc, char **argv) {
  init_chip8();
  sound_init();

  if (argc < 2) {
    printf("Usage: %s <rom_path> [-v verbose]\n", argv[0]);
    return 1;
  }

  if (argc > 2 && strcmp(argv[2], "-v") == 0) {
    verbose = true;
    vlog("verbose mode activated, logging everything!\n");
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

  SDL_Event e;
  int quit = 0;
  uint32_t last_timer_tick = SDL_GetTicks();
  uint32_t last_cycle = SDL_GetTicks();

  while (!quit) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        quit = 1;
        break;
      case SDL_KEYDOWN:
        handle_key_press(e.key.keysym.sym);
        break;
      case SDL_KEYUP:
        handle_key_up(e.key.keysym.sym);
        break;
      }
    }

    uint32_t now = SDL_GetTicks();

    if (now - last_cycle >= MS_PER_CYCLE) {
      execute();
      last_cycle = now;
    }

    if (now - last_timer_tick >= MS_PER_TIMER_TICK) {
      if (chip8->delay_timer > 0)
        chip8->delay_timer--;

      if (chip8->sound_timer > 0) {
        chip8->sound_timer--;
        if(chip8->sound_timer == 0) {
          SDL_CreateThread(beep_thread, "BeepThread", NULL);
        }
      }


      last_timer_tick = now;
    }

    if (chip8->paused) {
      start_debug_shell();
      return 1;
    }

    SDL_Delay(1);
  }

  destroy_window(main_window);
  return 0;
}
