#include <thread>
#include <iostream>
#include <vector>
using namespace std;



class Obj{
private:
  vector<int> v;

public:
  Obj();
  void steve(vector<int> const & v, size_t index);
  void foobar();
};

Obj::Obj(){
  v.push_back(0);
  v.push_back(1);
}

void Obj::steve(vector<int> const & v, size_t index){
  cout << v[index] << endl;
}

void Obj::foobar(){
  thread t(ref(Obj::steve), v, 0);
  thread t2(ref(Obj::steve), v, 1);
  t.join();
  t2.join();
}

int main(){
  Obj o;
  o.foobar();
}
