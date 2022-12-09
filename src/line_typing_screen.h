#ifndef LINE_TYPING_SCREEN_H
#define LINE_TYPING_SCREEN_H

#include <string>
#include <vector>
#include <ncurses.h>

//짧은 글 연습 시작 함수
void line_typing_screen();

//진척도를 출력하는 함수
void print_progression(WINDOW* information_win,std::vector<std::string> txt);

//유저에게 문자열을 입력하는 함수
std::string get_typing(WINDOW* typing_win);

//벡터에서  0을 제외하고 평균을 구함
double get_average_ignore_zero(std::vector<double> arr);

//그래프 출력하는 함수
void print_graph(WINDOW* win, int y, int x, int current_status, int total);

//결과를 출력하는 함수
void print_result(WINDOW* result_win,int average_typing_speed,int max_typing_speed, double accuration); 


#endif