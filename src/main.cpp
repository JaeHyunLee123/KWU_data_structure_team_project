#include "main_screen.h"
#include "line_typing_screen.h"
#include "acidrain.h"
#include "metal_dig.h"
#include "transcript.h"

int main(){

    while(true){

    int choose = print_main_screen();

    switch (choose)
        {
        case 1:
            line_typing_screen(); //짧은 글 연습
            break;
        case 2:
            typing_screen();//긴 글 연습
            break;
        case 3:
            acidrain(); // 산성비
            break;
        case 4:
            metal_dig(); // 자원캐기
            break;
        case 5:
            return 0;
        }    

    }
    
    return 0;

}
