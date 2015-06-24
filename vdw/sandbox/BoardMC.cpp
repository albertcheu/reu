#include "BoardMC.h"

using namespace std;

State::State(

float score(State s){
  float ans = s.successful / (float) s.numTrials;
  return (s.redPlayer?ans:-ans);
}

BoardMC::BoardMC(size_t n, size_t k)
  :Board(n,k), c(0)
{

  for(int i = 0; i < n; i++){
    moves.emplace(i);
    startStates.push_back(State(1,i,0,0,false,NULL));
  }
  
  //Build tree up to STORE_DEPTH
  
  for(int i = 0; i < n; i++){
    std::unordered_set<std::int>::iterator got = moves.find(i);
    moves.erase(got);
    buildTree(&(startStates[i]));
    moves.emplace(i);
  }
}

void BoardMC::buildTree(State* s){
  int depth = s->depth;
  //fill children
  for(int i = 0; i < n; i++){
    std::unordered_set<std::int>::iterator itr = moves.find(i);
    if (itr == moves.end()) { continue; }

    State* child = new State(depth+1,i,0,0,!(s->redPlayer),s);
    s->children.push_back(child);

    //recurse if depth hasn't been reached yet
    if (depth+1 < STORE_DEPTH) {
      moves.erase(itr);
      buildTree(child);
      moves.emplace(i);
    }

  }  

}

BoardMC::~BoardMC(){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n-1; j++){
      freeRecursive(startStates[i].children[j]);
    }  
  }
  startStates.clear();
}

void BoardMC::freeRecursive(State* s){
  if (s->depth < STORE_DEPTH){
    for(int i = 0; i < s->children.size(); i++)
      { freeRecursive(s->children[i]); }
  }
  free(s);
}

void BoardMC::montecarlo(){
  
}

int main(){
  return 0;
}
