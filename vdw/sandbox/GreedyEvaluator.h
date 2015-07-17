#include "Evaluator.h"
#include <unordered_set>

struct pair_hash {
    inline std::size_t operator()(const std::pair<size_t,size_t> & v) const {
        return v.first*31+v.second;
    }
};

typedef pair<size_t,size_t> Kap;

class GreedyEvaluator: public Evaluator{
 public:
  GreedyEvaluator(size_t n, size_t k);
  bool play(char c, int loc);

 private:
  void actualEvaluate(size_t i);
  vector<unordered_set<Kap,pair_hash> > possibleR, possibleB;

  char whodWin(size_t loc, bool redPlayer);

  unordered_set<Kap,pair_hash> removeFromOpponent(bool maximize, int loc);

  void restoreToOpponent(bool maximize, unordered_set<Kap,pair_hash>& kaps);
};
