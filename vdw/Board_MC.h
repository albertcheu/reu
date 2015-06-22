#pragma once
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

 public:
  Board_MC(size_t n, size_t k);
  //See if the player using char c has won
  //bool won(char c);
  //bool won_helper(char c, int start, int d);
  //bool filled();

  int montecarlo(bool redPlayer, int numTrials);
};
