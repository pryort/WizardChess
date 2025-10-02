#include "chess_ui.h"
#include "user_gameplay.h"
#include <stdio.h>
#include <stdlib.h>
#include "esp_random.h"

static void square_event_handler(lv_event_t * e) {
    lv_obj_t * target = lv_event_get_target(e);

    // gets display resolution
    lv_coord_t hor_res = lv_disp_get_hor_res(NULL);
    lv_coord_t ver_res = lv_disp_get_ver_res(NULL);

    // picks random location within bounds
    int btn_w = lv_obj_get_width(target);
    int btn_h = lv_obj_get_height(target);

    int x = esp_random() % (hor_res - btn_w);
    int y = esp_random() % (ver_res - btn_h);

    lv_obj_set_pos(target, x, y);

    printf("Button moved to (%d, %d)\n", x, y);
}

void create_chessboard(void) {
    // sets screen to white
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), 0);
    lv_scr_load(scr);

    // creates test button
    lv_obj_t * btn = lv_btn_create(scr);
    lv_obj_set_size(btn, 80, 80);
    lv_obj_set_pos(btn, 300, 150);

    // sets default color
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x007BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // changes color of clicked button 
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFD700), LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Test");
    lv_obj_center(label);

    lv_obj_add_event_cb(btn, square_event_handler, LV_EVENT_CLICKED, NULL);

    // creates back button
    lv_obj_t * back_btn = lv_btn_create(scr);
    lv_obj_set_size(back_btn, 120, 60);
    lv_obj_set_pos(back_btn, 10, 400);

    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xAA0000), LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_t * back_lbl = lv_label_create(back_btn);
    lv_label_set_text(back_lbl, "Back");
    lv_obj_center(back_lbl);

    lv_obj_add_event_cb(back_btn, button_event_handler, LV_EVENT_CLICKED, (void*)99);
}