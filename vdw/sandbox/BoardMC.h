
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include "Board.h"

#define STORE_DEPTH

struct State{
  int depth, loc, numTrials, successful;
  bool redPlayer;
  State* parent;
  std::vector<State*> children;
};

float score(State s);

class Board_MC: public Board{
 private:
  //Unlike alpha-beta, we don't care if our list of available moves is ordeed
  std::unordered_set<int> moves;

  void runTrial();
  //char memberOfAP(int loc, bool redPlayer);
  bool memberOfAP_played(int loc);
  
 public:
  Board_MC(size_t n, size_t k);

  std::vector<State> firstMoves;

  void montecarlo();
};
