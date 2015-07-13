#include "Board_AB.h"
#include <mutex>
class BoardThread: public Board_AB
{
 private:
  size_t id;
  /*
  size_t& turn;
  vector<bool>& wantsToEnter;
  */
  mutex& lock;
  bool alphabeta_helper(int i, bool maximize, size_t depth,
			int& max, int& min, int& loc,
			int& alpha, int& beta);
 public:

  BoardThread(size_t n, size_t k,
	      //size_t id, size_t& turn, vector<bool>& wantsToEnter,
	      mutex& lock, size_t id,
	      vector<pair<Bitstring,Bitstring> >& assignmentX,
	      unordered_map<BitstringKey,pair<Bitstring,int> >& table);

  void fillTable(scoreAndLoc& sal);
  
  scoreAndLoc alphabeta(bool maximize, int alpha, int beta,
			size_t depth, int justPlayed=-1);
};
