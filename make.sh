#!/bin/bash

#분할 컴파일
g++ -c src/useful_funtion.cpp -lncurses
g++ -c src/acidrain.cpp -lncurses -lpthread
g++ -c src/line_typing_screen.cpp -lncurses
g++ -c src/main.cpp
g++ -c src/main_screen.cpp -lncurses
g++ -c src/metal_dig.cpp -lncurses
g++ -c src/metal_dig_ingame.cpp -lncurses
g++ -c src/transcript.cpp -lncurses

#오브젝트 파일을 obj 폴더로 이동
mv *.o obj/


# 'TypingPractice'라는 이름의 프로그램 생성
g++ obj/acidrain.o obj/line_typing_screen.o obj/main.o obj/main_screen.o obj/metal_dig.o obj/metal_dig_ingame.o obj/useful_funtion.o obj/transcript.o -o TypingPractice -lncurses
