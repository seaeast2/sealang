#include "scope.h"

namespace AST {

  Scope::Scope() {
    parent_ = nullptr;
  }

  Scope::~Scope() {
    // destroy all children.
    for (int i = 0; i < children_.GetSize(); i++) {
      Scope* s = children_[i];
      delete s;
    }
  }

  Scope* Scope::GetParent() { 
    return parent_; 
  }

  Scope* Scope::GetChild(int index) { 
    if (index > 0 && index < children_.GetSize())
      return children_[index]; 
    return nullptr;
  }

  void Scope::AddNamedDecl(NamedDecl* node) {
    decls_.PushBack(node);
  }


  Scope* Scope::AddChild() {
    Scope* child = new Scope();
    children_.PushBack(child);
    return child;
  }

  NamedDecl* Scope::GetDecl(const char* variable_name) {
    Scope* cur = this;
    while(cur) {
      for (int i = 0; i < decls_.GetSize(); i++) {
        if (!strcmp(decls_[i]->GetName(), variable_name)) {
          return decls_[i];
        }
      }
      cur = cur->GetParent();
    }

    return nullptr;
  }

  bool Scope::IsDuplicatedNameInCurScope(const char* variable_name) {
    for (int i = 0; i < decls_.GetSize(); i++) {
      if (!strcmp(decls_[i]->GetName(), variable_name))
        return true; 
    }
    return false;
  }
};

