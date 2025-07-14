#ifndef GRAPHICS_H
#define GRAPHICS_H

// more things

#define SCREEN_WIDTH 728
#define SCREEN_HEIGHT 364

#include <SDL2/SDL.h>

int create_window(SDL_Window **window);
int destroy_window(SDL_Window *window);

// void render_graphics(void *buffer);

#endif
