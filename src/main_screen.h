#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <ncurses.h>

void print_menu(WINDOW *menu_win, int highlight);
void print_main_screen();

#endif