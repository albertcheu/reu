//#pragma once
#include <cmath>
#include <algorithm>
#include "Board.h"

#define MC_TRIALS 3000

struct Record{
  int redWins,blueWins,numDraws;
} ;

class Board_MC: public Board{
 private:

  Record runTrials(bool redPlayer, int numTrials,
		   std::vector<int>& indices,
		   int i);
  char memberOfAP(int loc, bool redPlayer);
  bool memberOfAP_played(int loc);
 public:
  Board_MC(size_t n, size_t k);

  int montecarlo(bool redPlayer, int numTrials);
};
