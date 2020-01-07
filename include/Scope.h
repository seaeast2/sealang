#ifndef _scope_h_
#define _scope_h_

#include "core/simple_vector.h"
#include "ASTNode.h"

namespace AST {

  class Scope {
    protected:
      Scope* parent_;
      // Current scope's declarations.
      SimpleVector<NamedDecl*> decls_;
      // Child scopes.
      SimpleVector<Scope*> children_;

    public:
      Scope();
      ~Scope();

      void SetParent(Scope* parent);

      // Scope control
      Scope* AddChild();
      Scope* GetParent();
      Scope* GetChild(int index);
      int GetChildNum() { return children_.GetSize(); }

      // Decl control
      void AddNamedDecl(NamedDecl* node);
      NamedDecl* FindDecl(const char* variable_name);
      // Check if there is duplicated name in this scope.
      bool HasDuplicatedName(const char* variable_name);

      int GetDeclNum() { return decls_.GetSize(); }
      NamedDecl* GetDecl(int index) { return decls_[index]; }
  };
};

#endif
