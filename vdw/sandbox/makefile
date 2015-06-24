all: Board.o BoardMC.h BoardMC.cpp
	g++ -std=c++11 BoardMC.cpp BoardMC.h Board.o -o runMe

Board.o: Board.h Board.cpp
	g++ -O3 -c Board.cpp Board.h

clean:
	rm *.gch *.o runMe
