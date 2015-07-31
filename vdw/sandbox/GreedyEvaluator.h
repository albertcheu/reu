#include "Evaluator.h"

struct pair_hash {
    inline std::size_t operator()(const std::pair<size_t,num> & v) const {
        return v.first*31+v.second;
    }
};

typedef pair<num,num> Kap;
typedef pair<unordered_set<num>,vector<size_t> > KapSet;

class GreedyEvaluator: public Evaluator{
 public:
  GreedyEvaluator(num n, num k);
  void place(bool maximize, num i);
  void undo(bool maximize, num i);

 private:
  vector<Kap> kaps;
  vector<KapSet> possibleR, possibleB;
  vector<size_t> stackR, stackB;

  size_t actualEvaluate(bool maximize, num i);

  void removeFromOpponent(bool maximize, num loc);

  void restoreToOpponent(bool maximize, num loc);
};
