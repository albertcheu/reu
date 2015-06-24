#include "BoardMC.h"

using namespace std;

State::State(int depth, int loc, int numTrials, int successful,
	     bool redPlayer, State* parent)
  :depth(depth), loc(loc), numTrials(numTrials), successful(successful),
   redPlayer(redPlayer), parent(parent)
{}

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
  int ans = 0;
  for(int i = 0; i < n; i++){
    std::unordered_set<int>::iterator itr = moves.find(i);
    moves.erase(itr);
    ans += buildTree(&(startStates[i]));
    moves.emplace(i);
  }
  std::cout << ans << endl; 
}

int BoardMC::buildTree(State* s){
  int ans = 1;//include s in the count

  int depth = s->depth;
  //fill children
  for(int i = 0; i < n; i++){
    std::unordered_set<int>::iterator itr = moves.find(i);
    if (itr == moves.end()) { continue; }

    State* child = new State(depth+1,i,0,0,!(s->redPlayer),s);
    s->children.push_back(child);

    //recurse if depth hasn't been reached yet
    if (depth+1 < STORE_DEPTH) {
      moves.erase(itr);
      ans += buildTree(child);
      moves.emplace(i);
    }

    else { ans += 1; }
  }

  return ans;
}

BoardMC::~BoardMC(){

  int ans = 0;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n-1; j++){
      ans += freeRecursive(startStates[i].children[j]);
    }  
  }

  std::cout << ans << endl;
  startStates.clear();
}

int BoardMC::freeRecursive(State* s){
  int ans = 1;
  if (s->depth < STORE_DEPTH){
    for(int i = 0; i < s->children.size(); i++){
      ans += freeRecursive(s->children[i]);
    }
  }
  free(s);
  return ans;
}

void BoardMC::montecarlo(){
  
}

int main(){
  BoardMC bmc(14,4);
  
  return 0;
}
