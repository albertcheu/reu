#include "Board_AB.h"
#include <unordered_set>

struct Kap{
  const size_t lmm;
  const size_t d;
};

class BoardGreedy:public Board_AB{
 public:
  BoardGreedy(size_t n, size_t k);
  
  size_t decide();

  bool play(char c, int loc);

 private:
  vector<unordered_set<Kap> > possibles;

  //bool contains(const Kap& kap, size_t y);

  char whodWin(size_t loc, bool redPlayer);
};
