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
  //cout << depth << endl;
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
    if (cutoff) { break; }

  }

  return scoreAndLoc((maximize?max:min), loc);
}

/*
bool BoardEval::alphabeta_helper(size_t i, bool maximize, size_t depth,
				int& max, int& min, int& loc,
				int& alpha, int& beta){
  if (i >= n || grid[i] != '.') { return false; }

  //Play
  grid[i] = (maximize?'R':'B');
  int score = 0;

  Bitstring g = (maximize?assignmentG[i].first:assignmentG[i].second);
  gamestate |= g;  

    score = alphabeta(!maximize, alpha, beta, depth+1, i).first;

  //Alpha beta pruning
  if (maximize) {
    if (score > max) {
      max = score;
      loc = i;
    }
    alpha = (alpha>max?alpha:max);
  }

  else {
    if (score < min){
      min = score;
      loc = i;
    }
    beta = (beta<min?beta:min);
  }  

  //Undo play
  grid[i] = '.';

  //zobrist ^= z;
  gamestate ^= g;

  return alpha >= beta;

}

*/
