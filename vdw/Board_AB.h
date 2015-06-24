//#pragma once
#include "Board.h"

#define R_WIN 1
#define DRAW 0
#define B_WIN -1

// the score (win, draw, lose) and the location of play that yields it
typedef std::pair<int,int> scoreAndLoc;

//"Useful" (dummy) constants
const scoreAndLoc r_win(R_WIN,R_WIN);
const scoreAndLoc b_win(B_WIN,B_WIN);
const scoreAndLoc draw(DRAW,DRAW);

class Board_AB: public Board {
 private:

  bool symmetric();

  //for killer heuristic
  std::vector<std::pair<size_t,size_t> > killers;

  //Given a candidate spot i at some game depth, test the move
  //If it is better than what we have seen, update max/min
  //Return whether or not we can stop searching (alpha >= beta)
  bool alphabeta_helper(size_t i, bool maximize, size_t depth,
			int& max, int& min, int& loc,
			int& alpha, int& beta);

 public:
  Board_AB(size_t n, size_t k);

  //The meat of the program
  scoreAndLoc alphabeta(bool maximize, int alpha, int beta,
			size_t depth);

};
