#include <cmath>
#include <cfloat>
#include <algorithm>
#include <unordered_set>
#include <cstdlib>
#include <cstdio>
#include "Board.h"

struct State{
  State(int depth, int loc, bool redPlayer, State* parent);

  int depth, loc, numTrials;
  int redWins, blueWins;
  bool redPlayer;
  State* parent;
  std::vector<State*> children;
};

int bestDepth(int n, int cutoff);

class BoardMC: public Board{
 private:
  //Unlike alpha-beta, our list of available moves doesnt need to be ordered
  std::unordered_set<int> moves;

  std::vector<int> indices;

  State* start;

  int storeDepth;

  bool memberOfAP(int loc);

  int buildTree(State* s);

  int freeRecursive(State* s);

  bool runTrial(State* s);
  bool runTrialTraverse(State* s);
  bool runTrialRandom(State* s);

  float score(State* s);
  
 public:
  BoardMC(size_t n, size_t k);
  ~BoardMC();

  void montecarlo();
};
