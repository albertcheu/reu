//Made by Albert Cheu, 5/3/15
#include "Board.h"

Board::Board(size_t boardSize){
  for(size_t i = 0; i < boardSize; i++){
    grid.push_back((char)(48 + i + 1));//48 is the ascii val of '0'
  }
}

size_t Board::size(){return grid.size();}

bool Board::won(char c){
  for(int d = 1; d <= 3; d++){
    for(int i = 0; i <= (grid.size()-d*2)-1; i++){
      if (grid[i] == c && grid[i+d] == c && grid[i+d*2] == c) { return true; }
    }
  }
  return false;
}

bool Board::noWinner(){ return !(won('R') || won('B')); }

int Board::findOpen(){
  for(int i = 0; i < grid.size(); i++){
    if (grid[i] != 'R' && grid[i] != 'B') { return i; }
  }
  return -1;
}

bool Board::noSpace(){
  return findOpen() == -1;
}

void Board::print(){
  for(int i = 0; i < grid.size(); i++){
    std::cout << grid[i];
  }
  std::cout << std::endl;
}

bool Board::play(char c, int loc){
  if (grid[loc] != 'R' && grid[loc] != 'B'){
    grid[loc] = c;
    return true;
  }
  return false;
}

scoreAndLoc Board::minimax(bool maximize){
  //Leaf
  if (won('R')) { return r_win; }
  if (won('B')) { return b_win; }
  if (noSpace()) { return draw; }

  //Internal node
  int max = -10;
  int min = 10;
  int loc = -1;

  for(int i = 0; i < grid.size(); i++){
    if (grid[i] != 'R' && grid[i] != 'B') {
      char old = grid[i];
      grid[i] = (maximize?'R':'B');
      scoreAndLoc p = minimax(!maximize);
      grid[i] = old;

      if (maximize && p.first > max) {
	max = p.first;
	loc = i;
      }

      else if (!maximize && p.first < min){
	min = p.first;
	loc = i;
      }

    }
  }

  return scoreAndLoc((maximize?max:min), loc);
}

scoreAndLoc Board::alphabeta(bool maximize, int alpha, int beta){
  //Leaf
  if (won('R')) { return r_win; }
  if (won('B')) { return b_win; }
  if (noSpace()) { return draw; }

  //Internal node
  int max = -10;
  int min = 10;
  int loc = -1;

  for(int i = 0; i < grid.size(); i++){
    if (grid[i] != 'R' && grid[i] != 'B') {
      char old = grid[i];
      grid[i] = (maximize?'R':'B');
      scoreAndLoc p = (maximize? alphabeta(false, max, beta)
		       : alphabeta(true,alpha,min));
      grid[i] = old;

      if (maximize && p.first > max) {
	max = p.first;
	loc = i;
	alpha = (alpha>max?alpha:max);
      }

      else if (!maximize && p.first < min){
	min = p.first;
	loc = i;
	beta = (beta<min?beta:min);
      }

      if (alpha >= beta) { break; }

    }
  }

  return scoreAndLoc((maximize?max:min), loc);
}
