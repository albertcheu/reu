#pragma once
#include <random>
#include <cstdlib>
#include <climits>
#include <ctime>

#include <iostream>
#include <fstream>
//#include <sstream>

#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include <bitset>

#define R_WIN 1
#define DRAW 0
#define B_WIN -1

using namespace std;
typedef unsigned int num;
const num BITSETSIZE = 256;

class Board{
 protected:
  //The actual board on which we play
  vector<char> grid;

  //the size of the arithmetic progression and of the board
  num k, n;

  bool symmetric();
  bool memberOfAP(num loc);
  bool memberOfAP(num loc, num limit);

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
