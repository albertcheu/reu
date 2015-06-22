all: Board_MC.o Board_AB.o Board.o main.cpp
	g++ main.cpp Board.o Board_AB.o Board_MC.o -o runMe

Board.o: Board.h Board.cpp
	g++ -O3 -c Board.cpp Board.h

Board_AB.o: Board.h Board_AB.h Board_AB.cpp
	g++ -O3 -c Board.h Board_AB.cpp Board_AB.h

Board_MC.o: Board.h Board_MC.h Board_MC.cpp
	g++ -O3 -c Board.h Board_MC.cpp Board_MC.h

clean:
	rm *.gch *.o runMe
