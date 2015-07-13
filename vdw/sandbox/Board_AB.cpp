#include "Board_AB.h"

Board_AB::Board_AB(size_t n, size_t k,
		   unordered_map<BitstringKey,pair<Bitstring,int> >& table)
  :Board(n,k), table(table), recursionCount(0)
{}

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

    //Check i
    if (!(depth == 0 && i==0) &&
	alphabeta_helper(i, maximize, depth, max, min, loc, alpha, beta))
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

  Bitstring which = (maximize?assignments[i].first:assignments[i].second);
  gamestate ^= which;

  //See if done before
  BitstringKey key = gamestate % MAXKEY;
  bool gotScore = false;
  unordered_map<BitstringKey,pair<Bitstring,int> >::iterator itr = table.find(key);

  if (itr != table.end()) {
    pair<Bitstring,int> stateAndScore = itr->second;
  
    if (stateAndScore.first == gamestate) {
      score = stateAndScore.second;
      gotScore = true;
    }
    else {cout << "Collision" << endl; }
  }

  if (!gotScore){

    scoreAndLoc p = (maximize
		     ? alphabeta(false, max, beta, depth+1, i)
		     : alphabeta(true, alpha, min, depth+1, i));
    score = p.first;

    table[key] = {gamestate, score};
    // pair<Bitstring,int> entry = {gamestate, score};
    // table.emplace(key, entry);

  }

  //Alpha beta pruning
  if (maximize && score > max) {
    max = score;
    loc = i;
    alpha = (alpha>max?alpha:max);
  }

  else if (!maximize && score < min){
    min = score;
    loc = i;
    beta = (beta<min?beta:min);
  }  

  //Undo play
  grid[i] = '.';
  gamestate ^= which;

  return alpha >= beta;

}
