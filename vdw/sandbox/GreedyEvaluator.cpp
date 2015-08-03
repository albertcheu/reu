#include "GreedyEvaluator.h"

GreedyEvaluator::GreedyEvaluator(num n, num k)
  :Evaluator(n,k)
{
  for(num i = 0; i < n; i++){
    possibleR.push_back(KapSet());
    possibleB.push_back(KapSet());
  }

  num maxD = (n-1)/(k-1);
  for(num i = 0; i < n; i++){
    for(num d = 1; d <= maxD; d++){

      if (i + d*(k-1) >= n) { continue; }

      for (num j = 0; j < k; j++){
	possibleR[i+j*d].first.emplace(kaps.size());
	possibleB[i+j*d].first.emplace(kaps.size());
      }
      kaps.push_back({i,d});

    }

  }

  for(num i = 0; i < n; i++){
    cout << possibleR[i].first.size() << ' ';
  }
  cout << endl;
}

void GreedyEvaluator::removeFromOpponent(bool maximize, num loc){
  vector<KapSet>& possibles = (maximize?possibleB:possibleR);
  vector<size_t>& stack = (maximize?stackB:stackR);

  stack.push_back(possibles[loc].first.size());

  for(unordered_set<num>::iterator itr = possibles[loc].first.begin();
      itr!= possibles[loc].first.end(); itr++){

    possibles[loc].second.push_back(*itr);

    for (num i = 0; i < n; i++){
      if (i == loc) { continue; }
      if (possibles[i].first.find(*itr) != possibles[i].first.end()) {
	possibles[i].first.erase(*itr);
      }
    }
  }
  possibles[loc].first.clear();
}

void GreedyEvaluator::restoreToOpponent(bool maximize, num loc){
  vector<KapSet >& possibles = (maximize?possibleB:possibleR);
  vector<size_t>& stack = (maximize?stackB:stackR);

  size_t numRestore = 0;
  try{
    numRestore = stack.back();
    stack.pop_back();
  }
  catch(int er){
    cout << "Unable to pop!" << endl;
    exit(1);
  }

  for(size_t i = 0; i < numRestore; i++){
    size_t index = possibles[loc].second.back();
    possibles[loc].second.pop_back();
    for(num j = 0; j < k; j++){
      num x = kaps[index].first + j*kaps[index].second;
      possibles[x].first.emplace(index);
    }
  }

}

void GreedyEvaluator::place(bool maximize, num i){
  grid[i] = (maximize?'R':'B');
  try{
    removeFromOpponent(maximize, i);
  }
  catch(int er){
    cout << "Failed to remove" << endl;
    removeFromOpponent(maximize, i);
  }
}

void GreedyEvaluator::undo(bool maximize, num i){
  grid[i] = '.';
  try{
    restoreToOpponent(maximize, i);
  }
  catch(int er){
    cout << "Failed to restore" << endl;
    restoreToOpponent(maximize, i);
  }
}


size_t GreedyEvaluator::actualEvaluate(bool maximize, num i){
  return (maximize?possibleR[i].first.size():possibleB[i].first.size());
}
