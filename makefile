all: build run

build:
	cd /home/benjaminemde/DEV/Horst/build && clang++ -g -W -Wall -Wextra -O0 -std=c++17 -c  ../src/main.cpp  
	cd /home/benjaminemde/DEV/Horst/build && clang++  -o Horst *.o   
run:
	cd /home/benjaminemde/DEV/Horst/build && ./Horst
.PHONY: all build run