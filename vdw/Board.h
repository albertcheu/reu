//Made by Albert Cheu, 5/3/15
#include <iostream>
#include <vector>

#define R_WIN 1
#define DRAW 0
#define B_WIN -1

// the score (win, draw, lose) and the location of play that yields it
typedef std::pair<int,int> scoreAndLoc;

//"Useful" (dummy) constants
const scoreAndLoc r_win(R_WIN,R_WIN);
const scoreAndLoc b_win(B_WIN,B_WIN);
const scoreAndLoc draw(DRAW,DRAW);

class Board{
 private:
  //The actual board on which we play
  std::vector<char> grid;

  //the size of the arithmetic progression and of the board
  size_t k, n;

  //first element is whether the board is empty, second is true iff symmetric
  std::pair<bool,bool> speedyCheck();

  //for killer heuristic
  std::vector<std::pair<size_t,size_t> > killers;

  //Given a candidate spot i at some game depth, test the move
  //If it is better than what we have seen, update max/min
  //Return whether or not we can stop searching (alpha >= beta)
  bool alphabeta_helper(size_t i, bool maximize, size_t depth,
			int& max, int& min, int& loc,
			int& alpha, int& beta);

 public:
  Board(size_t n, size_t k);
  size_t size();
  void print();

  //Put the player in this loc, return false if already occupied
  bool play(char c, int loc);

  //See if the player using char c has won
  bool won(char c);

  //Check if neither has won
  bool noWinner();
  
  //Check if there's no more plays left
  bool filled();

  //how many turns passed
  size_t numTurns();

  //The meat of the program
  scoreAndLoc minimax(bool maximize);
  scoreAndLoc alphabeta(bool maximize, int alpha, int beta, size_t depth=0);

};
