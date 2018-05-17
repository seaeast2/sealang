#include "ast_type.h"
#include <iostream>
#include <string>

using namespace std;

namespace AST {

  void Type::Print() {
    cout << "Base Type" << endl;
  }

  VoidType* VoidType::Get(ASTContext* ac) {
    return ac->GetVoidTy();
  }

  CharType* CharType::Get(ASTContext* ac) {
    return ac->GetCharTy();
  }

}

