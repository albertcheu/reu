#include "Board_AB.h"

Board_AB::Board_AB(size_t n, size_t k)
  :Board(n,k), gamestate(0), zobrist(0), recursionCount(0)
{
  assignmentG.clear();
  assignmentZ.clear();

  mt19937 generator((unsigned)time(NULL));
  for(size_t i = 0; i < n; i++){
    pair<Bitstring,Bitstring> p = {((Bitstring)1)<<(2*i + 1),
				   ((Bitstring)1)<<(2*i)};
    assignmentG.push_back(p);

    p = {generator(),generator()};
    assignmentZ.push_back(p);

    //killers.push_back({n,n});
  }
}

bool Board_AB::symmetric(){
  size_t middle = (n-1)/2;
  for (size_t i = 0; i < n; i++){
    if (i <= middle && grid[i] != grid[n-1-i]) { return false; }
  }
  return true;
}

bool Board_AB::play(char c, char loc){
  if (gamestate == 0) cout << "Ran thru " << recursionCount << " nodes" << endl;
  recursionCount = 0;
  Board::play(c,loc);
  zobrist ^= (c=='R'?assignmentZ[loc].first:assignmentZ[loc].second);
  gamestate |= (c=='R'?assignmentG[loc].first:assignmentG[loc].second);

}

bool Board_AB::retrieve(BitstringKey key, Bitstring gs,
			int& score, int& loc, int& alpha, int& beta){

  if (table.find(key) == table.end()){ return false; }

  size_t size = table[key].size();
  Bitstring* data = table[key].data();

  for(size_t j = 0; j < size; j++){
    Bitstring stored = data[j];
    Bitstring storedState = (stored << METADATA);
    storedState >>= METADATA;
    short metadata = (stored >> GAMESTATE);
    char storedScore = (metadata % 4);
    storedScore--;
    char storedFlag = (metadata % 16);
    storedFlag >>= 2;
    short storedLoc = (metadata >> 4);

    if (storedState == gs) {
      
      if (storedFlag == EXACT) {
	loc = storedLoc;
	score = (int)storedScore;
	return true;
      }
      else if (storedFlag == LOWER)
	{ alpha = (alpha>storedScore?alpha:storedScore); }
      else { beta = (beta<storedScore?beta:storedScore); }

      if (alpha >= beta) {
	loc = storedLoc;
	score = (int)storedScore;
	return true;
      }

      return false;
    }

  }
  /*
  for(int i = 0; i < table[key].size(); i++){
    Entry entry = table[key][i];
    Bitstring storedState = entry.state;

    if (storedState == gs){
      int storedFlag = entry.flag;
      int storedLoc = entry.loc;
      int storedScore = entry.score;

      if (storedFlag == EXACT) {
	loc = storedLoc;
	score = storedScore; return true;
      }

      else if (storedFlag == LOWER)
	{ alpha = (alpha>storedScore?alpha:storedScore); }

      else { beta = (beta<storedScore?beta:storedScore); }

      if (alpha >= beta) {
	loc = storedLoc;
	score = storedScore;
	return true;
      }

      return false;
    }
  }
  */
  return false;
}

void Board_AB::store(BitstringKey key, Bitstring gs,
		     int score, int loc, int alphaOrig, int beta){
  char flag = EXACT;
  if (score <= alphaOrig) { flag = UPPER; }
  else if (score >= beta) { flag = LOWER; }
  flag <<= 2;

  char storedScore = score+1;

  short storedLoc = loc;
  storedLoc <<= 4;

  Bitstring storedVal = (storedLoc | flag | storedScore);
  storedVal <<= GAMESTATE;
  storedVal |= gs;//gamestate;

  //BitstringKey key = zobrist % MAXKEY;
  if (table.find(key) == table.end()) {
    table[key].push_back(storedVal);
    return;
  }

  bool push = true;
  for(int i = 0; i < table[key].size(); i++){
    Bitstring stored = table[key][i];
    Bitstring storedState = (stored << METADATA) >> METADATA;
    short metadata = (stored >> GAMESTATE);
    char existedFlag = (metadata % 16) >> 2;
    if (storedState == gs) {
      if (existedFlag != EXACT) { table[key][i] = storedVal; }
      push = false;
      break;
    }
  }

    /*
  Entry storedVal = {score, loc, flag, gamestate};
  BitstringKey key = zobrist % MAXKEY;
  if (table.find(key) == table.end()) {
    table[key].push_back(storedVal);
    return;
  }
  
  bool push = true;
  for(int i = 0; i < table[key].size(); i++){
    Entry entry = table[key][i];
    if (entry.state == gamestate) {
      if (entry.flag != EXACT) { table[key][i] = storedVal; }
      push = false;
      break;
    }
  }

  */
  if (push) { table[key].push_back(storedVal); }
  
}

bool Board_AB::retrieveSmart(int& score, int& loc, int& alpha, int& beta){
  bool ans = false;

  Bitstring mirroredState = 0;
  Bitstring mirroredZ = 0;

  for(int i = n-1; i > -1; i--){
    if (grid[i] != '.') {
      pair<Bitstring,Bitstring>& aZ = assignmentZ[n-i-1];
      pair<Bitstring,Bitstring>& aG = assignmentG[n-i-1];
      mirroredZ ^= (grid[i]=='R'?aZ.first:aZ.second);
      mirroredState |= (grid[i]=='R'?aG.first:aG.second);
    }
  }
  
  BitstringKey key = zobrist % MAXKEY;
  BitstringKey mirroredKey = mirroredZ % MAXKEY;

  if (mirroredKey < key){
    ans = retrieve(mirroredKey,mirroredState,score, loc, alpha, beta);
    loc = n-loc-1;
  }

  else { ans = retrieve(key,gamestate,score, loc, alpha, beta);}

  return ans;
}

void Board_AB::storeSmart(int score, int loc, int alphaOrig, int beta){
  Bitstring mirroredState = 0;
  Bitstring mirroredZ = 0;
  for(int i = n-1; i > -1; i--){
    if (grid[i] != '.') {
      pair<Bitstring,Bitstring>& aZ = assignmentZ[n-i-1];
      pair<Bitstring,Bitstring>& aG = assignmentG[n-i-1];
      mirroredZ ^= (grid[i]=='R'?aZ.first:aZ.second);
      mirroredState |= (grid[i]=='R'?aG.first:aG.second);
    }
  }

  BitstringKey mirroredKey = mirroredZ % MAXKEY;
  BitstringKey key = zobrist % MAXKEY;
  if (mirroredKey < key)
    { store(mirroredKey,mirroredState,score, n-loc-1, alphaOrig, beta);}
  else { store(key,gamestate,score, loc, alphaOrig, beta);}
}

scoreAndLoc Board_AB::alphabeta(bool maximize, int alpha, int beta,
				size_t depth, int x){
  recursionCount++;

  int score = -10;
  int loc = -1;
  int alphaOrig = alpha;
  if (retrieveSmart(score, loc, alpha, beta)){ return scoreAndLoc(score,loc); }

  if (depth > 0){
    if (memberOfAP(x)){
      int sign = (maximize?1:-1);
      int result = sign * ((!maximize)?R_WIN:B_WIN);

      return scoreAndLoc(result, x);
    }
    if (depth == n) { return draw; }
  }
  /*
  size_t& killer1 = killers[depth].first;
  if (killer1 != n && alphabeta_helper(killer1, maximize, depth, score,
				       loc, alpha, beta)){
    return scoreAndLoc(score,loc);
  }
  size_t killer2 = killers[depth].second;
  if (killer2 != n && alphabeta_helper(killer2, maximize, depth, score,
				       loc, alpha, beta)){
    return scoreAndLoc(score,loc);
  }
  */

  //symmetry -> don't bother checking rhs
  bool s = symmetric();
  bool firstChild = true;

  for(int i = ((n%2)?(n/2):((n/2) - 1)); i > -1; i--){
    if (//i != killer1 && i != killer2 &&
	alphabeta_helper(i, maximize, depth, score, loc, alpha, beta,firstChild))
      {
	//killers[depth].second = killer1;
	//killer1 = i;
	break;
      }
    
    if (s) {continue;}
    
    int j = n-i-1;
    if (//j != killer1 && j != killer2 &&
	alphabeta_helper(j, maximize, depth, score, loc, alpha, beta,firstChild))
      {
	//killers[depth].second = killer1;
	//killer1 = i;
	break;
      }
    
  }

  storeSmart(score, loc, alphaOrig, beta);

  return scoreAndLoc(score, loc);
}

bool Board_AB::alphabeta_helper(size_t i, bool maximize, size_t depth,
				int& score, int& loc,
				int& alpha, int& beta,
				bool& firstChild
				){
  if (i >= n || grid[i] != '.') { return false; }

  grid[i] = (maximize?'R':'B');

  Bitstring z = (maximize?assignmentZ[i].first:assignmentZ[i].second);
  zobrist ^= z;

  Bitstring g = (maximize?assignmentG[i].first:assignmentG[i].second);
  gamestate |= g;
  
  int curScore = 0;

  //PVS
  if (firstChild) {
    curScore = -alphabeta(!maximize, -beta, -alpha, depth+1, i).first;
    firstChild = false;
  }
  else {
    curScore = -alphabeta(!maximize, -alpha - 1, -alpha, depth+1, i).first;
    if (alpha < curScore && curScore < beta)
      { curScore = -alphabeta(!maximize, -beta, -curScore, depth+1, i).first; }
  }
  

  if (curScore > score) {
    loc = i;
    score = curScore;
    alpha = (alpha<score?score:alpha);
  }

  grid[i] = '.';
  zobrist ^= z;
  gamestate ^= g;

  return alpha >= beta;

}
