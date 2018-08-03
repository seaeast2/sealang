#include "astcontext.h"
#include <iostream>
#include "ast_printer.h"

using namespace std;

namespace AST {
  ASTContext::ASTContext() {
    imported_decls_ = nullptr;
    decls_ = nullptr;
  }

  ASTContext::~ASTContext() {
    if (imported_decls_)
      delete imported_decls_;
    if (decls_)
      delete decls_;
  }

  // Type environment control
  bool ASTContext::AddType(Type* ty) {
    return global_env_.AddType(ty);
  }

  bool ASTContext::RemoveType(const char* type_name) {
    return global_env_.RemoveType(type_name);
  }

  Type* ASTContext::GetType(const char* type_name) {
    return global_env_.GetType(type_name);
  }

  void ASTContext::PrintAST() {
    ASTPrinter ast_printer;

    ast_printer.Print(GetLocalDecl());
    ast_printer.Print(GetImportedDecl());
  }

};
