#ifndef _common_h_
#define _common_h_

#include <vector>
#include <string>

using namespace std;

namespace Common {
  typedef vector<string> VecStr;

  enum SAResult {
    SA_FALSE = 0,
    SA_TRUE,
    SA_ERR
  }
};

#endif
