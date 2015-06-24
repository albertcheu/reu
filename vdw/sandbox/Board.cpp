
#include "Board.h"

Board::Board(size_t n, size_t k)
  :n(n), k(k)
{
  for(size_t i = 0; i < n; i++){
    grid.push_back('.');
    empties.push_back(i);
  }
}

size_t Board::size(){return n;}

char Board::winner(){
for(int i = 0; i <= n-k; i++){
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

size_t Board::numTurns(){ return n - empties.size(); }

bool Board::filled(){ return empties.size() == 0; }

void Board::print(){
  for(int i = 0; i < n; i++){ std::cout << grid[i]; }
  std::cout << std::endl;
}

bool Board::play(char c, int loc){
  if (grid[loc] != 'R' && grid[loc] != 'B'){
    grid[loc] = c;
    //Delete loc in empties, if it is in that vector
    std::vector<int>::iterator itr= std::lower_bound(empties.begin(),
						     empties.end(),
						     loc);
    if (*itr == loc) { empties.erase(itr); }
    return true;
  }

  return false;
}
