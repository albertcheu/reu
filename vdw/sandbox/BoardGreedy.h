#include "Board_AB.h"
#include <unordered_set>

/*
struct Kap{
  const size_t lmm;
  const size_t d;
};
*/
struct pair_hash {
    inline std::size_t operator()(const std::pair<size_t,size_t> & v) const {
        return v.first*31+v.second;
    }
};

typedef pair<size_t,size_t> Kap;

class BoardGreedy:public Board_AB{
 public:
  BoardGreedy(size_t n, size_t k);
  
  size_t decide(bool redPlayer, size_t depth);

  bool play(char c, int loc);

 private:
  vector<unordered_set<Kap,pair_hash> > possibles;

  char whodWin(size_t loc, bool redPlayer);
};
