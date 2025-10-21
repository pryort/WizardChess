idf_component_register(
    SRCS "Spec_size_font_60.c" "font_chess_35_lvgl_8.c" "color_pick.c" "user_gameplay.c" "chess_ui.c" "waveshare_rgb_lcd_port.c" "main.c" "lvgl_port.c"
    INCLUDE_DIRS ".")

idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_options(${lvgl_lib} PRIVATE -Wno-format)


