#include "chess_ui.h"
#include "font_chess_35_lvgl_8.h"
#include "user_gameplay.h"
#include "color_pick.h"
#include <stdio.h>
#include <stdlib.h>
#include "esp_random.h"
#include "driver/i2c.h"

#define X_START 150
#define BUTTON_W 60
#define BOARD_W 8

typedef struct {
    lv_color_t default_color;
    int row;
    int col;
    char user_color;
} btn_userdata_t;

static uint8_t demo_board_white[8][8] = {
    {9, 11, 10, 8, 7, 10, 11, 9},
    {12, 12, 12, 12, 0, 12, 12, 12},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 12, 0, 0, 0},
    {0, 0, 0, 0, 6, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {6, 6, 6, 6, 0, 6, 6, 6},
    {3, 5, 4, 2, 1, 4, 5, 3}
};

static uint8_t demo_board_black[8][8] = {
    {3, 5, 4, 1, 2, 4, 5, 3},
    {6, 6, 6, 6, 0, 6, 6, 6},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 6, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {12, 12, 12, 12, 12, 12, 12, 12},
    {9, 11, 10, 7, 8, 10, 11, 9}
};

static lv_obj_t *last_btn = NULL;
static lv_obj_t *board_square[BOARD_W][BOARD_W];
static lv_obj_t *board_piece[BOARD_W][BOARD_W];

void back_event(lv_event_t *e) {
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    char *mode = lv_event_get_user_data(e);
    printf("Back button to color choice\n");
    color_choice_menu(mode[0]);
    
}

void square_event_handler(lv_event_t *e) {
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(btn, 0);
    const char *text = lv_label_get_text(label);
    btn_userdata_t *ud = lv_obj_get_user_data(btn);

    if (last_btn != NULL) {
        lv_obj_t *last_label = lv_obj_get_child(last_btn, 0);
        const char *last_text = lv_label_get_text(last_label);
        btn_userdata_t *last_ud = lv_obj_get_user_data(last_btn);
        lv_obj_set_style_bg_color(last_btn, last_ud->default_color, LV_PART_MAIN);
        if (ud->user_color == '1') {
            if(strcmp(text, "♔") && strcmp(text, "♕") && strcmp(text, "♖") && strcmp(text, "♗") && strcmp(text, "♘") && strcmp(text, "♙")) {
                printf("legal move\n");
                lv_label_set_text(label, last_text);
                lv_label_set_text(last_label, "");
                printf("move to (%d, %d)\n", ud->row, ud->col);
                last_btn = NULL;
                return;
            }
        }
        else {
            if(strcmp(text, "♚") && strcmp(text, "♛") && strcmp(text, "♜") && strcmp(text, "♝") && strcmp(text, "♞") && strcmp(text, "♟")) {
                printf("legal move\n");
                lv_label_set_text(label, last_text);
                lv_label_set_text(last_label, "");
                printf("move to (%d, %d)\n", ud->row, ud->col);
                last_btn = NULL;
                return;
            }
        }
    }

    if (ud->user_color == '1') {
        if(strcmp(text, "♔") == 0 || strcmp(text, "♕") == 0 || strcmp(text, "♖") == 0 || strcmp(text, "♗") == 0 || strcmp(text, "♘") == 0 || strcmp(text, "♙") == 0) {
            printf("Selected piece: (%d, %d)\n", ud->row, ud->col);
            printf("%s\n", text);
            lv_obj_set_style_bg_color(btn, lv_color_hex(0x008000), LV_PART_MAIN);
            last_btn = btn;
        }
    }
    else {
        if(strcmp(text, "♚") == 0 || strcmp(text, "♛") == 0 || strcmp(text, "♜") == 0 || strcmp(text, "♝") == 0 || strcmp(text, "♞") == 0 || strcmp(text, "♟") == 0) {
            printf("Selected piece: (%d, %d)\n", ud->row, ud->col);
            printf("%s\n", text);
            lv_obj_set_style_bg_color(btn, lv_color_hex(0x008000), LV_PART_MAIN);
            last_btn = btn;
        }
    }

    

    printf("Square clicked\n");
}

void create_chessboard(char *mode) {
    // sets screen to white
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), 0);
    lv_scr_load(scr);
    printf("mode %c, color %c\n", mode[0], mode[1]);

    if(mode[0] == 'u') {
        // creates chess board
        for (int i = 0; i < BOARD_W; i++) {
            for (int j = 0; j < BOARD_W; j++) {
                // creates chess board
                lv_obj_t *btn = lv_btn_create(scr);
                lv_obj_set_size(btn, BUTTON_W, BUTTON_W);
                lv_obj_set_style_radius(btn, 0, LV_PART_MAIN);
                lv_obj_set_pos(btn, X_START + i * BUTTON_W, 0 + j * BUTTON_W);

                // colors chess board
                lv_color_t def_col = ((i + j) % 2 == 1) ? lv_color_hex(0x808080): lv_color_hex(0xFFFFFF);
                lv_obj_set_style_bg_color(btn, def_col, LV_PART_MAIN);

                lv_obj_t *label = lv_label_create(btn);
                lv_label_set_text(label, "");
                lv_obj_center(label);

                board_piece[i][j] = label;
                board_square[i][j] = btn;

                // allows for unicode chess piece labels
                lv_obj_set_style_text_font(board_piece[i][j], &font_chess_35, 0);
                lv_obj_set_style_text_color(board_piece[i][j], lv_color_hex(0x000000), LV_PART_MAIN);
                lv_obj_center(board_piece[i][j]);
                
                // allocates memory for buttons
                btn_userdata_t *ud = lv_mem_alloc(sizeof(btn_userdata_t));
                ud->default_color = def_col;
                ud->row = i;
                ud->col = j;
                ud->user_color = mode[1];
                lv_obj_set_user_data(btn, ud);
                lv_obj_add_event_cb(btn, square_event_handler, LV_EVENT_ALL, ud);
            }
        }

        // sets pieces in starting order
        int row = 1;
        if(mode[1] == '1') {
            printf("user is white\n");

            lv_obj_t *demo_btn = lv_btn_create(scr);
            lv_obj_set_size(demo_btn, 120, 60);
            lv_obj_set_pos(demo_btn, 650, 400);

            lv_obj_t *demo_lbl = lv_label_create(demo_btn);
            lv_label_set_text(demo_lbl, "Demo");
            lv_obj_center(demo_lbl);

            lv_obj_add_event_cb(demo_btn, update_board_white, LV_EVENT_CLICKED, demo_board_white);

            for (int i = 0; i < BOARD_W; i++) {
                // ♟ black pawn
                lv_label_set_text(board_piece[i][row], "♟");
            }
            row = 6;
            for (int i = 0; i < BOARD_W; i++) {
                // ♙ white pawn
                lv_label_set_text(board_piece[i][row], "♙");
            }
            
            // ♜ black rook
            lv_label_set_text(board_piece[0][0], "♜");

            // ♞ black knight
            lv_label_set_text(board_piece[1][0], "♞");

            // ♝ black bishop
            lv_label_set_text(board_piece[2][0], "♝");

            // ♛ black queen
            lv_label_set_text(board_piece[3][0], "♛");

            // ♚ black king
            lv_label_set_text(board_piece[4][0], "♚");

            // ♝ black bishop
            lv_label_set_text(board_piece[5][0], "♝");

            // ♞ black knight
            lv_label_set_text(board_piece[6][0], "♞");

            // ♜ black rook
            lv_label_set_text(board_piece[7][0], "♜");

            // ♖ white rook
            lv_label_set_text(board_piece[0][7], "♖");

            // ♘ white knight
            lv_label_set_text(board_piece[1][7], "♘");

            // ♗ white bishop
            lv_label_set_text(board_piece[2][7], "♗");

            // ♕ white queen
            lv_label_set_text(board_piece[3][7], "♕");

            // ♔ white king
            lv_label_set_text(board_piece[4][7], "♔");

            // ♗ white bishop
            lv_label_set_text(board_piece[5][7], "♗");

            // ♘ white knight
            lv_label_set_text(board_piece[6][7], "♘");

            // ♖ white rook
            lv_label_set_text(board_piece[7][7], "♖");
        }
        else {
            printf("user is black\n");

            lv_obj_t *demo_btn = lv_btn_create(scr);
            lv_obj_set_size(demo_btn, 120, 60);
            lv_obj_set_pos(demo_btn, 650, 400);

            lv_obj_t *demo_lbl = lv_label_create(demo_btn);
            lv_label_set_text(demo_lbl, "Demo");
            lv_obj_center(demo_lbl);

            lv_obj_add_event_cb(demo_btn, update_board_black, LV_EVENT_CLICKED, demo_board_black);

            for (int i = 0; i < BOARD_W; i++) {
                // ♙ white pawn
                lv_label_set_text(board_piece[i][row], "♙");
            }
            row = 6;
            for (int i = 0; i < BOARD_W; i++) {
                // ♟ black pawn
                lv_label_set_text(board_piece[i][row], "♟");
            }
            
            // ♖ white rook
            lv_label_set_text(board_piece[0][0], "♖");

            // ♘ white knight
            lv_label_set_text(board_piece[1][0], "♘");

            // ♗ white bishop
            lv_label_set_text(board_piece[2][0], "♗");

            // ♔ white king
            lv_label_set_text(board_piece[3][0], "♔");

            // ♕ white queen
            lv_label_set_text(board_piece[4][0], "♕");

            // ♗ white bishop
            lv_label_set_text(board_piece[5][0], "♗");

            // ♘ white knight
            lv_label_set_text(board_piece[6][0], "♘");

            // ♖ white rook
            lv_label_set_text(board_piece[7][0], "♖");

            // ♜ black rook
            lv_label_set_text(board_piece[0][7], "♜");

            // ♞ black knight
            lv_label_set_text(board_piece[1][7], "♞");

            // ♝ black bishop
            lv_label_set_text(board_piece[2][7], "♝");

            // ♚ black king
            lv_label_set_text(board_piece[3][7], "♚");

            // ♛ black queen
            lv_label_set_text(board_piece[4][7], "♛");

            // ♝ black bishop
            lv_label_set_text(board_piece[5][7], "♝");

            // ♞ black knight
            lv_label_set_text(board_piece[6][7], "♞");

            // ♜ black rook
            lv_label_set_text(board_piece[7][7], "♜");
        }
    }
    else {
        for (int i = 0; i < BOARD_W; i++) {
            for (int j = 0; j < BOARD_W; j++) {
                // creates chess board
                lv_obj_t *square = lv_obj_create(scr);
                lv_obj_set_size(square, BUTTON_W, BUTTON_W);
                lv_obj_set_style_radius(square, 0, LV_PART_MAIN);
                lv_obj_set_style_border_width(square, 0, LV_PART_MAIN);
                lv_obj_set_style_outline_width(square, 0, LV_PART_MAIN);
                lv_obj_clear_flag(square, LV_OBJ_FLAG_SCROLLABLE);
                lv_obj_set_scrollbar_mode(square, LV_SCROLLBAR_MODE_OFF);

                lv_obj_set_pos(square, X_START + i * BUTTON_W, 0 + j * BUTTON_W);

                // colors chess board
                lv_color_t def_col = ((i + j) % 2 == 1) ? lv_color_hex(0x808080): lv_color_hex(0xFFFFFF);
                lv_obj_set_style_bg_color(square, def_col, LV_PART_MAIN);

                lv_obj_t *label = lv_label_create(square);
                lv_label_set_text(label, "");
                lv_obj_center(label);

                board_piece[i][j] = label;
                board_square[i][j] = square;

                // allows for unicode chess piece labels
                lv_obj_set_style_text_font(board_piece[i][j], &font_chess_35, 0);
                lv_obj_set_style_text_color(board_piece[i][j], lv_color_hex(0x000000), LV_PART_MAIN);
                lv_obj_center(board_piece[i][j]);
            }
        }

        // sets pieces in starting order
        int row = 1;
        if(mode[1] == '1') {
            printf("user is white\n");

            lv_obj_t *demo_btn = lv_btn_create(scr);
            lv_obj_set_size(demo_btn, 120, 60);
            lv_obj_set_pos(demo_btn, 650, 400);

            lv_obj_t *demo_lbl = lv_label_create(demo_btn);
            lv_label_set_text(demo_lbl, "Demo");
            lv_obj_center(demo_lbl);

            lv_obj_add_event_cb(demo_btn, update_board_white, LV_EVENT_CLICKED, demo_board_white);

            for (int i = 0; i < BOARD_W; i++) {
                // ♟ black pawn
                lv_label_set_text(board_piece[i][row], "♟");
            }
            row = 6;
            for (int i = 0; i < BOARD_W; i++) {
                // ♙ white pawn
                lv_label_set_text(board_piece[i][row], "♙");
            }
            
            // ♜ black rook
            lv_label_set_text(board_piece[0][0], "♜");

            // ♞ black knight
            lv_label_set_text(board_piece[1][0], "♞");

            // ♝ black bishop
            lv_label_set_text(board_piece[2][0], "♝");

            // ♛ black queen
            lv_label_set_text(board_piece[3][0], "♛");

            // ♚ black king
            lv_label_set_text(board_piece[4][0], "♚");

            // ♝ black bishop
            lv_label_set_text(board_piece[5][0], "♝");

            // ♞ black knight
            lv_label_set_text(board_piece[6][0], "♞");

            // ♜ black rook
            lv_label_set_text(board_piece[7][0], "♜");

            // ♖ white rook
            lv_label_set_text(board_piece[0][7], "♖");

            // ♘ white knight
            lv_label_set_text(board_piece[1][7], "♘");

            // ♗ white bishop
            lv_label_set_text(board_piece[2][7], "♗");

            // ♕ white queen
            lv_label_set_text(board_piece[3][7], "♕");

            // ♔ white king
            lv_label_set_text(board_piece[4][7], "♔");

            // ♗ white bishop
            lv_label_set_text(board_piece[5][7], "♗");

            // ♘ white knight
            lv_label_set_text(board_piece[6][7], "♘");

            // ♖ white rook
            lv_label_set_text(board_piece[7][7], "♖");
        }
        else {
            printf("user is black\n");

            lv_obj_t *demo_btn = lv_btn_create(scr);
            lv_obj_set_size(demo_btn, 120, 60);
            lv_obj_set_pos(demo_btn, 650, 400);

            lv_obj_t *demo_lbl = lv_label_create(demo_btn);
            lv_label_set_text(demo_lbl, "Demo");
            lv_obj_center(demo_lbl);

            lv_obj_add_event_cb(demo_btn, update_board_black, LV_EVENT_CLICKED, demo_board_black);

            for (int i = 0; i < BOARD_W; i++) {
                // ♙ white pawn
                lv_label_set_text(board_piece[i][row], "♙");
            }
            row = 6;
            for (int i = 0; i < BOARD_W; i++) {
                // ♟ black pawn
                lv_label_set_text(board_piece[i][row], "♟");
            }
            
            // ♖ white rook
            lv_label_set_text(board_piece[0][0], "♖");

            // ♘ white knight
            lv_label_set_text(board_piece[1][0], "♘");

            // ♗ white bishop
            lv_label_set_text(board_piece[2][0], "♗");

            // ♔ white king
            lv_label_set_text(board_piece[3][0], "♔");

            // ♕ white queen
            lv_label_set_text(board_piece[4][0], "♕");

            // ♗ white bishop
            lv_label_set_text(board_piece[5][0], "♗");

            // ♘ white knight
            lv_label_set_text(board_piece[6][0], "♘");

            // ♖ white rook
            lv_label_set_text(board_piece[7][0], "♖");

            // ♜ black rook
            lv_label_set_text(board_piece[0][7], "♜");

            // ♞ black knight
            lv_label_set_text(board_piece[1][7], "♞");

            // ♝ black bishop
            lv_label_set_text(board_piece[2][7], "♝");

            // ♚ black king
            lv_label_set_text(board_piece[3][7], "♚");

            // ♛ black queen
            lv_label_set_text(board_piece[4][7], "♛");

            // ♝ black bishop
            lv_label_set_text(board_piece[5][7], "♝");

            // ♞ black knight
            lv_label_set_text(board_piece[6][7], "♞");

            // ♜ black rook
            lv_label_set_text(board_piece[7][7], "♜");
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

    // creates back button
    lv_obj_t *back_btn = lv_btn_create(scr);
    lv_obj_set_size(back_btn, 120, 60);
    lv_obj_set_pos(back_btn, 10, 400);

    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_set_style_bg_color(back_btn, lv_color_hex(0xAA0000), LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_t *back_lbl = lv_label_create(back_btn);
    lv_label_set_text(back_lbl, "Exit Game");
    lv_obj_center(back_lbl);

    lv_obj_add_event_cb(back_btn, back_event, LV_EVENT_CLICKED, mode);
}

void update_board_white(lv_event_t *e) {
    uint8_t (*board)[8] = lv_event_get_user_data(e);

    const char *pieces[] = {
        "", "♔", "♕", "♖", "♗", "♘", "♙", "♚", "♛", "♜", "♝", "♞", "♟"
    };

    printf("Board state received:\n");
    for (int i = 0; i < BOARD_W; i++) {
        for (int j = 0; j < BOARD_W; j++) {
            lv_label_set_text(board_piece[j][i], pieces[board[i][j]]);
            if(strcmp(pieces[board[i][j]], "") == 0) {
                printf("  ");
            }
            else {
                printf("%s ", pieces[board[i][j]]);
            }
        }
        printf("\n");
    }
}

void update_board_black(lv_event_t *e) {
    uint8_t (*board)[8] = lv_event_get_user_data(e);

    const char *pieces[] = {
        "", "♔", "♕", "♖", "♗", "♘", "♙", "♚", "♛", "♜", "♝", "♞", "♟"
    };

    printf("Board state received:\n");
    for (int i = 0; i < BOARD_W; i++) {
        for (int j = 0; j < BOARD_W; j++) {
            lv_label_set_text(board_piece[j][i], pieces[board[i][j]]);
            if(strcmp(pieces[board[i][j]], "") == 0) {
                printf("  ");
            }
            else {
                printf("%s ", pieces[board[i][j]]);
            }
        }
        printf("\n");
    }
}