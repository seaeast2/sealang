#ifndef _astcontext_h_
#define _astcontext_h_

#include "common.h"
#include "ast_node.h"

using namespace std;

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
