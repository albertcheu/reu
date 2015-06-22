#include "Board_AB.h"
#include "Board_MC.h"

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

void play_game(int n, int k){
  cout << "Welcome to VDW game. You are 'B' and I am 'R'" << endl;

  //Is the computer playing first? 
  bool computer1 = true;

  string s = "";
  while(s != "y" && s != "n"){
    cout << "Do you want to play first? [y/n]: ";
    cin >> s;
    if (s == "y") { computer1 = false; }
  }

  bool player1 = true;
  Board_AB b(n, k);
  b.print();
  size_t depth = (computer1?0:1);

  //While there is no winner (and we can still play)
  while(b.noWinner() && ! b.filled()){
    cout << endl << "Player " << (player1?1:2) << " plays on: ";
    int loc;

    if (player1 == computer1){
      scoreAndLoc sal = b.alphabeta(true,-10,10,depth,0);
      loc = sal.second;
      cout << loc+1 << endl;
      b.play('R', loc);
    }
    
    else{
      //Get user input
      cin >> s;

      //Fail if not number in range
      loc = toNumber(s, n);
      if (loc == -1) {
	cout << "Please enter a number from 1 to " << n << endl;
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

    depth++;
  }

  char winner = b.winner();
  if (winner == 'R') { cout << "R won!" << endl; }
  else if (winner == 'B') { cout << "B won!" << endl; }
  else { cout << "Nobody won!" << endl; }
}

void search_for_G_AB(int n, int k){
  //Iterate thru board sizes

  while(true){
    cout << "Testing game(" << n << "," << k << ")..." << endl;
    clock_t t = clock();

    Board_AB b(n,k);

    //Play alphabeta against itself (red player = player 1)
    bool redPlayer = true;
    size_t depth = 0;
    while(b.noWinner() && depth != n){
      int loc;
      scoreAndLoc sal = b.alphabeta(redPlayer,-10,10, depth++, 0);
      loc = sal.second;
      cout << loc+1 << endl;
      b.play(redPlayer?'R':'B', loc);
      redPlayer = (!redPlayer);
    }
    b.print();

    t = clock() - t;
    cout << "It took me " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;

    //Stop when player 1 wins
    if (b.winner() == 'R') { break; }

    //Continue with bigger board size
    else { n++; }

  }

  cout << "I think GW(k=" << k << ") = " << n << endl;
}

Record test_n(int n, int k){
  cout << "Testing k = " << k << ", n = " << n << endl;

  clock_t t = clock();
  Record r = {0,0,0};

  for(int i = 0; i < 100; i++){
    Board_MC b(n,k);

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

void search_for_G_MC(int n, int k){
  
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
      else if (r1.redWins < 50) { n += 2; }
    }
  }
  cout << "I think GW(" << k << ") = " << n << endl;
}

int main(int argc, char** argv){

  bool search = true;
  string s = "";

  if (argc >= 4) {

    //n
    s = argv[1];
    int n = toNumber(s, 10000);
    if (n == -1) {
      cout << "Please enter 0 < n < 10000" << endl;
      return 0;
    }
    
    //k
    s = argv[2];
    int k = toNumber(s, 10000);
    if (k == -1) {
      cout << "Please enter 0 < k < 10000" << endl;
      return 0;
    }
    else if (n < k) {
      cout << "Your board size (n) is smaller than the progression length!";
      cout << endl;
      return 0;
    }

    //whether to play montecarlo or alphabeta
    s = argv[3];
    if (s == "mc") {
      cout << "Running Monte Carlo..." << endl;
      search_for_G_MC(n, k);
    }
    else if (s == "ab") {
      cout << "Running alpha-beta..." << endl;
      search_for_G_AB(n, k);
    }
  }

}
