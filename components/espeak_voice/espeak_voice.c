#include "audio_player.h"
#include <driver/gpio.h>

void esv_init(gpio_num_t dac_pin) { audio_player_init(); }

void esv_speak(const char *text, int volume) { audio_player_play(volume); }
