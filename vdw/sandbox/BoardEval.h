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
  BoardEval(num n, num k, Evaluator& e);

  scoreAndLoc alphabeta(bool maximize, char alpha, char beta,
			num depth, num x=255);

  bool play(char c, num loc);

};
