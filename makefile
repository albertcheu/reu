all: Board.o main.cpp
	g++ main.cpp Board.o -o runMe
Board.o: Board.h Board.cpp
	g++ -c Board.cpp Board.h

clean:
	rm *.gch *.o runMe