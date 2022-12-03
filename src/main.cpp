#include "main_screen.h"
#include "line_typing_screen.h"
//#include "acidrain.h"
#include "metal_dig.h"

int main(){

    while(true){

    int choose = print_main_screen();

    switch (choose)
        {
        case 1:
            line_typing_screen();
            break;
        case 2:
            break;
        case 3:
  //          acidrain();
        case 4:
            metal_dig();
            break;
        case 5:
            return 0;
        }    

    }
    
    return 0;

}
