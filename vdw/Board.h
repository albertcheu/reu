#pragma once
#include <random>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>

#define R_WIN 1
#define DRAW 0
#define B_WIN -1

using namespace std;
typedef size_t num;

class Board{
 protected:
  //The actual board on which we play
  vector<char> grid;

  //the size of the arithmetic progression and of the board
  num k, n;

  bool memberOfAP(num loc);

 public:
  Board(num n, num k);
  num size();
  void print();

  //Put the player in this loc, return false if already occupied
  virtual bool play(char c, num loc);

  //Return who won ('R' or 'B'); if no winner, return '.'
  char winner();

  //See if the player using char c has won
  bool won(char c);

  //Check if neither has won
  bool noWinner();

};
