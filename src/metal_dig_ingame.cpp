#include "metal_dig_ingame.h"
#include "useful_funtion.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <ncurses.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>
// 게임에서 출력되는 단어의 수 정의
const int WORD_COUNT = 35;

using namespace std;

// ingame_ui함수
void ingame_ui() {
    string path = "./worker.txt";
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_CYAN);
    refresh();
    int key = 0;   // 방향키의 위치
    int get_key;   // 방향키를 입력 받을 변수
    int level = 0; // 레벨을 저장할 변수

    // level을 선택하는 ui 생성 및 설정
    WINDOW *select_level = newwin(20, 25, 20, 60);
    wbkgd(select_level, COLOR_PAIR(2));
    keypad(select_level, TRUE); // 특수키의 입력을 활성화(방향키..)
    box(select_level, ACS_VLINE, ACS_HLINE);
    // 레벨을 선택할때까지 반복한다.
    do {

        // 선택 메뉴 출력
        if (key == 0) {
            wattron(select_level, COLOR_PAIR(3));
            mvwprintw(select_level, 2, 5, "level 1");
            wattroff(select_level, COLOR_PAIR(3));

        } else {
            mvwprintw(select_level, 2, 5, "level 1");
        }

        if (key == 1) {
            wattron(select_level, COLOR_PAIR(3));
            mvwprintw(select_level, 7, 5, "level 2");
            wattroff(select_level, COLOR_PAIR(3));

        } else {
            mvwprintw(select_level, 7, 5, "level 2");
        }

        if (key == 2) {
            wattron(select_level, COLOR_PAIR(3));
            mvwprintw(select_level, 12, 5, "level 3");
            wattroff(select_level, COLOR_PAIR(3));

        } else {
            mvwprintw(select_level, 12, 5, "level 3");
        }

        if (key == 3) {
            wattron(select_level, COLOR_PAIR(3));
            mvwprintw(select_level, 17, 5, "exit");
            wattroff(select_level, COLOR_PAIR(3));

        } else {
            mvwprintw(select_level, 17, 5, "exit");
        }

        // select_level 출력
        wrefresh(select_level);

        get_key = wgetch(select_level);
        switch (get_key) {
        // 위쪽 방향키일때
        case KEY_UP:
            if (key == 0)
                key = 3;
            else
                --key;
            break;
        // 아래쪽 방향키일때
        case KEY_DOWN:
            if (key == 3)
                key = 0;
            else
                ++key;
            break;
        case 10:
            switch (key) {
            // level return ;
            case 0:
                level = 1;
                werase(select_level);
                delwin(select_level);
                break;

            case 1:
                level = 2;
                werase(select_level);
                delwin(select_level);
                break;

            case 2:
                level = 3;
                werase(select_level);
                delwin(select_level);
                break;
            // exit를 선택한 경우로 타이틀로 돌아간다.
            case 3:
                return;
            }
        }
    } while (level == 0);

    // 아스키 아트가 포함된 ui를 구현한다.
    WINDOW *ingame_ui = newwin(40, 150, 0, 0);
    wbkgd(ingame_ui, COLOR_PAIR(1));

    // 아스키 아트 출력
    new_draw_ascii_art(ingame_ui, path, 7, 10, 7);
    new_draw_ascii_art(ingame_ui, path, 30, 15, 7);
    new_draw_ascii_art(ingame_ui, path, 25, 130, 7);
    new_draw_ascii_art(ingame_ui, path, 6, 120, 7);

    box(ingame_ui, 0, 0);
    wrefresh(ingame_ui);

    // 선택한 레벨을 인자로하는 play_game함수 호출
    play_game(level);

    // play_game이 종료되면 타이틀로 돌아가기 위해 화면을 비우고 생성한 모든
    // 윈도우를 반환한다ee
    werase(ingame_ui);
    delwin(ingame_ui);
}

// choose_word함수
vector<string> choose_word() {
    int fd = 0;
    int random;
    string txt = "./metal_dig_word.txt";
    char buf[1] = {'\0'};
    ssize_t rsize = 0;
    string word = "";
    vector<string> total_word;
    vector<string> select_word;

    // 텍스트을 연다.
    fd = open(txt.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("open() error!");
        exit(-1);
    }

    // 텍스트에 저장된 단어를 total_word에 저장한다.
    for (int i = 0; i < 250; i++) {
        memset(buf, '\0', 1);

        while (1) {
            rsize = read(fd, buf, 1);
            if ((buf[0] == ' ') || (buf[0] == 10))
                break;
            if (rsize == -1) {
                perror("read() error!\n");
                exit(-2);
            }
            word += buf[0];
        }

        if (word != "") {
            total_word.push_back(word);
            word = "";
        } else {
            --i;
        }
    }
    // 파일 지시자 닫고 랜덤 생성
    close(fd);
    srand((unsigned int)time(NULL));

    // 중복되는 단어 없이 선정후 select_word에 저장
    for (int i = 0; i < WORD_COUNT; i++) {

        random = rand() % 250;
        if (find(select_word.begin(), select_word.end(), total_word[random]) ==
            select_word.end()) {
            select_word.push_back(total_word[random]);
        } else {
            i--;
        }
    }
    // 랜덤 선정한 단어가 저장된 벡터 반환
    return select_word;
}

// score_ui를 출력하는 함수
void showScore(WINDOW *score_ui, int player_score, int ai_score, int level) {
    werase(score_ui);
    score_ui = newwin(50, 30, 0, 150);

    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    wbkgd(score_ui, COLOR_PAIR(2));
    box(score_ui, 0, 0);
    wrefresh(score_ui);

    mvwprintw(score_ui, 10, 5, "Level : %d", level);
    mvwprintw(score_ui, 20, 5, "Player Score : %d", player_score);
    mvwprintw(score_ui, 30, 5, "AI Score : %d", ai_score);
    wrefresh(score_ui);
}

// word_box를 출력하는 함수
void show_box(WINDOW *word_box, vector<string> word, vector<int> count) {
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    // 한 행에 5개의 단어가 존재하고 count를 확인해서 존재하는 단어만 출력하고
    // 이미 입력된 단어는 출력하지 않는다.
    for (int i = 0; i < WORD_COUNT; i += 5) {
        if (count[0 + i] == 1) {
            mvwprintw(word_box, 1 + ((i / 5) * 3), 2, "%s",
                      word[0 + i].c_str());
        }
        if (count[1 + i] == 1) {
            mvwprintw(word_box, 1 + ((i / 5) * 3), 15, "%s",
                      word[1 + i].c_str());
        }
        if (count[2 + i] == 1) {
            mvwprintw(word_box, 1 + ((i / 5) * 3), 28, "%s",
                      word[2 + i].c_str());
        }
        if (count[3 + i] == 1) {
            mvwprintw(word_box, 1 + ((i / 5) * 3), 41, "%s",
                      word[3 + i].c_str());
        }
        if (count[4 + i] == 1) {
            mvwprintw(word_box, 1 + ((i / 5) * 3), 54, "%s",
                      word[4 + i].c_str());
        }
    }
    wrefresh(word_box);
}

// text_box 창을 호출하는 함수
void showTextBox(WINDOW *text_box) {
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    wbkgd(text_box, COLOR_PAIR(1));

    wclear(text_box);
    box(text_box, 0, 0);
    mvwhline(text_box, 7, 65, '-', 20);
    wrefresh(text_box);
}

void AI(vector<int> &empty, vector<int> logic, WINDOW *word_box,
        WINDOW *text_box, WINDOW *score_box, vector<string> word, int level,
        int &ai_score, int &player_score) {

    int x = 0, y = 0; // 커서의 값을 저장하기 위한 변수다.
    int count = 0;

    // isEmpty에 1이 존재하면 반복->1이 있다는 것은 아직 입력되지 못한 단어가
    // 남아있다는 의미이다.
    while (find(empty.begin(), empty.end(), 1) != empty.end()) {
        // level에 따라 속도를 조절한다.
        if (level == 1) {
            sleep(5);
        } else if (level == 2) {
            sleep(3);
        } else if (level == 3) {
            sleep(1);
        }
        // 단어가 입력되지 않은거면 삭제하고 점수 올리고 화면갱신
        // 입력된거면 count를 올려서 logic의 다음 값을 불러온다
        while (1) {
            if (empty[logic[count]] == 1) {
                empty[logic[count]] = 0;
                getyx(text_box, y, x);
                werase(word_box);
                box(word_box, 0, 0);
                show_box(word_box, word, empty);
                wrefresh(word_box);
                wmove(text_box, y, x);
                ai_score += 100;
                showScore(score_box, player_score, ai_score, level);
            }
            ++count;
            break;
        }
    }
}

// 게임의 결과를 보여주는 함수
void showTotalScore(int player_score, int ai_score) {
    WINDOW *total_score_ui = newwin(15, 25, 22, 60);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_CYAN);
    refresh();

    wbkgd(total_score_ui, COLOR_PAIR(2));
    box(total_score_ui, 0, 0);

    mvwprintw(total_score_ui, 2, 3, "player socore : %d", player_score);
    mvwprintw(total_score_ui, 7, 3, "AI socore : %d", ai_score);
    wattron(total_score_ui, COLOR_PAIR(3));
    if (player_score > ai_score) {

        mvwprintw(total_score_ui, 12, 3, "Player Win!!");

    } else if (player_score < ai_score) {

        mvwprintw(total_score_ui, 12, 3, "Player Lose");
    } else {
        mvwprintw(total_score_ui, 12, 3, "Draw");
    }
    wattroff(total_score_ui, COLOR_PAIR(3));
    wrefresh(total_score_ui);
    getchar();
    wclear(total_score_ui);
    delwin(total_score_ui);
}

void play_game(int level) {
    WINDOW *score_ui = newwin(50, 30, 0, 150);
    WINDOW *word_box = newwin(25, 70, 7, 40);
    WINDOW *text_box = newwin(10, 150, 40, 0);
    vector<int> ai_logic; // ai의 단어 호출 순서를 저장할 변수
    int ai_score = 0;
    int player_score = 0;
    vector<string> word = choose_word();
    vector<int> isEmpty(WORD_COUNT, 1); // 원소가 1이면 미입력, 0이면 입력된것

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);

    wbkgd(word_box, COLOR_PAIR(1));

    box(word_box, 0, 0);
    mvwprintw(word_box, 10, 30, "wait, please...");

    wrefresh(word_box);
    showTextBox(text_box);
    showScore(score_ui, player_score, ai_score, level);

    int random;
    srand((unsigned int)time(NULL));

    // ai가 단어를 입력할 순서를 정한다.
    // 미리 정해놓지 않으면 ai 작동시 오차가 발생해서 성능 저하가 발생하기
    // 때문이다.
    while (ai_logic.size() < WORD_COUNT) {
        random = rand() % 35;
        if (find(ai_logic.begin(), ai_logic.end(), random) == ai_logic.end()) {
            ai_logic.push_back(random);
        }
    }
    werase(word_box);
    box(word_box, 0, 0);
    mvwprintw(word_box, 10, 30, "start!");
    wrefresh(word_box);
    sleep(3);

    werase(word_box);
    box(word_box, 0, 0);
    show_box(word_box, word, isEmpty);

    // ai 작동
    thread t1(AI, ref(isEmpty), ai_logic, word_box, text_box, score_ui, word,
              level, ref(ai_score), ref(player_score));

    // 플레이어 차례
    char buf[20] = {
        '\0',
    };
    int index = 0;
    // 미입력된 단어가 남아있다면 반복문 작동
    while (find(isEmpty.begin(), isEmpty.end(), 1) != isEmpty.end()) {
        // 10이란 숫자가 개행 문자를 나타냄
        // 개행 문자가 나올때까지 반복할 수 있게 된다.
        memset(buf, '\0', 19);
        mvwgetnstr(text_box, 6, 65, buf, 19);

        // 입력한 단어가 존재하면 단어 삭제, 점수 상승, 화면 초기화 지행한다.
        // 아니면 입력 창만 초기화
        auto it = find(word.begin(), word.end(), buf);
        if (it != word.end()) {
            index = it - word.begin();
            if (isEmpty[index] == 1) {
                isEmpty[index] = 0;
                werase(word_box);
                box(word_box, 0, 0);
                show_box(word_box, word, isEmpty);
                wrefresh(word_box);
                showTextBox(text_box);
                player_score += 100;
                showScore(score_ui, player_score, ai_score, level);
            } else {
                showTextBox(text_box);
            }
        } else {
            showTextBox(text_box);
        }
    }
    t1.join(); // 스레드 종료까지 대기
    clear();
    showTotalScore(player_score, ai_score); // 총점 출력

    clear();
    delwin(score_ui);
    delwin(word_box);
    delwin(text_box);
    refresh();
}