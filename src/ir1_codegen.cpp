#include "ir1_codegen.h"

using namespace AST;

namespace IR1 {
bool CodeGen::GenerateIR(Declarations* decl) {
  for (int i = 0; i < decl->GetVariableNum(); i++) {
    VariableDecl* vd = decl->GetVariable(i);
    Visit(vd);
  }

  for (int i = 0; i < decl->GetVariableNum(); i++) {
    FunctionDecl* fd = decl->GetFunction(i);
    Visit(fd);
  }

  return true;
}

bool CodeGen::VisitStmt(StmtNode* nodes) {
  return true;
}

bool CodeGen::VisitExpr(ExprNode* node) {
  return true;
}

bool CodeGen::Visit(IfNode* node) {
  return true;
}

bool CodeGen::Visit(FunctionDecl* node) {
  return true;
}

bool CodeGen::Visit(VariableDecl* node) {
  return true;
}

}
