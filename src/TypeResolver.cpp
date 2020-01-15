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


