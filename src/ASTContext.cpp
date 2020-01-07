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
    if (typeTable_.Find(ty->GetTypeName()))
      return false;
    typeTable_.Insert(ty->GetTypeName(), ty);
    return true;
  }

  bool ASTContext::RemoveType(const char* type_name) {
    if (!typeTable_.Find(type_name))
      return false;
    return typeTable_.Delete(type_name);
  }

  Type* ASTContext::GetType(const char* type_name) {
    return typeTable_.Find(type_name);
  }

  FunctionType* ASTContext::GetFunctionTypeFromDecl(FunctionDecl* fd) {
    Type* retty = fd->GetReturnType()->GetType();
    Types param_tys;
    for (int i = 0; i < fd->GetParamNum(); i++) {
      param_tys.PushBack(fd->GetParamNode(i)->GetType()->GetType());
    }
    Type* this_class_ty = fd->GetThisClass()->GetType()->GetType();

    FunctionType* ft = FunctionType::Get(this, retty, param_tys, this_class_ty);
    if (!ft) {
      assert(0 && "Can't create FunctionType");
      return nullptr;
    }

    return ft;
  }

  void ASTContext::ResetTypeItr() {
    typeTable_.ResetItr();
  }

  Type* ASTContext::GetTypeAndNext() {
    return typeTable_.Next();
  }

  void ASTContext::ResolveLocalVariable() {
    LocalVarResolver lvr;
    lvr.Resolve(decls_, &top_scope_);
  }

  void ASTContext::MakeCompleteType() {
    TypeResolver tr;
    tr.MakeCompleteType(this);
  }

  void ASTContext::CheckVoidArray() {
    TypeResolver tr;
    tr.CheckVoidArray(this);
  }

  void ASTContext::CheckRecursiveTypeDef() {
    TypeResolver tr;
    tr.CheckRecursiveTypeDef(this);
  }

  void ASTContext::PrintAST() {
    ASTPrinter ast_printer;
    ast_printer.Print(GetLocalDecl());
  }

};
