#include "Evaluator.h"

Evaluator::Evaluator(num n, num k)
  :Board(n,k)
{}

bool pairCompare(const std::pair<size_t,num>& firstElem,
		 const std::pair<size_t,num>& secondElem)
{ return firstElem.first >= secondElem.first; }

void Evaluator::evaluate(bool maximize, RankingVector& rankedPositions){

  for(num i = 0; i < n; i++){
    if (grid[i] != '.') { continue; }
    rankedPositions.push_back({actualEvaluate(maximize, i), i});
  }

  //Sort by ranking (first element of each pair in rankedPositions)
  sort(rankedPositions.begin(), rankedPositions.end(), pairCompare);
}
