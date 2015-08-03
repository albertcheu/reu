#include <cmath>
#include <cfloat>
#include <algorithm>
#include <unordered_set>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include "Board.h"

struct State{
  State(int depth, int loc, bool redPlayer, State* parent);

  int depth, loc, numTrials;
  int redWins, blueWins;
  bool redPlayer;
  State* parent;
  vector<State*> children;
};

int bestDepth(int n, int cutoff);

class BoardMC: public Board{
 private:

  mt19937 gen;

  unordered_set<int> moves;

  vector<int> indices,empties;

  State* start;

  int storeDepth;

  int buildTree(State* s);

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
