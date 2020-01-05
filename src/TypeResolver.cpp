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
  ClassNode* cn = nullptr;
  ClassType* ct = nullptr;

  Declarations* decls = ac->GetLocalDecl();

  for (int i = 0; i < decls->GetClassNum(); i++) {
    cn = decls->GetClass(i);
    ct = (ClassType*)cn->GetType()->GetType();
    if (recursive_type_checker_.Find((unsigned long)ct)) {
      assert(0 && "Error duplicated ClassType.");
      return false;
    }

    recursive_type_checker_.Insert((unsigned long)ct, ct);
    if (!VisitClassType(ct)) {
      assert(0 && "Error Recursive Type definition detected.");
      recursive_type_checker_.Clear();
      return false;
    }
    recursive_type_checker_.Clear();
  }

  return true;
}

bool TypeResolver::VisitClassType(ClassType* ct) {
  Type* cur_ty = nullptr;
  for (int i = 0; i < ct->GetMemberNum(); i++) {
    cur_ty = ct->GetMemberType(i);
    // In case UserType
    if(cur_ty->IsKindOf(Type::UserTy)) {
      Type* org_ty = ((UserType*)cur_ty)->GetOriginalType();
      if (org_ty->IsKindOf(Type::ClassTy)) {
        cur_ty = org_ty;
      }
    }
    // TODO : need to check array type with user type or class type

    if (cur_ty->IsKindOf(Type::ClassTy)) {
      if (recursive_type_checker_.Find((unsigned long)cur_ty)) {
        assert(0 && "Error Recursive Type definition detected.");
        return false;
      }

      recursive_type_checker_.Insert((unsigned long)cur_ty, cur_ty);
      if (!VisitClassType((ClassType*)cur_ty))
        return false;
    }
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


