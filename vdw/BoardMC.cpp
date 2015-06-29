#include "BoardMC.h"

using namespace std;

State::State(int depth, int loc, bool redPlayer, State* parent)
  :depth(depth), loc(loc), redPlayer(redPlayer), parent(parent),
   redWins(0), blueWins(0), numTrials(0)
{}

BoardMC::BoardMC(size_t n, size_t k)
  :Board(n,k)
{

  start = new State(0,-1,true,NULL);

  for(int i = 0; i < n; i++){
    //The board is empty, so all of [1,n] are available moves  
    moves.emplace(i);
    //
    indices.push_back(i);
  }
  
  //Build tree up to STORE_DEPTH
  int ans = buildTree(start);
  cout << ans << endl;

}

int BoardMC::buildTree(State* s){
  int ans = 1;

  //fill children
  for(int i = 0; i < n; i++){
    unordered_set<int>::iterator itr = moves.find(i);
    //Cannot use moves we've already used
    if (itr == moves.end()) { continue; }

    State* child = new State(s->depth+1,i,!(s->redPlayer),s);

    s->children.push_back(child);

    //recurse if child isn't too deep
    if (child->depth < STORE_DEPTH) {
      moves.erase(itr);
      ans += buildTree(child);
      moves.emplace(i);
    }

    else { ans += 1; }
  }

  return ans;
}

BoardMC::~BoardMC(){
  int ans = freeRecursive(start);
  cout << ans << endl;
  moves.clear();
}

int BoardMC::freeRecursive(State* s){
  int ans = 1;

  if (s->depth < STORE_DEPTH){
    int size = s->children.size();
    for(int i = 0; i < size; i++){
      ans += freeRecursive(s->children[i]);
    }
    s->children.clear();
  }
  else { ans = 1; }

  free(s);
  return ans;
}

void BoardMC::montecarlo(){
  FILE* gp = (FILE*) popen("gnuplot -persist","w");  
  fprintf(gp, "%s\n", "plot '-' with lines");

  string s = "";
  while(s != "quit"){
    if (start->numTrials % 1000000 == 0 && start->numTrials > 0){
      cout << "Completed " << start->numTrials << " trials.";
      cout << " Enter anything but 'quit' to continue: ";
      cin >> s;
    }

    if (start->numTrials % 1000 == 0 && start->numTrials > 0) {
      float avgSuccess, numWins, numTrials;
      for (int i = 0; i < n; i++){
	numWins = start->children[i]->redWins;
	numTrials = start->children[i]->numTrials;
	avgSuccess = numWins / numTrials;
	fprintf(gp, "%f\n", avgSuccess);
      }

      fprintf(gp, "E\nrefresh\n");
      if (s != "quit") {
	fprintf(gp, "replot\n");
	fflush(gp);
      }

    }

    //Conduct experiment
    runTrial(start);
  }

}

bool BoardMC::memberOfAP(int loc){
 int d = 1;
  while (true){

    if (loc+d >= n && loc-d < 0) { return false; }

    int numLeft, numRight;
    numLeft = numRight = 0;

    for(int i = loc+d; i < n; i += d){
      if (grid[i] == grid[loc]) { numRight++; }
      else { break; }
    }
    if (1+numRight >= k) { return true; }

    for(int i = loc-d; i > -1; i -= d){
      if (grid[i] == grid[loc]) { numLeft++; }
      else { break; }
    }
    if (numLeft + numRight + 1 >= k) { return true; }

    d++;
  }

  return false; 
}

float BoardMC::score(State* s){
  //The parent state evaluates its child states
  bool parentIsRed = s->parent->redPlayer;

  //If the parent is red, the scores of its children are based on red
  //to facilitate maximizing scores
  float avgSuccess = (parentIsRed?s->redWins:s->blueWins)/(float)s->numTrials;

  //Standard formula
  float regret = sqrt(2*log(start->numTrials)/s->numTrials);

  return avgSuccess + regret;
}

bool BoardMC::runTrial(State* s){
  if (s->depth) {
    //If State s is red's turn, red receives a grid with spot loc colored blue
    //If s belongs to blue, blue receives a grid with spot loc colored red
    bool parentIsRed = s->parent->redPlayer;
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

  }

  //If we're not at the STORE_DEPTH, go toward it
  if (s->depth < STORE_DEPTH) { return runTrialTraverse(s); }

  //else color available moves in random order
  return runTrialRandom(s);

}

bool BoardMC::runTrialTraverse(State* s){
  bool redWon = true;
  float optimalScore = 0.0f;
  State* bestChild = NULL;
  for (int i = 0; i < s->children.size(); i++){

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
  bool parentIsRed = s->redPlayer;
  bool draw = true;

  random_shuffle(indices.begin(),indices.end());
  for(int i = 0; i < n; i++){
    unordered_set<int>::iterator itr = moves.find(indices[i]);      
    if (itr == moves.end()){ continue; }
    grid[indices[i]] = (parentIsRed?'R':'B');

    //Check who won every time a number is colored  
    if (memberOfAP(indices[i])) {
      draw = false;
      redWon = parentIsRed;
      break;
    }

    //Alternate!
    parentIsRed = !parentIsRed;
  }

  //Clear out what we randomly put in
  for(int i = 0; i < n; i++){
    unordered_set<int>::iterator itr = moves.find(indices[i]);      
    if (itr != moves.end()){ grid[indices[i]] = '.'; }
  }

  moves.emplace(s->loc);
  grid[s->loc] = '.';
  s->numTrials++;
  if (draw || !redWon) { s->blueWins++; return false; }

  s->redWins++;
  return true;  
}

/*
int toNumber(string s, int maxNum){
  int ans = 0;
  int place = 1;
  for(int i = s.size()-1; i > -1; i--){
    if (! isdigit(s[i])) { return -1; }
    int digit = ((int)s[i]) - 48;
    ans += place*digit;
    place *= 10;
  }
  if (ans < 1 || ans > maxNum) { return -1; }
  return ans;
}
*/
/*
int main(int argc, char** argv){
  if (argc == 3) {
    srand((unsigned)time(NULL));
    string s;
    
    //n
    s = argv[1];
    int n = toNumber(s, 10000);
    if (n == -1) {
      cout << "Please enter 0 < n < 10000" << endl;
      return 0;
    }
    
    //k
    s = argv[2];
    int k = toNumber(s, 10000);
    if (k == -1) {
      cout << "Please enter 0 < k < 10000" << endl;
      return 0;
    }
    else if (n < k) {
      cout << "Your board size (n) is smaller than the progression length!";
      cout << endl;
      return 0;
    }

    BoardMC bmc(n,k);
    bmc.montecarlo();
    return 0;
  }
  return 1;
}
*/
