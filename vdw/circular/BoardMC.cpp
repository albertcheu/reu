#include "BoardMC.h"

State::State(num depth, num loc, bool redPlayer, State* parent)
  :depth(depth), loc(loc), redPlayer(redPlayer), parent(parent),
   redWins(0), blueWins(0), numTrials(0)
{}

int bestDepth(num n, int cutoff){
  int numNodes = 1;
  int leaves = n/2;
  int factor = n-2;
  int depth = 1;

  while (factor > 0){
    depth++;
    numNodes += leaves;
    leaves *= (factor--);
    if (numNodes+leaves >= cutoff) { break; }
  }

  return (3>depth-1?3:depth-1);
}

BoardMC::BoardMC(num n, num k)
  :RoundBoard(n,k), storeDepth(bestDepth(n,40000000)),
   gen((unsigned)chrono::system_clock::now().time_since_epoch().count())   
{
  cout << storeDepth << endl ;

  start = new State(0,0,false,NULL);

  for(num i = 1; i < n; i++){
    moves.emplace(i);
    indices.push_back(i);
    empties.push_back(i);
  }

  //Build tree up to storeDepth
  cout << buildTree(start) << endl;

}

int BoardMC::buildTree(State* s){
  int ans = 1;

  //fill children
  for(num i = 1; i < n; i++){

    unordered_set<num>::iterator itr = moves.find(i);
    //Cannot use moves we've already used
    if (itr == moves.end()) { continue; }

    State* child = new State(s->depth+1,i,!(s->redPlayer),s);

    s->children.push_back(child);

    //recurse if child isn't too deep
    if (child->depth < storeDepth) {
      moves.erase(itr);
      ans += buildTree(child);
      moves.emplace(i);
    }

    else { ans += 1; }

    if (s == start && s->children.size() == n/2) { break; }
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

  int size = s->children.size();
  for(int i = 0; i < size; i++){
    ans += freeRecursive(s->children[i]);
  }
  s->children.clear();

  free(s);
  return ans;
}

void BoardMC::montecarlo(){

  cout << "Starting montecarlo" << endl;
  string userInput = "";
  while(true){
    size_t total = start->numTrials;
    //cout << total << endl;

    if (total % 10000 == 0 && total > 0) {
      cout << total << endl;
      float avgSuccess, numWins;
      numWins = start->redWins;

      avgSuccess = numWins / total;
      cout << avgSuccess << endl;

      //Every million trials, ask if we should stop
      if (total % 1000000 == 0){
	cout << "Completed " << total << " trials.";
	cout << " Enter anything but 'quit' to continue: ";
	cin >> userInput;
      }

      if (userInput == "quit") { break; }
      
    }

    //Conduct experiment
    runTrial(start);

  }
}

bool BoardMC::runTrial(State* s){

  //If State s is red's turn, red receives a grid with spot loc colored blue
  //If s belongs to blue, blue receives a grid with spot loc colored red
  bool parentIsRed = !(s->redPlayer);
  //cout << (int)s->loc << endl;
  grid[s->loc] = (parentIsRed?'R':'B');
  if (s != start) { moves.erase(moves.find(s->loc)); }

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
  bool parentIsRed = !(s->redPlayer);

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

  //cout << "Ran trial" << endl;

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
  bool redPlayer = s->redPlayer;
  bool draw = true;

  //Randomly color remaining moves
  shuffle(indices.begin(),indices.end(), gen);
  int j = 0;
  for(size_t i = 0; i < indices.size(); i++){
    unordered_set<num>::iterator itr = moves.find(indices[i]);
    if (itr == moves.end()){ continue; }

    //Color
    grid[indices[i]] = (redPlayer?'R':'B');

    //Keep track of what we color
    empties[j++] = indices[i];

    //Check who won every time a number is colored  
    if (memberOfAP(indices[i])) {
      draw = false;
      redWon = redPlayer;
      break;
    }

    //Alternate!
    redPlayer = !redPlayer;
  }

  //Clear out what we colored
  grid[s->loc] = '.';
  for(size_t i = 0; i < j; i++){
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
