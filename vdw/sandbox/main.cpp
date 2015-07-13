#include "BoardThread.h"

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

void blarg(BoardThread& b, scoreAndLoc& sal, bool& done){
  b.fillTable(sal);
  done = true;
}

void search_for_G(int n, int k){
  //Iterate thru board sizes

  while(true){
    cout << "Testing game(" << n << "," << k << ")..." << endl;
    clock_t t = clock();

    unordered_map<BitstringKey,pair<Bitstring,int> > table;
    Board_AB b(n,k,table);
    /*
    size_t turn = 0;
    vector<bool> wantsToEnter;
    wantsToEnter.push_back(false);wantsToEnter.push_back(false);
    BoardThread even(n,k,0,ref(turn),ref(wantsToEnter),
		     ref(b.assignments), ref(table));
    BoardThread odd(n,k,1,ref(turn),ref(wantsToEnter),
		    ref(b.assignments), ref(table));
    */
    mutex lock;
    BoardThread even(n,k, ref(lock), 0,
		     ref(b.assignments), ref(table));
    BoardThread odd(n,k, ref(lock), 1,
		    ref(b.assignments), ref(table));

    scoreAndLoc sal1 = draw; scoreAndLoc sal2 = draw;
    bool doneOdd = false; bool doneEven = false;
    thread tOdd(blarg, ref(odd), ref(sal1), ref(doneOdd));
    thread tEven(blarg, ref(even), ref(sal2), ref(doneEven));

    while(!doneOdd && !doneEven){}
    tOdd.join();
    tEven.join();

    bool redPlayer = true;
    size_t depth = 0;
    int justPlayed = -1;
    while(b.noWinner() && depth != n){
      int loc;
      scoreAndLoc sal = b.alphabeta(redPlayer,-10,10, depth++, justPlayed);
      //cout << "Winner: " << sal.first << endl;
      loc = sal.second;

      cout << loc << endl;
      b.play(redPlayer?'R':'B', loc);
      redPlayer = (!redPlayer);
      justPlayed = loc;
    }
    b.print();

    t = clock() - t;
    cout << "It took me " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;

    char winner = b.winner();

    if (winner == 'R') { break; }
    else if (winner == 'B') {
      cout << "Oh no, B should never win" << endl;
      break;
    }
    else { n++; }

  }

  cout << "I think GW(k=" << k << ") = " << n << endl;
}

int main(int argc, char** argv){
  string s = "";

  if (argc >= 3) {

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

    search_for_G(n, k);
    
  }

}
/*
  if (depth==0 && !child){
    scoreAndLoc ans = draw;

    //Halve work
    Board_AB odd(n,k,true,table);
    odd.assignments = assignments;
    Board_AB even(n,k,true,table);
    even.assignments = assignments;

    //Spawn children
    size_t turn = 0;
    vector<bool> wantsToEnter;
    wantsToEnter.push_back(false);wantsToEnter.push_back(false);
    thread tOdd(childThread, odd, ans, 1, 0, turn, wantsToEnter);
    thread tEven(childThread, even, ans, 0, 1, turn, wantsToEnter);

    //Finish
    tOdd.join();
    tEven.join();

    recursionCount = odd.recursionCount + even.recursionCount;
    return ans;
  }


 */
