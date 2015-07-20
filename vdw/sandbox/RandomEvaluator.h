#include "Evaluator.h"

class RandomEvaluator: public Evaluator{
 public:
  RandomEvaluator(size_t n, size_t k);
  size_t actualEvaluate(bool maximize, size_t i);
  void place(bool maximize, size_t i);
  void undo(bool maximize, size_t i);

 private:
  vector<size_t> ranks;
};
