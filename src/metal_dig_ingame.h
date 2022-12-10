/*
게임을 구현하는 코드를 담은 헤더이다.
게임과정을 설명해보겠다.
1. 레벨1~3 중 한개를 선택한다. 이 때, exit을 선택하면 타이틀로 돌아간다.
2. 게임을 한다.
3. 게임이 끝난 후 점수가 나온다.
4. 타이틀로 돌아간다.

함수에 대해 간단히 설명하겠다.
1.void ingame_ui()
가장 처음 호출되고 가장 마지막에 종료되기에 게임 구현 코드들의 메인함수라고
볼수 있다.
worker.txt에 저장된 아스키 아트룰 구현하고 레벨 선택을 구현한 윈도우를 호출한다.
레벨을 선택하면 play_game()을 호출해서 게임을 실행하고 게임이 끝난 후 다시 이
함수로 돌아와서 타이틀로 돌아간다.

2.vector<string> choose_word()함수
텍스트에 저장되어 있는 단어를 WORD_COUNT만큼 vector<string>에 저장한다.
이때 srand를 사용해서 랜덤하게 저장한다.

3.void showScore(WINDOW *score_ui, int player_score, int ai_score)함수
score_ui의 window 변수와 player, ai의 점수가 저장된 변수가 인자로 받는다.
그럼 점수가 기록된 score_ui가 출력된다.

4.void show_box(WINDOW *word_box, vector<string> word, vector<int> count)
인자를 설명하면 word_box는 문자가 출력되는 창의 변수명이다.
word는 WORD_COUNT 개의 선정된 단어가 저장되어 있는 벡터이고 count는 벡터의
원소가 사용되었는지 확인하기 위한 벡터다
이 함수를 실행하면 word_box가 출력되고 이미 입력해서 사라진 단어는 나오지
않는다.

5.void showTextBox(WINDOW *text_box)
인자로 text_box는 유저가 단어를 입력하는 창의 변수명이다.
호출하면 text_box를 출력한다.

6.void AI(vector<int> &empty, vector<int> logic, WINDOW *word_box,
        WINDOW *text_box, WINDOW *score_box, vector<string> word, int level,
        int &ai_score, int &player_score);
스레드용으로 만든 함수로 ai의 작동을 구현한 함수다.
많은 인자를 받는데 순서대로 설명하겠다.
empty는 4번째 함수의 count와 원본을 공유하는 변수다. 레퍼런스로 호출한 이유는
수정한 데이터가 유지되어야 하기 때문이다.
logic은 ai가 word 배열에서 단어를 불러올 순서를 저장한 벡터다. 시간 소요를
줄이기 위해 미리 만들어야 했다.
다음에 오는 3변수는 함수에서 호출한 3개의 윈도우 변수다.
word는 랜덤으로 선정했던 WORD_COUNT개의 단어가 저장된 벡타다.
level은 첫번째 함수에서 선택했던 level이다.
나머지 2변수는 유저와 ai의 score가 저장된 변수다.

함수 자제는 간단하다. 모든 단어를 입력할 때까지 반복하고 더이상 입력할 단어가
없으면 함수가 종료된다.


7.void showTotalScore(int player_score, int ai_score)
게임이 끝나고 각자의 점수를 보여준다.
enter을 누르면 종료한다.

8.void play_game(int level)
ingame_ui에서 호출되서 게임을 진행하는 함수이다.
스레드로 ai가 돌아가는 동안 플레이어도 단어를 입력하고 더 이상 입력할 단어가
남아있지 않으면 총점을 보여주고 함수를 종료한다.
*/

#ifndef METAL_DIG_INGAME_H
#define METAL_DIG_INGAME_H

#include <ncurses.h>
#include <string>
#include <vector>

using namespace std;

// 기본적인 UI생성
void ingame_ui();

// 게임에 사용할 단어 선택 함수
vector<string> choose_word();

// score_ui를 지속적으로 갱신해야 함으로 이를 출력하는 함수를 만들었다.
void showScore(WINDOW *score_ui, int player_score, int ai_score, int level);

// word_box가 지속적으로 갱신됨으로 이를 위한 함수를 따로 만듬
void show_box(WINDOW *word_box, vector<string> word, vector<int> count);

// text_box를 출력하는 함수
void showTextBox(WINDOW *text_box);

// 게임 실행 함수
void play_game(int level);

// AI 함수
void AI(vector<int> &empty, vector<int> logic, WINDOW *word_box,
        WINDOW *text_box, WINDOW *score_box, vector<string> word, int level,
        int &ai_score, int &player_score);

void showTotalScore(int player_score, int ai_score);

#endif