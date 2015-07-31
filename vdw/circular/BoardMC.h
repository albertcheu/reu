#include <cmath>
#include <cfloat>
#include <algorithm>
#include <unordered_set>
#include <cstdlib>
#include <cstdio>
#include "RoundBoard.h"

struct State{
  State(int depth, num loc, bool redPlayer, State* parent);

  int depth, numTrials;
  num loc;
  int redWins, blueWins;
  bool redPlayer;
  State* parent;
  vector<State*> children;
};

int bestDepth(int n, int cutoff);

class BoardMC: public RoundBoard{
 private:
  unordered_set<num> moves;

  vector<num> indices,empties;

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
