#include "MCEvaluator.h"

MCEvaluator::MCEvaluator(num n, num k)
  :Evaluator(n,k), b(0), depth(0)
{
  //load data from file "game(n,k).leaves"
  char buf[32];
  sprintf(buf,"game(%d,%d).leaves",n,k);
  ifstream ifs(buf);
  string line;
  while(ifs){
    ifs >> line;
    if (line.size() < BITSETSIZE) {
      data.push_back(vector<bitset<BITSETSIZE> >());
    }
    else{
      data[data.size()-1].push_back(bitset<BITSETSIZE>(line));
    }
  }
  ifs.close();
  cout << "Obtained data for the lower " << data.size() << " depths" << endl;
}

void MCEvaluator::place(bool maximize, num i){
  grid[i] = (maximize?'R':'B');
  b[2*i + (maximize?1:0)] = 1;
  depth++;
}

void MCEvaluator::undo(bool maximize, num i){
  grid[i] = '.';
  b[2*i + (maximize?1:0)] = 0;
  depth--;
}

float MCEvaluator::actualEvaluate(bool maximize, num i){
  float numWins = 0;
  float total = 0;

  place(maximize,i);
  //the bitset b is now changed
  if (memberOfAP(i)) { undo(maximize,i); return 1; }

  //Find endstates that are descendants of b
  //Return ratio of {wins for maximize? player} to {total descendants}

  num start = (depth+1);
  if (start < 2*k-1) { start = 2*k-1; }

  for(num j = start-(2*k-1); j < data.size(); j++){    
    for(bitset<BITSETSIZE>& node: data[j]){
      //Is node a descendant of b? every 1 in b should be in node
      if ((b | node) == node) {
	total++;
	if (j+(2*k-1) % 2 == maximize) { numWins++; }
      }
    }

  }
  undo(maximize,i);

  return numWins / total;
}
