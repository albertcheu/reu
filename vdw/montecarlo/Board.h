//Made by Albert Cheu, 5/3/15
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdlib>

#define MC_TRIALS 3000

struct Record{
  int redWins,blueWins,numDraws;
} ;

class Board{
 private:
  //The actual board on which we play
  std::vector<char> grid;

  std::vector<int> empties;

  //the size of the arithmetic progression and of the board
  size_t k, n;

  //bool symmetric();
  //bool symmetric;

  //how many turns passed
  size_t numTurns();

  Record runTrials(bool redPlayer, int numTrials,
		   std::vector<int>& indices,
		   const std::vector<int>& empties, int i);

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

  int montecarlo(bool redPlayer, int numTrials);
};
