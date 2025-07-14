#include "graphics.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

static SDL_Renderer *renderer;

int create_window(SDL_Window **window) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    printf("could not init sdl %s\n", SDL_GetError());
    return 1;
  } else {
    printf("sdl audio and video init success\n");
  }

  *window =
      SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

  if (!*window) {
    printf("could not open window %s\n", SDL_GetError());
    return 1;
  }

  renderer = SDL_CreateRenderer(*window, -1, 0);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  return 0;
}

int destroy_window(SDL_Window *window) {
  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
  }
  if (window) {
    SDL_DestroyWindow(window);
  }
  SDL_Quit();
  return 0;
}
