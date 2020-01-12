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
  RecordDecl* RD = nullptr;
  RecordType* ct = nullptr;

  Declarations* decls = ac->GetLocalDecl();

  for (int i = 0; i < decls->GetRecordDeclNum(); i++) {
    RD = decls->GetRecordDecl(i);
    ct = (RecordType*)RD->GetTypeNode()->GetType();
    if (recursive_type_checker_.Find((unsigned long)ct)) {
      assert(0 && "Error duplicated RecordType.");
      return false;
    }

    recursive_type_checker_.Insert((unsigned long)ct, ct);
    if (!VisitRecordType(ct)) {
      assert(0 && "Error Recursive Type definition detected.");
      recursive_type_checker_.Clear();
      return false;
    }
    recursive_type_checker_.Clear();
  }

  return true;
}

bool TypeResolver::VisitRecordType(RecordType* ct) {
  Type* cur_ty = nullptr;
  for (int i = 0; i < ct->GetMemberNum(); i++) {
    cur_ty = ct->GetMemberType(i);
    // In case UserType
    if(cur_ty->IsKindOf(Type::UserTy)) {
      Type* org_ty = ((UserType*)cur_ty)->GetOriginalType();
      if (org_ty->IsKindOf(Type::RecordTy)) {
        cur_ty = org_ty;
      }
    }
    // TODO : need to check array type with user type or class type

    if (cur_ty->IsKindOf(Type::RecordTy)) {
      if (recursive_type_checker_.Find((unsigned long)cur_ty)) {
        assert(0 && "Error Recursive Type definition detected.");
        return false;
      }

      recursive_type_checker_.Insert((unsigned long)cur_ty, cur_ty);
      if (!VisitRecordType((RecordType*)cur_ty))
        return false;
    }
  }
  return true;
}

bool TypeResolver::CompleteFunctionType(ASTContext* ac, FunctionDecl* FD) {
  // Retrive function type info
  // Get return type
  Type* retTy = FD->GetReturnTypeNode()->GetType();
  
  // Get parameter types
  Types paramTys;
  for (int i = 0; i < FD->GetParamNum(); i++) {
    paramTys.PushBack(FD->GetParamNode(i)->GetType()->GetType());
  }

  // Get this class type in case member function.
  Type* thisClassType = FD->GetThisClass()->GetTypeNode()->GetType();

  // Get Complete FunctionType
  FunctionType* FT  = FunctionType::Get(ac, retTy, paramTys, thisClassType);
  if (!FT) {
    assert(0 && "Can't create FunctionType");
    return false;
  }

  if (!FT->IsIncomplete()) {
    assert(0 && "Already defined FunctionType");
    return false;
  }

  FT->Incomplete(false); // mark as complete
  return true;
}

bool TypeResolver::CompleteRecordType(ASTContext* ac, RecordDecl* cn) {
  // Get class type by class name
  RecordType* ct = RecordType::Get(ac, cn->GetTypeName());

  if (!ct->IsIncomplete()) {
    assert(0 && "Already defined RecordType.");
    return false;
  }

  // insert class member variable type
  VariableDecl* vd = nullptr;
  for (int i = 0; i < cn->GetMemVarDeclNum(); i++) {
    vd = cn->GetMemVariableDecl(i);
    if (!vd) {
      assert(0 &&"invalid class member variable");
      return false;
    }
    ct->AddMemberType(vd->GetType()->GetType());
  }
  
  // insert class member function type
  FunctionDecl* fd = nullptr;
  for (int i = 0; i < cn->GetMemFunDeclNum(); i++) {
    fd = cn->GetMemFunctionDecl(i);
    if (!fd) {
      assert(0 &&"invalid class member function");
      return false;
    }
    ct->AddMemberFuncType(fd->GetTypeNode()->GetType());
  }
  return true;
}


