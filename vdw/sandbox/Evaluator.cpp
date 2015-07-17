#include "Evaluator.h"

Evaluator::Evaluator(size_t n, size_t k)
  :Board(n,k)
{
  for(size_t i = 0; i < n; i++){
    rankedPositions.push_back({0.0f,i});
  }
}

pair<RankingVector::iterator,RankingVector::iterator> Evaluator::evaluate(){

  //The indices of empty spots are put in the beginning of rankedPositions
  size_t index = 0;
  for(size_t i = 0; i < n; i++){
    if (grid[i] != '.') { continue; }
    float rank = actualEvaluate(i);
    rankedPositions[index].first = rank;
    rankedPositions[index++].second = i;
  }

  if (index < n) {
    //We put invalid index n in rankedPositions to mark the end of that stretch
    rankedPositions[index].second = n;
  }

  //Sort by ranking (first element of each pair in rankedPositions)
  sort(rankedPositions.begin(), rankedPositions.begin()+index, pairCompare);
}
