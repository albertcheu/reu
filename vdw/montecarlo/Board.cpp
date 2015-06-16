//Made by Albert Cheu, 5/3/15
#include "Board.h"

Board::Board(size_t n, size_t k)
  :n(n), k(k)
{
  for(size_t i = 0; i < n; i++){ grid.push_back('.'); }
}

size_t Board::size(){return n;}

bool Board::won(char c){
  for(int i = 0; i <= n-k; i++){
    int d = 1;
    while (true){
      int rightmost = i + (k-1)*d;
      if (rightmost >= n) { break; }

      bool homogenous = true;
      for(int j = 0; j < k; j++){
	if (grid[i+(j*d)] != c) {
	  homogenous = false;
	  break;
	}
      }
      if (homogenous) { return true; }

      d++;
    }

    if (d == 1) { break; }

  }

  return false;
}

bool Board::noWinner(){ return !(won('R') || won('B')); }

size_t Board::numTurns(){
  size_t ans = 0;
  for(int i = 0; i < n; i++){
    if (grid[i] == 'R' || grid[i] == 'B') { ans++; }
  }
  return ans;
}

bool Board::filled(){
  for(int i = 0; i < n; i++){
    if (grid[i] != 'R' && grid[i] != 'B') { return false; }
  }
  return true;
}

void Board::print(){
  for(int i = 0; i < n; i++){ std::cout << grid[i]; }
  std::cout << std::endl;
}

bool Board::play(char c, int loc){
  if (grid[loc] != 'R' && grid[loc] != 'B'){
    grid[loc] = c;
    return true;
  }
  return false;
}

bool Board::symmetric(){
  size_t middle = (n-1)/2;
  for (size_t i = 0; i < n; i++){
    if (i <= middle && grid[i] != grid[n-1-i]) { return false; }
  }
  return true;
}

int Board::montecarlo(bool redPlayer, int numTrials){  
  int ans = -1;
  int mostWins = -1;
  int lastDraws = -1;

  std::vector<int> empties, indices;
  for(int i = 0; i < n; i++){
    if (grid[i] == '.') {
      empties.push_back(i);
      indices.push_back(indices.size());
      ans = i;
    }
  }

  for(int e = 0; e < empties.size(); e++){
    int i = empties[e];

    //Put character in
    grid[i] = (redPlayer?'R':'B');

    //Run <numTrials> trials, keeping count of wins/losses/draws
    Record r = runTrials(!redPlayer, numTrials, indices, empties, i);

    //Make empty again
    grid[i] = '.';

    //Update
    int numWins = (redPlayer?r.redWins:r.blueWins);
    if (numWins > mostWins || 
	(numWins == mostWins && r.numDraws > lastDraws)){
      ans = i;
      mostWins = numWins;
      lastDraws = r.numDraws;
    }

  }

  return ans;
}

Record Board::runTrials(bool redPlayer, int numTrials,
			std::vector<int>& indices,
			const std::vector<int>& empties, int i){
  Record r = {0,0,0};

  for(int t = 0; t < numTrials; t++){
    bool red = redPlayer;

    //Shuffle indices
    std::random_shuffle(indices.begin(), indices.end());

    //Traverse empties by indices
    //Skip i == empties[index]
    for(int index = 0; index < indices.size(); index++){
      int j = indices[index];
      if (empties[j] == i) { continue; }
      grid[empties[j]] = (red?'R':'B');
      red = !red;
    }

    //Update counts of won/lost/draw
    if (won('R')) { r.redWins++; }
    else if (won('B')) { r.blueWins++; }
    else { r.numDraws++; }
  }

  //Make empty spots empty again
  for (int j = 0; j < empties.size(); j++){
    if (empties[j] == i) { continue; }
    grid[empties[j]] = '.';
  }


  return r;
}
