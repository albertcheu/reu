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

      Kap kap; kap.first=i; kap.second=d;
      for (size_t j = 0; j < k; j++){
	possibleR[i+j*d].emplace(kap);
	possibleB[i+j*d].emplace(kap);
      }

    }

  }

}

void GreedyEvaluator::removeFromOpponent(bool maximize, int loc){
  vector<KapSet>& possibles = (maximize?possibleB:possibleR);

  stack.push_back(possibles[loc]);

  possibles[loc].clear();

  for(KapSet::iterator itr = stack.back().begin();
      itr!= stack.back().end(); itr++){

    for (size_t i = 0; i < n; i++){
      if (i == loc) { continue; }

      if (possibles[i].find(*itr) != possibles[i].end()) 
	{ possibles[i].erase(*itr); }
    }
  }

}

void GreedyEvaluator::restoreToOpponent(bool maximize){

  vector<KapSet >& possibles = (maximize?possibleB:possibleR);

  for(KapSet::iterator itr = stack.back().begin();
      itr != stack.back().end(); itr++){
    Kap kap = *itr;
    for (int elem = 0; elem < k; elem++){
      possibles[kap.first + elem*kap.second].emplace(kap);
    }
  }

  stack.pop_back();

}

void GreedyEvaluator::place(bool maximize, size_t i){
  grid[i] = (maximize?'R':'B');
  removeFromOpponent(maximize, i);
}

void GreedyEvaluator::undo(bool maximize, size_t i){
  grid[i] = '.';
  restoreToOpponent(maximize);
}

size_t GreedyEvaluator::actualEvaluate(bool maximize, size_t i){
  return (maximize?possibleR[i].size():possibleB[i].size());
}
