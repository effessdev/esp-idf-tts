#include "esp_log.h"
#include "espeak_voice.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

const char *TAG = "MAIN";

void app_main(void) {
  esv_init(GPIO_NUM_25);

  while (1) {
    ESP_LOGI(TAG, "Speaking Hello world...\n");

    esv_speak("Hello world", 25);

    ESP_LOGI(TAG, "Finished playing. Waiting for 1 second...\n");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
