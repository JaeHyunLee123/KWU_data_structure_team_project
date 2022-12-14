#ifndef TRANSCRIPT_H
#define TRANSCRIPT_H

#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <vector>

using namespace std;


void main_screen();
void no_screen();
void typing_screen();
void sampletext(WINDOW*, int);
void draw_ascii_art(WINDOW*, int, int, string);
void menu(WINDOW*);
void sideframe(WINDOW*);
void line(WINDOW*);

#endif