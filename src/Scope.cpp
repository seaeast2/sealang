#include "Scope.h"

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

  void Scope::SetParent(Scope* parent) {
    parent_ = parent;
  }

  Scope* Scope::GetParent() { 
    return parent_; 
  }

  Scope* Scope::GetTop() {
    Scope* cur = this;
    while (cur->parent_ != nullptr) {
      cur = cur->parent_;
    }
    return cur;
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
    child->SetParent(this);
    children_.PushBack(child);
    return child;
  }

  NamedDecl* Scope::FindDecl(const char* variable_name) {
    Scope* cur = this;
    while(cur) {
      for (int i = 0; i < cur->GetDeclNum(); i++) {
        if (!strcmp(cur->GetDecl(i)->GetName(), variable_name)) {
          return cur->GetDecl(i); 
        }
      }
      cur = cur->parent_;
    }

    return nullptr;
  }

  NamedDecl* Scope::FindFuncDecl(const char* funcName) {
    Scope* cur = this;
    while(cur) {
      for (int i = 0; i < cur->GetDeclNum(); i++) {
        NamedDecl* ND = cur->GetDecl(i);
        if (!strcmp(ND->GetName(), funcName) && 
            ND->IsKindOf(AST::BaseNode::FunctionDeclTy)) {
          return cur->GetDecl(i); 
        }
      }
      cur = cur->parent_;
    }

    return nullptr;
  }

  bool Scope::HasDuplicatedName(const char* variable_name) {
    for (int i = 0; i < decls_.GetSize(); i++) {
      if (!strcmp(decls_[i]->GetName(), variable_name))
        return true; 
    }
    return false;
  }
};

