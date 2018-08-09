#ifndef _type_resolver_h_
#define _type_resolver_h_

#include "common.h"
#include "ast_node.h"


namespace AST {

  // Traverse AST and adjust incomplete types
  class TypeResolver {
    Declarations* decls_;
    TypeTable* type_table_;

    public:
      TypeResolver();
      virtual ~TypeResolver();

      bool Check(Declarations* decl, TypeTable* tt);
  };
};

#endif
