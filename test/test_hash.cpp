
#include <iostream>
#include "core/hash.h"

using namespace std;

struct AAA {
  int n;
};

int main() {
  AAA *a = new AAA, b, *c = new AAA, d, e;
  a->n = 10;
  b.n = 20;
  c->n = 30;
  d.n = 40;
  e.n = 50;

  HashTable<AAA*, 20> hashptr;
  HashTable<AAA, 20> hash;

  hashptr.Insert("10", a);
  hash.Insert("20", b);
  hashptr.Insert("30", c);
  hash.Insert("40", d);
  hash.Insert("50", e);


  AAA* result = nullptr;

  result = hashptr.Find("30");
  cout << "30 : " << result->n << endl;
  
  result = hash.Find("40");
  cout << "40 : " << result->n << endl;

  return 0;
}

