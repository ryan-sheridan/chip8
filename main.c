#include "graphics.h"
#include "include/chip8.h"
#include <stdio.h>

int main(int argc, char **argv) {
  if(argc < 2) {
    printf("Usage: %s <rom_path>\n", argv[0]);
    return 1;
  }

  char *rom_path = argv[1];
  FILE *fp = fopen(rom_path, "r");
  if(!fp) {
    fprintf(stderr, "failed to open rom %s\n", rom_path);
    return 1;
  }

  SDL_Window *main_window = NULL;

  create_window(&main_window);
  if (!main_window) {
    fprintf(stderr, "failed at create_window\n");
    return 1;
  }

  printf("window: %p", &main_window);

  // keep window open unless quit is pressed
  SDL_Event e;
  int quit = 0;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = 1;
      }
    }
    set_pixel(0, 0, 1);
    SDL_Delay(16); // ~60 FPS
  }

  destroy_window(main_window);
  fclose(fp);
  return 0;
}
