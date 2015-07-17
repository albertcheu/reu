#include "Board.h"
#include <cmath>
#include <cfloat>
#include <algorithm>
#include <unordered_set>
#include <cstdlib>
#include <cstdio>

bool pairCompare(const std::pair<float,size_t>& firstElem,
		 const std::pair<float,size_t>& secondElem)
{ return firstElem.first < secondElem.first; }


class Evaluator: public Board{

 protected:
  RankingVector rankedPositions;
  Evaluator(size_t n, size_t k);
  virtual void actualEvaluate(size_t i)=0;

 public:
  pair<RankingVector::iterator,RankingVector::iterator> evaluate();
};
