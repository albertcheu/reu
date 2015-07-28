#pragma once
#include "Board.h"

class RoundBoard: public Board{
 protected:
  bool memberOfAP(char loc);
  void shiftGrid();

 public:
  RoundBoard(char n, char k);

  char winner();
};
