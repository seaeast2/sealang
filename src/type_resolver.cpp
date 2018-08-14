#include <assert.h>
#include "type_resolver.h"

using namespace AST;

TypeResolver::TypeResolver() {
}

TypeResolver::~TypeResolver() {
}

bool TypeResolver::Check(ASTContext* ac) {
  ac_ = ac;

  Declarations* decls = ac_->GetLocalDecl();

  // 1.convert function decl to function type.
  FunctionDecl* fd = nullptr;
  for (int i = 0; i < decls->GetFunctionNum(); i++) {
    fd = decls->GetFunction(i);
    if (!ResolveFunctionType(fd))
      return false;
  }

  // 2. convert classdecl to classtype.
  ClassNode* cn = nullptr;
  for (int i = 0; i < decls->GetClassNum(); i++) {
    cn = decls->GetClass(i);
    if (!ResolveClassType(cn))
      return false;
  }

  return true;
}

bool TypeResolver::ResolveFunctionType(FunctionDecl* fd) {
  FunctionType* ft = ac_->GetFunctionTypeFromDecl(fd);
  if (!ft->IsIncomplete()) {
    assert(0 && "Already defined FunctionType");
    return false;
  }

  ft->Incomplete(false); // mark as complete
  return true;
}


bool TypeResolver::ResolveClassType(ClassNode* cn) {
  // Get class type by class name
  ClassType* ct = ClassType::Get(ac_, cn->GetTypeName());

  if (!ct->IsIncomplete()) {
    assert(0 && "Already defined ClassType.");
    return false;
  }

  // insert class member variable type
  VariableDecl* vd = nullptr;
  for (int i = 0; i < cn->GetMemVarNum(); i++) {
    vd = cn->GetMemVariable(i);
    if (!vd) {
      assert(0 &&"invalid class member variable");
      return false;
    }
    ct->AddMemberType(vd->GetType()->GetType());
  }
  
  // insert class member function type
  FunctionDecl* fd = nullptr;
  for (int i = 0; i < cn->GetMemFunNum(); i++) {
    fd = cn->GetMemFunction(i);
    if (!fd) {
      assert(0 &&"invalid class member function");
      return false;
    }
    ct->AddMemberFuncType(ac_->GetFunctionTypeFromDecl(fd));
  }

  return true;
}
