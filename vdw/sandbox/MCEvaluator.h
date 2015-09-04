#pragma once
#include "Evaluator.h"

class MCEvaluator: public Evaluator{
 public:
  MCEvaluator(num n, num k);
  void place(bool maximize, num i);
  void undo(bool maximize, num i);

 private:
  num depth;
  bitset<BITSETSIZE> b;
  vector<vector<bitset<BITSETSIZE> > > data;
  
  float actualEvaluate(bool maximize, num i);
};
