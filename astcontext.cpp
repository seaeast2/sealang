#include "astcontext.h"
#include <iostream>

using namespace std;

namespace Parser {
void ASTContext::AddImport(const string &import) {
  imports_.push_back(import);
}

ASTContext::vstrings::iterator ASTContext::import_begin() {
  return imports_.begin();
}

ASTContext::vstrings::iterator ASTContext::import_end() {
  return imports_.end();
}

void ASTContext::PrintImports() {
  vstrings::iterator  i = import_begin(),
                      e = import_end();
  for (;i != e; i++) {
    cout << *i << endl;
  }
}

};
