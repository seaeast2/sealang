#ifndef _environment_h_
#define _environment_h_

#include "core/hash.h"
#include "core/simple_vector.h"
#include "ast_type.h"
#include "scope.h"

namespace AST {
  class Environment {
    private:
      // type table 
      HashTable<Type*, 64> type_table_;
      
      Scope top_scope_;
      Scope* cur_scope_;

    public:
      Environment() {
        cur_scope_ = &top_scope_;
      }
      ~Environment() {
      }

      // Type environment control
      bool AddType(Type* type);
      bool RemoveType(const char* type_name);
      Type* GetType(const char* type_name);

      // Scope control
      Scope* AddSiblingScope();
      Scope* AddChildScope();
      Scope* GetCurScope();
      void SetCurScope(Scope* scp);
      void AddNamedDecl(NamedDecl* decl);
      NamedDecl* GetDecl(const char* variable_name);
  };
};

#endif
