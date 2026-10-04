idf_component_register(
    SRCS "main.c"
         "meltdown_app.c"
         "meltdown_model.c"
         "meltdown_store.c"
         "meltdown_audio.c"
         "meltdown_network.c"
         "meltdown_blufi_security.c"
         "meltdown_ui.c"
         "../assets/dots/meltdown_dots.c"
         "../assets/music/meltdown_c3_pcm.c"
         "../assets/music/meltdown_egg_pcm.c"
    INCLUDE_DIRS "." "../assets/dots"
    REQUIRES bsp bt esp_event esp_hw_support esp_netif esp_timer esp_wifi lwip mbedtls nvs_flash
)
