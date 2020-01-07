#ifndef _type_resolver_h_
#define _type_resolver_h_

#include "Common.h"
#include "ASTNode.h"
#include "ASTContext.h"
#include "core/hash_int.h"


namespace AST {

  // Traverse AST and turn incomplete type to complete type
  class TypeResolver {
    private:
      HashInt<unsigned long, Type*, 64> recursive_type_checker_;

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
      bool CompleteRecordType(ASTContext* ac, RecordDecl* RD);
      // Check Recursive type definition.
      bool VisitRecordType(RecordType* ct);
  };
};

#endif