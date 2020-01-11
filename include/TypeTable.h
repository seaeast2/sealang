#ifndef _TypeTable_h_
#define _TypeTable_h_

#include "./core/hash.h"

namespace AST {
  class TypeTable {
    private:
      HashTable<Type*, 64> table_;

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
  };
};

#endif 


