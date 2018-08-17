#ifndef _type_resolver_h_
#define _type_resolver_h_

#include "common.h"
#include "ast_node.h"
#include "astcontext.h"


namespace AST {

  // Traverse AST and adjust incomplete types
  class TypeResolver {
    ASTContext* ac_;

    public:
      TypeResolver();
      virtual ~TypeResolver();

      bool CheckIncompleteType(ASTContext* ac);
      bool CheckVoidArray(ASTContext* ac);

    private:
      // Make incomplete type to complete type
      bool CompleteFunctionType(FunctionDecl* fd);
      bool CompleteClassType(ClassNode* cn);
      // Check Void Array
      bool CheckVoidArray();
  };
};

#endif
