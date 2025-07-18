#ifndef GRAPHICS_H
#define GRAPHICS_H

// more things

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320
#define SCALE 10

#include <SDL2/SDL.h>
#include "chip8.h"

int create_window(SDL_Window **window);
int destroy_window(SDL_Window *window);
void clear_framebuffer(void);
void set_pixel(int x, int y, int value);
uint8_t get_pixel(int x, int y);
void render_framebuffer(void);

#endif
