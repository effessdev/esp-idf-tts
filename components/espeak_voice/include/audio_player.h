#pragma once

#include <driver/gpio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the DAC continuous audio player.
 */
void audio_player_init(gpio_num_t dac_pin);

/**
 * @brief Play the embedded PCM audio file synchronously.
 *
 * @param volume_percent Playback volume as a percentage (0-100), clamped to
 * 100.
 */
void audio_player_play(const uint8_t *audio_data_start,
                       const uint8_t *audio_data_end, uint8_t volume_percent);

#ifdef __cplusplus
}
#endif
