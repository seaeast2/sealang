#ifndef _type_resolver_h_
#define _type_resolver_h_

#include "Common.h"
#include "ASTNode.h"
#include "ASTContext.h"
#include "core/hash_int.h"


namespace AST {

  // TODO : Need to move member functions to FunctionDecl, RecordDecl.
  class TypeResolver {
    private:
      HashInt<unsigned long, Type*, 64> recursive_type_checker_;

    public:
      TypeResolver();
      virtual ~TypeResolver();

      bool MakeCompleteType(ASTContext* ac);

    private:
      // Make incomplete type to complete type
      bool CompleteFunctionType(ASTContext* ac, FunctionDecl* fd);
      bool CompleteRecordType(ASTContext* ac, RecordDecl* RD);
  };
};

#endif
