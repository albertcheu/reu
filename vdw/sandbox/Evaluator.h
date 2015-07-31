#pragma once

#include "Board.h"
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <unordered_set>

typedef vector<pair<size_t,num> > RankingVector;

class Evaluator: public Board{

 protected:

  Evaluator(num n, num k);
  virtual size_t actualEvaluate(bool maximize, num i)=0;


 public:
  //During tree traversal, we plop in and pull out characters
  virtual void place(bool maximize, num i)=0;
  virtual void undo(bool maximize, num i)=0;

  void evaluate(bool maximize, RankingVector& rankedPositions);
};
