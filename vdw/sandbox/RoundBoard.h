#pragma once
#include "Board.h"
#include <algorithm>
#include <unordered_set>
class RoundBoard: public Board{
 protected:
  bool memberOfAP(char loc);
  void shiftGrid();
  vector<vector<char> > checker;

 public:
  RoundBoard(char n, char k);

  char winner();
};
