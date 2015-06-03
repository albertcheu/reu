//Made by Albert Cheu, 5/3/15
#include "Board.h"

Board::Board(){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      int numberDisplayed = i*3 + j + 1;
      grid[i][j] = (char)(48 + numberDisplayed);//48 is the ascii val of '0'
    }
  }
}

bool Board::won(char c){
  for(int i = 0; i < 3; i++){
    //Rows
    if (grid[i][0] == c && grid[i][1] == c && grid[i][2] == c ) { return true; }
    //Cols
    if (grid[0][i] == c && grid[1][i] == c && grid[2][i] == c ) { return true; }
  }

  //Diagonals
  if (grid[0][0] == c && grid[1][1] == c && grid[2][2] == c ) { return true; }
  if (grid[0][2] == c && grid[1][1] == c && grid[2][0] == c ) { return true; }

  return false;
}

bool Board::noWinner(){ return !(won('X') || won('O')); }

int Board::findOpen(){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      if (grid[i][j] != 'X' && grid[i][j] != 'O') { return i*3 + j + 1; }
    }
  }
  return -1;
}

bool Board::noSpace(){
  return findOpen() == -1;
}

void Board::print(){
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){ std::cout << grid[i][j]; }
    std::cout << std::endl;
  }
}

bool Board::play(char c, int loc){
  int row = (loc-1)/3;
  int col = (loc-1)%3;
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

  for(int i = 0; i < 9; i++){
    int row = i/3;
    int col = i%3;
    if (grid[row][col] != 'X' && grid[row][col] != 'O') {
      char old = grid[row][col];
      grid[row][col] = (maximize?'X':'O');
      scoreAndLoc p = minimax(!maximize);
      grid[row][col] = old;

      if (maximize && p.first > max) {
	max = p.first;
	loc = i+1;
      }

      else if (!maximize && p.first < min){
	min = p.first;
	loc = i+1;
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

  for(int i = 0; i < 9; i++){
    int row = i/3;
    int col = i%3;
    if (grid[row][col] != 'X' && grid[row][col] != 'O') {

      char old = grid[row][col];
      grid[row][col] = (maximize?'X':'O');
      scoreAndLoc p = (maximize? alphabeta(false, max, beta)
		       : alphabeta(true,alpha,min));
      grid[row][col] = old;

      if (maximize){
	if (p.first > max){
	  max = p.first;
	  loc = i+1;
	}

	alpha = (alpha>max?alpha:max);
	if (alpha >= beta) { break; }
      }

      else{
	if(p.first < min){
	  min = p.first;
	  loc = i+1;
	}
	beta = (beta<min?beta:min);
	if (beta <= alpha) { break; }	
      }
    }
  }

  return scoreAndLoc(maximize?max:min, loc); 
}
