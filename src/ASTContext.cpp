#include "astcontext.h"
#include <iostream>
#include <assert.h>
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
    type_table_.ResetItr();
  }

  Type* ASTContext::GetTypeAndNext() {
    return type_table_.Next();
  }

  void ASTContext::CheckLocalVar() {
    LocalVarChecker lvc;
    lvc.Check(decls_, &top_scope_);
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
