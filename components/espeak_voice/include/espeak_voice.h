#include <driver/gpio.h>

void esv_init(gpio_num_t dac_pin);

// Speak a string
void esv_speak(const char *text, int volume);
