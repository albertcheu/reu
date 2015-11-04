#include "RoundBoard.h"

RoundBoard::RoundBoard(num n, num k)
  :Board(n,k)
{
  //checker = vector<vector<num> >();
  checker.push_back(vector<num>());
		    
  for(num d = 1; d <= (n-1)/(k-1); d++){
    //cout << d << ' ';
    vector<num> c;
    for(num left = n-d; left < n; left -= d) { c.push_back(left); }
    reverse(c.begin(), c.end());
    c.push_back(0);
    for(num right = d; right < n; right += d) { c.push_back(right); }

    checker.push_back(c);
  }
  //cout << endl << "Made checker" << endl;
}

void RoundBoard::shiftGrid(){
  num x = grid[0];
  for(int i = 0; i < n-1; i++){
    grid[i] = grid[i+1];
  }
  grid[n-1] = x;
}

bool RoundBoard::memberOfAP(num loc){
  /*
  bool ans = false;

  for(num i = 0; i < n; i++){
    if (! ans){
      num pos = loc-i;
      if (pos < 0) { pos += n; }
      if (Board::memberOfAP(pos)) { ans = true; }
    }
    shiftGrid();
  }

  return ans;
  */  

  //return winner() == grid[loc];
    
  for(num d = 1; d <= (n-1)/(k-1); d++){
    const vector<num>& c = checker[d];
    int left = (c.size() / 2) - 1;
    while(left > -1){
      num l = (c[left]+loc)%n;
      if (grid[l] != grid[loc]) { break; }
      left--;
    }
    left++;

    num right = (c.size() / 2) + 1;
    while(right < c.size()){
      num r = (c[right]+loc)%n;
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

  for(num i = 0; i < n; i++){
    //There can't be a winner using this spot if it is empty
    if (grid[i] == '.') { continue; }

    //Otherwise, iterate thru d's
    for(num d = 1; d <= (n-1)/(k-1); d++){

      bool homogenous = true;
      for(int j = 0; j < k-1; j++){
	num a = (i+(j*d)) % n;
	num b = (i+((j+1)*d)) % n;
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
