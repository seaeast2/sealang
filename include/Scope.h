#ifndef _scope_h_
#define _scope_h_

#include "core/simple_vector.h"
#include "ASTNode.h"

namespace AST {

  class Scope {
    protected:
      Scope* parent_;
      SimpleVector<Scope*> children_;
      SimpleVector<NamedDecl*> decls_;

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
      bool IsDuplicatedNameInCurScope(const char* variable_name);

      int GetDeclNum() { return decls_.GetSize(); }
      NamedDecl* GetDecl(int index) { return decls_[index]; }
  };
};

#endif
