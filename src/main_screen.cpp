#include <ncurses.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <cassert>
#include <stdlib.h>
#include "useful_funtion.h"
#include "line_typing_screen.h"
#include "main_screen.h"
#include "metal_dig.h"
#include "acidrain.h"

std::string MAIN_CHOICES[]= {
    "1.Line Typing Prcatice",
    "2.Transcription Practice",
    "3.Acid rain Game",
    "4.Mining Game",
    "5.Exit"
};

std::string TYPING[]={
R"!(  _________  __  __   ______   ________  ___   __    _______   )!",  
R"!(/________/\/_/\/_/\ /_____/\ /_______/\/__/\ /__/\ /______/\   )!", 
R"!(\__.::.__\/\ \ \ \ \\:::_ \ \\__.::._\/\::\_\\  \ \\::::__\/__ )!", 
R"!(   \::\ \   \:\_\ \ \\:(_) \ \  \::\ \  \:. `-\  \ \\:\ /____/\)!", 
R"!(    \::\ \   \::::_\/ \: ___\/  _\::\ \__\:. _    \ \\:\\_  _\/)!", 
R"!(     \::\ \    \::\ \  \ \ \   /__\::\__/\\. \`-\  \ \\:\_\ \ \)!", 
R"!(      \__\/     \__\/   \_\/   \________\/ \__\/ \__\/ \_____\/)!"  
};

std::string PRACTICE[]={
R"!( ______   ______    ________   ______  _________  ________  ______   ______      )!",
R"!(/_____/\ /_____/\  /_______/\ /_____/\/________/\/_______/\/_____/\ /_____/\     )!",
R"!(\:::_ \ \\:::_ \ \ \::: _  \ \\:::__\/\__.::.__\/\__.::._\/\:::__\/ \::::_\/_    )!",
R"!( \:(_) \ \\:(_) ) )_\::(_)  \ \\:\ \  __ \::\ \     \::\ \  \:\ \  __\:\/___/\   )!",
R"!(  \: ___\/ \: __ `\ \\:: __  \ \\:\ \/_/\ \::\ \    _\::\ \__\:\ \/_/\\::___\/_  )!",
R"!(   \ \ \    \ \ `\ \ \\:.\ \  \ \\:\_\ \ \ \::\ \  /__\::\__/\\:\_\ \ \\:\____/\ )!",
R"!(    \_\/     \_\/ \_\/ \__\/\__\/ \_____\/  \__\/  \________\/ \_____\/ \_____\/ )!"
};



/*-------------------- Funtion Definition --------------------*/


void print_main_menu(WINDOW *menu_win, int highlight){
    int x=7;
    int y=2;

    for(int i = 0; i<5; i++){
        if(highlight == i+1){
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y+(i*2), x, "%s", MAIN_CHOICES[i].c_str());
            wattroff(menu_win, A_REVERSE);
            y++;
        }
        else{
            mvwprintw(menu_win, y+(i*2), x, "%s", MAIN_CHOICES[i].c_str());
            y++;
        }
    }
    wrefresh(menu_win);
}


void print_main_screen(){
    int highlight=1;
    int key;
    
    initscr();
    clear();
    noecho();
    cbreak();

    WINDOW *win1 = newwin(50,150,0,0);
    WINDOW *menu = newwin(17,50,25,50);
    keypad(menu, true);

    
    refresh();

    if(has_colors() == FALSE){
        puts("Terminal does not support colors!");
        endwin();
        return;
    }else{
        start_color();
        init_pair(1,COLOR_BLACK, COLOR_WHITE);
        init_pair(2,COLOR_WHITE, COLOR_CYAN);
    }

    wbkgd(win1, COLOR_PAIR(2));
    wbkgd(menu,COLOR_PAIR(1));

    draw_ascii_art(win1,3,20,TYPING,7);
    draw_ascii_art(win1,13,35,PRACTICE,7);

    box(win1, '|', '-');
    box(menu, '|', '-');

    wrefresh(win1);
    
    print_main_menu(menu,highlight);
    
    while(true){
        key = wgetch(menu);
		switch(key)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = 5;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == 5)
					highlight = 1;
				else 
					++highlight;
				break;
            case '\n':
                switch(highlight){
                case 1:
                    endwin();
                    line_typing_screen();
                    break;
                case 3:
                    endwin();
                    acidrain();
                    break;     
                case 4:
                    endwin();
                    metal_dig();
                    break;
                case 5:
                
                    clrtoeol();
                    refresh();
                    endwin();
                    return;
                }
                break;
        }
        print_main_menu(menu,highlight);        
    }


    clrtoeol();
	refresh();
    endwin();

}
