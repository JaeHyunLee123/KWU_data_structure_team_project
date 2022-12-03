#ifndef ACID_RAIN
#define ACID_RAIN

#include <unistd.h>


// 단어 노드
struct node;
typedef struct node *nodePointer;

void all_Clear();
void word_Check(const char *);
nodePointer makeNode();
void down_Word();
void add_Word(const char *, int);
const char *wordDB();
void draw(int, int, const char *);
void blankdraw(int);
void exit(int);

// game
void start_game();
void *game_Board(void *);
void level_up();
// mode
void reverse(nodePointer *);
void diagonal(nodePointer *);
void cute();
int cute_pos_comp(int, int, int);
void fast(int);
int acidrain();
void test();



void print_result(WINDOW *, int);

void print_menu(WINDOW *menu_win, int highlight);

int acidrain();


#endif