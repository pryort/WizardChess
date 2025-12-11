#ifndef CHESS_UI_H
#define CHESS_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include <stdbool.h>

void create_chessboard(char *mode);

void back_event(lv_event_t *e);

void square_event_handler(lv_event_t *e);

bool update_board(int patience, int delay);

void legal_moves(void);

//void promotion_screen(void);

#ifdef __cplusplus
extern "C" {
#endif

#endif
