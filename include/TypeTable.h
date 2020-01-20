#ifndef _TypeTable_h_
#define _TypeTable_h_

#include "./core/hash.h"
#include "ASTType.h"

namespace AST {
  class TypeTable {
    private:
      // same as Map<std::string, Type*>
      HashTable<Type*, 64> table_;

      // Check recursive type definition.
      // ex) class AA { BB b; }
      //     class BB { AA a; }
      //     
      // ex) class AA { BB b; }
      //     typedef class AA CC;
      //     class BB { CC c; }
      bool CheckRecursiveTypeDef(Type const* originTy, Type const * targetTy);
    public:
      TypeTable();
      ~TypeTable();

      bool Add(Type* ty);
      bool Remove(const char* typeName);
      Type* Find(const char* typeName);

      // Iterator function.
      // TODO : Need to expose interator later
      void ResetItr();
      Type* Next();
      
      // Check Semantic type error
      bool CheckSemanticError();
  };
};

#endif 


