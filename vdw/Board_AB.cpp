#include "Board_AB.h"

Board_AB::Board_AB(size_t n, size_t k)
  :Board(n,k), gamestate(0), zobrist(0), recursionCount(0)
{
  mt19937 generator((unsigned)time(NULL));
  for(size_t i = 0; i < n; i++){
    pair<Bitstring,Bitstring> p = {((Bitstring)1)<<(2*i + 1),
				   ((Bitstring)1)<<(2*i)};
    assignmentG.push_back(p);

    p = {generator(),generator()};
    assignmentZ.push_back(p);
  }
}

bool Board_AB::symmetric(){
  size_t middle = (n-1)/2;
  for (size_t i = 0; i < n; i++){
    if (i <= middle && grid[i] != grid[n-1-i]) { return false; }
  }
  return true;
}

bool Board_AB::play(char c, int loc){
  if (gamestate == 0) cout << "Ran thru " << recursionCount << " nodes" << endl;
  recursionCount = 0;
  Board::play(c,loc);
  zobrist ^= (c=='R'?assignmentZ[loc].first:assignmentZ[loc].second);
  gamestate |= (c=='R'?assignmentG[loc].first:assignmentG[loc].second);

}

scoreAndLoc Board_AB::alphabeta(bool maximize, int alpha, int beta,
				size_t depth, int justPlayed){
  recursionCount++;
  if (depth > 0 && memberOfAP(justPlayed))
    { return ((grid[justPlayed]=='R')?r_win:b_win); }

  if (depth == n) { return draw; }

  int max = -10;  int min = 10;  int loc = -1;

  //symmetry -> don't bother checking rhs
  bool s = symmetric();

  //Loop
  for(int i = ((n%2)?(n/2):((n/2) - 1)); i > -1; i--){
  //for(size_t i = 0; i < n; i++){
    //Check i
    if (alphabeta_helper(i, maximize, depth, max, min, loc, alpha, beta))
      { break; }


    if (s) {continue;}
    int j = n-i-1;
    if (alphabeta_helper(j, maximize, depth, max, min, loc, alpha, beta))
      { break; }

  }

  return scoreAndLoc((maximize?max:min), loc);
}

bool Board_AB::alphabeta_helper(size_t i, bool maximize, size_t depth,
				int& max, int& min, int& loc,
				int& alpha, int& beta){
  if (i >= n || grid[i] != '.') { return false; }

  //Play
  grid[i] = (maximize?'R':'B');
  int score = 0;

  //See if done before  
  Bitstring z = (maximize?assignmentZ[i].first:assignmentZ[i].second);
  zobrist ^= z;
  BitstringKey key = zobrist % MAXKEY;

  Bitstring g = (maximize?assignmentG[i].first:assignmentG[i].second);
  gamestate |= g;  
  //BitstringKey key = gamestate % MAXKEY;

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

    score = alphabeta(!maximize, alpha, beta, depth+1, i).first;
    if (overwrite) {
      unsigned char repScore = score+1;
      unsigned char repDepth = depth;
      repDepth <<= 2;
      Bitstring storedVal = (repDepth | repScore);
      storedVal <<= INUSE;//make room for gamestate
      storedVal |= gamestate;
      table[key] = storedVal;
    }
  }

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

  return alpha >= beta;

}
