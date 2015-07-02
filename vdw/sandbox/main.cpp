#include "BoardGreedy.h"

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

void search_for_G(int n, int k, bool greedyFirst){
  int match = 1;
  if (greedyFirst) { match = 0; }

  //Iterate thru board sizes

  while(true){
    cout << "Testing game(" << n << "," << k << ")..." << endl;
    clock_t t = clock();

    BoardGreedy b(n,k);

    bool redPlayer = true;
    size_t depth = 0;
    int justPlayed = -1;
    while(b.noWinner() && depth != n){
      int loc;

      if (depth % 2 == match) {
	loc = b.decide(redPlayer,depth++);
      }

      else {
	scoreAndLoc sal = b.alphabeta(redPlayer,-10,10, depth++, justPlayed);
	loc = sal.second;
      }

      cout << loc << endl;
      b.play(redPlayer?'R':'B', loc);
      redPlayer = (!redPlayer);
      justPlayed = loc;
    }
    b.print();

    t = clock() - t;
    cout << "It took me " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;

    char winner = b.winner();
    //Stop when player 1 wins
    if (winner == 'R') { break; }
    else if (winner == 'B') { cout << "Oh no, B should never win" << endl; break; }

    //Continue with bigger board size
    else { n++; }

  }

  cout << "I think GW(k=" << k << ") = " << n << endl;
}

int main(int argc, char** argv){
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

    s = argv[3];
    bool greedyFirst = (s == "first");

    search_for_G(n, k, greedyFirst);
    
  }

}
