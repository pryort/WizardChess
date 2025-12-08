#include "final_screen.h"
#include "color_pick.h"
#include "chess_ui.h"
#include "user_gameplay.h"
#include "Spec_size_font_60.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void back_to_menu(lv_event_t *e) {
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    printf("Back button to starting menu\n");
    create_starting_menu();
}

void win_screen(void) {
    // creates a new screen
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x008000), 0);

    // title
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "You Win!");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_update_layout(label);
    lv_coord_t title_w = lv_obj_get_width(label);
    int title_x = (780 - title_w) / 2;
    lv_obj_set_pos(label, title_x, 300);

    // create back button
    lv_obj_t *back_btn = lv_btn_create(scr);
    lv_obj_set_size(back_btn, 120, 60);
    lv_obj_set_pos(back_btn, 10, 10);

    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xAA0000), LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_t *back_lbl = lv_label_create(back_btn);
    lv_label_set_text(back_lbl, "Back");
    lv_obj_center(back_lbl);
    lv_obj_add_event_cb(back_btn, back_to_menu, LV_EVENT_CLICKED, NULL);

    lv_scr_load(scr);
}

void lose_screen(void) {
    // creates a new screen
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xFF0000), 0);

    // title
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "You Lose :(");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_update_layout(label);
    lv_coord_t title_w = lv_obj_get_width(label);
    int title_x = (780 - title_w) / 2;
    lv_obj_set_pos(label, title_x, 300);

    // create back button
    lv_obj_t *back_btn = lv_btn_create(scr);
    lv_obj_set_size(back_btn, 120, 60);
    lv_obj_set_pos(back_btn, 10, 10);

    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xAA0000), LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_t *back_lbl = lv_label_create(back_btn);
    lv_label_set_text(back_lbl, "Back");
    lv_obj_center(back_lbl);
    lv_obj_add_event_cb(back_btn, back_to_menu, LV_EVENT_CLICKED, NULL);

    lv_scr_load(scr);
}

void tie_screen(void) {
    // creates a new screen
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), 0);

    // title
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "Stalemate");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_update_layout(label);
    lv_coord_t title_w = lv_obj_get_width(label);
    int title_x = (780 - title_w) / 2;
    lv_obj_set_pos(label, title_x, 300);

    // create back button
    lv_obj_t *back_btn = lv_btn_create(scr);
    lv_obj_set_size(back_btn, 120, 60);
    lv_obj_set_pos(back_btn, 10, 10);

    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xAA0000), LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_t *back_lbl = lv_label_create(back_btn);
    lv_label_set_text(back_lbl, "Back");
    lv_obj_center(back_lbl);
    lv_obj_add_event_cb(back_btn, back_to_menu, LV_EVENT_CLICKED, NULL);

    lv_scr_load(scr);
}