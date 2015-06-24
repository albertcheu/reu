#pragma once
#include <time.h>
#include <iostream>
#include <vector>
#include <string>

class Board{
 protected:
  //The actual board on which we play
  std::vector<char> grid;
  std::vector<int> empties;
  //the size of the arithmetic progression and of the board
  size_t k, n;

  //how many turns passed
  size_t numTurns();

 public:
  Board(size_t n, size_t k);
  size_t size();
  void print();

  //Put the player in this loc, return false if already occupied
  bool play(char c, int loc);

  //Return who won ('R' or 'B'); if no winner, return '.'
  char winner();

  //See if the player using char c has won
  bool won(char c);

  //Check if neither has won
  bool noWinner();
  
  //Check if there's no more plays left
  bool filled();

};
