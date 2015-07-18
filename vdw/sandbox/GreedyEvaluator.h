#include "Evaluator.h"
#include <unordered_set>

struct pair_hash {
    inline std::size_t operator()(const std::pair<size_t,size_t> & v) const {
        return v.first*31+v.second;
    }
};

typedef pair<size_t,size_t> Kap;
typedef unordered_set<size_t> KapSet;

class GreedyEvaluator: public Evaluator{
 public:
  GreedyEvaluator(size_t n, size_t k);
  void place(bool maximize, size_t i);
  void undo(bool maximize, size_t i);

 private:
  vector<Kap> kaps;
  vector<KapSet> possibleR, possibleB, stack;

  size_t actualEvaluate(bool maximize, size_t i);


  void removeFromOpponent(bool maximize, int loc);

  void restoreToOpponent(bool maximize);
};
