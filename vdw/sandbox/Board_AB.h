#pragma once
#include "Board.h"

#define EXACT 0
#define LOWER 1
#define UPPER 2

//64 bit number
typedef unsigned long long int Bitstring;
const unsigned int GAMESTATE = 55;
const unsigned int METADATA = 9;

//32 bit number
typedef unsigned long int BitstringKey;
const BitstringKey MAXKEY = ULONG_MAX;

typedef list<Bitstring> Chain;

// the score (win, draw, lose) and the location of play that yields it
typedef pair<char,num> scoreAndLoc;

//"Useful" (dummy) constants
const scoreAndLoc r_win(R_WIN,R_WIN);
const scoreAndLoc b_win(B_WIN,B_WIN);
const scoreAndLoc draw(DRAW,DRAW);

class Board_AB: public Board {

protected:
  num bound;

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

  //Given a candidate spot i at some game depth, test the move
  //If it is better than what we have seen, update max/min
  //Return whether or not we can stop searching (alpha >= beta)
  
  bool alphabeta_helper(num i, bool maximize, num depth,
			char& score, num& loc,
			char& alpha, char& beta,
			bool& firstChild
			);

  //If there is no entry in the table, return false
  //Otherwise, extract (loc/flag/score/gamestate)
  //Update values
  //Return true if flag == exact, false otherwise
  bool retrieve(BitstringKey key, Bitstring gs,
		char& score, num& loc, char& alpha, char& beta);
  bool retrieveSmart(char& score, num& loc, char& alpha, char& beta);
  
  void store(BitstringKey key, Bitstring gs,
	     char score, num loc, char alphaOrig, char beta);
  void storeSmart(char score, num loc, char alphaOrig, char beta);
  
  bool withinBound(num i); 

 public:
  Board_AB(num n, num k, num bound);

  bool play(char c, num loc);

  virtual scoreAndLoc alphabeta(bool maximize, char alpha, char beta,
				num depth, num x=255);

  scoreAndLoc mtd(int f);
};
