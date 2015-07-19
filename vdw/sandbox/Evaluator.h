#pragma once

#include "Board.h"
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <unordered_set>

typedef vector<pair<size_t,size_t> > RankingVector;

class Evaluator: public Board{

 protected:

  Evaluator(size_t n, size_t k);
  virtual size_t actualEvaluate(bool maximize, size_t i)=0;


 public:
  //During tree traversal, we plop in and pull out characters
  virtual void place(bool maximize, size_t i)=0;
  virtual void undo(bool maximize, size_t i)=0;

  void evaluate(bool maximize, RankingVector& rankedPositions);
};
