#include "sound.h"

// gpt to bits, i dont get audio synthesis

static SDL_AudioDeviceID device_id = 0;
static double phase = 0.0;

void beep_callback(void *userdata, uint8_t *stream, int len) {
  Sint16 *samples = (Sint16 *)stream;
  int sample_count = len / sizeof(Sint16);
  for (int i = 0; i < sample_count; i++) {
    samples[i] = (Sint16)(3000 * sin(2.0 * M_PI * 440.0 * phase));
    phase += 1.0 / 44100.0;
  }
}

int beep_thread(void *unused) {
  SDL_PauseAudioDevice(device_id, 0); // Start playback
  SDL_Delay(200);                     // Play for 200ms
  SDL_PauseAudioDevice(device_id, 1); // Stop playback
  return 0;
}

void sound_init(void) {
  SDL_Init(SDL_INIT_AUDIO);
  SDL_AudioSpec spec = {0};
  spec.freq = 44100;
  spec.format = AUDIO_S16SYS;
  spec.channels = 1;
  spec.samples = 512;
  spec.callback = beep_callback;

  device_id = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
  if (!device_id) {
    fprintf(stderr, "Failed to open audio: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_PauseAudioDevice(device_id, 1); // Start paused
}

void sound_cleanup(void) {
  SDL_CloseAudioDevice(device_id);
  SDL_Quit();
}
