#include "Evaluator.h"

Evaluator::Evaluator(size_t n, size_t k)
  :Board(n,k)
{}

bool pairCompare(const std::pair<size_t,size_t>& firstElem,
		 const std::pair<size_t,size_t>& secondElem)
{ return firstElem.first >= secondElem.first; }

void Evaluator::evaluate(bool maximize, RankingVector& rankedPositions){

  for(size_t i = 0; i < n; i++){
    if (grid[i] != '.') { continue; }
    rankedPositions.push_back({actualEvaluate(maximize, i), i});
  }

  //Sort by ranking (first element of each pair in rankedPositions)
  sort(rankedPositions.begin(), rankedPositions.end(), pairCompare);
}
