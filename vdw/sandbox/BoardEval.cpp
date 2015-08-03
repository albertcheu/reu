#include "BoardEval.h"

BoardEval::BoardEval(num n, num k, Evaluator& e)
  :Board_AB(n,k), e(e)
{}

bool BoardEval::play(char c, num loc){
  bool ans = Board_AB::play(c,loc);
  if (ans) { e.place(c=='R',loc); }
  return ans;
}

scoreAndLoc BoardEval::alphabeta(bool maximize, char alpha, char beta,
				 num depth, num x){
  //if (recursionCount % 10000 == 0) { cout << recursionCount << endl; }
  recursionCount++;

  char score = -10;
  num loc = n;
  char alphaOrig = alpha;

  if (depth >= 2*k-1){
    if (memberOfAP(x)){
      char sign = (maximize?1:-1);
      char result = sign * ((!maximize)?R_WIN:B_WIN);
      return scoreAndLoc(result, x);
    }
    if (depth == n) { return draw; }
  }

  if (retrieveSmart(score, loc, alpha, beta)) { return scoreAndLoc(score,loc); }

  RankingVector rv;
  e.evaluate(maximize, rv);

  //Loop
  bool firstChild = true;
  for (size_t j = 0; j < rv.size(); j++){
    num i = rv[j].second;
    if (loc == n) { loc = i; }

    e.place(maximize, i);
    bool cutoff = alphabeta_helper(i, maximize, depth, score,
				   loc, alpha, beta,firstChild);
    e.undo(maximize, i);
    if (cutoff) { break; }

  }

  storeSmart(score, loc, alphaOrig, beta);

  if (depth == 0) { cout << recursionCount << endl; }

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
