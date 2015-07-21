//#pragma once
#include "Board.h"
#include <unordered_map>

//64 bit number
typedef unsigned long long Bitstring;
const unsigned GAMESTATE = 55;
const unsigned METADATA = 9;

//32 bit number
typedef unsigned long BitstringKey;
const BitstringKey MAXKEY = ULONG_MAX;

typedef vector<Bitstring> Chain;

// the score (win, draw, lose) and the location of play that yields it
typedef pair<int,int> scoreAndLoc;

//"Useful" (dummy) constants
const scoreAndLoc r_win(R_WIN,R_WIN);
const scoreAndLoc b_win(B_WIN,B_WIN);
const scoreAndLoc draw(DRAW,DRAW);

class Board_AB: public Board {

protected:

  //two bits dedicated to score
  //log(n) bits dedicated to depth; n <= 32 means it is 5
  //remainder: every two bits corresponds to one position; 
  Bitstring gamestate;
  vector<pair<Bitstring,Bitstring> > assignmentG;

  vector<pair<Bitstring,Bitstring> > assignmentZ;

  //this will be hashed to get the key
  Bitstring zobrist;

  unordered_map<BitstringKey,Chain> table;

  size_t recursionCount;

  bool symmetric();

  //Given a candidate spot i at some game depth, test the move
  //If it is better than what we have seen, update max/min
  //Return whether or not we can stop searching (alpha >= beta)
  
  bool alphabeta_helper(size_t i, bool maximize, size_t depth,
			int& score, int& loc,
			int& alpha, int& beta);
  
 public:
  Board_AB(size_t n, size_t k);

  bool play(char c, int loc);

  //The meat of the program
  scoreAndLoc alphabeta(bool maximize, int alpha, int beta,
			size_t depth, int x=-1);
    
};
