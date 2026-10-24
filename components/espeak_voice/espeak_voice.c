#include "audio_player.h"
#include "soc/gpio_num.h"
#include <driver/gpio.h>

// Linker symbols generated for "audio.pcm" (dots become underscores)
extern const uint8_t _binary_audio_pcm_start[];
extern const uint8_t _binary_audio_pcm_end[];

void esv_init(gpio_num_t dac_pin) {
  // GPIO_NUM_25 isn't functional yet
  audio_player_init(GPIO_NUM_25);
}

void esv_speak(const char *text, int volume) {
  audio_player_play(_binary_audio_pcm_start, _binary_audio_pcm_end, volume);
}
