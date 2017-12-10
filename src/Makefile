FLAGS=-std=c++11 -I. -ID:\SFML-2.4.2\include -LD:\SFML-2.4.2\lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

all: ponkanome.o

ponkanome.o: main.o metronome.o gui.o ui.o
	g++ main.o metronome.o gui.o ui.o $(FLAGS) -o ..\Ponkanome.exe
	
main.o: main.cpp
	g++ -c main.cpp $(FLAGS)
	
metronome.o: metronome.cpp
	g++ -c metronome.cpp $(FLAGS)
	
gui.o: gui.cpp
	g++ -c gui.cpp $(FLAGS)
	
ui.o: ui.cpp
	g++ -c ui.cpp $(FLAGS)