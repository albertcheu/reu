all: main.cpp Board.o RoundBoard.o Board_AB.o BoardMC.o
	g++ Board.o RoundBoard.o Board_AB.o BoardMC.o main.cpp -o runMe -std=c++11 

Board.o: Board.h Board.cpp
	g++ -std=c++11 -O3 -c Board.cpp Board.h

RoundBoard.o: Board.h RoundBoard.h RoundBoard.cpp
	g++ -std=c++11 -O3 -c Board.h RoundBoard.h RoundBoard.cpp

Board_AB.o: RoundBoard.h Board_AB.h Board_AB.cpp
	g++ -std=c++11 -O3 -c RoundBoard.h Board_AB.cpp Board_AB.h

BoardMC.o: RoundBoard.h BoardMC.h BoardMC.cpp
	g++ -std=c++11 -O3 -c RoundBoard.h BoardMC.cpp BoardMC.h


Evaluator.o: Board.h Evaluator.h Evaluator.cpp
	g++ -std=c++11 -c Board.h Evaluator.h Evaluator.cpp

GreedyEvaluator.o: Evaluator.h GreedyEvaluator.h GreedyEvaluator.cpp
	g++ -std=c++11 -O3 -c Evaluator.h GreedyEvaluator.h GreedyEvaluator.cpp

MCEvaluator.o: Evaluator.h MCEvaluator.h MCEvaluator.cpp
	g++ -std=c++11 -O3 -c Evaluator.h MCEvaluator.h MCEvaluator.cpp

RandomEvaluator.o: Evaluator.h RandomEvaluator.h RandomEvaluator.cpp
	g++ -std=c++11 -O3 -c Evaluator.h RandomEvaluator.h RandomEvaluator.cpp

BoardEval.o: Board_AB.h Evaluator.h BoardEval.h BoardEval.cpp
	g++ -std=c++11 -c Board_AB.h Evaluator.h BoardEval.h BoardEval.cpp

clean:
	rm *.gch *.o runMe
