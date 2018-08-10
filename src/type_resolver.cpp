#include <assert.h>
#include "type_resolver.h"

using namespace AST;

TypeResolver::TypeResolver() {
}

TypeResolver::~TypeResolver() {
}

bool TypeResolver::Check(ASTContext* ac) {
  ac_ = ac;

  /*FunctionDecl* fd = nullptr;
  ClassNode* cn = nullptr;

  for (int i = 0; i < decls_->GetFunctionNum(); i++) {
    fd = decls_->GetFunction(i);
  }

  for (int i = 0; i < decls_->GetClassNum(); i++) {
    cn = decls_->GetClass(i)
  }*/

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
