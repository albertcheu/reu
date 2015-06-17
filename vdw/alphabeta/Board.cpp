//Made by Albert Cheu, 5/3/15
#include "Board.h"

Board::Board(size_t n, size_t k)
  :n(n), k(k)
{
  for(size_t i = 0; i < n; i++){
    grid.push_back('.');
    empties.push_back(i);
  }
  //Depth 0 is the first move, no other branches
  //so we put a dummy here
  killers.push_back(std::pair<size_t,size_t>(n,n));
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

size_t Board::numTurns(){ return n - empties.size(); }

bool Board::filled(){ return empties.size() == 0; }

void Board::print(){
  for(int i = 0; i < n; i++){ std::cout << grid[i]; }
  std::cout << std::endl;
}

bool Board::play(char c, int loc){
  if (grid[loc] != 'R' && grid[loc] != 'B'){
    grid[loc] = c;
    //Delete loc in empties, if it is in that vector
    std::vector<int>::iterator itr= std::lower_bound(empties.begin(),
						     empties.end(),
						     loc);
    if (*itr == loc) { empties.erase(itr); }
    return true;
  }

  return false;
}

scoreAndLoc Board::minimax(bool maximize){
  //Leaf
  if (won('R')) { return r_win; }
  if (won('B')) { return b_win; }
  if (numTurns() == n) { return draw; }

  //Internal node
  int max = -10;
  int min = 10;
  int loc = -1;

  for(int i = 0; i < n; i++){
    if (grid[i] != 'R' && grid[i] != 'B') {
      char old = grid[i];
      grid[i] = (maximize?'R':'B');
      scoreAndLoc p = minimax(!maximize);
      grid[i] = old;

      if (maximize && p.first > max) {
	max = p.first;
	loc = i;
      }

      else if (!maximize && p.first < min){
	min = p.first;
	loc = i;
      }

    }
  }

  return scoreAndLoc((maximize?max:min), loc);
}

bool Board::symmetric(){
  size_t middle = (n-1)/2;
  for (size_t i = 0; i < n; i++){
    if (i <= middle && grid[i] != grid[n-1-i]) { return false; }
  }
  return true;
}

scoreAndLoc Board::alphabeta(bool maximize, int alpha, int beta, size_t depth){
  if (won('R')) { return r_win; }
  if (won('B')) { return b_win; }
  if (depth == n) { return draw; }

  int max = -10;  int min = 10;  int loc = -1;

  //Add placeholder that will be changed later on
  if (depth == killers.size())
    { killers.push_back(std::pair<size_t,size_t>(n,n)); }

  //Do our (valid) killer moves for this depth result in a cutoff?
  size_t killer = killers[depth].first;
  if (killer != n &&
      alphabeta_helper(killer, maximize, depth, max, min, loc, alpha, beta))
    {  return scoreAndLoc((maximize?max:min), loc); }
  killer = killers[depth].second;
  if (killer != n &&
      alphabeta_helper(killer, maximize, depth, max, min, loc, alpha, beta))
    {  return scoreAndLoc((maximize?max:min), loc); }
  
  //Otherwise, look at all possible moves
  //symmetry -> don't bother checking rhs
  bool s = symmetric();

  //Loop
  int emptiesIndex = (empties.size()-1)/2;

  while(emptiesIndex > -1){
    int i = empties[emptiesIndex];

    //Check i
    if (!(depth == 0 && i==0) &&
	i != killers[depth].first && i != killers[depth].second &&
	alphabeta_helper(i, maximize, depth, max, min, loc, alpha, beta))
	//Store as a killer move
      {
	if (killers[depth].second != n)
	  { killers[depth].first = killers[depth].second; }
	killers[depth].second = i;
	break;
      }

    int se = empties.size()-emptiesIndex-1;
    int j = empties[se];
    emptiesIndex--;
    if (s) {continue;}
    //if not symmetrical, check "reflection" of i
    if (j != killers[depth].first && j != killers[depth].second &&
	alphabeta_helper(j, maximize, depth, max, min, loc, alpha, beta))
      //Store as a killer move
      {
	if (killers[depth].second != n)
	  { killers[depth].first = killers[depth].second; }
	killers[depth].second = j;
	break;
      }

  }

  return scoreAndLoc((maximize?max:min), loc);
}

bool Board::alphabeta_helper(size_t i, bool maximize, size_t depth,
			     int& max, int& min, int& loc,
			     int& alpha, int& beta){
  char old = grid[i];
  if (old == 'R' || old == 'B') { return false; }
  grid[i] = (maximize?'R':'B');
  scoreAndLoc p = (maximize
		   ? alphabeta(false, max, beta, depth+1)
		   : alphabeta(true, alpha, min, depth+1));
  grid[i] = old;

  if (maximize && p.first > max) {
    max = p.first;
    loc = i;
    alpha = (alpha>max?alpha:max);
  }

  else if (!maximize && p.first < min){
    min = p.first;
    loc = i;
    beta = (beta<min?beta:min);
  }  

  return alpha >= beta;

}
