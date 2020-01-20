#include "core/hash.h"

#include "TypeChecker.h"

using namespace AST;


TypeChecker::~TypeChecker() {
}

bool TypeChecker::Check(Declarations* decls) {
  // Check global variables
  for (int i = 0; i < decls->GetVariableDeclNum(); i++) {
    Visit(decls->GetVariableDecl(i));
  }

  // Check errors in function decls.
  for (int i = 0; i < decls->GetFunctionDeclNum(); i++) {
    FunctionDecl* FD = decls->GetFunctionDecl(i);
    FD->Accept(this);
  }

  return true;
}


bool TypeChecker::CheckInvalidVariable(VariableDecl* VD) {
  // Check type
  if (!CheckInvalidVariableType(VD->GetTypeNode()->GetType())) {
    assert(0&&"Error : Invalid variable type.");
    return false;  
  }
  return true;
}


//Checkers ==========================================================
bool TypeChecker::CheckInvalidVariableType(Type* t) {
  if (t->GetKind() == Type::VoidTy)
    return false;
  if (t->GetKind() == Type::ArrayTy) {
    if (((ArrayType*)t)->GetBaseType()->GetKind() == Type::VoidTy)
      return false;
  }
  return true;
}

bool TypeChecker::CheckDuplicatedMemberName(RecordDecl* r) {
  // Check variable name.
  HashTable<int, 20> names; // we just need key duplication check.

  for (int i = 0; i < r->GetMemVarDeclNum(); i++) {
    VariableDecl* VD = r->GetMemVariableDecl(i);
    if (names.Find(VD->GetName()) == nullptr) {
      // no matching
      names.Insert(VD->GetName(), i);
    }
    else {
      assert(0&&"Error : Duplicated member variable name.");
      return false;  
    }
  }

  // TODO : need to check member function as well.

  return true;
}

bool TypeChecker::CheckVoidArray() {
  if (!typeTable_)
    return false;

  typeTable_->ResetItr();
  while(Type* ty = typeTable_->Next()) {
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

// Visit Functions ==================================================
bool TypeChecker::Visit(VariableDecl* node) {
  CheckInvalidVariable(node);
  return ASTVisitor::Visit(node);
}


bool TypeChecker::Visit(ParamDecl* node) {
  CheckInvalidVariable(node);
  return ASTVisitor::Visit(node);
}

bool TypeChecker::Visit(RecordDecl* node) {
  CheckDuplicatedMemberName(node);
  return ASTVisitor::Visit(node);
}
