#include <cmath>
#include <cfloat>
#include <algorithm>
#include <unordered_set>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include <queue>
#include <bitset>
#include "Board.h"

const int BITSETSIZE = 256;

struct State{
  State(num depth, num loc, const bitset<BITSETSIZE>& gamestate, State* parent);

  num depth, loc;
  bitset<BITSETSIZE> gamestate;
  vector<State*> children;
  State* parent;

  num redWins, blueWins, numTrials;
};

class BoardMC: public Board{
 private:

  mt19937 gen;

  unordered_set<num> moves;

  vector<num> indices,empties;

  State* start;

  size_t buildTree();
  bool symmetricBitset(const bitset<BITSETSIZE>& b);

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
