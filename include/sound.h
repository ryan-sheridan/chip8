#ifndef SOUND_H
#define SOUND_H

// sound pal
#define FREQ 44100
#define CHANNELS 1
#define SAMPLES 4096
#define BEEP_HZ 440.0
#define BEEP_DURATION_MS 200

#include <stdlib.h>
#include "debug.h"
#include <SDL2/SDL.h>

void beep_callback(void *userdata, uint8_t *stream, int len);
void beep(void);
int beep_thread(void *unused);
void sound_init(void);

#endif
