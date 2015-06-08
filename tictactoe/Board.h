//Made by Albert Cheu, 5/3/15
#include <iostream>
#include <vector>

#define X_WIN 1
#define DRAW 0
#define O_WIN -1

// the score (win, draw, lose) and the location of play [0,n-1] that yields it
typedef std::pair<int,int> scoreAndLoc;

//"Useful" (dummy) constants
const scoreAndLoc x_win(X_WIN,X_WIN);
const scoreAndLoc o_win(O_WIN,O_WIN);
const scoreAndLoc draw(DRAW,DRAW);

class Board{
 private:
  //The grid with which we play on/with
  std::vector<std::vector<char> > grid;

  int findOpen();
  size_t size;

 public:
  Board(size_t size=3);
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
