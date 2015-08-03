#include "BoardMC.h"

State::  State(num depth, num loc, bool redPlayer, State* parent)
  :depth(depth), loc(loc), redPlayer(redPlayer), parent(parent),
   redWins(0), blueWins(0), numTrials(0)
{}

int bestDepth(num n, int cutoff){

  int numNodes = 1;
  int leaves = 1;//n/2;
  int factor = n-2;//n-1;
  int depth = 0;
  while (factor > 0 && numNodes < cutoff) {
    depth++;
    numNodes += leaves;
    leaves *= (factor--);
  }

  return (3>depth-1?3:depth-1);
}

BoardMC::BoardMC(num n, num k)
  :RoundBoard(n,k), storeDepth(bestDepth(n,6000000))
{
  cout << storeDepth << endl ;

  start = new State(0,0,true,NULL);

  for(num i = 0; i < n; i++){
    //The board is empty, so all of [1,n] are available moves  
    moves.emplace(i);
    //
    indices.push_back(i);
    empties.push_back(i);
  }

  //Build tree up to storeDepth
  cout << buildTree(start) << endl;

}

int BoardMC::buildTree(State* s){
  int ans = 1;

  //fill children
  for(num i = 0; i < n; i++){

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
    //if (s->depth==0 && i==n/2) { break; }
    if (s == start) { break; }
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
  /*
  FILE* gp = (FILE*) popen("gnuplot -persist","w");  
  //Title
  fprintf(gp,"set title \"game(%d,%d)\"\n",n,k);
  //labels
  fprintf(gp,"set ylabel \"Percent of games won\"\n");
  fprintf(gp,"set ylabel font \",16\"\n");
  //fprintf(gp,"set xlabel \"First move\"\n");
  fprintf(gp,"unset key\n");

  //Data
  fprintf(gp, "%s\n", "plot '-' with lines");
  */
  cout << "Starting montecarlo" << endl;
  string userInput = "";
  while(true){
    size_t total = start->numTrials;
    cout << total << endl;

    //Every ten thousand trials, draw
    if (total % 10000 == 0 && total > 0) {
      cout << total << endl;
      float avgSuccess, numWins, numTrials;
      
      //size_t size = start->children.size();

      //for (int i = 0; i < size; i++){
      numWins = start->redWins;//children[i]->redWins;
      numTrials = start->numTrials;//children[i]->numTrials;

	avgSuccess = numWins / numTrials;
	cout << avgSuccess << endl;
	/*
	fprintf(gp, "%f\n", avgSuccess*100);
	fprintf(gp, "%f\n", avgSuccess*100);
	//}

      fprintf(gp,"E\n");
      fprintf(gp,"set arrow 1 from 0,50 to 1,50 nohead\n");
      fprintf(gp,"refresh\n");

      if ((total+10000) % 1000000 == 0) {
	//Save image
	fprintf(gp, "set term png\nset output \"montecarlo.png\"\n");
      }
      else { fprintf(gp, "set term wxt\n"); }
	*/
      //Every million trials, ask if we should stop
      if (total % 1000000 == 0){
	cout << "Completed " << total << " trials.";
	cout << " Enter anything but 'quit' to continue: ";
	cin >> userInput;
      }

      if (userInput != "quit"){
	//fprintf(gp, "replot\n");
	//fflush(gp);
      }
      else { break; }
      
    }

    //Conduct experiment
    runTrialTraverse(start);

  }
  //pclose(gp);
}

bool BoardMC::runTrial(State* s){

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
  
  return (s->children.size()? runTrialTraverse(s):runTrialRandom(s));
}

float BoardMC::score(State* s){
  //The parent state evaluates its child states
  bool parentIsRed = s->parent->redPlayer;

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
  bool parentIsRed = s->redPlayer;
  bool draw = true;

  //Randomly color remaining moves
  random_shuffle(indices.begin(),indices.end());
  int j = 0;
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
