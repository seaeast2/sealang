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

  Type* ASTContext::FindType(const char* key) {
    return type_table_.Find(key);
  }

  bool ASTContext::AddType(const char* key, Type* type) {
    if (type_table_.Find(key))
      return false;
    type_table_.Insert(key, type);
    return true;
  }

  bool ASTContext::RemoveType(const char* key) {
    if (!type_table_.Find(key))
      return false;

    return type_table_.Delete(key);
  }

  void ASTContext::PrintAST() {
    ASTPrinter ast_printer;

    ast_printer.Print(GetLocalDecl());
    ast_printer.Print(GetImportedDecl());
  }

};
