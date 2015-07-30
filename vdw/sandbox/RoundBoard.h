#pragma once
#include "Board.h"
#include <algorithm>
#include <unordered_set>
class RoundBoard: public Board{
 protected:
  bool memberOfAP(num loc);
  void shiftGrid();
  vector<vector<num> > checker;

 public:
  RoundBoard(num n, num k);

  char winner();
};
