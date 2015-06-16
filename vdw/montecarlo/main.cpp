//Made by Albert Cheu, 5/3/15
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

void play_game(){
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
  size_t depth = (computer1?0:1);

  //While there is no winner (and we can still play)
  while(b.noWinner() && ! b.filled()){
    cout << endl << "Player " << (player1?1:2) << " plays on: ";
    int loc;

    if (player1 == computer1){
      loc = b.montecarlo(true,MC_TRIALS);
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

void test_n(){
  string s;

  //Ask for k
  int k = 3;
  while(true){
    cout << "How long should the arthmetic sequence be? k = ";
    cin >> s;
    k = toNumber(s, 10000);
    if (k == -1) { cout << "Please enter 0 < k < 10000" << endl; }
    else { break; }
  }

  size_t n = 3;
  while(true){
    cout << "How big should the board be? n = ";
    cin >> s;
    n = toNumber(s, 10000);
    if (n == -1) { cout << "Please enter 0 < n < 10000" << endl; }
    else { break; }
  }

  clock_t t = clock();
  Record r = {0,0,0};
  for(int i = 0; i < 100; i++){
    Board b(n,k);
    //b.print();

    //Play montecarlo against itself
    bool redPlayer = true;
    size_t depth = 0;
    while(b.noWinner() && depth != n){
      int loc = b.montecarlo(redPlayer, MC_TRIALS);
      //cout << loc+1 << endl;
      b.play(redPlayer?'R':'B', loc);
      redPlayer = (!redPlayer);
      depth++;
    }
    //b.print();

    if (b.won('R')) { r.redWins++; }
    else if (b.won('B')) { r.blueWins++; }
    else { r.numDraws++; }
  }

  t = clock() - t;
  cout << "It took me " << t << " clicks (";
  cout << ((float)t)/CLOCKS_PER_SEC << " seconds)" << endl;

  cout << "Number of R wins: " << r.redWins << endl;
  cout << "Number of B wins: " << r.blueWins << endl;
  cout << "Number of draws: " << r.numDraws << endl;
}

int main(){
  srand(unsigned(time(0)));
  bool test = true;
  string s = "";
  while(s != "y" && s != "n"){
    cout << "Do you want to play (y) or test (n)? [y/n]: ";
    cin >> s;
    if (s == "y") { test = false; }
  }

  if (test) { test_n(); }

  else { play_game(); }

}
