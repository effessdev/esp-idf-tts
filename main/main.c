#include "audio_player.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void app_main(void) {
  // Initialize the player once at startup.
  // Volume is a percentage (0-100); start conservatively low and raise
  // it deliberately rather than defaulting to full volume.
  audio_player_init(40);

  while (1) {
    printf("Playing audio now...\n");

    // This call blocks until the audio finishes playing completely
    audio_player_play();

    printf("Audio finished. Waiting 1 second before playing again...\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
