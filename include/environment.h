#ifndef _environment_h_
#define _environment_h_

#include "core/hash.h"
#include "core/simple_vector.h"
#include "ast_type.h"
#include "scope.h"

namespace AST {
  class Environment {
    private:

      // Type environment control
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
