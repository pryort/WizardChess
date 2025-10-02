#include "chess_ui.h"
#include "user_gameplay.h"
#include <stdio.h>
#include <stdlib.h>
#include "esp_random.h"

#define X_START 150
#define BUTTON_W 60
#define BOARD_W 8

typedef struct {
    lv_color_t default_color;
} btn_userdata_t;

static lv_obj_t *last_btn = NULL;
static lv_obj_t *board_square[BOARD_W][BOARD_W];
static lv_obj_t *board_piece[BOARD_W][BOARD_W];

static void square_event_handler(lv_event_t * e) {
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    lv_obj_t *btn = lv_event_get_target(e);
    // btn_userdata_t *ud = lv_event_get_user_data(e);

    if (last_btn != NULL) {
        btn_userdata_t *last_ud = lv_obj_get_user_data(last_btn);
        lv_obj_set_style_bg_color(last_btn, last_ud->default_color, LV_PART_MAIN);
    }

    lv_obj_set_style_bg_color(btn, lv_color_hex(0x008000), LV_PART_MAIN);

    last_btn = btn;

    printf("Square clicked\n");
}

void create_chessboard(void) {
    // sets screen to white
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), 0);
    lv_scr_load(scr);

    // creates chess board
    for (int i = 0; i < BOARD_W; i++) {
        for (int j = 0; j < BOARD_W; j++) {
            // creates chess board
            lv_obj_t *btn = lv_btn_create(scr);
            lv_obj_set_size(btn, BUTTON_W, BUTTON_W);
            lv_obj_set_style_radius(btn, 0, LV_PART_MAIN);
            lv_obj_set_pos(btn, X_START + i * BUTTON_W, 0 + j * BUTTON_W);

            // colors chess board
            lv_color_t def_col = ((i + j) % 2 == 1) ? lv_color_hex(0x000000): lv_color_hex(0xFFFFFF);
            lv_obj_set_style_bg_color(btn, def_col, LV_PART_MAIN);

            lv_obj_t *label = lv_label_create(btn);
            lv_label_set_text(label, "");
            lv_obj_center(label);

            board_piece[i][j] = label;
            board_square[i][j] = btn;
            
            // allocates memory for buttons
            btn_userdata_t *ud = lv_mem_alloc(sizeof(btn_userdata_t));
            ud->default_color = def_col;
            lv_obj_set_user_data(btn, ud);
            lv_obj_add_event_cb(btn, square_event_handler, LV_EVENT_ALL, ud);
        }
    }

    // finalizes board
    static lv_point_t line_points[] = { {0, 0}, {0, 480} };
    lv_obj_t *line1 = lv_line_create(lv_scr_act());
    lv_line_set_points(line1, line_points, 2);
    lv_obj_set_style_line_color(line1, lv_color_hex(0x000000), 0);
    lv_obj_set_style_width(line1, 1, 0);
    lv_obj_set_pos(line1, 149, 0);

    lv_obj_t *line2 = lv_line_create(lv_scr_act());
    lv_line_set_points(line2, line_points, 2);
    lv_obj_set_style_line_color(line2, lv_color_hex(0x000000), 0);
    lv_obj_set_style_width(line2, 1, 0);
    lv_obj_set_pos(line2, 630, 0);

    // sets pieces in starting order
    lv_label_set_text(board_piece[0][0], "R");
    lv_obj_set_style_text_color(board_piece[0][0], lv_color_hex(0x808080), LV_PART_MAIN);
    lv_obj_center(board_piece[0][0]);
    lv_label_set_text(board_piece[7][0], "R");
    lv_obj_set_style_text_color(board_piece[7][0], lv_color_hex(0x808080), LV_PART_MAIN);
    lv_obj_center(board_piece[7][0]);


    // creates back button
    lv_obj_t * back_btn = lv_btn_create(scr);
    lv_obj_set_size(back_btn, 120, 60);
    lv_obj_set_pos(back_btn, 10, 400);

    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xAA0000), LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_t * back_lbl = lv_label_create(back_btn);
    lv_label_set_text(back_lbl, "Back");
    lv_obj_center(back_lbl);

    lv_obj_add_event_cb(back_btn, button_event_handler, LV_EVENT_CLICKED, (void*)99);
}