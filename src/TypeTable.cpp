#include "TypeTable.h"

using namespace AST;


TypeTable::TypeTable() {
}
      
TypeTable::~TypeTable() {
}

bool TypeTable::Add(Type* ty) {
    if (table_.Find(ty->GetTypeName()))
      return false;
    table_.Insert(ty->GetTypeName(), ty);
    return true;
}

bool TypeTable::Remove(const char* typeName) {
    if (!table_.Find(typeName))
      return false;
    return table_.Delete(typeName);
}

Type* TypeTable::Find(const char* typeName) {
  return table_.Find(typeName);
}

void TypeTable::ResetItr() {
  table_.ResetItr();
}

Type* TypeTable::Next() {
  return table_.Next();
}


// Type Error Checker ========================================
bool TypeTable::CheckSemanticError() {
  table_.ResetItr();

  // check recursive record type definition
  while(Type* ty = table_.Next()) {
    if (ty->IsKindOf(RecordTy)) {
      for (int i = 0; i < ty->GetMemberNum(); i++) {
        Type* memTy = ty->GetMemberType(i);
        if (!CheckRecursiveTypeDef(ty, memTy)) {
          assert(0 && "Recursive record type definition found.");
          return false;
        }
      }
    }
  }
  return true;
}

bool TypeTable::CheckRecursiveTypeDef(Type const* originTy, Type* targetTy) {
  // check user type
  if (targetTy->IsKindOf(UserTy)) {
    Type* userBaseTy= ((UserType*)targetTy)->GetBaseType();
    if (*originTy == *userBaseTy) {
      assert(0&&"Error : recursive type defintion found");
      return false;
    }
    return CheckRecursiveTypeDef(originTy, userBaseTy);
  }

  // check array type
  if (targetTy->IsKindOf(ArrayTy)) {
    Type* arrBaseTy= ((ArrayType*)targetTy)->GetBaseType();
    if (*originTy == *arrBaseTy) {
      assert(0&&"Error : recursive type defintion found");
      return false;
    }
    return CheckRecursiveTypeDef(originTy, arrBaseTy);
  }

  // check record type
  if (targetTy->IsKindOf(RecordTy)) {
    if (*originTy == *targetTy) {
      assert(0&&"Error : recursive type defintion found");
      return false;
    }
    // step into member type
    for (int i = 0; i < targetTy->GetMemberNum(); i++) {
      Type* memTy = targetTy->GetMemberType(i);
      return CheckRecursiveTypeDef(originTy, memTy);
    }
  }

  return true;
}
