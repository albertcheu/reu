#include "BoardEval.h"

BoardEval::BoardEval(size_t n, size_t k, Evaluator& e)
  :Board_AB(n,k), e(e)
{}

bool BoardEval::play(char c, int loc){
  bool ans = Board_AB::play(c,loc);
  if (ans) { cout << c << endl; e.place(c=='R',loc); }
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

    if (alphabeta_helper(i, maximize, depth, max, min,
			 loc, alpha, beta))
      { break; }

  }

  return scoreAndLoc((maximize?max:min), loc);
}

bool BoardEval::alphabeta_helper(size_t i, bool maximize, size_t depth,
				int& max, int& min, int& loc,
				int& alpha, int& beta){
  //if (i >= n || grid[i] != '.') { cout << "Bad!" << endl; return false; }

  //Play
  grid[i] = (maximize?'R':'B');
  e.place(maximize, i);

  int score = 0;

  //See if done before  
  Bitstring z = (maximize?assignmentZ[i].first:assignmentZ[i].second);
  zobrist ^= z;
  BitstringKey key = zobrist % MAXKEY;

  Bitstring g = (maximize?assignmentG[i].first:assignmentG[i].second);
  gamestate |= g;  

  /*
  bool gotScore = false;
  bool overwrite = true;
  if (table.find(key) != table.end()){
    Bitstring stored = table[key];
    Bitstring inuse = (stored << REM);
    inuse >>= REM;
    unsigned char rem = (stored >> INUSE);
    unsigned char storedScore = (rem << 6);
    storedScore >>= 6;
    unsigned char storedDepth = (rem >> 2);

    if (inuse == gamestate) {
      gotScore = true;
      score = storedScore;
      score--;//2,1,0 becomes 1,0,-1
    }
    else if (storedDepth <= depth) { overwrite = false; }

  }
  if (!gotScore) {
  */

    score = alphabeta(!maximize,alpha, beta, depth+1, i).first;
    /*
    if (overwrite) {
      unsigned char repScore = score+1;
      unsigned char repDepth = depth;
      repDepth <<= 2;
      Bitstring storedVal = (repDepth | repScore);
      storedVal <<= INUSE;
      storedVal |= gamestate;
      table[key] = storedVal;
    }
  }
    */
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
  zobrist ^= z;
  gamestate ^= g;
  e.undo(maximize, i);
    
  return alpha >= beta;

}
