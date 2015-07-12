#include "BoardGreedy.h"

BoardGreedy::BoardGreedy(size_t n, size_t k)
  :Board_AB(n,k)
{
  for(size_t i = 0; i < n; i++){
    possibleR.push_back(unordered_set<Kap,pair_hash>());
    possibleB.push_back(unordered_set<Kap,pair_hash>());
  }

  size_t maxD = (n-1)/(k-1);
  for(size_t i = 0; i < n; i++){
    for(size_t d = 1; d <= maxD; d++){

      if (i + d*(k-1) >= n) { continue; }

      Kap kap; kap.first=i; kap.second=d;
      for (size_t j = 0; j < k; j++){
	possibleR[i+j*d].emplace(kap);
	possibleB[i+j*d].emplace(kap);
      }

    }

  }

}

char BoardGreedy::whodWin(size_t loc, bool redPlayer){
  int d = 1;
  //cout << "Who'd win by playing on " << loc << "?" << endl;
  while (d <= (n-1)/(k-1)){

    if (loc+d >= n && loc-d < 0) { return '.'; }

    int numLeftR, numRightR, numLeftB, numRightB;
    numLeftR = numRightR = numLeftB = numRightB = 0;
    bool searchR, searchB;
    searchB = searchR = true;

    for(int i = loc+d; i < n; i += d){
      if (searchR && grid[i] == 'R') { numRightR++; }
      else { searchR = false; }
      if (searchB && grid[i] == 'B') { numRightB++; }
      else { searchB = false; }
      
      if (!searchR && !searchB) { break; }
    }
    
    if (redPlayer) {
      if (1+numRightR >= k) { return 'R'; }
      if (1+numRightB >= k) { return 'B'; }
    }
    else {
      if (1+numRightB >= k) { return 'B'; }
      if (1+numRightR >= k) { return 'R'; }
    }

    searchR = searchB = true;
    for(int i = loc-d; i > -1; i -= d){
      if (searchR && grid[i] == 'R') { numLeftR++; }
      else { searchR = false; }
      if (searchB && grid[i] == 'B') { numLeftB++; }
      else { searchB = false; }

      if (!searchR && !searchB) { break; }
    }

    if (redPlayer) {
      if (1+numLeftR+numRightR >= k) { return 'R'; }
      if (1+numLeftB+numRightB >= k) { return 'B'; }
    }
    else {
      if (1+numLeftB+numRightB >= k) { return 'B'; }
      if (1+numLeftR+numRightR >= k) { return 'R'; }
    }

    d++;
  }

  return '.';
}

bool pairCompare(const pair<size_t, size_t>& a, const pair<size_t, size_t>& b)
{ return a.first < b.first; }

scoreAndLoc BoardGreedy::alphabeta(bool maximize, int alpha, int beta,
				   size_t depth, int justPlayed){
  recursionCount++;
  if (justPlayed > -1 && memberOfAP(justPlayed))
    { return ((grid[justPlayed]=='R')?r_win:b_win); }

  if (depth == n) { return draw; }

  int max = -10;  int min = 10;  int loc = -1;

  size_t bestOption = 0;
  vector<pair<size_t,size_t> > preference;
  for (size_t i = 0; i < n; i++){
    vector<unordered_set<Kap,pair_hash> >& possibles = maximize?possibleR
      :possibleB;

    preference.push_back({possibles[i].size(), i});
    if (possibles[i].size() > bestOption) { bestOption = possibles[i].size(); }
  }

  if (bestOption > 0) {
    sort(preference.begin(), preference.end(), pairCompare);

    //Loop
    for (size_t i = 0; i < n; i++){
      size_t j = preference[i].second;
      if (!(depth == 0 && j==0) &&
	  alphabeta_helper(j, maximize, depth, max, min, loc, alpha, beta)){
	break;
      }    
    }
  }

  else{
    bool s = symmetric();
    for(int i = ((n%2)?(n/2):((n/2) - 1)); i > -1; i--){
      //Check i
      if (!(depth == 0 && i==0) &&
	  alphabeta_helper(i, maximize, depth, max, min, loc, alpha, beta)){
	break;
      }
      if (s) {continue;}
      int j = n-i-1;
      if (alphabeta_helper(j, maximize, depth, max, min, loc, alpha, beta)){
	break;
      }
    }
  }

  return scoreAndLoc((maximize?max:min), loc);
  
}

bool BoardGreedy::alphabeta_helper(size_t i, bool maximize, size_t depth,
				int& max, int& min, int& loc,
				int& alpha, int& beta){
  if (i >= n || grid[i] != '.') { return false; }

  //Play
  grid[i] = (maximize?'R':'B');
  unordered_set<Kap,pair_hash> copy = removeFromOpponent(maximize,i);
  int score = 0;

  Bitstring which = (maximize?assignments[i].first:assignments[i].second);
  gamestate ^= which;

  //See if done before
  BitstringKey key = gamestate % MAXKEY;
  bool gotScore = false;
  if (table.find(key) != table.end()) {
    pair<Bitstring,int> stateAndScore = table[key];
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
  restoreToOpponent(maximize, copy);

  return alpha >= beta;

}

bool BoardGreedy::play(char c, int loc){
  bool ans = Board_AB::play(c,loc);

  //if valid, remove possible k-APs from opponent
  if (ans){ removeFromOpponent(c=='R', loc); }

  return ans;
}

unordered_set<Kap,pair_hash> BoardGreedy::removeFromOpponent(bool maximize, int loc){
  vector<unordered_set<Kap,pair_hash> >& possibles = (maximize?
						      possibleB:possibleR);

  unordered_set<Kap,pair_hash> copy = possibles[loc];
  possibles[loc].clear();

  //Iterate thru copy
  for(unordered_set<Kap,pair_hash>::iterator itr = copy.begin();
      itr!= copy.end(); itr++){

    //Iterate thru possibles
    for (size_t i = 0; i < n; i++){
      if (i == loc) { continue; }
      //Delete instances
      if (possibles[i].find(*itr) != possibles[i].end()) 
	{ possibles[i].erase(*itr); }
    }
  }

  return copy;
}

void BoardGreedy::restoreToOpponent(bool maximize,
				    unordered_set<Kap,pair_hash>& kaps){
  vector<unordered_set<Kap,pair_hash> >& possibles = (maximize?
						      possibleB:possibleR);
  for(unordered_set<Kap,pair_hash>::iterator itr = kaps.begin();
      itr != kaps.end(); itr++){
    Kap kap = *itr;
    for (int elem = 0; elem < k; elem++){
      possibles[kap.first + elem*kap.second].emplace(kap);
    }
  }
}


size_t BoardGreedy::decide(bool redPlayer, size_t depth){
  size_t ans = 0;

  bool foundWinner = false;
  bool foundBlocker = false;
  if (depth != 0) {
    char playerChar = (redPlayer?'R':'B');
    for(size_t i = 0; i < n; i++){
      if (grid[i] != '.') { continue; }

      char w = whodWin(i, redPlayer);

      //If current player can win by coloring i, do it
      if (w == playerChar){
	cout << playerChar <<" can win by coloring " << i << endl;
	ans = i; foundWinner = true; break;
      }

      //Otherwise, prevent any winning moves
      else if (w != '.') {
	cout << playerChar << " can block by coloring " << i << endl;
	ans = i; foundBlocker = true;
      }

    }
  }

  if (foundWinner || foundBlocker) { return ans; }

  size_t best = 0;
  for(size_t i = 0; i < n; i++){
    if (grid[i] != '.') { continue; }

    size_t cur = (redPlayer?possibleR[i].size():possibleB[i].size());

    if (cur > best) {
      best = cur;
      ans = i;
    }
    //if (cur == best && i <= n/2) { ans = i; }
  }

  return ans;
}
