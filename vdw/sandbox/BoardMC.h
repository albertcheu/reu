#include <cmath>
#include <cfloat>
#include <algorithm>
#include <unordered_set>
#include "Board.h"

#define STORE_DEPTH 4

struct State{
  State(int depth, int loc, bool redPlayer, State* parent);

  int depth, loc, numTrials;
  int redWins, blueWins;
  bool redPlayer;
  State* parent;
  std::vector<State*> children;
};



class BoardMC: public Board{
 private:
  //Unlike alpha-beta, our list of available moves doesnt need to be ordered
  std::unordered_set<int> moves;

  std::vector<int> indices;

  State* start;

  //char memberOfAP(int loc, bool redPlayer);
  bool memberOfAP_played(int loc);

  int buildTree(State* s);

  int freeRecursive(State* s);

  bool runTrial(State* s);

  float score(State* s);
  
 public:
  BoardMC(size_t n, size_t k);
  ~BoardMC();

  void montecarlo();
};
