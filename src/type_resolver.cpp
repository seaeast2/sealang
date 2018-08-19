#include <assert.h>
#include "core/hash_int.h"
#include "type_resolver.h"

using namespace AST;

TypeResolver::TypeResolver() {
}

TypeResolver::~TypeResolver() {
}

bool TypeResolver::MakeCompleteType(ASTContext* ac) {

  Declarations* decls = ac->GetLocalDecl();

  // 1.convert function decl to function type.
  FunctionDecl* fd = nullptr;
  for (int i = 0; i < decls->GetFunctionNum(); i++) {
    fd = decls->GetFunction(i);
    if (!CompleteFunctionType(ac, fd))
      return false;
  }

  // 2. convert classdecl to classtype.
  ClassNode* cn = nullptr;
  for (int i = 0; i < decls->GetClassNum(); i++) {
    cn = decls->GetClass(i);
    if (!CompleteClassType(ac, cn))
      return false;
  }

  return true;
}


bool TypeResolver::CheckVoidArray(ASTContext* ac) {
  Type* ty = nullptr; 

  ac->ResetTypeItr();
  while(ty = ac->GetTypeAndNext()) {
    if(ty->IsKindOf(Type::ArrayTy)) {
      Type* basety = ((ArrayType*)ty)->GetBaseType();
      if (basety && basety->IsKindOf(Type::VoidTy)) {
        assert(0 && "Error : void type can't be a base type of array.");
        return false;
      }
    }
  }

  return true;
}

bool TypeResolver::CheckRecursiveTypeDef(ASTContext* ac) {
  // TODO : working here
  HashInt<unsigned long, Type*, 64> RecursiveTypeDefChecker;

  ClassNode* cn = nullptr;
  ClassType* ct = nullptr;
  for (int i = 0; i < decls->GetClassNum(); i++) {
    cn = decls->GetClass(i);
    ct = cn->GetType();
  }

  return true;
}

bool TypeResolver::CompleteFunctionType(ASTContext* ac, FunctionDecl* fd) {
  FunctionType* ft = ac->GetFunctionTypeFromDecl(fd);
  if (!ft->IsIncomplete()) {
    assert(0 && "Already defined FunctionType");
    return false;
  }

  ft->Incomplete(false); // mark as complete
  return true;
}

bool TypeResolver::VisitClassType(ClassType* ct) {
  return true;
}


bool TypeResolver::CompleteClassType(ASTContext* ac, ClassNode* cn) {
  // Get class type by class name
  ClassType* ct = ClassType::Get(ac, cn->GetTypeName());

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
    ct->AddMemberFuncType(ac->GetFunctionTypeFromDecl(fd));
  }
  return true;
}


