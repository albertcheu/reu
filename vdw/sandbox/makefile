all: main.cpp Board.o BoardMC.o
	g++ main.cpp Board.o BoardMC.o -o runMe

Board.o: Board.h Board.cpp
	g++ -O3 -c Board.cpp Board.h

BoardMC.o: Board.h BoardMC.h BoardMC.cpp
	g++ -O3 -c Board.h BoardMC.cpp BoardMC.h

clean:
	rm *.gch *.o runMe
