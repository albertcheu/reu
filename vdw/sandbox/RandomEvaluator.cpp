#include "RandomEvaluator.h"

RandomEvaluator::RandomEvaluator(size_t n, size_t k)
  :Evaluator(n,k)
{
  for (size_t i = 0 ; i < n; i++){
    ranks.push_back(i);
  }
  random_shuffle(ranks.begin(), ranks.end());
}

void RandomEvaluator::place(bool maximize, size_t i){
  grid[i] = (maximize?'R':'B');

}

void RandomEvaluator::undo(bool maximize, size_t i){
  grid[i] = '.';
}


size_t RandomEvaluator::actualEvaluate(bool maximize, size_t i){
  return ranks[i];
}
