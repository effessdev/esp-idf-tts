#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the DAC continuous audio player.
 */
void audio_player_init(void);

/**
 * @brief Play the embedded PCM audio file synchronously.
 *
 * @param volume_percent Playback volume as a percentage (0-100), clamped to
 * 100.
 */
void audio_player_play(uint8_t volume_percent);

#ifdef __cplusplus
}
#endif
