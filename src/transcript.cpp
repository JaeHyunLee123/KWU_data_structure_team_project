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
#include "transcript.h"

using namespace std;

#define ESC 0x1b
// #define backspace
vector<string> get;

const int MAX_BUF=1024;

void main_screen() {
    WINDOW *Windows;

    initscr();

    if (has_colors() == FALSE) {
        puts("Terminal does not support colors!");
        endwin();

    } else {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_CYAN);
        init_pair(2, COLOR_RED, COLOR_CYAN);
        init_pair(3, COLOR_BLACK, COLOR_CYAN);
    }
    char t;

    while (t != 3) {
        refresh();
        Windows = newwin(50, 150, 0, 0);
        wbkgd(Windows, COLOR_PAIR(1));
        menu(Windows);
        mvwprintw(Windows, 35, 60, "What do you want ?:");
        move(35, 79);
        wrefresh(Windows);
        attron(COLOR_PAIR(1));
        t = getch();
        attroff(COLOR_PAIR(1));
        switch (t) {
        case 1:
            typing_screen();
            break;
        case 2:
            no_screen();
            break;
        default:
            mvwprintw(Windows, 36, 60, "Wrong Input!!");
            mvwprintw(Windows, 39, 60, "--Press Any Key!--");
            getch();
            break;
        }
    }
    Windows = newwin(50, 150, 0, 0);
    wbkgd(Windows, COLOR_PAIR(1));
    mvwprintw(Windows, 25, 60, "BYE - BYE");
    wrefresh(Windows);
    getch();
    endwin();
}
void no_screen() {
    WINDOW *win;

    initscr();

    if (has_colors() == FALSE) {
        puts("Terminal does not support colors!");
        endwin();

    } else {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_CYAN);
        init_pair(2, COLOR_RED, COLOR_CYAN);
        init_pair(3, COLOR_BLACK, COLOR_CYAN);
    }

    refresh();
    win = newwin(50, 150, 0, 0);
    wbkgd(win, COLOR_PAIR(1));
    mvwprintw(win, 25, 65, "YET NOT WORKING");
    wrefresh(win);
    getch();
}
void typing_screen() {
    WINDOW *TS;
    WINDOW *info;

    initscr();

    if (has_colors() == FALSE) {
        puts("Terminal does not support colors!");
        endwin();

    } else {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_CYAN);
        init_pair(2, COLOR_RED, COLOR_CYAN);
        init_pair(3, COLOR_BLACK, COLOR_CYAN);
    }

    refresh();

    TS = newwin(50, 150, 0, 0);
    wbkgd(TS, COLOR_PAIR(1));
    keypad(TS, true);
    int ran = rand() % 3;
    sampletext(TS, ran);
    char t;
    char buf[200] = {
        '\0',
    };
    int i = 0;    // index
    int j, k = 0; // j makes buf[] reset and k change compere n th
    int line = 5; // your typing line change
    int Line = 0; // vector Line change
    char compare[200] = {
        '\0',
    };
    time_t start = 0, end = 0; // time 시작과 끝
    float gap = 0.0; // 시작과 끝이 차이
    float tasu = 0.0; // typing / time
    int typing = 0, red = 0; // 타자 친 수, 오타

    move(line, 0);
    wrefresh(TS);
    strcpy(compare, get[k].c_str());
    time(&start);
    while (t != ESC) {
        attron(COLOR_PAIR(3));
        t = getch();
        attroff(COLOR_PAIR(3));
        // if (t == KEY_BACKSPACE || t == '\b') {
        //     attron(COLOR_PAIR(1));
        //     addch(' ');
        //     attroff(COLOR_PAIR(1));
        //     move(line, i - 2);
        //     refresh();
        // }
        if (t == '\n' || i > 200) {
            strcpy(compare, get[++k].c_str());
            line += 2;
            Line++;
            move(line, 0);
            refresh();
            typing++;
            i = 0;
            for (j = 0; j < 200; j++) {
                if (buf[j]) {
                    buf[j] = '\0';
                }
            }

            if (Line == get.size() - 1) { // 모든 Line을 입력했으면 자동 종료
                break;
            }

        } else {
            buf[i] = t;
            if (buf[i] != compare[i]) { // 치다가 틀리면 위 문자는 빨갛게 변함
                move(line - 1, i);
                attron(COLOR_PAIR(2));
                addch(compare[i]);
                attroff(COLOR_PAIR(2));
                move(line, i + 1);
                refresh();
                red++;
            }
            if (strlen(compare) - 1 == i) { // 문자를 끝가지 입력했으면, 자동으로 줄 바꿈
                strcpy(compare, get[++k].c_str());
                Line++;
                line += 2;
                move(line, 0);
                refresh();
                i = -1;
                for (j = 0; j < 200; j++) {
                    if (buf[j]) {
                        buf[j] = '\0';
                    }
                }
            }
            if (Line == get.size() - 1) { // 모든 Line을 입력했으면 자동 종료
                break;
            }
            i++;
            typing++;
        }
    }
    time(&end);

    gap = end - start;
    tasu = (typing / gap) * 60;

    float per = (float)typing - (float)red; // 정확도
    per /= (float)typing;
    per *= 100;
    info = newwin(5, 70, 25, 35);
    wbkgd(info, COLOR_PAIR(1));
    mvwprintw(info, 0, 10, "time: %.lf", (float)gap);
    mvwprintw(info, 1, 10, "TaSU: %.lf", tasu);
    mvwprintw(info, 2, 10, "per: %.lf", per);
    mvwprintw(info, 3, 10, "typing: %d", typing);
    mvwprintw(info, 4, 10, "per: %d", red);
    wrefresh(info);

    getch();
}
////////////////////////////////////////////////////////////////////////////////
void sampletext(WINDOW *Windows, int k) {
    switch (k) {
    case 0:
        mvwprintw(Windows, 2, 60, " The goal of neuromorphic chips");
        draw_ascii_art(Windows, 4, 0, "data.txt");
        break;
    case 1:
        mvwprintw(Windows, 2, 60, " Why do we start this business?");
        draw_ascii_art(Windows, 4, 0, "data2.txt");
        break;
    case 2:
        mvwprintw(Windows, 2, 60, " Problems to solve");
        draw_ascii_art(Windows, 4, 0, "data3.txt");
        break;
    default:
        break;
    }
}

void draw_ascii_art(WINDOW* win, int y, int x, string txt) {
    // 문자열을 저장하는 벡터 생성
    vector<string> ascii_art; // 요기 다시 string으로 원복
    // txt파일 열기
    int fd = 0;

    char buf[MAX_BUF + 1] = {
        '\0',
    };
    ssize_t r = 0;
    // 해당 txt 없을 시 파일을 찾을 수 없다는 문구 출력 후 함수 종료
    fd = open(txt.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("open() error!");
        exit(-1);
    }
    // txt파일을 한 줄씩 읽어서 벡터에 저장
    string s;
    do {
        memset(buf, '\0', MAX_BUF + 1);
        r = read(fd, buf, MAX_BUF);
        if (r == -1) {
            perror("read() error!");
            exit(-2);
        }
        s = buf;

        //////////////////////////////////////////////////////////////////////////
        // 한줄씩 ascii_art 벡터에 넣는 작업 추가
        while (s.size()) {
            size_t pos = s.find('\n');

            if (pos != -1) {
                string s2 = s.substr(0, pos);

                if (s2.size())
                    ascii_art.push_back(s2);

                s.erase(0, pos + 1);
            }
            else {
                ascii_art.push_back(s);
                break;
            }
        }
        //////////////////////////////////////////////////////////////////////////
    } while (r > 0);
    close(fd);
    // 아스키 아트 출력
    get = ascii_art;
    for (int i = 0; i < ascii_art.size() - 1; i++) {
        mvwprintw(
            win, y + 2 * i, x, "%s",
            ascii_art[i].c_str()); // 이부분은 위에서 한줄씩 끊어서 vector에
                                   // 넣어줬기 때문에 변경 안해도 됨
    }
}


void menu(WINDOW *Windows) {
    line(Windows);

    mvwprintw(Windows, 7, 2,
              " _________  __  __   ______   ________  ___   __    _______     "
              " ______   ______    ________   ______  _________  ________  "
              "______   ______");
    mvwprintw(Windows, 8, 2,
              "/________/\\/_/\\/_/\\ /_____/\\ /_______/\\/__/\\ /__/\\ "
              "/______/\\    /_____/\\ /_____/\\  /_______/\\ "
              "/_____/\\/________/\\/_______/\\/_____/\\ /_____/\\");
    mvwprintw(Windows, 9, 2,
              "\\__.::.__\\/\\ \\ \\ \\ \\:::_ \\ \\__.::._\\/\\::\\_\\  \\ "
              "\\::::__\\/__  \\:::_ \\ \\:::_ \\ \\ \\::: _  \\ "
              "\\:::__\\/\\__.::.__\\/\\__.::._\\/\\:::__\\/ \\::::_\\/_");
    mvwprintw(Windows, 10, 2,
              "   \\::\\ \\   \\:\\_\\ \\ \\:(_) \\ \\  \\::\\ \\  \\:. `-\\  "
              "\\ \\:\\ /____/\\  \\:(_) \\ \\:(_) ) )_\\::(_)  \\ \\:\\ \\  "
              "__ \\::\\ \\     \\::\\ \\  \\:\\ \\  __\\:\\/___/\\");
    mvwprintw(Windows, 11, 2,
              "    \\::\\ \\   \\::::_\\/ \\: ___\\/  _\\::\\ \\__\\:. _    \\ "
              "\\\\:\\\\_  _\\/   \\: ___\\/ \\: __ `\\ \\:: __  \\ \\\\:\\ "
              "\\/_/\\ \\::\\ \\    _\\::\\ \\__\\:\\ \\/_/\\\\::___\\/_");
    mvwprintw(
        Windows, 12, 2,
        "     \\::\\ \\    \\::\\ \\  \\ \\ \\   /__\\::\\__/\\\\. \\`-\\  \\ "
        "\\\\:\\_\\ \\ \\    \\ \\ \\    \\ \\ `\\ \\ \\\\:.\\ \\  \\ "
        "\\\\:\\_\\ \\ \\ \\::\\ \\  /__\\::\\__/\\\\:\\_\\ \\ \\\\:\\____/\\");
    mvwprintw(Windows, 13, 2,
              "      \\__\\/     \\__\\/   \\_\\/   \\________\\/ \\__\\/ "
              "\\__\\/ \\_____\\/     \\_\\/     \\_\\/ \\_\\/ \\__\\/\\__\\/ "
              "\\_____\\/  \\__\\/  \\________\\/ \\_____\\/ \\_____\\/");

    draw_ascii_art(Windows, 18, 63, "b.txt");
    sideframe(Windows);
    mvwprintw(Windows, 49, 0,
              "|--------------------------------------------------------"
              "---------------------------------------------------------"
              "-----------------------------------|");
}
void sideframe(WINDOW *win) {
    for (int i = 1; i < 49; i++) {
        mvwprintw(win, i, 0, "|");
        mvwprintw(win, i, 149, "|");
    }
}

void line(WINDOW *Windows) {
    wprintw(Windows, "|--------------------------------------------------------"
                     "---------------------------------------------------------"
                     "-----------------------------------|");
}