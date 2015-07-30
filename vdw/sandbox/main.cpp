//#include "BoardEval.h"
//#include "GreedyEvaluator.h"
//#include "RandomEvaluator.h"
#include "Board_AB.h"
#include "BoardMC.h"

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
/*
void blarg(int n,int k, mutex& lock, int i, int numThreads,
	   vector<pair<Bitstring,Bitstring> >& assignments,
	   unordered_map<BitstringKey,pair<Bitstring,int> >& table){
  BoardThread bt(n,k,lock,i,numThreads,assignments,table);
  bt.fillTable();
}

void fillTable(int n, int k, Board_AB& b, int numThreads,
	       unordered_map<BitstringKey,pair<Bitstring,int> >& table){

  mutex lock;
  vector<thread> threads;

  for(int i = 0; i < numThreads; i++){
    threads.push_back(thread(blarg, n,k,ref(lock),i, numThreads,
			     ref(b.assignments), ref(table)));
  }

  for(int i = 0; i < numThreads; i++){
    threads[i].join();
  }
  
}
*/

void search_for_G(int n, int k){
  //Iterate thru board sizes
  while(true){
    cout << "game(" << n << "," << k << ")" << endl;
    clock_t t = clock();
    /*
    unordered_map<BitstringKey,pair<Bitstring,int> > table;
    Board_AB b(n,k,table);
    if (numThreads > 1) { fillTable(n,k,b,numThreads,ref(table)); }
    */

    Board_AB b(n,k);
    //GreedyEvaluator e(n,k);
    //MCEvaluator e(n,k);
    //RandomEvaluator e(n,k);
    //BoardEval b(n,k,&e);

    scoreAndLoc sal = b.alphabeta(true,-10,10, 0);

    /*
    bool redPlayer = true;
    char depth = 0;
    
    int justPlayed = -1;
    while(b.noWinner() && depth != n){
      int loc;
      scoreAndLoc sal = b.alphabeta(redPlayer,-10,10, depth++, justPlayed);
      //cout << "Winner: " << sal.first << endl;
      loc = sal.second;

      //cout << "Loc: " << loc << endl;
      b.play(redPlayer?'R':'B', loc);
      redPlayer = (!redPlayer);
      justPlayed = loc;
    }
    
    b.print();
    */

    t = clock() - t;
    cout << "It took me " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;

    /*
    char winner = b.winner();
    if (winner == 'R') { break; }
    else if (winner == 'B') {
      cout << "Oh no, B should never win" << endl;
      break;
    }
    */
    if (sal.first == 1) { cout << "Red won" << endl; break; }
    else if (sal.first == -1) { cout << "Blue won" << endl; break; }
    else { n++; }

  }

  cout << "GW(k = " << k << ") = " << n << endl;
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
    if (s == "ab"){ search_for_G(n, k); }

    else {
      srand((unsigned)time(NULL));
      BoardMC bmc(n,k);
      bmc.montecarlo();
    }

  }

}
