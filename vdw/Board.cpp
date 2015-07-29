#include "Board.h"

bool memberHelper(vector<char> & grid, char& w,
		  char n, char k, char d, char loc){
  if (w != '.') { return true; }

  if (loc+d >= n && loc-d < 0) { return false; }

  int numLeft, numRight;
  numLeft = numRight = 0;
    
  for(int i = loc+d; i < n; i += d){
    if (grid[i] == grid[loc]) { numRight++; }
    else { break; }
  }

  if (1+numRight >= k) { w = grid[loc]; return true; }//true
    
  for(int i = loc-d; i > -1; i -= d){
    if (grid[i] == grid[loc]) { numLeft++; }
    else { break; }
  }

  if (numLeft + numRight + 1 >= k) { w = grid[loc]; return true; }

  return true;

}

Board::Board(char n, char k)
  :n(n), k(k)
{
  for(char i = 0; i < n; i++){ grid.push_back('.'); }
}

char Board::size(){return n;}

bool Board::memberOfAP(char loc){
  for(int d = 1; d <= (n-1)/(k-1); d++){

    if (loc+d >= n && loc-d < 0) { return false; }

    int numLeft, numRight;
    numLeft = numRight = 0;
    
    for(int i = loc+d; i < n; i += d){
      if (grid[i] == grid[loc]) { numRight++; }
      else { break; }
    }

    if (1+numRight >= k) { return true; }
    
    for(int i = loc-d; i > -1; i -= d){
      if (grid[i] == grid[loc]) { numLeft++; }
      else { break; }
    }

    if (numLeft + numRight + 1 >= k) { return true; }
    
  }
  return false; 

}

char Board::winner(){
  for(int i = 0; i <= n-k; i++){
    //There can't be a winner using this spot if it is empty
    if (grid[i] == '.') { continue; }

    //Otherwise, iterate thru d's
    int d = 1;
    while (true){
      int rightmost = i + (k-1)*d;
      if (rightmost >= n) { break; }

      bool homogenous = true;
      for(int j = 0; j < k-1; j++){
	if (grid[i+(j*d)] != grid[i+((j+1)*d)]) {
	  homogenous = false;
	  break;
	}
      }
      if (homogenous) { return grid[i]; }

      d++;
    }

    if (d == 1) { break; }

  }

 return '.';  
}

bool Board::won(char c){ return winner() == c; }

bool Board::noWinner(){ return winner() == '.'; }

void Board::print(){
  for(int i = 0; i < n; i++){ cout << grid[i]; }
  cout << endl;
}

bool Board::play(char c, char loc){
  if (grid[loc] == '.'){

    grid[loc] = c;
    return true;
  }

  return false;
}
