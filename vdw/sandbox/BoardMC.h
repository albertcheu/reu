#include <cmath>
#include <cfloat>
#include <algorithm>
#include <unordered_set>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <queue>
#include "Board.h"

struct State{
  State(num depth, num loc, State* parent);

  num depth, loc;
  State* parent;
  vector<State*> children;

  num redWins, blueWins, numTrials;
};

class BoardMC: public Board{
 private:

  mt19937 gen;

  unordered_set<num> moves;

  vector<num> indices,empties;

  State* start;

  size_t buildTree();

  size_t freeRecursive(State* s);

  bool runTrial(State* s);
  bool runTrialTraverse(State* s);
  bool runTrialRandom(State* s);

  float score(State* s);
  
 public:
  BoardMC(num n, num k);
  ~BoardMC();

  void montecarlo();
};
