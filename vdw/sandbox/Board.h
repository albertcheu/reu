#pragma once
#include <random>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <iostream>
#include <vector>
#include <string>
#include <omp.h>

//#include <thread>

#define R_WIN 1
#define DRAW 0
#define B_WIN -1

using namespace std;

bool memberHelper(vector<char>  & grid, char& w,
		  size_t n, size_t k, char d, char loc);

class Board{
 protected:
  //The actual board on which we play
  vector<char> grid;

  //the size of the arithmetic progression and of the board
  size_t k, n;

  bool memberOfAP(char loc);

 public:
  Board(size_t n, size_t k);
  size_t size();
  void print();

  //Put the player in this loc, return false if already occupied
  virtual bool play(char c, char loc);

  //Return who won ('R' or 'B'); if no winner, return '.'
  char winner();

  //See if the player using char c has won
  bool won(char c);

  //Check if neither has won
  bool noWinner();

};
