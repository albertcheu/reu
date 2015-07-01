#include "Board_AB.h"

Board_AB::Board_AB(size_t n, size_t k)
  :Board(n,k)
{
  //Depth 0 is the first move, no other branches
  //so we put a dummy here
  killers.push_back(pair<size_t,size_t>(n,n));
}

bool Board_AB::symmetric(){
  size_t middle = (n-1)/2;
  for (size_t i = 0; i < n; i++){
    if (i <= middle && grid[i] != grid[n-1-i]) { return false; }
  }
  return true;
}

scoreAndLoc Board_AB::alphabeta(bool maximize, int alpha, int beta,
				size_t depth, int justPlayed){
  if (depth == n) { return draw; }
  if (justPlayed > -1 && memberOfAP(justPlayed))
    { return ((grid[justPlayed]=='R')?r_win:b_win); }

  int max = -10;  int min = 10;  int loc = -1;

  //Add placeholder that will be changed later on
  if (depth == killers.size())
    { killers.push_back(pair<size_t,size_t>(n,n)); }

  //Do our (valid) killer moves for this depth result in a cutoff?
  size_t killer = killers[depth].first;
  if (killer != n &&
      alphabeta_helper(killer, maximize, depth, max, min,
		       loc, alpha, beta))
    {  return scoreAndLoc((maximize?max:min), loc); }
  killer = killers[depth].second;
  if (killer != n &&
      alphabeta_helper(killer, maximize, depth, max, min,
		       loc, alpha, beta))
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
	alphabeta_helper(i, maximize, depth, max, min,
			 loc, alpha, beta))
	
      {
	//Store as a killer move
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
	alphabeta_helper(j, maximize, depth, max, min,
			 loc, alpha, beta))
      {
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
  char old = grid[i];
  if (old == 'R' || old == 'B') { return false; }
  grid[i] = (maximize?'R':'B');
  scoreAndLoc p = (maximize
		   ? alphabeta(false, max, beta, depth+1, i)
		   : alphabeta(true, alpha, min, depth+1, i));
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
