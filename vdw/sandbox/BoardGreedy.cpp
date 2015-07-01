#include "BoardGreedy.h"

/*
bool BoardGreedy::contains(const Kap& kap, size_t y){
  size_t lmm = kap.lmm;
  if (y < lmm || y > n-1) {return false;}
  size_t d = kap.d;
  if ((y - lmm) % d) { return false; }
  return ((y - lmm) / d) > (k-1);
}
*/

BoardGreedy::BoardGreedy(size_t n, size_t k)
  :Board(n,k)
{
  for(size_t i = 0; i < n; i++){
    possibles.push_back(unordered_set<Kap>());
  }

  size_t maxD = (n-1)/(k-1);
  for(size_t i = 0; i < n; i++){
    for(size_t d = 1; d <= maxD; d++){

      if (i + d*(k-1) >= n) { continue; }

      Kap kap = {i, d};
      for (size_t j = 0; j < k; j++){
	possibles[i+j*d].emplace(kap);
      }

    }
  }
  
}

char BoardGreedy::whodWin(size_t loc, bool redPlayer){
    int d = 1;
  while (true){

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

size_t BoardGreedy::decide(bool redPlayer, size_t depth, int justPlayed){
  size_t ans = empties[0];

  bool foundWinner = false;
  bool foundBlocker = false;
  if (depth != 0) {
    char playerChar = (redPlayer?'R':'B');
    for(int e = 0; e < empties.size(); e++){
      size_t i = empties[e];
      char w = whodWin(i, redPlayer);

      //If current player can win by coloring i, do it
      if (w == playerChar){ ans = i; foundWinner = true; break; }

      //Otherwise, prevent any winning moves
      else if (w != '.') { ans = i; foundBlocker = true; }

    }
  }

  if (! foundWinner && !foundBlocker) {
    size_t best = 0;
    for(int e = 0; e < empties.size(); e++){
      size_t i = empties[e];
      size_t cur = possibles[i].size();
      if (cur > best) {
	best = cur;
	ans = i;
      }
    }
  }

  return ans;
}

bool BoardGreedy::play(char c, int loc){
  bool ans = Board::play(c,loc);

  //if valid and we just played as Blue, remove possible k-APs
  if (ans && c == 'B'){
    unordered_set<Kap> copy = possibles[loc];
    possibles[loc].clear();
    //Iterate thru copy
    for(unordered_set<Kap>::iterator itr = copy.begin();
	itr!= copy.end(); itr++){
      //Iterate thru possibles
      for (size_t i = 0; i < n; i++){
	if (i == loc) { continue; }
	//Delete instances
	possibles[i].erase(*itr);
      }
    }
  }

  return ans;
}
