#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "main_screen.h"
#include "acidrain.h"

#define MAX 100
#define BASIC_MODE 0
#define HIDE_MODE 1
#define BLINK_MODE 2
#define REVERSE_MODE 3
#define DIAGONAL_MODE 4

// 단어 노드
typedef struct node *nodePointer;
typedef struct node {
    nodePointer right, left;
    char str[MAX]; // 출력 문자
    int row, col;  // 출력 행열
    int mode;      // 출력 모드
    int modeset;
} node;

// HP
int hp = 20;
char hp_bar[3] = "";
// SCORE
int score = 0;
char score_bar[MAX] = "";
// LEVEL
int level = 1;
int level_mode = 1;
char level_bar[2] = "";

int move_cute = 0;
int cute_mode = 1;

nodePointer ptr = NULL;

// TypingText
char typingText[20] = {0};
int enter_position = 0;

int timer = 1000000;
int word_interval = 2;
int word_clock = 0;
int levelUP_Clock = 20;
int level_Clock = 0;
int speed_up = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

std::string ascii[] = {
    R"( ________   ______    ________  ______       ______    ________    ________  ___   __        _______    ________   ___ __ __   ______      )",
    R"(/_______/\ /_____/\  /_______/\/_____/\     /_____/\  /_______/\  /_______/\/__/\ /__/\     /______/\  /_______/\ /__//_//_/\ /_____/\     )",
    R"(\::: _  \ \\:::__\/  \__.::._\/\:::_ \ \    \:::_ \ \ \::: _  \ \ \__.::._\/\::\_\\  \ \    \::::__\/__\::: _  \ \\::\| \| \ \\::::_\/_    )",
    R"( \::(_)  \ \\:\ \  __   \::\ \  \:\ \ \ \    \:(_) ) )_\::(_)  \ \   \::\ \  \:. `-\  \ \    \:\ /____/\\::(_)  \ \\:.      \ \\:\/___/\   )",
    R"(  \:: __  \ \\:\ \/_/\  _\::\ \__\:\ \ \ \    \: __ `\ \\:: __  \ \  _\::\ \__\:. _    \ \    \:\\_  _\/ \:: __  \ \\:.\-/\  \ \\::___\/_  )",
    R"(   \:.\ \  \ \\:\_\ \ \/__\::\__/\\:\/.:| |    \ \ `\ \ \\:.\ \  \ \/__\::\__/\\. \`-\  \ \    \:\_\ \ \  \:.\ \  \ \\. \  \  \ \\:\____/\ )",
    R"(    \__\/\__\/ \_____\/\________\/ \____/_/     \_\/ \_\/ \__\/\__\/\________\/ \__\/ \__\/     \_____\/   \__\/\__\/ \__\/ \__\/ \_____\/ )",

};

std::string CHOICES[] = {"1. GAME START", "2. EXIT"};


//커서이동부분
void print_menu(WINDOW *menu_win, int highlight) {
    int x = 68;
    int y = 28;

    for (int i = 0; i < 2; i++) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y + (i * 2), x, "%s", CHOICES[i].c_str());
            wattroff(menu_win, A_REVERSE);
            y++;
        } else {
            mvwprintw(menu_win, y + (i * 2), x, "%s", CHOICES[i].c_str());
            y++;
        }
    }
    wrefresh(menu_win);
}

// 메인 함수
int acidrain() {

    WINDOW *window1;
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    refresh();
    window1 = newwin(50, 150, 0, 0);
    wbkgd(window1, COLOR_PAIR(1));

    for (int i = 0; i < 7; i++) {
        mvwprintw(window1, 7 + i, 6, ascii[i].c_str());
    }

    wprintw(window1, "\n\n\n\n\n\n\n");
    wprintw(window1,
            "\n\n\n\n\t\t\t\t\t                            [ Main menu "
            "]\n\n\n\n\n\n\n"); // 메인메뉴

    curs_set(0);

    wrefresh(window1);

    initscr();
    srand(time(NULL));
    // signal 처리
    signal(SIGQUIT, exit);
    refresh();

    //커서이동부분
    keypad(window1, true);
    int key;
    int highlight = 1;
    print_menu(window1, highlight);
    while (true) {
        key = wgetch(window1);
        switch (key) {
        case KEY_UP:
            if (highlight == 1)
                highlight = 2;
            else
                highlight = 1;
            break;
        case KEY_DOWN:
            if (highlight == 2)
                highlight = 1;
            else
                highlight = 2;
            break;
        case '\n':
            if (highlight == 2) {
                endwin();
                return 0;
            } else if (highlight == 1) {
                start_game();
                all_Clear();
                acidrain();
            }
        }
        print_menu(window1, highlight);
    }

    endwin();
}

// 게임 시작 함수
void start_game() {

    WINDOW *win;
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    refresh();
    win = newwin(50, 144, 0, 0);
    wbkgd(win, COLOR_PAIR(1));
    // wattron(win, COLOR_PAIR(1));
    wrefresh(win);
    //  delwin(win);
    // endwin();

    for (int i = 0; i < 7; i++) {
        mvwprintw(win, 5 + i, 1, ascii[i].c_str());
    }
    wrefresh(win);

    int i = 0;
    pthread_t th;

    // Game Name
    move(15, 131);
    addstr("Level");
    move(16, 134);
    sprintf(level_bar, "%02d", level);
    addstr(level_bar);
    move(15, 71);
    addstr("ADDING_MODE");
    move(16, 71);
    addstr("HIDE_MODE");

    // score print
    move(16, 7);
    addstr("score : ");
    sprintf(score_bar, "%03d", score);
    addstr(score_bar);

    // hp print
    move(15, 7);
    addstr("life  : ");
    sprintf(hp_bar, "%3d", hp);
    addstr(hp_bar);
    refresh();

    draw(44, 7,
         "---------------------------------------------------------------------"
         "------------------------------------------------------------");

    // enter_position
    blankdraw(45);
    draw(45, 57, "   | Enter | : ");

    // 쓰레드 생성
    pthread_create(&th, NULL, game_Board, NULL);

    // hp > 0 이상일 동안 반복
    while (hp > 0) {

        // 반복문
        for (enter_position = 0; enter_position < 20;) {
            int c = getch();

            if (hp == 0)
                break;
            else {
                if (enter_position == 19 && c != '\n' && c != 127) {
                    move(45, 73);
                    addstr(typingText);
                }
                // enter 들어오면 문자열 찾아서 삭제
                else if (c == '\n') {
                    typingText[enter_position] = '\0';
                    word_Check(typingText);

                    for (i = 0; i < 20; i++)
                        typingText[i] = '\0';

                    draw(44, 7,
                         "-----------------------------------------------------"
                         "----------------"
                         "-----------------------------------------------------"
                         "-------");

                    blankdraw(45);
                    draw(45, 57, "   | Enter | : ");
                    move(45, 73);

                    break;

                }
                // backspace 들어오면 문자 하나 삭제
                else if (c == 127) {
                    // 남아있으면 삭제
                    if (enter_position > 0) {
                        typingText[--enter_position] = '\0';

                        draw(44, 7,
                             "-------------------------------------------------"
                             "--------------------"
                             "-------------------------------------------------"
                             "-----------");
                        blankdraw(45);
                        draw(45, 57, "   | Enter | : ");
                        move(45, 73);
                        addstr(typingText);
                    }
                    // 없으면 빈문자열 출력
                    else {
                        draw(44, 7,
                             "-------------------------------------------------"
                             "--------------------"
                             "-------------------------------------------------"
                             "-----------");
                        blankdraw(45);
                        draw(45, 57, "   | Enter | : ");
                        move(45, 73);
                        addstr("                       ");
                    }
                }
                // 입력이 되면 하나추가해서 보여주기
                else {
                    typingText[enter_position++] = c;

                    draw(44, 7,
                         "-----------------------------------------------------"
                         "----------------"
                         "-----------------------------------------------------"
                         "-------");
                    blankdraw(45);
                    draw(45, 57, "   | Enter | : ");
                    move(45, 73);
                    addstr(typingText);
                }

                refresh();
            }
        }
    }
    // 쓰레드 조인
    pthread_join(th, NULL);

    WINDOW *result;
    initscr();
    start_color();
    init_pair(2, COLOR_WHITE, COLOR_BLUE);

    refresh();
    result = newwin(10, 40, 23, 51);
    wbkgd(result, COLOR_PAIR(2));
    print_result(result, score);
    all_Clear();
    clear();
    endwin();
}

// 강제 종료
void exit(int signum) {
    all_Clear();
    curs_set(1);
    endwin();
    exit(1);
}

// ALL CLEAR
void all_Clear() {
    nodePointer temp = NULL;
    nodePointer btemp = NULL;

    if (ptr) {
        while (temp) {
            temp = ptr->right;
            btemp = ptr;

            free(btemp);

            ptr = temp;
        }
    }
}

// 단어 출력 쓰레드
void *game_Board(void *m) {
    nodePointer temp = NULL;
    nodePointer del = NULL;

    while (hp > 0) {

        word_clock++;
        level_Clock++;

        if (word_clock == word_interval) {
            add_Word(wordDB(), (rand() % 55) + 40);
            word_clock = 0;
        } else
            add_Word("", 0);

        if ((levelUP_Clock == level_Clock) && (level < 9))
            level_up();

        for (int i = 17; i < 44; i++)
            blankdraw(i); // screen clear

        temp = ptr;
        // 문자열 전부 출력
        while (temp) {
            int touch_cute =
                cute_pos_comp(temp->row, temp->col, strlen(temp->str));

            // 빈 문자열이 아닌 노드 중에서 touch가 안된 노드만 출력
            if (!touch_cute) {
                if (temp->row < 44 && strcmp(temp->str, "")) {
                    if (temp->mode == HIDE_MODE && temp->modeset == 0)
                        draw(temp->row, temp->col, temp->str);
                    else if (temp->mode == BLINK_MODE && temp->modeset == 0)
                        draw(temp->row, temp->col, temp->str);

                    if (temp->mode != HIDE_MODE && temp->mode != BLINK_MODE)
                        draw(temp->row, temp->col, temp->str);
                }
            }

            // row가 19 이상이면 없애고 점수까기
            if (temp->row >= 44 && strcmp(temp->str, "")) {
                pthread_mutex_lock(&lock); // 제어
                                           // 삭제
                del = temp;
                temp->left->right = NULL;
                temp = temp->right;
                free(del);

                // hp print
                // hp--;
                pthread_mutex_unlock(&lock); // 제어 unlock

                move(15, 7);
                addstr("life  : ");
                hp--;
                sprintf(hp_bar, "%3d", hp);
                addstr(hp_bar);
                refresh();
            }

            // row 44 이상이고 빈문자열일 경우
            else if (temp->row >= 44) {
                pthread_mutex_lock(&lock); // 제어
                                           // 삭제
                del = temp;
                temp->left->right = NULL;
                temp = temp->right;
                free(del);
                pthread_mutex_unlock(&lock); // 제어 unlock

            }

            else
                temp = temp->right;
        }

        move(45, 73); // 커서 이동

        if (speed_up)
            usleep(timer / 2);
        else
            usleep(timer); // 대기
    }
    return 0;
}

void level_up() {
    timer -= 100000; //간격 0.1초 감소
    level++;         //레벨업
    move(15, 131);
    addstr("Level");
    move(16, 134);
    sprintf(level_bar, "%02d", level);
    addstr(level_bar);
    if (level == 2) {
        move(16, 71);
        addstr("HIDE_MODE");
        addstr(" FAST_MODE");
    }
    if (level == 3) {
        move(16, 71);
        addstr("HIDE_MODE");
        addstr(" FAST_MODE");
        addstr(" BLINK_MODE");
    }
    if (level == 4) {
        move(16, 71);
        addstr("HIDE_MODE");
        addstr(" FAST_MODE");
        addstr(" BLINK_MODE");
        addstr(" REVERSE_MODE");
    }
    if (level == 5) {
        move(16, 71);
        addstr("HIDE_MODE");
        addstr(" FAST_MODE");
        addstr(" BLINK_MODE");
        addstr(" REVERSE_MODE");
        addstr(" DIAGONAL_MODE");
    }

    if (level == 6) {
        move(16, 71);
        addstr("HIDE_MODE");
        addstr(" BLINK_MODE");
        addstr(" REVERSE_MODE");
        addstr(" DIAGONAL_MODE");
        addstr("         ");
    }

    level_Clock = 0; //시계 초기화

    if (level < 6)
        level_mode++;
}

// 해당 단어 찾기 함수
void word_Check(const char *str) {

    nodePointer temp = NULL;

    temp = ptr;

    // 맨끝으로 가기
    while (temp->right)
        temp = temp->right;
    // 밑에서부터 탐색
    while (temp) {

        if (!strcmp(str, ""))
            return;

        // 같은 걸 찾으면 빈문자열로 바꿔준 후 종료
        if (!strcmp(temp->str, str)) {
            strcpy(temp->str, "");

            score++;
            move_cute++;

            // score print
            move(16, 7);
            addstr("score : ");
            sprintf(score_bar, "%03d", score);
            addstr(score_bar);
            refresh();
            return;
        }
        // 다음 노드
        else
            temp = temp->left;
    }
}

// 노드 생성
nodePointer makeNode() {
    nodePointer temp;

    temp = (nodePointer)malloc(sizeof(*temp));
    temp->right = temp->left = NULL;
    temp->mode = rand() % level_mode;
    temp->modeset = 0;
    return temp;
}

// 한 칸씩 내리기
void down_Word() {

    nodePointer temp = ptr->right;

    // row 1 증가
    while (temp) {
        temp->row += 1;

        if (temp->mode == HIDE_MODE && temp->row >= 9)
            temp->modeset = 1;

        if (temp->mode == BLINK_MODE && temp->modeset == 0)
            temp->modeset = 1;
        else if (temp->mode == BLINK_MODE && temp->modeset == 1)
            temp->modeset = 0;

        if (temp->mode == DIAGONAL_MODE)
            diagonal(&temp);

        temp = temp->right;
    }
}

// 단어 추가
void add_Word(const char *str, int col) {

    nodePointer temp;

    // 첫 노드 일 경우, 단어 추가
    if (ptr == NULL) {
        ptr = makeNode();
        strcpy(ptr->str, str); // 받은 문자열로 단어 만들기

        if (ptr->mode == REVERSE_MODE)
            reverse(&temp);

        ptr->row = 17;
        ptr->col = col;
    }

    // 단어 추가
    else {
        temp = makeNode();
        strcpy(temp->str, str); // 받은 문자열로 단어 만들기

        if (temp->mode == REVERSE_MODE)
            reverse(&temp);

        temp->row = 17;
        temp->col = col;
        temp->right = ptr; // 양방향 연결
        ptr->left = temp;  // 양방향 연결
        ptr = temp; // ptr == 맨 마지막에 생성된 노드(가장 윗줄의 단어)
                    // ptr->left == NULL, 가장 먼저 성생된 노드(가장 아랫줄의
                    // 단어)->right == NULL
        down_Word();
    }
}

// 문자열 선정
const char *wordDB() {

    const char *DB1[] = {
        "paper",    "chair",    "desk",       "book",    "note",
        "computer", "cloth",    "closet",     "sign",    "door",
        "room",     "pencil",   "pen",        "pants",   "shop",
        "building", "keyboard", "dictionary", "mouse",   "cellphone",
        "speaker",  "hospital", "car",        "bicycle", "cup"};
    const char *DB2[] = {
        "anxiety",   "boredom", "confuse",    "dragon",       "excitement",
        "frustrate", "greed",   "happiness",  "impatience",   "jealousy",
        "kindess",   "loyalty", "master",     "necessary",    "overwhelming",
        "pessimism", "quietus", "relief",     "satisfaction", "thrill",
        "union",     "view",    "worriation", "yell",         "zealot"};
    const char *DB3[] = {
        "grape",   "orange",     "potato",   "watermelon", "melon",
        "peach",   "tomato",     "onion",    "spinach",    "cabbage",
        "carrot",  "strawberry", "pepper",   "pear",       "persimmon",
        "pumpkin", "mandarin",   "mushroom", "apple",      "banana",
        "cherry",  "plum",       "jujube",   "citron",     "pineapple"};
    const char *DB4[] = {
        "Lion", "Tiger",           "Elephant", "Dog",       "Penguin",
        "Frog", "Horse",           "Giraffe",  "Sunflower", "Mugunghwa",
        "Rose", "Cherry blossoms", "Maple",    "Tulip",     "Cosmos"};

    int dice = rand() % 90;

    if (dice < 25)
        return DB1[dice];
    else if (dice < 50)
        return DB2[dice - 25];
    else if (dice < 75)
        return DB3[dice - 50];
    else
        return DB4[dice - 75];
}

// 그리기
void draw(int row, int col, const char *str) {
    move(row, col);
    addstr(str);
    refresh();
}

void blankdraw(int row) {
    move(row, 7);
    addstr("                                                                   "
           "                                                              ");
    refresh();
}

void reverse(nodePointer *node) //노드의 데이터를 반대로 표현.
{
    int size = strlen((*node)->str);
    char temp;
    if ((*node)->modeset == 0) {
        (*node)->modeset = 1;
        for (int i = 0; i < size / 2; i++) {
            temp = (*node)->str[i];
            (*node)->str[i] = (*node)->str[(size - 1) - i];
            (*node)->str[(size - 1) - i] = temp;
        }
    }
}

void diagonal(nodePointer *node) //대각선 이동
{
    int n = rand() % 2; // 0 왼쪽 1 오른쪽

    if (n == 0) {
        if ((*node)->col - 2 > 0)
            (*node)->col -= 3;
    }

    if (n == 1) {
        if ((*node)->col + strlen((*node)->str) - 3 < 80)
            (*node)->col += 3;
    }
}

void fast(int i) {
    if (speed_up == 0)
        speed_up = 1;
    else
        speed_up = 0;

    if (level < 6)
        alarm(5); // 함수가 호출된 후 5초 뒤 속도
}

int cute_pos_comp(int row, int col, int len) {
    int cute_col_1 = 60 - move_cute;
    int cute_col_2 = 63 - move_cute;

    if (cute_mode == 0) {
        if (row >= 19) { // 쭉 편 상태
            for (int i = 0; i < len; i++) {
                if ((col + i >= cute_col_1) && (col + i < cute_col_1 + 17)) {
                    return 1;
                }
            }
        }
    }

    if (cute_mode == 1) {
        if (row >= 16) {     // 쪼그린 상태
            if (row >= 19) { // 아랫부분
                for (int i = 0; i < len; i++) {
                    if ((col + i >= cute_col_1) &&
                        (col + i < (cute_col_1 + 11))) {
                        return 1;
                    }
                }
            } else {
                for (int i = 0; i < len; i++) {
                    if ((col + i >= cute_col_2) &&
                        (col + i < cute_col_2 + 4)) { // 윗부분
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

void print_result(WINDOW *result_win, int score) {
    box(result_win, '|', '-');
    mvwprintw(result_win, 2, 7, "<RESULT>");
    mvwprintw(result_win, 4, 7, "FINAL SCORE : %d", score);
    mvwprintw(result_win, 6, 7, "Press any key to go back");
    mvwprintw(result_win, 7, 7, "to main screen.");

    wrefresh(result_win);

    getch();
    clrtoeol();
    refresh();
    endwin();
}

