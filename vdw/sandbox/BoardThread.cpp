#include "BoardThread.h"

BoardThread::BoardThread(size_t n, size_t k,
			 mutex& lock,size_t id, size_t numThreads,
			 vector<pair<Bitstring,Bitstring> >& assignmentX,
			 unordered_map<BitstringKey,pair<Bitstring,int> >& table)
  : Board_AB(n,k,table), lock(lock), id(id), numThreads(numThreads)
{
  assignments = assignmentX;
}

void BoardThread::fillTable(){
  alphabeta(true,-10,10,0);
}

scoreAndLoc BoardThread::alphabeta(bool maximize, int alpha, int beta,
				   size_t depth, int justPlayed){
  recursionCount++;
  if (depth > 0 && memberOfAP(justPlayed))
    { return ((grid[justPlayed]=='R')?r_win:b_win); }

  if (depth == n) { return draw; }
  
  int max = -10;  int min = 10;  int loc = -1;

  //symmetry -> don't bother checking rhs
  bool s = symmetric();

  //Loop
  for(int i = ((n%2)?(n/2):((n/2) - 1)); i > -1; i--){
    if (depth == 0 && i%numThreads != id) { continue; }

    //Check i
    if (!(depth == 0 && i==0) &&
	alphabeta_helper(i, maximize, depth, max, min, loc, alpha, beta))
      { break; }

    if (s) {continue;}
    int j = n-i-1;

    if (alphabeta_helper(j, maximize, depth, max, min, loc, alpha, beta))
      { break; }

  }

  return scoreAndLoc((maximize?max:min), loc);

}

bool BoardThread::alphabeta_helper(int i, bool maximize, size_t depth,
				   int& max, int& min, int& loc,
				   int& alpha, int& beta){
  if (i >= n || grid[i] != '.') { return false; }
  //size_t other = ((size_t)1)-id;

  //Play
  grid[i] = (maximize?'R':'B');
  int score = 0;

  while(! lock.try_lock()){}
  Bitstring which = (maximize?assignments[i].first:assignments[i].second);
  lock.unlock();

  gamestate ^= which;

  //See if done before
  BitstringKey key = gamestate % MAXKEY;
  bool gotScore = false;

  while(! lock.try_lock()){}
  unordered_map<BitstringKey,pair<Bitstring,int> >::iterator itr, end;
  itr = table.find(key);
  end = table.end();
  lock.unlock();

  if (itr != end) {
    pair<Bitstring,int> stateAndScore = itr->second;

    if (stateAndScore.first == gamestate) {
      score = stateAndScore.second;
      gotScore = true;
    }

    //else {cout << "Collision" << endl; }

  }

  if (!gotScore){

    scoreAndLoc p = (maximize
		     ? alphabeta(false, max, beta, depth+1, i)
		     : alphabeta(true, alpha, min, depth+1, i));
    score = p.first;

    while(! lock.try_lock()){}
    table[key] = {gamestate, score};
    lock.unlock();
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

  return alpha >= beta;
}
