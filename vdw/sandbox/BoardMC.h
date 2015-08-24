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
  State(int depth, int loc, State* parent);

  int depth, loc;
  State* parent;
  vector<State*> children;

  int redWins, blueWins, numTrials;
};

//int bestDepth(int n, int cutoff);

class BoardMC: public Board{
 private:

  mt19937 gen;

  unordered_set<int> moves;

  vector<int> indices,empties;

  State* start;

  //int storeDepth;

  //int buildTree(State* s);
  int buildTree();

  int freeRecursive(State* s);

  bool runTrial(State* s);
  bool runTrialTraverse(State* s);
  bool runTrialRandom(State* s);

  float score(State* s);
  
 public:
  BoardMC(num n, num k);
  ~BoardMC();

  void montecarlo();
};
