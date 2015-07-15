#include <omp.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
using namespace std;

void sieveSingle(unsigned int max){
  clock_t t = clock();
  vector<bool> isComposite(max);
  for(size_t i = 0; i < isComposite.size(); i++){ isComposite[i] = false; }

  for(unsigned int i = 2; i < max; i++){
    for(unsigned int cur = 2*i; cur < max; cur += i) {
      isComposite[cur] = true;
    }
  }

  t = clock()-t;
  cout << "It took me " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;
}

void sieveThreaded(unsigned int max){
  clock_t t = clock();
  vector<bool> isComposite(max);
  for(size_t i = 0; i < isComposite.size(); i++){ isComposite[i] = false; }

  #pragma omp parallel for
  for(unsigned int i = 2; i < max; i++){
    for(unsigned int cur = 2*i; cur < max; cur += i) {
      isComposite[cur] = true;
    }
  }

  t = clock()-t;
  cout << "It took me " << ((float)t)/CLOCKS_PER_SEC << " seconds" << endl;
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

int main(int argc, char* argv[]){
  string s = argv[1];
  int max = toNumber(s, 1000000);
  if (max > 0){
    sieveSingle((unsigned int) max);
    sieveThreaded((unsigned int) max);
  }
}
