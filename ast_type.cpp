#include <iostream>
#include <string>
#include "ast_type.h"
#include "astcontext.h"

using namespace std;

namespace AST {

  void Type::Print() {
    cout << "Base Type" << endl;
  }

  const VoidType* VoidType::Get(ASTContext* ac) {
    return ac->GetVoidTy();
  }

  const CharType* CharType::Get(ASTContext* ac) {
    return ac->GetCharTy();
  }

  const ShortType* ShortType::Get(ASTContext* ac) {
    return ac->GetShortTy();
  }

  const IntType* IntType::Get(ASTContext* ac) {
    return ac->GetIntTy();
  }

  const LongType* LongType::Get(ASTContext* ac) {
    return ac->GetLongTy();
  }

  const FloatType* FloatType::Get(ASTContext* ac) {
    return ac->GetFloatTy();
  }

  const DoubleType* DoubleType::Get(ASTContext* ac) {
    return ac->GetDoubleTy();
  }
}

