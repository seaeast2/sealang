#ifndef _astcontext_h_
#define _astcontext_h_

#include "common.h"
#include "ast_type.h"
#include "ast_node.h"
#include "core/hash.h"
#include "scope.h"

namespace AST {
  // AST context manager
  
  class ASTContext {
    protected:
      TypeTable type_table_;
      Declarations* decls_;

      Scope top_scope_;

    public:
      ASTContext();
      ~ASTContext();
      
      void SetLocalDecl(Declarations* decls);
      Declarations* GetLocalDecl();

      // Type environment control
      bool AddType(Type* ty);
      bool RemoveType(const char* type_name);
      Type* GetType(const char* type_name);

      void CheckLocalVar();
      void CheckIncompleteType();
      void PrintAST();
  };

};

#endif
