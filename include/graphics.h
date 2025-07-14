#ifndef GRAPHICS_H
#define GRAPHICS_H

// more things

#define SCREEN_WIDTH 728
#define SCREEN_HEIGHT 364
#define SCALE 12

#include <SDL2/SDL.h>

typedef uint64_t framebuffer_t[32];

framebuffer_t fb;

int create_window(SDL_Window **window);
int destroy_window(SDL_Window *window);

void clear_framebuffer(framebuffer_t fb);
void set_pixel(int x, int y, int value);

#endif
