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

  //the size of the arithmetic progression
  size_t k;

  int findOpen();

 public:
  Board(size_t boardSize, size_t k);
  size_t size();
  void print();

  //Put the player in this loc, return false if already occupied
  bool play(char c, int loc);

  //See if the player using char c has won
  bool won(char c);

  //Check if neither has won
  bool noWinner();
  
  //Check if there's no more plays left
  bool noSpace();

  //The meat of the program
  scoreAndLoc minimax(bool maximize);
  scoreAndLoc alphabeta(bool maximize, int alpha, int beta);

};
