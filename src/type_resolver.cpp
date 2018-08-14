#include <assert.h>
#include "type_resolver.h"

using namespace AST;

TypeResolver::TypeResolver() {
}

TypeResolver::~TypeResolver() {
}

bool TypeResolver::Check(ASTContext* ac) {
  ac_ = ac;

  // 1.convert function decl to function type.
  FunctionDecl* fd = nullptr;
  for (int i = 0; i < decls_->GetFunctionNum(); i++) {
    fd = decls_->GetFunction(i);
  }

  // 2. convert classdecl to classtype.
  ClassNode* cn = nullptr;
  for (int i = 0; i < decls_->GetClassNum(); i++) {
    cn = decls_->GetClass(i)
  }

  return true;
}

bool TypeResolver::CreateFunctionType(FunctionDecl* fd) {
  Type* retty = fd->GetReturnType()->GetType();
  Types param_tys;
  for (int i = 0; i < fd->GetParamNum(); i++) {
    param_tys.PushBack(fd->GetParamNode(i)->GetType()->GetType());
  }
  Type* this_class_ty = fd->GetThisClass()->GetType()->GetType();

  FunctionType* ft = FunctionType::Get(ac, retty, param_tys, this_class_ty);
  if (!ft) {
    assert(0 && "Can't create FunctionType");
    return false;
  }
  ft->Incomplete(false); // mark as complete

  return true;
}

/*  FunctionType* FunctionDecl::GetType(ASTContext* ac) {
    // return type
    Type* retty = ret_ty_->GetType();
    Types param_tys;
    for (int i = 0; i < params_.GetSize(); i++) {
      param_tys.PushBack(params_[i]->GetType()->GetType());
    }
    Type* this_classty = this_class_->GetType()->GetType();

    return FunctionType::Get(ac, retty, param_tys, this_classty);
  }*/
