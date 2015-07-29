#include "RoundBoard.h"

RoundBoard::RoundBoard(char n, char k)
  :Board(n,k)
{
  checker = vector<vector<char> >();
  checker.push_back(vector<char>());
		    
  for(char d = 1; d <= (n-1)/(k-1); d++){
    vector<char> c;
    for(char left = n-d; left > 0; left -= d) { c.push_back(left); }
    reverse(c.begin(), c.end());
    c.push_back(0);
    for(char right = d; right < n; right += d) { c.push_back(right); }

    checker.push_back(c);
  }
}

void RoundBoard::shiftGrid(){
  char x = grid[0];
  for(char i = 0; i < n-1; i++){
    grid[i] = grid[i+1];
  }
  grid[n-1] = x;
}

bool RoundBoard::memberOfAP(char loc){
  /*
  bool ans = false;

  for(char i = 0; i < n; i++){
    if (! ans){
      char pos = loc-i;
      if (pos < 0) { pos += n; }
      if (Board::memberOfAP(pos)) { ans = true; }
    }
    shiftGrid();
  }

  return ans;
  */  

  //return winner() == grid[loc];
    
  for(int d = 1; d <= (n-1)/(k-1); d++){
    const vector<char>& c = checker[d];
    char left = (c.size() / 2) - 1;
    while(left > -1){
      char l = (c[left]+loc)%n;
      if (grid[l] != grid[loc]) { break; }
      left--;
    }
    left++;

    char right = (c.size() / 2) + 1;
    while(right < c.size()){
      char r = (c[right]+loc)%n;
      if (grid[r] != grid[loc]) { break; }
      right++;
    }
    right--;

    //if (right-left+1 < k) {continue;}

    if (right-left+1 >= k) { return true; }
  }

  return false;

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
