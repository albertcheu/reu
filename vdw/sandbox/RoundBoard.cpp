#include "RoundBoard.h"

RoundBoard::RoundBoard(char n, char k)
  :Board(n,k)
{}

void RoundBoard::shiftGrid(){
  char x = grid[0];
  for(char i = 0; i < n-1; i++){
    grid[i] = grid[i+1];
  }
  grid[n-1] = x;
}

bool RoundBoard::memberOfAP(char loc){
  return winner() == grid[loc];
  /*
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
  */
  /*
  for(char d = 1; d <= (n-1)/(k-1); d++){

    //if ((loc+d)%n > limit || (loc-d)%n < limit) { return false; }

    int numLeft, numRight; numLeft = numRight = 0;

    bool rising = true;
    for(char i = (loc+d)%n; i <= limit; i = (i+d)%n){


      if (grid[i] == grid[loc]) {
	numRight++;
      }
      else { break; }
    }

    if (1+numRight >= k) { return true; }
    
    for(char i = (loc-d)%n; i >= limit; i = (i-d)%n){
      if (i== limit && touchedLimit) { break; }
      if (grid[i] == grid[loc]) { numLeft++; }
      else { break; }
    }

    if (numLeft + numRight + 1 >= k) { return true; }
    
  }
  return false; 
  */
}

char RoundBoard::winner(){

  for(char i = 0; i < n; i++){
    //There can't be a winner using this spot if it is empty
    if (grid[i] == '.') { continue; }

    //Otherwise, iterate thru d's
    for(char d = 1; d <= (n-1)/(k-1); d++){

      bool homogenous = true;
      for(int j = 0; j < k-1; j++){
	char a = (i+(j*d)) % n;
	char b = (i+((j+1)*d)) % n;
	if (grid[a] != grid[b]) {
	  homogenous = false;
	  break;
	}
      }

      if (homogenous) { return grid[i]; }

    }

  }

  return '.';
}
