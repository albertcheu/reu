all: main.cpp Board.o Board_AB.o
	g++ -std=c++11 main.cpp Board.o Board_AB.o -o runMe

Board.o: Board.h Board.cpp
	g++ -O3 -c Board.cpp Board.h

Board_AB.o: Board.h Board_AB.h Board_AB.cpp
	g++ -std=c++11 -O3 -c Board.h Board_AB.cpp Board_AB.h

clean:
	rm *.gch *.o runMe
