#include "Board_MC.h"

Board_MC::Board_MC(size_t n, size_t k)
  :Board(n,k) {}

int Board_MC::montecarlo(bool redPlayer, int numTrials){  
  int ans = empties[0];
  float bestScore = -1.0f;

  //Because we want to traverse empties in random order,
  //make a vector of indices that will be shuffled
  std::vector<int> indices;
  for(int e = 0; e < empties.size(); e++){ indices.push_back(e); }

  //The number of trials should decrease as the number of available moves does
  float frac = sqrt((float)n*n - (n-empties.size())*(n-empties.size())) / n;
  int nt = (int)(numTrials * frac);
  nt = (nt>1?nt:1);

  char playerChar = (redPlayer?'R':'B');
  bool foundStopPoint = false;
  for(int e = 0; e < empties.size(); e++){
    int i = empties[e];

    char w = memberOfAP(i, redPlayer);
    //If current player can win by coloring i, do it
    if (w == playerChar){ return i; }
    //Otherwise, prevent any winning moves
    else if (w != '.') { ans = i; foundStopPoint = true; }
    if (foundStopPoint) { continue; }

    //Put character in
    grid[i] = playerChar;

    Record r = runTrials(!redPlayer,
			 nt,
			 indices, i);

    //Make empty again
    grid[i] = '.';

    //Update
    int numWins = (redPlayer?r.redWins:r.blueWins);
    int numLosses = (redPlayer?r.blueWins:r.redWins);
    float score = numWins + (r.numDraws / 2.0f) - (numLosses / 3.0f);

    if (score > bestScore) {
      bestScore = score;
      ans = i;
    }

  }

  return ans;
}

Record Board_MC::runTrials(bool redPlayer, int numTrials,
			std::vector<int>& indices, int i){
  Record r = {0,0,0};

  for(int t = 0; t < numTrials; t++){
    //Did red or blue win a particular trial-run?
    bool rw, bw;
    rw = bw = false;

    //In a trial-run, is it the red player's turn?
    bool red = redPlayer;

    //Shuffle indices
    std::random_shuffle(indices.begin(), indices.end());

    //Traverse empties by indices
    for(int index = 0; index < indices.size(); index++){
      int j = indices[index];

      //Skip i, which is the played position that kicked off the trials
      if (empties[j] == i) { continue; }

      //Current player colors empty spot indicated by empties[j]
      char player = (red?'R':'B');
      grid[empties[j]] = player;

      //If the current player has won, keep track
      if (memberOfAP_played(empties[j])){
	if (player == 'R') { r.redWins++; rw = true; break; }
	else { r.blueWins++; bw = true; break; }
      }

      //Otherwise, alternate player
      red = !red;
    }

    //After a game has completed with no winner, it's a draw
    if (!rw && ! bw) { r.numDraws++; }

    //Make empty spots empty again, for next trial-run
    for (int j = 0; j < empties.size(); j++){
      if (empties[j] == i) { continue; }
      grid[empties[j]] = '.';
    }

  }

  return r;
}

//Check if the player who played on loc has won
bool Board_MC::memberOfAP_played(int loc){
  int d = 1;
  while (true){

    if (loc+d >= n && loc-d < 0) { return false; }

    int numLeft, numRight;
    numLeft = numRight = 0;
    bool search = true;

    for(int i = loc+d; i < n; i += d){
      if (search && grid[i] == grid[loc]) { numRight++; }
      else { break; }
    }
    if (1+numRight >= k) { return true; }

    search = true;
    for(int i = loc-d; i > -1; i -= d){
      if (search && grid[i] == grid[loc]) { numLeft++; }
      else { break; }
    }
    if (numLeft + numRight + 1 >= k) { return true; }

    d++;
  }

  return false;
}

//Check which player (if any) can win by playing on loc
//If the current player can win, return its char token
//If its enemy can win, return the enemy char
//If nobody can win, return '.'
char Board_MC::memberOfAP(int loc, bool redPlayer){
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
