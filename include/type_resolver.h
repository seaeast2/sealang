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

      bool Check(ASTContext* ac);
      bool ResolveFunctionType(FunctionDecl* fd);
      bool ResolveClassType(ClassNode* cn);
  };
};

#endif
