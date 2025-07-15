#ifndef GRAPHICS_H
#define GRAPHICS_H

// more things

#define SCREEN_WIDTH 728
#define SCREEN_HEIGHT 364
#define SCALE 12

#include <SDL2/SDL.h>
#include "chip8.h"


int create_window(SDL_Window **window);
int destroy_window(SDL_Window *window);

void clear_framebuffer(void);
void set_pixel(int x, int y, int value);

uint8_t get_pixel(int x, int y);

#endif
