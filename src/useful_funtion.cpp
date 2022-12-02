#include <cassert>
#include <fcntl.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <random>
#include <algorithm>
#include "useful_funtion.h"

const int PERMS = 0644;
const int MAX_BUF_SIZE = 1024;


/*-------------------- Funtion Definition --------------------*/

//인자 설명 (출력하려는 화면, 아스키아트를 저장한 string 배열, y축, x측 ,
//아스키아트 라인 수 )

void new_draw_ascii_art(WINDOW *win, std::string txt, int y, int x, int height) {
    int fd = 0;
    std::string line = ""; //아스키아트 한줄을 저장할 변수

    char buf[1] = {'\0'};
    ssize_t rsize = 0;

    fd = open(txt.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("open() error!");
        exit(-1);
    }

    for (int i = 0; i < height; i++) {

        memset(buf, '\0', 1);

        // 10이란 숫자가 개행 문자를 나타냄
        // 개행 문자가 나올때까지 반복할 수 있게 된다.
        while (buf[0] != 10) {
            rsize = read(fd, buf, 1);

            if (rsize == -1) {
                perror("read() error!\n");
                exit(-2);
            }
            line += buf[0]; // line변수에 문자를 저장한다.
        }
        mvwprintw(win, y + i, x, "%s", line.c_str());

        lseek(fd, 1, SEEK_CUR); //개행문자를 넘어가기 위한 코드다.
        line = "";              // line 변수 초기화
    }

    close(fd);
}


//인자 설명 (출력하려는 화면, y축, x측 , 아스키아트를 저장한 string 배열, 배열의 크기)
void draw_ascii_art(WINDOW *win, int y, int x, std::string txt[], int arr_size){
    assert(x>=0 && y>=0);
    

    for(int i=0;i<arr_size;i++){
        mvwprintw(win,y+i,x,"%s\n",txt[i].c_str());
    }
}

std::vector<std::string> read_txt_line_by_line(std::string txt, int height){
    int fd = 0;
    std::vector<std::string> result;
    std::string temp;

    char buf[1]={'\0'};
    ssize_t rsize = 0;

    fd = open(txt.c_str(), O_RDONLY);
    if(fd==-1){
        perror("open() error!");
        exit(-1);
    }

    
    for(int i=0; i<height; i++){
        
        memset(buf,'\0',1);
        
        while(buf[0] != '<'){
            rsize = read(fd,buf,1);

            if(rsize == -1){
                perror("read() error!\n");
                exit(-2);
            }

            temp.push_back(buf[0]);

        }

        lseek(fd, 1, SEEK_CUR);
        temp.pop_back();
        result.push_back(temp);
        temp.clear();
    }

    close(fd);

    return result;
}
 
template<typename T>
void pop_front(std::vector<T> &v)
{
    if (v.size() > 0) {
        v.erase(v.begin());
    }
}

int get_random_number(int min, int max){
      // 시드값을 얻기 위한 random_device 생성.
    std::random_device rd;

    // random_device 를 통해 난수 생성 엔진을 초기화 한다.
    std::mt19937 gen(rd());

    // min 부터 max 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
    std::uniform_int_distribution<int> dis(min, max);


    return dis(gen);
}

std::vector<std::string> shrink_randomly(std::vector<std::string> target, int size){
    std::vector<std::string> result;

    for(int i=0; i<size; i++){
        result.push_back(target[get_random_number(0,target.size()-1)]);
    }    
    
    return result;
}
