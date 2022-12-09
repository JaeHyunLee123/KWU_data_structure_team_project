#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <ncurses.h>

//메뉴 창 동작하는 함수
void print_main_menu(WINDOW *menu_win, int highlight);

//1 리턴은 짧은 글 연습, 2는 긴글연습, 3은 산성비, 4는 자원캐기, 5는 종료
int print_main_screen();

#endif