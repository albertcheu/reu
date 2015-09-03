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

const num BITSETSIZE = 64;
const num KEYSIZE = 64;

typedef unordered_set<bitset<BITSETSIZE> > BitsetTable;

struct State{
  State(num depth, num loc, const bitset<BITSETSIZE>& gamestate, State* parent);

  num depth, loc;
  bitset<BITSETSIZE> gamestate;
  State* parent, *firstChild, *nextSibling;
  size_t redWins,numTrials;
};

class BoardMC: public Board{
 private:
  BitsetTable bt;
  bitset<BITSETSIZE> b, mirror;

  void store();

  mt19937_64 gen;

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
