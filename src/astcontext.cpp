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

  Type* ASTContext::FindType(const char* type_name) {
    return type_env_.GetValue(type_name);
  }

  bool ASTContext::AddType(const char* key, Type* type) {
    return type_env_.Push(key, type);
  }

  Type* ASTContext::RemoveType(const char* key) {
    Type* ty = type_env_.GetValue(key);
    type_env_.Pop(key);
    return ty;
  }

  void ASTContext::PrintAST() {
    ASTPrinter ast_printer;

    ast_printer.Print(GetLocalDecl());
    ast_printer.Print(GetImportedDecl());
  }

};
