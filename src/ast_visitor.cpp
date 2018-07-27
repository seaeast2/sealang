#include "ast_visitor.h"
#include "ast_node.h"

using namespace AST;


bool ASTVisitor::VisitStmt(StmtNode* node) {
  if (node)
    return node->Accept(this);
  return true;
}

bool ASTVisitor::VisitExpr(ExprNode* node) {
  if (node)
    return node->Accept(this);
  return true;
}

// Statements
bool ASTVisitor::Visit(BlockNode* node){ 
  // variables
  VariableDecl* var = nullptr;
  for (int i = 0; i < node->GetVarSize(); i++) {
    var = node->GetVariable(i);
    if (var->HasInitializer()) {
      if(!VisitExpr(var->GetInitializer()))
        return false;
    }
  }
  // statements
  StmtNode* stmt = nullptr;
  for (int i = 0; i < node->GetStmtSize(); i++) {
    stmt = node->GetStmt(i);
    if(!VisitStmt(stmt))
      return false;
  }
  return true;
}

bool ASTVisitor::Visit(LabelNode* node) { 
  StmtNode* stmt = node->GetStmt();
  return VisitStmt(stmt);
}

bool ASTVisitor::Visit(ExprStmtNode* node) {
  ExprNode* expr = node->GetExpr();
  return VisitExpr(expr);
}

bool ASTVisitor::Visit(IfNode* node) { 
  ExprNode* cond = node->GetCond();
  StmtNode* then_body = node->GetThenBody(), *else_body = node->GetElseBody();

  if(!VisitExpr(cond))
    return false;
  if(!VisitStmt(then_body))
    return false;
  if(!VisitStmt(else_body))
    return false;
  return true;
}

bool ASTVisitor::Visit(WhileNode* node) { 
  ExprNode* cond = node->GetCond();
  StmtNode* body = node->GetBody();
  if(!VisitExpr(cond))
    return false;
  if(!VisitStmt(body))
    return false;
  return true; 
}

bool ASTVisitor::Visit(DoWhileNode* node){ 
  ExprNode* cond = node->GetCond();
  StmtNode* body = node->GetBody();
  if(!VisitExpr(cond))
    return false;
  if(!VisitStmt(body))
    return false;
  return true; 
}

bool ASTVisitor::Visit(ForNode* node){ 
  ExprNode* init = node->GetInit();
  ExprNode* cond = node->GetCond();
  ExprNode* inc = node->GetInc();
  StmtNode* body = node->GetBody();

  if(!VisitExpr(init))
    return false;
  if(!VisitExpr(cond))
    return false;
  if(!VisitExpr(inc))
    return false;
  if(!VisitStmt(body))
    return false;
  return true; 
}

bool ASTVisitor::Visit(CaseNode* node){ 
  ExprNode* val = node->GetCaseValue();
  StmtNode* body = node->GetBody();

  if(!VisitExpr(val))
    return false;
  if(!VisitStmt(body))
    return false;
  return true; 
}

bool ASTVisitor::Visit(SwitchNode* node){ 
  ExprNode* cond = node->GetCond();
  
  if (!VisitExpr(cond))
    return false;

  for (int i = 0; i < node->GetCaseNum(); i++) {
    if(!Visit(node->GetCase(i)))
      return false;
  }
  return true; 
}

bool ASTVisitor::Visit(BreakNode* node){ return true; }
bool ASTVisitor::Visit(ContinueNode* node){ return true; }
bool ASTVisitor::Visit(GotoNode* node){ return true; }
bool ASTVisitor::Visit(ReturnNode* node) { 
  ExprNode* expr = node->GetExpr();
  if(!VisitExpr(expr))
    return false;
  return true; 
}

// Expression
bool ASTVisitor::Visit(AssignNode* node){ 
  ExprNode* lhs = node->GetLHS();
  ExprNode* rhs = node->GetRHS();

  if(!VisitExpr(lhs))
    return false;
  if(!VisitExpr(rhs))
    return false;
  return true; 
}

bool ASTVisitor::Visit(OpAssignNode* node) { 
  ExprNode* lhs = node->GetLHS();
  ExprNode* rhs = node->GetRHS();

  if(!VisitExpr(lhs))
    return false;
  if(!VisitExpr(rhs))
    return false;
  return true; 
}

bool ASTVisitor::Visit(AddressNode* node){ 
  ExprNode* expr = node->GetExpr();
  if(!VisitExpr(expr))
    return false;
  return true; 
}

bool ASTVisitor::Visit(BinaryOpNode* node){ 
  ExprNode* left = node->GetLeft();
  ExprNode* right = node->GetRight();

  if(!VisitExpr(left))
    return false;
  if(!VisitExpr(right))
    return false;
  return true; 
}

bool ASTVisitor::Visit(LogicalAndNode* node){ 
  ExprNode* left = node->GetLeft();
  ExprNode* right = node->GetRight();

  if(!VisitExpr(left))
    return false;
  if(!VisitExpr(right))
    return false;
  return true; 
}

bool ASTVisitor::Visit(LogicalOrNode* node){ 
  ExprNode* left = node->GetLeft();
  ExprNode* right = node->GetRight();

  if(!VisitExpr(left))
    return false;
  if(!VisitExpr(right))
    return false;
  return true; 
}

bool ASTVisitor::Visit(CastNode* node){ 
  TypeNode* ty = node->GetCastType();
  ExprNode* term = node->GetTermExpr();
  if(!Visit(ty))
    return false;
  if(!VisitExpr(term))
    return false;
  return true; 
}

bool ASTVisitor::Visit(CondExprNode* node){ 
  ExprNode* cond = node->GetCond();
  ExprNode* then = node->GetThen();
  ExprNode* Else = node->GetElse();
  if (!VisitExpr(cond))
    return false;
  if (!VisitExpr(then))
    return false;
  if (!VisitExpr(Else))
    return false;
  return true; 
}

bool ASTVisitor::Visit(FuncCallNode* node){ 
  ExprNode* fexpr = node->GetFuncExpr();
  ArgsNode* args = node->GetArgs();

  if (!VisitExpr(fexpr))
    return false;
  if (!Visit(args))
    return false;
  return true; 
}

bool ASTVisitor::Visit(ArrayRefNode* node){ 
  ExprNode* arr_base = node->GetArrayBaseExpr();
  ExprNode* arr_size = node->GetArraySizeExpr();
  if (!VisitExpr(arr_base))
    return false;
  if (!VisitExpr(arr_size))
    return false;
  return true; 
}

bool ASTVisitor::Visit(DereferenceNode* node){ 
  ExprNode* base_expr = node->GetBaseExpr();
  if (!VisitExpr(base_expr))
    return false;
  return true; 
}

bool ASTVisitor::Visit(MemberRefNode* node){ 
  ExprNode* base_expr = node->GetBaseExpr();
  if (!VisitExpr(base_expr))
    return false;
  return true; 
}

bool ASTVisitor::Visit(PtrMemberRefNode* node){ return true; }
bool ASTVisitor::Visit(VariableNode* node){ return true; }
bool ASTVisitor::Visit(IntegerLiteralNode* node){ return true; }
bool ASTVisitor::Visit(RealLiteralNode* node){ return true; }
bool ASTVisitor::Visit(StringLiteralNode* node){ return true; }
bool ASTVisitor::Visit(SizeofExprNode* node){ return true; }
bool ASTVisitor::Visit(SizeofTypeNode* node){ return true; }
bool ASTVisitor::Visit(UnaryOpNode* node){ return true; }
bool ASTVisitor::Visit(PrefixOpNode* node){ return true; }
bool ASTVisitor::Visit(SuffixOpNode* node){ return true; }

// Etc
bool ASTVisitor::Visit(FunctionDecl* node){ return true; }
bool ASTVisitor::Visit(VariableDecl* node){ return true; }
bool ASTVisitor::Visit(ConstantDecl* node){ return true; }
bool ASTVisitor::Visit(TypeNode* node){ return true; }
bool ASTVisitor::Visit(ParamNode* node){ return true; }
bool ASTVisitor::Visit(ImportNode* node){ return true; }
bool ASTVisitor::Visit(ArgsNode* node){ return true; }
bool ASTVisitor::Visit(ClassNode* node){ return true; }
bool ASTVisitor::Visit(TypedefNode* node){ return true; }
