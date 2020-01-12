#ifndef _ast_visitor_h_
#define _ast_visitor_h_

#include "Common.h"

namespace AST {
  // Iterate AST
  class ASTVisitor {
    public:
      ASTVisitor() {}
      virtual ~ASTVisitor() {}

      bool VisitStmt(StmtNode* nodes);
      bool VisitExpr(ExprNode* node);

      // Statements
      virtual bool Visit(BlockNode* node);
      virtual bool Visit(LabelNode* node);
      virtual bool Visit(ExprStmtNode* node);
      virtual bool Visit(IfNode* node);
      virtual bool Visit(WhileNode* node);
      virtual bool Visit(DoWhileNode* node);
      virtual bool Visit(ForNode* node);
      virtual bool Visit(CaseNode* node);
      virtual bool Visit(SwitchNode* node);
      virtual bool Visit(BreakNode* node);
      virtual bool Visit(ContinueNode* node);
      virtual bool Visit(GotoNode* node);
      virtual bool Visit(ReturnNode* node);

      // Expression
      virtual bool Visit(AssignNode* node);
      virtual bool Visit(OpAssignNode* node);
      virtual bool Visit(AddressNode* node);
      virtual bool Visit(BinaryOpNode* node);
      virtual bool Visit(LogicalAndNode* node);
      virtual bool Visit(LogicalOrNode* node);
      virtual bool Visit(CastNode* node);
      virtual bool Visit(CondExprNode* node);
      virtual bool Visit(FuncCallNode* node);
      virtual bool Visit(ArrayRefNode* node);
      virtual bool Visit(DereferenceNode* node);
      virtual bool Visit(MemberRefNode* node);
      virtual bool Visit(PtrMemberRefNode* node);
      virtual bool Visit(VariableNode* node);
      virtual bool Visit(IntegerLiteralNode* node);
      virtual bool Visit(RealLiteralNode* node);
      virtual bool Visit(StringLiteralNode* node);
      virtual bool Visit(SizeofExprNode* node);
      virtual bool Visit(SizeofTypeNode* node);
      virtual bool Visit(UnaryOpNode* node);
      virtual bool Visit(PrefixOpNode* node);
      virtual bool Visit(SuffixOpNode* node);

      // Etc
      virtual bool Visit(FunctionDecl* node);
      virtual bool Visit(VariableDecl* node);
      virtual bool Visit(ConstantDecl* node);
      virtual bool Visit(TypeNode* node);
      virtual bool Visit(ParamDecl* node);
      virtual bool Visit(ImportNode* node);
      virtual bool Visit(ArgsNode* node);
      //virtual bool Visit(ClassNode* node);
      virtual bool Visit(RecordDecl* node);
      virtual bool Visit(TypedefNode* node);
  };
}

#endif
