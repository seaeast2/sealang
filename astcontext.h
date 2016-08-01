#ifndef _astcontext_h_
#define _astcontext_h_

#include <vector>
#include <string>
using namespace std;

namespace Parser {

class ASTContext {
  public:
    typedef vector<string> vstrings;

  private:
    vstrings imports_;

  public:
    ASTContext() {}
    ~ASTContext() {}

    void AddImport(const string &import);
    vstrings::iterator import_begin();
    vstrings::iterator import_end();

    // Debug functions
    void PrintImports();
};

};

#endif
