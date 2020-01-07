#ifndef _astcontext_h_
#define _astcontext_h_

#include "Common.h"
#include "ASTType.h"
#include "ASTNode.h"
#include "core/hash.h"
#include "Scope.h"

namespace AST {
  // AST context manager
  
  class ASTContext {
    protected:
      TypeTable typeTable_;
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
      FunctionType* GetFunctionTypeFromDecl(FunctionDecl* fd);
      void ResetTypeItr(); // reset type iteration
      Type* GetTypeAndNext(); // type iteration

      // Check if local variable is in right usage.
      void ResolveLocalVariable();
      void MakeCompleteType();
      void CheckVoidArray();
      void CheckRecursiveTypeDef();
      void PrintAST();
  };

};

#endif
