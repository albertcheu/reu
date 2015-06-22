#include "Board_MC.h"

Board_MC::Board_MC(size_t n, size_t k)
  :Board(n,k) {}
/*
bool Board::won_helper(char c, int start, int d){
  for(int j = 0; j < k; j++){
    if (grid[start+(j*d)] != c) { return false; }
  }
  return true;
}

bool Board::won(char c){
  for(int i = 0; i <= n-k; i++){
    int d = 1;
    while (true){
      int rightmost = i + (k-1)*d;
      if (rightmost >= n) { break; }

      if (won_helper(c, i, d)) { return true; }

      d++;
    }

    //If a k-AP doesnt fit with d=1, it can't work for any other d
    if (d == 1) { break; }

  }

  return false;
}
*/

int Board_MC::montecarlo(bool redPlayer, int numTrials){  
  int ans = empties[0];
  int mostWins = -1;
  int fewestLosses = numTrials;

  std::vector<int> indices;
  for(int e = 0; e < empties.size(); e++){
    indices.push_back(e);
  }

  for(int e = 0; e < empties.size(); e++){
    int i = empties[e];

    //Put character in
    grid[i] = (redPlayer?'R':'B');

    //Run trials, keeping count of wins/losses/draws
    float frac = sqrt((float)n*n - (n-empties.size())*(n-empties.size())) / n;
    //int nt = numTrials * empties.size() / n;
    int nt = (int)(numTrials * frac);
    Record r = runTrials(!redPlayer,
			 //numTrials,
			 (nt>4?nt:4),
			 indices, i);

    //Make empty again
    grid[i] = '.';

    //Update
    int numWins = (redPlayer?r.redWins:r.blueWins);
    int numLosses = (redPlayer?r.blueWins:r.redWins);
    /*
    if (numWins > mostWins) {
      ans = i;
      mostWins = numWins;
      //fewestLosses = (fewestLosses>numLosses?numLosses:fewestLosses);
    }
    */
    if (numLosses < fewestLosses) {
      ans = i;
      fewestLosses = numLosses;
    }
    
  }

  return ans;
}

Record Board_MC::runTrials(bool redPlayer, int numTrials,
			std::vector<int>& indices, int i){
  Record r = {0,0,0};

  for(int t = 0; t < numTrials; t++){
    bool red = redPlayer;

    //Shuffle indices
    std::random_shuffle(indices.begin(), indices.end());

    //Traverse empties by indices
    //Skip i == empties[index]
    bool rw, bw;
    rw = bw = false;
    for(int index = 0; index < indices.size(); index++){
      int j = indices[index];
      if (empties[j] == i) { continue; }
      grid[empties[j]] = (red?'R':'B');

      if (won('R')) { r.redWins++; rw = true; break;}
      else if (won('B')) { r.blueWins++; bw = true; break; }

      red = !red;
    }

    if (!rw && ! bw) { r.numDraws++; }

    //Make empty spots empty again
    for (int j = 0; j < empties.size(); j++){
      if (empties[j] == i) { continue; }
      grid[empties[j]] = '.';
    }

  }

  return r;
}
