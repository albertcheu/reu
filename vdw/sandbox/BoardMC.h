#include <cmath>
#include <algorithm>
#include <unordered_set>
#include "Board.h"

#define STORE_DEPTH 4

struct State{
  State(int depth, int loc, int numTrials, int successful,
	bool redPlayer, State* parent);

  int depth, loc, numTrials, successful;
  bool redPlayer;
  State* parent;
  std::vector<State*> children;
};

float score(State s);

class BoardMC: public Board{
 private:
  int c;

  //Unlike alpha-beta, we don't care if our list of available moves is ordered
  std::unordered_set<int> moves;

  std::vector<State> startStates;

  void runTrial();
  //char memberOfAP(int loc, bool redPlayer);
  bool memberOfAP_played(int loc);

  int buildTree(State* s);
  
 public:
  BoardMC(size_t n, size_t k);
  ~BoardMC();
  int freeRecursive(State* s);
  void montecarlo();
};
