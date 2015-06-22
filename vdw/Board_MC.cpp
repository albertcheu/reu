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
  //int mostWins = -1;
  //int fewestLosses = numTrials;
  //int biggestDiff = -1;
  float bestScore = -1.0f;

  std::vector<int> indices;
  for(int e = 0; e < empties.size(); e++){ indices.push_back(e); }

  float frac = sqrt((float)n*n - (n-empties.size())*(n-empties.size())) / n;
  int nt = (int)(numTrials * frac);
  nt = (nt>1?nt:1);

  for(int e = 0; e < empties.size(); e++){
    int i = empties[e];

    //Put character in
    grid[i] = (redPlayer?'R':'B');

    //Run trials
    Record r = runTrials(!redPlayer,
			 //numTrials,
			 nt,
			 indices, i);

    //Make empty again
    grid[i] = '.';

    //Update
    int numWins = (redPlayer?r.redWins:r.blueWins);
    int numLosses = (redPlayer?r.blueWins:r.redWins);
    float score = numWins + (r.numDraws / 2.0f) - (numLosses / 4.0f);

    if (score > bestScore) {
      bestScore = score;
      ans = i;
    }

    /*
    int diff = numWins - numLosses;
    if (diff > biggestDiff) {
      ans = i;
      biggestDiff = diff;
    }
    */

    /*    
    if (numWins >= mostWins && numLosses < fewestLosses) {
      ans = i;
      mostWins = numWins;
      fewestLosses = numLosses;
    }
    */    

    /*
    if (numLosses < fewestLosses) {
      ans = i;
      fewestLosses = numLosses;
    }
    */
  }

  return ans;
}

Record Board_MC::runTrials(bool redPlayer, int numTrials,
			std::vector<int>& indices, int i){
  Record r = {0,0,0};

  //std::cout << empties.size() << ' ' << indices.size() << std::endl;

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

      char w = winner();

      if (w == 'R') { r.redWins++; rw = true; break;}
      else if (w == 'B') { r.blueWins++; bw = true; break; }

      red = !red;
    }

    if (!rw && ! bw) { r.numDraws++; }

    //Make empty spots empty again
    for (int j = 0; j < empties.size(); j++){
      if (empties[j] == i) { continue; }
      grid[empties[j]] = '.';
    }

  }

  //std::cout << r.redWins << ' ' << r.blueWins << std::endl;
  return r;
}
