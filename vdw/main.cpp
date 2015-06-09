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

int foobar(){
  cout << "\tRBRBRBRBRBR" << endl;
  cout << "Welcome to VDW. You are 'B' and I am 'R'" << endl;
  cout << "\tRBRBRBRBRBR" << endl << endl;

  //Is the computer playing first? 
  bool computer1 = true;

  string s = "";
  while(s != "y" && s != "n"){
    cout << "Do you want to play first? [y/n]: ";
    cin >> s;
    if (s == "y") { computer1 = false; }
  }

  int k = 3;
  while(true){
    cout << "How long should the arthmetic sequence be? k = ";
    cin >> s;
    k = toNumber(s, 10000);
    if (k == -1) { cout << "Please enter 0 < k < 10000" << endl; }
    else { break; }
  }


  int boardSize = 8;
  while(true){
    cout << "How big do you want the board to be? ";
    cin >> s;
    boardSize = toNumber(s, 10000);
    if (boardSize == -1)
      { cout << "Please enter 0 < boardSize < 10000" << endl; }
    else if (boardSize < k) { cout << "Your board is smaller than k!" << endl; }
    else { break; }
  }

  bool player1 = true;
  Board b(boardSize, k);
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
      b.play('R', loc);
    }
    
    else{
      //Get user input
      cin >> s;

      //Fail if not number in range
      loc = toNumber(s, b.size());
      if (loc == -1) {
	cout << "Please enter a number from 1 to " << b.size() << endl;
	continue;
      }

      //Fail if space already occupied (play otherwise)
      loc--;
      if (!b.play('B', loc)) {
	cout << "Space taken" << endl;
	continue;
      }
    }
    //Show grid
    b.print();

    //Switch turns
    player1 = !player1;
  }

  if (b.won('R')) { cout << "R won!" << endl; }
  else if (b.won('B')) { cout << "B won!" << endl; }
  else { cout << "Nobody won!" << endl; }
}

int main(){
  //Ask for k
  string s;
  int k = 3;
  while(true){
    cout << "How long should the arthmetic sequence be? k = ";
    cin >> s;
    k = toNumber(s, 10000);
    if (k == -1) { cout << "Please enter 0 < k < 10000" << endl; }
    else { break; }
  }

  //Iterate thru board sizes
  size_t boardSize = 3;
  while(true){
    Board b(boardSize,k);
    b.print();

    //Play alphabeta against itself (red player = player 1)
    bool redPlayer = true;
    while(b.noWinner() && ! b.noSpace()){
      int loc;
      scoreAndLoc sal = b.alphabeta(redPlayer,-10,10);
      loc = sal.second;
      cout << loc+1 << endl;
      b.play(redPlayer?'R':'B', loc);
      redPlayer = (!redPlayer);
    }
    b.print();

    //Stop when player 1 wins
    if (b.won('R')) { break; }

    //Continue with bigger board size
    else { boardSize++; }
  }
  cout << "GW(k=" << k << ") = " << boardSize << endl;
}
