#include "graphics.h"
#include "include/chip8.h"
#include <stdio.h>

int main(void) {
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
    SDL_Delay(16); // ~60 FPS
  }

  destroy_window(main_window);
  return 0;
}
