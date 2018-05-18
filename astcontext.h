#ifndef _astcontext_h_
#define _astcontext_h_

#include "common.h"
#include "ast_type.h"
#include "ast_node.h"
#include "environment.h"


namespace AST {
  // AST context manager
  class ASTContext {
    protected:
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

      // Type environment control
      Type* FindType(const char* type_name);
      bool AddType(const char* key, Type* ty);
      Type* RemoveType(const char* key);

      // Debug functions
      void PrintImports();
  };

};

#endif
