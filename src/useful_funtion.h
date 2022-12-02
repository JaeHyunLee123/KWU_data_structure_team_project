#ifndef USEFUL_FUNTION_H
#define USEFUL_FUNTION_H

#include <ncurses.h>
#include <string>
#include <vector>
#include <stdlib.h>

/*-------------------- Funtion Declalation --------------------*/

//인자 설명 (출력하려는 화면, y축, x측 , 아스키아트를 저장한 string 배열, 배열의
//크기)
void new_draw_ascii_art(WINDOW *win, std::string txt, int y, int x, int height);

void draw_ascii_art(WINDOW *win, int y, int x, std::string txt[], int arr_size);

//인자 설명 (읽을 txt 파일 이름, txt파일의 행 길이)
//prefix: txt파일의 줄 끝 마다 '<'가 있어야 함
//postfix: txt 파일의 글을 한 줄씩 벡터에 저장
std::vector<std::string> read_txt_line_by_line(std::string txt, int height);

int get_random_number(int min, int max);

template<typename T>
void pop_front(std::vector<T> &v);

std::vector<std::string> shrink_randomly(std::vector<std::string> target, int size);


#endif
