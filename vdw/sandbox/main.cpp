#include "Board_AB.h"
#include "BoardMC.h"
//#include "Evaluator.h"
//#include "MCEvaluator.h"
//#include "BoardEval.h"

void kapUsageLinear(size_t n, size_t k){
  vector<size_t> possibles;
  for(size_t i = 0; i < n; i++){ possibles.push_back(0); }

  size_t maxD = (n-1)/(k-1);
  size_t max = 0;
  for(size_t i = 0; i < n; i++){
    for(size_t d = 1; d <= maxD; d++){
      if (i + d*(k-1) >= n) { continue; }
      for (size_t j = 0; j < k; j++){
	possibles[i+j*d]++;
	if (possibles[i+j*d] > max) { max = possibles[i+j*d]; }
      }
    }
  }

  cout << max << " at";
  for(size_t i = 0; i < n; i++){
    if (possibles[i] == max) { cout << ", " << i; }
  }
  cout << endl;
}

void kapUsageCircular(size_t n, size_t k){
  vector<size_t> possibles;
  for(size_t i = 0; i < n; i++){ possibles.push_back(0); }

  vector<size_t> x;

  size_t maxD = (n-1)/(k-1);
  for(size_t i = 0; i < n; i++){
    for(size_t d = 1; d <= maxD; d++){
      size_t end = i+d*(k-1);

      //Never end past the start
      if (end >= n && end % n >= i) { continue; }

      //Which is smaller?
      size_t min = (i < end%n ? i : end%n);

      //If we haven't seen the smaller boundary point before,
      if (x.size() <= min) {
        x.push_back(d);
        for (size_t j = 0; j < k; j++){ possibles[(i+j*d) % n]++; }
      }

      //Otherwise
      else {
        //
        if (((min+d)%n == i || (min+d)%n == end) && x[min] >= d) { continue; }
        x[min] = d;
        for (size_t j = 0; j < k; j++){ possibles[(i+j*d) % n]++; }
      }
    }
  }

  size_t max = 0;
  for(size_t i = 0; i <n; i++){ max = (max>possibles[i]?max:possibles[i]); }
  cout << max << endl;
}


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

void search_for_G_AB(int n, int k, int bound){
  //Iterate thru board sizes

  while(true){
    cout << "Testing game(" << n << "," << k << ")..." << endl;
    clock_t t = clock();

    Board_AB b(n,k, bound);
    num depth = 0;
    scoreAndLoc sal = b.alphabeta(true,-10,10,depth);

    t = clock() - t;
    cout << "It took me " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;

    //Stop when player 1 wins
    if (sal.first == 1) { break; }

    else if (sal.first == -1){
      cout << "Oh no, blue won!" <<endl;
      break;
    }

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

    //whether to play montecarlo or alphabeta
    s = argv[3];
    if (s == "mc") {
      cout << "Running Monte Carlo..." << endl;
      srand((unsigned)time(NULL));
      BoardMC bmc(n,k);
      bmc.montecarlo();
    }
    else if (s == "ab") {
      cout << "Running alpha-beta..." << endl;
      s = argv[4];
      int bound = toNumber(s, 10000);
      if (bound == -1) {
	cout << "Please enter 0 < bound < 10000" << endl;
	return 0;
      }

      search_for_G_AB(n, k, bound);
    }
    else if (s == "linear") {
      kapUsageLinear(n, k);
    }
    else if (s == "circular") {
      kapUsageCircular(n, k);
    }

  }

}
