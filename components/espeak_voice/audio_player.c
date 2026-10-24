#include "audio_player.h"
#include "driver/dac_continuous.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "AUDIO_PLAYER";

static dac_continuous_handle_t s_dac_handle = NULL;
static bool s_is_initialized = false;

#define AUDIO_SAMPLE_RATE_HZ 20000
#define AUDIO_VOLUME_MAX_PERCENT 100
#define AUDIO_SCALE_CHUNK_BYTES 1024

void audio_player_init(gpio_num_t dac_pin) {
  if (s_is_initialized) {
    return;
  }

  ESP_LOGI(TAG, "Initializing DAC Continuous Driver...");

  ESP_LOGW(TAG, "Setting DAC pin isn't currently supported, skipping and "
                "defaulting to GPIO25");

  dac_continuous_config_t dac_cfg = {
      .chan_mask = DAC_CHANNEL_MASK_CH0, // GPIO25 (DAC Channel 1)
      .desc_num = 4,
      .buf_size = 2048,
      .freq_hz = AUDIO_SAMPLE_RATE_HZ,
      .offset = 0,
      .clk_src = DAC_DIGI_CLK_SRC_DEFAULT,
      .chan_mode = DAC_CHANNEL_MODE_SIMUL,
  };

  ESP_ERROR_CHECK(dac_continuous_new_channels(&dac_cfg, &s_dac_handle));
  ESP_ERROR_CHECK(dac_continuous_enable(s_dac_handle));

  s_is_initialized = true;
}

// Scales 8-bit unsigned PCM samples (centered on 128) by volume_percent.
static void scale_samples_for_volume(const uint8_t *src, uint8_t *dst,
                                     size_t len, uint8_t volume_percent) {
  for (size_t i = 0; i < len; i++) {
    int centered = (int)src[i] - 128;
    dst[i] = (uint8_t)(128 + (centered * volume_percent) / 100);
  }
}

void audio_player_play(const uint8_t *audio_data_start,
                       const uint8_t *audio_data_end, uint8_t volume_percent) {
  if (!s_is_initialized) {
    // No implicit fallback to a default volume: without an explicit
    // audio_player_init(volume_percent) call we don't know a safe level,
    // so refuse to play rather than risk driving the speaker too hard.
    ESP_LOGE(TAG, "audio_player_play() called before audio_player_init(); "
                  "refusing to play");
    return;
  }

  if (volume_percent > AUDIO_VOLUME_MAX_PERCENT) {
    ESP_LOGW(TAG, "Requested volume %u%% exceeds max, clamping to %u%%",
             volume_percent, AUDIO_VOLUME_MAX_PERCENT);
    volume_percent = AUDIO_VOLUME_MAX_PERCENT;
  }

  // Calculate length using the symbols generated from audio.pcm
  const size_t audio_len = audio_data_end - audio_data_start;

  ESP_LOGI(TAG, "Starting audio playback (%d bytes) at %u%% volume...",
           audio_len, volume_percent);

  // Scale and write in fixed-size chunks via a static buffer, rather than
  // heap-allocating a copy of the whole clip (which can fail/fragment the
  // heap for larger clips).
  static uint8_t s_chunk_buf[AUDIO_SCALE_CHUNK_BYTES];
  size_t offset = 0;
  while (offset < audio_len) {
    size_t chunk_len = audio_len - offset;
    if (chunk_len > AUDIO_SCALE_CHUNK_BYTES) {
      chunk_len = AUDIO_SCALE_CHUNK_BYTES;
    }

    scale_samples_for_volume(audio_data_start + offset, s_chunk_buf, chunk_len,
                             volume_percent);

    size_t bytes_written = 0;
    ESP_ERROR_CHECK(dac_continuous_write(s_dac_handle, s_chunk_buf, chunk_len,
                                         &bytes_written, portMAX_DELAY));
    offset += chunk_len;
  }

  // Wait until physical playback completes based on the audio duration
  TickType_t playback_duration_ticks =
      pdMS_TO_TICKS((audio_len * 1000) / AUDIO_SAMPLE_RATE_HZ);
  vTaskDelay(playback_duration_ticks > 0 ? playback_duration_ticks : 1);

  ESP_LOGI(TAG, "Audio playback finished.");
}
