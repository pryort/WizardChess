#include "color_pick.h"
#include "chess_ui.h"
#include "user_gameplay.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void back_to_menu_event(lv_event_t * e) {
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    // char *mode = lv_event_get_user_data(e);
    printf("Back button to starting menu\n");
    create_starting_menu();
    
}

void color_event_handler(lv_event_t * e) {
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    char *mode = lv_event_get_user_data(e);
    srand(time(NULL));
    int random = rand();
    printf("%c\n", *mode);
    create_chessboard(*mode);
}

void color_choice_menu(char mode) {
    // creates a new screen
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), 0);
    printf("%c\n", mode);
    char *data = lv_mem_alloc(sizeof(char));
    *data = mode;

    // title
    lv_obj_t * label = lv_label_create(scr);
    lv_label_set_text(label, "PICK COLOR");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_update_layout(label);
    lv_coord_t title_w = lv_obj_get_width(label);
    int title_x = (780 - title_w) / 2;
    lv_obj_set_pos(label, title_x, 10);

    // button 1
    lv_obj_t * btn1 = lv_btn_create(scr);
    lv_obj_set_size(btn1, 250, 350);
    lv_obj_set_pos(btn1, 15, 120);
    lv_obj_add_event_cb(btn1, color_event_handler, LV_EVENT_CLICKED, data);

    // button color
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0x007BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // button pressed color
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0xFFD700), LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_t * lbl1 = lv_label_create(btn1);
    lv_label_set_text(lbl1, "BLACK");
    lv_obj_center(lbl1);

    // button 2
    lv_obj_t * btn2 = lv_btn_create(scr);
    lv_obj_set_size(btn2, 250, 350);
    lv_obj_set_pos(btn2, 275, 120);
    lv_obj_add_event_cb(btn2, color_event_handler, LV_EVENT_CLICKED, data);

    // button color
    lv_obj_set_style_bg_color(btn2, lv_color_hex(0x007BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // button pressed color
    lv_obj_set_style_bg_color(btn2, lv_color_hex(0xFFD700), LV_PART_MAIN | LV_STATE_PRESSED);
    
    lv_obj_t * lbl2 = lv_label_create(btn2);
    lv_label_set_text(lbl2, "WHITE");
    lv_obj_center(lbl2);

    // button 3
    lv_obj_t * btn3 = lv_btn_create(scr);
    lv_obj_set_size(btn3, 250, 350);
    lv_obj_set_pos(btn3, 535, 120);
    lv_obj_add_event_cb(btn3, color_event_handler, LV_EVENT_CLICKED, data);
    
    // button color
    lv_obj_set_style_bg_color(btn3, lv_color_hex(0x007BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // button pressed color
    lv_obj_set_style_bg_color(btn3, lv_color_hex(0xFFD700), LV_PART_MAIN | LV_STATE_PRESSED);
    
    lv_obj_t * lbl3 = lv_label_create(btn3);
    lv_label_set_text(lbl3, "RANDOM");
    lv_obj_center(lbl3);

    // create back button
    lv_obj_t * back_btn = lv_btn_create(scr);
    lv_obj_set_size(back_btn, 120, 60);
    lv_obj_set_pos(back_btn, 10, 10);

    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xAA0000), LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_t * back_lbl = lv_label_create(back_btn);
    lv_label_set_text(back_lbl, "Back");
    lv_obj_center(back_lbl);

    lv_obj_add_event_cb(back_btn, back_to_menu_event, LV_EVENT_CLICKED, data);

    lv_scr_load(scr);
}