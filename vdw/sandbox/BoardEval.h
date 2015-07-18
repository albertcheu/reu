#pragma once
#include "Board_AB.h"
#include "Evaluator.h"

class BoardEval: public Board_AB{
 protected:
  Evaluator& e;

 public:
  BoardEval(size_t n, size_t k, Evaluator& e);

  scoreAndLoc alphabeta(bool maximize, int alpha, int beta,
			size_t depth, int justPlayed=-1);

  bool play(char c, int loc);


};
