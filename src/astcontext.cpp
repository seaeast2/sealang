#include "astcontext.h"
#include <iostream>
#include "ast_printer.h"
#include "local_var_checker.h"
#include "type_resolver.h"

using namespace std;

namespace AST {
  ASTContext::ASTContext() {
    decls_ = nullptr;
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
    if (type_table_.Find(ty->GetTypeName()))
      return false;
    type_table_.Insert(ty->GetTypeName(), ty);
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

  void ASTContext::CheckLocalVar() {
    LocalVarChecker lvc;
    lvc.Check(decls_, &top_scope_);
  }

  void ASTContext::CheckIncompleteType() {
    TypeResolver tc;
    tc.Check(this);
  }

  void ASTContext::PrintAST() {
    ASTPrinter ast_printer;
    ast_printer.Print(GetLocalDecl());
  }

};
