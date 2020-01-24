#include "ASTContext.h"
#include <iostream>
#include <assert.h>
#include "ASTPrinter.h"
#include "LocalVarResolver.h"
#include "TypeResolver.h"

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
    return typeTable_.Add(ty);
  }

  bool ASTContext::RemoveType(const char* typeName) {
    return typeTable_.Remove(typeName);
  }

  Type* ASTContext::GetType(const char* type_name) {
    return typeTable_.Find(type_name);
  }

  void ASTContext::ResetTypeItr() {
    typeTable_.ResetItr();
  }

  Type* ASTContext::GetTypeAndNext() {
    return typeTable_.Next();
  }

  bool ASTContext::ResolveLocalVariable() {
    LocalVarResolver LVR;
    return LVR.Resolve(decls_, &topScope_);
  }

  void ASTContext::MakeCompleteType() {
    TypeResolver tr;
    tr.MakeCompleteType(this);
  }

  void ASTContext::PrintAST() {
    ASTPrinter ast_printer;
    ast_printer.Print(GetLocalDecl());
  }

};
