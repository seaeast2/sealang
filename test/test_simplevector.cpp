#include <iostream>
#include "core/simple_vector.h"

using namespace std;

struct AAA {
  int a;
  int b;
  int c;
};

int main() {
  AAA* aa = new AAA();
  aa->a = 10;
  aa->b = 20;
  aa->c = 30;

  SimpleVector<AAA*> *a = new SimpleVector<AAA*>();
  SimpleVector<AAA*> b;
  a->PushBack(aa);

  b = *a;

  delete a;

  AAA* bb = b.Front();
  cout << "Result " << bb->a << " : " << bb->b << " : " << bb->c << endl;

  delete aa;
  return 0;
}

