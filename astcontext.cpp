#include "astcontext.h"
#include <iostream>

using namespace std;

namespace AST {
void ASTContext::AddImport(const string &import) {
  imports_.push_back(import);
}

VecStr::iterator ASTContext::import_begin() {
  return imports_.begin();
}

VecStr::iterator ASTContext::import_end() {
  return imports_.end();
}

void ASTContext::PrintImports() {
  VecStr::iterator  i = import_begin(), e = import_end();
  for (;i != e; i++) {
    cout << *i << endl;
  }
}

};
