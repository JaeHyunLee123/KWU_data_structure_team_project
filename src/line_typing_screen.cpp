#include <ncurses.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <queue>
#include <array>
#include <cassert>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>
#include <sys/time.h>
#include <iterator>
#include "useful_funtion.h"
#include "main_screen.h"
#include "line_typing_screen.h"

const int GRAGH_REACH = 100;

void line_typing_screen(){
    //strig을 저장하기 위한 변수들
    std::array<std::string,3> complete_string={"",};
    std::array<std::string,3> user_input_string={"",};
    std::string to_type_string="";

    int progression = -1;
    int total_line;

    double accuration=100;
    double wrong_words=0;
    double total_words=0;

    time_t start =0, end=0;
    double gap = 0;
    std::vector<double> typing_speed;
    double max_typing_speed=0;
    double average_typing_speed=0;
    double temp_typing_speed=0;

    initscr();
    clear();
    cbreak();

    WINDOW *back_ground = newwin(50,150,0,0); //배경 화면
    WINDOW *information_win = newwin(11,140,3,5); //정보 화면
    
    WINDOW *complete_win = newwin(15,140,15,5); //입력이 완료된 문자열 출력하는 화면
    WINDOW *typing_win = newwin(2,140,30,5); //입력 중인 문자열 출력하는 화면
    WINDOW *waiting_win = newwin(15,140,32,5); //입력 대기 중인 문자열 출력하는 화면


    keypad(typing_win, true); //특수문자 입력 허용
    curs_set(2); // 두꺼운 커서
    echo(); //유저가 입려하는 것을 화면에 보여줌

    refresh();

    if(has_colors() == FALSE){
        puts("Terminal does not support colors!");
        endwin();
        return;
    }else{
        start_color();
        init_pair(1,COLOR_BLACK, COLOR_WHITE);
        init_pair(2,COLOR_WHITE, COLOR_CYAN);
        init_pair(3,COLOR_BLUE, COLOR_WHITE);
        init_pair(4,COLOR_RED, COLOR_CYAN);
        init_pair(5,COLOR_RED, COLOR_WHITE);
        init_pair(6,COLOR_WHITE, COLOR_BLACK);
    }

    wbkgd(back_ground, COLOR_PAIR(2));
    wbkgd(information_win, COLOR_PAIR(1));
    wbkgd(complete_win, COLOR_PAIR(1));
    wbkgd(typing_win,COLOR_PAIR(2));
    wbkgd(waiting_win, COLOR_PAIR(1));

    mvwprintw(back_ground, 1, 64, "[LINE TYPING PRACTICE]");

    mvwprintw(information_win,1,5, "Progression : 0/10           [----------------------------------------------------------------------------------------------------]");
    mvwprintw(information_win,3,5, "Average type speed : 000     [----------------------------------------------------------------------------------------------------]");
    mvwprintw(information_win,5,5, "Max typing speed   : 000     [||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||]");
    mvwprintw(information_win,7,5, "Current typing speed  : 000  [----------------------------------------------------------------------------------------------------]");
    mvwprintw(information_win,9,5, "Accuration :  100.00         [||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||]");


    mvwprintw(typing_win,0,5, "Press Enter to start.");

    wrefresh(back_ground);
    wrefresh(information_win);

    wrefresh(complete_win);   
    wrefresh(typing_win);
    wrefresh(waiting_win);   

    //텍스트 파일  불러옴
    std::vector<std::string> txt = read_txt_line_by_line("idioms.txt",55);

    //10개로 랜덤으로 줄임
    txt = shrink_randomly(txt,10);

    total_line = txt.size();

    //받은 텍스트 줄 수 만큼 반복
    for(int i=0; i<=txt.size(); i++){

        //대기 중인 문자열 출력
        if(i<txt.size()){
            mvwprintw(waiting_win, 2, 5, "%s",txt[i].c_str());
        }else{
            mvwprintw(waiting_win, 2, 5, " ");
        }
        if(i+1<txt.size()) {
            mvwprintw(waiting_win, 7, 5, "%s",txt[i+1].c_str());
        }else{
            mvwprintw(waiting_win, 7, 5, " ");
        }
        if(i+2<txt.size()) {
            mvwprintw(waiting_win, 12, 5, "%s",txt[i+2].c_str());
        }else{
            mvwprintw(waiting_win, 12, 5, " ");
        }

        //입력해야하는 문자열 출력
        mvwprintw(typing_win,0,5,"%s",to_type_string.c_str());

        //완료한 문자열 출력
        wbkgd(complete_win,COLOR_PAIR(3));
        mvwprintw(complete_win,2,5,"%s",complete_string[2].c_str());
        mvwprintw(complete_win,7,5,"%s",complete_string[1].c_str());
        mvwprintw(complete_win,12,5,"%s",complete_string[0].c_str());

        
        //유저가 입력한 문자열 출력
        wattron(complete_win, COLOR_PAIR(1));
        mvwprintw(complete_win,3,5,"%s",user_input_string[2].c_str());
        mvwprintw(complete_win,8,5,"%s",user_input_string[1].c_str());
        mvwprintw(complete_win,13,5,"%s",user_input_string[0].c_str());
        wattroff(complete_win,COLOR_PAIR(1));

        //입력해야하는 문자열과 사용자가 입력한 문자열이 다르면 빨간색으로 표시
        for(int i=0; i<3; i++){
            for(int j=0; j<complete_string[i].length(); j++){
                if(complete_string[i][j] != user_input_string[i][j]){
                    wattron(complete_win,COLOR_PAIR(5));
                    mvwprintw(complete_win,12-i*5,5+j,"%c",complete_string[i][j]);
                    mvwprintw(complete_win,13-i*5,5+j,"%c",user_input_string[i][j]);
                    wattroff(complete_win,COLOR_PAIR(5));
                }
            }
        }

        wrefresh(complete_win);   
        wrefresh(waiting_win); 
        wrefresh(typing_win);

        std::string temp;

        wmove(typing_win,1,5);

        time(&start);
        temp = get_typing(typing_win);
        time(&end);


        //입력한 문자열 한줄씩 밀기
        complete_string[2] = complete_string[1];
        complete_string[1] = complete_string[0];
        complete_string[0] = to_type_string;
        //유저가 입력한 문자열 한줄 씩 밀기
        user_input_string[2] = user_input_string[1];
        user_input_string[1] = user_input_string[0];
        user_input_string[0] = temp; //위에서 입력받은 문자열을 첫번째 배열의 원소로 넣음

        //Progression 표시
        progression++;
        mvwprintw(information_win,1,5, "Progression : %d/%d",progression, total_line);

        //정확도 표시
        total_words += to_type_string.length();
        for(int i=0; i<to_type_string.length(); i++){
            if(to_type_string[i]!=temp[i]) wrong_words +=1;
        }        
        if(wrong_words>0) accuration = (1-(wrong_words/total_words))*100;
        mvwprintw(information_win,9,5, "Accuration :  %.2lf",accuration);

        //타수 계산
        gap = end - start;
        temp_typing_speed = 60*((double)temp.length()/gap); //방금 입력한 문장의 타수 계산
        if(temp_typing_speed>max_typing_speed) max_typing_speed = temp_typing_speed; //방금 입력한 문장의 타수가 기존의 타수보다 빠르면 교체
        if(progression!=0){
            typing_speed.push_back(temp_typing_speed); //방금 타수를 벡터에 저장해서
            average_typing_speed = get_average_ignore_zero(typing_speed); // 평균을 구함
        }
        //타수 출력
        mvwprintw(information_win,3,5, "Average type speed : %.0lf",average_typing_speed); //평균 타수
        mvwprintw(information_win,5,5, "Max typing speed   : %.0lf",max_typing_speed); // 최대 타수
        mvwprintw(information_win,7,5, "Current typing speed  : %.0lf",temp_typing_speed); // 현재 타수


        //그래프 출력
        print_graph(information_win,1,34,progression,total_line); //Progression
        print_graph(information_win,3,34,(int)average_typing_speed,(int)max_typing_speed); //평균타수
        print_graph(information_win,5,34,100,100); //최대 타수
        print_graph(information_win,7,34,(int)temp_typing_speed,(int)max_typing_speed); //현재 타수
        print_graph(information_win,9,34,(int)accuration,100); //정확도


        wrefresh(information_win);


        //입력해야하는 문자열 초기화
        to_type_string = txt[i];
        
        //반복문 시작전 화면을 비워줌
        wclear(complete_win);
        wclear(waiting_win);
        wclear(typing_win);
        wclear(information_win);
    }

    WINDOW *result_win = newwin(16,70,17,40);//결과 화면
    wbkgd(result_win,COLOR_PAIR(6));
    print_result(result_win,(int)average_typing_speed, (int)max_typing_speed,accuration);


    clrtoeol();
    refresh();
    endwin();
}

std::string get_typing(WINDOW* typing_win){
    char user_input[100]={'\0',};

    mvwgetnstr(typing_win,1,5,user_input,99);
     
    std::string result(user_input);

    return result;
}

double get_average_ignore_zero(std::vector<double> arr){
    if(arr.size() == 0) return 0;
    
    double result;
    int number_of_zero=0;
    std::vector<double>::iterator iter;
    for(iter = arr.begin(); iter!=arr.end(); iter++){
        result += *iter;
    }

    return result/(double)arr.size();
}

void print_graph(WINDOW* win, int y, int x, int current_status, int total){
    double current_status_percentage = ((double) current_status / (double) total)*100;

    mvwprintw(win,y,x,"[");
    for(int i=1; i<101; i++){
        if(i<=current_status_percentage){
            mvwprintw(win,y,x+i,"|");
        }else{
            mvwprintw(win,y,x+i,"-");
        }
    }
    mvwprintw(win,y,x+101,"]");

    wrefresh(win);

}

void print_result(WINDOW* result_win,int average_typing_speed,int max_typing_speed, double accuration){


    box(result_win,'|','-');

    mvwprintw(result_win, 3, 10,"<RESULT>");
    mvwprintw(result_win, 5, 10,"Average Typing Speed : %d",average_typing_speed);
    mvwprintw(result_win, 7, 10,"Max Typing Speed : %d",max_typing_speed);
    mvwprintw(result_win, 9, 10,"Accuration : %.2lf",accuration);   
    mvwprintw(result_win, 11, 10,"Press any key to go back to main screen.");

    wrefresh(result_win);

    getch();

    clrtoeol();
	refresh();
    endwin();

}
