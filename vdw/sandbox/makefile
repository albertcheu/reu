all: main.cpp Board.o Board_AB.o
	g++ Board.o Board_AB.o main.cpp -o runMe -std=c++11 

Board.o: Board.h Board.cpp
	g++ -std=c++11 -O3 -c Board.cpp Board.h

Board_AB.o: Board.h Board_AB.h Board_AB.cpp
	g++ -std=c++11 -O3 -c Board.h Board_AB.cpp Board_AB.h

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
