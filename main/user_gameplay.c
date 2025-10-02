#include "user_gameplay.h"
#include "chess_ui.h"
#include <stdio.h>

void button_event_handler(lv_event_t * e) {
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    int id = (int) lv_event_get_user_data(e);

    if (id == 1) {
        printf("Going to chess visual screen\n");
        create_chessboard();
    }
    else if (id == 2) {
        printf("Going to chess visual screen\n");
        create_chessboard();
    }
    else if (id == 99) {
        printf("Back to menu\n");
        create_starting_menu();
    }
}

void create_starting_menu(void) {
    // creates a new screen
    lv_obj_t * scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), 0);

    // title
    lv_obj_t * label = lv_label_create(scr);
    lv_label_set_text(label, "PICK GAME MODE");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_update_layout(label);
    lv_coord_t title_w = lv_obj_get_width(label);
    int title_x = (780 - title_w) / 2;
    lv_obj_set_pos(label, title_x, 10);

    // button 1
    lv_obj_t * btn1 = lv_btn_create(scr);
    lv_obj_set_size(btn1, 385, 350);
    lv_obj_set_pos(btn1, 10, 120);
    lv_obj_add_event_cb(btn1, button_event_handler, LV_EVENT_CLICKED, (void*)1);

    // button color
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0x007BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // button pressed color
    lv_obj_set_style_bg_color(btn1, lv_color_hex(0xFFD700), LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_add_event_cb(btn1, button_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t * lbl1 = lv_label_create(btn1);
    lv_label_set_text(lbl1, "PHYSICAL PIECE MOVEMENT");
    lv_obj_center(lbl1);

    // button 2
    lv_obj_t * btn2 = lv_btn_create(scr);
    lv_obj_set_size(btn2, 385, 350);
    lv_obj_set_pos(btn2, 405, 120);
    lv_obj_add_event_cb(btn2, button_event_handler, LV_EVENT_CLICKED, (void*)2);
    
    // button color
    lv_obj_set_style_bg_color(btn2, lv_color_hex(0x007BFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    // button pressed color
    lv_obj_set_style_bg_color(btn2, lv_color_hex(0xFFD700), LV_PART_MAIN | LV_STATE_PRESSED);
    
    lv_obj_add_event_cb(btn2, button_event_handler, LV_EVENT_CLICKED, NULL);

    lv_obj_t * lbl2 = lv_label_create(btn2);
    lv_label_set_text(lbl2, "USER INTERFACE");
    lv_obj_center(lbl2);

    lv_scr_load(scr);
}