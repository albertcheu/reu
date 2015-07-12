#include "Board_AB.h"
#include <unordered_set>
#include <algorithm>

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

  scoreAndLoc alphabeta(bool maximize, int alpha, int beta,
                        size_t depth, int justPlayed=-1);

 private:
  vector<unordered_set<Kap,pair_hash> > possibleR, possibleB;

  char whodWin(size_t loc, bool redPlayer);

  bool alphabeta_helper(size_t i, bool maximize, size_t depth,
			int& max, int& min, int& loc,
			int& alpha, int& beta);

  unordered_set<Kap,pair_hash> removeFromOpponent(bool maximize, int loc);

  void restoreToOpponent(bool maximize, unordered_set<Kap,pair_hash>& kaps);
};
