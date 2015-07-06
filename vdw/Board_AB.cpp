#include "Board_AB.h"

Board_AB::Board_AB(size_t n, size_t k)
  :Board(n,k), gamestate(0), recursionCount(0)
{
  //Depth 0 is the first move, no other branches
  //so we put a dummy here
  killers.push_back(pair<size_t,size_t>(n,n));

  //Generates 64 random bits, which is what we need
  mt19937 generator((unsigned)time(NULL));
  for(size_t i = 0; i < n; i++){

    pair<Bitstring,Bitstring> p = {generator(),generator()};
    assignments.push_back(p);
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
  cout << "Ran thru " << recursionCount << " nodes" << endl;
  recursionCount = 0;
  Board::play(c,loc);
  //update gamestate;
  pair<Bitstring,Bitstring> p = assignments[loc];
  Bitstring which = (c=='R'?p.first:p.second);
  gamestate ^= which;
}

scoreAndLoc Board_AB::alphabeta(bool maximize, int alpha, int beta,
				size_t depth, int justPlayed){
  recursionCount++;
  if (justPlayed > -1 && memberOfAP(justPlayed))
    { return ((grid[justPlayed]=='R')?r_win:b_win); }

  if (depth == n) { return draw; }

  int max = -10;  int min = 10;  int loc = -1;

  //Add placeholder that will be changed later on
  if (depth == killers.size())
    { killers.push_back(pair<size_t,size_t>(n,n)); }

  //Do our (valid) killer moves for this depth result in a cutoff?
  size_t killer = killers[depth].first;
  if (killer != n && grid[killer] == '.' &&
      alphabeta_helper(killer, maximize, depth, max, min,
		       loc, alpha, beta))
      {  return scoreAndLoc((maximize?max:min), loc); }
  
  killer = killers[depth].second;
  if (killer != n && grid[killer] == '.' &&
      alphabeta_helper(killer, maximize, depth, max, min,
		       loc, alpha, beta))
    {  return scoreAndLoc((maximize?max:min), loc); }
  
  //Otherwise, look at all possible moves
  //symmetry -> don't bother checking rhs
  bool s = symmetric();

  //Loop
  for(int i = ((n%2)?(n/2):((n/2) - 1)); i > -1; i--){

    //Check i
    if (grid[i] =='.' && !(depth == 0 && i==0) &&
	i != killers[depth].first && i != killers[depth].second &&
	alphabeta_helper(i, maximize, depth, max, min, loc, alpha, beta)){
      //Store as a killer move
      if (killers[depth].second != n)
	{ killers[depth].first = killers[depth].second; }
      killers[depth].second = i;
      break;
    }
    

    if (s) {continue;}
    int j = n-i-1;
    if (grid[j] != '.') { continue; }

    if (j != killers[depth].first && j != killers[depth].second &&
	alphabeta_helper(j, maximize, depth, max, min, loc, alpha, beta)){
      if (killers[depth].second != n)
	{ killers[depth].first = killers[depth].second; }
      killers[depth].second = j;
      break;
      
    }
  }

  return scoreAndLoc((maximize?max:min), loc);
}

bool Board_AB::alphabeta_helper(size_t i, bool maximize, size_t depth,
				int& max, int& min, int& loc,
				int& alpha, int& beta){
  //Play
  grid[i] = (maximize?'R':'B');
  int score = 0;
  Bitstring which = (maximize?assignments[i].first:assignments[i].second);
  gamestate ^= which;

  //See if done before
  BitstringKey key = gamestate % MAXKEY;
  bool gotScore = false;
  if (table.find(key) != table.end()) {
    pair<vector<char>,int> stateAndScore = table[key];
    if (stateAndScore.first == grid) {
      score = stateAndScore.second;
      gotScore = true;
    }
    else {
      cout << "Collision" << endl;
    }
  }

  if (!gotScore){
    scoreAndLoc p = (maximize
		     ? alphabeta(false, max, beta, depth+1, i)
		     : alphabeta(true, alpha, min, depth+1, i));
    score = p.first;
    //table[key] = {grid, score};
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

  //If this is a great move, cache it
  if (alpha >= beta) { table[key] = {grid,score}; }

  //Undo play
  grid[i] = '.';
  gamestate ^= which;

  return alpha >= beta;

}
