#ifndef _astcontext_h_
#define _astcontext_h_

#include "common_typedef.h"

using namespace std;
using namespace CommonType;

namespace AST {

  // AST context manager
  class ASTContext {
    private:
      VecStr imports_;

    public:
      ASTContext() {}
      ~ASTContext() {}

      void AddImport(const string &import);
      VecStr::iterator import_begin();
      VecStr::iterator import_end();

      // Debug functions
      void PrintImports();
  };

};

#endif
