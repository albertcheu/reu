#include "BoardMC.h"
State::State(num depth, num loc,
	     const bitset<BITSETSIZE>& gamestate, State* parent)
  :depth(depth), loc(loc), gamestate(gamestate), parent(parent),
   redWins(0), blueWins(0), numTrials(0)
{}

BoardMC::BoardMC(num n, num k)
  :Board(n,k),
   gen((unsigned)chrono::system_clock::now().time_since_epoch().count())
{
  start = new State(0,n,0,NULL);

  for(num i = 0; i < n; i++){
    //The board is empty, so all of [1,n] are available moves  
    moves.emplace(i);
    //
    indices.push_back(i);
    empties.push_back(i);
  }

  size_t ans = buildTree();
  cout << ans << endl;

}

bool retrieve(const bitset<BITSETSIZE>& b, BitsetTable& bt){
  //hash b
  bitset<KEYSIZE> key = 0;
  for(num i = 0; i < KEYSIZE; i++) { key[i] = b[i]; }

  //if hash isn't present return false
  if (bt.find(key) == bt.end()) { return false; }

  //Else iterate thru chain and compare
  for (BitsetChain::const_iterator itr = bt[key].begin();
       itr != bt[key].end(); itr++){
    if (*itr == b) { return true; }
  }

  return false;
}

void store(const bitset<BITSETSIZE>& b, BitsetTable& bt){
  //hash b
  bitset<KEYSIZE> key = 0;
  for(num i = 0; i < KEYSIZE; i++) { key[i] = b[i]; }

  bt[key].push_back(b);
}

bool BoardMC::symmetricBitset(const bitset<BITSETSIZE>& b){
  num left,right; left = 0; right = 2*(n-1);
  while (left < right) {
    if (b[left] != b[right] || b[left+1] != b[right+1]) { return false; }
    left += 2;
    right -= 2;
  }
  return true;
}

size_t BoardMC::buildTree(){
  cout << "Building tree" << endl;
  
  size_t ans = 1;
  size_t counter = 0;
  size_t capacity = 1000000;
  num prevDepth = 0;
  num stopDepth = n;

  BitsetTable bt;
  queue<State> q;
  q.push(*start);

  while(true){

    if (counter < capacity) {

      State* s = start;

      if (q.front().depth) {
	s = new State(q.front().depth, q.front().loc,
		      q.front().gamestate, q.front().parent);
	s->parent->children.push_back(s);
	ans++;
      }

      q.pop();

      if (prevDepth < s->depth) { prevDepth = s->depth; bt.clear(); }

      num end = n-1;
      if (symmetricBitset(s->gamestate)) { end = (n%2)?(n/2):(n/2 - 1); }
      bitset<BITSETSIZE> newBitset = s->gamestate;
      for(num i = 0; i <= end; i++){
	if (s->gamestate[2*i] || s->gamestate[2*i + 1]) { continue; }

	newBitset.flip(2*i + s->depth%2);
	if (!retrieve(newBitset,bt)) {
	  State child(s->depth+1,i,newBitset,s); 
	  q.push(child);
	  store(newBitset, bt);
	  counter++;
	  if (counter == capacity) { stopDepth = child.depth; break; }
	}
	newBitset.flip(2*i + s->depth%2);	

      }
    }

    else if (q.front().depth < stopDepth) {
      State* s = new State(q.front().depth, q.front().loc,
			   q.front().gamestate, q.front().parent);
      q.pop();
      s->parent->children.push_back(s);
      ans++;
    }
    
    else { break; }
    
  }

  cout << stopDepth << endl;
  return ans;
}

BoardMC::~BoardMC(){
  size_t ans = freeRecursive(start);
  cout << ans << endl;
  moves.clear();
}

size_t BoardMC::freeRecursive(State* s){
  size_t ans = 1;

  size_t size = s->children.size();
  for(size_t i = 0; i < size; i++){
    ans += freeRecursive(s->children[i]);
  }
  s->children.clear();

  free(s);
  return ans;
}

void BoardMC::montecarlo(){
  size_t size = start->children.size();

  FILE* gp = (FILE*) popen("gnuplot -persist","w");  
  //Title
  fprintf(gp,"set title \"game(%lu,%lu)\" font \",16\"\n",n,k);
  //labels
  fprintf(gp,"set ylabel \"Percent of games won\"\n");
  fprintf(gp,"set ylabel font \",14\"\n");
  fprintf(gp,"set xlabel \"First move\"\n");
  fprintf(gp,"set xlabel font \",14\"\n");
  fprintf(gp,"set xrange [1:%lu]\n",size);
  fprintf(gp,"unset key\n");
  //Data
  fprintf(gp, "plot '-' u ($0+1):1 w lines\n");

  string userInput = "";
  while(true){
    num total = start->numTrials;

    //Every ten thousand trials, draw
    if (total % 10000 == 0 && total > 0) {
      double avgSuccess, numWins, numTrials;
      double maxSuccess = 0;
      for (size_t i = 0; i < size; i++){
	numWins = start->children[i]->redWins;
	numTrials = start->children[i]->numTrials;
	avgSuccess = numWins / numTrials;
	if (avgSuccess > maxSuccess) { maxSuccess = avgSuccess; }
	fprintf(gp, "%f\n", avgSuccess*100);
      }
      
      fprintf(gp,"E\n");
      if (maxSuccess > 0.5)
	{ fprintf(gp,"set arrow 1 from 1,50 to %lu,50 nohead\n",size); }
      else { fprintf(gp,"set noarrow\n"); }
      fprintf(gp,"refresh\n");

      if ((total+10000) % 1000000 == 0) {
	//Save image
	fprintf(gp, "set term png\nset output \"game(%lu,%lu).png\"\n",n,k);
      }
      else { fprintf(gp, "set term wxt\n"); }

      //Every million trials, ask if we should stop
      if (total % 1000000 == 0){
	cout << "Completed " << total << " trials.";
	cout << " Enter anything but 'quit' to continue: ";
	cin >> userInput;
      }

      if (userInput != "quit"){
	fprintf(gp, "replot\n");
	fflush(gp);
      }
      else { break; }
      
    }

    //Conduct experiment
    runTrialTraverse(start);

  }
  pclose(gp);
}

bool BoardMC::runTrial(State* s){

  //If State s is red's turn, red receives a grid with spot loc colored blue
  //If s belongs to blue, blue receives a grid with spot loc colored red
  bool parentIsRed = s->depth % 2;
  grid[s->loc] = (parentIsRed?'R':'B');
  moves.erase(moves.find(s->loc));

  //If there is a winner, return who won    
  if (memberOfAP(s->loc)) {
    moves.emplace(s->loc);
    grid[s->loc] = '.';
    s->numTrials++;
    (parentIsRed? s->redWins++ : s->blueWins++);
    return parentIsRed;
  }
  
  return (s->children.size()? runTrialTraverse(s):runTrialRandom(s));
}

float BoardMC::score(State* s){
  //The parent state evaluates its child states
  bool parentIsRed = s->depth % 2;

  //If the parent is red, the scores of its children are based on red
  //to facilitate maximizing scores
  float avgSuccess = (parentIsRed?s->redWins:s->blueWins)/(float)s->numTrials;

  //Standard formula
  float regret = sqrt(2.0f*log(start->numTrials)/(float)s->numTrials);

  return avgSuccess + regret;
}

bool BoardMC::runTrialTraverse(State* s){

  bool redWon = true;
  float optimalScore = 0.0f;
  State* bestChild = NULL;
  for (size_t i = 0; i < s->children.size(); i++){

    //Find a "bandit arm" that hasn't been played...
    if (s->children[i]->numTrials == 0) { bestChild = s->children[i]; break; }

    //or best satisfies our objective function
    float childScore = score(s->children[i]);
    if (childScore > optimalScore) {
      optimalScore = childScore;
      bestChild = s->children[i];
    }
  }

  //Recursion
  redWon = runTrial(bestChild);

  //Revert to previous state
  if (s->depth > 0) {
    grid[s->loc] = '.';
    moves.emplace(s->loc);
  }

  //Increment numTrials
  (s->numTrials)++;
  (redWon?(s->redWins)++:(s->blueWins)++);

  return redWon;   
}

bool BoardMC::runTrialRandom(State* s){
  bool redWon = true;
  bool parentIsRed = !(s->depth % 2);
  bool draw = true;

  //Randomly color remaining moves

  shuffle(indices.begin(),indices.end(),gen);
  num j = 0;
  for(num i = 0; i < n; i++){
    unordered_set<num>::iterator itr = moves.find(indices[i]);
    if (itr == moves.end()){ continue; }

    //Color
    grid[indices[i]] = (parentIsRed?'R':'B');

    //Keep track of what we color
    empties[j++] = indices[i];

    //Check who won every time a number is colored  
    if (memberOfAP(indices[i])) {
      draw = false;
      redWon = parentIsRed;
      break;
    }

    //Alternate!
    parentIsRed = !parentIsRed;
  }

  //Clear out what we colored
  grid[s->loc] = '.';
  for(num i = 0; i < j; i++){
    grid[empties[i]] = '.';
  }

  //Restore moves
  moves.emplace(s->loc);

  //Keep track of results
  s->numTrials++;
  if (draw || !redWon) { s->blueWins++; return false; }
  s->redWins++;
  return true;  
}
