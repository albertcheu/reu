//Made by Albert Cheu, 5/3/15
#include <string>
#include "Board.h"

using namespace std;

int toNumber(string s, size_t maxNum){
  int ans = 0;
  int place = 1;
  for(int i = s.size()-1; i > -1; i--){
    if (! isdigit(s[i])) { return -1; }
    int digit = ((int)s[i]) - 48;
    ans += place*digit;
    place *= 10;
  }
  if (ans < 1 || ans > maxNum) { return -1; }
  return ans;
}

int main(){
  cout << "\tXOXOXOXOXOX" << endl;
  cout << "Welcome to tic-tac-toe. You are 'O' and I am 'X'" << endl;
  cout << "\tXOXOXOXOXOX" << endl << endl;

  //Is the computer playing first? 
  bool computer1 = true;

  string s = "";
  while(s != "y" && s != "n"){
    cout << "Do you want to play first? [y/n]: ";
    cin >> s;
    if (s == "y") { computer1 = false; }
  }

  size_t size = 3;
  while(true){
    cout << "How long should a side of our board be? ";
    cin >> s;
    size = toNumber(s, 100);
    if (size == -1) {
      cout << "Please give positive number under a hundred" << endl;
      continue;
    }
    else { break; }
  }

  bool player1 = true;
  Board b(size);
  b.print();

  //While there is no winner (and we can still play)
  while(b.noWinner() && ! b.noSpace()){
    cout << endl << "Player " << (player1?1:2) << " plays on: ";
    int loc;

    if (player1 == computer1){
      //scoreAndLoc sal = b.minimax(true);
      scoreAndLoc sal = b.alphabeta(true,-10,10);
      loc = sal.second;
      cout << loc+1 << endl;
      b.play('X', loc);
    }
    
    else{
      //Get user input
      cin >> s;
      loc = toNumber(s, size*size);
      if (loc == -1) {
	cout << "Please give positive number in range" << endl;
	continue;
      }

      //make it 0-indexed
      loc--;
      if (!b.play('O', loc)) {
	cout << "Space taken" << endl;
	continue;
      }
    }
    //Show grid
    b.print();

    //Switch turns
    player1 = !player1;
  }

  if (b.won('X')) { cout << "X won!" << endl; }
  else if (b.won('O')) { cout << "O won!" << endl; }
  else { cout << "Nobody won!" << endl; }
}
