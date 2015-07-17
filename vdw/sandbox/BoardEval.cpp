#include "BoardEval.h"

BoardEval::BoardEval(size_t n, size_t k, Evaluator& e)
  :Board_AB(n,k), e(e)
{}

bool BoardEval::play(char c, int loc){
  Board_AB::play(c,loc);
  e.play(c,loc);
}

scoreAndLoc BoardEval::alphabeta(bool maximize, int alpha, int beta,
					 size_t depth, int justPlayed){
  recursionCount++;
  if (depth > 0 && memberOfAP(justPlayed))
    { return ((grid[justPlayed]=='R')?r_win:b_win); }

  if (depth == n) { return draw; }

  int max = -10;  int min = 10;  int loc = -1;

  e.evaluate();

  //Loop
  /*
  size_t index = 0;
  while(index < n){

    pair<float,size_t> p = rankedPositions[index++];
    size_t i = p.second;
    if (i == n) { break; }
    float rank = p.first;

    if (alphabeta_helper(i, maximize, depth, max, min, loc, alpha, beta))
      { break; }
  }
  */

  return scoreAndLoc((maximize?max:min), loc);
}
