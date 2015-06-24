#include "BoardMC.h"

using namespace std;

float score(State s){
  float ans = s.successful / (float) s.numTrials;
  return (s.redPlayer?ans:-ans);
}

int main(){
  return 0;
}
