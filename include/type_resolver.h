#ifndef _type_resolver_h_
#define _type_resolver_h_

#include "common.h"
#include "ast_node.h"
#include "astcontext.h"


namespace AST {

  // Traverse AST and adjust incomplete types
  class TypeResolver {

    public:
      TypeResolver();
      virtual ~TypeResolver();

      bool MakeCompleteType(ASTContext* ac);
      // Check Void Array
      bool CheckVoidArray(ASTContext* ac);
      // Check Recursive type definition.
      bool CheckRecursiveTypeDef(ASTContext* ac);

    private:
      // Make incomplete type to complete type
      bool CompleteFunctionType(ASTContext* ac, FunctionDecl* fd);
      bool CompleteClassType(ASTContext* ac, ClassNode* cn);
  };
};

#endif
