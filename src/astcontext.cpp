#include "astcontext.h"
#include <iostream>
#include "ast_printer.h"

using namespace std;

namespace AST {
  ASTContext::ASTContext() {
    decls_ = nullptr;
    cur_scope_ = &top_scope_;
  }

  ASTContext::~ASTContext() {
    if (decls_)
      delete decls_;
  }

  void ASTContext::SetLocalDecl(Declarations* decls) {
    decls_ = decls;
  }

  Declarations* ASTContext::GetLocalDecl() {
    return decls_;
  }

  bool ASTContext::AddType(Type* ty) {
    if (type_table_.Find(type->GetTypeName()))
      return false;
    type_table_.Insert(type->GetTypeName(), type);
    return true;
  }

  bool ASTContext::RemoveType(const char* type_name) {
    if (!type_table_.Find(type_name))
      return false;
    return type_table_.Delete(type_name);
  }

  Type* ASTContext::GetType(const char* type_name) {
    return type_table_.Find(type_name);
  }

  // Scope control
  Scope* ASTContext::AddSiblingScope() {
    return cur_scope_->AddSibling();
  }

  Scope* ASTContext::AddChildScope() {
    return cur_scope_->AddChild();
  }
  
  Scope* ASTContext::GetCurScope() {
    return cur_scope_;
  }

  void ASTContext::SetCurScope(Scope* scp) {
    cur_scope_ = scp;
  }

  void ASTContext::AddNamedDecl(NamedDecl* decl) {
    cur_scope_->AddNamedDecl(decl);
  }

  NamedDecl* ASTContext::GetDecl(const char* variable_name) {
    return cur_scope_->GetDecl(variable_name);
  }
  void ASTContext::ResolveVar() {

  }

  void ASTContext::PrintAST() {
    ASTPrinter ast_printer;

    ast_printer.Print(GetLocalDecl());
  }

};
