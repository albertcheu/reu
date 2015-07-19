#include "MCEvaluator.h"

MCEvaluator::MCEvaluator(size_t n, size_t k)
  :Evaluator(n,k), numTrials(0), redWins(0)
{
  for(size_t i = 0; i < n; i++){
    colored.push_back(0);
    order.push_back(i);
    tree.push_back(State(0,false));
    moves.emplace(i);
  }
}

void MCEvaluator::place(bool maximize, size_t i){
  grid[i] = (maximize?'R':'B');
  //tree.pop_back();
  moves.erase(i);
}

void MCEvaluator::undo(bool maximize, size_t i){
  grid[i] = '.';
  //tree.push_back(State(0,false));
  moves.emplace(i);
}

bool MCEvaluator::play(char c, int loc){
  bool ans = Board::play(c,loc);
  if (ans) { moves.erase(loc); }
  return ans;
}

float MCEvaluator::score(State& s){
  //The parent state evaluates its child states
  bool parentIsRed = !(s.redPlayer);

  //If the parent is red, the scores of its children are based on red
  //to facilitate maximizing scores
  float avgSuccess = (parentIsRed?s.redWins:s.blueWins)/(float)s.numTrials;

  //Standard formula
  float regret = sqrt(2.0f*log(numTrials)/(float)s.numTrials);

  return avgSuccess + regret;
}


void MCEvaluator::runTrial(){

  State* s = NULL;
  float optimalScore = 0.0f;

  for(size_t i = 0; i < moves.size(); i++){
    //Find a "bandit arm" that hasn't been played...
    if (tree[i].numTrials == 0) { s = &(tree[i]); break; }

    //or best satisfies our objective function
    float childScore = score(tree[i]);
    if (childScore < 0.0f) { cout << "Oh no! Negative score" << endl; }
    if (childScore > optimalScore) {
      optimalScore = childScore;
      s = &(tree[i]);
    }
  }

  if (!s) { cout << "Oh no! Null pointer" << endl; }

  numTrials++;
  if (runTrialRandom(s)) { redWins++; }

}

bool MCEvaluator::runTrialRandom(State* s){

  grid[s->loc] = (s->redPlayer?'R':'B');
  bool red = !(s->redPlayer);

  bool redWon = true;
  bool draw = true;
  random_shuffle(order.begin(),order.end());

  int j = 0;
  for(size_t i = 0; i < n; i++){
    if (order[i] == s->loc || moves.find(order[i]) == moves.end()){ continue; }

    //Color
    grid[order[i]] = (red?'R':'B');

    //Keep track of what we color
    colored[j++] = order[i];

    //Check who won every time a number is colored  
    if (memberOfAP(order[i])) {
      draw = false;
      redWon = red;
      break;
    }

    //Alternate!
    red = !red;
  }

  //Clear out what we colored
  grid[s->loc] = '.';
  for(int i = 0; i < j; i++){
    grid[colored[i]] = '.';
  }

  //Keep track of results
  s->numTrials++;
  if (draw || !redWon) { s->blueWins++; return false; }
  s->redWins++;
  return true;

}

State::State(unsigned int loc, bool redPlayer)
  :loc(loc), redPlayer(redPlayer), numTrials(0), redWins(0), blueWins(0)
{}

size_t MCEvaluator::actualEvaluate(bool maximize, size_t i){
  numTrials = 0;
  redWins = 0;

  //cout << "About to run trials" << endl;
  //print();
  place(maximize, i);
  if (moves.size() <= 1) { undo(maximize,i); return 1; }

  size_t index = 0;
  for(unordered_set<size_t>::iterator itr = moves.begin(); itr != moves.end();
      itr++){
    tree[index] = State(*itr,!maximize);
    tree[index].numTrials = tree[index].redWins = tree[index].blueWins = 0;
    index++;
  }

  for(size_t x = 0; x < 500; x++){ runTrial(); }

  undo(maximize,i);
  //print();
  //cout << "Done." << endl;

  float top = 1000.0f * (maximize?redWins:(numTrials-redWins));
  return (size_t) (top / numTrials);
}
