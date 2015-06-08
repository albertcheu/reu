//Made by Albert Cheu, 5/3/15
#include "Board.h"

Board::Board(size_t size)
  :size(size)
{
  for(int i = 0; i < size; i++){
    grid.push_back(std::vector<char>());
    for(int j = 0; j < size; j++){
      int numberDisplayed = i*size + j + 1;
      char c = (char)(48 + numberDisplayed);//48 is the ascii val of '0'
      grid[i].push_back(c);
    }
  }
}

bool Board::won(char c){
  bool diag1, diag2, rows, cols;
  diag1 = diag2 = rows = cols = true;
  for(int i = 0; i < size; i++){
    //Diagonals
    if (grid[i][i] != c) { diag1 = false; }
    if (grid[i][size-i-1] != c) { diag2 = false; }

    bool currentRow = true;
    bool currentCol = true;
    for(int j = 0; j < size; j++){
      //Row i
      if (grid[i][j] != c) {currentRow = false;}
      //Column i
      if (grid[j][i] != c) {currentCol = false;}
      //Shortcircuit
      if (!currentRow && !currentCol) { break; }
    }
    rows = ((i==0)?currentRow:(rows || currentRow));
    cols = ((i==0)?currentCol:(cols || currentCol));
  }

  return diag1 || diag2 || rows || cols;
}

bool Board::noWinner(){ return !(won('X') || won('O')); }

int Board::findOpen(){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      if (grid[i][j] != 'X' && grid[i][j] != 'O') { return i*size + j; }
    }
  }
  return -1;
}

bool Board::noSpace(){
  return findOpen() == -1;
}

void Board::print(){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){ std::cout << grid[i][j]; }
    std::cout << std::endl;
  }
}

bool Board::play(char c, int loc){
  int row = loc/size;
  int col = loc%size;
  if (grid[row][col] != 'X' && grid[row][col] != 'O'){
    grid[row][col] = c;
    return true;
  }
  return false;
}

scoreAndLoc Board::minimax(bool maximize){
  //Leaf
  if (won('X')) { return x_win; }
  if (won('O')) { return o_win; }
  if (noSpace()) { return draw; }

  //Internal node
  int max = -10;
  int min = 10;
  int loc = -1;

  for(int i = 0; i < size*size; i++){
    int row = i/size;
    int col = i%size;
    if (grid[row][col] != 'X' && grid[row][col] != 'O') {
      char old = grid[row][col];
      grid[row][col] = (maximize?'X':'O');
      scoreAndLoc p = minimax(!maximize);
      grid[row][col] = old;

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
  if (won('X')) { return x_win; }
  if (won('O')) { return o_win; }
  if (noSpace()) { return draw; }

  //Internal node
  int max = -10;
  int min = 10;
  int loc = -1;

  for(int i = 0; i < size*size; i++){
    int row = i/size;
    int col = i%size;
    if (grid[row][col] != 'X' && grid[row][col] != 'O') {

      char old = grid[row][col];
      grid[row][col] = (maximize?'X':'O');
      scoreAndLoc p = (maximize? alphabeta(false, max, beta)
		       : alphabeta(true,alpha,min));
      grid[row][col] = old;

      if (maximize && p.first > max){
	max = p.first;
	loc = i;
	alpha = (alpha>max?alpha:max);
      }

      else if(!maximize && p.first < min){
	min = p.first;
	loc = i;
	beta = (beta<min?beta:min);
      }

      if (alpha >= beta) { break; }
    }
  }

  return scoreAndLoc(maximize?max:min, loc); 
}
