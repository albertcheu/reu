#pragma once
#include "Board_AB.h"
#include "Evaluator.h"

class BoardEval: public Board_AB{
 protected:
  Evaluator& e;
  /*
  bool alphabeta_helper(size_t i, bool maximize, size_t depth,
                        int& max, int& min, int& loc,
                        int& alpha, int& beta);
  */
 public:
  BoardEval(size_t n, size_t k, Evaluator& e);

  scoreAndLoc alphabeta(bool maximize, int alpha, int beta,
			size_t depth, int x=-1);

  bool play(char c, int loc);


};
