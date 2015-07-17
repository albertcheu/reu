#include "GreedyEvaluator.h"

GreedyEvaluator::GreedyEvaluator(size_t n, size_t k)
  :Evaluator(n,k)
{

}


char GreedyEvaluator::whodWin(size_t loc, bool redPlayer){

}

unordered_set<Kap,pair_hash> GreedyEvaluator::removeFromOpponent(bool maximize,
								 int loc){

}

void GreedyEvaluator::restoreToOpponent(bool maximize,
					unordered_set<Kap,pair_hash>& kaps){

}

bool GreedyEvaluator::play(char c, int loc){
  
}

void GreedyEvaluator::actualEvaluate(size_t i){

}
