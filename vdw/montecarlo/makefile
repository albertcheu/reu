all: Board.o main.cpp
	g++ main.cpp Board.o -o runMe
Board.o: Board.h Board.cpp
	g++ -O3 -c Board.cpp Board.h

clean:
	rm *.gch *.o runMe