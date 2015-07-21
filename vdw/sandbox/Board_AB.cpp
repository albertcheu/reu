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
				size_t depth, int x){
  recursionCount++;

  if (depth > 0){
    if (memberOfAP(x)){
      int sign = (maximize?1:-1);
      int result = ((!maximize)?R_WIN:B_WIN);
      return scoreAndLoc(sign * result, x);
    }
    if (depth == n) { return draw; }
  }

  int score = -10;
  int loc = -1;

  //symmetry -> don't bother checking rhs
  //bool s = symmetric();

  //Loop
  for(int i = ((n%2)?(n/2):((n/2) - 1)); i > -1; i--){
  //for(size_t i = 0; i < n; i++){
    //Check i

    if (alphabeta_helper(i, maximize, depth, score, loc, alpha, beta))
      { break; }

    //if (s) {continue;}
    int j = n-i-1;
    if (alphabeta_helper(j, maximize, depth, score, loc, alpha, beta))
      { break; }

  }
  

  return scoreAndLoc(score, loc);
}

bool Board_AB::alphabeta_helper(size_t i, bool maximize, size_t depth,
				int& score, int& loc,
				int& alpha, int& beta){
  if (i >= n || grid[i] != '.') { return false; }

  grid[i] = (maximize?'R':'B');
  /*
  //See if done before  
  Bitstring z = (maximize?assignmentZ[i].first:assignmentZ[i].second);
  zobrist ^= z;
  BitstringKey key = zobrist % MAXKEY;

  Bitstring g = (maximize?assignmentG[i].first:assignmentG[i].second);
  gamestate |= g;

  bool gotScore = false;

  if (table.find(key) != table.end()){
    size_t size = table[key].size();
    Bitstring* data = table[key].data();

    for(size_t j = 0; j < size; j++){
      Bitstring stored = data[j];
      Bitstring storedState = (stored << METADATA);
      inuse >>= METADATA;
      short metadata = (stored >> GAMESTATE);
      short storedScore = (metadata << 14);
      storedScore >>= 14;
      short storedFlag = (metadata << 12);
      storedScore >>= 14;
      short storedDepth = (metadata >> 4);

      if (inuse == gamestate) {

      }

    }
  }

  if (!gotScore) {

    score = alphabeta(!maximize, alpha, beta, depth+1, i).first;

    unsigned char storedScore = score+1;
    unsigned char storedDepth = depth;
    repDepth <<= 2;

    Bitstring storedVal = (repDepth | repScore);
    storedVal <<= INUSE;//make room for gamestate
    storedVal |= gamestate;


    if (table.find(key) == table.end()) { table[key] = Chain(); }
    table[key].push_back(storedVal);

  }
  */
  
  int curScore = -1 * alphabeta(!maximize, -beta, -alpha, depth+1, i).first;
  if (curScore > score) {
    loc = i;
    score = curScore;
    alpha = (alpha<score?score:alpha);
  }


  grid[i] = '.';
  //zobrist ^= z;
  //gamestate ^= g;

  return alpha >= beta;

}
