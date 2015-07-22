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
				 size_t depth, int x){
  recursionCount++;

  int score = -10;
  int loc = -1;
  int alphaOrig = alpha;
  if (retrieve(score, loc, alpha, beta)) { return scoreAndLoc(score,loc); }

  if (depth > 0){
    if (memberOfAP(x)){
      int sign = (maximize?1:-1);
      int result = ((!maximize)?R_WIN:B_WIN);
      return scoreAndLoc(sign * result, x);
    }
    if (depth == n) { return draw; }
  }

  RankingVector rv;
  e.evaluate(maximize, rv);

  //Loop
  for (size_t j = 0; j < rv.size(); j++){
    size_t i = rv[j].second;
    e.place(maximize, i);

    bool cutoff = alphabeta_helper(i, maximize, depth, score,
				   loc, alpha, beta);
    e.undo(maximize, i);
    if (cutoff) { break; }

  }

  store(score, loc, alphaOrig, beta);
    
  return scoreAndLoc(score, loc);
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
