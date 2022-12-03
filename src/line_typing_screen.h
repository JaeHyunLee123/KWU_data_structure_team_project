#ifndef LINE_TYPING_SCREEN_H
#define LINE_TYPING_SCREEN_H

#include <string>
#include <vector>
#include <ncurses.h>

void line_typing_screen();

void print_progression(WINDOW* information_win,std::vector<std::string> txt);

std::string get_typing(WINDOW* typing_win);

double get_average_ignore_zero(std::vector<double> arr);

void print_graph(WINDOW* win, int y, int x, int current_status, int total);

void print_result(WINDOW* result_win,int average_typing_speed,int max_typing_speed, double accuration); 


#endif