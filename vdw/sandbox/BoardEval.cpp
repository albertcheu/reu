#include "BoardEval.h"

BoardEval::BoardEval(size_t n, size_t k, Evaluator& e)
  :Board_AB(n,k), e(e)
{}

bool BoardEval::play(char c, int loc){
  bool ans = Board_AB::play(c,loc);
  if (ans) { e.place(c=='R',loc); }
  return ans;
}

scoreAndLoc BoardEval::alphabeta(bool maximize, int alpha, int beta,
					 size_t depth, int justPlayed){
  recursionCount++;
  if (depth > 0 && memberOfAP(justPlayed)) { return (maximize?b_win:r_win); }
  if (depth == n) { return draw; }

  int max = -10;  int min = 10;  int loc = -1;
  RankingVector rv;
  e.evaluate(maximize, rv);

  //Loop
  for (size_t x = 0; x < rv.size(); x++){
    size_t i = rv[x].second;
    e.place(maximize, i);

    bool cutoff = alphabeta_helper(i, maximize, depth, max, min,
				   loc, alpha, beta);
    e.undo(maximize, i);
    if (cutoff)
      { break; }

  }

  return scoreAndLoc((maximize?max:min), loc);
}
