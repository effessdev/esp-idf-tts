idf_component_register(SRCS "espeak_voice.c" "audio_player.c"
                    INCLUDE_DIRS "." "include"
                    EMBED_FILES "audio.pcm"
                    REQUIRES esp_driver_gpio esp_driver_dac)
