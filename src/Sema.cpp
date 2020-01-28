#include "Sema.h"

using namespace AST;

Sema::Sema() {
}

Sema::~Sema() {
}

bool Sema::Check(Declarations* decls) {
  // ConstantDecl
  for (int i = 0; i < decls->GetConstantDeclNum(); i++) {
    ConstantDecl* constDecl = decls->GetConstantDecl(i);
    ASTVisitor::Visit(constDecl);
  }
  // VariableDecl
  for (int i = 0; i < decls->GetVariableDeclNum(); i++) {
    VariableDecl* varDecl = decls->GetVariableDecl(i);
    ASTVisitor::Visit(varDecl);
  }
  // RecordDecl
  for (int i = 0; i < decls->GetRecordDeclNum(); i++) {
    RecordDecl* recDecl = decls->GetRecordDecl(i);
    ASTVisitor::Visit(recDecl);
  }
  // FunctionDecl
  for (int i = 0; i < decls->GetFunctionDeclNum(); i++) {
    FunctionDecl* funDecl = decls->GetFunctionDecl(i);
    ASTVisitor::Visit(funDecl);
  }
  
  return true;
}

bool Sema::Visit(AssignNode* node) {
  ExprNode* lhs = node->GetLHS();
  if (!CheckAssignment(lhs)) {
    assert(0 && "Error : wrong lvalue");
    return false;
  }

  return ASTVisitor::Visit(node);
}

bool Sema::Visit(OpAssignNode* node) {
  ExprNode* lhs = node->GetLHS();
  if (!CheckAssignment(lhs)) {
    assert(0 && "Error : wrong lvalue");
    return false;
  }

  return ASTVisitor::Visit(node);
}

bool Sema::Visit(FuncCallNode* node) {
  if (!CheckInvalidFunCall(node)) {
    assert(0 && "Error : wrong function name");
    return false;
  }
  return ASTVisitor::Visit(node);
}

bool Sema::Visit(ArrayRefNode* node) {
  // DOING : working here
  return true;
}

// Semantic error check functions =======================================
bool Sema::CheckAssignment(const ExprNode* EN) {
  return EN->IsAssignable();
}
      
bool Sema::CheckInvalidFunCall(const FuncCallNode* FN) {
  const ExprNode* EN = FN->GetFuncExpr();
  if (EN->IsKindOf(BaseNode::VariableNodeTy))
    return true;
  return false;
}


bool Sema::CheckInvalidArrRef(const ArrayRefNode* arrNode) {

  ExprNode* arrBase = arrNode->GetArrayBaseExpr();

  return true;
}

