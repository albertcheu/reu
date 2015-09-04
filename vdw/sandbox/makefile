all: main.cpp Board.o Board_AB.o BoardMC.o Evaluator.o MCEvaluator.o BoardEval.o
	g++ -std=c++11 main.cpp Board.o Board_AB.o BoardMC.o Evaluator.o MCEvaluator.o BoardEval.o -o runMe

Board.o: Board.h Board.cpp
	g++ -std=c++11 -O3 -c Board.cpp Board.h

Board_AB.o: Board.h Board_AB.h Board_AB.cpp
	g++ -std=c++11 -O3 -c Board.h Board_AB.cpp Board_AB.h

BoardMC.o: Board.h BoardMC.h BoardMC.cpp
	g++ -std=c++11 -O3 -c Board.h BoardMC.cpp BoardMC.h

Evaluator.o: Board.h Evaluator.h Evaluator.cpp
	g++ -std=c++11 -O3 -c Board.o Evaluator.h Evaluator.cpp

MCEvaluator.o: Evaluator.h MCEvaluator.h MCEvaluator.cpp
	g++ -std=c++11 -O3 -c Evaluator.h MCEvaluator.h MCEvaluator.cpp

BoardEval.o: Evaluator.h Board_AB.h BoardEval.h BoardEval.cpp
	g++ -std=c++11 -O3 -c Evaluator.h Board_AB.h BoardEval.h BoardEval.cpp

clean:
	rm *.gch *.o runMe
