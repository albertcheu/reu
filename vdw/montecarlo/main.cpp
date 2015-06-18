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

Record test_n(int k, int n){
  cout << "Testing k = " << k << ", n = " << n << endl;

  clock_t t = clock();
  Record r = {0,0,0};

  for(int i = 0; i < 100; i++){
    Board b(n,k);

    //Play montecarlo against itself
    bool redPlayer = true;
    size_t depth = 0;
    while(b.noWinner() && depth != n){
      int loc = b.montecarlo(redPlayer, MC_TRIALS);
      b.play(redPlayer?'R':'B', loc);
      redPlayer = (!redPlayer);
      depth++;
    }

    if (b.won('R')) { r.redWins++; }
    else if (b.won('B')) { r.blueWins++; }
    else { r.numDraws++; }
  }

  t = clock() - t;
  cout << "It took me " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;

  cout << "Number of R wins: " << r.redWins << endl;
  cout << "Number of B wins: " << r.blueWins << endl;
  cout << "Number of draws: " << r.numDraws << endl;

  return r;
}

void search_for_G(){
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
    cout << "What's the starting size of the board? n = ";
    cin >> s;
    n = toNumber(s, 10000);
    if (n == -1) { cout << "Please enter 0 < n < 10000" << endl; }
    else { break; }
  }
  
  int upperBound = 100000;
  int lowerBound = 0;
  Record r1,r2;
  bool binSearch = false;

  while(lowerBound < upperBound) {
    r1 = test_n(k,n);
    r2 = test_n(k,n-1);
    if (r1.redWins >= 50 && r2.redWins < 50) { break; }
    else if (r2.redWins >= 50) { binSearch = true; }

    if (binSearch) {
      if (r2.redWins < 50) { lowerBound = n; }
      else { upperBound = n; }
      n = (upperBound + lowerBound) / 2;
    }
    else {
      lowerBound = n;
      if (r1.redWins < 20) { n *= 2; }
      else if (r1.redWins < 40) { n += 10; }
      else if (r1.redWins < 50) { n++; }
    }
  }
  cout << "I think GW(" << k << ") = " << n << endl;
}

int main(){
  srand(unsigned(time(0)));
  bool search = true;
  string s = "";
  while(s != "y" && s != "n"){
    cout << "Do you want to play (y) or search (n)? [y/n]: ";
    cin >> s;
    if (s == "y") { search = false; }
  }

  if (search) { search_for_G(); }

  else { play_game(); }

}
