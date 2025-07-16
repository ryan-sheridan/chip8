#include "graphics.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <unistd.h>
#include "debug.h"

static SDL_Renderer *renderer;

int create_window(SDL_Window **window) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    printf("could not init sdl %s\n", SDL_GetError());
    return 1;
  } else {
    printf("sdl video init success\n");
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

void render_framebuffer(void) {
  // set screen as black
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  // set render draw colour as white
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  // loop through framebuffer and draw a rect * scale
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      int bit_pos = 63 - x;
      int pixel = (chip8->fb[y] >> bit_pos) & 1;
      // if the pixel at x and y is 1
      if (pixel) {
        // create an SDL_Rect * the scale, each pixel is scaled up by 12
        // x, y, w, h
        SDL_Rect rect = {x * SCALE, y * SCALE, SCALE, SCALE};
        // render the rect
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

void set_pixel(int x, int y, int value) {
  int bit_pos = 63 - x;
  if (value) {
    chip8->fb[y] |= (1ULL << bit_pos);
  } else {
    chip8->fb[y] &= ~(1ULL << bit_pos);
  }
}

uint8_t get_pixel(int x, int y) {
  int bit_pos = 63 - x;
  return (chip8->fb[y] >> bit_pos) & 1;
}

void clear_framebuffer(void) {
  for (int i = 0; i < 32; i++) {
    chip8->fb[i] = 0;
  }
  render_framebuffer();
}
