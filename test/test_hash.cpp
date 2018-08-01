
#include <iostream>
#include "core/hash.h"

using namespace std;

struct AAA {
  int n;
};

int main() {
  AAA a, b, c, d, e;
  a.n = 10;
  b.n = 20;
  c.n = 30;
  d.n = 40;
  e.n = 50;

  HashTable<AAA*, 20> hash;

  hash.Insert("10", &a);
  hash.Insert("20", &b);
  hash.Insert("30", &c);
  hash.Insert("40", &d);
  hash.Insert("50", &e);


  AAA** result = nullptr;

  result = hash.Find("20");
  cout << "20 : " << (*result)->n << endl;
  
  result = hash.Find("50");
  cout << "50 : " << (*result)->n << endl;

  return 0;
}

