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

size_t BoardGreedy::decide(bool redPlayer, size_t depth){
  //if (depth == 0) { return n / 2; }

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

  if (! foundWinner && !foundBlocker) {
    size_t best = 0;
    for(size_t i = 0; i < n; i++){
      if (grid[i] != '.') { continue; }

      size_t cur = (redPlayer?possibleR.size():possibleB.size());
      //possibles[i].size();

      if (cur > best) {
	best = cur;
	ans = i;
      }
      if (cur == best && i <= n/2) { ans = i; }
    }

  }

  return ans;
}

bool BoardGreedy::play(char c, int loc){
  cout << "Playing " << c << " on " << loc << endl;
  bool ans = Board_AB::play(c,loc);

  //if valid, remove possible k-APs from opponent
  if (ans){
    vector<unordered_set<Kap,pair_hash> >& possibles = (c=='R'?
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
	possibles[i].erase(*itr);
      }
    }
  }

  //if (winner() != '.') { cout << "Somebody won" << endl; }

  return ans;
}
