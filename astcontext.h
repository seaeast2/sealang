#ifndef _astcontext_h_
#define _astcontext_h_

#include "common_typedef.h"
#include "ast.h"

using namespace std;
using namespace CommonType;

namespace AST {

  // AST context manager
  class ASTContext {
    private:
      VecStr imports_;
      RootNode* rootNode_;


    public:
      ASTContext();
      ~ASTContext();

      void AddImport(const string &import);
      VecStr::iterator import_begin();
      VecStr::iterator import_end();
      
      RootNode* GetAstRoot() { return rootNode_; }

      // Debug functions
      void PrintImports();
  };

};

#endif
