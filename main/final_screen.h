#ifndef FINAL_SCREEN_H
#define FINAL_SCREEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void win_screen(void);

void lose_screen(void);

void tie_screen(void);

//void color_event_handler(lv_event_t * e);

void back_to_menu(lv_event_t *e);


#ifdef __cplusplus
extern "C" {
#endif

#endif
