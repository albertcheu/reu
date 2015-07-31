all: main.cpp Board.o Board_AB.o BoardMC.o Evaluator.o GreedyEvaluator.o
	g++ -std=c++11 -pthread -Wl,--no-as-needed main.cpp Board.o Board_AB.o BoardMC.o Evaluator.o GreedyEvaluator.o -o runMe

Board.o: Board.h Board.cpp
	g++ -std=c++11 -pthread -Wl,--no-as-needed -O3 -c Board.cpp Board.h

Board_AB.o: Board.h Board_AB.h Board_AB.cpp
	g++ -std=c++11 -O3 -c Board.h Board_AB.cpp Board_AB.h

BoardMC.o: Board.h BoardMC.h BoardMC.cpp
	g++ -std=c++11 -O3 -c Board.h BoardMC.cpp BoardMC.h

Evaluator.o: Board.h Evaluator.h Evaluator.cpp
	g++ -std=c++11 -c Board.h Evaluator.h Evaluator.cpp

GreedyEvaluator.o: Evaluator.h GreedyEvaluator.h GreedyEvaluator.cpp
	g++ -std=c++11 -O3 -c Evaluator.h GreedyEvaluator.h GreedyEvaluator.cpp

clean:
	rm *.gch *.o runMe
