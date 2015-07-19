#include "Evaluator.h"

struct State{
  State(unsigned int loc, bool redPlayer);

  unsigned int loc, numTrials, redWins, blueWins;
  bool redPlayer;
};


class MCEvaluator: public Evaluator{
 public:
  MCEvaluator(size_t n, size_t k);
  void place(bool maximize, size_t i);
  void undo(bool maximize, size_t i);
  bool play(char c, int loc);

 private:
  unsigned int numTrials, redWins;
  vector<State> tree;
  vector<size_t> colored, order;
  unordered_set<size_t> moves;

  float score(State& s);

  void runTrial();

  bool runTrialRandom(State* s);

  size_t actualEvaluate(bool maximize, size_t i);
};
