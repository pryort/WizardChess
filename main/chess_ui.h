#ifndef CHESS_UI_H
#define CHESS_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void create_chessboard(char *mode);

void back_event(lv_event_t *e);

void square_event_handler(lv_event_t *e);

void update_board(void);

#ifdef __cplusplus
extern "C" {
#endif

#endif
