#ifndef _astcontext_h_
#define _astcontext_h_

#include "common.h"
#include "ast_type.h"
#include "ast_node.h"
#include "environment.h"


namespace AST {
  // AST context manager
  class ASTContext {
    private:
      VecStr imports_;
      RootNode* rootNode_;

      // type environment
      Environment<Type*> type_env_;
      // variable evironment
      Environment<VariableNode*> var_evn_;

    public:
      ASTContext();
      ~ASTContext();

      void AddImport(const std::string &import);
      VecStr::iterator import_begin();
      VecStr::iterator import_end();
      
      RootNode* GetAstRoot() { return rootNode_; }

      // Debug functions
      void PrintImports();
  };

};

#endif
