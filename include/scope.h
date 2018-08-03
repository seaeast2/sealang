#ifndef _scope_h_
#define _scope_h_

#include "core/simple_vector.h"
#include "ast_node.h"

namespace AST {

  class Scope {
    protected:
      Scope* parent_;
      SimpleVector<Scope*> children_;
      SimpleVector<NamedDecl*> decls_;

    public:
      Scope();
      ~Scope();
      // Scope control
      Scope* AddSibling();
      Scope* AddChild();
      Scope* GetParent();
      Scope* GetChild(int index);

      // Decl control
      void AddNamedDecl(NamedDecl* node);
      NamedDecl* GetDecl(const char* variable_name);
  };
};

#endif
