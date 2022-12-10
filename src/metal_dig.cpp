#include "metal_dig.h"
#include "main_screen.h"
#include "metal_dig_ingame.h" //ingame을 구현하기 위한 헤더
#include "useful_funtion.h"   //아스키 아트 구현이 담긴 헤더
#include <cstdlib>
#include <ncurses.h>
#include <string>

using namespace std;

void metal_dig() {

    WINDOW *menu_ui;
    int key = 0; //방향키의 위치
    int get_key; //방향키를 입력 받을 변수

    initscr();   // ncurses 시작
    noecho();    //커서에 출력값 숨김
    cbreak();    //줄 버퍼링 꺼짐 -> 문자 단위로 읽을 수 있다.
    curs_set(0); //커서 위치 숨김

    //색 설정
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK); //배경:검정  글씨 :흰색
    init_pair(2, COLOR_BLACK, COLOR_WHITE); //배경:흰색  글씨 :검정
    init_pair(3, COLOR_WHITE, COLOR_CYAN);  //배경:흰색  글씨 :민트
    refresh();

    //타이틀 호출
    title();

    //윈도우2 생성및 설정
    menu_ui = newwin(15, 25, 22, 60);
    wbkgd(menu_ui, COLOR_PAIR(2));
    keypad(menu_ui, TRUE); //특수키의 입력을 활성화(방향키..)
    box(menu_ui, ACS_VLINE, ACS_HLINE);

    //선택 메뉴 출력
    //엔터가 아닐때까지 반복
    do {

        //선택 메뉴 출력
        //커서가 가리키는 문장을 강조
        if (key == 0) {
            wattron(menu_ui, COLOR_PAIR(3));
            mvwprintw(menu_ui, 2, 5, "1. game start");
            wattroff(menu_ui, COLOR_PAIR(3));

        } else {
            mvwprintw(menu_ui, 2, 5, "1. game start");
        }

        if (key == 1) {
            wattron(menu_ui, COLOR_PAIR(3));
            mvwprintw(menu_ui, 7, 5, "2. help");
            wattroff(menu_ui, COLOR_PAIR(3));

        } else {
            mvwprintw(menu_ui, 7, 5, "2. help");
        }

        if (key == 2) {
            wattron(menu_ui, COLOR_PAIR(3));
            mvwprintw(menu_ui, 12, 5, "3. exit");
            wattroff(menu_ui, COLOR_PAIR(3));

        } else {
            mvwprintw(menu_ui, 12, 5, "3. exit");
        }
        // menu_ui 화면 출력
        wrefresh(menu_ui);

        //키 입력을 구현함
        //방향키 위쪽, 아래쪽, 엔터키만 구현했다.
        //엔터키를 입력하면 커서가 가리키는 기능이 출력된다.
        //커서가 최상단 혹은 최하단에 있으면 각각 위, 아래를 누르면
        //최하단과 최상단으로 이동하게 만들었다.
        get_key = wgetch(menu_ui);
        switch (get_key) {
        //위쪽 방향키일때
        case KEY_UP:
            if (key == 0)
                key = 2;
            else
                --key;
            break;
        //아래쪽 방향키일때
        case KEY_DOWN:
            if (key == 2)
                key = 0;
            else
                ++key;
            break;
        case 10:
            switch (key) {
            // game start
            case 0:
                //인게임 화면에서는 커서가 나와야 입력 문자가 보이기 때문에
                // echo 해제함
                clear();
                echo();
                refresh();
                ingame_ui();
                noecho();
                title();
                box(menu_ui, ACS_VLINE, ACS_HLINE);
                wrefresh(menu_ui);
                break;

            // help
            case 1:
                help();
                title();
                box(menu_ui, ACS_VLINE, ACS_HLINE);
                wrefresh(menu_ui);
                break;
            // exit
            case 2:

                clrtoeol();
                refresh();
                endwin();
                return;
            }
        }
    } while (1);
}

void title() {
    WINDOW *title_ui = newwin(50, 150, 0, 0);
    string path = "metal_dig.txt"; //아스키 아트가 저장된 텍스트 파일

    //윈도우 생성 및 색 설정
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    refresh();
    wbkgd(title_ui, COLOR_PAIR(1));

    //아스키 아트 출력
    new_draw_ascii_art(title_ui, path, 7, 30, 7);
    box(title_ui, 0, 0);

    //위에서 설정했던 title_ui 출력
    wrefresh(title_ui);
}

void help() {
    //윈도우 생성 및 색 설정
    WINDOW *help_ui = newwin(30, 80, 0, 0);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    wbkgd(help_ui, COLOR_PAIR(1));
    box(help_ui, 0, 0);

    //도움말 출력
    mvwprintw(help_ui, 10, 10, "1. select level.");
    mvwprintw(help_ui, 12, 10, "2. type the word.");
    mvwprintw(help_ui, 14, 10, "3. When you type a word, AI also input words.");
    mvwprintw(help_ui, 16, 10,
              "4. When all the words are gone, you can watch total score.");
    mvwprintw(help_ui, 18, 10, "5.press enter key to exit this window.");
    wrefresh(help_ui);

    //엔터 입력시 종료
    int get_key;
    while (1) {
        get_key = wgetch(help_ui);
        if (get_key == 10)
            break;
    }

    //화면 비우고 윈도우 종료
    clear();
    delwin(help_ui);
    refresh();
}