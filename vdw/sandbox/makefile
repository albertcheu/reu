all: main.cpp Board.o Board_AB.o
	g++ Board.o Board_AB.o  main.cpp -o runMe -pthread -Wl,--no-as-needed -std=c++11 

Board.o: Board.h Board.cpp
	g++ -std=c++11 -pthread -Wl,--no-as-needed -O3 -c Board.cpp Board.h

Board_AB.o: Board.h Board_AB.h Board_AB.cpp
	g++ -std=c++11 -O3 -c Board.h Board_AB.cpp Board_AB.h

clean:
	rm *.gch *.o runMe
