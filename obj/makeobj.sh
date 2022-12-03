#!/bin/bash

g++ -c ../src/useful_funtion.cpp -lncurses
g++ -c ../src/acidrain.cpp -lncurses -lpthread
g++ -c ../src/line_typing_screen.cpp -lncurses
g++ -c ../src/main.cpp
g++ -c ../src/main_screen.cpp -lncurses
g++ -c ../src/metal_dig.cpp -lncurses
g++ -c ../src/metal_dig_ingame.cpp -lncurses