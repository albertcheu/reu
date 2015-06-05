//Made by Albert Cheu, 5/3/15
#include <string>
#include "Board.h"

using namespace std;

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

  bool player1 = true;
  Board b;
  b.print();

  //While there is no winner (and we can still play)
  while(b.noWinner() && ! b.noSpace()){
    cout << endl << "Player " << (player1?1:2) << " plays on: ";
    int loc;

    if (player1 == computer1){
      //scoreAndLoc sal = b.minimax(true);
      scoreAndLoc sal = b.alphabeta(true,-10,10);
      loc = sal.second;
      cout << loc << endl;
      b.play('X', loc);
    }
    
    else{
      //Get user input
      cin >> loc;
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
