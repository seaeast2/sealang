#include <assert.h>
#include "core/hash_int.h"
#include "TypeResolver.h"

using namespace AST;

TypeResolver::TypeResolver() {
}

TypeResolver::~TypeResolver() {
}

bool TypeResolver::MakeCompleteType(ASTContext* ac) {

  Declarations* decls = ac->GetLocalDecl();

  // 1.convert function decl to function type.
  FunctionDecl* fd = nullptr;
  for (int i = 0; i < decls->GetFunctionDeclNum(); i++) {
    fd = decls->GetFunctionDecl(i);
    if (!CompleteFunctionType(ac, fd))
      return false;
  }

  // 2. convert classdecl to RecordType
  RecordDecl* RD = nullptr;
  for (int i = 0; i < decls->GetRecordDeclNum(); i++) {
    RD = decls->GetRecordDecl(i);
    if (!CompleteRecordType(ac, RD))
      return false;
  }

  return true;
}

bool TypeResolver::CompleteFunctionType(ASTContext* ac, FunctionDecl* FD) {
  // Retrive function type info
  // 1. Get return type
  Type* retTy = FD->GetReturnTypeNode()->GetType();
  
  // 2. Get parameter types
  Types paramTys;
  for (int i = 0; i < FD->GetParamNum(); i++) {
    paramTys.PushBack(FD->GetParamNode(i)->GetTypeNode()->GetType());
  }

  // 3. Get this class type in case member function.
  Type* thisClassType = FD->GetThisClass()->GetTypeNode()->GetType();

  // 4. Create Complete FunctionType
  FunctionType* FT  = FunctionType::Get(ac, retTy, paramTys, thisClassType);
  if (!FT) {
    assert(0 && "Can't create FunctionType");
    return false;
  }

  // 5. Marks as complete type
  FT->Complete(true);

  // 6. Set TypeNode to FunctionDecl.
  if (!FD->GetTypeNode()) {
    FD->SetTypeNode(new TypeNode(FT));
  }
  return true;
}

bool TypeResolver::CompleteRecordType(ASTContext* ac, RecordDecl* RD) {
  // 1. Get record type
  RecordType* RTy = RecordType::Get(ac, RD->GetTypeName());

  if (RTy->IsComplete()) // already completed type
    return true;

  // 2. insert class member variable type
  VariableDecl* vd = nullptr;
  for (int i = 0; i < RD->GetMemVarDeclNum(); i++) {
    vd = RD->GetMemVariableDecl(i);
    if (!vd) {
      assert(0 &&"invalid class member variable");
      return false;
    }
    RTy->AddMemberType(vd->GetTypeNode()->GetType());
  }
  
  // 3. insert class member function type
  FunctionDecl* fd = nullptr;
  for (int i = 0; i < RD->GetMemFunDeclNum(); i++) {
    fd = RD->GetMemFunctionDecl(i);
    if (!fd) {
      assert(0 &&"invalid class member function");
      return false;
    }
    RTy->AddMemberFuncType(static_cast<FunctionType*>(fd->GetTypeNode()->GetType()));
  }

  // 4. Mark complete type
  RTy->Complete(true);

  // 5. Set TypeNode
  RD->GetTypeNode()->SetType(RTy);
  
  return true;
}


