#include "GreedyEvaluator.h"

GreedyEvaluator::GreedyEvaluator(size_t n, size_t k)
  :Evaluator(n,k)
{
  for(size_t i = 0; i < n; i++){
    possibleR.push_back(KapSet());
    possibleB.push_back(KapSet());
  }

  size_t maxD = (n-1)/(k-1);
  for(size_t i = 0; i < n; i++){
    for(size_t d = 1; d <= maxD; d++){

      if (i + d*(k-1) >= n) { continue; }

      for (size_t j = 0; j < k; j++){
	possibleR[i+j*d].first.emplace(kaps.size());
	possibleB[i+j*d].first.emplace(kaps.size());
      }
      kaps.push_back({i,d});

    }

  }

}

void GreedyEvaluator::removeFromOpponent(bool maximize, int loc){
  vector<KapSet>& possibles = (maximize?possibleB:possibleR);
  vector<size_t>& stack = (maximize?stackB:stackR);

  stack.push_back(possibles[loc].first.size());

  for(unordered_set<size_t>::iterator itr = possibles[loc].first.begin();
      itr!= possibles[loc].first.end(); itr++){

    for (size_t i = 0; i < n; i++){
      if (i == loc) { continue; }
      if (possibles[i].first.find(*itr) != possibles[i].first.end()) {
	possibles[i].first.erase(*itr);
      }
    }
  }

  for(unordered_set<size_t>::iterator itr = possibles[loc].first.begin();
      itr!= possibles[loc].first.end(); itr++){
    possibles[loc].second.push_back(*itr);
  }
  possibles[loc].first.clear();
}

void GreedyEvaluator::restoreToOpponent(bool maximize, int loc){
  vector<KapSet >& possibles = (maximize?possibleB:possibleR);
  vector<size_t>& stack = (maximize?stackB:stackR);

  size_t numRestore = stack.back();
  stack.pop_back();

  for(size_t i = 0; i < numRestore; i++){
    size_t index = possibles[loc].second.back();
    possibles[loc].second.pop_back();
    for(size_t j = 0; j < k; j++){
      size_t x = kaps[index].first + j*kaps[index].second;
      possibles[x].first.emplace(index);
    }
  }

}

void GreedyEvaluator::place(bool maximize, size_t i){
  grid[i] = (maximize?'R':'B');
  removeFromOpponent(maximize, i);
}

void GreedyEvaluator::undo(bool maximize, size_t i){
  grid[i] = '.';
  restoreToOpponent(maximize, i);
}

size_t GreedyEvaluator::actualEvaluate(bool maximize, size_t i){
  return (maximize?possibleR[i].first.size():possibleB[i].first.size());
}
