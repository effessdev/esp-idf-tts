#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the DAC continuous audio player.
 *
 * @param volume_percent Playback volume as a percentage (0-100).
 *                        Values above 100 are clamped down to 100.
 *                        There is no implicit "full volume" fallback —
 *                        callers must pass an explicit, deliberately
 *                        chosen value so the speaker is never driven
 *                        harder than intended.
 */
void audio_player_init(uint8_t volume_percent);

/**
 * @brief Play the embedded PCM audio file synchronously.
 *        This function blocks and returns only after the entire
 *        audio clip has finished playing.
 */
void audio_player_play(void);

#ifdef __cplusplus
}
#endif
