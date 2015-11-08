#include "Board.h"

Board::Board(num n, num k)
  :n(n), k(k)
{
  for(num i = 0; i < n; i++){ grid.push_back('.'); }
}

num Board::size(){return n;}

bool Board::symmetric(){
  num middle = (n-1)/2;
  for (num i = 0; i < n; i++){
    if (i <= middle && grid[i] != grid[n-1-i]) { return false; }
  }
  return true;
}


bool Board::memberOfAP(num loc){ return memberOfAP(loc, (n-1)/(k-1)); }
bool Board::memberOfAP(num loc, num limit, bool dLimit){
  int maxD = dLimit?limit:(n-1)/(k-1);
  for(int d = 1; d <= maxD; d++){

    if (loc+d >= n && loc < d) { return false; }

    int numLeft, numRight;
    numLeft = numRight = 0;
    
    for(int i = loc+d; i < n; i += d){
      if (!dLimit && i >= n-limit) { break; }
      else if (grid[i] == grid[loc]) { numRight++; }
      else { break; }
    }
    
    if (1+numRight >= k) { return true; }
    
    for(int i = ((int)loc)-d; i > -1; i -= d){
      if (!dLimit && i < limit) { break; }
      else if (grid[i] == grid[loc]) { numLeft++; }
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

bool Board::play(char c, num loc){
  if (grid[loc] == '.'){

    grid[loc] = c;
    return true;
  }

  return false;
}
