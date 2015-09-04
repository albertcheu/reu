#pragma once
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <unordered_set>
#include <cstdio>
#include <chrono>
#include <queue>
#include "Board.h"

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
  size_t numEndstates;
  vector<BitsetTable> bts;
  bitset<BITSETSIZE> b, mirror;

  void store(num depth);

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
