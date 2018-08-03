#include "environment.h"

namespace AST {

  bool Environment::AddType(Type* type) {
    if (type_table_.Find(type->GetTypeName()))
      return false;
    type_table_.Insert(type->GetTypeName(), type);
    return true;
  }

  bool Environment::RemoveType(const char* type_name) {
    if (!type_table_.Find(type_name))
      return false;

    return type_table_.Delete(type_name);
  }

  Type* Environment::GetType(const char* type_name) {
    return type_table_.Find(type_name);
  }

  // Scope control
  Scope* Environment::AddSiblingScope() {
    return cur_scope_->AddSibling();
  }

  Scope* Environment::AddChildScope() {
    return cur_scope_->AddChild();
  }
  
  Scope* Environment::GetCurScope() {
    return cur_scope_;
  }

  void Environment::SetCurScope(Scope* scp) {
    cur_scope_ = scp;
  }

  void Environment::AddNamedDecl(NamedDecl* decl) {
    cur_scope_->AddNamedDecl(decl);
  }

  NamedDecl* Environment::GetDecl(const char* variable_name) {
    return cur_scope_->GetDecl(variable_name);
  }
};

